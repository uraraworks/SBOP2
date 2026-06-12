/**
 * views/map-object-place.js
 * マップオブジェクト配置画面 (route: map-objects)
 * F4: RPGツクール風グリッドペイントエディタ + 配置テーブル + テンプレート管理
 *
 * データモデル:
 *   - マップ選択 → /api/maps/objects でタイル + オブジェクト一覧を取得
 *   - テンプレート → /api/maps/objects/templates で管理
 *   - 配置 CRUD → /api/maps/placements (POST/PUT/DELETE)
 *
 * 機能:
 *   - タイル背景を描画したグリッドキャンバス
 *   - オブジェクトをオーバーレイ表示 (●マーカー)
 *   - 左クリック: 新規配置または既存オブジェクト選択
 *   - 右クリック: スポイト (tiles から partsId 取得)
 *   - 配置フォーム: objectId, x, y → POST/PUT
 *   - 削除ボタン
 *   - テンプレート管理: 一覧/新規/更新/削除
 *   - iframe postMessage 連携 (sbop2_admin_pick を維持)
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const MAP_OBJECT_TYPE_OPTIONS = [
  { value: "collision",    label: "当たり判定あり" },
  { value: "no-collision", label: "当たり判定なし" },
];

// ----------------------------------------------------------------
// データ正規化 (app.js から移植)
// ----------------------------------------------------------------

function formatHex(value) {
  if (value === null || value === undefined) return null;
  const numeric = Number(value);
  if (!Number.isFinite(numeric)) return null;
  return `0x${(numeric >>> 0).toString(16).toUpperCase().padStart(8, "0")}`;
}

function normalizeMapObject(rawObject, mapId, index) {
  const placementId = typeof rawObject?.placementId === "number" ? rawObject.placementId : null;
  const objectId    = typeof rawObject?.objectId === "number"    ? rawObject.objectId    : null;
  const x = Number(rawObject?.x ?? 0);
  const y = Number(rawObject?.y ?? 0);
  const hasCollision = Boolean(rawObject?.hasCollision);
  const attribute    = typeof rawObject?.attribute === "number" ? rawObject.attribute : null;
  const attributeHex = typeof rawObject?.attributeHex === "string" && rawObject.attributeHex
    ? rawObject.attributeHex : formatHex(attribute);
  const objectIdHex  = typeof rawObject?.objectIdHex === "string" && rawObject.objectIdHex
    ? rawObject.objectIdHex : formatHex(objectId);
  const sizeWidth  = Number(rawObject?.size?.width);
  const sizeHeight = Number(rawObject?.size?.height);
  const hasSize = Number.isFinite(sizeWidth) && Number.isFinite(sizeHeight);
  const size = hasSize ? { width: sizeWidth, height: sizeHeight } : null;
  const baseName = typeof rawObject?.name === "string" && rawObject.name.length
    ? rawObject.name
    : objectId !== null ? `オブジェクトID ${objectId}` : `配置${index + 1}`;
  const uniqueId = placementId !== null
    ? `placement-${placementId}`
    : `map${mapId}-obj${objectId !== null ? objectId : `idx${index}`}-${x}-${y}`;

  return {
    id: uniqueId,
    placementId,
    objectId,
    objectIdHex,
    name: baseName,
    type: hasCollision ? "collision" : "no-collision",
    attributeHex: attributeHex || "-",
    attribute,
    hasCollision,
    x: Number.isFinite(x) ? x : 0,
    y: Number.isFinite(y) ? y : 0,
    size,
  };
}

function normalizeMapEntry(raw) {
  const id = String(raw?.id ?? "");
  const width  = Number(raw?.width ?? 0);
  const height = Number(raw?.height ?? 0);
  const objects = Array.isArray(raw?.objects)
    ? raw.objects.map((item, i) => normalizeMapObject(item, id, i))
    : [];
  const tiles        = Array.isArray(raw?.tiles)            ? raw.tiles            : [];
  const tilesBase    = Array.isArray(raw?.tilesBase)        ? raw.tilesBase        : tiles;
  const tilesPile    = Array.isArray(raw?.tilesPile)        ? raw.tilesPile        : [];

  return {
    id,
    name: typeof raw?.name === "string" && raw.name.length ? raw.name : (id ? `マップID ${id}` : "（名称未設定）"),
    width,
    height,
    objects,
    tiles,
    tilesBase: tilesBase.map(Number),
    tilesPile: tilesPile.map(Number),
  };
}

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function setFeedback(el, msg, type) {
  if (!el) return;
  el.textContent = msg || "";
  el.className = "result-message";
  if (type) el.classList.add(type);
}

function resolveSheetUrl(baseUrl, sheetIndex) {
  const base = baseUrl.endsWith("/") ? baseUrl.slice(0, -1) : baseUrl;
  return `${base}/${sheetIndex}.png`;
}

function grpIdToCoord(grpId, sheetTileWidth) {
  if (!grpId) return null;
  const sheet = Math.floor(grpId / 1024);
  const tile  = grpId % 1024;
  return { sheet, tileX: tile % sheetTileWidth, tileY: Math.floor(tile / sheetTileWidth) };
}

// ----------------------------------------------------------------
// グリッドキャンバス（オブジェクト配置専用）
// ----------------------------------------------------------------

function createObjectGridCanvas({ sheetBaseUrl, tileSize, sheetTileWidth, sheetTileHeight, onCellClick }) {
  const ZOOM = 2;
  let map = null;
  let selectedObjectId = null;
  let previewCell = null;  // { x, y }

  const imgCache = new Map();
  function getImg(url) {
    if (imgCache.has(url)) return imgCache.get(url);
    const img = new Image();
    img.src = url;
    img.onload = () => requestDraw();
    imgCache.set(url, img);
    return img;
  }

  const wrap = document.createElement("div");
  wrap.className = "mp-obj-grid-wrap";
  const canvas = document.createElement("canvas");
  canvas.className = "mp-obj-grid-canvas";
  wrap.appendChild(canvas);
  const ctx = canvas.getContext("2d");

  let rafPending = false;
  function requestDraw() {
    if (!rafPending) {
      rafPending = true;
      requestAnimationFrame(() => { rafPending = false; draw(); });
    }
  }

  function cellPx() { return tileSize * ZOOM; }

  function draw() {
    if (!map) { ctx.clearRect(0, 0, canvas.width, canvas.height); return; }
    const cp = cellPx();
    canvas.width  = map.width  * cp;
    canvas.height = map.height * cp;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.imageSmoothingEnabled = false;

    // タイル背景
    for (const [layerTiles] of [[map.tilesBase], [map.tilesPile]]) {
      for (let row = 0; row < map.height; row++) {
        for (let col = 0; col < map.width; col++) {
          const grpId = Number(layerTiles[row * map.width + col] || 0);
          if (!grpId) continue;
          const coord = grpIdToCoord(grpId, sheetTileWidth);
          if (!coord) continue;
          const img = getImg(resolveSheetUrl(sheetBaseUrl, coord.sheet));
          if (img && img.complete && img.naturalWidth) {
            ctx.drawImage(img, coord.tileX * tileSize, coord.tileY * tileSize, tileSize, tileSize,
              col * cp, row * cp, cp, cp);
          }
        }
      }
    }

    // グリッド線
    ctx.strokeStyle = "rgba(0,0,0,0.15)";
    ctx.lineWidth = 1;
    for (let col = 0; col <= map.width; col++) {
      ctx.beginPath(); ctx.moveTo(col * cp, 0); ctx.lineTo(col * cp, canvas.height); ctx.stroke();
    }
    for (let row = 0; row <= map.height; row++) {
      ctx.beginPath(); ctx.moveTo(0, row * cp); ctx.lineTo(canvas.width, row * cp); ctx.stroke();
    }

    // オブジェクト マーカー
    if (!map.objects) return;
    const occupantMap = new Map();
    map.objects.forEach((obj) => occupantMap.set(`${obj.x},${obj.y}`, obj));

    for (let row = 0; row < map.height; row++) {
      for (let col = 0; col < map.width; col++) {
        const isSelected = previewCell && previewCell.x === col && previewCell.y === row;
        const obj = occupantMap.get(`${col},${row}`);
        if (isSelected) {
          ctx.fillStyle = "rgba(255,200,0,0.45)";
          ctx.fillRect(col * cp, row * cp, cp, cp);
        }
        if (obj) {
          const isObjSelected = obj.id === selectedObjectId;
          ctx.fillStyle = isObjSelected ? "rgba(255,80,0,0.75)" : "rgba(0,120,255,0.55)";
          ctx.fillRect(col * cp + 2, row * cp + 2, cp - 4, cp - 4);
          ctx.fillStyle = "#fff";
          ctx.font = `bold ${Math.max(8, cp / 2.5)}px sans-serif`;
          ctx.textAlign = "center";
          ctx.textBaseline = "middle";
          ctx.fillText("●", col * cp + cp / 2, row * cp + cp / 2);
        }
      }
    }
    ctx.textAlign = "start";
    ctx.textBaseline = "alphabetic";
  }

  canvas.addEventListener("contextmenu", (ev) => ev.preventDefault());
  canvas.addEventListener("click", (ev) => {
    if (!map) return;
    const rect = canvas.getBoundingClientRect();
    const cp = cellPx();
    const cx = Math.floor((ev.clientX - rect.left) / cp);
    const cy = Math.floor((ev.clientY - rect.top) / cp);
    if (cx < 0 || cx >= map.width || cy < 0 || cy >= map.height) return;
    const occupantMap = new Map();
    map.objects.forEach((obj) => occupantMap.set(`${obj.x},${obj.y}`, obj));
    const occupant = occupantMap.get(`${cx},${cy}`) || null;
    if (onCellClick) onCellClick(cx, cy, occupant);
  });

  function setMap(newMap) {
    map = newMap;
    selectedObjectId = null;
    previewCell = null;
    requestDraw();
  }

  function setSelection(objId, cell) {
    selectedObjectId = objId;
    previewCell = cell;
    requestDraw();
  }

  return { el: wrap, setMap, setSelection };
}

// ----------------------------------------------------------------
// テンプレート管理パネル
// ----------------------------------------------------------------

function createTemplatePanel({ onTemplatesChange }) {
  let templates = [];
  let selectedId = null;

  const root = document.createElement("section");
  root.className = "card";
  const h3 = document.createElement("h3");
  h3.textContent = "オブジェクトテンプレート管理";
  root.appendChild(h3);

  const summaryEl = document.createElement("p");
  summaryEl.className = "muted";
  root.appendChild(summaryEl);

  // テーブル
  const tableWrap = document.createElement("div");
  tableWrap.className = "table-wrapper";
  const table = document.createElement("table");
  table.className = "data-table";
  const thead = document.createElement("thead");
  thead.innerHTML = "<tr><th>ID</th><th>名称</th><th>属性</th><th>当たり</th><th>サイズ</th><th>hideY</th></tr>";
  const tbody = document.createElement("tbody");
  table.appendChild(thead);
  table.appendChild(tbody);
  tableWrap.appendChild(table);
  root.appendChild(tableWrap);

  // フォーム
  const form = document.createElement("form");
  form.className = "map-object-form";
  form.noValidate = true;
  form.innerHTML = `
    <div class="form-grid compact">
      <label class="form-field"><span>オブジェクトID <small>(自動)</small></span>
        <input type="text" name="objectId" readonly /></label>
      <label class="form-field"><span>名称</span>
        <input type="text" name="name" required /></label>
      <label class="form-field"><span>属性</span>
        <input type="number" name="attribute" value="0" /></label>
      <label class="form-field"><span>hideY</span>
        <input type="number" name="hideY" value="0" /></label>
      <label class="form-field"><span>幅</span>
        <input type="number" name="width" value="1" min="1" /></label>
      <label class="form-field"><span>高さ</span>
        <input type="number" name="height" value="1" min="1" /></label>
      <label class="form-field"><span>当たり判定</span>
        <input type="checkbox" name="hasCollision" /></label>
    </div>
    <div class="form-actions">
      <button type="submit" class="button primary">保存</button>
      <button type="button" class="button secondary" id="tmpl-new-btn">新規</button>
      <button type="button" class="button danger"    id="tmpl-del-btn" disabled>削除</button>
    </div>
  `;
  const feedbackEl = document.createElement("div");
  feedbackEl.className = "result-message";
  feedbackEl.setAttribute("role", "status");
  root.appendChild(form);
  root.appendChild(feedbackEl);

  const newBtn = form.querySelector("#tmpl-new-btn");
  const delBtn = form.querySelector("#tmpl-del-btn");

  function renderTable() {
    tbody.innerHTML = "";
    if (!templates.length) {
      const tr = document.createElement("tr");
      const td = document.createElement("td");
      td.colSpan = 6;
      td.textContent = "テンプレートがありません";
      tr.appendChild(td);
      tbody.appendChild(tr);
      summaryEl.textContent = "テンプレートなし";
      return;
    }
    summaryEl.textContent = `テンプレート数: ${templates.length}`;
    templates.forEach((tmpl) => {
      const tr = document.createElement("tr");
      if (tmpl.objectId === selectedId) tr.classList.add("is-selected-row");
      tr.style.cursor = "pointer";
      [tmpl.objectId, tmpl.name || "(無名)", tmpl.attribute, tmpl.hasCollision ? "あり" : "なし",
        `${tmpl.width}×${tmpl.height}`, tmpl.hideY].forEach((text) => {
        const td = document.createElement("td");
        td.textContent = String(text ?? "");
        tr.appendChild(td);
      });
      tr.addEventListener("click", () => selectTemplate(tmpl.objectId));
      tbody.appendChild(tr);
    });
  }

  function selectTemplate(objectId) {
    const tmpl = templates.find((t) => t.objectId === objectId);
    selectedId = tmpl ? tmpl.objectId : null;
    if (tmpl) {
      form.objectId.value      = tmpl.objectId;
      form.name.value          = tmpl.name || "";
      form.attribute.value     = tmpl.attribute || 0;
      form.hideY.value         = tmpl.hideY || 0;
      form.width.value         = tmpl.width || 1;
      form.height.value        = tmpl.height || 1;
      form.hasCollision.checked = Boolean(tmpl.hasCollision);
      delBtn.disabled = false;
    } else {
      clearForm();
    }
    renderTable();
  }

  function clearForm() {
    form.objectId.value = "";
    form.name.value     = "";
    form.attribute.value = 0;
    form.hideY.value    = 0;
    form.width.value    = 1;
    form.height.value   = 1;
    form.hasCollision.checked = false;
    delBtn.disabled = true;
  }

  async function loadTemplates() {
    const { response, data } = await fetchJson("/api/maps/objects/templates");
    if (response.ok && Array.isArray(data?.templates)) {
      templates = data.templates;
    } else {
      templates = [];
    }
    renderTable();
    if (onTemplatesChange) onTemplatesChange(templates);
  }

  newBtn.addEventListener("click", () => {
    selectedId = null;
    clearForm();
    renderTable();
    setFeedback(feedbackEl, "新規追加モード: 情報を入力して「保存」", null);
  });

  form.addEventListener("submit", async (ev) => {
    ev.preventDefault();
    const name = form.name.value.trim();
    if (!name) { setFeedback(feedbackEl, "名称を入力してください", "error"); return; }
    const objectId = Number(form.objectId.value) || 0;
    const isUpdate = objectId > 0;
    const payload = {
      name,
      attribute:    Number(form.attribute.value) || 0,
      hideY:        Number(form.hideY.value)      || 0,
      width:        Number(form.width.value)      || 1,
      height:       Number(form.height.value)     || 1,
      hasCollision: form.hasCollision.checked,
    };
    if (isUpdate) payload.objectId = objectId;
    setFeedback(feedbackEl, "保存中...", null);
    const { response, data } = await fetchJson("/api/maps/objects/templates", {
      method: isUpdate ? "PUT" : "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    if (!response.ok) {
      setFeedback(feedbackEl, `保存失敗: ${data?.error || response.status}`, "error");
      return;
    }
    setFeedback(feedbackEl, `テンプレートを${isUpdate ? "更新" : "追加"}しました (ID: ${data.objectId})`, "success");
    await loadTemplates();
    selectTemplate(data.objectId);
  });

  delBtn.addEventListener("click", async () => {
    const objectId = Number(form.objectId.value) || 0;
    if (!objectId) { setFeedback(feedbackEl, "削除対象を選択してください", "error"); return; }
    if (!window.confirm(`オブジェクトID ${objectId} のテンプレートを削除しますか？`)) return;
    setFeedback(feedbackEl, "削除中...", null);
    const { response, data } = await fetchJson("/api/maps/objects/templates", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ objectId }),
    });
    if (!response.ok) {
      setFeedback(feedbackEl, `削除失敗: ${data?.error || response.status}`, "error");
      return;
    }
    setFeedback(feedbackEl, "テンプレートを削除しました", "success");
    selectedId = null;
    await loadTemplates();
    clearForm();
  });

  loadTemplates();

  return {
    el: root,
    getTemplates() { return templates; },
  };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const adminGameFrame = document.getElementById("admin-game-frame");

  // ---- 状態 ----
  const state = {
    maps: [],
    selectedMapId: null,
    objects: [],         // 現在選択マップのオブジェクト一覧 (normalizeMapObject済み)
    selectedObjectId: null,
    previewCell: null,   // { x, y }
    filterType: "all",
    templates: [],
    sheetBaseUrl: "/api/assets/map-parts/sheets",
    tileSize: 16,
    sheetTileWidth: 32,
    sheetTileHeight: 32,
    isLoading: false,
  };

  // ---- レイアウト構築 ----
  // セクション1: ブラウザ（テーブル + グリッド）
  const browserSection = document.createElement("section");
  browserSection.className = "card";
  const h2 = document.createElement("h2");
  h2.textContent = "マップオブジェクト配置";
  browserSection.appendChild(h2);
  const desc = document.createElement("p");
  desc.className = "card-description";
  desc.textContent = "マップを選択するとオブジェクト一覧と配置状況を確認できます。左クリックで座標/オブジェクト選択。";
  browserSection.appendChild(desc);

  // コントロールバー
  const ctrlBar = document.createElement("div");
  ctrlBar.className = "map-object-controls";
  const mapLabel = document.createElement("label");
  mapLabel.className = "form-field compact";
  mapLabel.innerHTML = "<span>マップ</span>";
  const mapSelect = document.createElement("select");
  mapSelect.name = "mapId";
  mapLabel.appendChild(mapSelect);
  ctrlBar.appendChild(mapLabel);

  const filterLabel = document.createElement("label");
  filterLabel.className = "form-field compact";
  filterLabel.innerHTML = "<span>表示条件</span>";
  const filterSelect = document.createElement("select");
  filterSelect.name = "objectType";
  // allとtype options
  [{ value: "all", label: "すべて" }, ...MAP_OBJECT_TYPE_OPTIONS].forEach(({ value, label }) => {
    const opt = document.createElement("option");
    opt.value = value;
    opt.textContent = label;
    filterSelect.appendChild(opt);
  });
  filterLabel.appendChild(filterSelect);
  ctrlBar.appendChild(filterLabel);

  const summaryEl = document.createElement("div");
  summaryEl.className = "map-object-summary";
  summaryEl.setAttribute("aria-live", "polite");
  ctrlBar.appendChild(summaryEl);
  browserSection.appendChild(ctrlBar);

  // テーブル
  const tableWrap = document.createElement("div");
  tableWrap.className = "table-wrapper";
  const table = document.createElement("table");
  table.className = "data-table";
  table.innerHTML = `<thead><tr>
    <th>配置ID</th><th>オブジェクトID</th><th>名称</th>
    <th>X</th><th>Y</th><th>属性</th><th>当たり判定</th>
  </tr></thead>`;
  const tbody = document.createElement("tbody");
  tbody.innerHTML = "<tr><td colspan='7'>マップを選択してください</td></tr>";
  table.appendChild(tbody);
  tableWrap.appendChild(table);
  browserSection.appendChild(tableWrap);
  container.appendChild(browserSection);

  // セクション2: エディタ（グリッドキャンバス + フォーム）
  const editorSection = document.createElement("section");
  editorSection.className = "card";
  const h2ed = document.createElement("h2");
  h2ed.textContent = "配置プレビューと編集";
  editorSection.appendChild(h2ed);
  const edDesc = document.createElement("p");
  edDesc.className = "card-description";
  edDesc.textContent = "座標グリッドをクリックすると、該当セルに新規オブジェクトを配置できます。";
  editorSection.appendChild(edDesc);

  const selectedLabel = document.createElement("div");
  selectedLabel.className = "map-object-selected";
  selectedLabel.setAttribute("aria-live", "polite");
  selectedLabel.textContent = "選択中: 未選択";
  editorSection.appendChild(selectedLabel);

  const gridCanvas = createObjectGridCanvas({
    sheetBaseUrl: state.sheetBaseUrl,
    tileSize: state.tileSize,
    sheetTileWidth: state.sheetTileWidth,
    sheetTileHeight: state.sheetTileHeight,
    onCellClick(cx, cy, occupant) {
      handleCellClick(cx, cy, occupant);
    },
  });

  const gridWrap = document.createElement("div");
  gridWrap.className = "map-object-preview";
  gridWrap.appendChild(gridCanvas.el);
  // legend
  const legend = document.createElement("div");
  legend.className = "map-object-legend";
  legend.innerHTML = `
    <span class="legend-item"><span class="legend-marker"></span>空きセル</span>
    <span class="legend-item"><span class="legend-marker" style="background:rgba(0,120,255,0.55)"></span>オブジェクトあり</span>
    <span class="legend-item"><span class="legend-marker" style="background:rgba(255,200,0,0.45)"></span>選択位置</span>
    <span class="legend-item"><span class="legend-marker" style="background:rgba(255,80,0,0.75)"></span>選択中オブジェクト</span>
  `;
  gridWrap.appendChild(legend);
  editorSection.appendChild(gridWrap);

  // 配置フォーム
  const placementForm = document.createElement("form");
  placementForm.className = "map-object-form";
  placementForm.noValidate = true;

  const hiddenDataId = document.createElement("input");
  hiddenDataId.type = "hidden";
  hiddenDataId.name = "dataId";

  const formGrid = document.createElement("div");
  formGrid.className = "form-grid compact";

  const placementIdLabel = document.createElement("label");
  placementIdLabel.className = "form-field";
  placementIdLabel.innerHTML = "<span>配置ID <small>(自動)</small></span>";
  const placementIdInput = document.createElement("input");
  placementIdInput.type = "text";
  placementIdInput.name = "placementIdDisplay";
  placementIdInput.readOnly = true;
  placementIdInput.value = "(新規)";
  placementIdLabel.appendChild(placementIdInput);

  const objTypeLabel = document.createElement("label");
  objTypeLabel.className = "form-field";
  objTypeLabel.innerHTML = "<span>オブジェクト</span>";
  const objTypeSelect = document.createElement("select");
  objTypeSelect.name = "objectId";
  objTypeSelect.required = true;
  const emptyOpt = document.createElement("option");
  emptyOpt.value = "";
  emptyOpt.textContent = "-- オブジェクトを選択 --";
  objTypeSelect.appendChild(emptyOpt);
  objTypeLabel.appendChild(objTypeSelect);

  const xLabel = document.createElement("label");
  xLabel.className = "form-field coordinate";
  xLabel.innerHTML = "<span>座標 X</span>";
  const xInput = document.createElement("input");
  xInput.type = "number"; xInput.name = "x"; xInput.min = "0"; xInput.required = true;
  xLabel.appendChild(xInput);

  const yLabel = document.createElement("label");
  yLabel.className = "form-field coordinate";
  yLabel.innerHTML = "<span>座標 Y</span>";
  const yInput = document.createElement("input");
  yInput.type = "number"; yInput.name = "y"; yInput.min = "0"; yInput.required = true;
  yLabel.appendChild(yInput);

  formGrid.appendChild(placementIdLabel);
  formGrid.appendChild(objTypeLabel);
  formGrid.appendChild(xLabel);
  formGrid.appendChild(yLabel);

  const formActions = document.createElement("div");
  formActions.className = "form-actions";
  const saveBtn   = document.createElement("button");
  saveBtn.type = "submit"; saveBtn.className = "button primary"; saveBtn.textContent = "配置を保存";
  const clearBtn  = document.createElement("button");
  clearBtn.type = "button"; clearBtn.className = "button secondary"; clearBtn.textContent = "選択をクリア";
  const deleteBtn = document.createElement("button");
  deleteBtn.type = "button"; deleteBtn.className = "button danger"; deleteBtn.textContent = "削除";
  deleteBtn.disabled = true;
  formActions.appendChild(saveBtn);
  formActions.appendChild(clearBtn);
  formActions.appendChild(deleteBtn);

  const placementFeedbackEl = document.createElement("div");
  placementFeedbackEl.className = "result-message";
  placementFeedbackEl.setAttribute("role", "status");

  placementForm.appendChild(hiddenDataId);
  placementForm.appendChild(formGrid);
  placementForm.appendChild(formActions);
  editorSection.appendChild(placementForm);
  editorSection.appendChild(placementFeedbackEl);
  container.appendChild(editorSection);

  // セクション3: テンプレート管理
  const templatePanel = createTemplatePanel({
    onTemplatesChange(templates) {
      state.templates = templates;
      populateObjTypeSelect();
    },
  });
  container.appendChild(templatePanel.el);

  // ----------------------------------------------------------------
  // ヘルパー関数
  // ----------------------------------------------------------------

  function populateObjTypeSelect() {
    const prev = objTypeSelect.value;
    objTypeSelect.innerHTML = "";
    const emp = document.createElement("option");
    emp.value = ""; emp.textContent = "-- オブジェクトを選択 --";
    objTypeSelect.appendChild(emp);
    state.templates.forEach((tmpl) => {
      const opt = document.createElement("option");
      opt.value = String(tmpl.objectId);
      opt.textContent = `[${tmpl.objectId}] ${tmpl.name || "(無名)"}`;
      objTypeSelect.appendChild(opt);
    });
    if (prev) objTypeSelect.value = prev;
  }

  function populateMapSelect() {
    mapSelect.innerHTML = "";
    if (!state.maps.length) {
      const opt = document.createElement("option");
      opt.value = ""; opt.textContent = state.isLoading ? "読み込み中..." : "マップデータなし";
      mapSelect.appendChild(opt);
      mapSelect.disabled = true;
      return;
    }
    state.maps.forEach((m) => {
      const opt = document.createElement("option");
      opt.value = String(m.id);
      opt.textContent = `${m.name} (ID: ${m.id})`;
      mapSelect.appendChild(opt);
    });
    mapSelect.disabled = false;
    if (state.selectedMapId) mapSelect.value = String(state.selectedMapId);
  }

  function getSelectedMap() {
    return state.maps.find((m) => String(m.id) === String(state.selectedMapId)) || null;
  }

  function renderSummary() {
    const map = getSelectedMap();
    if (!map) {
      summaryEl.textContent = state.maps.length ? "マップが選択されていません" : "マップデータなし";
      return;
    }
    const total = map.objects.length;
    summaryEl.textContent = `${map.name} / ${map.width}×${map.height} / オブジェクト数: ${total}`;
  }

  function getFilteredObjects() {
    const map = getSelectedMap();
    if (!map) return [];
    return map.objects.filter((obj) => {
      if (state.filterType === "collision")    return obj.hasCollision;
      if (state.filterType === "no-collision") return !obj.hasCollision;
      return true;
    });
  }

  function renderTable() {
    tbody.innerHTML = "";
    const filtered = getFilteredObjects().sort((a, b) => a.y === b.y ? a.x - b.x : a.y - b.y);
    if (!filtered.length) {
      const tr = document.createElement("tr");
      const td = document.createElement("td"); td.colSpan = 7; td.textContent = "オブジェクトなし";
      tr.appendChild(td); tbody.appendChild(tr);
      return;
    }
    filtered.forEach((obj) => {
      const tr = document.createElement("tr");
      tr.dataset.objectId = obj.id;
      tr.tabIndex = 0;
      if (obj.id === state.selectedObjectId) tr.classList.add("is-selected");
      [
        obj.placementId !== null ? String(obj.placementId) : "-",
        obj.objectIdHex ? `${obj.objectId} (${obj.objectIdHex})` : (obj.objectId !== null ? String(obj.objectId) : "-"),
        obj.name,
        String(obj.x), String(obj.y),
        obj.attributeHex || "-",
        obj.hasCollision ? "あり" : "なし",
      ].forEach((text) => {
        const td = document.createElement("td");
        td.textContent = text;
        tr.appendChild(td);
      });
      tr.addEventListener("click", () => selectObject(obj.id));
      tr.addEventListener("keydown", (ev) => {
        if (ev.key === "Enter" || ev.key === " ") { ev.preventDefault(); selectObject(obj.id); }
      });
      tbody.appendChild(tr);
    });
  }

  function applyObjectToForm(obj) {
    const map = getSelectedMap();
    const maxX = map ? map.width - 1 : 0;
    const maxY = map ? map.height - 1 : 0;
    if (obj) {
      hiddenDataId.value = obj.placementId !== null ? String(obj.placementId) : "";
      placementIdInput.value = obj.placementId !== null ? String(obj.placementId) : "(新規)";
      if (obj.objectId !== null) objTypeSelect.value = String(obj.objectId);
      xInput.value = String(Math.min(Math.max(obj.x, 0), maxX));
      yInput.value = String(Math.min(Math.max(obj.y, 0), maxY));
      selectedLabel.textContent = `選択中: ${obj.name}${obj.objectIdHex ? ` (${obj.objectIdHex})` : ""}`;
      deleteBtn.disabled = false;
    } else {
      hiddenDataId.value = "";
      placementIdInput.value = "(新規)";
      if (objTypeSelect.options.length) objTypeSelect.selectedIndex = 0;
      const px = state.previewCell;
      xInput.value = String(Math.min(Math.max(px ? px.x : 0, 0), maxX));
      yInput.value = String(Math.min(Math.max(px ? px.y : 0, 0), maxY));
      selectedLabel.textContent = "選択中: 未選択";
      deleteBtn.disabled = true;
    }
  }

  function selectObject(objectId) {
    const map = getSelectedMap();
    if (!map) return;
    const obj = objectId ? map.objects.find((o) => o.id === objectId) : null;
    state.selectedObjectId = obj ? objectId : null;
    state.previewCell = obj ? { x: obj.x, y: obj.y } : null;
    applyObjectToForm(obj || null);
    renderTable();
    gridCanvas.setSelection(state.selectedObjectId, state.previewCell);
  }

  function handleCellClick(cx, cy, occupant) {
    if (occupant) {
      selectObject(occupant.id);
      return;
    }
    state.previewCell = { x: cx, y: cy };
    state.selectedObjectId = null;
    applyObjectToForm(null);
    xInput.value = String(cx);
    yInput.value = String(cy);
    gridCanvas.setSelection(null, state.previewCell);
    setFeedback(placementFeedbackEl, `座標 (${cx}, ${cy}) を選択。オブジェクトを選んで「配置を保存」で追加します。`, null);
  }

  async function loadMapData(forceReload = false) {
    if (state.isLoading) return;
    state.isLoading = true;
    summaryEl.textContent = "読み込み中...";
    try {
      const { response, data } = await fetchJson("/api/maps/objects");
      if (!response.ok || !Array.isArray(data?.maps)) throw new Error("データ取得失敗");
      if (data.sheetBaseUrl) {
        state.sheetBaseUrl = data.sheetBaseUrl;
      }
      if (data.tileSize)       state.tileSize = Number(data.tileSize) || state.tileSize;
      if (data.sheetTileWidth)  state.sheetTileWidth = Number(data.sheetTileWidth) || state.sheetTileWidth;
      if (data.sheetTileHeight) state.sheetTileHeight = Number(data.sheetTileHeight) || state.sheetTileHeight;

      state.maps = data.maps.map(normalizeMapEntry);
      if (!state.selectedMapId || !state.maps.some((m) => String(m.id) === String(state.selectedMapId))) {
        state.selectedMapId = state.maps.length ? state.maps[0].id : null;
      }
      state.selectedObjectId = null;
      state.previewCell = null;
      populateMapSelect();
      renderSummary();
      renderTable();
      const map = getSelectedMap();
      gridCanvas.setMap(map || null);
      applyObjectToForm(null);
    } catch (err) {
      summaryEl.textContent = `取得エラー: ${err.message}`;
    } finally {
      state.isLoading = false;
    }
  }

  // ---- イベント ----
  mapSelect.addEventListener("change", () => {
    state.selectedMapId = mapSelect.value;
    state.selectedObjectId = null;
    state.previewCell = null;
    renderSummary();
    renderTable();
    const map = getSelectedMap();
    gridCanvas.setMap(map || null);
    applyObjectToForm(null);
    setFeedback(placementFeedbackEl, "", null);
    if (adminGameFrame && adminGameFrame.contentWindow) {
      adminGameFrame.contentWindow.postMessage({ kind: "sbop2_set_admin_mode", mode: 0 }, "*");
    }
  });

  filterSelect.addEventListener("change", () => {
    state.filterType = filterSelect.value;
    renderTable();
  });

  clearBtn.addEventListener("click", () => {
    selectObject(null);
    setFeedback(placementFeedbackEl, "選択をクリアしました", null);
  });

  placementForm.addEventListener("submit", async (ev) => {
    ev.preventDefault();
    const map = getSelectedMap();
    if (!map || !map.id) {
      setFeedback(placementFeedbackEl, "マップを選択してください", "error"); return;
    }
    const objectIdVal = Number(objTypeSelect.value);
    if (!objectIdVal || objectIdVal <= 0) {
      setFeedback(placementFeedbackEl, "オブジェクトを選択してください", "error"); return;
    }
    const x      = Number(xInput.value);
    const y      = Number(yInput.value);
    const dataId = Number(hiddenDataId.value) || 0;
    setFeedback(placementFeedbackEl, "保存中...", null);
    try {
      const isUpdate = dataId > 0;
      const payload  = isUpdate
        ? { mapId: map.id, dataId, objectId: objectIdVal, x, y }
        : { mapId: map.id, objectId: objectIdVal, x, y };
      const { response, data } = await fetchJson("/api/maps/placements", {
        method: isUpdate ? "PUT" : "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        setFeedback(placementFeedbackEl, `保存失敗: ${data?.error || response.status}`, "error"); return;
      }
      setFeedback(placementFeedbackEl, `配置を${isUpdate ? "更新" : "追加"}しました (配置ID: ${data.dataId})`, "success");
      await loadMapData(true);
      const newMap = getSelectedMap();
      if (newMap && data.dataId) {
        const added = newMap.objects.find((o) => o.placementId === data.dataId);
        if (added) selectObject(added.id);
      }
    } catch (err) {
      setFeedback(placementFeedbackEl, "保存中にエラーが発生しました", "error");
    }
  });

  deleteBtn.addEventListener("click", async () => {
    const map = getSelectedMap();
    if (!map || !map.id) { setFeedback(placementFeedbackEl, "マップを選択してください", "error"); return; }
    const dataId = Number(hiddenDataId.value) || 0;
    if (!dataId) { setFeedback(placementFeedbackEl, "削除する配置を選択してください", "error"); return; }
    if (!window.confirm("この配置を削除しますか？")) return;
    setFeedback(placementFeedbackEl, "削除中...", null);
    try {
      const { response, data } = await fetchJson("/api/maps/placements", {
        method: "DELETE",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ mapId: map.id, dataId }),
      });
      if (!response.ok) {
        setFeedback(placementFeedbackEl, `削除失敗: ${data?.error || response.status}`, "error"); return;
      }
      setFeedback(placementFeedbackEl, "配置を削除しました", "success");
      state.selectedObjectId = null;
      state.previewCell = null;
      await loadMapData(true);
      applyObjectToForm(null);
    } catch (err) {
      setFeedback(placementFeedbackEl, "削除中にエラーが発生しました", "error");
    }
  });

  // ---- iframe メッセージ連携 ----
  // app.js の handleAdminGamePick は currentRoute === "map-objects" の時に
  // 独自処理していないため、ここで sbop2_admin_pick を受け取り
  // クリックセル情報をフォームに反映する
  function onAdminMessage(ev) {
    if (adminGameFrame && ev.source !== adminGameFrame.contentWindow) return;
    const msg = ev.data;
    if (!msg || typeof msg !== "object") return;
    if (msg.kind === "sbop2_admin_pick") {
      const mapId = Number(msg.mapId) || 0;
      const cellX = Number(msg.cellX) || 0;
      const cellY = Number(msg.cellY) || 0;
      if (mapId > 0) {
        // マップが違う場合は切り替え
        if (String(state.selectedMapId) !== String(mapId)) {
          state.selectedMapId = mapId;
          mapSelect.value = String(mapId);
          loadMapData(false).then(() => {
            handleCellClick(cellX, cellY, null);
          });
        } else {
          const map = getSelectedMap();
          if (map) {
            const occupantMap = new Map();
            map.objects.forEach((o) => occupantMap.set(`${o.x},${o.y}`, o));
            const occ = occupantMap.get(`${cellX},${cellY}`) || null;
            handleCellClick(cellX, cellY, occ);
          }
        }
      }
    }
  }
  window.addEventListener("message", onAdminMessage);

  // ---- 初期ロード ----
  loadMapData(false);

  _destroyFn = () => {
    window.removeEventListener("message", onAdminMessage);
    container.innerHTML = "";
  };
}

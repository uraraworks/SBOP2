/**
 * views/map-parts-edit.js
 * マップパーツ編集画面 (route: map-parts)
 * 一覧はパーツ完成画像のパレットグリッド表示。
 * クリックで詳細画面へ切替(list-detail 同等の操作感)。
 * openPartsDetail(partsId) で外部から詳細を直接開ける。
 */

import { fetchJson } from "../core/api.js";
import { fetchMapPartsData, invalidateMapPartsData } from "../data/map-parts-data.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createDragList } from "../components/drag-list.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { createSpriteThumbLazy } from "../components/sprite-thumb.js";
import { loadCatalog, calcSpriteCoord, loadSheetImage } from "../data/assets.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const VIEW_TYPE_LABELS = {
  0: "通常レイヤー",
  1: "背景レイヤー",
  2: "装飾レイヤー",
  3: "イベントレイヤー",
  4: "水辺レイヤー",
  5: "特殊レイヤー"
};

const ANIME_TYPE_LABELS = {
  0: "静止",
  1: "往復",
  2: "循環",
  3: "ランダム"
};

const DIRECTION_LABELS = { up: "上", down: "下", left: "左", right: "右" };

// ----------------------------------------------------------------
// API ヘルパ
// ----------------------------------------------------------------

async function fetchPartsList() {
  const data = await fetchMapPartsData();
  return data.parts.map(normalizePart).filter(Boolean).sort((a, b) => a.partsId - b.partsId);
}

function normalizePart(raw) {
  if (!raw || typeof raw !== "object") return null;
  const partsId = Number(raw.partsId ?? raw.partsID ?? 0);
  const viewType = Number(raw.viewType ?? 0);
  const animeType = Number(raw.animeType ?? 0);
  const animeCount = Number(raw.animeCount ?? 0);
  const level = Number(raw.level ?? 0);
  const posX = Number(raw?.viewPosition?.x ?? 0);
  const posY = Number(raw?.viewPosition?.y ?? 0);
  const movementDirection = typeof raw?.movement?.direction === "string"
    ? raw.movement.direction.trim().toLowerCase() : "";

  const flags = {
    block: Boolean(raw?.flags?.block),
    pile: Boolean(raw?.flags?.pile),
    pileBack: Boolean(raw?.flags?.pileBack),
    fishing: Boolean(raw?.flags?.fishing),
    drawLast: Boolean(raw?.flags?.drawLast),
    counter: Boolean(raw?.flags?.counter),
    blockUp: Boolean(raw?.flags?.blockDirections?.up),
    blockDown: Boolean(raw?.flags?.blockDirections?.down),
    blockLeft: Boolean(raw?.flags?.blockDirections?.left),
    blockRight: Boolean(raw?.flags?.blockDirections?.right),
  };

  const animeFrames = Array.isArray(raw.animeFrames) ? raw.animeFrames : [];

  const grpIdBase = raw?.sprites?.base
    ? (Number(raw.sprites.base.sheet) * 1024 + Number(raw.sprites.base.tile))
    : (Number(raw.grpIdBase ?? 0));
  const grpIdPile = raw?.sprites?.overlay
    ? (Number(raw.sprites.overlay.sheet) * 1024 + Number(raw.sprites.overlay.tile))
    : (Number(raw.grpIdPile ?? 0));

  return {
    partsId, viewType, animeType, animeCount, level,
    viewPosition: { x: posX, y: posY },
    movementDirection, flags, grpIdBase, grpIdPile, animeFrames,
  };
}

// ----------------------------------------------------------------
// パレットタイル用: grpIdBase + grpIdPile を合成して描画する canvas
// ----------------------------------------------------------------

const TILE_SIZE = 40; // パレットタイルのピクセルサイズ

/**
 * grpIdBase と grpIdPile を合成したサムネ canvas 要素を返す。
 * カタログ取得後に非同期で描画する。
 */
function createCompositeThumb(grpIdBase, grpIdPile) {
  const canvas = document.createElement("canvas");
  canvas.width = TILE_SIZE;
  canvas.height = TILE_SIZE;

  loadCatalog().then((catalog) => {
    const cat = catalog.find((c) => c.key === "mapParts") ?? null;
    if (!cat) return;
    const ctx = canvas.getContext("2d");
    ctx.imageSmoothingEnabled = false;

    function drawLayer(sub) {
      if (!sub) return Promise.resolve();
      const coord = calcSpriteCoord(cat, sub);
      return loadSheetImage(coord.sheetUrl).then((img) => {
        ctx.drawImage(img, coord.x, coord.y, cat.cellSize, cat.cellSize, 0, 0, TILE_SIZE, TILE_SIZE);
      }).catch(() => {});
    }

    drawLayer(grpIdBase).then(() => drawLayer(grpIdPile));
  }).catch(() => {});

  return canvas;
}

// ----------------------------------------------------------------
// 詳細フォーム描画 (mount / openPartsDetail 共用)
// ----------------------------------------------------------------

function buildDetailForm(part) {
  const wrap = document.createElement("div");
  wrap.className = "map-parts-edit-detail";

  // ---- グラフィック ----
  const grpSection = document.createElement("section");
  grpSection.className = "detail-section";
  const grpH3 = document.createElement("h3");
  grpH3.textContent = "グラフィック";
  grpSection.appendChild(grpH3);

  const sfBase = createSpriteField({
    categoryKey: "mapParts",
    value: part.grpIdBase,
    label: "下地スプライト (grpIdBase)",
  });
  const sfPile = createSpriteField({
    categoryKey: "mapParts",
    value: part.grpIdPile,
    label: "重ねスプライト (grpIdPile)",
  });
  grpSection.appendChild(sfBase.el);
  grpSection.appendChild(sfPile.el);
  wrap.appendChild(grpSection);

  // ---- 基本パラメータ ----
  const paramSection = document.createElement("section");
  paramSection.className = "detail-section";
  const paramH3 = document.createElement("h3");
  paramH3.textContent = "基本パラメータ";
  paramSection.appendChild(paramH3);

  const grid = document.createElement("div");
  grid.className = "form-grid compact";

  // 表示種別
  const viewTypeLabel = document.createElement("label");
  viewTypeLabel.className = "form-field";
  const viewTypeSpan = document.createElement("span");
  viewTypeSpan.textContent = "表示種別";
  const viewTypeSelect = document.createElement("select");
  viewTypeSelect.name = "viewType";
  Object.entries(VIEW_TYPE_LABELS).forEach(([v, l]) => {
    const opt = document.createElement("option");
    opt.value = v; opt.textContent = `${l} (${v})`;
    if (Number(v) === part.viewType) opt.selected = true;
    viewTypeSelect.appendChild(opt);
  });
  viewTypeLabel.append(viewTypeSpan, viewTypeSelect);
  grid.appendChild(viewTypeLabel);

  // アニメ種別
  const animeTypeLabel = document.createElement("label");
  animeTypeLabel.className = "form-field";
  const animeTypeSpan = document.createElement("span");
  animeTypeSpan.textContent = "アニメーション種別";
  const animeTypeSelect = document.createElement("select");
  animeTypeSelect.name = "animeType";
  Object.entries(ANIME_TYPE_LABELS).forEach(([v, l]) => {
    const opt = document.createElement("option");
    opt.value = v; opt.textContent = `${l} (${v})`;
    if (Number(v) === part.animeType) opt.selected = true;
    animeTypeSelect.appendChild(opt);
  });
  animeTypeLabel.append(animeTypeSpan, animeTypeSelect);
  grid.appendChild(animeTypeLabel);

  // 透明度
  const levelLabel = document.createElement("label");
  levelLabel.className = "form-field";
  const levelSpan = document.createElement("span");
  levelSpan.textContent = "透明度";
  const levelInput = document.createElement("input");
  levelInput.type = "range"; levelInput.name = "level";
  levelInput.min = "0"; levelInput.max = "255";
  levelInput.value = String(part.level);
  const levelValue = document.createElement("span");
  levelValue.className = "range-value";
  levelValue.textContent = String(part.level);
  levelInput.addEventListener("input", () => { levelValue.textContent = levelInput.value; });
  levelLabel.append(levelSpan, levelInput, levelValue);
  grid.appendChild(levelLabel);

  // 移動方向
  const moveLabel = document.createElement("label");
  moveLabel.className = "form-field";
  const moveSpan = document.createElement("span");
  moveSpan.textContent = "移動方向";
  const moveSelect = document.createElement("select");
  moveSelect.name = "moveDirection";
  [["", "なし"], ...Object.entries(DIRECTION_LABELS)].forEach(([v, l]) => {
    const opt = document.createElement("option");
    opt.value = v; opt.textContent = l;
    if (v === part.movementDirection) opt.selected = true;
    moveSelect.appendChild(opt);
  });
  moveLabel.append(moveSpan, moveSelect);
  grid.appendChild(moveLabel);

  // 表示位置 X/Y
  const posXLabel = document.createElement("label");
  posXLabel.className = "form-field coordinate";
  const posXSpan = document.createElement("span");
  posXSpan.textContent = "表示位置 X";
  const posXInput = document.createElement("input");
  posXInput.type = "number"; posXInput.name = "viewPositionX"; posXInput.min = "0";
  posXInput.value = String(part.viewPosition.x);
  posXLabel.append(posXSpan, posXInput);
  grid.appendChild(posXLabel);

  const posYLabel = document.createElement("label");
  posYLabel.className = "form-field coordinate";
  const posYSpan = document.createElement("span");
  posYSpan.textContent = "表示位置 Y";
  const posYInput = document.createElement("input");
  posYInput.type = "number"; posYInput.name = "viewPositionY"; posYInput.min = "0";
  posYInput.value = String(part.viewPosition.y);
  posYLabel.append(posYSpan, posYInput);
  grid.appendChild(posYLabel);

  paramSection.appendChild(grid);
  wrap.appendChild(paramSection);

  // ---- フラグ ----
  const flagSection = document.createElement("section");
  flagSection.className = "detail-section";
  const flagH3 = document.createElement("h3");
  flagH3.textContent = "フラグ";
  flagSection.appendChild(flagH3);

  const flagGrid = document.createElement("div");
  flagGrid.className = "map-parts-edit-flags";

  const flagDefs = [
    { key: "block",    label: "ぶつかる" },
    { key: "pile",     label: "重なる" },
    { key: "pileBack", label: "下地も重ねる" },
    { key: "fishing",  label: "釣り場" },
    { key: "drawLast", label: "最前面" },
    { key: "counter",  label: "カウンター" },
  ];
  const flagCheckboxes = {};
  flagDefs.forEach(({ key, label }) => {
    const lbl = document.createElement("label");
    const cb = document.createElement("input");
    cb.type = "checkbox"; cb.name = key; cb.checked = Boolean(part.flags[key]);
    flagCheckboxes[key] = cb;
    lbl.append(cb, ` ${label}`);
    flagGrid.appendChild(lbl);
  });
  flagSection.appendChild(flagGrid);

  const dirFieldset = document.createElement("fieldset");
  dirFieldset.className = "form-field roles-field";
  const dirLegend = document.createElement("legend");
  dirLegend.textContent = "ぶつかり方向";
  dirFieldset.appendChild(dirLegend);
  const dirGrid = document.createElement("div");
  dirGrid.className = "map-parts-edit-flags";
  const dirKeys = { blockUp: "上", blockDown: "下", blockLeft: "左", blockRight: "右" };
  const dirCheckboxes = {};
  Object.entries(dirKeys).forEach(([key, label]) => {
    const lbl = document.createElement("label");
    const cb = document.createElement("input");
    cb.type = "checkbox"; cb.name = key; cb.checked = Boolean(part.flags[key]);
    dirCheckboxes[key] = cb;
    lbl.append(cb, ` ${label}`);
    dirGrid.appendChild(lbl);
  });
  dirFieldset.appendChild(dirGrid);
  flagSection.appendChild(dirFieldset);
  wrap.appendChild(flagSection);

  // ---- アニメーションフレーム ----
  const animeSection = document.createElement("section");
  animeSection.className = "detail-section";
  const animeH3 = document.createElement("h3");
  animeH3.textContent = "アニメーションフレーム";
  animeSection.appendChild(animeH3);

  let _frames = part.animeFrames.slice();

  function makeFrameRow(frame) {
    const row = document.createElement("div");
    row.className = "anime-frame-fields";
    row.style.cssText = "display:flex;gap:8px;align-items:center;";

    const fields = [
      { label: "待ち(×10ms)", key: "wait",      min: 0, max: 255 },
      { label: "透明度",       key: "level",     min: 0, max: 255 },
      { label: "下地GrpID",    key: "grpIdBase", min: 0, max: 65535 },
      { label: "重ねGrpID",    key: "grpIdPile", min: 0, max: 65535 },
    ];
    fields.forEach(({ label, key, min, max }) => {
      const lbl = document.createElement("label");
      lbl.className = "form-field compact";
      const span = document.createElement("span");
      span.textContent = label;
      const inp = document.createElement("input");
      inp.type = "number"; inp.dataset.frameField = key;
      inp.value = String(frame ? (frame[key] ?? 0) : 0);
      inp.min = String(min); inp.max = String(max);
      inp.style.width = "70px";
      lbl.append(span, inp);
      row.appendChild(lbl);
    });

    const thumbWrap = document.createElement("div");
    thumbWrap.className = "anime-frame-thumb";
    const thumb = createSpriteThumbLazy({ categoryKey: "mapParts", sub: frame?.grpIdBase ?? 0, size: 24 });
    thumbWrap.appendChild(thumb.el);
    const grpBaseInput = row.querySelector("[data-frame-field='grpIdBase']");
    if (grpBaseInput) {
      grpBaseInput.addEventListener("change", () => {
        const v = parseInt(grpBaseInput.value, 10);
        if (Number.isFinite(v)) thumb.update(v);
      });
    }
    row.appendChild(thumbWrap);
    return row;
  }

  let _dragList = null;

  function buildDragList() {
    if (_dragList) { _dragList.el.remove(); _dragList = null; }
    _dragList = createDragList({
      items: _frames,
      renderItem: (frame) => makeFrameRow(frame),
      onReorder: (newItems) => { _frames = newItems; },
    });
    animeSection.insertBefore(_dragList.el, addFrameBtn);
  }

  const addFrameBtn = document.createElement("button");
  addFrameBtn.type = "button";
  addFrameBtn.className = "button small";
  addFrameBtn.textContent = "+ フレーム追加";
  addFrameBtn.addEventListener("click", () => {
    _frames.push({ wait: 10, level: 0, grpIdBase: 0, grpIdPile: 0 });
    buildDragList();
  });

  animeSection.appendChild(addFrameBtn);
  buildDragList();
  wrap.appendChild(animeSection);

  // ---- anime-preview ----
  const previewSection = document.createElement("section");
  previewSection.className = "detail-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "プレビュー";
  previewSection.appendChild(previewH3);

  const preview = createAnimePreview({ width: 16, height: 16, scale: 6 });
  previewSection.appendChild(preview.el);

  const previewBtn = document.createElement("button");
  previewBtn.type = "button";
  previewBtn.className = "button small";
  previewBtn.textContent = "プレビュー更新";

  function collectFrames() {
    if (!_dragList) return [];
    const rows = _dragList.el.querySelectorAll(".drag-list-item");
    const frames = [];
    rows.forEach((li) => {
      const frame = {};
      li.querySelectorAll("[data-frame-field]").forEach((inp) => {
        frame[inp.dataset.frameField] = Number(inp.value) || 0;
      });
      frames.push(frame);
    });
    return frames;
  }

  function updatePreview() {
    const frames = collectFrames();
    if (!frames.length) {
      preview.setFrames([{
        wait: 100,
        layers: [
          { categoryKey: "mapParts", sub: sfBase.getValue() },
          { categoryKey: "mapParts", sub: sfPile.getValue() },
        ],
      }]);
    } else {
      preview.setFrames(frames.map((f) => ({
        wait: f.wait,
        layers: [
          { categoryKey: "mapParts", sub: f.grpIdBase },
          { categoryKey: "mapParts", sub: f.grpIdPile },
        ],
      })));
    }
  }

  previewBtn.addEventListener("click", updatePreview);
  previewSection.appendChild(previewBtn);
  wrap.appendChild(previewSection);
  updatePreview();

  // ---- ペイロード収集 ----
  wrap._collectData = function () {
    return {
      partsId: part.partsId,
      viewType: Number(viewTypeSelect.value),
      animeType: Number(animeTypeSelect.value),
      level: Number(levelInput.value),
      moveDirection: moveSelect.value,
      viewPositionX: Number(posXInput.value),
      viewPositionY: Number(posYInput.value),
      grpIdBase: sfBase.getValue(),
      grpIdPile: sfPile.getValue(),
      block: flagCheckboxes.block.checked,
      pile: flagCheckboxes.pile.checked,
      pileBack: flagCheckboxes.pileBack.checked,
      fishing: flagCheckboxes.fishing.checked,
      drawLast: flagCheckboxes.drawLast.checked,
      counter: flagCheckboxes.counter.checked,
      blockUp: dirCheckboxes.blockUp.checked,
      blockDown: dirCheckboxes.blockDown.checked,
      blockLeft: dirCheckboxes.blockLeft.checked,
      blockRight: dirCheckboxes.blockRight.checked,
      animeFrames: collectFrames(),
    };
  };

  return wrap;
}

// ----------------------------------------------------------------
// フィードバック表示
// ----------------------------------------------------------------

function showFeedback(container, message, type) {
  let fb = container.querySelector(".mp-feedback");
  if (!fb) {
    fb = document.createElement("p");
    fb.className = "mp-feedback result-message";
    fb.setAttribute("role", "status");
    fb.setAttribute("aria-live", "polite");
    container.prepend(fb);
  }
  fb.textContent = message;
  fb.className = "mp-feedback result-message";
  if (type) fb.classList.add(type);
}

// ----------------------------------------------------------------
// パレットグリッド付き 一覧⇔詳細 UI
// ----------------------------------------------------------------

function buildPartsUI(container, opts) {
  const {
    fetchList: _fetchList,
    onSave: _onSave,
    onCreate: _onCreate,
    onDelete: _onDelete,
  } = opts;

  container.innerHTML = "";

  let _allItems = [];
  let _selectedItem = null;
  let _dirty = false;

  // ---- シェル ----
  const shell = document.createElement("div");
  shell.className = "list-detail-shell";

  // === 一覧ペイン ===
  const listPane = document.createElement("div");
  listPane.className = "ld-list-pane";

  const listHeader = document.createElement("div");
  listHeader.className = "ld-list-header";
  const listTitle = document.createElement("div");
  listTitle.className = "ld-list-pane-title";
  listTitle.textContent = "マップパーツ一覧";
  listHeader.appendChild(listTitle);

  // パレットグリッド本体
  const paletteGrid = document.createElement("div");
  paletteGrid.className = "parts-palette-grid";
  paletteGrid.style.cssText =
    "display:grid;grid-template-columns:repeat(auto-fill," + TILE_SIZE + "px);" +
    "gap:4px;padding:8px;overflow-y:auto;flex:1;";

  const listFooter = document.createElement("div");
  listFooter.className = "ld-list-footer";
  const btnNew = document.createElement("button");
  btnNew.type = "button";
  btnNew.className = "button";
  btnNew.textContent = "+ 新規";
  listFooter.appendChild(btnNew);

  listPane.append(listHeader, paletteGrid, listFooter);

  // === 詳細ペイン ===
  const detailPane = document.createElement("div");
  detailPane.className = "ld-detail-pane";
  detailPane.style.display = "none";

  const detailToolbar = document.createElement("div");
  detailToolbar.className = "ld-detail-toolbar";

  const btnBack = document.createElement("button");
  btnBack.type = "button"; btnBack.className = "button"; btnBack.textContent = "← 戻る";

  const btnSave = document.createElement("button");
  btnSave.type = "button"; btnSave.className = "button primary"; btnSave.textContent = "保存";

  const btnDelete = document.createElement("button");
  btnDelete.type = "button"; btnDelete.className = "button danger"; btnDelete.textContent = "削除";

  const dirtyBadge = document.createElement("span");
  dirtyBadge.className = "ld-dirty-badge"; dirtyBadge.textContent = "未保存";
  dirtyBadge.style.display = "none";

  detailToolbar.append(btnBack, btnSave, btnDelete, dirtyBadge);

  const detailBody = document.createElement("div");
  detailBody.className = "ld-detail-body";

  detailPane.append(detailToolbar, detailBody);
  shell.append(listPane, detailPane);
  container.appendChild(shell);

  // ---- dirty 管理 ----
  function setDirty(f) {
    _dirty = f;
    dirtyBadge.style.display = f ? "" : "none";
  }

  detailBody.addEventListener("input",  () => { if (_selectedItem) setDirty(true); });
  detailBody.addEventListener("change", () => { if (_selectedItem) setDirty(true); });

  // ---- パレット描画 ----
  function renderPalette() {
    paletteGrid.innerHTML = "";
    _allItems.forEach((item) => {
      const cell = document.createElement("div");
      cell.className = "parts-palette-cell";
      cell.style.cssText =
        "width:" + TILE_SIZE + "px;height:" + (TILE_SIZE + 14) + "px;" +
        "display:flex;flex-direction:column;align-items:center;" +
        "cursor:pointer;border:1px solid transparent;border-radius:3px;" +
        "padding:1px;box-sizing:border-box;";
      cell.title = `パーツ ${item.partsId}`;
      if (item === _selectedItem) {
        cell.style.borderColor = "var(--color-accent, #4e9af1)";
        cell.style.background = "var(--color-accent-subtle, rgba(78,154,241,0.15))";
      }

      const thumb = createCompositeThumb(item.grpIdBase, item.grpIdPile);
      thumb.style.cssText = "width:" + TILE_SIZE + "px;height:" + TILE_SIZE + "px;image-rendering:pixelated;";

      const idLabel = document.createElement("span");
      idLabel.textContent = item.partsId;
      idLabel.style.cssText = "font-size:10px;line-height:14px;color:var(--color-muted,#888);";

      cell.append(thumb, idLabel);
      cell.addEventListener("click", () => selectItem(item));
      paletteGrid.appendChild(cell);
    });
  }

  // ---- 詳細フォーム ----
  function showDetailPane() {
    listPane.style.display = "none";
    detailPane.style.display = "";
  }
  function showListPane() {
    detailPane.style.display = "none";
    listPane.style.display = "";
  }

  function renderDetail(item) {
    detailBody.innerHTML = "";
    if (!item) return;
    const el = buildDetailForm(item);
    detailBody.appendChild(el);
  }

  function selectItem(item) {
    if (_dirty && !confirm("未保存の変更があります。破棄して切り替えますか?")) return;
    _selectedItem = item;
    setDirty(false);
    renderDetail(item);
    renderPalette(); // 選択ハイライト更新
    showDetailPane();
  }

  // ---- ボタンイベント ----
  btnBack.addEventListener("click", () => {
    if (_dirty && !confirm("未保存の変更があります。破棄して一覧に戻りますか?")) return;
    _selectedItem = null;
    setDirty(false);
    renderPalette();
    showListPane();
  });

  btnSave.addEventListener("click", async () => {
    if (!_selectedItem) return;
    const detailEl = detailBody.querySelector(".map-parts-edit-detail");
    if (!detailEl?._collectData) { alert("フォームが見つかりません"); return; }
    try {
      await _onSave(_selectedItem, detailEl._collectData());
      setDirty(false);
      const savedId = _selectedItem.partsId;
      await load();
      const found = _allItems.find((i) => i.partsId === savedId) ?? null;
      _selectedItem = found;
      renderDetail(found);
      renderPalette();
    } catch (err) {
      alert("保存に失敗しました: " + String(err?.message ?? err));
    }
  });

  btnDelete.addEventListener("click", async () => {
    if (!_selectedItem || !confirm("この項目を削除しますか?")) return;
    try {
      await _onDelete(_selectedItem);
      _selectedItem = null;
      setDirty(false);
      await load();
      showListPane();
    } catch (err) {
      alert("削除に失敗しました: " + String(err?.message ?? err));
    }
  });

  btnNew.addEventListener("click", async () => {
    if (_dirty && !confirm("未保存の変更があります。破棄して新規作成しますか?")) return;
    try {
      const newId = await _onCreate();
      await load();
      if (newId != null) {
        const found = _allItems.find((i) => i.partsId === newId) ?? _allItems[_allItems.length - 1];
        if (found) selectItem(found);
      }
    } catch (err) {
      alert("新規作成に失敗しました: " + String(err?.message ?? err));
    }
  });

  // ---- データ読み込み ----
  async function load() {
    try {
      _allItems = await _fetchList();
    } catch (e) {
      _allItems = [];
      console.error("パーツ一覧取得失敗:", e);
    }
    renderPalette();
  }

  // ---- 外部から詳細を開く ----
  async function openById(partsId) {
    if (!_allItems.length) await load();
    const item = _allItems.find((i) => i.partsId === partsId);
    if (item) selectItem(item);
  }

  load();

  return { openById };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;
let _ui = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  _ui = buildPartsUI(container, {
    fetchList: fetchPartsList,

    onSave: async (part, payload) => {
      const { response, data } = await fetchJson("/api/maps/parts", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        const msg = data?.error ?? `HTTP ${response.status}`;
        showFeedback(container, `保存に失敗しました: ${msg}`, "error");
        throw new Error(msg);
      }
      invalidateMapPartsData();
      showFeedback(container, "保存しました", "success");
    },

    onCreate: async () => {
      const defaultPayload = {
        viewType: 0, animeType: 0, level: 0, moveDirection: "",
        viewPositionX: 0, viewPositionY: 0,
        grpIdBase: 0, grpIdPile: 0,
        block: false, pile: false, pileBack: false, fishing: false,
        drawLast: false, counter: false,
        blockUp: false, blockDown: false, blockLeft: false, blockRight: false,
        animeFrames: [],
      };
      const { response, data } = await fetchJson("/api/maps/parts", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(defaultPayload),
      });
      if (!response.ok) {
        const msg = data?.error ?? `HTTP ${response.status}`;
        showFeedback(container, `新規追加に失敗しました: ${msg}`, "error");
        throw new Error(msg);
      }
      invalidateMapPartsData();
      showFeedback(container, `パーツ ${data?.partsId ?? ""} を追加しました`, "success");
      return data?.partsId ?? null;
    },

    onDelete: async (part) => {
      const { response, data } = await fetchJson("/api/maps/parts", {
        method: "DELETE",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ partsId: part.partsId }),
      });
      if (!response.ok) {
        const msg = data?.error ?? `HTTP ${response.status}`;
        showFeedback(container, `削除に失敗しました: ${msg}`, "error");
        throw new Error(msg);
      }
      invalidateMapPartsData();
      showFeedback(container, `パーツ ${part.partsId} を削除しました`, "success");
    },
  });

  _destroyFn = () => { container.innerHTML = ""; _ui = null; };
}

// ----------------------------------------------------------------
// openPartsDetail — 外部から詳細フォームをモーダルで開く API
// ----------------------------------------------------------------

/**
 * マップパーツ編集の詳細フォームを開く。
 * options.mountIn(HTMLElement)が指定された場合はその要素内に描画する(ビュー内パネル)。
 * 未指定の場合は従来どおり全画面オーバーレイで開く。
 * 保存 → { saved: true, partsId } / キャンセル → { saved: false } で resolve。
 *
 * @param {number} partsId
 * @param {{ mountIn?: HTMLElement }} [options]
 * @returns {Promise<{saved: boolean, partsId?: number}>}
 */
export async function openPartsDetail(partsId, options) {
  // パーツデータ取得
  const parts = await fetchPartsList();
  const part = parts.find((p) => p.partsId === partsId);
  if (!part) throw new Error(`パーツ ${partsId} が見つかりません`);

  const mountIn = options?.mountIn ?? null;

  return new Promise((resolve) => {
    // ---- コンテナ: mountIn 指定時はその要素内に flex 縦積みで描画 ----
    let panel;
    if (mountIn) {
      // mountIn 要素自体をパネルとして使う(position:absolute;inset:0 は呼び出し側が設定済み)
      panel = mountIn;
      panel.innerHTML = "";
      panel.style.cssText =
        "position:absolute;inset:0;display:flex;flex-direction:column;overflow:hidden;" +
        "background:var(--color-bg,#1a1a2e);";
    } else {
      // 全画面オーバーレイ(後方互換)
      panel = document.createElement("div");
      panel.style.cssText =
        "position:fixed;inset:0;z-index:9000;display:flex;flex-direction:column;" +
        "background:var(--color-bg,#1a1a2e);overflow:hidden;";
      document.body.appendChild(panel);
    }

    // ---- ツールバー ----
    const toolbar = document.createElement("div");
    toolbar.className = "ld-detail-toolbar";
    toolbar.style.cssText = "flex-shrink:0;";

    const btnCancel = document.createElement("button");
    btnCancel.type = "button"; btnCancel.className = "button"; btnCancel.textContent = "← キャンセル";

    const btnSave = document.createElement("button");
    btnSave.type = "button"; btnSave.className = "button primary"; btnSave.textContent = "保存";

    const dirtyBadge = document.createElement("span");
    dirtyBadge.className = "ld-dirty-badge"; dirtyBadge.textContent = "未保存";
    dirtyBadge.style.display = "none";

    toolbar.append(btnCancel, btnSave, dirtyBadge);

    // ---- フォーム本体 ----
    const body = document.createElement("div");
    body.className = "ld-detail-body";
    body.style.cssText = "flex:1;overflow-y:auto;padding:12px;";

    const formEl = buildDetailForm(part);
    body.appendChild(formEl);

    body.addEventListener("input",  () => { dirtyBadge.style.display = ""; });
    body.addEventListener("change", () => { dirtyBadge.style.display = ""; });

    panel.append(toolbar, body);

    function close(result) {
      if (mountIn) {
        // mountIn 使用時は呼び出し側が hideDetail() でパネルを隠す
        panel.innerHTML = "";
      } else {
        panel.remove();
      }
      resolve(result);
    }

    btnCancel.addEventListener("click", () => {
      if (dirtyBadge.style.display !== "none" &&
          !confirm("未保存の変更があります。破棄して閉じますか?")) return;
      close({ saved: false });
    });

    btnSave.addEventListener("click", async () => {
      const payload = formEl._collectData();
      const { response, data } = await fetchJson("/api/maps/parts", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        alert("保存に失敗しました: " + (data?.error ?? `HTTP ${response.status}`));
        return;
      }
      invalidateMapPartsData();
      close({ saved: true, partsId });
    });
  });
}

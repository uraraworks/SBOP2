/**
 * views/map-shadow-edit.js
 * マップ影カタログ編集画面 (route: map-shadows)
 * 一覧は影スプライトのパレットグリッド表示。
 * クリックで詳細画面へ切替(list-detail 同等の操作感)。
 */

import { fetchJson } from "../core/api.js";
import { createDragList } from "../components/drag-list.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumbLazy } from "../components/sprite-thumb.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const VIEW_TYPE_LABELS = {
  0: "通常",
  1: "背景",
  2: "装飾",
  3: "特殊"
};

const ANIME_TYPE_LABELS = {
  0: "静止",
  1: "往復",
  2: "循環",
  3: "ランダム"
};

const TILE_SIZE = 40;

// ----------------------------------------------------------------
// API ヘルパ
// ----------------------------------------------------------------

async function fetchShadowList() {
  const { response, data } = await fetchJson("/api/maps/shadows");
  if (!response.ok || !Array.isArray(data?.shadows)) {
    throw new Error("影カタログの取得に失敗しました");
  }
  return data.shadows.slice().sort((a, b) => a.id - b.id);
}

// ----------------------------------------------------------------
// 詳細フォーム描画
// ----------------------------------------------------------------

function buildDetailForm(shadow) {
  const wrap = document.createElement("div");
  wrap.className = "map-shadow-edit-detail";

  // ---- グラフィック ----
  const grpSection = document.createElement("section");
  grpSection.className = "detail-section";
  const grpH3 = document.createElement("h3");
  grpH3.textContent = "グラフィック";
  grpSection.appendChild(grpH3);

  const sfGrp = createSpriteField({
    categoryKey: "mapShadow",
    value: Number(shadow.grpId ?? 0),
    label: "グラフィックID (grpId)",
  });
  grpSection.appendChild(sfGrp.el);
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
    if (Number(v) === Number(shadow.viewType ?? 0)) opt.selected = true;
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
    if (Number(v) === Number(shadow.animeType ?? 0)) opt.selected = true;
    animeTypeSelect.appendChild(opt);
  });
  animeTypeLabel.append(animeTypeSpan, animeTypeSelect);
  grid.appendChild(animeTypeLabel);

  // アニメコマ数
  const animeCountLabel = document.createElement("label");
  animeCountLabel.className = "form-field";
  const animeCountSpan = document.createElement("span");
  animeCountSpan.textContent = "アニメコマ数";
  const animeCountInput = document.createElement("input");
  animeCountInput.type = "number";
  animeCountInput.name = "animeCount";
  animeCountInput.min = "0"; animeCountInput.max = "255";
  animeCountInput.value = String(shadow.animeCount ?? 0);
  animeCountLabel.append(animeCountSpan, animeCountInput);
  grid.appendChild(animeCountLabel);

  // 透明度
  const levelLabel = document.createElement("label");
  levelLabel.className = "form-field";
  const levelSpan = document.createElement("span");
  levelSpan.textContent = "透明度";
  const levelInput = document.createElement("input");
  levelInput.type = "range"; levelInput.name = "level";
  levelInput.min = "0"; levelInput.max = "255";
  levelInput.value = String(shadow.level ?? 0);
  const levelValue = document.createElement("span");
  levelValue.className = "range-value";
  levelValue.textContent = String(shadow.level ?? 0);
  levelInput.addEventListener("input", () => { levelValue.textContent = levelInput.value; });
  levelLabel.append(levelSpan, levelInput, levelValue);
  grid.appendChild(levelLabel);

  // 明度モード
  const lightLabel = document.createElement("label");
  lightLabel.className = "form-field";
  const lightCb = document.createElement("input");
  lightCb.type = "checkbox"; lightCb.name = "light"; lightCb.checked = Boolean(shadow.light);
  const lightSpan = document.createElement("span");
  lightSpan.textContent = " 透明度を明度として使う";
  lightLabel.append(lightCb, lightSpan);
  grid.appendChild(lightLabel);

  // 表示位置 X/Y
  const posXLabel = document.createElement("label");
  posXLabel.className = "form-field coordinate";
  const posXSpan = document.createElement("span");
  posXSpan.textContent = "表示位置 X";
  const posXInput = document.createElement("input");
  posXInput.type = "number"; posXInput.name = "viewPosX";
  posXInput.value = String(shadow.viewPos?.x ?? 0);
  posXLabel.append(posXSpan, posXInput);
  grid.appendChild(posXLabel);

  const posYLabel = document.createElement("label");
  posYLabel.className = "form-field coordinate";
  const posYSpan = document.createElement("span");
  posYSpan.textContent = "表示位置 Y";
  const posYInput = document.createElement("input");
  posYInput.type = "number"; posYInput.name = "viewPosY";
  posYInput.value = String(shadow.viewPos?.y ?? 0);
  posYLabel.append(posYSpan, posYInput);
  grid.appendChild(posYLabel);

  paramSection.appendChild(grid);
  wrap.appendChild(paramSection);

  // ---- アニメーションフレーム ----
  const animeSection = document.createElement("section");
  animeSection.className = "detail-section";
  const animeH3 = document.createElement("h3");
  animeH3.textContent = "アニメーションフレーム";
  animeSection.appendChild(animeH3);

  let _frames = Array.isArray(shadow.animeFrames) ? shadow.animeFrames.slice() : [];

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

  const preview = createAnimePreview({ width: 32, height: 32, scale: 3 });
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
        layers: [{ categoryKey: "mapShadow", sub: sfGrp.getValue() }],
      }]);
    } else {
      preview.setFrames(frames.map((f) => ({
        wait: f.wait,
        layers: [
          { categoryKey: "mapShadow", sub: f.grpIdBase, alpha: f.level / 255 },
          { categoryKey: "mapShadow", sub: f.grpIdPile },
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
      id: shadow.id,
      grpId: sfGrp.getValue(),
      viewType: Number(viewTypeSelect.value),
      animeType: Number(animeTypeSelect.value),
      animeCount: Number(animeCountInput.value),
      level: Number(levelInput.value),
      light: lightCb.checked,
      viewPos: {
        x: Number(posXInput.value),
        y: Number(posYInput.value),
      },
      animeFrames: collectFrames(),
    };
  };

  return wrap;
}

// ----------------------------------------------------------------
// フィードバック表示
// ----------------------------------------------------------------

function showFeedback(container, message, type) {
  let fb = container.querySelector(".ms-feedback");
  if (!fb) {
    fb = document.createElement("p");
    fb.className = "ms-feedback result-message";
    fb.setAttribute("role", "status");
    fb.setAttribute("aria-live", "polite");
    container.prepend(fb);
  }
  fb.textContent = message;
  fb.className = "ms-feedback result-message";
  if (type) fb.classList.add(type);
}

// ----------------------------------------------------------------
// パレットグリッド付き 一覧⇔詳細 UI
// ----------------------------------------------------------------

function buildShadowUI(container, opts) {
  const { fetchList: _fetchList, onSave: _onSave, onCreate: _onCreate, onDelete: _onDelete } = opts;

  container.innerHTML = "";

  let _allItems = [];
  let _selectedItem = null;
  let _dirty = false;

  const shell = document.createElement("div");
  shell.className = "list-detail-shell";

  // === 一覧ペイン ===
  const listPane = document.createElement("div");
  listPane.className = "ld-list-pane";

  const listHeader = document.createElement("div");
  listHeader.className = "ld-list-header";
  const listTitle = document.createElement("div");
  listTitle.className = "ld-list-pane-title";
  listTitle.textContent = "影カタログ一覧";
  listHeader.appendChild(listTitle);

  const paletteGrid = document.createElement("div");
  paletteGrid.className = "parts-palette-grid";
  paletteGrid.style.cssText =
    "display:grid;grid-template-columns:repeat(auto-fill," + TILE_SIZE + "px);" +
    "gap:4px;padding:8px;overflow-y:auto;flex:1;";

  const listFooter = document.createElement("div");
  listFooter.className = "ld-list-footer";
  const btnNew = document.createElement("button");
  btnNew.type = "button"; btnNew.className = "button"; btnNew.textContent = "+ 新規";
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
      cell.title = `影 ${item.id} (grp:${item.grpId})`;
      if (item === _selectedItem) {
        cell.style.borderColor = "var(--color-accent, #4e9af1)";
        cell.style.background = "var(--color-accent-subtle, rgba(78,154,241,0.15))";
      }

      const thumb = createSpriteThumbLazy({ categoryKey: "mapShadow", sub: Number(item.grpId ?? 0), size: TILE_SIZE });
      thumb.el.style.cssText = "width:" + TILE_SIZE + "px;height:" + TILE_SIZE + "px;";

      const idLabel = document.createElement("span");
      idLabel.textContent = item.id;
      idLabel.style.cssText = "font-size:10px;line-height:14px;color:var(--color-muted,#888);";

      cell.append(thumb.el, idLabel);
      cell.addEventListener("click", () => selectItem(item));
      paletteGrid.appendChild(cell);
    });
  }

  function showDetailPane() { listPane.style.display = "none"; detailPane.style.display = ""; }
  function showListPane()   { detailPane.style.display = "none"; listPane.style.display = ""; }

  function renderDetail(item) {
    detailBody.innerHTML = "";
    if (!item) return;
    detailBody.appendChild(buildDetailForm(item));
  }

  function selectItem(item) {
    if (_dirty && !confirm("未保存の変更があります。破棄して切り替えますか?")) return;
    _selectedItem = item;
    setDirty(false);
    renderDetail(item);
    renderPalette();
    showDetailPane();
  }

  btnBack.addEventListener("click", () => {
    if (_dirty && !confirm("未保存の変更があります。破棄して一覧に戻りますか?")) return;
    _selectedItem = null;
    setDirty(false);
    renderPalette();
    showListPane();
  });

  btnSave.addEventListener("click", async () => {
    if (!_selectedItem) return;
    const detailEl = detailBody.querySelector(".map-shadow-edit-detail");
    if (!detailEl?._collectData) { alert("フォームが見つかりません"); return; }
    try {
      await _onSave(_selectedItem, detailEl._collectData());
      setDirty(false);
      const savedId = _selectedItem.id;
      await load();
      const found = _allItems.find((i) => i.id === savedId) ?? null;
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
        const found = _allItems.find((i) => i.id === newId) ?? _allItems[_allItems.length - 1];
        if (found) selectItem(found);
      }
    } catch (err) {
      alert("新規作成に失敗しました: " + String(err?.message ?? err));
    }
  });

  async function load() {
    try {
      _allItems = await _fetchList();
    } catch (e) {
      _allItems = [];
      console.error("影一覧取得失敗:", e);
    }
    renderPalette();
  }

  load();
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  buildShadowUI(container, {
    fetchList: fetchShadowList,

    onSave: async (shadow, payload) => {
      const { response, data } = await fetchJson("/api/maps/shadows", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        const msg = data?.error ?? `HTTP ${response.status}`;
        showFeedback(container, `保存に失敗しました: ${msg}`, "error");
        throw new Error(msg);
      }
      showFeedback(container, "保存しました", "success");
    },

    onCreate: async () => {
      const defaultPayload = {
        grpId: 0, viewType: 0, animeType: 0, animeCount: 0,
        level: 0, light: false,
        viewPos: { x: 0, y: 0 },
        animeFrames: [],
      };
      const { response, data } = await fetchJson("/api/maps/shadows", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(defaultPayload),
      });
      if (!response.ok) {
        const msg = data?.error ?? `HTTP ${response.status}`;
        showFeedback(container, `新規追加に失敗しました: ${msg}`, "error");
        throw new Error(msg);
      }
      showFeedback(container, `影 ${data?.id ?? ""} を追加しました`, "success");
      return data?.id ?? null;
    },

    onDelete: async (shadow) => {
      const { response, data } = await fetchJson(`/api/maps/shadows?id=${shadow.id}`, {
        method: "DELETE",
      });
      if (!response.ok) {
        const msg = data?.error ?? `HTTP ${response.status}`;
        showFeedback(container, `削除に失敗しました: ${msg}`, "error");
        throw new Error(msg);
      }
      showFeedback(container, `影 ${shadow.id} を削除しました`, "success");
    },
  });

  _destroyFn = () => { container.innerHTML = ""; };
}

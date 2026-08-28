/**
 * components/sprite-paint.js
 * スプライトシートの簡易ペイント (S4)
 *
 * 設計の前提:
 *   - 素材は 8bit パレット PNG。ゲーム側 (Read256) は非パレット PNG を読まないため、
 *     編集も保存もインデックス値のまま行い、パレットは一切いじらない。
 *     色は「パレットにある色から選ぶ」方式で、新しい色は作れない。
 *   - 消しゴムはインデックス 0（＝透過）で塗ることと同義。
 *   - 編集単位は 1 セル。シート全体をキャンバスに載せず、選択セルだけを拡大表示する。
 *     保存時に元のインデックス列へ書き戻してシート 1 枚として PUT する。
 *
 * 下敷き（オニオンスキン）:
 *   2x2 キャラ系カテゴリのときだけ、編集セルに対応する他レイヤーを
 *   薄く敷いて位置合わせできるようにする。座標計算は char-composer と共有する。
 */

import { decodeIndexedPng, encodeIndexedPng, indicesToImageData } from "../lib/indexed-png.js";
import { CELL, COMPOSABLE_KEYS, EYE_ORIGIN_OPTS, frameCellOrigin } from "./char-composer.js";

const MAX_UPLOAD_BYTES = 2 * 1024 * 1024;
const UNDO_LIMIT = 50;

const TOOLS = [
  { id: "pen",     label: "ペン" },
  { id: "eraser",  label: "消しゴム" },
  { id: "picker",  label: "スポイト" },
  { id: "fill",    label: "塗りつぶし" },
];

// 下敷きに使うレイヤー。描画順は char-composer と同じ（体→服→髪→目）。
const UNDERLAY_LAYERS = [
  { slot: "body",  key: "char2x2",  label: "体" },
  { slot: "cloth", key: "cloth2x2", label: "服" },
  { slot: "hair",  key: "hair2x2",  label: "髪" },
  { slot: "eye",   key: "eye2x2",   label: "目", originOpts: EYE_ORIGIN_OPTS },
];

// 編集中カテゴリが占めるスロット（そのレイヤーは下敷きから除く）
const SLOT_OF_KEY = {
  char2x2: "body",
  cloth2x2: "cloth",
  spCloth2x2: "cloth",
  spHair2x2: "cloth",
  hair2x2: "hair",
  eye2x2: "eye",
};

// ----------------------------------------------------------------
// 小物
// ----------------------------------------------------------------

function el(tag, className, text) {
  const node = document.createElement(tag);
  if (className) node.className = className;
  if (text != null) node.textContent = text;
  return node;
}

function labeled(text, control) {
  const label = el("label", "cc-field");
  label.append(el("span", null, text), control);
  return label;
}

function makeSelect() {
  return document.createElement("select");
}

function fillOptions(select, count, labeller) {
  const prev = select.value;
  select.innerHTML = "";
  for (let i = 0; i < count; i++) {
    const opt = document.createElement("option");
    opt.value = String(i);
    opt.textContent = labeller ? labeller(i) : String(i);
    select.appendChild(opt);
  }
  if (prev !== "" && Number(prev) < count) select.value = prev;
}

// ----------------------------------------------------------------
// 本体
// ----------------------------------------------------------------

/**
 * @param {object}   opts
 * @param {Array}    opts.categories  /api/image-categories の categories
 * @param {Function} opts.onSaved     保存成功時に呼ばれる（親が履歴/プレビューを更新する）
 * @param {Function} opts.onFeedback  (message, type) 形式の通知
 */
export function createSpritePaint({ categories, onSaved, onFeedback }) {
  const catByKey = new Map((categories ?? []).map((c) => [c.key, c]));
  const sheetCountOf = (key) => Number(catByKey.get(key)?.sheetCount ?? 0);

  let _cat = null;
  let _index = 0;
  let _sheet = null;        // { width, height, indices, palette }
  let _cellSize = CELL;
  let _cols = 0, _rows = 0;
  let _col = 0, _row = 0;
  let _color = 1;           // 選択中のパレットインデックス
  let _tool = "pen";
  let _zoom = 12;
  let _dirty = false;
  let _painting = false;
  let _loadSeq = 0;

  const undoStack = [];
  const redoStack = [];

  const underlayImages = new Map(); // "key/index" -> HTMLImageElement | null

  const section = el("section", "detail-section sp-section");
  section.appendChild(el("h3", null, "ペイント"));

  const desc = el("p", "card-description",
    "パレットの色だけで 1 セルずつ編集します。新しい色は作れません（ゲームがパレット PNG しか読めないため）。");
  section.appendChild(desc);

  // --- ツールバー: セル選択 ---
  const cellBar = el("div", "cc-toolbar");
  const colSelect = makeSelect();
  const rowSelect = makeSelect();
  colSelect.addEventListener("change", () => selectCell(Number(colSelect.value) || 0, _row));
  rowSelect.addEventListener("change", () => selectCell(_col, Number(rowSelect.value) || 0));
  const cellInfo = el("span", "sp-cell-info");
  cellBar.append(labeled("列", colSelect), labeled("行", rowSelect), cellInfo);
  section.appendChild(cellBar);

  // --- ツールバー: 描画ツール ---
  const toolBar = el("div", "cc-toolbar");
  const toolButtons = new Map();
  TOOLS.forEach((t) => {
    const btn = el("button", "button small", t.label);
    btn.type = "button";
    btn.addEventListener("click", () => setTool(t.id));
    toolButtons.set(t.id, btn);
    toolBar.appendChild(btn);
  });

  const undoBtn = el("button", "button small", "元に戻す");
  undoBtn.type = "button";
  undoBtn.addEventListener("click", undo);
  const redoBtn = el("button", "button small", "やり直す");
  redoBtn.type = "button";
  redoBtn.addEventListener("click", redo);

  const zoomSelect = makeSelect();
  [8, 12, 16, 24].forEach((z) => {
    const opt = document.createElement("option");
    opt.value = String(z);
    opt.textContent = `${z}x`;
    if (z === _zoom) opt.selected = true;
    zoomSelect.appendChild(opt);
  });
  zoomSelect.addEventListener("change", () => {
    _zoom = Number(zoomSelect.value) || 12;
    applyCanvasSize();
    render();
  });

  toolBar.append(undoBtn, redoBtn, labeled("拡大率", zoomSelect));
  section.appendChild(toolBar);

  // --- ツールバー: 下敷き ---
  const underlayBar = el("div", "cc-toolbar sp-underlay-bar");
  const underlayCb = document.createElement("input");
  underlayCb.type = "checkbox";
  underlayCb.checked = true;
  underlayCb.addEventListener("change", render);
  const underlayLabel = el("label", "cc-check");
  underlayLabel.append(underlayCb, " 下敷き表示");

  const underlayAlpha = document.createElement("input");
  underlayAlpha.type = "range";
  underlayAlpha.min = "10";
  underlayAlpha.max = "100";
  underlayAlpha.value = "45";
  underlayAlpha.addEventListener("input", render);

  const sexSelect = makeSelect();
  [["男", 0], ["女", 1]].forEach(([label, value]) => {
    const opt = document.createElement("option");
    opt.value = String(value);
    opt.textContent = label;
    sexSelect.appendChild(opt);
  });
  sexSelect.addEventListener("change", () => { void reloadUnderlay(); });
  const sexField = labeled("性別", sexSelect);

  underlayBar.append(underlayLabel, labeled("濃さ", underlayAlpha), sexField);
  section.appendChild(underlayBar);

  // --- キャンバス ---
  const stage = el("div", "ie-preview-stage sp-stage");
  const canvas = document.createElement("canvas");
  canvas.className = "sp-canvas";
  stage.appendChild(canvas);
  section.appendChild(stage);

  // --- パレット ---
  const paletteWrap = el("div", "sp-palette");
  section.appendChild(paletteWrap);
  const colorInfo = el("p", "ie-dim-info");
  section.appendChild(colorInfo);

  // --- 保存 ---
  const saveBar = el("div", "cc-toolbar");
  const saveBtn = el("button", "button primary", "この内容で保存");
  saveBtn.type = "button";
  saveBtn.addEventListener("click", () => { void save(); });
  const discardBtn = el("button", "button small", "編集を破棄");
  discardBtn.type = "button";
  discardBtn.addEventListener("click", () => { void load({ force: true }); });
  const dirtyMark = el("span", "sp-dirty");
  saveBar.append(saveBtn, discardBtn, dirtyMark);
  section.appendChild(saveBar);

  // ----------------------------------------------------------------
  // 表示
  // ----------------------------------------------------------------

  function applyCanvasSize() {
    canvas.width = _cellSize;
    canvas.height = _cellSize;
    canvas.style.width = `${_cellSize * _zoom}px`;
    canvas.style.height = `${_cellSize * _zoom}px`;
  }

  function cellOrigin() {
    return { x: _col * _cellSize, y: _row * _cellSize };
  }

  // 編集セルから「フレーム番号と性別」を復元する。下敷きの座標計算に使う。
  function underlayContext() {
    if (_cat?.key === "eye2x2") {
      // 目シートは x が 4 セル分ずれているので frame を戻す。性別は判断材料が
      // 無いため（目シートに男女の別が無い）ユーザーの指定を使う。
      return { frame: _row * 16 + _col + 4, sex: Number(sexSelect.value) || 0 };
    }
    return { frame: (_row % 4) * 16 + _col, sex: _row >= 4 ? 1 : 0 };
  }

  function drawUnderlay(ctx) {
    if (!underlayCb.checked) return;
    if (!_cat || !COMPOSABLE_KEYS.has(_cat.key) || _cat.key === "npc2x2") return;

    const editedSlot = SLOT_OF_KEY[_cat.key];
    const { frame, sex } = underlayContext();

    ctx.save();
    ctx.globalAlpha = (Number(underlayAlpha.value) || 45) / 100;
    for (const layer of UNDERLAY_LAYERS) {
      if (layer.slot === editedSlot) continue;
      const img = underlayImages.get(`${layer.key}/0`);
      if (!img) continue;
      const { sx, sy } = frameCellOrigin(frame, sex, layer.originOpts);
      if (sx < 0 || sy < 0) continue;
      if (sx + CELL > img.naturalWidth || sy + CELL > img.naturalHeight) continue;
      ctx.drawImage(img, sx, sy, CELL, CELL, 0, 0, CELL, CELL);
    }
    ctx.restore();
  }

  function render() {
    if (!_sheet) return;
    const ctx = canvas.getContext("2d");
    ctx.imageSmoothingEnabled = false;
    ctx.clearRect(0, 0, _cellSize, _cellSize);

    drawUnderlay(ctx);

    // 編集セルのインデックス列を切り出して描く
    const { x, y } = cellOrigin();
    const cellIdx = new Uint8Array(_cellSize * _cellSize);
    for (let row = 0; row < _cellSize; row++) {
      const src = (y + row) * _sheet.width + x;
      cellIdx.set(_sheet.indices.subarray(src, src + _cellSize), row * _cellSize);
    }
    const imageData = indicesToImageData(cellIdx, _cellSize, _cellSize, _sheet.palette);

    // ImageData は直接置くと下敷きを消してしまうので、一度別 canvas に載せて合成する
    const tmp = document.createElement("canvas");
    tmp.width = _cellSize;
    tmp.height = _cellSize;
    tmp.getContext("2d").putImageData(imageData, 0, 0);
    ctx.drawImage(tmp, 0, 0);

    dirtyMark.textContent = _dirty ? "未保存の変更があります" : "";
    dirtyMark.className = "sp-dirty" + (_dirty ? " is-dirty" : "");
    undoBtn.disabled = undoStack.length === 0;
    redoBtn.disabled = redoStack.length === 0;
    cellInfo.textContent = `セル (${_col}, ${_row}) / シート ${_sheet.width}x${_sheet.height} = ${_cols}x${_rows} セル`;
  }

  function renderPalette() {
    paletteWrap.innerHTML = "";
    if (!_sheet) return;
    _sheet.palette.forEach((c, i) => {
      const sw = el("button", "sp-swatch");
      sw.type = "button";
      sw.title = i === 0
        ? "0: 透過"
        : `${i}: rgb(${c.r}, ${c.g}, ${c.b})${(c.a ?? 255) !== 255 ? ` a=${c.a}` : ""}`;
      if ((c.a ?? 255) === 0) {
        sw.classList.add("is-transparent");
      } else {
        sw.style.background = `rgb(${c.r}, ${c.g}, ${c.b})`;
      }
      if (i === _color) sw.classList.add("is-selected");
      sw.addEventListener("click", () => setColor(i));
      paletteWrap.appendChild(sw);
    });
    const c = _sheet.palette[_color];
    colorInfo.textContent = c
      ? `選択色: インデックス ${_color}` + (_color === 0 ? "（透過）" : ` rgb(${c.r}, ${c.g}, ${c.b})`)
      : "";
  }

  function setColor(i) {
    _color = i;
    renderPalette();
  }

  function setTool(id) {
    _tool = id;
    toolButtons.forEach((btn, key) => {
      btn.classList.toggle("is-selected", key === id);
    });
  }

  function selectCell(col, row) {
    _col = Math.max(0, Math.min(_cols - 1, col));
    _row = Math.max(0, Math.min(_rows - 1, row));
    colSelect.value = String(_col);
    rowSelect.value = String(_row);
    // セルを跨いだ Undo は混乱のもとなので履歴はセル単位で捨てる
    undoStack.length = 0;
    redoStack.length = 0;
    render();
  }

  // ----------------------------------------------------------------
  // 編集
  // ----------------------------------------------------------------

  function snapshotCell() {
    const { x, y } = cellOrigin();
    const snap = new Uint8Array(_cellSize * _cellSize);
    for (let row = 0; row < _cellSize; row++) {
      const src = (y + row) * _sheet.width + x;
      snap.set(_sheet.indices.subarray(src, src + _cellSize), row * _cellSize);
    }
    return snap;
  }

  function restoreCell(snap) {
    const { x, y } = cellOrigin();
    for (let row = 0; row < _cellSize; row++) {
      _sheet.indices.set(snap.subarray(row * _cellSize, (row + 1) * _cellSize),
                          (y + row) * _sheet.width + x);
    }
  }

  function pushUndo() {
    undoStack.push(snapshotCell());
    if (undoStack.length > UNDO_LIMIT) undoStack.shift();
    redoStack.length = 0;
  }

  function undo() {
    if (!undoStack.length) return;
    redoStack.push(snapshotCell());
    restoreCell(undoStack.pop());
    _dirty = true;
    render();
  }

  function redo() {
    if (!redoStack.length) return;
    undoStack.push(snapshotCell());
    restoreCell(redoStack.pop());
    _dirty = true;
    render();
  }

  function pixelAt(px, py) {
    const { x, y } = cellOrigin();
    return _sheet.indices[(y + py) * _sheet.width + (x + px)];
  }

  function setPixel(px, py, value) {
    const { x, y } = cellOrigin();
    const at = (y + py) * _sheet.width + (x + px);
    if (_sheet.indices[at] === value) return false;
    _sheet.indices[at] = value;
    return true;
  }

  function floodFill(px, py, value) {
    const target = pixelAt(px, py);
    if (target === value) return false;
    const stack = [[px, py]];
    let changed = false;
    while (stack.length) {
      const [cx, cy] = stack.pop();
      if (cx < 0 || cy < 0 || cx >= _cellSize || cy >= _cellSize) continue;
      if (pixelAt(cx, cy) !== target) continue;
      setPixel(cx, cy, value);
      changed = true;
      stack.push([cx + 1, cy], [cx - 1, cy], [cx, cy + 1], [cx, cy - 1]);
    }
    return changed;
  }

  // canvas 上の座標をセル内ピクセルへ
  function toPixel(event) {
    const rect = canvas.getBoundingClientRect();
    const px = Math.floor((event.clientX - rect.left) / rect.width * _cellSize);
    const py = Math.floor((event.clientY - rect.top) / rect.height * _cellSize);
    if (px < 0 || py < 0 || px >= _cellSize || py >= _cellSize) return null;
    return { px, py };
  }

  function applyTool(px, py) {
    if (_tool === "picker") {
      setColor(pixelAt(px, py));
      return false;
    }
    if (_tool === "fill")   return floodFill(px, py, _color);
    if (_tool === "eraser") return setPixel(px, py, 0);
    return setPixel(px, py, _color);
  }

  canvas.addEventListener("pointerdown", (e) => {
    if (!_sheet) return;
    const pos = toPixel(e);
    if (!pos) return;
    // スポイトは履歴を汚さない
    if (_tool !== "picker") pushUndo();
    _painting = _tool === "pen" || _tool === "eraser";
    if (_painting) canvas.setPointerCapture(e.pointerId);
    if (applyTool(pos.px, pos.py)) { _dirty = true; }
    render();
  });

  canvas.addEventListener("pointermove", (e) => {
    if (!_painting || !_sheet) return;
    const pos = toPixel(e);
    if (!pos) return;
    if (applyTool(pos.px, pos.py)) { _dirty = true; render(); }
  });

  const endPaint = () => { _painting = false; };
  canvas.addEventListener("pointerup", endPaint);
  canvas.addEventListener("pointercancel", endPaint);

  // ----------------------------------------------------------------
  // 読み込み・保存
  // ----------------------------------------------------------------

  function loadImage(url) {
    return new Promise((resolve) => {
      const img = new Image();
      img.onload = () => resolve(img);
      img.onerror = () => resolve(null);
      img.src = url;
    });
  }

  async function reloadUnderlay() {
    underlayImages.clear();
    if (!_cat || !COMPOSABLE_KEYS.has(_cat.key) || _cat.key === "npc2x2") { render(); return; }
    const editedSlot = SLOT_OF_KEY[_cat.key];
    const stamp = Date.now();
    await Promise.all(UNDERLAY_LAYERS.map(async (layer) => {
      if (layer.slot === editedSlot) return;
      if (sheetCountOf(layer.key) <= 0) return;
      const img = await loadImage(`/api/assets/sprites/${encodeURIComponent(layer.key)}/0?v=${stamp}`);
      underlayImages.set(`${layer.key}/0`, img);
    }));
    render();
  }

  async function load({ force = false } = {}) {
    if (!_cat) return;
    if (!force && _dirty) return;
    const seq = ++_loadSeq;

    _sheet = null;
    undoStack.length = 0;
    redoStack.length = 0;
    _dirty = false;

    try {
      const res = await fetch(
        `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}?v=${Date.now()}`,
        { credentials: "include" }
      );
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      const buf = await res.arrayBuffer();
      const decoded = await decodeIndexedPng(buf);
      if (seq !== _loadSeq) return;

      _sheet = decoded;
      _cellSize = Number(_cat.cellSize) || CELL;
      _cols = Math.max(1, Math.floor(decoded.width / _cellSize));
      _rows = Math.max(1, Math.floor(decoded.height / _cellSize));
      _col = Math.min(_col, _cols - 1);
      _row = Math.min(_row, _rows - 1);

      fillOptions(colSelect, _cols);
      fillOptions(rowSelect, _rows);
      colSelect.value = String(_col);
      rowSelect.value = String(_row);

      if (_color >= decoded.palette.length) _color = 1;
      applyCanvasSize();
      renderPalette();
      render();
      setSectionEnabled(true);
      await reloadUnderlay();
    } catch (e) {
      if (seq !== _loadSeq) return;
      setSectionEnabled(false, e?.message);
    }
  }

  function setSectionEnabled(enabled, reason) {
    stage.style.display = enabled ? "" : "none";
    paletteWrap.style.display = enabled ? "" : "none";
    saveBar.style.display = enabled ? "" : "none";
    toolBar.style.display = enabled ? "" : "none";
    cellBar.style.display = enabled ? "" : "none";
    underlayBar.style.display =
      enabled && _cat && COMPOSABLE_KEYS.has(_cat.key) && _cat.key !== "npc2x2" ? "" : "none";
    sexField.style.display = _cat?.key === "eye2x2" ? "" : "none";
    desc.textContent = enabled
      ? "パレットの色だけで 1 セルずつ編集します。新しい色は作れません（ゲームがパレット PNG しか読めないため）。"
      : `このシートはペイントできません（${reason || "パレット PNG ではありません"}）。差し替えは上のアップロードから行えます。`;
  }

  async function save() {
    if (!_sheet || !_cat) return;
    saveBtn.disabled = true;
    try {
      const png = await encodeIndexedPng(_sheet);
      if (png.length > MAX_UPLOAD_BYTES) {
        onFeedback?.(`保存後のサイズが 2MB を超えました (${(png.length / 1024).toFixed(0)}KB)`, "error");
        return;
      }
      const response = await fetch(
        `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}`,
        {
          method: "PUT",
          credentials: "include",
          headers: { "Content-Type": "image/png" },
          body: png,
        }
      );
      if (!response.ok) {
        let data = null;
        try { data = await response.json(); } catch { data = null; }
        onFeedback?.(`保存に失敗しました: ${data?.error || `HTTP ${response.status}`}`, "error");
        return;
      }
      _dirty = false;
      render();
      onFeedback?.("ペイントした内容を保存しました", "success");
      onSaved?.();
    } catch (e) {
      onFeedback?.("保存に失敗しました: " + String(e?.message ?? e), "error");
    } finally {
      saveBtn.disabled = false;
    }
  }

  /**
   * 編集対象を切り替える。未保存の変更があれば確認する。
   */
  function setTarget(cat, index) {
    // 断られた場合は編集中のシートを開いたままにする（作業を失わせない）。
    // 画面の他の部分は既に切り替わっているので、その旨を通知する。
    if (_dirty && !confirm("未保存のペイント内容があります。破棄して切り替えますか?")) {
      onFeedback?.(
        `ペイントは ${_cat?.label || _cat?.key} #${_index} を編集したままです。保存するか「編集を破棄」を押してください`,
        "warning"
      );
      return false;
    }
    _cat = cat;
    _index = index;
    _dirty = false;
    setTool(_tool);
    void load({ force: true });
    return true;
  }

  /** 親側で差し替え/復元が起きた時に読み直す */
  function refresh() {
    void load({ force: true });
  }

  function hasUnsavedChanges() {
    return _dirty;
  }

  setTool("pen");

  return { el: section, setTarget, refresh, hasUnsavedChanges };
}

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
import { fetchJson } from "../core/api.js";
import { CELL, COMPOSABLE_KEYS, EYE_ORIGIN_OPTS, frameCellOrigin } from "./char-composer.js";
import { icon } from "./icons.js";
import { attachTip } from "./tooltip.js";

const MAX_UPLOAD_BYTES = 2 * 1024 * 1024;
const UNDO_LIMIT = 50;

// key: デザインツール風のワンキーショートカット（handleShortcut() で解釈する）
const TOOLS = [
  { id: "pen",     label: "ペン",       iconName: "pen",    key: "B" },
  { id: "eraser",  label: "消しゴム",   iconName: "eraser", key: "E" },
  { id: "picker",  label: "スポイト",   iconName: "picker", key: "I" },
  { id: "fill",    label: "塗りつぶし", iconName: "fill",   key: "G" },
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

// Unix epoch 秒 → ローカル時刻。image-editor 側と同じ書式にそろえる
function formatEpoch(sec) {
  if (sec == null || !Number.isFinite(Number(sec))) return "-";
  const d = new Date(Number(sec) * 1000);
  if (Number.isNaN(d.getTime())) return "-";
  const pad = (n) => String(n).padStart(2, "0");
  return `${d.getFullYear()}/${pad(d.getMonth() + 1)}/${pad(d.getDate())} ` +
    `${pad(d.getHours())}:${pad(d.getMinutes())}`;
}

// 環境によっては confirm() / prompt() が使えない（別ウィンドウや埋め込み表示など）。
// 名前入力はインラインの入力欄に、削除は 2 段階クリックに置き換えてあるので、
// ここで確認できないのは「未保存の変更を捨てるか」の確認だけ。
// 使えない環境では操作を止めない（＝続行）方に倒す。
function askConfirm(message) {
  try {
    return window.confirm(message);
  } catch {
    return true;
  }
}

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

// アイコンのみのボタン。文字ラベルは出さず、ツールチップと aria-label で補う。
function iconOnlyButton(className, iconName, tipText) {
  const btn = el("button", className);
  btn.type = "button";
  btn.classList.add("icon-only");
  btn.appendChild(icon(iconName));
  attachTip(btn, tipText);
  return btn;
}

// アイコン + 文字ラベルの両方を持つボタン。
function iconLabelButton(className, iconName, labelText) {
  const btn = el("button", className);
  btn.type = "button";
  btn.appendChild(icon(iconName));
  btn.appendChild(el("span", null, labelText));
  return btn;
}

// セルサイズに応じた既定の拡大率。32px セルをそのまま 12x にすると中央カラムに
// 収まりきらないため、セルが大きいほど控えめにする。
function defaultZoomFor(cellSize) {
  if (cellSize <= 16) return 16;
  if (cellSize === 24) return 12;
  return 8; // 32px 以上
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
 * @param {Function} [opts.onRequestTarget] (catKey, sheetIndex) 途中セーブを開く時に
 *                                       親へカテゴリ切り替えを依頼する。
 * @param {Function} [opts.onCellChange] (col, row) 編集セルが変わった時。
 *                                       プレビュー側の選択枠を追従させるのに使う。
 * @param {object}   opts.context      createCharContext() が返す共有ストア。
 *                                     下敷きの性別/服/髪/目 はここから読む。
 */
export function createSpritePaint({ categories, onSaved, onFeedback, onCellChange, onRequestTarget, context }) {
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
  let _zoomUserSet = false; // 拡大率セレクトをユーザーが触ったら、以後は既定値で上書きしない
  let _dirty = false;
  let _painting = false;
  let _loadSeq = 0;

  const undoStack = [];
  const redoStack = [];

  // 開こうとしている途中セーブ。シートの読み込み完了後に貼り込む。
  let _pendingDraft = null;

  const underlayImages = new Map(); // "key/index" -> HTMLImageElement | null

  const section = el("section", "detail-section sp-section");
  section.appendChild(el("h3", null, "ペイント"));

  const DESC_TEXT =
    "パレットの色だけで 1 セルずつ編集します（新しい色は作れません）。" +
    "プレビューをクリックすると編集するセルを選べます。右クリックでその場の色を拾えます。";
  const desc = el("p", "card-description", DESC_TEXT);
  section.appendChild(desc);

  // --- ツールバー: セル選択 ---
  const cellBar = el("div", "cc-toolbar");
  const colSelect = makeSelect();
  const rowSelect = makeSelect();
  colSelect.addEventListener("change", () => selectCell(Number(colSelect.value) || 0, _row));
  rowSelect.addEventListener("change", () => selectCell(_col, Number(rowSelect.value) || 0));
  // セル位置の表示は中央ヘッダのパンくず(image-editor 側)に統合したので、
  // ここでは持たない。代わりに onCellChange で寸法情報ごと親へ伝える。
  cellBar.append(labeled("列", colSelect), labeled("行", rowSelect));
  section.appendChild(cellBar);

  // --- ツールバー: 描画ツール ---
  const toolBar = el("div", "cc-toolbar");
  const toolButtons = new Map();
  TOOLS.forEach((t) => {
    const btn = iconOnlyButton("button small", t.iconName, `${t.label} (${t.key})`);
    btn.addEventListener("click", () => setTool(t.id));
    toolButtons.set(t.id, btn);
    toolBar.appendChild(btn);
  });

  const undoBtn = iconOnlyButton("button small", "undo", "元に戻す (Ctrl+Z)");
  undoBtn.addEventListener("click", undo);
  const redoBtn = iconOnlyButton("button small", "redo", "やり直す (Ctrl+Shift+Z)");
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
    _zoomUserSet = true;
    applyCanvasSize();
    render();
  });

  const zoomField = el("label", "cc-field");
  zoomField.append(icon("zoom"), el("span", null, "拡大率"), zoomSelect);

  toolBar.append(undoBtn, redoBtn, zoomField);
  section.appendChild(toolBar);

  // --- ツールバー: 下敷き ---
  const underlayBar = el("div", "cc-toolbar sp-underlay-bar");
  const underlayCb = document.createElement("input");
  underlayCb.type = "checkbox";
  underlayCb.checked = true;
  underlayCb.addEventListener("change", render);
  const underlayLabel = el("label", "cc-check");
  underlayLabel.append(underlayCb, icon("layers"), " 下敷き表示");

  const underlayAlpha = document.createElement("input");
  underlayAlpha.type = "range";
  underlayAlpha.min = "10";
  underlayAlpha.max = "100";
  underlayAlpha.value = "45";
  underlayAlpha.addEventListener("input", render);

  // 性別/服/髪/目 は char-context.js（共有ストア）へ集約したので、
  // ここでは持たない。値の変更は context.subscribe() 経由で拾う。
  underlayBar.append(underlayLabel, labeled("濃さ", underlayAlpha));
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
  // 選択中の色を大きめのスウォッチで示す(テキストだけでは分かりづらいため)
  const colorRow = el("div", "sp-color-row");
  const currentSwatch = el("span", "sp-current-color");
  const colorInfo = el("p", "ie-dim-info");
  colorRow.append(currentSwatch, colorInfo);
  section.appendChild(colorRow);

  // --- 保存 ---
  const saveBar = el("div", "cc-toolbar");
  const saveBtn = iconLabelButton("button primary", "save", "この内容で保存");
  attachTip(saveBtn, "この内容で保存 (Ctrl+S)");
  saveBtn.addEventListener("click", () => { void save(); });
  const discardBtn = iconLabelButton("button small", "revert", "編集を破棄");
  discardBtn.addEventListener("click", () => { void load({ force: true }); });
  const dirtyMark = el("span", "sp-dirty");
  saveBar.append(saveBtn, discardBtn, dirtyMark);
  section.appendChild(saveBar);

  // --- 途中セーブ ---
  // ゲームに反映せずに作業中の 1 セルを保存しておく置き場。
  // サーバー側では grp_draft テーブルに入り、配信経路からは参照されない。
  const draftSec = el("section", "detail-section sp-draft-section");
  draftSec.appendChild(el("h3", null, "途中セーブ"));
  draftSec.appendChild(el("p", "card-description",
    "ゲームには反映されません。作業中のセルを名前を付けて保存し、あとから選んで再開できます。"));

  const draftBar = el("div", "cc-toolbar");

  // 名前は prompt() ではなくインラインの入力欄で受ける。
  // prompt() は別ウィンドウや埋め込み表示だとブロックされて例外になり、
  // 「押しても何も起きない」状態になるため使わない。
  const draftNameInput = document.createElement("input");
  draftNameInput.type = "text";
  draftNameInput.className = "sp-draft-name";
  draftNameInput.placeholder = "途中セーブの名前";
  draftBar.appendChild(labeled("名前", draftNameInput));

  const draftNewBtn = iconLabelButton("button small", "plus", "新規途中セーブ");
  draftNewBtn.addEventListener("click", () => { void saveDraft({ asNew: true }); });

  const draftOverwriteBtn = iconLabelButton("button small", "save", "上書き途中セーブ");
  draftOverwriteBtn.addEventListener("click", () => { void saveDraft({ asNew: false }); });

  const draftCurrent = el("span", "sp-draft-current");
  draftBar.append(draftNewBtn, draftOverwriteBtn, draftCurrent);
  draftSec.appendChild(draftBar);

  const draftListBody = el("div", "sp-draft-list");
  draftSec.appendChild(draftListBody);
  section.appendChild(draftSec);

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

  // 下敷きの各レイヤーが参照するシート番号。体(char2x2)は常に #0、
  // 服/髪/目は共有ストア(char-context.js)の指定に従う。
  function underlaySheetIndex(slot) {
    const ctx = context.get();
    if (slot === "cloth") return ctx.cloth;
    if (slot === "hair") return ctx.hair;
    if (slot === "eye") return ctx.eye;
    return 0; // body
  }

  // 編集セルから「フレーム番号と性別」を復元する。下敷きの座標計算に使う。
  function underlayContext() {
    const sex = context.get().sex;
    if (_cat?.key === "eye2x2") {
      // 目シートは x が 4 セル分ずれているので frame を戻す。性別は判断材料が
      // 無いため（目シートに男女の別が無い）共有ストアの指定を使う。
      return { frame: _row * 16 + _col + 4, sex };
    }
    return { frame: (_row % 4) * 16 + _col, sex };
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
      const img = underlayImages.get(`${layer.key}/${underlaySheetIndex(layer.slot)}`);
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
  }

  // onCellChange の第3引数用。シート寸法とセル数を渡す（パンくず表示に使う）
  function sheetInfoPayload() {
    return _sheet
      ? { sheetWidth: _sheet.width, sheetHeight: _sheet.height, cols: _cols, rows: _rows }
      : { sheetWidth: 0, sheetHeight: 0, cols: 0, rows: 0 };
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

    if (c && (c.a ?? 255) !== 0) {
      currentSwatch.classList.remove("is-transparent");
      currentSwatch.style.background = `rgb(${c.r}, ${c.g}, ${c.b})`;
    } else {
      currentSwatch.classList.add("is-transparent");
      currentSwatch.style.background = "";
    }
  }

  function setColor(i) {
    _color = i;
    renderPalette();
  }

  // パレットの選択色を前後へ循環させる（ショートカット [ / ] 用）
  function cyclePalette(delta) {
    if (!_sheet || !_sheet.palette.length) return;
    const n = _sheet.palette.length;
    setColor((_color + delta + n) % n);
  }

  function setTool(id) {
    _tool = id;
    toolButtons.forEach((btn, key) => {
      btn.classList.toggle("is-selected", key === id);
    });
  }

  function selectCell(col, row) {
    // シート未読込（まだ load 中、またはパレット PNG でなく編集不可）の時は
    // 値だけ覚えておく。load() 側で寸法に合わせてクランプされる。
    if (!_sheet) {
      _col = Math.max(0, col);
      _row = Math.max(0, row);
      onCellChange?.(_col, _row, sheetInfoPayload());
      return;
    }
    _col = Math.max(0, Math.min(_cols - 1, col));
    _row = Math.max(0, Math.min(_rows - 1, row));
    colSelect.value = String(_col);
    rowSelect.value = String(_row);
    // セルを跨いだ Undo は混乱のもとなので履歴はセル単位で捨てる
    undoStack.length = 0;
    redoStack.length = 0;
    onCellChange?.(_col, _row, sheetInfoPayload());
    updateDraftUi();  // 名前欄の既定値にセル位置が入るので追従させる
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

  // 右クリックはツールに関わらずスポイト。ブラウザのメニューは出さない。
  canvas.addEventListener("contextmenu", (e) => e.preventDefault());

  canvas.addEventListener("pointerdown", (e) => {
    if (!_sheet) return;
    const pos = toPixel(e);
    if (!pos) return;

    // 右クリック: 色を拾うだけ。履歴も dirty も動かさない
    if (e.button === 2) {
      setColor(pixelAt(pos.px, pos.py));
      return;
    }
    if (e.button !== 0) return;

    // スポイトは履歴を汚さない
    if (_tool !== "picker") pushUndo();
    _painting = _tool === "pen" || _tool === "eraser";
    // setPointerCapture は pointerId が実在しないと例外を投げる。
    // ここで throw すると以降の描画処理ごと中断してしまうので握りつぶす
    // （キャプチャできなくてもドラッグ以外の描画は成立する）。
    if (_painting) {
      try { canvas.setPointerCapture(e.pointerId); } catch { /* 無視 */ }
    }
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
      const index = underlaySheetIndex(layer.slot);
      const img = await loadImage(`/api/assets/sprites/${encodeURIComponent(layer.key)}/${index}?v=${stamp}`);
      underlayImages.set(`${layer.key}/${index}`, img);
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
      // ユーザーが拡大率を触っていなければ、セルサイズに応じた既定値へ合わせる
      if (!_zoomUserSet) {
        _zoom = defaultZoomFor(_cellSize);
        zoomSelect.value = String(_zoom);
      }
      _cols = Math.max(1, Math.floor(decoded.width / _cellSize));
      _rows = Math.max(1, Math.floor(decoded.height / _cellSize));
      _col = Math.min(_col, _cols - 1);
      _row = Math.min(_row, _rows - 1);

      fillOptions(colSelect, _cols);
      fillOptions(rowSelect, _rows);
      colSelect.value = String(_col);
      rowSelect.value = String(_row);
      // 読み込み後の寸法でクランプされた結果をプレビュー側の枠にも反映する
      onCellChange?.(_col, _row, sheetInfoPayload());

      if (_color >= decoded.palette.length) _color = 1;
      applyCanvasSize();
      renderPalette();
      render();
      setSectionEnabled(true);
      await reloadUnderlay();
      await reloadDraftList();
      await applyPendingDraft();
      updateDraftUi();
    } catch (e) {
      if (seq !== _loadSeq) return;
      setSectionEnabled(false, e?.message);
    }
  }

  function setSectionEnabled(enabled, reason) {
    // 編集できないシートでプレビューに選択枠だけ残ると紛らわしいので、
    // col に負値を渡して枠を隠してもらう
    if (!enabled) onCellChange?.(-1, 0, sheetInfoPayload());
    stage.style.display = enabled ? "" : "none";
    paletteWrap.style.display = enabled ? "" : "none";
    saveBar.style.display = enabled ? "" : "none";
    toolBar.style.display = enabled ? "" : "none";
    cellBar.style.display = enabled ? "" : "none";
    underlayBar.style.display =
      enabled && _cat && COMPOSABLE_KEYS.has(_cat.key) && _cat.key !== "npc2x2" ? "" : "none";
    desc.textContent = enabled
      ? DESC_TEXT
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
    if (_dirty && !askConfirm("未保存のペイント内容があります。破棄して切り替えますか?")) {
      onFeedback?.(
        `ペイントは ${_cat?.label || _cat?.key} #${_index} を編集したままです。保存するか「編集を破棄」を押してください`,
        "warning"
      );
      return false;
    }
    _cat = cat;
    _index = index;
    _dirty = false;
    _zoomUserSet = false; // カテゴリを切り替えたら拡大率は既定へ戻す
    // 途中セーブを開く途中でなければ、紐付けは切る（別の対象に上書きしないため）
    if (!_pendingDraft) {
      _draftId = null;
      _draftName = "";
    }
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

  /**
   * デザインツール風のキーボードショートカットを解釈する。
   * 新しいロジックは持たず、既存の setTool/undo/redo/selectCell/save/
   * cyclePalette を呼ぶだけにとどめる。呼び出し側(image-editor.js)は
   * 入力欄へのフォーカス判定だけ行い、キー判定と実行はここへ集約する。
   * 処理した場合は true を返す（呼び出し側で preventDefault() する）。
   */
  function handleShortcut(event) {
    if (event.altKey || event.metaKey) return false;
    const key = event.key;
    const lower = typeof key === "string" ? key.toLowerCase() : key;

    if (event.ctrlKey) {
      if (lower === "s") { void save(); return true; }
      if (lower === "z" && event.shiftKey) { redo(); return true; }
      if (lower === "z") { undo(); return true; }
      if (lower === "y") { redo(); return true; }
      return false;
    }

    // ここから下はシートが読み込まれていないと意味を持たない操作
    if (!_sheet) return false;

    switch (lower) {
      case "b": setTool("pen"); return true;
      case "e": setTool("eraser"); return true;
      case "i": setTool("picker"); return true;
      case "g": setTool("fill"); return true;
      case "[": cyclePalette(-1); return true;
      case "]": cyclePalette(1); return true;
    }
    switch (key) {
      case "ArrowLeft":  selectCell(_col - 1, _row); return true;
      case "ArrowRight": selectCell(_col + 1, _row); return true;
      case "ArrowUp":    selectCell(_col, _row - 1); return true;
      case "ArrowDown":  selectCell(_col, _row + 1); return true;
    }
    return false;
  }

  // 共有ストア(性別/服/髪/目)が変わったら下敷きを読み直す
  context.subscribe(() => { void reloadUnderlay(); });

  setTool("pen");


  // ----------------------------------------------------------------
  // 途中セーブ
  //
  // 保存単位は「いま編集しているセル 1 個」。シート 1 枚ではなくセルにすると
  // 32x32 のパレット PNG で 1KB 前後に収まり、既定のボディ上限 64KB の内側で
  // 扱える（スプライト差し替えのように上限を緩める必要がない）。
  // ----------------------------------------------------------------

  let _draftId = null;      // 開いている / 上書き対象の途中セーブ id
  let _draftName = "";
  let _drafts = [];

  function updateDraftUi() {
    draftOverwriteBtn.disabled = _draftId == null || !_sheet;
    draftNewBtn.disabled = !_sheet;
    draftNameInput.disabled = !_sheet;
    draftNameInput.placeholder = suggestedDraftName() || "途中セーブの名前";
    draftCurrent.textContent = _draftId != null
      ? `編集中の途中セーブ: ${_draftName}（#${_draftId}）`
      : "途中セーブから開いていません";
  }

  // 現在のセルだけを切り出したパレット PNG を作る
  async function encodeCurrentCellPng() {
    const { x, y } = cellOrigin();
    const cell = new Uint8Array(_cellSize * _cellSize);
    for (let row = 0; row < _cellSize; row++) {
      const src = (y + row) * _sheet.width + x;
      cell.set(_sheet.indices.subarray(src, src + _cellSize), row * _cellSize);
    }
    return encodeIndexedPng({
      width: _cellSize,
      height: _cellSize,
      indices: cell,
      palette: _sheet.palette,
    });
  }

  function bytesToBase64(bytes) {
    let binary = "";
    const CHUNK = 0x8000;  // apply の引数上限に触れないよう分割する
    for (let i = 0; i < bytes.length; i += CHUNK) {
      binary += String.fromCharCode.apply(null, bytes.subarray(i, i + CHUNK));
    }
    return btoa(binary);
  }

  function base64ToBytes(text) {
    const binary = atob(text);
    const bytes = new Uint8Array(binary.length);
    for (let i = 0; i < binary.length; i++) bytes[i] = binary.charCodeAt(i);
    return bytes;
  }

  function suggestedDraftName() {
    if (!_cat) return "";
    return `${_cat.label || _cat.key} #${_index} (${_col},${_row})`;
  }

  async function saveDraft({ asNew }) {
    if (!_sheet || !_cat) {
      onFeedback?.("編集できるシートが読み込まれていません", "error");
      return;
    }

    let name = _draftName;
    if (asNew || _draftId == null) {
      name = draftNameInput.value.trim() || suggestedDraftName();
    }

    try {
      const png = await encodeCurrentCellPng();
      const payload = {
        name,
        catKey: _cat.key,
        sheetIndex: _index,
        col: _col,
        row: _row,
        width: _cellSize,
        height: _cellSize,
        png: bytesToBase64(png),
      };
      const useUpdate = !asNew && _draftId != null;
      const { response, data } = await fetchJson(
        useUpdate ? `/api/assets/drafts/${_draftId}` : "/api/assets/drafts",
        {
          method: useUpdate ? "PUT" : "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload),
        }
      );
      if (!response.ok) {
        onFeedback?.(`途中セーブに失敗しました: ${data?.error || `HTTP ${response.status}`}`, "error");
        return;
      }
      _draftId = data?.id ?? _draftId;
      _draftName = name;
      draftNameInput.value = name;
      onFeedback?.(useUpdate ? `途中セーブ「${name}」を上書きしました` : `途中セーブ「${name}」を作成しました`, "success");
      await reloadDraftList();
      updateDraftUi();
    } catch (e) {
      onFeedback?.("途中セーブに失敗しました: " + String(e?.message ?? e), "error");
    }
  }

  async function reloadDraftList() {
    try {
      const { response, data } = await fetchJson("/api/assets/drafts");
      if (!response.ok) {
        draftListBody.innerHTML = "";
        draftListBody.appendChild(el("p", "card-description", "途中セーブ一覧を取得できませんでした"));
        return;
      }
      _drafts = data?.drafts ?? [];
      renderDraftList();
    } catch (e) {
      draftListBody.innerHTML = "";
      draftListBody.appendChild(el("p", "card-description",
        "途中セーブ一覧の取得に失敗しました: " + String(e?.message ?? e)));
    }
  }

  function renderDraftList() {
    draftListBody.innerHTML = "";
    if (!_drafts.length) {
      draftListBody.appendChild(el("p", "card-description", "途中セーブはありません"));
      return;
    }

    const table = document.createElement("table");
    table.className = "data-table";
    const thead = document.createElement("thead");
    thead.innerHTML = "<tr><th>名前</th><th>対象</th><th>保存日時</th><th>保存者</th><th>操作</th></tr>";
    table.appendChild(thead);

    const tbody = document.createElement("tbody");
    _drafts.forEach((d) => {
      const tr = document.createElement("tr");
      if (d.id === _draftId) tr.classList.add("is-selected");

      const label = catByKey.get(d.catKey)?.label || d.catKey;
      tr.append(
        el("td", null, d.name || "(名前なし)"),
        el("td", null, `${label} #${d.sheetIndex} (${d.col},${d.row})`),
        el("td", null, formatEpoch(d.updatedAt)),
        el("td", null, d.updatedBy || "-")
      );

      const tdOp = document.createElement("td");
      // 未保存の変更があるときは confirm() ではなく 2 回クリックで確認する。
      // confirm() は環境によって自動的に打ち消され、「押しても何も起きない」
      // 状態になってしまうため使わない。
      // ラベルはアイコンと分離した span に入れる。textContent を丸ごと
      // 差し替えるとアイコン(SVG子要素)まで消えてしまうため。
      const openBtn = el("button", "button small");
      openBtn.type = "button";
      openBtn.appendChild(icon("folderOpen"));
      const openLabel = el("span", null, "開く");
      openBtn.appendChild(openLabel);
      let openArmed = false;
      let openTimer = null;
      openBtn.addEventListener("click", () => {
        if (_dirty && !openArmed) {
          openArmed = true;
          openLabel.textContent = "変更を捨てて開く";
          onFeedback?.("未保存のペイント内容があります。もう一度押すと破棄して開きます", "error");
          openTimer = setTimeout(() => {
            openArmed = false;
            openLabel.textContent = "開く";
          }, 5000);
          return;
        }
        if (openTimer) clearTimeout(openTimer);
        void openDraft(d);
      });

      // confirm() が使えない環境（別ウィンドウ等）でも確実に確認を挟めるよう、
      // モーダルではなく 2 回クリックで消す方式にする
      const delBtn = el("button", "button small danger");
      delBtn.type = "button";
      delBtn.appendChild(icon("trash"));
      const delLabel = el("span", null, "削除");
      delBtn.appendChild(delLabel);
      let armed = false;
      let armTimer = null;
      delBtn.addEventListener("click", () => {
        if (!armed) {
          armed = true;
          delLabel.textContent = "本当に削除?";
          armTimer = setTimeout(() => {
            armed = false;
            delLabel.textContent = "削除";
          }, 4000);
          return;
        }
        if (armTimer) clearTimeout(armTimer);
        void deleteDraft(d);
      });

      tdOp.append(openBtn, delBtn);
      tr.appendChild(tdOp);
      tbody.appendChild(tr);
    });
    table.appendChild(tbody);
    draftListBody.appendChild(table);
  }

  async function deleteDraft(d) {
    try {
      const { response, data } = await fetchJson(`/api/assets/drafts/${d.id}`, { method: "DELETE" });
      if (!response.ok) {
        onFeedback?.(`削除に失敗しました: ${data?.error || `HTTP ${response.status}`}`, "error");
        return;
      }
      if (_draftId === d.id) { _draftId = null; _draftName = ""; }
      onFeedback?.(`途中セーブ「${d.name}」を削除しました`, "success");
      await reloadDraftList();
      updateDraftUi();
    } catch (e) {
      onFeedback?.("削除に失敗しました: " + String(e?.message ?? e), "error");
    }
  }

  // 未保存の確認は一覧側の 2 段階クリックで済ませてあるので、ここでは行わない
  async function openDraft(d) {
    try {
      const { response, data } = await fetchJson(`/api/assets/drafts/${d.id}`);
      if (!response.ok) {
        onFeedback?.(`途中セーブを開けませんでした: ${data?.error || `HTTP ${response.status}`}`, "error");
        return;
      }
      const draft = data?.draft;
      if (!draft?.png) {
        onFeedback?.("途中セーブの内容が空でした", "error");
        return;
      }
      // シートの読み込み完了後に貼り込む（load() の最後で処理される）
      _pendingDraft = {
        id: draft.id,
        name: draft.name,
        col: draft.col,
        row: draft.row,
        bytes: base64ToBytes(draft.png),
      };
      _dirty = false;  // ここで破棄の確認は済んでいる

      // カテゴリ/シートの切り替えは親に依頼する（プレビューや履歴も追従させるため）
      if (onRequestTarget) {
        onRequestTarget(draft.catKey, draft.sheetIndex);
      } else {
        await load({ force: true });
      }
    } catch (e) {
      onFeedback?.("途中セーブを開けませんでした: " + String(e?.message ?? e), "error");
    }
  }

  // load() の最後で呼ぶ。開こうとしている途中セーブがあればセルへ貼り込む。
  async function applyPendingDraft() {
    if (!_pendingDraft || !_sheet) return;
    const pending = _pendingDraft;
    _pendingDraft = null;

    try {
      const cell = await decodeIndexedPng(pending.bytes);
      if (cell.width !== _cellSize || cell.height !== _cellSize) {
        onFeedback?.(
          `途中セーブのセル寸法が合いません（${cell.width}x${cell.height} / 現在 ${_cellSize}x${_cellSize}）`,
          "error");
        return;
      }
      selectCell(pending.col, pending.row);

      const { x, y } = cellOrigin();
      for (let row = 0; row < _cellSize; row++) {
        _sheet.indices.set(cell.indices.subarray(row * _cellSize, (row + 1) * _cellSize),
                            (y + row) * _sheet.width + x);
      }
      _draftId = pending.id;
      _draftName = pending.name;
      draftNameInput.value = pending.name;
      _dirty = true;  // まだゲームには反映されていない
      undoStack.length = 0;
      redoStack.length = 0;
      updateDraftUi();
      renderDraftList();
      render();
      onFeedback?.(`途中セーブ「${pending.name}」を開きました。ゲームに反映するには「この内容で保存」を押してください`, "success");
    } catch (e) {
      onFeedback?.("途中セーブの読み込みに失敗しました: " + String(e?.message ?? e), "error");
    }
  }

  return {
    el: section, // 後方互換用。image-editor 側は parts を個別に配置するので使わない
    parts: { desc, cellBar, toolBar, underlayBar, stage, palette: paletteWrap, colorInfo, colorRow, saveBar, draftSec },
    setTarget, refresh, hasUnsavedChanges, selectCell, handleShortcut,
  };
}

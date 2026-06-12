/**
 * components/sprite-picker.js
 * スプライト選択モーダルダイアログ。
 * openSpritePicker() でモーダルを開き、ユーザーが選択→ onSelect コールバック。
 * createSpriteField() でサムネ+数値input+選択ボタン の横並びフィールドを返す。
 */

import { loadCatalog, calcSpriteCoord, loadSheetImage } from "../data/assets.js";
import { createSpriteThumb } from "./sprite-thumb.js";

// ----------------------------------------------------------------
// openSpritePicker
// ----------------------------------------------------------------

/**
 * スプライト選択モーダルを開く。
 *
 * @param {{
 *   categoryKey: string,
 *   current?: number,
 *   allowCategorySwitch?: boolean,
 *   onSelect: (result: { categoryKey: string, sub: number }) => void,
 * }} options
 */
export async function openSpritePicker({ categoryKey, current = 0, allowCategorySwitch = false, onSelect }) {
  const catalog = await loadCatalog();

  let _catKey = categoryKey;
  let _cat = catalog.find((c) => c.key === _catKey) ?? catalog[0];
  if (!_cat) return;

  const cellsPerSheet = _cat.countX * _cat.countY;
  let _sheetIndex = Math.floor(current / cellsPerSheet);
  let _selectedSub = current;
  let _hoverSub = -1;
  let _sheetImg = null;
  const DISPLAY_SCALE = _cat.cellSize <= 16 ? 2 : 1;

  // --- backdrop ---
  const backdrop = document.createElement("div");
  backdrop.className = "sp-backdrop";
  backdrop.setAttribute("role", "dialog");
  backdrop.setAttribute("aria-modal", "true");

  // --- dialog ---
  const dialog = document.createElement("div");
  dialog.className = "sp-dialog";
  backdrop.appendChild(dialog);

  // --- header ---
  const header = document.createElement("div");
  header.className = "sp-header";

  let catSelect = null;
  if (allowCategorySwitch) {
    catSelect = document.createElement("select");
    catalog.forEach((c) => {
      const opt = document.createElement("option");
      opt.value = c.key;
      opt.textContent = c.label ?? c.key;
      if (c.key === _catKey) opt.selected = true;
      catSelect.appendChild(opt);
    });
    catSelect.addEventListener("change", () => {
      _catKey = catSelect.value;
      _cat = catalog.find((c) => c.key === _catKey);
      _sheetIndex = 0;
      _selectedSub = 0;
      _updateIdInput();
      _loadSheet();
    });
    header.appendChild(catSelect);
  }

  // シート切替
  const sheetNav = document.createElement("div");
  sheetNav.className = "sp-sheet-nav";
  const btnPrev = document.createElement("button");
  btnPrev.type = "button";
  btnPrev.textContent = "◀";
  btnPrev.setAttribute("aria-label", "前のシート");
  const sheetLabel = document.createElement("span");
  const btnNext = document.createElement("button");
  btnNext.type = "button";
  btnNext.textContent = "▶";
  btnNext.setAttribute("aria-label", "次のシート");
  sheetNav.append(btnPrev, sheetLabel, btnNext);
  header.appendChild(sheetNav);

  // ID直接入力
  const idRow = document.createElement("div");
  idRow.className = "sp-id-row";
  const idLabel = document.createElement("label");
  idLabel.textContent = "ID:";
  const idInput = document.createElement("input");
  idInput.type = "number";
  idInput.min = "0";
  idRow.append(idLabel, idInput);
  header.appendChild(idRow);

  dialog.appendChild(header);

  // --- body ---
  const body = document.createElement("div");
  body.className = "sp-body";

  const sheetWrap = document.createElement("div");
  sheetWrap.className = "sp-sheet-wrap";

  const sheetImg = document.createElement("img");
  sheetImg.className = "sp-sheet-img";
  sheetImg.alt = "";

  const gridOverlay = document.createElement("div");
  gridOverlay.className = "sp-grid-overlay";

  const hoverRect = document.createElement("div");
  hoverRect.className = "sp-hover-rect";
  hoverRect.style.display = "none";

  const selectedRect = document.createElement("div");
  selectedRect.className = "sp-selected-rect";

  gridOverlay.append(hoverRect, selectedRect);
  sheetWrap.append(sheetImg, gridOverlay);
  body.appendChild(sheetWrap);
  dialog.appendChild(body);

  // --- footer ---
  const footer = document.createElement("div");
  footer.className = "sp-footer";

  const btnCancel = document.createElement("button");
  btnCancel.type = "button";
  btnCancel.className = "button";
  btnCancel.textContent = "キャンセル";

  const btnOk = document.createElement("button");
  btnOk.type = "button";
  btnOk.className = "button primary";
  btnOk.textContent = "決定";

  footer.append(btnCancel, btnOk);
  dialog.appendChild(footer);

  document.body.appendChild(backdrop);

  // ----------------------------------------------------------------
  // 内部ヘルパ
  // ----------------------------------------------------------------

  function _cellPx() {
    return _cat.cellSize * DISPLAY_SCALE;
  }

  function _updateSheetLabel() {
    sheetLabel.textContent = `シート ${_sheetIndex + 1} / ${_cat.sheetCount}`;
    btnPrev.disabled = _sheetIndex <= 0;
    btnNext.disabled = _sheetIndex >= _cat.sheetCount - 1;
  }

  function _cellToRect(cellIndex) {
    const px = _cellPx();
    const col = cellIndex % _cat.countX;
    const row = Math.floor(cellIndex / _cat.countX);
    return { left: col * px, top: row * px, size: px };
  }

  function _updateSelectedRect() {
    const cps = _cat.countX * _cat.countY;
    const cellOnSheet = _selectedSub % cps;
    const { left, top, size } = _cellToRect(cellOnSheet);
    selectedRect.style.left = left + "px";
    selectedRect.style.top = top + "px";
    selectedRect.style.width = size + "px";
    selectedRect.style.height = size + "px";
  }

  function _updateIdInput() {
    idInput.value = String(_selectedSub);
    idInput.max = String(_cat.countX * _cat.countY * _cat.sheetCount - 1);
  }

  async function _loadSheet() {
    const scale = _cat.cellSize <= 16 ? 2 : 1;
    const url = `${_cat.sheetUrl}/${_sheetIndex}`;
    _updateSheetLabel();
    try {
      const img = await loadSheetImage(url);
      _sheetImg = img;
      sheetImg.src = img.src;
      sheetImg.style.width = (img.naturalWidth * scale) + "px";
      sheetImg.style.height = (img.naturalHeight * scale) + "px";
      sheetWrap.style.width = (img.naturalWidth * scale) + "px";
      sheetWrap.style.height = (img.naturalHeight * scale) + "px";
      gridOverlay.style.width = (img.naturalWidth * scale) + "px";
      gridOverlay.style.height = (img.naturalHeight * scale) + "px";
      _updateSelectedRect();
    } catch {
      sheetImg.removeAttribute("src");
    }
  }

  function _subFromMouse(e) {
    const rect = sheetWrap.getBoundingClientRect();
    const px = _cellPx();
    const col = Math.floor((e.clientX - rect.left) / px);
    const row = Math.floor((e.clientY - rect.top) / px);
    if (col < 0 || col >= _cat.countX || row < 0 || row >= _cat.countY) return -1;
    const cellIndex = row * _cat.countX + col;
    return _sheetIndex * (_cat.countX * _cat.countY) + cellIndex;
  }

  function _close() {
    backdrop.remove();
    document.removeEventListener("keydown", _onKeydown);
  }

  function _onKeydown(e) {
    if (e.key === "Escape") { e.preventDefault(); _close(); }
  }

  // ----------------------------------------------------------------
  // イベント
  // ----------------------------------------------------------------

  btnPrev.addEventListener("click", () => {
    if (_sheetIndex > 0) { _sheetIndex--; _loadSheet(); }
  });
  btnNext.addEventListener("click", () => {
    if (_sheetIndex < _cat.sheetCount - 1) { _sheetIndex++; _loadSheet(); }
  });

  idInput.addEventListener("change", () => {
    const v = parseInt(idInput.value, 10);
    if (!Number.isFinite(v)) return;
    const max = _cat.countX * _cat.countY * _cat.sheetCount - 1;
    const clamped = Math.max(0, Math.min(max, v));
    _selectedSub = clamped;
    const cps = _cat.countX * _cat.countY;
    _sheetIndex = Math.floor(clamped / cps);
    _loadSheet();
    _updateIdInput();
  });

  sheetWrap.addEventListener("mousemove", (e) => {
    const s = _subFromMouse(e);
    if (s === _hoverSub) return;
    _hoverSub = s;
    if (s < 0) {
      hoverRect.style.display = "none";
      return;
    }
    const cps = _cat.countX * _cat.countY;
    const { left, top, size } = _cellToRect(s % cps);
    hoverRect.style.display = "block";
    hoverRect.style.left = left + "px";
    hoverRect.style.top = top + "px";
    hoverRect.style.width = size + "px";
    hoverRect.style.height = size + "px";
  });

  sheetWrap.addEventListener("mouseleave", () => {
    hoverRect.style.display = "none";
    _hoverSub = -1;
  });

  sheetWrap.addEventListener("click", (e) => {
    const s = _subFromMouse(e);
    if (s < 0) return;
    _selectedSub = s;
    _updateIdInput();
    _updateSelectedRect();
  });

  backdrop.addEventListener("click", (e) => {
    if (e.target === backdrop) _close();
  });

  btnCancel.addEventListener("click", _close);

  btnOk.addEventListener("click", () => {
    onSelect({ categoryKey: _catKey, sub: _selectedSub });
    _close();
  });

  document.addEventListener("keydown", _onKeydown);

  // 初期表示
  _updateIdInput();
  _loadSheet();
}

// ----------------------------------------------------------------
// createSpriteField
// ----------------------------------------------------------------

/**
 * サムネ + 数値input + 選択ボタン の横並びフィールドを返す。
 *
 * @param {{
 *   categoryKey: string,
 *   value?: number,
 *   onChange?: (sub: number) => void,
 *   label?: string,
 *   allowCategorySwitch?: boolean,
 * }} options
 * @returns {{ el: HTMLElement, getValue: () => number, setValue: (sub: number) => void }}
 */
export function createSpriteField({ categoryKey, value = 0, onChange, label, allowCategorySwitch = false }) {
  const wrap = document.createElement("div");
  wrap.className = "sprite-field";

  if (label) {
    const lbl = document.createElement("span");
    lbl.className = "sprite-field-label";
    lbl.textContent = label;
    wrap.appendChild(lbl);
  }

  let _catKey = categoryKey;
  let _value = value;

  const thumb = createSpriteThumb({ categoryKey: _catKey, sub: _value, size: 32 });
  wrap.appendChild(thumb.el);

  const numInput = document.createElement("input");
  numInput.type = "number";
  numInput.min = "0";
  numInput.value = String(_value);
  wrap.appendChild(numInput);

  const pickBtn = document.createElement("button");
  pickBtn.type = "button";
  pickBtn.className = "button small";
  pickBtn.textContent = "選択…";
  wrap.appendChild(pickBtn);

  numInput.addEventListener("change", () => {
    const v = parseInt(numInput.value, 10);
    if (!Number.isFinite(v) || v < 0) return;
    _value = v;
    thumb.update(_value);
    onChange?.(_value);
  });

  pickBtn.addEventListener("click", () => {
    openSpritePicker({
      categoryKey: _catKey,
      current: _value,
      allowCategorySwitch,
      onSelect: ({ categoryKey: ck, sub }) => {
        _catKey = ck;
        _value = sub;
        numInput.value = String(_value);
        thumb.update(_value);
        onChange?.(_value);
      },
    });
  });

  function getValue() { return _value; }
  function setValue(sub) {
    _value = sub;
    numInput.value = String(sub);
    thumb.update(sub);
  }

  return { el: wrap, getValue, setValue };
}

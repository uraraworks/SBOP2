/**
 * components/number-spinner.js
 * [-][input][+] の数値スピナーコンポーネント。
 * ±ボタン長押しでリピート + 加速対応。
 */

/**
 * 数値スピナーを生成する。
 *
 * @param {{
 *   value?: number,
 *   min?: number,
 *   max?: number,
 *   step?: number,
 *   onChange?: (value: number) => void,
 * }} options
 * @returns {{ el: HTMLElement, getValue: () => number, setValue: (v: number) => void }}
 */
export function createNumberSpinner({ value = 0, min = -Infinity, max = Infinity, step = 1, onChange } = {}) {
  let _value = _clamp(value);

  const wrap = document.createElement("span");
  wrap.className = "number-spinner";

  const btnMinus = document.createElement("button");
  btnMinus.type = "button";
  btnMinus.textContent = "−";
  btnMinus.setAttribute("aria-label", "減らす");

  const input = document.createElement("input");
  input.type = "number";
  input.value = String(_value);
  input.min = Number.isFinite(min) ? String(min) : "";
  input.max = Number.isFinite(max) ? String(max) : "";
  input.step = String(step);

  const btnPlus = document.createElement("button");
  btnPlus.type = "button";
  btnPlus.textContent = "+";
  btnPlus.setAttribute("aria-label", "増やす");

  wrap.append(btnMinus, input, btnPlus);

  // ----------------------------------------------------------------
  // 内部ヘルパ
  // ----------------------------------------------------------------

  function _clamp(v) {
    return Math.min(max, Math.max(min, v));
  }

  function _set(v) {
    _value = _clamp(v);
    input.value = String(_value);
    _updateDisabled();
    onChange?.(_value);
  }

  function _updateDisabled() {
    btnMinus.disabled = _value <= min;
    btnPlus.disabled = _value >= max;
  }

  // ----------------------------------------------------------------
  // 長押しリピート(加速あり)
  // ----------------------------------------------------------------

  let _repeatTimer = null;
  let _repeatInterval = null;
  let _repeatCount = 0;

  function _startRepeat(delta) {
    _clearRepeat();
    _repeatTimer = setTimeout(() => {
      _repeatInterval = setInterval(() => {
        _repeatCount++;
        // 20回を超えると step*5 に加速
        const d = _repeatCount > 20 ? delta * 5 : delta;
        _set(_value + d);
      }, 60);
    }, 400);
  }

  function _clearRepeat() {
    _repeatCount = 0;
    if (_repeatTimer !== null) { clearTimeout(_repeatTimer); _repeatTimer = null; }
    if (_repeatInterval !== null) { clearInterval(_repeatInterval); _repeatInterval = null; }
  }

  function _onPointerDown(delta) {
    _set(_value + delta);
    _startRepeat(delta);
  }

  btnMinus.addEventListener("pointerdown", () => _onPointerDown(-step));
  btnPlus.addEventListener("pointerdown", () => _onPointerDown(step));

  const _stopEvents = ["pointerup", "pointerleave", "pointercancel"];
  _stopEvents.forEach((ev) => {
    btnMinus.addEventListener(ev, _clearRepeat);
    btnPlus.addEventListener(ev, _clearRepeat);
  });

  // キーボード操作
  btnMinus.addEventListener("keydown", (e) => {
    if (e.key === " " || e.key === "Enter") _set(_value - step);
  });
  btnPlus.addEventListener("keydown", (e) => {
    if (e.key === " " || e.key === "Enter") _set(_value + step);
  });

  // input 直接変更
  input.addEventListener("change", () => {
    const v = parseFloat(input.value);
    if (!Number.isFinite(v)) { input.value = String(_value); return; }
    _set(v);
  });

  _updateDisabled();

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  function getValue() { return _value; }
  function setValue(v) { _set(v); }

  return { el: wrap, getValue, setValue };
}

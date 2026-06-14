/**
 * components/number-spinner.js
 * [input] のみの数値スピナーコンポーネント。
 * ブラウザ標準の上下ボタンを利用するため、独自の±ボタンは持たない。
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

  const input = document.createElement("input");
  input.type = "number";
  input.value = String(_value);
  input.min = Number.isFinite(min) ? String(min) : "";
  input.max = Number.isFinite(max) ? String(max) : "";
  input.step = String(step);

  wrap.appendChild(input);

  // ----------------------------------------------------------------
  // 内部ヘルパ
  // ----------------------------------------------------------------

  function _clamp(v) {
    return Math.min(max, Math.max(min, v));
  }

  function _set(v) {
    _value = _clamp(v);
    input.value = String(_value);
    onChange?.(_value);
  }

  // input 直接変更
  input.addEventListener("change", () => {
    const v = parseFloat(input.value);
    if (!Number.isFinite(v)) { input.value = String(_value); return; }
    _set(v);
  });

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  function getValue() { return _value; }
  function setValue(v) { _set(v); }

  return { el: wrap, getValue, setValue };
}

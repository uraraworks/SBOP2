/**
 * components/sound-picker.js
 * /api/sounds から select を生成するサウンド選択コンポーネント。
 */

import { loadSounds } from "../data/assets.js";

/**
 * サウンド選択 select を生成する。
 *
 * @param {{
 *   value?: number,
 *   onChange?: (soundId: number) => void,
 * }} options
 * @returns {{ el: HTMLElement, getValue: () => number, setValue: (id: number) => void }}
 */
export function createSoundPicker({ value = 0, onChange } = {}) {
  const wrap = document.createElement("div");
  wrap.className = "sound-picker";

  const select = document.createElement("select");

  // 「(なし)」をデフォルトで追加
  const noneOpt = document.createElement("option");
  noneOpt.value = "0";
  noneOpt.textContent = "(なし)";
  select.appendChild(noneOpt);

  wrap.appendChild(select);

  let _value = value;

  // 非同期でサウンド一覧を読み込む
  loadSounds().then((sounds) => {
    sounds.forEach((s) => {
      const opt = document.createElement("option");
      // サウンドオブジェクトは { id, name } または数値を想定
      const id = typeof s === "object" ? (s.id ?? s.soundId ?? 0) : Number(s);
      const label = typeof s === "object" ? (s.name ?? s.label ?? String(id)) : String(s);
      opt.value = String(id);
      opt.textContent = `${id}: ${label}`;
      select.appendChild(opt);
    });
    // 現在値を反映
    select.value = String(_value);
  }).catch(() => {
    // 取得失敗時は(なし)のみ表示
  });

  select.addEventListener("change", () => {
    _value = parseInt(select.value, 10);
    onChange?.(_value);
  });

  function getValue() { return _value; }

  function setValue(id) {
    _value = id;
    select.value = String(id);
  }

  return { el: wrap, getValue, setValue };
}

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
/**
 * 管理画面のゲームiframe（#admin-game-frame）を探す。
 * 編集ペインは workspace-layout.js により別ウィンドウ（popup）へ adoptNode
 * されることがあるが、iframe 自体はメインウィンドウ側に残る。
 * このモジュールのコード自体はメインウィンドウの realm で動き続けるので
 * 通常は `document` で見つかるが、念のため popup 側 document から
 * `window.opener` 経由で辿るフォールバックも用意する。
 */
function findAdminGameFrame(el) {
  const direct = document.getElementById("admin-game-frame");
  if (direct) return direct;

  const ownerWin = el?.ownerDocument?.defaultView;
  const opener = ownerWin && ownerWin !== window ? ownerWin.opener : null;
  if (opener && opener.document) {
    return opener.document.getElementById("admin-game-frame");
  }
  return null;
}

export function createSoundPicker({ value = 0, onChange } = {}) {
  const wrap = document.createElement("div");
  wrap.className = "sound-picker";

  const select = document.createElement("select");

  // 「(なし)」をデフォルトで追加
  const noneOpt = document.createElement("option");
  noneOpt.value = "0";
  noneOpt.textContent = "(なし)";
  select.appendChild(noneOpt);

  const playBtn = document.createElement("button");
  playBtn.type = "button";
  playBtn.className = "sound-picker-play";
  playBtn.textContent = "▶";
  playBtn.title = "選択中の効果音を試聴";
  playBtn.disabled = true;
  playBtn.addEventListener("click", () => {
    const frame = findAdminGameFrame(wrap);
    if (!frame || !frame.contentWindow) {
      playBtn.title = "ゲーム画面が見つからないため試聴できません";
      return;
    }
    playBtn.title = "選択中の効果音を試聴";
    frame.contentWindow.postMessage(
      { kind: "sbop2_admin_play_sound", soundId: _value },
      "*"
    );
  });

  wrap.append(select, playBtn);

  let _value = value;
  playBtn.disabled = !_value;

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
    playBtn.disabled = !_value;
    onChange?.(_value);
  });

  function getValue() { return _value; }

  function setValue(id) {
    _value = id;
    select.value = String(id);
    playBtn.disabled = !_value;
  }

  return { el: wrap, getValue, setValue };
}

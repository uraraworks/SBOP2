/**
 * core/save-shortcut.js
 * Ctrl+S (Mac は Cmd+S) をグローバル捕捉し、現在ルートに登録された
 * 保存ハンドラを呼び出す。
 *
 * 使い方 (各 view の mount() 内):
 *   import { registerSaveHandler } from "../core/save-shortcut.js";
 *   registerSaveHandler("map-info", () => saveMapInfo());
 *
 * 1 view = 1 route の想定(game-pick.js の registerPickHandler と同じ方式)。
 * 同じ route への再登録は上書きになるため、view が再 mount されるたびに
 * 呼び直しても古いハンドラが残って二重実行される心配はない。
 * 明示的に外したい場合だけ unregisterSaveHandler(route, fn) を使う
 * (fn を渡した場合、登録されているものと一致する時だけ解除する)。
 *
 * ポップアップ対応:
 *   workspace-layout.js が編集ペインを別ウィンドウへ adoptNode で移すと、
 *   そのウィンドウで発生する keydown はメインウィンドウの document には
 *   届かない。そのためポップアップの window/document にも同じリスナーを張る
 *   (workspace-layout.js の onPopupWindowChange フックを購読)。
 *   ポップアップが閉じれば window ごと破棄されるので、リスナーの明示的な
 *   removeEventListener は不要。
 */

import { currentRoute } from "./router.js";
import { showInfoToast } from "../components/toast.js";
import { onPopupWindowChange } from "./workspace-layout.js";

/** @type {Map<string, () => void>} ルート名 → 保存ハンドラ */
const _handlers = new Map();

const _attachedDocs = new WeakSet();

/**
 * 現在ルート用の保存ハンドラを登録する(同じ route は上書き)。
 * @param {string} route
 * @param {() => void} fn
 */
export function registerSaveHandler(route, fn) {
  if (!route || typeof fn !== "function") {
    return;
  }
  _handlers.set(route, fn);
}

/**
 * 登録済みの保存ハンドラを解除する。
 * fn を渡した場合、登録されているものと一致する時だけ解除する
 * (別のハンドラに上書きされていた場合は誤って消さないため)。
 * @param {string} route
 * @param {() => void} [fn]
 */
export function unregisterSaveHandler(route, fn) {
  if (!route) {
    return;
  }
  if (fn && _handlers.get(route) !== fn) {
    return;
  }
  _handlers.delete(route);
}

function isSaveCombo(event) {
  const key = (event.key || "").toLowerCase();
  if (key !== "s") {
    return false;
  }
  // Windows/Linux は Ctrl+S、Mac は Cmd(meta)+S。Alt 併用は別ショートカットとみなし除外。
  return (event.ctrlKey || event.metaKey) && !event.altKey;
}

function handleKeydown(event) {
  if (!isSaveCombo(event)) {
    return;
  }
  event.preventDefault();
  if (event.repeat) {
    return;
  }
  const route = currentRoute();
  const handler = _handlers.get(route);
  if (!handler) {
    showInfoToast("この画面は Ctrl+S 保存に未対応です");
    return;
  }
  try {
    handler();
  } catch (err) {
    console.error("[save-shortcut] handler failed:", err);
  }
}

/**
 * 指定 document に Ctrl+S の keydown リスナーを張る(二重登録防止つき)。
 * @param {Document} doc
 */
export function attachSaveShortcut(doc) {
  const d = doc || document;
  if (_attachedDocs.has(d)) {
    return;
  }
  _attachedDocs.add(d);
  d.addEventListener("keydown", handleKeydown);
}

attachSaveShortcut(document);

// 編集ペインが別ウィンドウへポップアウトした時、そちらの document にも張る
// (workspace-layout.js は save-shortcut.js を知らない一方向の依存)。
onPopupWindowChange((win) => {
  if (win && win.document) {
    attachSaveShortcut(win.document);
  }
});

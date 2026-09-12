/**
 * core/game-pick.js
 * ゲーム画面(iframe)からの sbop2_admin_pick 受け口を一本化する。
 *
 * 経緯:
 *   従来は app.js の handleAdminGamePick が currentRoute ごとの if 分岐で
 *   処理しており、map-events.js は window._mapEventsState 等の window 経由公開、
 *   map-object-place.js は独自の window message リスナーを持つ二重実装だった。
 *   これを「ルート → ハンドラ」のレジストリ方式に統一する。
 *
 * 使い方 (ES Module 側の view):
 *   import { registerPickHandler, unregisterPickHandler } from "../core/game-pick.js";
 *   function onPick(pick) { ...; return true; } // 処理したら true
 *   registerPickHandler("map-events", onPick);
 *   // unmount 時:
 *   unregisterPickHandler("map-events", onPick);
 *
 * 使い方 (app.js からの利用、非 module のため window 経由):
 *   window.__gamePick.dispatchGamePick(message)  // true なら処理済み
 *
 * dirty-guard との関係:
 *   登録ハンドラがあるルートでは、確認ダイアログはハンドラ自身の責務とする
 *   (例: 選択中のイベントを差し替える時だけハンドラが confirmDiscard を呼ぶ)。
 *   dispatchGamePick はここでは confirmDiscard を呼ばない。
 *   登録ハンドラが無いルート(handler なし、または false を返した場合)は
 *   呼び出し元(app.js の handleAdminGamePick)が従来どおり dirty 確認してから
 *   既定の分岐処理を行うこと。
 *
 * 「次の1回のクリックを横取りする」一時モード:
 *   requestNextPick({ route, onPick, onCancel, message }) を呼ぶと、
 *   次に届いた pick は登録済みハンドラより優先してこの onPick に渡される。
 *   Esc キー、または cancelPendingPick() で中止できる。
 *   待機中は画面にトースト(info)で案内を表示する。
 *   マップイベントの「移動先をクリックで指定」、NPC 追加の
 *   「座標をクリックで指定」等、次段階の機能から利用する想定。
 */

import { currentRoute } from "./router.js";
import { showInfoToast } from "../components/toast.js";

/**
 * ルート名 → pick ハンドラ。
 * 1 view = 1 route の想定なので、route ごとに 1 つだけ保持する
 * (再 mount 時は同じ route のハンドラを上書きすれば良い)。
 * @type {Map<string, (pick: NormalizedPick) => boolean>}
 */
const _handlers = new Map();

/**
 * @typedef {Object} NormalizedPick
 * @property {number} mapId
 * @property {number} cellX
 * @property {number} cellY
 * @property {number} charId
 * @property {number} itemId
 * @property {any} raw  元の postMessage データ
 */

/**
 * @typedef {Object} PendingPick
 * @property {string} route
 * @property {(pick: NormalizedPick) => void} onPick
 * @property {(() => void)|undefined} onCancel
 * @property {{dismiss: () => void}|null} toastHandle
 */

/** @type {PendingPick|null} */
let _pendingPick = null;

/**
 * 現在ルート用の pick ハンドラを登録する(同じ route は上書き)。
 * @param {string} route
 * @param {(pick: NormalizedPick) => boolean} handler  処理したら true を返す
 */
export function registerPickHandler(route, handler) {
  if (!route || typeof handler !== "function") {
    return;
  }
  _handlers.set(route, handler);
}

/**
 * 登録済みハンドラを解除する。
 * handler を渡した場合、登録されているものと一致する時だけ解除する
 * (別のハンドラに上書きされていた場合は誤って消さないため)。
 * @param {string} route
 * @param {(pick: NormalizedPick) => boolean} [handler]
 */
export function unregisterPickHandler(route, handler) {
  if (!route) {
    return;
  }
  if (handler && _handlers.get(route) !== handler) {
    return;
  }
  _handlers.delete(route);
}

/**
 * postMessage の生データを NormalizedPick に正規化する。
 * @param {any} message
 * @returns {NormalizedPick}
 */
function normalizePick(message) {
  const src = message || {};
  return {
    mapId:  Number(src.mapId)  || 0,
    cellX:  Number(src.cellX)  || 0,
    cellY:  Number(src.cellY)  || 0,
    charId: Number(src.charId) || 0,
    itemId: Number(src.itemId) || 0,
    raw: src,
  };
}

function onPendingKeydown(event) {
  if (event.key === "Escape" || event.key === "Esc") {
    cancelPendingPick();
  }
}

/**
 * 次の 1 回の pick を横取りするモードに入る。
 * @param {Object} options
 * @param {string} [options.route]  対象ルート(省略時は現在のルート)。
 *   この route から離脱しても自動解除はしない(呼び出し元が hashchange 等で
 *   必要なら cancelPendingPick() すること)。
 * @param {(pick: NormalizedPick) => void} options.onPick
 * @param {() => void} [options.onCancel]
 * @param {string} [options.message]  案内トーストの文言
 */
export function requestNextPick(options) {
  const opts = options || {};
  if (typeof opts.onPick !== "function") {
    return;
  }
  cancelPendingPick({ silent: true });
  const toastHandle = showInfoToast(
    opts.message || "ゲーム画面をクリックしてください(Esc で中止)",
    {
      duration: 0,
      action: { label: "中止", onClick: () => cancelPendingPick() },
    }
  );
  _pendingPick = {
    route: opts.route || currentRoute(),
    onPick: opts.onPick,
    onCancel: opts.onCancel,
    toastHandle: toastHandle || null,
  };
  document.addEventListener("keydown", onPendingKeydown);
}

/**
 * 横取りモード中かどうか。
 * @returns {boolean}
 */
export function isPendingPick() {
  return !!_pendingPick;
}

/**
 * 横取りモードを解除する。
 * @param {{silent?: boolean}} [opts]  silent: true の時は onCancel を呼ばない
 */
export function cancelPendingPick(opts) {
  if (!_pendingPick) {
    return;
  }
  const { onCancel, toastHandle } = _pendingPick;
  _pendingPick = null;
  document.removeEventListener("keydown", onPendingKeydown);
  if (toastHandle && typeof toastHandle.dismiss === "function") {
    toastHandle.dismiss();
  }
  if (!(opts && opts.silent) && typeof onCancel === "function") {
    try {
      onCancel();
    } catch (err) {
      console.error("[game-pick] onCancel failed:", err);
    }
  }
}

/**
 * ゲーム iframe からの sbop2_admin_pick を振り分ける。
 * 横取りモード中ならそちらへ、そうでなければ現在ルートの登録ハンドラへ渡す。
 * @param {any} message  postMessage のデータ(sbop2_admin_pick)
 * @returns {boolean} 処理済みなら true(呼び出し元は以降の既定処理を行わない)
 */
export function dispatchGamePick(message) {
  const pick = normalizePick(message);

  const route = currentRoute();

  if (_pendingPick) {
    if (_pendingPick.route !== route) {
      // 待機中に別画面へ移っていた場合は待機を打ち切り、通常の振り分けへ進む。
      cancelPendingPick();
    } else {
      const { onPick } = _pendingPick;
      cancelPendingPick({ silent: true });
      try {
        onPick(pick);
      } catch (err) {
        console.error("[game-pick] onPick failed:", err);
      }
      return true;
    }
  }

  const handler = _handlers.get(route);
  if (!handler) {
    return false;
  }
  try {
    return !!handler(pick);
  } catch (err) {
    console.error("[game-pick] handler failed:", err);
    return false;
  }
}

/**
 * どの画面からでもキャラ詳細(character-overview)を開く。
 * 既に character-overview なら char-edit.js の window._charEditMount を直接呼び、
 * そうでなければ pendingCharId を積んでから hash を切り替える
 * (character-list.js の既存遷移と同じ方式。hashchange は非同期発火のため、
 *  mount 完了前に _charEditMount を呼んでしまわないよう pendingCharId 経由にする)。
 * @param {number|string} charId
 */
export function openCharacterEditor(charId) {
  const n = Number(charId);
  if (!Number.isFinite(n) || n <= 0) {
    return;
  }
  if (currentRoute() === "character-overview") {
    if (typeof window._charEditMount === "function") {
      window._charEditMount(n);
    } else {
      window._charEditPendingCharId = n;
    }
    return;
  }
  window._charEditPendingCharId = n;
  window.location.hash = "#character-overview";
}

if (typeof window !== "undefined") {
  // 待機中に別画面へ hash 遷移した場合、次の pick 受信を待たずに即座に打ち切る。
  window.addEventListener("hashchange", () => {
    if (_pendingPick) {
      cancelPendingPick();
    }
  });

  // app.js はレガシーの非 module スクリプトのため ES import できない。
  // dirty-guard.js と同じ方式で window 経由でも公開する。
  window.__gamePick = {
    registerPickHandler,
    unregisterPickHandler,
    dispatchGamePick,
    requestNextPick,
    cancelPendingPick,
    isPendingPick,
    openCharacterEditor,
  };
}

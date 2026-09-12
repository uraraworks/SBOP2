/**
 * core/dirty-guard.js
 * 未保存変更の共通ガード。
 *
 * registerDirty(() => boolean) で「現在の view が dirty か」を返す関数を登録する。
 * 登録は 1 view = 1 チェッカーの想定(同時に有効な view は 1 つのみ)。
 * view を離れる/dirty が解消したら clearDirty() で解除すること。
 *
 * 確認ダイアログについて:
 *   workspace-layout.js の popOut() は、編集ペインを別ウィンドウへ DOM ごと
 *   移した後も window.confirm/alert/prompt をメインウィンドウの実装のまま
 *   window.focus() でメインウィンドウを前面に出してから呼ぶよう上書き済み
 *   (installDialogFocusGuard)。このモジュールのスクリプト自体もメインウィンドウの
 *   realm で動くため、ここでは素の confirm() を使えば良い
 *   (「別ウィンドウ表示中は confirm が裏に隠れる」問題は workspace-layout.js 側で
 *   既に対策されている)。
 *
 * hashchange について:
 *   main.js の onHashChange の前段で guardHashChange() を呼ぶ。
 *   キャンセルされた場合は window.location.hash を直前の値に戻すが、
 *   それ自体が新たな hashchange を発火させ、レガシー app.js 側の
 *   hashchange リスナー(app.js 内、window.addEventListener("hashchange", ...))
 *   にも「キャンセル前の新 hash」で一度届いてしまう(同一イベント内で
 *   listener 登録順に同期実行されるため、hash の書き戻しは間に合わない)。
 *   これを避けるため、guardHashChange() は呼び出し元から渡された Event に対して
 *   stopImmediatePropagation() できるよう、キャンセル時に false を返す契約にし、
 *   main.js 側でイベント伝播を止める。hash を書き戻したことで発生する
 *   次の hashchange は「直前の値に戻すだけ」なので、guardHashChange() 内の
 *   サプレスフラグでこの2回目も確認ダイアログを出さずに false を返し、
 *   main.js / app.js のどちらのリスナーにも何もさせない(view の再 mount を防ぐ)。
 */

/** @type {(() => boolean)|null} */
let _dirtyChecker = null;

let _previousHash = typeof window !== "undefined" ? window.location.hash : "";
let _suppressNextHashGuard = false;

/**
 * 現在の view の dirty 判定関数を登録する。
 * @param {() => boolean} checker
 */
export function registerDirty(checker) {
  _dirtyChecker = typeof checker === "function" ? checker : null;
}

/** 登録を解除する(view 離脱時などに呼ぶ)。 */
export function clearDirty() {
  _dirtyChecker = null;
}

/** @returns {boolean} 現在 dirty かどうか */
export function isDirty() {
  if (!_dirtyChecker) {
    return false;
  }
  try {
    return !!_dirtyChecker();
  } catch (err) {
    console.error("[dirty-guard] checker failed:", err);
    return false;
  }
}

/**
 * dirty な場合のみ確認ダイアログを出す。
 * @param {string} [message]
 * @returns {boolean} 続行してよければ true(dirty でない、または確認 OK)
 */
export function confirmDiscard(message = "未保存の変更があります。破棄しますか?") {
  if (!isDirty()) {
    return true;
  }
  const ok = window.confirm(message);
  if (ok) {
    clearDirty();
  }
  return ok;
}

/**
 * main.js の onHashChange 前段から呼ぶ。
 * dirty で確認が取れなければ hash を直前の値に戻し false を返す
 * (呼び出し元は false の時、event.stopImmediatePropagation() で
 *  レガシー app.js 側リスナーへの伝播を止めること)。
 * @returns {boolean}
 */
export function guardHashChange() {
  if (_suppressNextHashGuard) {
    _suppressNextHashGuard = false;
    _previousHash = window.location.hash;
    return false;
  }
  if (!isDirty()) {
    _previousHash = window.location.hash;
    return true;
  }
  if (confirmDiscard()) {
    _previousHash = window.location.hash;
    return true;
  }
  _suppressNextHashGuard = true;
  window.location.hash = _previousHash;
  return false;
}

if (typeof window !== "undefined") {
  // setRouteParams (router.js) は history.replaceState で hash のクエリだけを
  // 書き換えるため hashchange が発火しない。これを放置すると _previousHash が
  // 更新されず、次に dirty で hashchange がキャンセルされた際に古いクエリへ
  // 巻き戻ってしまう。router.js はこのモジュールを import しない
  // (依存を増やさない) ため、CustomEvent 経由で受け取る。
  window.addEventListener("sbop2:routeparamschange", () => {
    _previousHash = window.location.hash;
  });

  window.addEventListener("beforeunload", (event) => {
    if (!isDirty()) {
      return;
    }
    event.preventDefault();
    // Chrome は returnValue 設定が必要
    event.returnValue = "";
  });

  // app.js はレガシーの非 module スクリプトで ES import できないため、
  // handleAdminGamePick 前段からの利用向けに window 経由でも公開する。
  window.__dirtyGuard = { registerDirty, clearDirty, isDirty, confirmDiscard, guardHashChange };
}

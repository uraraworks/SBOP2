/**
 * core/router.js
 * ハッシュルーティングユーティリティ。
 * レガシー app.js の hashchange リスナーと競合しない設計:
 *   - window.location.hash への書き込みは行わない
 *   - 既存リスナーはそのまま動作し続ける
 *   - このモジュールは「移行済みビューかどうかの判定」だけを担当する
 */

/** @type {Map<string, () => void>} ルート名 → マウント関数 */
const _registry = new Map();

/**
 * ビューコンテナを探す起点。既定は document。
 * 編集ペインを別ウィンドウへ移すと document 直下から辿れなくなるため、
 * 要素参照を起点にできるようにしてある（要素参照なら所属 document が
 * 変わっても querySelector は機能する）。
 * @type {Document|Element}
 */
let _viewRoot = document;

/**
 * ビュー検索の起点を差し替える。
 * @param {Document|Element} root
 */
export function setViewRoot(root) {
  _viewRoot = root || document;
}

/**
 * hash 文字列を { route, params } に分解する。
 * `#item-types?id=3` のようにクエリが付いていても、ルート名は
 * クエリを含まない部分だけを返す(存在しないルートとして扱われるのを防ぐ)。
 * @param {string} [hash]  省略時は window.location.hash
 * @returns {{ route: string, params: URLSearchParams }}
 */
export function parseHash(hash) {
  const raw = (hash === undefined ? window.location.hash : hash || "").replace(/^#/, "");
  const qIndex = raw.indexOf("?");
  const route = qIndex === -1 ? raw : raw.slice(0, qIndex);
  const query = qIndex === -1 ? "" : raw.slice(qIndex + 1);
  return { route, params: new URLSearchParams(query) };
}

/**
 * 現在の hash からルート名を取得する(クエリ部分は除く)。
 * @returns {string}
 */
export function currentRoute() {
  return parseHash().route;
}

/**
 * 現在の hash のクエリ部分を取得する。
 * @returns {URLSearchParams}
 */
export function getRouteParams() {
  return parseHash().params;
}

/**
 * 現在の hash のクエリ部分を書き換える(ルート名はそのまま)。
 * history.replaceState で書き換えるため hashchange は発火しない
 * (再 mount も dirty 確認も起きない)。値が null/undefined/空文字のキーは削除する。
 * @param {Record<string, string|number|null|undefined>} params
 * @param {{ replace?: boolean }} [options]  現状 replace 固定(将来の拡張用に残す)
 */
export function setRouteParams(params, options = {}) {
  void options;
  const { route, params: current } = parseHash();
  Object.entries(params || {}).forEach(([key, value]) => {
    if (value === null || value === undefined || value === "") {
      current.delete(key);
    } else {
      current.set(key, String(value));
    }
  });
  const qs = current.toString();
  const newHash = "#" + route + (qs ? "?" + qs : "");
  if (window.location.hash === newHash) {
    return;
  }
  const url = window.location.pathname + window.location.search + newHash;
  window.history.replaceState(window.history.state, "", url);
  window.dispatchEvent(
    new CustomEvent("sbop2:routeparamschange", { detail: { hash: window.location.hash } })
  );
}

/**
 * ルートを移行済みとして登録する。
 * @param {string} route  data-view / data-route の値
 * @param {() => void} mountFn  そのビューをマウントするコールバック
 */
export function registerRoute(route, mountFn) {
  _registry.set(route, mountFn);
}

/**
 * 指定ルートが移行済みか確認する。
 * @param {string} route
 * @returns {boolean}
 */
export function isMigrated(route) {
  return _registry.has(route);
}

/**
 * hash 変更時に呼び出す。移行済みなら mountFn を実行し true を返す。
 * 未移行なら何もせず false を返す(レガシーに委ねる)。
 * @param {string} route
 * @returns {boolean}
 */
export function handleRoute(route) {
  const fn = _registry.get(route);
  if (fn) {
    // ビューの mount(container) にはルートと同名の data-view コンテナを渡す。
    // 検索の起点を document 固定にしないのは、編集ペインを別ウィンドウへ
    // 移した時（workspace-layout.js のポップアップ）に見失わないため。
    const container = _viewRoot.querySelector('.view[data-view="' + route + '"]');
    if (!container) {
      console.error("router: data-view container not found for route:", route);
      return false;
    }
    fn(container);
    return true;
  }
  return false;
}

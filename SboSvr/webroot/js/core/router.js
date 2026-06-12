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
 * 現在の hash からルート名を取得する。
 * @returns {string}
 */
export function currentRoute() {
  return window.location.hash.replace(/^#/, "");
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
    // ビューの mount(container) にはルートと同名の data-view コンテナを渡す
    const container = document.querySelector('.view[data-view="' + route + '"]');
    if (!container) {
      console.error("router: data-view container not found for route:", route);
      return false;
    }
    fn(container);
    return true;
  }
  return false;
}

/**
 * core/dom.js
 * DOM 生成ヘルパと文字列エスケープ小物。
 */

/**
 * 要素を生成して返す。
 * @param {string} tag  タグ名
 * @param {Record<string, string|boolean|null|undefined>} [attrs]  属性オブジェクト
 *   - boolean true  → 属性を設定(値なし / "")
 *   - null/undefined → スキップ
 * @param {(string|Node)[]} [children]  子ノード(文字列はテキストノードになる)
 * @returns {HTMLElement}
 */
export function el(tag, attrs = {}, children = []) {
  const node = document.createElement(tag);
  for (const [key, value] of Object.entries(attrs)) {
    if (value == null) {
      continue;
    }
    if (value === true) {
      node.setAttribute(key, "");
    } else if (value !== false) {
      node.setAttribute(key, String(value));
    }
  }
  for (const child of children) {
    if (child == null) {
      continue;
    }
    node.append(typeof child === "string" ? document.createTextNode(child) : child);
  }
  return node;
}

/**
 * HTML 特殊文字をエスケープして文字列を返す。
 * @param {unknown} value
 * @returns {string}
 */
export function escapeHtml(value) {
  return String(value ?? "")
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#39;");
}

/**
 * セレクタで要素を取得する。見つからなければ null を返す。
 * @param {string} selector
 * @param {ParentNode} [root]
 * @returns {HTMLElement|null}
 */
export function qs(selector, root = document) {
  return root.querySelector(selector);
}

/**
 * セレクタで全要素を取得する。
 * @param {string} selector
 * @param {ParentNode} [root]
 * @returns {HTMLElement[]}
 */
export function qsa(selector, root = document) {
  return Array.from(root.querySelectorAll(selector));
}

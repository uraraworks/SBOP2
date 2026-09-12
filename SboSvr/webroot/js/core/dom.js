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

const BUSY_LABEL = new WeakMap();

/**
 * ボタンを押した非同期処理を実行中は disabled にし、文言を「保存中…」等に差し替える。
 * 実行中の二重クリックは無視する(同じボタンで実行中なら何もしない)。
 * @template T
 * @param {HTMLButtonElement} button
 * @param {() => Promise<T>} asyncFn
 * @param {{ busyText?: string }} [options]  busyText 省略時は "保存中…"
 * @returns {Promise<T|undefined>}  二重実行で無視した場合は undefined
 */
export async function withBusy(button, asyncFn, options = {}) {
  if (!button) {
    return asyncFn();
  }
  if (button.dataset.busy === "1") {
    // 実行中の二重クリック防止
    return undefined;
  }
  const busyText = options.busyText ?? "保存中…";
  button.dataset.busy = "1";
  const wasDisabled = button.disabled;
  if (!BUSY_LABEL.has(button)) {
    BUSY_LABEL.set(button, button.textContent);
  }
  button.disabled = true;
  button.textContent = busyText;
  try {
    return await asyncFn();
  } finally {
    delete button.dataset.busy;
    button.disabled = wasDisabled;
    const originalText = BUSY_LABEL.get(button);
    if (originalText != null) {
      button.textContent = originalText;
    }
    BUSY_LABEL.delete(button);
  }
}

// ----------------------------------------------------------------
// 二度押し確定ボタン（confirm() の代わり）
//
// confirm() は使わない。編集ペインを別ウィンドウへポップアウトしていると、
// ダイアログはスクリプトの所属する「元のウィンドウ」に出るため、背後に隠れて
// 見えず「押しても何も起きない」状態になる。
// 代わりに 1 回目のクリックで文言を変えて確認し、2 回目で実行する。
// 元は views/image-editor.js のローカル実装。他画面でも使うため共通化した。
// ----------------------------------------------------------------

function positionArmMessage(msgEl, button) {
  const rect = button.getBoundingClientRect();
  const msgRect = msgEl.getBoundingClientRect();
  let left = rect.right - msgRect.width;
  if (left < 4) {
    left = 4;
  }
  msgEl.style.top = (rect.bottom + 6) + "px";
  msgEl.style.left = left + "px";
}

/**
 * ボタンを二度押し確定方式にする。
 * 1 回目のクリックでボタンを強調状態にし、timeoutMs 以内の 2 回目のクリックで
 * onConfirm を実行する。timeoutMs 経過やフォーカスが外れると元に戻る。
 * @param {HTMLButtonElement} button
 * @param {{
 *   onConfirm: () => (void|Promise<void>),
 *   armedLabel?: string,  省略時は「本当に削除？（もう一度押す）」
 *   timeoutMs?: number,   省略時は 4000ms
 *   message?: string | (() => (string|undefined|Promise<string|undefined>)),
 *     アーム中にボタン近くへ表示する警告文（例: 参照件数）。
 *     関数を渡すと 1 回目のクリック時に毎回呼び直す（押すたびに件数を取り直す等）。
 *     Promise を返す場合、解決するまで「確認中…」を表示し、その間に 2 回目の
 *     クリックが来ても件数が確定するまでは onConfirm を実行しない。
 *     取得に失敗した場合は「参照件数を確認できませんでした」を表示してアームは継続する。
 * }} options
 * @returns {{ disarm: () => void }}
 */
export function armConfirmButton(button, options = {}) {
  const {
    onConfirm,
    armedLabel = "本当に削除？（もう一度押す）",
    timeoutMs = 4000,
    message,
  } = options;

  // アイコンを子要素として持つボタンでは button.textContent への代入で
  // アイコン(SVG)ごと消えてしまう。ラベル用の <span class="btn-label"> が
  // あればそちらだけを差し替え、無いボタンでは従来どおり textContent を使う。
  const labelEl = button.querySelector(".btn-label");
  const getLabel = () => (labelEl ? labelEl.textContent : button.textContent);
  const setLabel = (text) => {
    if (labelEl) labelEl.textContent = text;
    else button.textContent = text;
  };

  const normalLabel = getLabel();
  let armed = false;
  let loading = false;
  let timer = null;
  let msgEl = null;

  const removeMsg = () => {
    if (msgEl) {
      msgEl.remove();
      msgEl = null;
    }
  };

  const showMsg = (text) => {
    removeMsg();
    if (!text) {
      return;
    }
    const doc = button.ownerDocument || document;
    msgEl = doc.createElement("span");
    msgEl.className = "arm-confirm-message";
    msgEl.textContent = text;
    (doc.body || doc.documentElement).appendChild(msgEl);
    positionArmMessage(msgEl, button);
  };

  const disarm = () => {
    armed = false;
    setLabel(normalLabel);
    button.classList.remove("is-armed");
    if (timer) {
      clearTimeout(timer);
      timer = null;
    }
    removeMsg();
  };

  button.addEventListener("blur", () => {
    if (armed) {
      disarm();
    }
  });

  button.addEventListener("click", async () => {
    if (loading) {
      // 件数取得中の連打は無視(確定させない)。
      return;
    }
    if (!armed) {
      if (typeof message === "function") {
        loading = true;
        const wasDisabled = button.disabled;
        button.disabled = true;
        setLabel(armedLabel);
        button.classList.add("is-armed");
        showMsg("確認中…");
        let resolved;
        try {
          resolved = await message();
        } catch (e) {
          resolved = "参照件数を確認できませんでした";
        }
        loading = false;
        button.disabled = wasDisabled;
        armed = true;
        showMsg(resolved);
        timer = setTimeout(disarm, timeoutMs);
        return;
      }
      armed = true;
      setLabel(armedLabel);
      button.classList.add("is-armed");
      showMsg(message);
      timer = setTimeout(disarm, timeoutMs);
      return;
    }
    disarm();
    onConfirm();
  });

  return { disarm };
}

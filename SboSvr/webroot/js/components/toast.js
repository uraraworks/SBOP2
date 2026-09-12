/**
 * components/toast.js
 * 画面右下に一定時間表示する通知トースト。success / error / info の3種。
 * エラーはクリックで詳細(サーバーの message 等)を展開表示できる。
 *
 * 表示先は常にメインウィンドウの document 固定。
 * workspace-layout.js で編集ペインが別ウィンドウ(popup)へ移ることがあるが、
 * トーストは操作結果の通知でありメインウィンドウのユーザーへ見せれば十分なため
 * 追従させていない(popup 側は常時前面ではない小窓なので、そちらに出しても
 * 見落とされやすい)。将来 popup 側にも出したくなったら
 * workspace-layout.js の isPoppedOut() を見て投げ分けること。
 */

const CONTAINER_ID = "sbop2-toast-container";
const DURATION_MS = 3000;
const DURATION_MS_ERROR = 6000;
const DURATION_MS_ACTION = 8000;
const REMOVE_FALLBACK_MS = 400;

/** @type {HTMLElement|null} */
let _container = null;

function ensureContainer() {
  if (_container && _container.isConnected) {
    return _container;
  }
  let container = document.getElementById(CONTAINER_ID);
  if (!container) {
    container = document.createElement("div");
    container.id = CONTAINER_ID;
    container.className = "toast-container";
    container.setAttribute("aria-live", "polite");
    document.body.appendChild(container);
  }
  _container = container;
  return container;
}

/**
 * トーストを表示する。
 * @param {string} message  表示文言
 * @param {"success"|"error"|"info"} [type="info"]
 * @param {{ detail?: string, action?: { label: string, onClick: () => void }, duration?: number }} [options]
 *   detail: error 時にクリックで展開する詳細文。
 *   action: ボタン付きトースト(例:「編集を開く」)。指定時は既定表示時間が 8 秒になる。
 *   duration: 表示時間(ms)を明示指定する。0 以下を指定すると自動消去しない。
 *     指定時は action や type による既定値より優先される。
 * @returns {{ dismiss: () => void }}
 */
export function showToast(message, type = "info", options = {}) {
  const text = String(message ?? "").trim();
  if (!text) {
    return { dismiss() {} };
  }
  const container = ensureContainer();

  const item = document.createElement("div");
  item.className = `toast toast-${type}`;
  item.setAttribute("role", type === "error" ? "alert" : "status");

  const msgEl = document.createElement("div");
  msgEl.className = "toast-message";
  msgEl.textContent = text;
  item.appendChild(msgEl);

  let dismissed = false;
  let timer = null;

  function dismiss() {
    if (dismissed) return;
    dismissed = true;
    if (timer) clearTimeout(timer);
    item.classList.remove("is-visible");
    item.addEventListener("transitionend", () => item.remove(), { once: true });
    // transitionend が発火しない環境(prefers-reduced-motion 等)向けの保険
    setTimeout(() => item.remove(), REMOVE_FALLBACK_MS);
  }

  const detail = options.detail ? String(options.detail).trim() : "";
  let detailEl = null;
  let expanded = false;
  if (type === "error" && detail) {
    item.classList.add("is-expandable");
    item.title = "クリックで詳細を表示";
    item.tabIndex = 0;
    const toggle = () => {
      expanded = !expanded;
      if (!detailEl) {
        detailEl = document.createElement("div");
        detailEl.className = "toast-detail";
        detailEl.textContent = detail;
        item.appendChild(detailEl);
      }
      detailEl.style.display = expanded ? "" : "none";
      // 詳細を読んでいる間に自動消去されないよう、展開したらタイマーを止める
      if (expanded && timer) {
        clearTimeout(timer);
        timer = null;
      }
    };
    item.addEventListener("click", toggle);
    item.addEventListener("keydown", (event) => {
      if (event.key === "Enter" || event.key === " ") {
        event.preventDefault();
        toggle();
      }
    });
  }

  if (type === "error") {
    const closeBtn = document.createElement("button");
    closeBtn.type = "button";
    closeBtn.className = "toast-close";
    closeBtn.setAttribute("aria-label", "閉じる");
    closeBtn.textContent = "×";
    closeBtn.addEventListener("click", (event) => {
      event.stopPropagation();
      dismiss();
    });
    item.appendChild(closeBtn);
  }

  const action = options.action && typeof options.action.onClick === "function" ? options.action : null;
  if (action) {
    const actionRow = document.createElement("div");
    actionRow.className = "toast-action-row";
    const actionBtn = document.createElement("button");
    actionBtn.type = "button";
    actionBtn.className = "toast-action";
    actionBtn.textContent = action.label || "実行";
    actionBtn.addEventListener("click", (event) => {
      event.stopPropagation();
      try {
        action.onClick();
      } finally {
        dismiss();
      }
    });
    actionRow.appendChild(actionBtn);
    item.appendChild(actionRow);
  }

  container.appendChild(item);
  // 追加直後は transition 前提のクラス無し状態から開始し、次フレームで is-visible を付与してフェードインさせる
  requestAnimationFrame(() => item.classList.add("is-visible"));

  const hasCustomDuration = typeof options.duration === "number" && !Number.isNaN(options.duration);
  const duration = hasCustomDuration
    ? options.duration
    : (action ? DURATION_MS_ACTION : (type === "error" ? DURATION_MS_ERROR : DURATION_MS));
  if (duration > 0) {
    timer = setTimeout(dismiss, duration);
  }

  return { dismiss };
}

/** @param {string} message */
export function showSuccessToast(message) {
  return showToast(message, "success");
}

/**
 * @param {string} message
 * @param {string} [detail]  クリックで展開する詳細文
 */
export function showErrorToast(message, detail) {
  return showToast(message, "error", { detail });
}

/**
 * @param {string} message
 * @param {{ detail?: string, action?: { label: string, onClick: () => void }, duration?: number }} [options]
 */
export function showInfoToast(message, options) {
  return showToast(message, "info", options || {});
}

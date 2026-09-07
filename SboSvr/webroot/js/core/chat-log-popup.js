/**
 * core/chat-log-popup.js
 * ゲーム画面(iframe)内のチャットログを、別ウィンドウへミラー表示する。
 *
 * 方式(ミラー):
 *   iframe 内の DOM は C++ 側が要素を ID で引いているため、そのまま別ウィンドウへ
 *   移動させることはできない(workspace-layout.js の編集ペインとは事情が異なる)。
 *   そこでゲーム内のチャット表示はそのまま残し、内容だけ postMessage で配信して
 *   別ウィンドウ側に大きく描画し直す。
 *
 * メッセージ種別 (kind):
 *   iframe → 親:
 *     sbop2_chat_log          1行追加 { text, color }
 *     sbop2_chat_log_snapshot 過去ログ一括 { logs: [{text,color}, ...] }
 *   親 → iframe:
 *     sbop2_chat_log_request  過去ログ要求(別ウィンドウを開いた直後に送る)
 *
 * targetOrigin には window.location.origin を使う流儀に合わせる。
 */

const LOG_LINE_MAX = 500; // 別ウィンドウ側の最大保持行数

let _adminGameFrame = null;
let _popup = null;
let _popupWatch = null;
let _popupLogEl = null;
let _button = null;

// ----------------------------------------------------------------
// ポップアップ本体
// ----------------------------------------------------------------

function isOpen() {
  return !!_popup && !_popup.closed;
}

function isAtBottom(el) {
  return el.scrollHeight - el.scrollTop - el.clientHeight < 8;
}

function scrollToBottom(el) {
  el.scrollTop = el.scrollHeight;
}

function appendLine(text, color) {
  if (!_popupLogEl) return;
  const shouldFollow = isAtBottom(_popupLogEl);
  const line = _popup.document.createElement("div");
  line.className = "chat-log-line";
  line.textContent = text;
  if (color) { line.style.color = color; }
  _popupLogEl.appendChild(line);
  while (_popupLogEl.children.length > LOG_LINE_MAX) {
    _popupLogEl.removeChild(_popupLogEl.firstChild);
  }
  if (shouldFollow) {
    scrollToBottom(_popupLogEl);
  }
}

function renderSnapshot(logs) {
  if (!_popupLogEl || !Array.isArray(logs)) return;
  _popupLogEl.innerHTML = "";
  logs.slice(-LOG_LINE_MAX).forEach((entry) => {
    if (!entry) return;
    appendLine(String(entry.text || ""), entry.color || null);
  });
}

function buildPopupDocument(popup) {
  const doc = popup.document;
  doc.title = "SBOP2 チャットログ";
  doc.body.style.margin = "0";
  doc.body.style.background = "#0b1220";
  doc.body.style.color = "#e2e8f0";
  doc.body.style.fontFamily = "\"BIZ UDGothic\", \"Yu Gothic\", sans-serif";

  const wrap = doc.createElement("div");
  wrap.id = "chat-log-popup-wrap";
  wrap.style.boxSizing = "border-box";
  wrap.style.height = "100vh";
  wrap.style.overflowY = "auto";
  wrap.style.padding = "0.75rem 1rem";
  wrap.style.fontSize = "1.15rem";
  wrap.style.lineHeight = "1.6";
  doc.body.appendChild(wrap);

  const style = doc.createElement("style");
  style.textContent = ".chat-log-line { white-space: pre-wrap; word-break: break-word; }";
  doc.head.appendChild(style);

  return wrap;
}

function requestSnapshot() {
  if (!_adminGameFrame || !_adminGameFrame.contentWindow) return;
  _adminGameFrame.contentWindow.postMessage({ kind: "sbop2_chat_log_request" }, window.location.origin);
}

function openPopup() {
  if (isOpen()) { _popup.focus(); return; }

  const popup = window.open("", "sbop2-chat-log", "popup=yes,width=520,height=760");
  if (!popup) {
    alert("ポップアップがブロックされました。このサイトのポップアップを許可してください。");
    return;
  }
  _popup = popup;
  _popupLogEl = buildPopupDocument(popup);
  updateButton();

  // 過去ログを埋める(iframe 側が持っているスナップショットを要求する)
  requestSnapshot();

  // 閉じられたら監視を止めて後始末する(workspace-layout.js の作法に合わせる)
  popup.addEventListener("pagehide", closePopup);
  _popupWatch = window.setInterval(() => {
    if (!_popup || _popup.closed) closePopup();
  }, 500);

  popup.focus();
}

function closePopup() {
  if (_popupWatch) { window.clearInterval(_popupWatch); _popupWatch = null; }
  if (!_popup) return;
  const popup = _popup;
  _popup = null;
  _popupLogEl = null;
  try { if (!popup.closed) popup.close(); } catch { /* 既に閉じている */ }
  updateButton();
}

function updateButton() {
  if (!_button) return;
  _button.textContent = isOpen() ? "チャットログを閉じる" : "チャットログを別ウィンドウで開く";
  _button.classList.toggle("is-selected", isOpen());
}

// ----------------------------------------------------------------
// iframe からのメッセージ受信
// ----------------------------------------------------------------

function onMessage(ev) {
  if (_adminGameFrame && ev.source !== _adminGameFrame.contentWindow) return;
  const msg = ev.data;
  if (!msg || typeof msg !== "object") return;

  if (msg.kind === "sbop2_chat_log") {
    if (isOpen()) {
      appendLine(String(msg.text || ""), msg.color || null);
    }
  } else if (msg.kind === "sbop2_chat_log_snapshot") {
    if (isOpen()) {
      renderSnapshot(msg.logs);
    }
  }
}

// ----------------------------------------------------------------
// 組み立て
// ----------------------------------------------------------------

/**
 * 起動時に一度だけ呼ぶ。workspace-layout.js が作るタブバーの隣にボタンを足す。
 */
export function initChatLogPopup() {
  _adminGameFrame = document.getElementById("admin-game-frame");
  const bar = document.querySelector(".workspace-tabs");
  if (!_adminGameFrame || !bar) return;

  const button = document.createElement("button");
  button.type = "button";
  button.className = "button small workspace-popup-button";
  button.addEventListener("click", () => { isOpen() ? closePopup() : openPopup(); });
  _button = button;
  bar.appendChild(button);
  updateButton();

  window.addEventListener("message", onMessage);

  // 親を閉じたらポップアップも道連れにする
  window.addEventListener("beforeunload", () => {
    if (_popup && !_popup.closed) {
      try { _popup.close(); } catch { /* 無視 */ }
    }
  });
}

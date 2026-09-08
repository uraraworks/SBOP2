/**
 * core/workspace-layout.js
 * ワークスペースの表示切り替え（ゲーム画面 / 並べて / 編集）とポップアップ
 *
 * 背景:
 *   .workspace-body はゲーム画面 iframe (aside) と編集ペイン (main) の 2 カラム。
 *   ドット絵編集のようにゲーム画面を見ない作業では、編集側が細長い列に
 *   押し込まれて上下スクロールが辛い。そこで表示するカラムを切り替える。
 *
 * ポップアップについて:
 *   同一オリジンなので、編集ペインの DOM を別ウィンドウへ adoptNode で
 *   移し替えられる。canvas の内容もイベントリスナーも生きたまま移動する。
 *   ゲーム画面 (iframe) の方を移さないのは、iframe は別 document へ移すと
 *   仕様上リロードが走り、Web クライアントが再起動してしまうため。
 *
 *   移動中もルーターが表示先を見つけられるよう、router 側は document ではなく
 *   この要素参照から検索する（setViewRoot）。
 */

import { setViewRoot } from "./router.js";

const STORAGE_KEY = "sbop2.workspaceLayout";
const MODES = ["game", "both", "editor"];
const MODE_LABELS = { game: "ゲーム画面", both: "並べて表示", editor: "編集" };

// ポップアップ側 body に付けるクラス（CSS でサイズを上書きする）
const POPUP_BODY_CLASS = "sbop2-popup-editor";

let _workspaceBody = null;
let _gamePanel = null;
let _viewContainer = null;
let _mode = "both";
let _popup = null;
let _popupWatch = null;
let _modeButtons = new Map();
let _popupButton = null;

// ----------------------------------------------------------------
// モード切り替え
// ----------------------------------------------------------------

function applyMode() {
  if (!_workspaceBody) return;
  MODES.forEach((m) => _workspaceBody.classList.toggle(`layout-${m}`, m === _mode));
  _modeButtons.forEach((btn, m) => {
    btn.classList.toggle("is-selected", m === _mode);
    btn.setAttribute("aria-pressed", String(m === _mode));
  });
  // ポップアップ中は編集ペインが手元に無いので、モード選択自体を無効にする
  const popped = isPoppedOut();
  _modeButtons.forEach((btn, m) => { btn.disabled = popped && m !== "game"; });
}

export function setMode(mode) {
  if (!MODES.includes(mode)) return;
  _mode = mode;
  try { localStorage.setItem(STORAGE_KEY, mode); } catch { /* プライベートモード等では無視 */ }
  applyMode();
}

// ----------------------------------------------------------------
// ポップアップ
// ----------------------------------------------------------------

export function isPoppedOut() {
  return !!_popup && !_popup.closed;
}

/** 親ドキュメントのスタイルシートをポップアップへ複製する */
function copyStyles(doc) {
  document.querySelectorAll('link[rel="stylesheet"]').forEach((link) => {
    const copy = doc.createElement("link");
    copy.rel = "stylesheet";
    copy.href = link.href; // href は絶対 URL に解決済み
    doc.head.appendChild(copy);
  });
  document.querySelectorAll("style").forEach((style) => {
    const copy = doc.createElement("style");
    copy.textContent = style.textContent;
    doc.head.appendChild(copy);
  });
}

// ポップアップ中、confirm/alert/prompt は「元のウィンドウ」に出る。
// スクリプトが所属する realm が元ウィンドウのままだからで、これは避けられない。
// そのまま出すと別ウィンドウの背後に隠れて「押しても何も起きない」ように見えるので、
// ダイアログを出す前に元ウィンドウを手前に持ってくる。
//
// 画像エディタの破壊的な操作は 2 段階クリックに置き換えてあるのでここを通らないが、
// 他の画面（キャラ編集など）はまだ confirm/alert を使っているための保険。
const _nativeDialogs = {};

function installDialogFocusGuard() {
  ["confirm", "alert", "prompt"].forEach((name) => {
    if (_nativeDialogs[name]) return;
    const original = window[name].bind(window);
    _nativeDialogs[name] = original;
    window[name] = (...args) => {
      try { window.focus(); } catch { /* 無視 */ }
      return original(...args);
    };
  });
}

function removeDialogFocusGuard() {
  Object.keys(_nativeDialogs).forEach((name) => {
    window[name] = _nativeDialogs[name];
    delete _nativeDialogs[name];
  });
}

function popOut() {
  if (isPoppedOut()) { _popup.focus(); return; }

  const popup = window.open("", "sbop2-editor", "popup=yes,width=1440,height=900");
  if (!popup) {
    alert("ポップアップがブロックされました。このサイトのポップアップを許可してください。");
    return;
  }
  _popup = popup;

  const doc = popup.document;
  doc.title = "SBOP2 編集ウィンドウ";
  doc.body.className = POPUP_BODY_CLASS;
  copyStyles(doc);

  // 編集ペインごと移す。イベントリスナーも canvas の内容もそのまま生き残る。
  doc.body.appendChild(doc.adoptNode(_viewContainer));

  // 手元にはゲーム画面しか残らないのでそちらを全面表示にする
  setMode("game");
  updatePopupButton();
  installDialogFocusGuard();

  // 閉じられたら戻す。pagehide だけだと取りこぼす環境があるので併用する。
  popup.addEventListener("pagehide", popIn);
  _popupWatch = setInterval(() => { if (!_popup || _popup.closed) popIn(); }, 500);

  popup.focus();
}

function popIn() {
  if (_popupWatch) { clearInterval(_popupWatch); _popupWatch = null; }
  if (!_popup) return;

  // ポップアップ側に残っている編集ペインを引き取る
  if (_viewContainer && _viewContainer.ownerDocument !== document) {
    _workspaceBody.appendChild(document.adoptNode(_viewContainer));
  }
  const popup = _popup;
  _popup = null;
  try { if (!popup.closed) popup.close(); } catch { /* 既に閉じている */ }
  removeDialogFocusGuard();

  setMode("both");
  updatePopupButton();
}

function updatePopupButton() {
  if (!_popupButton) return;
  const popped = isPoppedOut();
  _popupButton.textContent = popped ? "編集を元に戻す" : "編集を別ウィンドウで開く";
  _popupButton.classList.toggle("is-selected", popped);
}

// ----------------------------------------------------------------
// 組み立て
// ----------------------------------------------------------------

function buildTabBar() {
  const bar = document.createElement("div");
  bar.className = "workspace-tabs";

  const group = document.createElement("div");
  group.className = "workspace-tab-group";
  group.setAttribute("role", "group");
  group.setAttribute("aria-label", "表示の切り替え");

  MODES.forEach((mode) => {
    const btn = document.createElement("button");
    btn.type = "button";
    btn.className = "button small workspace-tab";
    btn.textContent = MODE_LABELS[mode];
    btn.addEventListener("click", () => {
      // ポップアップ中に「並べて」「編集」を押したら呼び戻す
      if (isPoppedOut() && mode !== "game") { popIn(); }
      setMode(mode);
    });
    _modeButtons.set(mode, btn);
    group.appendChild(btn);
  });

  const popupBtn = document.createElement("button");
  popupBtn.type = "button";
  popupBtn.className = "button small workspace-popup-button";
  popupBtn.addEventListener("click", () => { isPoppedOut() ? popIn() : popOut(); });
  _popupButton = popupBtn;

  bar.append(group, popupBtn);
  return bar;
}

/**
 * 起動時に一度だけ呼ぶ。
 */
export function initWorkspaceLayout() {
  _workspaceBody = document.querySelector(".workspace-body");
  _gamePanel = document.getElementById("admin-game-panel");
  _viewContainer = document.getElementById("view-container");
  if (!_workspaceBody || !_viewContainer) return;

  // ルーターが document 依存だと、ポップアップへ移した後に表示先を見失う
  setViewRoot(_viewContainer);

  _workspaceBody.parentNode.insertBefore(buildTabBar(), _workspaceBody);

  let saved = null;
  try { saved = localStorage.getItem(STORAGE_KEY); } catch { saved = null; }
  setMode(MODES.includes(saved) ? saved : "both");

  updatePopupButton();

  // 親を閉じたらポップアップも道連れにする（迷子のウィンドウを残さない）
  window.addEventListener("beforeunload", () => {
    if (_popup && !_popup.closed) {
      try { _popup.close(); } catch { /* 無視 */ }
    }
  });
}

const adminLogoutButton = document.getElementById("admin-logout-button");
const adminLoginForm = document.getElementById("admin-login-form");
const adminLoginIdInput = document.getElementById("admin-login-id");
const adminLoginPasswordInput = document.getElementById("admin-login-password");
const adminLoginSubmitButton = document.getElementById("admin-login-submit");
const adminLoginErrorEl = document.getElementById("admin-login-error");

function updateAppViewportHeight() {
  const viewport = window.visualViewport;
  const height = viewport && viewport.height ? viewport.height : window.innerHeight;
  if (height > 0) {
    document.documentElement.style.setProperty("--app-height", `${height}px`);
    syncAdminGameViewportHeight(height);
  }
}

function syncAdminGameViewportHeight(height = null) {
  const frame = document.getElementById("admin-game-frame");
  const viewport = window.visualViewport;
  const nextHeight = height || (viewport && viewport.height ? viewport.height : window.innerHeight);
  if (!frame || !frame.contentWindow || !nextHeight || nextHeight < 240) {
    return;
  }
  frame.contentWindow.postMessage({ kind: "sbop2_admin_viewport", height: Math.floor(nextHeight) }, "*");
}

updateAppViewportHeight();
window.addEventListener("resize", updateAppViewportHeight);
window.addEventListener("orientationchange", updateAppViewportHeight);
if (window.visualViewport) {
  window.visualViewport.addEventListener("resize", updateAppViewportHeight);
  window.visualViewport.addEventListener("scroll", updateAppViewportHeight);
}

/* F3〜F9 移行済み: 各 views/*.js が DOM・状態・イベント登録を担当 */

/* adminGameFrame / adminGameMissingEl: iframe 連携共通基盤として残す */
const adminGameFrame = document.getElementById("admin-game-frame");
const adminGameMissingEl = document.getElementById("admin-game-missing");

/* ------------------------------------------------------------------ */
/* /ws/admin WebSocket クライアント                                      */
/* ------------------------------------------------------------------ */

const adminWs = {
  socket: null,
  retryDelay: 1000,   // 次回再接続までの待機時間 (ms)
  retryTimer: null
};

/** ws:// または wss:// の URL を返す。 */
function getAdminWsUrl() {
  const proto = location.protocol === "https:" ? "wss" : "ws";
  return `${proto}://${location.host}/ws/admin`;
}

/** 管理者 WebSocket を 1 本だけ確立する。既に接続中なら何もしない。 */
function ensureAdminWebSocket() {
  if (adminWs.socket &&
      (adminWs.socket.readyState === WebSocket.OPEN ||
       adminWs.socket.readyState === WebSocket.CONNECTING)) {
    return;
  }

  const ws = new WebSocket(getAdminWsUrl());
  adminWs.socket = ws;

  ws.addEventListener("open", () => {
    console.debug("[admin-ws] 接続しました");
    adminWs.retryDelay = 1000; // 接続成功でバックオフをリセット
  });

  ws.addEventListener("message", (event) => {
    let msg;
    try {
      msg = JSON.parse(event.data);
    } catch {
      console.debug("[admin-ws] JSON パース失敗:", event.data);
      return;
    }
    if (msg.kind === "selection_changed") {
      console.debug("[admin-ws] selection_changed", msg.payload);
      // map-window.js に移行済みのため window._ API 経由で通知
      if (typeof window._mapWindowApplySelection === "function") {
        window._mapWindowApplySelection(msg.payload);
      }
    } else {
      console.debug("[admin-ws] 受信:", msg);
    }
  });

  ws.addEventListener("close", (event) => {
    console.debug("[admin-ws] 切断 (code:", event.code, ") — 再接続まで", adminWs.retryDelay, "ms");
    adminWs.socket = null;
    // 指数バックオフで再接続（最短 1s、最大 30s）
    adminWs.retryTimer = setTimeout(() => {
      adminWs.retryTimer = null;
      ensureAdminWebSocket();
    }, adminWs.retryDelay);
    adminWs.retryDelay = Math.min(adminWs.retryDelay * 2, 30000);
  });

  ws.addEventListener("error", () => {
    // close イベントが後続するので再接続処理はそちらに任せる
    console.debug("[admin-ws] エラー発生");
  });
}

/* formatHex / getWeatherLabel は map-window.js に内蔵済みのため削除 */
/* normalizeMapObject / normalizeMapEntry は map-window.js に内蔵済みのため削除 */
/* setMapObjectLoading / setMapObjectError / loadMapObjectData は F4 移行のため削除 */



const DEFAULT_ROUTE = "server-dashboard";
const views = document.querySelectorAll("[data-view]");
const navLinks = document.querySelectorAll("[data-route]");
const navGroups = document.querySelectorAll(".main-nav details");
let currentRoute = null;
let adminWorkspaceInitialized = false;

// ----------------------------------------------------------------
// ナビゲーション: メニュー開閉状態の記憶 (1-8)
// localStorage が使えない環境(プライベートモード等)でも既定=全展開で
// 動くよう、失敗は try/catch で無視する。
// ----------------------------------------------------------------

const NAV_DETAILS_STORAGE_KEY = "sbop2.navOpenGroups";

function navDetailsKey(details, index) {
  const summary = details.querySelector("summary");
  return summary ? summary.textContent.trim() : `group-${index}`;
}

function restoreNavDetailsState() {
  let saved = null;
  try {
    saved = JSON.parse(localStorage.getItem(NAV_DETAILS_STORAGE_KEY) || "null");
  } catch {
    saved = null;
  }
  if (!saved || typeof saved !== "object") {
    return; // 保存が無い/壊れている場合は index.html の既定(open)のまま
  }
  navGroups.forEach((details, index) => {
    const key = navDetailsKey(details, index);
    if (Object.prototype.hasOwnProperty.call(saved, key)) {
      details.open = !!saved[key];
    }
  });
}

function persistNavDetailsState() {
  const state = {};
  navGroups.forEach((details, index) => {
    state[navDetailsKey(details, index)] = details.open;
  });
  try {
    localStorage.setItem(NAV_DETAILS_STORAGE_KEY, JSON.stringify(state));
  } catch {
    // プライベートモード等では無視
  }
}

navGroups.forEach((details) => {
  details.addEventListener("toggle", persistNavDetailsState);
});
restoreNavDetailsState();

// ----------------------------------------------------------------
// ナビゲーション: 最近使った画面 (1-8)
// ルート遷移のたびに記録し、メニュー最上部に表示する。
// DEFAULT_ROUTE とログイン画面(未認証状態)は記録しない。
// ----------------------------------------------------------------

const RECENT_ROUTES_STORAGE_KEY = "sbop2.recentRoutes";
const RECENT_ROUTES_MAX = 5;
const mainNavEl = document.querySelector(".main-nav");
let recentRoutesSection = null;

function getRouteLabel(route) {
  const link = Array.from(navLinks).find((l) => l.dataset.route === route);
  return link ? link.textContent.trim() : "";
}

function loadRecentRoutes() {
  try {
    const raw = JSON.parse(localStorage.getItem(RECENT_ROUTES_STORAGE_KEY) || "[]");
    return Array.isArray(raw) ? raw.filter((r) => typeof r === "string") : [];
  } catch {
    return [];
  }
}

function saveRecentRoutes(list) {
  try {
    localStorage.setItem(RECENT_ROUTES_STORAGE_KEY, JSON.stringify(list));
  } catch {
    // プライベートモード等では無視
  }
}

function handleRecentRouteClick(event) {
  event.preventDefault();
  const link = event.currentTarget;
  const targetRoute = link.dataset.route;
  if (!targetRoute) {
    return;
  }
  const currentHash = stripRouteQuery(window.location.hash.replace(/^#/, ""));
  if (currentHash === targetRoute) {
    activateRoute(targetRoute, { forceReload: true });
  } else {
    window.location.hash = `#${targetRoute}`;
  }
}

function renderRecentRoutes() {
  if (!mainNavEl) {
    return;
  }
  if (!recentRoutesSection) {
    recentRoutesSection = document.createElement("div");
    recentRoutesSection.className = "main-nav-recent";
    const header = mainNavEl.querySelector(".main-nav-header");
    if (header) {
      header.insertAdjacentElement("afterend", recentRoutesSection);
    } else {
      mainNavEl.insertBefore(recentRoutesSection, mainNavEl.firstChild);
    }
  }
  const list = loadRecentRoutes()
    .map((route) => ({ route, label: getRouteLabel(route) }))
    .filter((entry) => entry.label);
  if (!list.length) {
    recentRoutesSection.innerHTML = "";
    recentRoutesSection.hidden = true;
    return;
  }
  recentRoutesSection.hidden = false;
  recentRoutesSection.innerHTML = '<div class="main-nav-recent-title">最近使った画面</div><ul></ul>';
  const ul = recentRoutesSection.querySelector("ul");
  list.forEach(({ route, label }) => {
    const li = document.createElement("li");
    const a = document.createElement("a");
    a.href = `#${route}`;
    a.dataset.route = route;
    a.className = "nav-link";
    a.textContent = label;
    a.addEventListener("click", handleRecentRouteClick);
    li.appendChild(a);
    ul.appendChild(li);
  });
}

function recordRecentRoute(route) {
  if (!route || route === DEFAULT_ROUTE) {
    return;
  }
  if (!document.body.classList.contains("admin-authorized")) {
    return; // ログイン画面状態(未認証)では記録しない
  }
  if (!getRouteLabel(route)) {
    return; // メニューに存在しないルートは対象外
  }
  const list = loadRecentRoutes().filter((r) => r !== route);
  list.unshift(route);
  saveRecentRoutes(list.slice(0, RECENT_ROUTES_MAX));
  renderRecentRoutes();
}

renderRecentRoutes();

function updateAdminGamePickInfo(message) {
  // pick 情報の常時表示は廃止。互換のため関数だけ残す（呼び出し元の no-op 化）
  void message;
}

function openCharacterDetailFromGame(charId) {
  const normalizedCharId = Number(charId);
  if (!Number.isFinite(normalizedCharId) || normalizedCharId <= 0) {
    return;
  }
  updateAdminGamePickInfo(`選択中のキャラ: ${normalizedCharId}`);
  // character-overview ビュー（ES module版）に通知する
  if (currentRoute === "character-overview" && typeof window._charEditMount === "function") {
    window._charEditMount(normalizedCharId);
  }
}

function handleAdminGamePick(message) {
  // ルート → ハンドラのレジストリ方式(core/game-pick.js)へまず振り分ける。
  // 登録ハンドラが処理した場合(true)は、確認ダイアログも含めハンドラ側の
  // 責務なのでここでは何もしない。false の時だけ以下の従来分岐にフォールバックする
  // (main.js が app.js より後に読み込まれるが、この関数はゲーム iframe からの
  //  postMessage で非同期に呼ばれるため、実行時点では既に main.js の評価が
  //  完了しており window.__gamePick は利用可能)。
  if (window.__gamePick && window.__gamePick.dispatchGamePick(message)) {
    return;
  }
  // 未保存の変更があれば確認する(dirty-guard.js)。
  // app.js は非 module のレガシースクリプトのため import できず、
  // main.js 側で window.__dirtyGuard として公開したものを参照する。
  if (window.__dirtyGuard && window.__dirtyGuard.isDirty() && !window.__dirtyGuard.confirmDiscard()) {
    return;
  }
  const charId = Number(message.charId) || 0;
  const itemId = Number(message.itemId) || 0;
  const mapId = Number(message.mapId) || 0;
  const cellX = Number(message.cellX) || 0;
  const cellY = Number(message.cellY) || 0;

  // ビュー別処理
  if (currentRoute === "character-overview") {
    // キャラ詳細ビュー: charId があれば詳細を表示
    if (charId > 0) {
      openCharacterDetailFromGame(charId);
      return;
    }
  } else if (currentRoute === "character-list") {
    // キャラ一覧ビュー: charId があれば character-overview へ遷移して詳細を表示
    // (core/game-pick.js の openCharacterEditor に統一。pendingCharId 経由なので
    //  hashchange が非同期でも mount 後に確実に反映される)
    if (charId > 0) {
      if (window.__gamePick) { window.__gamePick.openCharacterEditor(charId); }
      return;
    }
  } else {
    // その他のビュー: 既存の char > item > cell 優先処理
    if (charId > 0) {
      openCharacterDetailFromGame(charId);
      return;
    }
    if (itemId > 0 && mapId > 0) {
      updateAdminGamePickInfo(`選択中の配置物: map=${mapId} (${cellX},${cellY}) item=${itemId}`);
      return;
    }
    if (mapId > 0) {
      updateAdminGamePickInfo(`選択中のマップセル: map=${mapId} (${cellX},${cellY})`);
      return;
    }
  }

  updateAdminGamePickInfo("選択中: なし");
}

function handleAdminGameMessage(event) {
  if (adminGameFrame && event.source !== adminGameFrame.contentWindow) {
    return;
  }
  const message = event.data;
  if (!message || typeof message !== "object") {
    return;
  }
  if (message.kind === "sbop2_admin_session_ready") {
    syncAdminGameViewportHeight();
    checkAdminAuthAndReveal().then((authorized) => {
      if (authorized) {
        initializeAdminWorkspace();
      }
    });
    return;
  }
  if (message.kind === "sbop2_admin_char_pick") {
    // 後方互換
    openCharacterDetailFromGame(message.charId);
    return;
  }
  if (message.kind === "sbop2_admin_pick") {
    handleAdminGamePick(message);
    return;
  }
  if (message.kind === "sbop2_admin_pickup_parts") {
    // 右クリックでゲーム側が拾ったパーツIDを F4 移行後の map-parts-place.js へ転送
    if (typeof message.partsId === "number" && message.partsId > 0) {
      window.dispatchEvent(new CustomEvent("sbop2_pickup_parts", { detail: { partsId: message.partsId } }));
    }
  }
}

function buildAdminGameFrameUrl() {
  const basePath = adminGameFrame ? (adminGameFrame.dataset.gameSrc || "/game/sbocli-title.html") : "/game/sbocli-title.html";
  const httpPort = Number(window.location.port) || 18080;
  const wsPort = httpPort + 1;
  const host = window.location.hostname || "127.0.0.1";
  const params = new URLSearchParams();
  params.set("server", `${host}:${wsPort}`);
  params.set("admin", "1");
  return `${basePath}?${params.toString()}`;
}

function initializeAdminGameFrame() {
  if (!adminGameFrame) {
    return;
  }
  const nextUrl = buildAdminGameFrameUrl();
  if (adminGameFrame.getAttribute("src") !== nextUrl) {
    adminGameFrame.setAttribute("src", nextUrl);
  }
}

function navigateTo(route, options = {}) {
  const normalized = getValidRoute(route);
  const currentHash = stripRouteQuery(window.location.hash.replace(/^#/, ""));
  if (currentHash === normalized) {
    activateRoute(normalized, { forceReload: !!options.forceReload });
    return;
  }
  window.location.hash = `#${normalized}`;
}

/**
 * サーバーから JSON を取得する共通関数。
 * レスポンス本文が JSON でない場合は text フィールドに文字列を格納する。
 */
async function fetchJson(url, options = {}) {
  const defaultOptions = {
    headers: {
      "Accept": "application/json"
    },
    credentials: "include"
  };
  const response = await fetch(url, {
    ...defaultOptions,
    ...options,
    headers: {
      ...defaultOptions.headers,
      ...(options && options.headers)
    }
  });
  const rawText = await response.text();
  let data = null;
  if (rawText) {
    try {
      data = JSON.parse(rawText);
    } catch (error) {
      data = null;
    }
  }
  return { response, data, text: rawText };
}

async function checkAdminAuthAndReveal() {
  try {
    const { response, data } = await fetchJson("/api/auth/me");
    const authorized = response.ok && data && data.authenticated && Number(data.adminLevel) > 0;
    document.body.classList.toggle("auth-pending", !authorized);
    document.body.classList.toggle("admin-authorized", authorized);
    return authorized;
  } catch (error) {
    document.body.classList.add("auth-pending");
    document.body.classList.remove("admin-authorized");
    return false;
  }
}

/**
 * ES Module 側(core/api.js の fetchJson/postJson/putJson/deleteJson)が
 * 401 を受けた時の共通処理。ログイン画面へ戻す。
 * role-management.js / account-create.js / image-editor.js の個別 401 分岐は
 * これに一本化したため削除済み(各画面はこの後 checkAdminAuthAndReveal 経由で
 * auth-pending 表示に切り替わる)。
 */
function handleApiUnauthorized() {
  document.body.classList.add("auth-pending");
  document.body.classList.remove("admin-authorized");
  adminWorkspaceInitialized = false;
  showAdminLoginError("セッションが切れました。再度ログインしてください。");
  if (adminLoginIdInput) {
    adminLoginIdInput.focus();
  }
}

// core/api.js は ES Module のため、非 module の app.js からは動的 import で登録する。
import("./js/core/api.js")
  .then(({ setUnauthorizedHandler }) => {
    setUnauthorizedHandler(handleApiUnauthorized);
  })
  .catch((err) => {
    console.error("[app] core/api.js の 401 ハンドラ登録に失敗:", err);
  });

async function clearAdminSession() {
  try {
    await fetchJson("/api/auth/logout", { method: "POST" });
  } finally {
    document.body.classList.add("auth-pending");
    document.body.classList.remove("admin-authorized");
    adminWorkspaceInitialized = false;
  }
}

async function handleLogoutQuery() {
  const params = new URLSearchParams(window.location.search);
  if (!params.has("logout")) {
    return false;
  }
  await clearAdminSession();
  params.delete("logout");
  const nextQuery = params.toString();
  const nextUrl = `${window.location.pathname}${nextQuery ? `?${nextQuery}` : ""}${window.location.hash}`;
  window.history.replaceState(null, "", nextUrl);
  return true;
}

function showAdminLoginError(message) {
  if (!adminLoginErrorEl) {
    return;
  }
  adminLoginErrorEl.textContent = message;
  adminLoginErrorEl.hidden = !message;
}

async function handleAdminLoginSubmit(event) {
  event.preventDefault();
  if (!adminLoginIdInput || !adminLoginPasswordInput) {
    return;
  }
  const loginId = adminLoginIdInput.value.trim();
  const password = adminLoginPasswordInput.value;

  showAdminLoginError("");
  if (adminLoginSubmitButton) {
    adminLoginSubmitButton.disabled = true;
  }

  try {
    const { response, data } = await fetchJson("/api/auth/admin-login", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({ loginId, password })
    });

    if (response.ok) {
      adminLoginPasswordInput.value = "";
      const authorized = await checkAdminAuthAndReveal();
      if (authorized) {
        initializeAdminWorkspace();
      }
      return;
    }

    if (response.status === 401) {
      showAdminLoginError("ログインIDまたはパスワードが違います。");
    } else if (response.status === 403) {
      showAdminLoginError("このアカウントには管理者権限がありません。");
    } else if (response.status === 429) {
      const retryAfterSeconds = data && Number(data.retryAfterSeconds);
      const waitMessage = Number.isFinite(retryAfterSeconds) && retryAfterSeconds > 0
        ? `試行回数が多すぎます。${retryAfterSeconds}秒後にもう一度お試しください。`
        : "試行回数が多すぎます。しばらくしてからもう一度お試しください。";
      showAdminLoginError(waitMessage);
    } else if (response.status === 400) {
      showAdminLoginError("ログインIDとパスワードを入力してください。");
    } else {
      showAdminLoginError("ログインに失敗しました。時間をおいて再度お試しください。");
    }
  } catch (error) {
    showAdminLoginError("ログインに失敗しました。時間をおいて再度お試しください。");
  } finally {
    if (adminLoginSubmitButton) {
      adminLoginSubmitButton.disabled = false;
    }
  }
}

/* server-dashboard 全関数は server-dashboard.js に移行済み */
/* account-create / role-management 全関数は各 views/*.js に移行済み */
/* map-parts 全関数は map-parts-edit.js / map-parts-place.js に移行済み */
/* map-info 全関数は map-info.js に移行済み */
/* map-window 全関数は map-window.js に移行済み */
/* map-events 全関数は map-events.js に移行済み */
/* character-list / character-account 全関数は各 views/*.js に移行済み */
/* talk-events 全関数は talk-events.js に移行済み */
/* operation-history (監査ログ) 全関数は operation-history.js に移行済み */

// hash は `#item-types?id=3` のようにクエリを伴うことがある。
// ルート名の判定は必ずクエリを除いた部分で行う(付けたままだと
// data-view のどれとも一致せず DEFAULT_ROUTE に落ちてしまう)。
function stripRouteQuery(route) {
  if (!route) {
    return route;
  }
  const qIndex = route.indexOf("?");
  return qIndex === -1 ? route : route.slice(0, qIndex);
}

function getValidRoute(route) {
  const name = stripRouteQuery(route);
  if (!name) {
    return DEFAULT_ROUTE;
  }
  const matchingView = Array.from(views).find((view) => view.dataset.view === name);
  return matchingView ? name : DEFAULT_ROUTE;
}

function updateActiveNavGroup(route) {
  navGroups.forEach((group) => {
    const hasActiveRoute = !!group.querySelector(`[data-route="${route}"]`);
    group.classList.toggle("is-active-group", hasActiveRoute);
    if (hasActiveRoute) {
      group.open = true;
    }
  });
}

function activateRoute(route, options = {}) {
  const normalized = getValidRoute(route);
  const shouldSkip = normalized === currentRoute && !options.forceReload && !options.initial;
  if (shouldSkip) {
    return;
  }

  views.forEach((view) => {
    if (view.dataset.view === normalized) {
      view.classList.add("is-active");
    } else {
      view.classList.remove("is-active");
    }
  });

  navLinks.forEach((link) => {
    link.classList.toggle("is-active", link.dataset.route === normalized);
  });

  updateActiveNavGroup(normalized);

  // server-dashboard / map-info / map-window / map-events / talk-events /
  // character-list / operation-history は F9 移行済み (main.js の registerRoute が処理)
  // map-objects / map-parts-place は F4 移行済み
  // map-parts / map-shadows は F3 移行済み

  // ゲーム iframe へ Web管理モードを通知
  if (adminGameFrame && adminGameFrame.contentWindow) {
    let adminMode = 0;
    if (normalized === "character-overview" || normalized === "character-list") {
      adminMode = 1; // キャラ枠表示
    } else if (normalized === "map-parts-place") {
      adminMode = 2; // マップパーツ枠表示
    } else if (normalized === "map-events") {
      adminMode = 3; // マップイベント枠表示
    }
    adminGameFrame.contentWindow.postMessage({ kind: "sbop2_set_admin_mode", mode: adminMode }, "*");
  }

  currentRoute = normalized;
  recordRecentRoute(normalized);
}

function initializeAdminWorkspace() {
  if (adminWorkspaceInitialized) {
    return;
  }
  adminWorkspaceInitialized = true;
  ensureAdminWebSocket();
  const initialRoute = window.location.hash ? window.location.hash.replace(/^#/, "") : DEFAULT_ROUTE;
  activateRoute(initialRoute, { initial: true, forceReload: true });
  // loadRoles() は account-create.js に移行済みのため削除
}

window.addEventListener("load", async () => {
  window.addEventListener("message", handleAdminGameMessage);
  initializeAdminGameFrame();
  if (adminGameFrame) {
    adminGameFrame.addEventListener("load", () => {
      syncAdminGameViewportHeight();
    });
    adminGameFrame.addEventListener("error", () => {
      if (adminGameMissingEl) {
        adminGameMissingEl.hidden = false;
      }
    });
  }

  if (adminLogoutButton) {
    adminLogoutButton.addEventListener("click", async () => {
      await clearAdminSession();
    });
  }

  if (adminLoginForm) {
    adminLoginForm.addEventListener("submit", handleAdminLoginSubmit);
  }

  await handleLogoutQuery();
  const authorized = await checkAdminAuthAndReveal();
  if (authorized) {
    adminWorkspaceInitialized = true;
    // 管理 WebSocket を起動する（再接続は ensureAdminWebSocket 内で自動管理）
    ensureAdminWebSocket();
  } else if (adminLoginIdInput) {
    adminLoginIdInput.focus();
  }

  const initialRoute = window.location.hash ? window.location.hash.replace(/^#/, "") : DEFAULT_ROUTE;
  activateRoute(initialRoute, { initial: true });

  navLinks.forEach((link) => {
    link.addEventListener("click", (event) => {
      event.preventDefault();
      const targetRoute = link.dataset.route;
      if (!targetRoute) {
        return;
      }
      const currentHash = stripRouteQuery(window.location.hash.replace(/^#/, ""));
      if (currentHash === targetRoute) {
        activateRoute(targetRoute, { forceReload: true });
      } else {
        window.location.hash = `#${targetRoute}`;
      }
    });
  });

  window.addEventListener("hashchange", () => {
    const nextRoute = window.location.hash.replace(/^#/, "");
    activateRoute(nextRoute);
  });

  // loadRoles / initAuditLogView は F9 移行済みのため削除
  // map-window / server-dashboard / account-create / role-management イベントバインドは
  // 各 views/*.js に移行済みのため削除
  // map-info / map-events イベントバインドは map-info.js / map-events.js に移行済みのため削除
  // map-parts イベントバインドは F3 移行済みのため削除
});

// 共通 picker（openPicker/pickerState 等）は未使用のため削除（entity-picker.js に統合方針、1-6）
// ---------------------------------------------------------------------------
// 管理画面全体で右クリックコンテキストメニューを抑止する
document.addEventListener("contextmenu", function (ev) {
  ev.preventDefault();
});

const adminLogoutButton = document.getElementById("admin-logout-button");

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
let currentRoute = null;
let adminWorkspaceInitialized = false;

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
    if (charId > 0) {
      navigateTo("character-overview");
      if (typeof window._charEditMount === "function") { window._charEditMount(charId); }
      return;
    }
  } else if (currentRoute === "map-events") {
    // マップイベント編集ビュー: map-events.js の window._ API 経由でセル選択を通知
    if (mapId > 0) {
      const state  = window._mapEventsState;
      const reload = window._mapEventsReload;
      const renderTable = window._mapEventsRenderTable;
      const renderForm  = window._mapEventsRenderForm;
      const feedback    = window._mapEventsFeedback;
      if (!state) { return; }

      const switchAndFind = function () {
        const ev = state.events.find(function (e) {
          if (e.hitType === 2) {
            const x1 = Math.min(e.pos.x, e.pos2.x);
            const x2 = Math.max(e.pos.x, e.pos2.x);
            const y1 = Math.min(e.pos.y, e.pos2.y);
            const y2 = Math.max(e.pos.y, e.pos2.y);
            return cellX >= x1 && cellX <= x2 && cellY >= y1 && cellY <= y2;
          }
          return e.pos.x === cellX && e.pos.y === cellY;
        });
        if (ev) {
          state.selectedEventId = ev.id;
          if (typeof renderTable === "function") { renderTable(); }
          if (typeof renderForm  === "function") { renderForm(ev); }
          if (typeof feedback    === "function") { feedback("(" + cellX + "," + cellY + ") のイベントを選択しました", "success"); }
        } else {
          if (typeof feedback === "function") { feedback("(" + cellX + "," + cellY + ") にイベントはありません", ""); }
        }
      };

      if (state.selectedMapId !== mapId) {
        state.selectedMapId = mapId;
        if (typeof reload === "function") {
          reload().then(switchAndFind).catch(function () {
            if (typeof feedback === "function") { feedback("イベント一覧の取得に失敗しました", "error"); }
          });
        }
      } else {
        switchAndFind();
      }
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
  const currentHash = window.location.hash.replace(/^#/, "");
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

/* server-dashboard 全関数は server-dashboard.js に移行済み */
/* account-create / role-management 全関数は各 views/*.js に移行済み */
/* map-parts 全関数は map-parts-edit.js / map-parts-place.js に移行済み */
/* map-info 全関数は map-info.js に移行済み */
/* map-window 全関数は map-window.js に移行済み */
/* map-events 全関数は map-events.js に移行済み */
/* character-list / character-account 全関数は各 views/*.js に移行済み */
/* talk-events 全関数は talk-events.js に移行済み */
/* operation-history (監査ログ) 全関数は operation-history.js に移行済み */

function getValidRoute(route) {
  if (!route) {
    return DEFAULT_ROUTE;
  }
  const matchingView = Array.from(views).find((view) => view.dataset.view === route);
  return matchingView ? route : DEFAULT_ROUTE;
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

  document.querySelectorAll(".main-nav details[open]").forEach((d) => {
    d.removeAttribute("open");
  });

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

  await handleLogoutQuery();
  const authorized = await checkAdminAuthAndReveal();
  if (authorized) {
    adminWorkspaceInitialized = true;
    // 管理 WebSocket を起動する（再接続は ensureAdminWebSocket 内で自動管理）
    ensureAdminWebSocket();
  }

  const initialRoute = window.location.hash ? window.location.hash.replace(/^#/, "") : DEFAULT_ROUTE;
  activateRoute(initialRoute, { initial: true });

  // メインメニューの details 排他制御: 1つ開くと他は閉じる
  document.querySelectorAll(".main-nav details").forEach(function (details) {
    details.addEventListener("toggle", function () {
      if (details.open) {
        document.querySelectorAll(".main-nav details").forEach(function (other) {
          if (other !== details && other.open) {
            other.removeAttribute("open");
          }
        });
      }
    });
  });

  navLinks.forEach((link) => {
    link.addEventListener("click", (event) => {
      event.preventDefault();
      const targetRoute = link.dataset.route;
      if (!targetRoute) {
        return;
      }
      const currentHash = window.location.hash.replace(/^#/, "");
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

/* -------------------------------------------------------------------- */
/* 共通 picker モジュール（画像 / アイテム等の選択ダイアログ）           */
/*                                                                      */
/* 外部 API:                                                            */
/*   openPicker({ type, initialValue, category, onSelect })             */
/*     - type: "image" | "item" | "effect" | "mapPart" | "mapShadow"    */
/*     - initialValue: 初期選択ID（省略時は null）                      */
/*     - category: 画像 picker 時のカテゴリ key（例 "efcBalloon"）      */
/*     - onSelect: (selectedValue) => void  決定時に呼ばれる            */
/*                                                                      */
/* HTML 側の <button data-picker="image|item" data-picker-target="ID"   */
/*                    data-picker-category="key"> で宣言的に使える。    */
/* -------------------------------------------------------------------- */
const pickerState = {
  modalEl: null,
  titleEl: null,
  categoryWrap: null,
  categorySelect: null,
  searchInput: null,
  summaryEl: null,
  feedbackEl: null,
  listEl: null,
  selectedLabelEl: null,
  confirmBtn: null,
  type: null,
  currentItems: [],
  selected: null,       // 現在の選択値（画像なら idSub、アイテムなら itemId）
  selectedMeta: null,   // 表示用メタ情報
  onSelect: null,
  imageCategories: null, // 画像カテゴリカタログ（1 回だけロード）
  itemsCache: null,      // アイテム一覧（1 回だけロード、必要に応じて再読込）
  effectsCache: null,    // エフェクト一覧（1 回だけロード、必要に応じて再読込）
  mapPartsCache: null,   // マップパーツ一覧（1 回だけロード、必要に応じて再読込）
  mapShadowsCache: null, // マップ影一覧（1 回だけロード、必要に応じて再読込）
};

/** picker の DOM 要素を初期化（初回呼び出し時のみ実行）。 */
function ensurePickerInitialized() {
  if (pickerState.modalEl) {
    return;
  }
  pickerState.modalEl = document.getElementById("picker-modal");
  if (!pickerState.modalEl) {
    return;
  }
  pickerState.titleEl = document.getElementById("picker-title");
  pickerState.categoryWrap = document.getElementById("picker-category-wrap");
  pickerState.categorySelect = document.getElementById("picker-category-select");
  pickerState.searchInput = document.getElementById("picker-search-input");
  pickerState.summaryEl = document.getElementById("picker-summary");
  pickerState.feedbackEl = document.getElementById("picker-feedback");
  pickerState.listEl = document.getElementById("picker-list");
  pickerState.selectedLabelEl = document.getElementById("picker-selected-label");
  pickerState.confirmBtn = document.getElementById("picker-confirm-btn");

  // 閉じるボタン / バックドロップ
  const dismissEls = pickerState.modalEl.querySelectorAll("[data-picker-dismiss]");
  dismissEls.forEach(function (el) {
    el.addEventListener("click", closePicker);
  });

  // 検索入力
  if (pickerState.searchInput) {
    pickerState.searchInput.addEventListener("input", function () {
      renderPickerList();
    });
  }
  // カテゴリ変更（画像 picker のみ）
  if (pickerState.categorySelect) {
    pickerState.categorySelect.addEventListener("change", function () {
      refreshPickerItems();
    });
  }
  // 決定ボタン
  if (pickerState.confirmBtn) {
    pickerState.confirmBtn.addEventListener("click", confirmPickerSelection);
  }
  // ESC キーで閉じる
  document.addEventListener("keydown", function (ev) {
    if (!pickerState.modalEl || pickerState.modalEl.hidden) {
      return;
    }
    if (ev.key === "Escape") {
      closePicker();
    } else if (ev.key === "Enter" && pickerState.selected !== null) {
      ev.preventDefault();
      confirmPickerSelection();
    }
  });
  // 一覧クリックのイベント委譲
  if (pickerState.listEl) {
    pickerState.listEl.addEventListener("click", function (ev) {
      const li = ev.target.closest("li[data-picker-value]");
      if (!li) { return; }
      const rawValue = li.getAttribute("data-picker-value");
      const numValue = Number(rawValue);
      const meta = li.getAttribute("data-picker-meta") || "";
      setPickerSelected(Number.isFinite(numValue) ? numValue : rawValue, meta);
    });
    pickerState.listEl.addEventListener("dblclick", function (ev) {
      const li = ev.target.closest("li[data-picker-value]");
      if (!li) { return; }
      const rawValue = li.getAttribute("data-picker-value");
      const numValue = Number(rawValue);
      const meta = li.getAttribute("data-picker-meta") || "";
      setPickerSelected(Number.isFinite(numValue) ? numValue : rawValue, meta);
      confirmPickerSelection();
    });
  }
}

/** picker を開く（外部公開 API）。 */
function openPicker(options) {
  ensurePickerInitialized();
  if (!pickerState.modalEl) {
    console.warn("picker モーダルの DOM が見つかりません");
    return;
  }
  const opts = options || {};
  pickerState.type = opts.type || "image";
  pickerState.onSelect = typeof opts.onSelect === "function" ? opts.onSelect : null;
  pickerState.selected = null;
  pickerState.selectedMeta = null;
  pickerState.currentItems = [];

  // タイトル / カテゴリ UI の切替
  if (pickerState.type === "image") {
    pickerState.titleEl.textContent = "画像を選択";
    if (pickerState.categoryWrap) { pickerState.categoryWrap.hidden = false; }
    if (pickerState.searchInput) { pickerState.searchInput.placeholder = "サブID で絞り込み"; }
  } else if (pickerState.type === "item") {
    pickerState.titleEl.textContent = "アイテムを選択";
    if (pickerState.categoryWrap) { pickerState.categoryWrap.hidden = true; }
    if (pickerState.searchInput) { pickerState.searchInput.placeholder = "ID や名前で絞り込み"; }
  } else if (pickerState.type === "effect") {
    pickerState.titleEl.textContent = "エフェクトを選択";
    if (pickerState.categoryWrap) { pickerState.categoryWrap.hidden = true; }
    if (pickerState.searchInput) { pickerState.searchInput.placeholder = "ID や名前で絞り込み"; }
  } else if (pickerState.type === "mapPart") {
    pickerState.titleEl.textContent = "マップパーツを選択";
    if (pickerState.categoryWrap) { pickerState.categoryWrap.hidden = true; }
    if (pickerState.searchInput) { pickerState.searchInput.placeholder = "パーツ ID で絞り込み"; }
  } else if (pickerState.type === "mapShadow") {
    pickerState.titleEl.textContent = "マップ影を選択";
    if (pickerState.categoryWrap) { pickerState.categoryWrap.hidden = true; }
    if (pickerState.searchInput) { pickerState.searchInput.placeholder = "影 ID で絞り込み"; }
  } else {
    pickerState.titleEl.textContent = "選択";
  }

  if (pickerState.searchInput) {
    pickerState.searchInput.value = "";
  }
  setPickerFeedback("");
  updatePickerSelectedLabel();

  pickerState.modalEl.hidden = false;
  pickerState.modalEl.setAttribute("aria-hidden", "false");

  // 初期値の保持（描画後に選択状態を復元）
  const initialValue = opts.initialValue != null ? Number(opts.initialValue) : null;

  if (pickerState.type === "image") {
    ensureImageCategoriesLoaded()
      .then(function () {
        // カテゴリ選択状態を反映
        if (opts.category && pickerState.categorySelect) {
          pickerState.categorySelect.value = opts.category;
        }
        return refreshPickerItems(initialValue);
      })
      .catch(function (err) {
        setPickerFeedback("カテゴリ取得に失敗: " + (err && err.message ? err.message : err), true);
      });
  } else if (pickerState.type === "item") {
    refreshPickerItems(initialValue).catch(function (err) {
      setPickerFeedback("アイテム一覧取得に失敗: " + (err && err.message ? err.message : err), true);
    });
  } else if (pickerState.type === "effect") {
    refreshPickerItems(initialValue).catch(function (err) {
      setPickerFeedback("エフェクト一覧取得に失敗: " + (err && err.message ? err.message : err), true);
    });
  } else if (pickerState.type === "mapPart") {
    refreshPickerItems(initialValue).catch(function (err) {
      setPickerFeedback("マップパーツ一覧取得に失敗: " + (err && err.message ? err.message : err), true);
    });
  } else if (pickerState.type === "mapShadow") {
    refreshPickerItems(initialValue).catch(function (err) {
      setPickerFeedback("マップ影一覧取得に失敗: " + (err && err.message ? err.message : err), true);
    });
  }

  // フォーカスを検索ボックスへ
  setTimeout(function () {
    if (pickerState.searchInput) {
      pickerState.searchInput.focus();
    }
  }, 0);
}

/** picker を閉じる。 */
function closePicker() {
  if (!pickerState.modalEl) { return; }
  pickerState.modalEl.hidden = true;
  pickerState.modalEl.setAttribute("aria-hidden", "true");
  pickerState.onSelect = null;
}

function setPickerFeedback(message, isError) {
  if (!pickerState.feedbackEl) { return; }
  pickerState.feedbackEl.textContent = message || "";
  pickerState.feedbackEl.style.color = isError ? "#f87171" : "";
}

function updatePickerSelectedLabel() {
  if (!pickerState.selectedLabelEl) { return; }
  if (pickerState.selected == null) {
    pickerState.selectedLabelEl.textContent = "-";
  } else if (pickerState.selectedMeta) {
    pickerState.selectedLabelEl.textContent = pickerState.selected + "（" + pickerState.selectedMeta + "）";
  } else {
    pickerState.selectedLabelEl.textContent = String(pickerState.selected);
  }
  if (pickerState.confirmBtn) {
    pickerState.confirmBtn.disabled = (pickerState.selected == null);
  }
}

function setPickerSelected(value, meta) {
  pickerState.selected = value;
  pickerState.selectedMeta = meta || null;
  updatePickerSelectedLabel();
  // 視覚的な選択反映
  if (pickerState.listEl) {
    const all = pickerState.listEl.querySelectorAll("li[data-picker-value]");
    all.forEach(function (li) {
      const raw = li.getAttribute("data-picker-value");
      const num = Number(raw);
      const matches = Number.isFinite(num) ? num === value : raw === String(value);
      li.classList.toggle("is-selected", matches);
    });
  }
}

function confirmPickerSelection() {
  if (pickerState.selected == null) { return; }
  const cb = pickerState.onSelect;
  const val = pickerState.selected;
  closePicker();
  if (cb) {
    try {
      cb(val);
    } catch (err) {
      console.error("picker onSelect でエラー:", err);
    }
  }
}

/** 画像カテゴリカタログを API から 1 回だけ取得。 */
async function ensureImageCategoriesLoaded() {
  if (pickerState.imageCategories && pickerState.imageCategories.length > 0) {
    return pickerState.imageCategories;
  }
  const { response, data } = await fetchJson("/api/image-categories");
  if (!response.ok || !data || !Array.isArray(data.categories)) {
    throw new Error("HTTP " + response.status);
  }
  pickerState.imageCategories = data.categories;
  // カテゴリ <select> を構築（none は除外）
  if (pickerState.categorySelect) {
    pickerState.categorySelect.innerHTML = "";
    pickerState.imageCategories.forEach(function (cat) {
      if (cat.idMain === 0) { return; }
      const opt = document.createElement("option");
      opt.value = cat.key;
      opt.textContent = cat.label + "（#" + cat.idMain + "）";
      opt.dataset.idMain = cat.idMain;
      opt.dataset.hintMaxSub = cat.hintMaxSub;
      pickerState.categorySelect.appendChild(opt);
    });
  }
  return pickerState.imageCategories;
}

/** 現在の picker 種別に応じて一覧を更新。 */
async function refreshPickerItems(initialValue) {
  if (pickerState.type === "image") {
    const selKey = pickerState.categorySelect ? pickerState.categorySelect.value : null;
    const cat = (pickerState.imageCategories || []).find(function (c) { return c.key === selKey; });
    if (!cat) {
      pickerState.currentItems = [];
      renderPickerList();
      return;
    }
    // 画像カテゴリは件数ベースで「0..hintMaxSub」の合成リストを作る
    const hintMax = Math.max(0, Number(cat.hintMaxSub) || 0);
    const items = [];
    for (let i = 0; i <= hintMax; i++) {
      items.push({ id: i, label: String(i), meta: cat.label });
    }
    pickerState.currentItems = items;
    setPickerFeedback("カテゴリ: " + cat.label + "（0〜" + hintMax + "）");
    renderPickerList();
    if (initialValue != null && Number.isFinite(Number(initialValue))) {
      setPickerSelected(Number(initialValue), cat.label);
    }
    return;
  }
  if (pickerState.type === "item") {
    setPickerFeedback("読み込み中...");
    const { response, data } = await fetchJson("/api/items");
    if (!response.ok || !data || !Array.isArray(data.items)) {
      pickerState.currentItems = [];
      renderPickerList();
      setPickerFeedback("一覧取得に失敗（HTTP " + response.status + "）", true);
      return;
    }
    pickerState.itemsCache = data.items;
    pickerState.currentItems = data.items.map(function (it) {
      const typeName = it.itemTypeName || ("種別#" + it.itemTypeId);
      return {
        id: it.itemId,
        label: it.name || ("アイテム#" + it.itemId),
        meta: typeName,
      };
    });
    setPickerFeedback("アイテム " + pickerState.currentItems.length + " 件");
    renderPickerList();
    if (initialValue != null) {
      const target = pickerState.currentItems.find(function (x) { return x.id === Number(initialValue); });
      if (target) {
        setPickerSelected(target.id, target.label);
      }
    }
    return;
  }
  if (pickerState.type === "mapShadow") {
    setPickerFeedback("読み込み中...");
    const { response, data } = await fetchJson("/api/maps/shadows");
    if (!response.ok || !data || !Array.isArray(data.shadows)) {
      pickerState.currentItems = [];
      renderPickerList();
      setPickerFeedback("一覧取得に失敗（HTTP " + response.status + "）", true);
      return;
    }
    pickerState.mapShadowsCache = data.shadows;
    pickerState.currentItems = data.shadows
      .slice()
      .sort(function (a, b) { return (a.id || 0) - (b.id || 0); })
      .map(function (s) {
        const parts = [];
        if (s.grpId != null) { parts.push("画像#" + s.grpId); }
        if (s.light) { parts.push("明度"); } else { parts.push("透明度"); }
        if (s.level != null) { parts.push("Lv" + s.level); }
        return {
          id: s.id,
          label: "影#" + s.id,
          meta: parts.join(" / ") || null,
        };
      });
    setPickerFeedback("マップ影 " + pickerState.currentItems.length + " 件");
    renderPickerList();
    if (initialValue != null) {
      const target = pickerState.currentItems.find(function (x) { return x.id === Number(initialValue); });
      if (target) {
        setPickerSelected(target.id, target.label);
      }
    }
    return;
  }
  if (pickerState.type === "mapPart") {
    setPickerFeedback("読み込み中...");
    const { response, data } = await fetchJson("/api/maps/parts");
    if (!response.ok || !data || !Array.isArray(data.parts)) {
      pickerState.currentItems = [];
      renderPickerList();
      setPickerFeedback("一覧取得に失敗（HTTP " + response.status + "）", true);
      return;
    }
    pickerState.mapPartsCache = data.parts;
    pickerState.currentItems = data.parts
      .slice()
      .sort(function (a, b) { return (a.partsId || 0) - (b.partsId || 0); })
      .map(function (p) {
        // フラグから代表的な属性を 1 〜 2 個メタに出す
        const flagLabels = [];
        if (p.flags) {
          if (p.flags.block) { flagLabels.push("ぶつかる"); }
          if (p.flags.pile) { flagLabels.push("重なる"); }
          if (p.flags.fishing) { flagLabels.push("釣り"); }
          if (p.flags.drawLast) { flagLabels.push("最前面"); }
        }
        const grpBase = (p.sprites && p.sprites.base) ? p.sprites.base.grpId : null;
        const meta = (grpBase != null ? ("画像#" + grpBase) : "")
          + (flagLabels.length ? (" / " + flagLabels.join("・")) : "");
        return {
          id: p.partsId,
          label: "パーツ#" + p.partsId,
          meta: meta || null,
        };
      });
    setPickerFeedback("マップパーツ " + pickerState.currentItems.length + " 件");
    renderPickerList();
    if (initialValue != null) {
      const target = pickerState.currentItems.find(function (x) { return x.id === Number(initialValue); });
      if (target) {
        setPickerSelected(target.id, target.label);
      }
    }
    return;
  }
  if (pickerState.type === "effect") {
    setPickerFeedback("読み込み中...");
    const { response, data } = await fetchJson("/api/effects");
    if (!response.ok || !data || !Array.isArray(data.items)) {
      pickerState.currentItems = [];
      renderPickerList();
      setPickerFeedback("一覧取得に失敗（HTTP " + response.status + "）", true);
      return;
    }
    pickerState.effectsCache = data.items;
    pickerState.currentItems = data.items.map(function (e) {
      const meta = (e.grpId != null ? ("画像#" + e.grpId) : "")
        + (e.soundId ? " / 音#" + e.soundId : "");
      return {
        id: e.effectId,
        label: e.name || ("エフェクト#" + e.effectId),
        meta: meta || null,
      };
    });
    setPickerFeedback("エフェクト " + pickerState.currentItems.length + " 件");
    renderPickerList();
    if (initialValue != null) {
      const target = pickerState.currentItems.find(function (x) { return x.id === Number(initialValue); });
      if (target) {
        setPickerSelected(target.id, target.label);
      }
    }
    return;
  }
}

/** 検索入力で絞り込みつつ現在のリストを描画。 */
function renderPickerList() {
  if (!pickerState.listEl) { return; }
  const keyword = pickerState.searchInput ? pickerState.searchInput.value.trim().toLowerCase() : "";
  const filtered = !keyword
    ? pickerState.currentItems
    : pickerState.currentItems.filter(function (it) {
        if (String(it.id).includes(keyword)) { return true; }
        if (it.label && it.label.toLowerCase().includes(keyword)) { return true; }
        if (it.meta && it.meta.toLowerCase().includes(keyword)) { return true; }
        return false;
      });

  // アイテム / エフェクト / マップパーツ / マップ影 picker は行表示にする（名前が長いため）
  pickerState.listEl.classList.toggle("picker-mode-rows",
    pickerState.type === "item" || pickerState.type === "effect"
    || pickerState.type === "mapPart" || pickerState.type === "mapShadow");

  if (pickerState.summaryEl) {
    pickerState.summaryEl.textContent = filtered.length + " 件 / " + pickerState.currentItems.length + " 件";
  }

  if (filtered.length === 0) {
    pickerState.listEl.innerHTML = '<li class="is-empty">該当する項目がありません</li>';
    return;
  }

  // 大量件数対策: 先頭 500 件のみ描画
  const MAX_RENDER = 500;
  const capped = filtered.slice(0, MAX_RENDER);
  const html = capped.map(function (it) {
    const selCls = (pickerState.selected === it.id) ? " is-selected" : "";
    const metaHtml = it.meta ? '<span class="picker-item-meta">' + escapeHtmlText(it.meta) + '</span>' : "";
    const labelHtml = escapeHtmlText(it.label || String(it.id));
    const metaAttr = it.meta ? ' data-picker-meta="' + escapeHtmlAttr(it.meta) + '"' : "";
    return '<li data-picker-value="' + it.id + '"' + metaAttr + ' class="' + selCls.trim() + '">'
      + '<span class="picker-item-id">#' + it.id + '</span>'
      + '<span class="picker-item-label">' + labelHtml + '</span>'
      + metaHtml
      + '</li>';
  }).join("");
  const truncHtml = (filtered.length > MAX_RENDER)
    ? '<li class="is-empty">（さらに ' + (filtered.length - MAX_RENDER) + ' 件。検索で絞り込んでください）</li>'
    : "";
  pickerState.listEl.innerHTML = html + truncHtml;
}

function escapeHtmlText(text) {
  return String(text == null ? "" : text)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;");
}
function escapeHtmlAttr(text) {
  return escapeHtmlText(text).replace(/"/g, "&quot;");
}

/** data-picker 属性を持つボタン全てに共通のクリックハンドラを設定。 */
function bindPickerButtons() {
  document.addEventListener("click", function (ev) {
    const btn = ev.target.closest("button[data-picker]");
    if (!btn) { return; }
    const type = btn.getAttribute("data-picker");
    const targetId = btn.getAttribute("data-picker-target");
    const category = btn.getAttribute("data-picker-category") || null;
    if (!targetId) {
      console.warn("data-picker-target が未指定です", btn);
      return;
    }
    const inputEl = document.getElementById(targetId);
    const initialValue = inputEl && inputEl.value !== "" ? Number(inputEl.value) : null;
    openPicker({
      type: type,
      initialValue: initialValue,
      category: category,
      onSelect: function (value) {
        if (!inputEl) { return; }
        inputEl.value = String(value);
        // change/input イベントを発火し、既存の入力監視ハンドラに気付かせる
        inputEl.dispatchEvent(new Event("input", { bubbles: true }));
        inputEl.dispatchEvent(new Event("change", { bubbles: true }));
      },
    });
  });
}

// 初期化（DOMContentLoaded 直後／即時のどちらでも動くよう防御的に呼ぶ）
if (document.readyState === "loading") {
  document.addEventListener("DOMContentLoaded", bindPickerButtons);
} else {
  bindPickerButtons();
}

// ---------------------------------------------------------------------------
// 管理画面全体で右クリックコンテキストメニューを抑止する
document.addEventListener("contextmenu", function (ev) {
  ev.preventDefault();
});
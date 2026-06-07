const serverStatusEl = document.getElementById("server-status");
const onlineEl = document.getElementById("server-online");
const versionEl = document.getElementById("server-version");
const cpuEl = document.getElementById("server-cpu");
const memoryEl = document.getElementById("server-memory");
const latencyEl = document.getElementById("server-latency");
const updatedEl = document.getElementById("server-updated");
const reloadButton = document.getElementById("reload-server");
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

const accountForm = document.getElementById("account-form");
const accountResultEl = document.getElementById("account-result");
const accountRolesContainer = document.getElementById("account-roles");
const accountResetButton = document.getElementById("account-reset");

const roleForm = document.getElementById("role-form");
const roleResultEl = document.getElementById("role-result");
const roleCheckboxContainer = document.getElementById("role-checkboxes");
const roleResetButton = document.getElementById("role-reset");

const mapObjectMapSelect = document.getElementById("map-object-map");
const mapObjectFilterSelect = document.getElementById("map-object-filter");
const mapObjectSummaryEl = document.getElementById("map-object-summary");
const mapObjectTableBody = document.getElementById("map-object-table-body");
const mapObjectGridEl = document.getElementById("map-object-grid");
const mapObjectSelectedLabel = document.getElementById("map-object-selected-label");
const mapObjectForm = document.getElementById("map-object-form");
const mapObjectDataIdInput = document.getElementById("map-object-data-id");
const mapObjectPlacementIdDisplay = document.getElementById("map-object-placement-id-display");
const mapObjectTypeSelect = document.getElementById("map-object-type");
const mapObjectXInput = document.getElementById("map-object-x");
const mapObjectYInput = document.getElementById("map-object-y");
const mapObjectClearButton = document.getElementById("map-object-clear");
const mapObjectDeleteButton = document.getElementById("map-object-delete");
const mapObjectSaveButton = document.getElementById("map-object-save");
const mapObjectFeedbackEl = document.getElementById("map-object-feedback");

/* オブジェクトテンプレート管理 */
const mapObjectTemplateTableBody = document.getElementById("map-object-template-table-body");
const mapObjectTemplateSummaryEl = document.getElementById("map-object-template-summary");
const mapObjectTemplateForm = document.getElementById("map-object-template-form");
const mapObjectTemplateFeedbackEl = document.getElementById("map-object-template-feedback");
const mapObjectTemplateIdInput = document.getElementById("map-object-template-id");
const mapObjectTemplateNameInput = document.getElementById("map-object-template-name");
const mapObjectTemplateAttrInput = document.getElementById("map-object-template-attr");
const mapObjectTemplateHideYInput = document.getElementById("map-object-template-hideY");
const mapObjectTemplateWidthInput = document.getElementById("map-object-template-width");
const mapObjectTemplateHeightInput = document.getElementById("map-object-template-height");
const mapObjectTemplateCollisionInput = document.getElementById("map-object-template-collision");
const mapObjectTemplateSaveBtn = document.getElementById("map-object-template-save-btn");
const mapObjectTemplateNewBtn = document.getElementById("map-object-template-new-btn");
const mapObjectTemplateDeleteBtn = document.getElementById("map-object-template-delete-btn");

const mapPartsFlagFilter = document.getElementById("map-parts-flag-filter");
const mapPartsSummaryEl = document.getElementById("map-parts-summary");
const mapPartsGallery = document.getElementById("map-parts-gallery");
const mapPartsFeedbackEl = document.getElementById("map-parts-feedback");
const mapPartsDetailId = document.getElementById("map-parts-detail-id");
const mapPartsDetailView = document.getElementById("map-parts-detail-view");
const mapPartsDetailAnime = document.getElementById("map-parts-detail-anime");
const mapPartsDetailLevel = document.getElementById("map-parts-detail-level");
const mapPartsDetailPosition = document.getElementById("map-parts-detail-position");
const mapPartsDetailMove = document.getElementById("map-parts-detail-move");
const mapPartsFlagsList = document.getElementById("map-parts-flags");
const mapPartsSpriteBaseInfo = document.getElementById("map-parts-sprite-base");
const mapPartsSpriteOverlayInfo = document.getElementById("map-parts-sprite-overlay");
const mapPartsPreviewBase = document.getElementById("map-parts-preview-base");
const mapPartsPreviewOverlay = document.getElementById("map-parts-preview-overlay");
const mapPartsSheetHint = document.getElementById("map-parts-sheet-hint");

const mapPartsEditForm = document.getElementById("map-parts-edit-form");
const mapPartsEditViewType = document.getElementById("map-parts-edit-view-type");
const mapPartsEditAnimeType = document.getElementById("map-parts-edit-anime-type");
const mapPartsEditLevel = document.getElementById("map-parts-edit-level");
const mapPartsEditLevelValue = document.getElementById("map-parts-edit-level-value");
const mapPartsEditMoveDirection = document.getElementById("map-parts-edit-move-direction");
const mapPartsEditPosX = document.getElementById("map-parts-edit-pos-x");
const mapPartsEditPosY = document.getElementById("map-parts-edit-pos-y");
const mapPartsEditGrpBase = document.getElementById("map-parts-edit-grp-base");
const mapPartsEditGrpPile = document.getElementById("map-parts-edit-grp-pile");
const mapPartsEditFlagBlock = document.getElementById("map-parts-edit-flag-block");
const mapPartsEditFlagPile = document.getElementById("map-parts-edit-flag-pile");
const mapPartsEditFlagPileBack = document.getElementById("map-parts-edit-flag-pile-back");
const mapPartsEditFlagFishing = document.getElementById("map-parts-edit-flag-fishing");
const mapPartsEditFlagDrawLast = document.getElementById("map-parts-edit-flag-draw-last");
const mapPartsEditFlagCounter = document.getElementById("map-parts-edit-flag-counter");
const mapPartsEditBlockUp = document.getElementById("map-parts-edit-block-up");
const mapPartsEditBlockDown = document.getElementById("map-parts-edit-block-down");
const mapPartsEditBlockLeft = document.getElementById("map-parts-edit-block-left");
const mapPartsEditBlockRight = document.getElementById("map-parts-edit-block-right");
const mapPartsSaveBtn = document.getElementById("map-parts-save-btn");
const mapPartsCreateBtn = document.getElementById("map-parts-create-btn");
const mapPartsDeleteBtn = document.getElementById("map-parts-delete-btn");

/* マップパーツ配置 */
const mapPartsPlaceMapSelect = document.getElementById("map-parts-place-map");
const mapPartsPlaceEditMasterBtn = document.getElementById("map-parts-place-edit-master-btn");
const mapPartsPlaceSelectedLabel = document.getElementById("map-parts-place-selected-label");
const mapPartsPlaceFeedbackEl = document.getElementById("map-parts-place-feedback");
const mapPartsPlaceGallery = document.getElementById("map-parts-place-gallery");

/* マップ情報編集 */
const mapInfoSelect = document.getElementById("map-info-select");
const mapInfoSummaryEl = document.getElementById("map-info-summary");
const mapInfoEditArea = document.getElementById("map-info-edit-area");
const mapInfoEditForm = document.getElementById("map-info-edit-form");
const mapInfoNameInput = document.getElementById("map-info-name");
const mapInfoBgmInput = document.getElementById("map-info-bgm");
const mapInfoWeatherSelect = document.getElementById("map-info-weather");
const mapInfoDarknessInput = document.getElementById("map-info-darkness");
const mapInfoBattleCheck = document.getElementById("map-info-battle");
const mapInfoRecoveryCheck = document.getElementById("map-info-recovery");
const mapInfoFeedbackEl = document.getElementById("map-info-feedback");
const mapInfoReloadBtn = document.getElementById("map-info-reload-btn");
const mapInfoAddBtn = document.getElementById("map-info-add-btn");
const mapInfoAddArea = document.getElementById("map-info-add-area");
const mapInfoCopySelect = document.getElementById("map-info-copy-select");
const mapInfoAddConfirmBtn = document.getElementById("map-info-add-confirm-btn");
const mapInfoAddCancelBtn = document.getElementById("map-info-add-cancel-btn");

/* マップイベント編集 */
const mapEventMapSelect = document.getElementById("map-event-map");
const mapEventSummaryEl = document.getElementById("map-event-summary");
const mapEventTableBody = document.getElementById("map-event-table-body");
const mapEventEditArea = document.getElementById("map-event-edit-area");
const mapEventEditForm = document.getElementById("map-event-edit-form");
const mapEventFeedbackEl = document.getElementById("map-event-feedback");
const mapEventNewBtn = document.getElementById("map-event-new-btn");

/* マップウィンドウ */
const mapWindowMapSelect = document.getElementById("map-window-map");
const mapWindowSummaryEl = document.getElementById("map-window-summary");
const mapWindowGridEl = document.getElementById("map-window-grid");
const mapWindowSelectedInfoEl = document.getElementById("map-window-selected-info");
const adminGameFrame = document.getElementById("admin-game-frame");
const adminGameMissingEl = document.getElementById("admin-game-missing");
const adminGamePickInfoEl = document.getElementById("admin-game-pick-info");

const POLL_INTERVAL_MS = 30000;
let serverTimerId = null;
let cachedRoles = [];

const MAP_OBJECT_TYPE_OPTIONS = [
  { value: "collision", label: "当たり判定あり" },
  { value: "no-collision", label: "当たり判定なし" }
];

const MAP_PARTS_FLAG_LABELS = {
  block: "ぶつかる",
  pile: "重なる",
  pileBack: "下地も重ねる",
  fishing: "釣り場",
  drawLast: "最前面",
  counter: "カウンター"
};

const MAP_PARTS_DIRECTION_LABELS = {
  up: "上",
  down: "下",
  left: "左",
  right: "右"
};

const MAP_PARTS_VIEW_TYPE_LABELS = {
  0: "通常レイヤー",
  1: "背景レイヤー",
  2: "装飾レイヤー",
  3: "イベントレイヤー",
  4: "水辺レイヤー",
  5: "特殊レイヤー"
};

const MAP_PARTS_ANIME_TYPE_LABELS = {
  0: "静止",
  1: "往復",
  2: "循環",
  3: "ランダム"
};

const mapObjectState = {
  maps: [],
  selectedMapId: null,
  selectedObjectId: null,
  filterType: "all",
  previewSelection: null,
  isLoading: false,
  loadError: null,
  tileSize: 16,
  sheetTileWidth: 32,
  sheetTileHeight: 32,
  sheetBaseUrl: "/api/assets/map-parts/sheets"
};

const mapObjectTemplateState = {
  templates: [],
  selectedId: null,
  isLoading: false
};

const mapPartsState = {
  parts: [],
  filtered: [],
  flagFilter: "all",
  selectedPartId: null,
  sheetBaseUrl: "/api/assets/map-parts/sheets",
  tileSize: 16,
  sheetTileWidth: 32,
  sheetTileHeight: 32,
  sheetCount: 0,
  sheets: [],
  isLoading: false,
  loadError: null
};

/* マップパーツ配置画面の状態 */
const mapPartsPlaceState = {
  selectedMapId: null,
  selectedPartsId: null,
};

/* マップ情報編集画面の状態 */
const mapInfoState = {
  maps: [],
  selectedMapId: null,
  isLoading: false,
  loadError: null
};

/* マップイベント編集画面の状態 */
const mapEventState = {
  maps: [],          // {id, name} の一覧（マップ選択に使用）
  selectedMapId: null,
  events: [],        // 選択中マップのイベント一覧
  selectedEventId: null,
  isLoading: false,
  loadError: null
};

/* マップウィンドウ画面の状態 */
const mapWindowState = {
  maps: [],             // normalizeMapEntry 済みのマップ一覧
  selectedMapId: null,  // 現在選択中のマップID
  selectedCell: null,   // 選択中セル { x, y } または null
  isInitialized: false,
  isLoading: false,
  loadError: null,
  tileSize: 16,
  sheetTileWidth: 32,
  sheetTileHeight: 32,
  sheetBaseUrl: "/api/assets/map-parts/sheets"
};

/* マップ影カタログ編集画面の状態 */
const mapShadowState = {
  shadows: [],          // 影カタログ一覧
  selectedId: null,     // 編集中の影ID（null = 新規）
  isLoading: false,
  loadError: null
};

const MAP_PARTS_GALLERY_SCALE = 4;
const MAP_PARTS_DETAIL_SCALE = 6;

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
      applySelectionPayload(msg.payload);
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

function formatHex(value) {
  if (value === null || value === undefined) {
    return null;
  }
  const numeric = Number(value);
  if (!Number.isFinite(numeric)) {
    return null;
  }
  const normalized = (numeric >>> 0).toString(16).toUpperCase().padStart(8, "0");
  return `0x${normalized}`;
}

function getWeatherLabel(weatherType) {
  switch (weatherType) {
    case 0:
      return "指定なし";
    case 1:
      return "雲";
    case 2:
      return "霧";
    case 3:
      return "雪";
    default:
      return "不明";
  }
}

function normalizeMapObject(rawObject, mapId, index) {
  const placementId = typeof rawObject?.placementId === "number" ? rawObject.placementId : null;
  const objectId = typeof rawObject?.objectId === "number" ? rawObject.objectId : null;
  const x = Number(rawObject?.x ?? 0);
  const y = Number(rawObject?.y ?? 0);
  const hasCollision = Boolean(rawObject?.hasCollision);
  const attribute = typeof rawObject?.attribute === "number" ? rawObject.attribute : null;
  const attributeHex = typeof rawObject?.attributeHex === "string" && rawObject.attributeHex
    ? rawObject.attributeHex
    : formatHex(attribute);
  const objectIdHex = typeof rawObject?.objectIdHex === "string" && rawObject.objectIdHex
    ? rawObject.objectIdHex
    : formatHex(objectId);
  const sizeWidth = Number(rawObject?.size?.width);
  const sizeHeight = Number(rawObject?.size?.height);
  const hasSize = Number.isFinite(sizeWidth) && Number.isFinite(sizeHeight);
  const size = hasSize ? { width: sizeWidth, height: sizeHeight } : null;
  const baseName = typeof rawObject?.name === "string" && rawObject.name.length
    ? rawObject.name
    : objectId !== null
      ? `オブジェクトID ${objectId}`
      : `配置${index + 1}`;
  const uniqueId = placementId !== null
    ? `placement-${placementId}`
    : `map${mapId}-obj${objectId !== null ? objectId : `idx${index}`}-${x}-${y}`;
  const noteLines = [];
  if (objectIdHex) {
    noteLines.push(`オブジェクトID: ${objectIdHex}`);
  }
  if (attributeHex) {
    noteLines.push(`属性: ${attributeHex}`);
  }
  if (size) {
    noteLines.push(`画像サイズ: ${size.width}×${size.height}`);
  }

  return {
    id: uniqueId,
    placementId,
    objectId,
    objectIdHex,
    name: baseName,
    type: hasCollision ? "collision" : "no-collision",
    layer: attributeHex || "-",
    attribute,
    attributeHex,
    x: Number.isFinite(x) ? x : 0,
    y: Number.isFinite(y) ? y : 0,
    respawn: null,
    notes: noteLines.join("\n"),
    hasCollision,
    size
  };
}

function normalizeMapEntry(rawMap) {
  const rawId = rawMap?.id;
  const id = rawId !== null && rawId !== undefined ? String(rawId) : "";
  const idHex = typeof rawMap?.idHex === "string" && rawMap.idHex ? rawMap.idHex : formatHex(rawId);
  const width = Number(rawMap?.width ?? 0);
  const height = Number(rawMap?.height ?? 0);
  const weatherType = typeof rawMap?.weatherType === "number" ? rawMap.weatherType : null;
  const weatherLabel = typeof rawMap?.weatherLabel === "string" && rawMap.weatherLabel
    ? rawMap.weatherLabel
    : getWeatherLabel(weatherType);
  const objects = Array.isArray(rawMap?.objects)
    ? rawMap.objects.map((item, index) => normalizeMapObject(item, id, index))
    : [];
  const tiles = Array.isArray(rawMap?.tiles) ? rawMap.tiles : [];
  // tilesBase: base grpId の配列。API が返さない場合は tiles（後方互換）を流用する。
  const tilesBase = Array.isArray(rawMap?.tilesBase) ? rawMap.tilesBase : tiles;
  // tilesPile: pile grpId の配列。API が返さない場合は空配列（overlay なし扱い）。
  const tilesPile = Array.isArray(rawMap?.tilesPile) ? rawMap.tilesPile : [];
  // tilesMapPileBase: 独立重ねレイヤ (m_pwMapPile) の base grpId 配列。
  // 古いサーバーが API フィールドを返さない場合は空配列フォールバック（描画なし扱い）。
  const tilesMapPileBase = Array.isArray(rawMap?.tilesMapPileBase) ? rawMap.tilesMapPileBase : [];
  // tilesMapPilePile: 独立重ねレイヤ (m_pwMapPile) の pile grpId 配列。
  // 古いサーバーが API フィールドを返さない場合は空配列フォールバック（描画なし扱い）。
  const tilesMapPilePile = Array.isArray(rawMap?.tilesMapPilePile) ? rawMap.tilesMapPilePile : [];

  return {
    id,
    idHex,
    name: typeof rawMap?.name === "string" && rawMap.name.length ? rawMap.name : (id ? `マップID ${id}` : "名称未設定"),
    width: Number.isFinite(width) ? width : 0,
    height: Number.isFinite(height) ? height : 0,
    weatherType,
    weatherLabel,
    battleEnabled: Boolean(rawMap?.battleEnabled),
    recoveryEnabled: Boolean(rawMap?.recoveryEnabled),
    objects,
    tiles,
    tilesBase,
    tilesPile,
    tilesMapPileBase,
    tilesMapPilePile
  };
}

function setMapObjectLoading(isLoading, message) {
  mapObjectState.isLoading = isLoading;
  if (mapObjectMapSelect) {
    mapObjectMapSelect.disabled = isLoading || mapObjectState.maps.length === 0;
  }
  if (mapObjectFilterSelect) {
    mapObjectFilterSelect.disabled = isLoading || mapObjectState.maps.length === 0;
  }
  if (mapObjectSummaryEl && isLoading) {
    mapObjectSummaryEl.textContent = message || "マップ情報を読み込み中...";
    mapObjectSummaryEl.title = "";
  }
}

function setMapObjectError(message) {
  mapObjectState.loadError = message || null;
  if (mapObjectSummaryEl && message) {
    mapObjectSummaryEl.textContent = message;
    mapObjectSummaryEl.title = "";
  }
}

async function loadMapObjectData(forceReload = false) {
  if (!mapObjectMapSelect) {
    return;
  }
  if (mapObjectState.isLoading) {
    return;
  }
  if (!forceReload && mapObjectState.maps.length && !mapObjectState.loadError) {
    return;
  }

  setMapObjectLoading(true, "マップ情報を読み込み中...");
  setMapObjectError(null);

  try {
    const { response, data } = await fetchJson("/api/maps/objects");
    if (!response.ok || !data || !Array.isArray(data.maps)) {
      throw new Error("invalid_response");
    }

    mapObjectState.tileSize = Number(data.tileSize) || mapObjectState.tileSize;
    mapObjectState.sheetTileWidth = Number(data.sheetTileWidth) || mapObjectState.sheetTileWidth;
    mapObjectState.sheetTileHeight = Number(data.sheetTileHeight) || mapObjectState.sheetTileHeight;
    if (typeof data.sheetBaseUrl === "string" && data.sheetBaseUrl.length) {
      mapObjectState.sheetBaseUrl = data.sheetBaseUrl;
    }

    const maps = data.maps.map((item) => normalizeMapEntry(item));
    mapObjectState.maps = maps;

    if (maps.length) {
      if (!mapObjectState.selectedMapId || !maps.some((map) => map.id === mapObjectState.selectedMapId)) {
        mapObjectState.selectedMapId = maps[0].id;
      }
    } else {
      mapObjectState.selectedMapId = null;
    }

    mapObjectState.selectedObjectId = null;
    mapObjectState.previewSelection = null;

    populateMapOptions();
    if (mapObjectMapSelect && mapObjectState.selectedMapId) {
      mapObjectMapSelect.value = mapObjectState.selectedMapId;
    }
    if (mapObjectFilterSelect) {
      mapObjectFilterSelect.value = mapObjectState.filterType;
      mapObjectFilterSelect.disabled = mapObjectState.maps.length === 0;
    }
    setMapObjectFeedback("現在は閲覧専用モードです。", null);
  } catch (error) {
    console.error("Failed to load map objects", error);
    mapObjectState.maps = [];
    mapObjectState.selectedMapId = null;
    mapObjectState.selectedObjectId = null;
    mapObjectState.previewSelection = null;
    populateMapOptions();
    setMapObjectError("マップデータの取得に失敗しました");
  } finally {
    setMapObjectLoading(false);
    renderMapComponents();
  }
}



const DEFAULT_ROUTE = "map-window";
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
  // character-overview ビューを開いている時のみ詳細を反映する
  if (currentRoute === "character-overview") {
    if (charDetailIdInput) {
      charDetailIdInput.value = String(normalizedCharId);
    }
    fetchCharacterDetail(normalizedCharId);
  }
}

function handleAdminGamePick(message) {
  const charId = Number(message.charId) || 0;
  const itemId = Number(message.itemId) || 0;
  const mapId = Number(message.mapId) || 0;
  const cellX = Number(message.cellX) || 0;
  const cellY = Number(message.cellY) || 0;

  // ゲーム画面クリックでは画面遷移はしない。pick 情報の表示のみ更新する
  // （character-overview ビュー時のみ char 詳細を内部更新する）
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
    // 右クリックでゲーム側が拾ったパーツIDをギャラリー選択に反映
    if (typeof message.partsId === "number" && message.partsId > 0) {
      selectMapPartsPlaceItem(message.partsId);
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

/** サーバー情報カードの表示を初期化する。 */
function resetServerDisplay(message) {
  serverStatusEl.textContent = message;
  onlineEl.textContent = "-";
  versionEl.textContent = "-";
  cpuEl.textContent = "-";
  memoryEl.textContent = "-";
  latencyEl.textContent = "-";
  updatedEl.textContent = "更新時刻: -";
}

/** サーバー情報カードに取得した値を反映する。 */
function populateServerDisplay(item, updatedAt) {
  serverStatusEl.textContent = item.status || "不明";
  onlineEl.textContent = typeof item.onlinePlayers === "number" ? item.onlinePlayers.toString() : "-";
  versionEl.textContent = item.lastPatchVersion || "-";
  if (item.metrics) {
    cpuEl.textContent = `${item.metrics.cpuUsage.toFixed(1)} %`;
    memoryEl.textContent = `${item.metrics.memoryUsage.toFixed(1)} %`;
    latencyEl.textContent = `${item.metrics.latencyMs.toFixed(1)}`;
  } else {
    cpuEl.textContent = "-";
    memoryEl.textContent = "-";
    latencyEl.textContent = "-";
  }
  updatedEl.textContent = `更新時刻: ${updatedAt || "不明"}`;
}

/** サーバー情報をロードし、結果に応じて表示を更新する。 */
async function loadServerInfo(showToast = false) {
  if (showToast) {
    resetServerDisplay("読み込み中...");
  }
  try {
    const { response, data } = await fetchJson("/api/server?includeMetrics=true");
    if (!response.ok || !data || !Array.isArray(data.items) || data.items.length === 0) {
      throw new Error("サーバー情報の取得に失敗しました");
    }
    populateServerDisplay(data.items[0], data.updatedAt);
  } catch (error) {
    resetServerDisplay("エラー");
    updatedEl.textContent = `更新時刻: ${error.message}`;
  }
}

/** 一定間隔でサーバー情報を更新するタイマーをセットする。 */
function startServerPolling() {
  stopServerPolling();
  serverTimerId = setInterval(() => {
    loadServerInfo(false);
  }, POLL_INTERVAL_MS);
}

function stopServerPolling() {
  if (serverTimerId !== null) {
    clearInterval(serverTimerId);
    serverTimerId = null;
  }
}

/** メッセージ用要素の内容とクラスを更新する。 */
function setResultMessage(element, message, type) {
  element.textContent = message;
  element.className = "result-message";
  if (type) {
    element.classList.add(type);
  }
}

/** 利用可能なロール一覧を描画する。 */
function renderRoleOptions() {
  accountRolesContainer.innerHTML = "";
  roleCheckboxContainer.innerHTML = "";

  if (!cachedRoles.length) {
    const emptyMessage = document.createElement("p");
    emptyMessage.textContent = "ロール情報を取得できませんでした";
    emptyMessage.className = "note";
    accountRolesContainer.appendChild(emptyMessage.cloneNode(true));
    roleCheckboxContainer.appendChild(emptyMessage);
    return;
  }

  cachedRoles.forEach((role) => {
    const accountOption = createRoleOption(role, "account-role");
    accountRolesContainer.appendChild(accountOption);

    const updateOption = createRoleOption(role, "update-role");
    roleCheckboxContainer.appendChild(updateOption);
  });
}

/** ロール選択肢の DOM を生成する。 */
function createRoleOption(role, prefix) {
  const wrapper = document.createElement("div");
  wrapper.className = "role-option";

  const input = document.createElement("input");
  input.type = "checkbox";
  input.value = role.id;
  const inputId = `${prefix}-${role.id}`;
  input.id = inputId;

  const label = document.createElement("label");
  label.setAttribute("for", inputId);

  const nameSpan = document.createElement("span");
  nameSpan.className = "role-name";
  nameSpan.textContent = role.name || role.id;

  label.appendChild(nameSpan);

  if (role.description) {
    const descSpan = document.createElement("span");
    descSpan.className = "role-description";
    descSpan.textContent = role.description;
    label.appendChild(descSpan);
  }

  if (Array.isArray(role.featureFlags) && role.featureFlags.length) {
    const flags = document.createElement("div");
    flags.className = "role-flags";
    role.featureFlags.forEach((flag) => {
      const flagEl = document.createElement("span");
      flagEl.className = "role-flag";
      flagEl.textContent = flag;
      flags.appendChild(flagEl);
    });
    label.appendChild(flags);
  }

  wrapper.appendChild(input);
  wrapper.appendChild(label);
  return wrapper;
}

/** 選択されているロール ID を配列で返す。 */
function collectSelectedRoles(container) {
  const values = [];
  const inputs = container.querySelectorAll("input[type=checkbox]:checked");
  inputs.forEach((input) => {
    values.push(input.value);
  });
  return values;
}

/** ロール一覧を API から取得してキャッシュする。 */
async function loadRoles() {
  try {
    const { response, data } = await fetchJson("/api/admin/roles");
    if (!response.ok || !Array.isArray(data)) {
      throw new Error();
    }
    cachedRoles = data;
  } catch (error) {
    cachedRoles = [];
  }
  renderRoleOptions();
}

/** アカウント作成フォーム送信時の処理。 */
async function handleAccountSubmit(event) {
  event.preventDefault();
  setResultMessage(accountResultEl, "送信中...", null);

  const loginId = accountForm.loginId.value.trim();
  const displayName = accountForm.displayName.value.trim();
  const emailRaw = accountForm.email.value.trim();
  const password = accountForm.password.value;
  const passwordConfirm = accountForm.passwordConfirm.value;
  const selectedRoles = collectSelectedRoles(accountRolesContainer);

  if (password !== passwordConfirm) {
    setResultMessage(accountResultEl, "パスワードと再入力が一致しません", "error");
    return;
  }
  if (!selectedRoles.length) {
    setResultMessage(accountResultEl, "少なくとも1つのロールを選択してください", "error");
    return;
  }

  const payload = {
    loginId,
    displayName,
    password,
    roles: selectedRoles
  };
  if (emailRaw) {
    payload.email = emailRaw;
  } else {
    payload.email = null;
  }

  try {
    const { response, data, text } = await fetchJson("/api/accounts", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(payload)
    });

    if (response.ok) {
      accountForm.reset();
      renderRoleOptions();
      const accountId = data && data.accountId ? data.accountId : "不明";
      setResultMessage(accountResultEl, `アカウントを作成しました (ID: ${accountId})`, "success");
      return;
    }

    // エラーレスポンスを日本語メッセージへ変換する。
    const message = buildAccountErrorMessage(response.status, data, text);
    setResultMessage(accountResultEl, message, "error");
  } catch (error) {
    setResultMessage(accountResultEl, "アカウント作成中にエラーが発生しました", "error");
  }
}

/** アカウント作成エラーを人間が読みやすい文に整形する。 */
function buildAccountErrorMessage(status, data, text) {
  if (!data || typeof data !== "object") {
    return `アカウント作成に失敗しました (HTTP ${status})\n${text || ""}`;
  }
  if (status === 400 || status === 422) {
    if (Array.isArray(data.details) && data.details.length) {
      const issues = data.details.map((issue) => `${issue.field}: ${issue.message}`).join("\n");
      return `入力内容に誤りがあります:\n${issues}`;
    }
    return "入力内容に誤りがあります";
  }
  if (status === 409) {
    if (data.error === "role_conflict" && data.conflictRole) {
      return `ロールの排他制約により更新できません: ${data.conflictRole}`;
    }
    return "同じログインIDのアカウントが既に存在します";
  }
  if (status === 401) {
    return "認証に失敗しました。再度ログインしてください";
  }
  if (status === 403) {
    return "アカウント作成の権限がありません";
  }
  return `アカウント作成に失敗しました (HTTP ${status})`;
}

/** ロール更新フォーム送信時の処理。 */
async function handleRoleSubmit(event) {
  event.preventDefault();
  setResultMessage(roleResultEl, "送信中...", null);

  const accountId = roleForm.accountId.value.trim();
  const comment = roleForm.comment.value.trim();
  const selectedRoles = collectSelectedRoles(roleCheckboxContainer);

  if (!accountId) {
    setResultMessage(roleResultEl, "アカウントIDを入力してください", "error");
    return;
  }
  if (!selectedRoles.length) {
    setResultMessage(roleResultEl, "付与するロールを選択してください", "error");
    return;
  }

  const payload = {
    roles: selectedRoles
  };
  if (comment) {
    payload.comment = comment;
  }

  try {
    const { response, data, text } = await fetchJson(`/api/admin/roles?accountId=${encodeURIComponent(accountId)}`, {
      method: "PUT",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(payload)
    });

    if (response.status === 204) {
      setResultMessage(roleResultEl, "ロールを更新しました", "success");
      return;
    }

    const message = buildRoleErrorMessage(response.status, data, text);
    setResultMessage(roleResultEl, message, "error");
  } catch (error) {
    setResultMessage(roleResultEl, "ロール更新中にエラーが発生しました", "error");
  }
}

/** ロール更新エラーを判読しやすくする。 */
function buildRoleErrorMessage(status, data, text) {
  if (status === 400) {
    return "リクエスト形式が正しくありません";
  }
  if (status === 404) {
    return "指定したアカウントIDが見つかりません";
  }
  if (status === 409) {
    if (data && data.conflictRole) {
      return `ロールの排他制約により更新できません: ${data.conflictRole}`;
    }
    return "ロールの排他制約により更新できません";
  }
  if (status === 401) {
    return "認証に失敗しました。再度ログインしてください";
  }
  if (status === 403) {
    return "ロール更新の権限がありません";
  }
  const fallback = text || "ロール更新に失敗しました";
  return `${fallback} (HTTP ${status})`;
}

/** フォームリセット時にメッセージとチェック状態を初期化する。 */
function handleAccountReset() {
  setResultMessage(accountResultEl, "", null);
  renderRoleOptions();
}

function handleRoleReset() {
  setResultMessage(roleResultEl, "", null);
  renderRoleOptions();
}

function populateObjectTypeSelectFromTemplates() {
  if (!mapObjectTypeSelect) {
    return;
  }
  const prevValue = mapObjectTypeSelect.value;
  mapObjectTypeSelect.innerHTML = "";
  const emptyOpt = document.createElement("option");
  emptyOpt.value = "";
  emptyOpt.textContent = "-- オブジェクトを選択 --";
  mapObjectTypeSelect.appendChild(emptyOpt);
  mapObjectTemplateState.templates.forEach((tmpl) => {
    const opt = document.createElement("option");
    opt.value = String(tmpl.objectId);
    opt.textContent = `[${tmpl.objectId}] ${tmpl.name || "(無名)"}`;
    mapObjectTypeSelect.appendChild(opt);
  });
  if (prevValue) {
    mapObjectTypeSelect.value = prevValue;
  }
}

function initializeMapObjectView() {
  if (!mapObjectMapSelect) {
    return;
  }
  populateMapObjectTypeSelect(mapObjectFilterSelect, true);
  populateMapOptions();
  if (mapObjectFilterSelect) {
    mapObjectFilterSelect.value = mapObjectState.filterType;
    mapObjectFilterSelect.disabled = mapObjectState.maps.length === 0;
  }

  applyObjectToForm(null, getSelectedMapData());
  renderMapComponents();
  setMapObjectFeedback("", null);
  /* テンプレートを先にロードして objectId セレクトを初期化してからデータをロード */
  if (!mapObjectTemplateState.templates.length && !mapObjectTemplateState.isLoading) {
    loadObjectTemplates().then(() => {
      populateObjectTypeSelectFromTemplates();
      loadMapObjectData(false);
    });
  } else {
    populateObjectTypeSelectFromTemplates();
    loadMapObjectData(false);
  }
}

function populateMapOptions() {
  if (!mapObjectMapSelect) {
    return;
  }
  mapObjectMapSelect.innerHTML = "";
  if (!mapObjectState.maps.length) {
    const option = document.createElement("option");
    option.value = "";
    option.textContent = mapObjectState.isLoading ? "読み込み中..." : "マップデータなし";
    mapObjectMapSelect.appendChild(option);
    mapObjectMapSelect.disabled = true;
    return;
  }
  mapObjectState.maps.forEach((map) => {
    const option = document.createElement("option");
    option.value = map.id;
    option.textContent = map.id ? `${map.name} (ID: ${map.id})` : map.name;
    mapObjectMapSelect.appendChild(option);
  });
  mapObjectMapSelect.disabled = mapObjectState.isLoading;
  if (mapObjectState.selectedMapId) {
    mapObjectMapSelect.value = mapObjectState.selectedMapId;
  }
}

function populateMapObjectTypeSelect(select, includeAll) {
  if (!select) {
    return;
  }
  select.innerHTML = "";
  if (includeAll) {
    const allOption = document.createElement("option");
    allOption.value = "all";
    allOption.textContent = "すべて";
    select.appendChild(allOption);
  }
  MAP_OBJECT_TYPE_OPTIONS.forEach((item) => {
    const option = document.createElement("option");
    option.value = item.value;
    option.textContent = item.label;
    select.appendChild(option);
  });
  if (includeAll) {
    select.value = mapObjectState.filterType;
  } else if (select.options.length) {
    select.value = select.options[0].value;
  }
}

function getSelectedMapData() {
  return mapObjectState.maps.find((map) => map.id === mapObjectState.selectedMapId) || null;
}

function getMapObjectTypeLabel(type) {
  const found = MAP_OBJECT_TYPE_OPTIONS.find((item) => item.value === type);
  return found ? found.label : type;
}

function renderMapSummary(map) {
  if (!mapObjectSummaryEl) {
    return;
  }
  if (mapObjectState.isLoading) {
    mapObjectSummaryEl.textContent = "マップ情報を読み込み中...";
    mapObjectSummaryEl.title = "";
    return;
  }
  if (mapObjectState.loadError) {
    mapObjectSummaryEl.textContent = mapObjectState.loadError;
    mapObjectSummaryEl.title = "";
    return;
  }
  if (!map) {
    const fallback = mapObjectState.maps.length ? "管理対象のマップが選択されていません" : "表示可能なマップデータがありません";
    mapObjectSummaryEl.textContent = fallback;
    mapObjectSummaryEl.title = "";
    return;
  }
  const weather = map.weatherLabel || getWeatherLabel(map.weatherType);
  const battle = map.battleEnabled ? "戦闘可能" : "戦闘不可";
  const recovery = map.recoveryEnabled ? "復活地点あり" : "復活地点なし";
  const idLabel = map.id ? (map.idHex ? `${map.id} (${map.idHex})` : map.id) : "-";
  mapObjectSummaryEl.textContent = `${map.name} / ID: ${idLabel} / サイズ: ${map.width}×${map.height} / 天候: ${weather} / ${battle} / ${recovery} / オブジェクト数: ${map.objects.length}`;
  mapObjectSummaryEl.title = map.name;
}

function renderMapObjectTable(map) {
  if (!mapObjectTableBody) {
    return;
  }
  mapObjectTableBody.innerHTML = "";
  const appendMessageRow = (text, columnSpan = 7) => {
    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = columnSpan;
    cell.textContent = text;
    row.appendChild(cell);
    mapObjectTableBody.appendChild(row);
  };

  if (mapObjectState.isLoading) {
    appendMessageRow("マップ情報を読み込み中です");
    return;
  }
  if (mapObjectState.loadError) {
    appendMessageRow(mapObjectState.loadError);
    return;
  }
  if (!map) {
    appendMessageRow(mapObjectState.maps.length ? "マップが選択されていません" : "表示できるマップがありません");
    return;
  }

  const filtered = map.objects.filter((object) => {
    if (mapObjectState.filterType === "collision") {
      return object.hasCollision;
    }
    if (mapObjectState.filterType === "no-collision") {
      return !object.hasCollision;
    }
    return true;
  });

  if (!filtered.length) {
    appendMessageRow("表示できるオブジェクトがありません");
    return;
  }

  filtered
    .slice()
    .sort((a, b) => {
      if (a.y === b.y) {
        return a.x - b.x;
      }
      return a.y - b.y;
    })
    .forEach((object) => {
      const row = document.createElement("tr");
      row.dataset.objectId = object.id;
      row.tabIndex = 0;
      if (object.id === mapObjectState.selectedObjectId) {
        row.classList.add("is-selected");
      }

      const placementCell = document.createElement("td");
      placementCell.textContent = object.placementId !== null ? object.placementId.toString() : "-";
      const objectIdCell = document.createElement("td");
      if (object.objectId !== null) {
        objectIdCell.textContent = object.objectIdHex ? `${object.objectId} (${object.objectIdHex})` : object.objectId.toString();
      } else {
        objectIdCell.textContent = "-";
      }
      const nameCell = document.createElement("td");
      nameCell.textContent = object.name;
      const xCell = document.createElement("td");
      xCell.textContent = object.x.toString();
      const yCell = document.createElement("td");
      yCell.textContent = object.y.toString();
      const attributeCell = document.createElement("td");
      attributeCell.textContent = object.attributeHex || "-";
      const collisionCell = document.createElement("td");
      collisionCell.textContent = object.hasCollision ? "あり" : "なし";

      row.appendChild(placementCell);
      row.appendChild(objectIdCell);
      row.appendChild(nameCell);
      row.appendChild(xCell);
      row.appendChild(yCell);
      row.appendChild(attributeCell);
      row.appendChild(collisionCell);

      row.addEventListener("click", () => {
        selectMapObject(object.id);
      });
      row.addEventListener("keydown", (event) => {
        if (event.key === "Enter" || event.key === " ") {
          event.preventDefault();
          selectMapObject(object.id);
        }
      });

      mapObjectTableBody.appendChild(row);
    });
}

function renderMapPreview(map) {
  if (!mapObjectGridEl) {
    return;
  }
  mapObjectGridEl.innerHTML = "";
  const showMessage = (message) => {
    const empty = document.createElement("div");
    empty.className = "empty-message";
    empty.textContent = message;
    mapObjectGridEl.style.gridTemplateColumns = "repeat(1, minmax(80px, 1fr))";
    mapObjectGridEl.appendChild(empty);
  };

  if (mapObjectState.isLoading) {
    showMessage("マップ情報を読み込み中です");
    return;
  }
  if (mapObjectState.loadError) {
    showMessage(mapObjectState.loadError);
    return;
  }
  if (!map) {
    showMessage(mapObjectState.maps.length ? "マップが選択されていません" : "表示できるマップがありません");
    return;
  }

  const hasTiles = Array.isArray(map.tiles) && map.tiles.length > 0;
  const tileSize = mapObjectState.tileSize || 16;
  const cellPx = tileSize * 2;
  const sheetTileWidth = mapObjectState.sheetTileWidth || 32;
  const sheetTileHeight = mapObjectState.sheetTileHeight || 32;
  const sheetBaseUrl = mapObjectState.sheetBaseUrl || "/api/assets/map-parts/sheets";

  if (hasTiles) {
    mapObjectGridEl.style.gridTemplateColumns = `repeat(${Math.max(map.width, 1)}, ${cellPx}px)`;
    mapObjectGridEl.classList.add("has-tiles");
  } else {
    mapObjectGridEl.style.gridTemplateColumns = `repeat(${Math.max(map.width, 1)}, minmax(28px, 1fr))`;
    mapObjectGridEl.classList.remove("has-tiles");
  }

  const occupantMap = new Map();
  map.objects.forEach((object) => {
    occupantMap.set(`${object.x},${object.y}`, object);
  });

  const preview = mapObjectState.previewSelection;
  const sheetWidthPx = sheetTileWidth * cellPx;
  const sheetHeightPx = sheetTileHeight * cellPx;

  for (let y = 0; y < map.height; y += 1) {
    for (let x = 0; x < map.width; x += 1) {
      const key = `${x},${y}`;
      const occupant = occupantMap.get(key) || null;
      const cellButton = document.createElement("button");
      cellButton.type = "button";
      cellButton.className = "map-object-cell";
      cellButton.dataset.x = x.toString();
      cellButton.dataset.y = y.toString();
      cellButton.setAttribute("role", "gridcell");
      cellButton.setAttribute("aria-label", `X: ${x}, Y: ${y}`);

      if (hasTiles) {
        const tileIndex = y * map.width + x;
        const grpId = Number(map.tiles[tileIndex]) || 0;
        const sheet = Math.floor(grpId / 1024);
        const tile = grpId % 1024;
        const tileX = tile % sheetTileWidth;
        const tileY = Math.floor(tile / sheetTileWidth);
        const normalizedBase = sheetBaseUrl.endsWith("/") ? sheetBaseUrl.slice(0, -1) : sheetBaseUrl;
        const sheetUrl = `${normalizedBase}/${sheet}.png`;
        const offsetX = -(tileX * cellPx);
        const offsetY = -(tileY * cellPx);
        cellButton.style.width = `${cellPx}px`;
        cellButton.style.height = `${cellPx}px`;
        cellButton.style.backgroundImage = `url(${sheetUrl})`;
        cellButton.style.backgroundSize = `${sheetWidthPx}px ${sheetHeightPx}px`;
        cellButton.style.backgroundPosition = `${offsetX}px ${offsetY}px`;
      }

      if (occupant) {
        cellButton.classList.add("has-object");
        const titleParts = [occupant.name, getMapObjectTypeLabel(occupant.type)];
        if (occupant.attributeHex) {
          titleParts.push(`属性: ${occupant.attributeHex}`);
        }
        if (occupant.objectIdHex) {
          titleParts.push(`ID: ${occupant.objectIdHex}`);
        }
        cellButton.title = titleParts.join(" / ");
        const mark = document.createElement("span");
        mark.textContent = "●";
        cellButton.appendChild(mark);
        if (occupant.id === mapObjectState.selectedObjectId) {
          cellButton.classList.add("is-selected");
        }
      } else if (preview && preview.x === x && preview.y === y) {
        cellButton.classList.add("is-selected");
      }

      cellButton.addEventListener("click", () => {
        handleMapCellClick(x, y, occupant ? occupant.id : null);
      });

      mapObjectGridEl.appendChild(cellButton);
    }
  }
}

function setMapObjectFeedback(message, type) {
  if (!mapObjectFeedbackEl) {
    return;
  }
  mapObjectFeedbackEl.textContent = message || "";
  mapObjectFeedbackEl.className = "result-message";
  if (type) {
    mapObjectFeedbackEl.classList.add(type);
  }
}

function clampCoordinate(value, max) {
  if (max < 0) {
    return 0;
  }
  return Math.max(0, Math.min(max, value));
}

function enforceMapObjectReadOnly() {
  /* 編集モードに変更したため何もしない */
}

function applyObjectToForm(object, map) {
  if (!mapObjectForm) {
    return;
  }

  const selectedMap = map || getSelectedMapData();
  const maxX = selectedMap ? selectedMap.width - 1 : 0;
  const maxY = selectedMap ? selectedMap.height - 1 : 0;

  if (object) {
    if (mapObjectDataIdInput) {
      mapObjectDataIdInput.value = object.placementId !== null ? object.placementId : "";
    }
    if (mapObjectPlacementIdDisplay) {
      mapObjectPlacementIdDisplay.value = object.placementId !== null ? String(object.placementId) : "(新規)";
    }
    if (mapObjectTypeSelect && object.objectId !== null) {
      mapObjectTypeSelect.value = String(object.objectId);
    }
    if (mapObjectXInput) {
      mapObjectXInput.value = clampCoordinate(object.x, maxX);
    }
    if (mapObjectYInput) {
      mapObjectYInput.value = clampCoordinate(object.y, maxY);
    }
    const idLabel = object.objectIdHex ? ` (${object.objectIdHex})` : "";
    if (mapObjectSelectedLabel) {
      mapObjectSelectedLabel.textContent = `選択中: ${object.name}${idLabel}`;
    }
    if (mapObjectDeleteButton) {
      mapObjectDeleteButton.disabled = false;
    }
  } else {
    const preview = mapObjectState.previewSelection;
    if (mapObjectDataIdInput) {
      mapObjectDataIdInput.value = "";
    }
    if (mapObjectPlacementIdDisplay) {
      mapObjectPlacementIdDisplay.value = "(新規)";
    }
    if (mapObjectTypeSelect && mapObjectTypeSelect.options.length) {
      mapObjectTypeSelect.selectedIndex = 0;
    }
    if (mapObjectXInput) {
      mapObjectXInput.value = clampCoordinate(preview ? preview.x : 0, maxX);
    }
    if (mapObjectYInput) {
      mapObjectYInput.value = clampCoordinate(preview ? preview.y : 0, maxY);
    }
    if (mapObjectSelectedLabel) {
      mapObjectSelectedLabel.textContent = "選択中: 未選択";
    }
    if (mapObjectDeleteButton) {
      mapObjectDeleteButton.disabled = true;
    }
  }
}

function renderMapComponents() {
  const map = getSelectedMapData();
  renderMapSummary(map);
  renderMapObjectTable(map);
  renderMapPreview(map);
}

function selectMapObject(objectId) {
  const map = getSelectedMapData();
  if (!map) {
    return;
  }
  if (!objectId) {
    mapObjectState.selectedObjectId = null;
    mapObjectState.previewSelection = null;
    applyObjectToForm(null, map);
    renderMapComponents();
    return;
  }
  const target = map.objects.find((item) => item.id === objectId);
  if (!target) {
    mapObjectState.selectedObjectId = null;
    applyObjectToForm(null, map);
    renderMapComponents();
    return;
  }
  mapObjectState.selectedObjectId = objectId;
  mapObjectState.previewSelection = { x: target.x, y: target.y };
  applyObjectToForm(target, map);
  renderMapComponents();
}

function handleMapSelectChange() {
  mapObjectState.selectedMapId = mapObjectMapSelect ? mapObjectMapSelect.value : null;
  mapObjectState.selectedObjectId = null;
  mapObjectState.previewSelection = null;
  setMapObjectFeedback("", null);
  const map = getSelectedMapData();
  applyObjectToForm(null, map);
  renderMapComponents();
}

function handleMapFilterChange() {
  mapObjectState.filterType = mapObjectFilterSelect ? mapObjectFilterSelect.value : "all";
  renderMapObjectTable(getSelectedMapData());
}

function handleMapCellClick(x, y, objectId) {
  const map = getSelectedMapData();
  if (!map) {
    return;
  }
  if (objectId) {
    selectMapObject(objectId);
    return;
  }
  mapObjectState.previewSelection = { x, y };
  mapObjectState.selectedObjectId = null;
  applyObjectToForm(null, map);
  mapObjectXInput.value = x;
  mapObjectYInput.value = y;
  renderMapComponents();
  setMapObjectFeedback(`座標 (${x}, ${y}) を選択しました。オブジェクトを選択して「配置を保存」で新規追加します。`, null);
}

async function handleMapObjectFormSubmit(event) {
  event.preventDefault();
  const map = getSelectedMapData();
  if (!map || !map.id) {
    setMapObjectFeedback("マップを選択してください", "error");
    return;
  }

  const objectIdVal = mapObjectTypeSelect ? Number(mapObjectTypeSelect.value) : 0;
  if (!objectIdVal || objectIdVal <= 0) {
    setMapObjectFeedback("オブジェクトを選択してください", "error");
    return;
  }
  const x = mapObjectXInput ? Number(mapObjectXInput.value) : 0;
  const y = mapObjectYInput ? Number(mapObjectYInput.value) : 0;
  const dataId = mapObjectDataIdInput ? Number(mapObjectDataIdInput.value) : 0;

  setMapObjectFeedback("保存中...", null);

  try {
    const isUpdate = dataId > 0;
    const payload = isUpdate
      ? { mapId: map.id, dataId, objectId: objectIdVal, x, y }
      : { mapId: map.id, objectId: objectIdVal, x, y };
    const { response, data } = await fetchJson(
      "/api/maps/placements",
      { method: isUpdate ? "PUT" : "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(payload) }
    );
    if (!response.ok) {
      setMapObjectFeedback(`保存失敗: ${data?.error || response.status}`, "error");
      return;
    }
    setMapObjectFeedback(`配置を${isUpdate ? "更新" : "追加"}しました (配置ID: ${data.dataId})`, "success");
    await loadMapObjectData(true);
    const newDataId = data.dataId;
    const newMap = getSelectedMapData();
    if (newMap) {
      const addedPlacement = newMap.objects.find((o) => o.placementId === newDataId);
      if (addedPlacement) {
        selectMapObject(addedPlacement.id);
      }
    }
  } catch (err) {
    setMapObjectFeedback("保存中にエラーが発生しました", "error");
  }
}

function handleMapObjectClear() {
  const map = getSelectedMapData();
  mapObjectState.selectedObjectId = null;
  mapObjectState.previewSelection = null;
  applyObjectToForm(null, map);
  renderMapComponents();
  setMapObjectFeedback("選択をクリアしました", null);
}

async function handleMapObjectDelete() {
  const map = getSelectedMapData();
  if (!map || !map.id) {
    setMapObjectFeedback("マップを選択してください", "error");
    return;
  }
  const dataId = mapObjectDataIdInput ? Number(mapObjectDataIdInput.value) : 0;
  if (!dataId || dataId <= 0) {
    setMapObjectFeedback("削除する配置を選択してください", "error");
    return;
  }
  if (!window.confirm("この配置を削除しますか？")) {
    return;
  }

  setMapObjectFeedback("削除中...", null);
  try {
    const { response, data } = await fetchJson(
      "/api/maps/placements",
      { method: "DELETE", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ mapId: map.id, dataId }) }
    );
    if (!response.ok) {
      setMapObjectFeedback(`削除失敗: ${data?.error || response.status}`, "error");
      return;
    }
    setMapObjectFeedback("配置を削除しました", "success");
    mapObjectState.selectedObjectId = null;
    mapObjectState.previewSelection = null;
    await loadMapObjectData(true);
    applyObjectToForm(null, getSelectedMapData());
    renderMapComponents();
  } catch (err) {
    setMapObjectFeedback("削除中にエラーが発生しました", "error");
  }
}

/* ================================================================
   オブジェクトテンプレート管理
   ================================================================ */

function setTemplateFeedback(message, type) {
  if (!mapObjectTemplateFeedbackEl) {
    return;
  }
  mapObjectTemplateFeedbackEl.textContent = message || "";
  mapObjectTemplateFeedbackEl.className = "result-message";
  if (type) {
    mapObjectTemplateFeedbackEl.classList.add(type);
  }
}

function renderTemplateTable() {
  if (!mapObjectTemplateTableBody) {
    return;
  }
  mapObjectTemplateTableBody.innerHTML = "";
  if (!mapObjectTemplateState.templates.length) {
    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = 6;
    cell.textContent = mapObjectTemplateState.isLoading ? "読み込み中..." : "テンプレートがありません";
    row.appendChild(cell);
    mapObjectTemplateTableBody.appendChild(row);
    if (mapObjectTemplateSummaryEl) {
      mapObjectTemplateSummaryEl.textContent = "テンプレートなし";
    }
    return;
  }
  if (mapObjectTemplateSummaryEl) {
    mapObjectTemplateSummaryEl.textContent = `テンプレート数: ${mapObjectTemplateState.templates.length}`;
  }
  mapObjectTemplateState.templates.forEach((tmpl) => {
    const row = document.createElement("tr");
    if (tmpl.objectId === mapObjectTemplateState.selectedId) {
      row.classList.add("is-selected-row");
    }
    row.style.cursor = "pointer";
    const cells = [
      tmpl.objectId,
      tmpl.name || "(無名)",
      tmpl.attribute,
      tmpl.hasCollision ? "あり" : "なし",
      `${tmpl.width}×${tmpl.height}`,
      tmpl.hideY
    ];
    cells.forEach((text) => {
      const td = document.createElement("td");
      td.textContent = String(text);
      row.appendChild(td);
    });
    row.addEventListener("click", () => {
      selectTemplate(tmpl.objectId);
    });
    mapObjectTemplateTableBody.appendChild(row);
  });
}

function selectTemplate(objectId) {
  const tmpl = mapObjectTemplateState.templates.find((t) => t.objectId === objectId) || null;
  mapObjectTemplateState.selectedId = tmpl ? tmpl.objectId : null;
  applyTemplateToForm(tmpl);
  renderTemplateTable();
}

function applyTemplateToForm(tmpl) {
  if (!mapObjectTemplateForm) {
    return;
  }
  if (tmpl) {
    if (mapObjectTemplateIdInput) {
      mapObjectTemplateIdInput.value = tmpl.objectId;
    }
    if (mapObjectTemplateNameInput) {
      mapObjectTemplateNameInput.value = tmpl.name || "";
    }
    if (mapObjectTemplateAttrInput) {
      mapObjectTemplateAttrInput.value = tmpl.attribute || 0;
    }
    if (mapObjectTemplateHideYInput) {
      mapObjectTemplateHideYInput.value = tmpl.hideY || 0;
    }
    if (mapObjectTemplateWidthInput) {
      mapObjectTemplateWidthInput.value = tmpl.width || 1;
    }
    if (mapObjectTemplateHeightInput) {
      mapObjectTemplateHeightInput.value = tmpl.height || 1;
    }
    if (mapObjectTemplateCollisionInput) {
      mapObjectTemplateCollisionInput.checked = Boolean(tmpl.hasCollision);
    }
    if (mapObjectTemplateDeleteBtn) {
      mapObjectTemplateDeleteBtn.disabled = false;
    }
  } else {
    if (mapObjectTemplateIdInput) {
      mapObjectTemplateIdInput.value = "";
    }
    if (mapObjectTemplateNameInput) {
      mapObjectTemplateNameInput.value = "";
    }
    if (mapObjectTemplateAttrInput) {
      mapObjectTemplateAttrInput.value = 0;
    }
    if (mapObjectTemplateHideYInput) {
      mapObjectTemplateHideYInput.value = 0;
    }
    if (mapObjectTemplateWidthInput) {
      mapObjectTemplateWidthInput.value = 1;
    }
    if (mapObjectTemplateHeightInput) {
      mapObjectTemplateHeightInput.value = 1;
    }
    if (mapObjectTemplateCollisionInput) {
      mapObjectTemplateCollisionInput.checked = false;
    }
    if (mapObjectTemplateDeleteBtn) {
      mapObjectTemplateDeleteBtn.disabled = true;
    }
  }
}

async function loadObjectTemplates() {
  mapObjectTemplateState.isLoading = true;
  renderTemplateTable();
  try {
    const { response, data } = await fetchJson("/api/maps/objects/templates");
    if (!response.ok || !Array.isArray(data?.templates)) {
      throw new Error("invalid_response");
    }
    mapObjectTemplateState.templates = data.templates;
  } catch (err) {
    mapObjectTemplateState.templates = [];
  }
  mapObjectTemplateState.isLoading = false;
  renderTemplateTable();
}

async function handleTemplateSave(event) {
  event.preventDefault();
  const name = mapObjectTemplateNameInput ? mapObjectTemplateNameInput.value.trim() : "";
  if (!name) {
    setTemplateFeedback("名称を入力してください", "error");
    return;
  }
  const objectId = mapObjectTemplateIdInput ? Number(mapObjectTemplateIdInput.value) : 0;
  const attribute = mapObjectTemplateAttrInput ? Number(mapObjectTemplateAttrInput.value) : 0;
  const hideY = mapObjectTemplateHideYInput ? Number(mapObjectTemplateHideYInput.value) : 0;
  const width = mapObjectTemplateWidthInput ? Number(mapObjectTemplateWidthInput.value) : 1;
  const height = mapObjectTemplateHeightInput ? Number(mapObjectTemplateHeightInput.value) : 1;
  const hasCollision = mapObjectTemplateCollisionInput ? mapObjectTemplateCollisionInput.checked : false;

  const isUpdate = objectId > 0;
  const payload = { name, attribute, hideY, width, height, hasCollision };
  if (isUpdate) {
    payload.objectId = objectId;
  }

  setTemplateFeedback("保存中...", null);
  try {
    const { response, data } = await fetchJson(
      "/api/maps/objects/templates",
      { method: isUpdate ? "PUT" : "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(payload) }
    );
    if (!response.ok) {
      setTemplateFeedback(`保存失敗: ${data?.error || response.status}`, "error");
      return;
    }
    setTemplateFeedback(`テンプレートを${isUpdate ? "更新" : "追加"}しました (ID: ${data.objectId})`, "success");
    await loadObjectTemplates();
    selectTemplate(data.objectId);
    /* 配置フォームのobjSelectも更新 */
    populateObjectTypeSelectFromTemplates();
  } catch (err) {
    setTemplateFeedback("保存中にエラーが発生しました", "error");
  }
}

async function handleTemplateDelete() {
  const objectId = mapObjectTemplateIdInput ? Number(mapObjectTemplateIdInput.value) : 0;
  if (!objectId || objectId <= 0) {
    setTemplateFeedback("削除するテンプレートを選択してください", "error");
    return;
  }
  if (!window.confirm(`オブジェクトID ${objectId} のテンプレートを削除しますか？`)) {
    return;
  }

  setTemplateFeedback("削除中...", null);
  try {
    const { response, data } = await fetchJson(
      "/api/maps/objects/templates",
      { method: "DELETE", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ objectId }) }
    );
    if (!response.ok) {
      setTemplateFeedback(`削除失敗: ${data?.error || response.status}`, "error");
      return;
    }
    setTemplateFeedback("テンプレートを削除しました", "success");
    mapObjectTemplateState.selectedId = null;
    await loadObjectTemplates();
    applyTemplateToForm(null);
    populateObjectTypeSelectFromTemplates();
  } catch (err) {
    setTemplateFeedback("削除中にエラーが発生しました", "error");
  }
}

function handleTemplateNew() {
  mapObjectTemplateState.selectedId = null;
  applyTemplateToForm(null);
  renderTemplateTable();
  setTemplateFeedback("新規追加モード: 情報を入力して「保存」してください", null);
}

function getMapPartsViewTypeLabel(viewType) {
  if (typeof viewType !== "number" || !Number.isFinite(viewType)) {
    return "-";
  }
  const label = Object.prototype.hasOwnProperty.call(MAP_PARTS_VIEW_TYPE_LABELS, viewType)
    ? MAP_PARTS_VIEW_TYPE_LABELS[viewType]
    : `タイプ ${viewType}`;
  return `${label} (${viewType})`;
}

function getMapPartsAnimeTypeLabel(animeType) {
  if (typeof animeType !== "number" || !Number.isFinite(animeType)) {
    return "-";
  }
  if (Object.prototype.hasOwnProperty.call(MAP_PARTS_ANIME_TYPE_LABELS, animeType)) {
    return `${MAP_PARTS_ANIME_TYPE_LABELS[animeType]} (${animeType})`;
  }
  return `タイプ ${animeType}`;
}

function getMapPartsMovementLabel(direction) {
  if (!direction) {
    return "なし";
  }
  const normalized = direction.toLowerCase();
  const label = Object.prototype.hasOwnProperty.call(MAP_PARTS_DIRECTION_LABELS, normalized)
    ? MAP_PARTS_DIRECTION_LABELS[normalized]
    : normalized;
  return `${label} 方向`;
}

function normalizeMapPartSprite(rawSprite) {
  if (!rawSprite || typeof rawSprite.sheet !== "number") {
    return null;
  }
  const sheet = Number(rawSprite.sheet);
  if (!Number.isFinite(sheet) || sheet < 0) {
    return null;
  }
  const tileWidth = mapPartsState.sheetTileWidth || 32;
  const tileHeight = mapPartsState.sheetTileHeight || 32;
  let tile = Number(rawSprite.tile);
  const tileXRaw = Number(rawSprite.tileX);
  const tileYRaw = Number(rawSprite.tileY);
  let tileX = Number.isFinite(tileXRaw) ? tileXRaw : 0;
  let tileY = Number.isFinite(tileYRaw) ? tileYRaw : 0;

  if (!Number.isFinite(tile)) {
    tile = tileY * tileWidth + tileX;
  } else {
    if (!Number.isFinite(tileXRaw)) {
      tileX = tile % tileWidth;
    }
    if (!Number.isFinite(tileYRaw)) {
      tileY = Math.floor(tile / tileWidth);
    }
  }

  tileX = Math.max(0, Math.min(tileWidth - 1, tileX));
  tileY = Math.max(0, Math.min(tileHeight - 1, tileY));

  return {
    sheet,
    tile: Number.isFinite(tile) ? tile : tileY * tileWidth + tileX,
    tileX,
    tileY
  };
}

function normalizeMapPart(raw) {
  if (!raw || typeof raw !== "object") {
    return null;
  }

  const partsIdRaw = raw.partsId ?? raw.partsID;
  const partsId = Number(partsIdRaw);
  const viewType = Number(raw.viewType);
  const animeType = Number(raw.animeType);
  const animeCount = Number(raw.animeCount);
  const level = Number(raw.level);
  const posX = Number(raw?.viewPosition?.x);
  const posY = Number(raw?.viewPosition?.y);
  const movementDirection = typeof raw?.movement?.direction === "string"
    ? raw.movement.direction.trim().toLowerCase()
    : null;

  const flags = {
    block: Boolean(raw?.flags?.block),
    pile: Boolean(raw?.flags?.pile),
    pileBack: Boolean(raw?.flags?.pileBack),
    fishing: Boolean(raw?.flags?.fishing),
    drawLast: Boolean(raw?.flags?.drawLast),
    counter: Boolean(raw?.flags?.counter),
    blockDirections: {
      up: Boolean(raw?.flags?.blockDirections?.up),
      down: Boolean(raw?.flags?.blockDirections?.down),
      left: Boolean(raw?.flags?.blockDirections?.left),
      right: Boolean(raw?.flags?.blockDirections?.right)
    }
  };

  const sprites = {
    base: normalizeMapPartSprite(raw?.sprites?.base),
    overlay: normalizeMapPartSprite(raw?.sprites?.overlay)
  };

  const blockDirectionLabels = Object.entries(flags.blockDirections)
    .filter(([, value]) => value)
    .map(([key]) => MAP_PARTS_DIRECTION_LABELS[key] || key.toUpperCase());

  const flagLabels = Object.entries(MAP_PARTS_FLAG_LABELS)
    .filter(([key]) => flags[key])
    .map(([, label]) => label);

  if (blockDirectionLabels.length) {
    flagLabels.push(`ぶつかり方向: ${blockDirectionLabels.join("・")}`);
  }

  return {
    partsId: Number.isFinite(partsId) ? partsId : 0,
    partsIdHex: formatHex(partsId),
    viewType: Number.isFinite(viewType) ? viewType : null,
    viewTypeLabel: getMapPartsViewTypeLabel(viewType),
    animeType: Number.isFinite(animeType) ? animeType : null,
    animeTypeLabel: getMapPartsAnimeTypeLabel(animeType),
    animeCount: Number.isFinite(animeCount) ? animeCount : null,
    level: Number.isFinite(level) ? level : null,
    viewPosition: {
      x: Number.isFinite(posX) ? posX : 0,
      y: Number.isFinite(posY) ? posY : 0
    },
    movementDirection,
    movementLabel: getMapPartsMovementLabel(movementDirection),
    flags,
    flagLabels,
    sprites,
    blockDirectionLabels
  };
}

function describeSprite(sprite) {
  if (!sprite) {
    return "なし";
  }
  const sheetLabel = formatMapPartsSheetLabel(sprite.sheet);
  return `${sheetLabel} / tile ${sprite.tile} (x: ${sprite.tileX}, y: ${sprite.tileY})`;
}

function getMapPartsSheetUrl(sheetIndex) {
  if (sheetIndex === null || sheetIndex === undefined) {
    return "";
  }
  const base = mapPartsState.sheetBaseUrl || "/api/assets/map-parts/sheets";
  const normalized = base.endsWith("/") ? base.slice(0, -1) : base;
  return `${normalized}/${sheetIndex}.png`;
}

function normalizeSheetIndex(sheetIndex) {
  const numeric = Number(sheetIndex);
  if (!Number.isFinite(numeric) || numeric < 0) {
    return null;
  }
  return Math.trunc(numeric);
}

function buildMapPartsSheetSuffix(normalizedIndex) {
  return String(normalizedIndex + 1).padStart(2, "0");
}

function getMapPartsSheetFileName(sheetIndex) {
  const normalized = normalizeSheetIndex(sheetIndex);
  if (normalized === null) {
    return null;
  }
  const suffix = buildMapPartsSheetSuffix(normalized);
  return `map${suffix}.png`;
}

function getMapPartsSheetResourceId(sheetIndex) {
  const normalized = normalizeSheetIndex(sheetIndex);
  if (normalized === null) {
    return null;
  }
  const suffix = buildMapPartsSheetSuffix(normalized);
  return `IDP_MAP_${suffix}`;
}

function formatMapPartsSheetLabel(sheetIndex) {
  const normalized = normalizeSheetIndex(sheetIndex);
  if (normalized === null) {
    return `シート ${sheetIndex}`;
  }
  const fileName = getMapPartsSheetFileName(normalized);
  const resourceId = getMapPartsSheetResourceId(normalized);
  if (fileName && resourceId) {
    return `${fileName} (${resourceId}, index ${normalized})`;
  }
  return `index ${normalized}`;
}

function updateSpriteLayer(element, sprite) {
  if (!element) {
    return;
  }
  const baseTileSize = mapPartsState.tileSize || 16;
  const datasetTilePixel = Number(element.dataset.tilePixelSize);
  const datasetScale = Number(element.dataset.previewScale);
  const tilePixelSize = Number.isFinite(datasetTilePixel) && datasetTilePixel > 0
    ? datasetTilePixel
    : baseTileSize * (Number.isFinite(datasetScale) && datasetScale > 0 ? datasetScale : 1);
  const sheetTileWidth = mapPartsState.sheetTileWidth || 32;
  const sheetTileHeight = mapPartsState.sheetTileHeight || 32;
  const sheetWidthPx = sheetTileWidth * tilePixelSize;
  const sheetHeightPx = sheetTileHeight * tilePixelSize;

  if (!sprite) {
    element.classList.add("is-empty");
    element.style.backgroundImage = "none";
    element.style.backgroundSize = `${sheetWidthPx}px ${sheetHeightPx}px`;
    element.style.backgroundPosition = "0 0";
    return;
  }

  element.classList.remove("is-empty");

  const offsetX = -(sprite.tileX * tilePixelSize);
  const offsetY = -(sprite.tileY * tilePixelSize);

  element.style.backgroundImage = `url(${getMapPartsSheetUrl(sprite.sheet)})`;
  element.style.backgroundSize = `${sheetWidthPx}px ${sheetHeightPx}px`;
  element.style.backgroundPosition = `${offsetX}px ${offsetY}px`;
}

function setMapPartsFeedback(message, type) {
  if (!mapPartsFeedbackEl) {
    return;
  }
  mapPartsFeedbackEl.textContent = message || "";
  mapPartsFeedbackEl.className = "result-message";
  if (type) {
    mapPartsFeedbackEl.classList.add(type);
  }
}

function setMapPartsSummary(message) {
  if (!mapPartsSummaryEl) {
    return;
  }
  mapPartsSummaryEl.textContent = message;
}

function updateMapPartsSummary() {
  if (!mapPartsSummaryEl) {
    return;
  }
  if (mapPartsState.isLoading) {
    mapPartsSummaryEl.textContent = "マップパーツ情報を読み込み中です...";
    return;
  }
  if (mapPartsState.loadError) {
    mapPartsSummaryEl.textContent = mapPartsState.loadError;
    return;
  }
  if (!mapPartsState.parts.length) {
    mapPartsSummaryEl.textContent = "表示できるマップパーツがありません";
    return;
  }

  const total = mapPartsState.parts.length;
  const filtered = mapPartsState.filtered.length;
  let sheetSummary = "-";
  if (mapPartsState.sheets.length) {
    const labels = mapPartsState.sheets.map((sheet) => {
      const normalized = normalizeSheetIndex(sheet);
      if (normalized === null) {
        return `index ${sheet}`;
      }
      const fileName = getMapPartsSheetFileName(normalized);
      return fileName ? `${fileName} (index ${normalized})` : `index ${normalized}`;
    });
    sheetSummary = labels.join(", ");
  } else if (mapPartsState.sheetCount > 0) {
    const lastIndex = Math.max(mapPartsState.sheetCount - 1, 0);
    const firstFile = getMapPartsSheetFileName(0) || "index 0";
    const lastFile = getMapPartsSheetFileName(lastIndex) || `index ${lastIndex}`;
    sheetSummary = `${firstFile}〜${lastFile} (index 0〜${lastIndex})`;
  }
  const filterNote = mapPartsState.flagFilter !== "all" ? "（フィルター適用中）" : "";

  mapPartsSummaryEl.textContent = `表示件数: ${filtered}/${total} 件 ${filterNote} / 使用シート: ${sheetSummary} / タイルサイズ: ${mapPartsState.tileSize}px`;
}

function renderMapPartsFlagList(part) {
  if (!mapPartsFlagsList) {
    return;
  }
  mapPartsFlagsList.innerHTML = "";
  const labels = part && part.flagLabels ? part.flagLabels : [];
  if (!labels.length) {
    const item = document.createElement("li");
    item.textContent = "フラグなし";
    item.classList.add("is-empty");
    mapPartsFlagsList.appendChild(item);
    return;
  }
  labels.forEach((label) => {
    const item = document.createElement("li");
    item.textContent = label;
    mapPartsFlagsList.appendChild(item);
  });
}

function renderMapPartsPreview() {
  if (!mapPartsDetailId) {
    return;
  }

  const tileSize = mapPartsState.tileSize || 16;
  const detailScale = MAP_PARTS_DETAIL_SCALE;
  const detailTileSize = tileSize * detailScale;

  if (mapPartsPreviewBase) {
    mapPartsPreviewBase.dataset.previewScale = String(detailScale);
    mapPartsPreviewBase.dataset.tilePixelSize = String(detailTileSize);
  }
  if (mapPartsPreviewOverlay) {
    mapPartsPreviewOverlay.dataset.previewScale = String(detailScale);
    mapPartsPreviewOverlay.dataset.tilePixelSize = String(detailTileSize);
  }
  if (mapPartsPreviewBase && mapPartsPreviewBase.parentElement) {
    mapPartsPreviewBase.parentElement.style.setProperty("--map-parts-preview-size", `${detailTileSize}px`);
  }

  const part = mapPartsState.parts.find((item) => item.partsId === mapPartsState.selectedPartId)
    || mapPartsState.filtered[0]
    || null;

  if (!part) {
    mapPartsDetailId.textContent = "-";
    mapPartsDetailView.textContent = "-";
    mapPartsDetailAnime.textContent = "-";
    mapPartsDetailLevel.textContent = "-";
    mapPartsDetailPosition.textContent = "-";
    mapPartsDetailMove.textContent = "-";
    mapPartsSpriteBaseInfo.textContent = "-";
    mapPartsSpriteOverlayInfo.textContent = "-";
    if (mapPartsSheetHint) {
      mapPartsSheetHint.textContent = "スプライトのプレビューはありません";
    }
    updateSpriteLayer(mapPartsPreviewBase, null);
    updateSpriteLayer(mapPartsPreviewOverlay, null);
    renderMapPartsFlagList(null);
    populateMapPartsEditForm(null);
    return;
  }

  mapPartsState.selectedPartId = part.partsId;

  const idLabel = part.partsIdHex
    ? `${part.partsId} (${part.partsIdHex})`
    : String(part.partsId);
  mapPartsDetailId.textContent = idLabel;
  mapPartsDetailView.textContent = part.viewTypeLabel;
  mapPartsDetailAnime.textContent = part.animeCount !== null
    ? `${part.animeTypeLabel} / コマ数: ${part.animeCount}`
    : part.animeTypeLabel;
  mapPartsDetailLevel.textContent = part.level !== null ? String(part.level) : "-";
  mapPartsDetailPosition.textContent = `(${part.viewPosition.x}, ${part.viewPosition.y})`;
  mapPartsDetailMove.textContent = part.movementLabel;

  mapPartsSpriteBaseInfo.textContent = describeSprite(part.sprites.base);
  mapPartsSpriteOverlayInfo.textContent = describeSprite(part.sprites.overlay);

  const usedSheets = [part.sprites.base?.sheet, part.sprites.overlay?.sheet]
    .filter((value) => value !== undefined && value !== null)
    .map((value) => value);

  if (mapPartsSheetHint) {
    if (usedSheets.length) {
      const uniqueSheets = Array.from(new Set(usedSheets)).sort((a, b) => a - b);
      const sheetDescriptions = uniqueSheets.map((sheet) => formatMapPartsSheetLabel(sheet));
      const urls = uniqueSheets.map((sheet) => getMapPartsSheetUrl(sheet));
      mapPartsSheetHint.textContent = `参照シート: ${sheetDescriptions.join(" / ")} / URL: ${urls.join(" / ")}`;
    } else {
      mapPartsSheetHint.textContent = "参照シートはありません";
    }
  }

  updateSpriteLayer(mapPartsPreviewBase, part.sprites.base);
  updateSpriteLayer(mapPartsPreviewOverlay, part.sprites.overlay);
  renderMapPartsFlagList(part);
  populateMapPartsEditForm(part.partsId);
}

function matchesMapPartsFlag(part, filter) {
  if (!part) {
    return false;
  }
  switch (filter) {
    case "block":
      return Boolean(part.flags.block);
    case "pile":
      return Boolean(part.flags.pile);
    case "pileBack":
      return Boolean(part.flags.pileBack);
    case "fishing":
      return Boolean(part.flags.fishing);
    case "drawLast":
      return Boolean(part.flags.drawLast);
    case "counter":
      return Boolean(part.flags.counter);
    case "overlay":
      return Boolean(part.sprites.overlay);
    case "movement":
      return Boolean(part.movementDirection);
    case "all":
    default:
      return true;
  }
}

function applyMapPartsFilters() {
  if (!mapPartsGallery) {
    return;
  }

  mapPartsState.filtered = mapPartsState.parts.filter((part) => matchesMapPartsFlag(part, mapPartsState.flagFilter));

  if (mapPartsState.filtered.length) {
    if (!mapPartsState.filtered.some((part) => part.partsId === mapPartsState.selectedPartId)) {
      mapPartsState.selectedPartId = mapPartsState.filtered[0].partsId;
    }
  } else {
    mapPartsState.selectedPartId = null;
  }

  renderMapPartsGallery();
  renderMapPartsPreview();
  updateMapPartsSummary();
}
/* ---- マップパーツ配置画面 ---- */

function populateMapPartsPlaceMapOptions() {
  if (!mapPartsPlaceMapSelect) {
    return;
  }
  mapPartsPlaceMapSelect.innerHTML = "";
  const maps = mapObjectState.maps;
  if (!maps.length) {
    const opt = document.createElement("option");
    opt.value = "";
    opt.textContent = "（マップなし）";
    mapPartsPlaceMapSelect.appendChild(opt);
    return;
  }
  maps.forEach((map) => {
    const opt = document.createElement("option");
    opt.value = String(map.id);
    opt.textContent = map.name || `マップ ${map.id}`;
    mapPartsPlaceMapSelect.appendChild(opt);
  });
  if (mapPartsPlaceState.selectedMapId && maps.some((m) => m.id === mapPartsPlaceState.selectedMapId)) {
    mapPartsPlaceMapSelect.value = String(mapPartsPlaceState.selectedMapId);
  } else if (maps.length) {
    mapPartsPlaceState.selectedMapId = maps[0].id;
    mapPartsPlaceMapSelect.value = String(maps[0].id);
  }
}

/**
 * map-parts-place ギャラリーでパーツIDを選択し、ラベル・aria・iframe への通知を一括更新する。
 * @param {number} partsId
 * @param {string} [name]
 */
function selectMapPartsPlaceItem(partsId, name) {
  mapPartsPlaceState.selectedPartsId = partsId;
  if (mapPartsPlaceSelectedLabel) {
    const hexId = `0x${partsId.toString(16).toUpperCase().padStart(4, "0")}`;
    const displayName = name || `パーツ ${partsId}`;
    mapPartsPlaceSelectedLabel.textContent = `選択中: ${displayName} (ID: ${hexId})`;
  }
  if (mapPartsPlaceGallery) {
    mapPartsPlaceGallery.querySelectorAll(".map-parts-entry").forEach((el) => {
      el.classList.toggle("is-selected", el.dataset.partId === String(partsId));
      el.setAttribute("aria-pressed", el.dataset.partId === String(partsId) ? "true" : "false");
    });
  }
  // ゲーム iframe へ選択中パーツIDを通知
  if (adminGameFrame && adminGameFrame.contentWindow) {
    adminGameFrame.contentWindow.postMessage({ kind: "sbop2_set_admin_parts_id", partsId: partsId }, "*");
  }
}

function renderMapPartsPlaceGallery(partsList) {
  if (!mapPartsPlaceGallery) {
    return;
  }
  mapPartsPlaceGallery.innerHTML = "";
  if (!partsList || !partsList.length) {
    const msg = document.createElement("div");
    msg.className = "map-parts-gallery-message";
    msg.textContent = "表示できるマップパーツがありません";
    mapPartsPlaceGallery.appendChild(msg);
    return;
  }
  const tileSize = mapPartsState.tileSize || 16;
  const previewScale = 2;
  const previewTileSize = tileSize * previewScale;

  partsList.slice().sort((a, b) => a.partsId - b.partsId).forEach((part) => {
    const item = document.createElement("button");
    item.type = "button";
    item.className = "map-parts-entry";
    item.dataset.partId = String(part.partsId);
    item.setAttribute("role", "listitem");
    item.setAttribute("aria-label", `パーツID ${part.partsId}`);
    if (part.partsId === mapPartsPlaceState.selectedPartsId) {
      item.classList.add("is-selected");
    }
    item.setAttribute("aria-pressed", part.partsId === mapPartsPlaceState.selectedPartsId ? "true" : "false");

    const thumb = document.createElement("div");
    thumb.className = "map-parts-thumb";
    const baseLayer = document.createElement("div");
    baseLayer.className = "map-parts-tile";
    const overlayLayer = document.createElement("div");
    overlayLayer.className = "map-parts-tile overlay";
    thumb.appendChild(baseLayer);
    thumb.appendChild(overlayLayer);

    thumb.style.width = "100%";
    thumb.style.height = "100%";
    baseLayer.dataset.previewScale = String(previewScale);
    baseLayer.dataset.tilePixelSize = String(previewTileSize);
    overlayLayer.dataset.previewScale = String(previewScale);
    overlayLayer.dataset.tilePixelSize = String(previewTileSize);

    updateSpriteLayer(baseLayer, part.sprites.base);
    updateSpriteLayer(overlayLayer, part.sprites.overlay);

    item.appendChild(thumb);

    item.addEventListener("click", () => {
      selectMapPartsPlaceItem(part.partsId, part.name);
    });

    mapPartsPlaceGallery.appendChild(item);
  });
}

async function initializeMapPartsPlaceView() {
  // マップ一覧は mapObjectState を流用（未ロードなら読み込む）
  if (!mapObjectState.maps.length && !mapObjectState.isLoading && !mapObjectState.loadError) {
    await loadMapObjectData(false);
  }
  populateMapPartsPlaceMapOptions();

  // パーツ一覧は mapPartsState を流用（未ロードなら読み込む）
  if (!mapPartsState.parts.length && !mapPartsState.isLoading && !mapPartsState.loadError) {
    await loadMapPartsData(false);
  }
  renderMapPartsPlaceGallery(mapPartsState.parts);
}

/* ---- マップパーツ配置画面 ここまで ---- */

function renderMapPartsGallery() {
  if (!mapPartsGallery) {
    return;
  }
  mapPartsGallery.innerHTML = "";

  const appendMessage = (text) => {
    const message = document.createElement("div");
    message.className = "map-parts-gallery-message";
    message.textContent = text;
    mapPartsGallery.appendChild(message);
  };

  if (mapPartsState.isLoading) {
    appendMessage("マップパーツ情報を読み込み中です...");
    return;
  }
  if (mapPartsState.loadError) {
    appendMessage(mapPartsState.loadError);
    return;
  }
  if (!mapPartsState.filtered.length) {
    appendMessage(mapPartsState.parts.length ? "条件に一致するパーツがありません" : "表示できるマップパーツがありません");
    return;
  }

  mapPartsState.filtered
    .slice()
    .sort((a, b) => a.partsId - b.partsId)
    .forEach((part) => {
      const item = document.createElement("button");
      item.type = "button";
      item.className = "map-parts-entry";
      item.dataset.partId = String(part.partsId);
      item.setAttribute("role", "listitem");
      item.setAttribute("aria-label", `パーツID ${part.partsId}`);
      if (part.partsId === mapPartsState.selectedPartId) {
        item.classList.add("is-selected");
      }
      item.setAttribute("aria-pressed", part.partsId === mapPartsState.selectedPartId ? "true" : "false");

    const thumb = document.createElement("div");
    thumb.className = "map-parts-thumb";
    const baseLayer = document.createElement("div");
    baseLayer.className = "map-parts-tile";
    const overlayLayer = document.createElement("div");
    overlayLayer.className = "map-parts-tile overlay";
    thumb.appendChild(baseLayer);
    thumb.appendChild(overlayLayer);

    const tileSize = mapPartsState.tileSize || 16;
    const previewScale = MAP_PARTS_GALLERY_SCALE;
    const previewTileSize = tileSize * previewScale;
    thumb.style.width = "100%";
    thumb.style.height = "100%";
    baseLayer.dataset.previewScale = String(previewScale);
    baseLayer.dataset.tilePixelSize = String(previewTileSize);
    overlayLayer.dataset.previewScale = String(previewScale);
    overlayLayer.dataset.tilePixelSize = String(previewTileSize);

    updateSpriteLayer(baseLayer, part.sprites.base);
    updateSpriteLayer(overlayLayer, part.sprites.overlay);

    item.appendChild(thumb);

      item.addEventListener("click", () => {
        selectMapPart(part.partsId);
      });

      mapPartsGallery.appendChild(item);
    });
}

function selectMapPart(partId) {
  mapPartsState.selectedPartId = partId;
  renderMapPartsGallery();
  if (mapPartsGallery) {
    const target = mapPartsGallery.querySelector(`[data-part-id="${String(partId)}"]`);
    if (target && typeof target.focus === "function") {
      target.focus({ preventScroll: false });
    }
  }
  renderMapPartsPreview();
  populateMapPartsEditForm(partId);
}

function populateMapPartsEditForm(partId) {
  if (!mapPartsEditForm) {
    return;
  }
  const part = mapPartsState.parts.find((p) => p.partsId === partId) || null;
  if (!part) {
    mapPartsEditForm.style.display = "none";
    return;
  }
  mapPartsEditForm.style.display = "";

  if (mapPartsEditViewType) {
    mapPartsEditViewType.value = part.viewType !== null ? String(part.viewType) : "0";
  }
  if (mapPartsEditAnimeType) {
    mapPartsEditAnimeType.value = part.animeType !== null ? String(part.animeType) : "0";
  }
  if (mapPartsEditLevel) {
    mapPartsEditLevel.value = part.level !== null ? String(part.level) : "0";
    if (mapPartsEditLevelValue) {
      mapPartsEditLevelValue.textContent = mapPartsEditLevel.value;
    }
  }
  if (mapPartsEditMoveDirection) {
    mapPartsEditMoveDirection.value = part.movementDirection || "";
  }
  if (mapPartsEditPosX) {
    mapPartsEditPosX.value = part.viewPosition.x;
  }
  if (mapPartsEditPosY) {
    mapPartsEditPosY.value = part.viewPosition.y;
  }
  if (mapPartsEditGrpBase) {
    const base = part.sprites.base;
    mapPartsEditGrpBase.value = base ? String(base.sheet * 1024 + base.tile) : "0";
  }
  if (mapPartsEditGrpPile) {
    const overlay = part.sprites.overlay;
    mapPartsEditGrpPile.value = overlay ? String(overlay.sheet * 1024 + overlay.tile) : "0";
  }

  if (mapPartsEditFlagBlock) { mapPartsEditFlagBlock.checked = part.flags.block; }
  if (mapPartsEditFlagPile) { mapPartsEditFlagPile.checked = part.flags.pile; }
  if (mapPartsEditFlagPileBack) { mapPartsEditFlagPileBack.checked = part.flags.pileBack; }
  if (mapPartsEditFlagFishing) { mapPartsEditFlagFishing.checked = part.flags.fishing; }
  if (mapPartsEditFlagDrawLast) { mapPartsEditFlagDrawLast.checked = part.flags.drawLast; }
  if (mapPartsEditFlagCounter) { mapPartsEditFlagCounter.checked = part.flags.counter; }

  if (mapPartsEditBlockUp) { mapPartsEditBlockUp.checked = part.flags.blockDirections.up; }
  if (mapPartsEditBlockDown) { mapPartsEditBlockDown.checked = part.flags.blockDirections.down; }
  if (mapPartsEditBlockLeft) { mapPartsEditBlockLeft.checked = part.flags.blockDirections.left; }
  if (mapPartsEditBlockRight) { mapPartsEditBlockRight.checked = part.flags.blockDirections.right; }
}

function collectMapPartsEditData() {
  return {
    viewType: mapPartsEditViewType ? Number(mapPartsEditViewType.value) : 0,
    animeType: mapPartsEditAnimeType ? Number(mapPartsEditAnimeType.value) : 0,
    level: mapPartsEditLevel ? Number(mapPartsEditLevel.value) : 0,
    moveDirection: mapPartsEditMoveDirection ? mapPartsEditMoveDirection.value : "",
    viewPositionX: mapPartsEditPosX ? Number(mapPartsEditPosX.value) : 0,
    viewPositionY: mapPartsEditPosY ? Number(mapPartsEditPosY.value) : 0,
    grpIdBase: mapPartsEditGrpBase ? Number(mapPartsEditGrpBase.value) : 0,
    grpIdPile: mapPartsEditGrpPile ? Number(mapPartsEditGrpPile.value) : 0,
    block: mapPartsEditFlagBlock ? mapPartsEditFlagBlock.checked : false,
    pile: mapPartsEditFlagPile ? mapPartsEditFlagPile.checked : false,
    pileBack: mapPartsEditFlagPileBack ? mapPartsEditFlagPileBack.checked : false,
    fishing: mapPartsEditFlagFishing ? mapPartsEditFlagFishing.checked : false,
    drawLast: mapPartsEditFlagDrawLast ? mapPartsEditFlagDrawLast.checked : false,
    counter: mapPartsEditFlagCounter ? mapPartsEditFlagCounter.checked : false,
    blockUp: mapPartsEditBlockUp ? mapPartsEditBlockUp.checked : false,
    blockDown: mapPartsEditBlockDown ? mapPartsEditBlockDown.checked : false,
    blockLeft: mapPartsEditBlockLeft ? mapPartsEditBlockLeft.checked : false,
    blockRight: mapPartsEditBlockRight ? mapPartsEditBlockRight.checked : false
  };
}

async function handleMapPartsSave(event) {
  event.preventDefault();
  if (!mapPartsState.selectedPartId) {
    setMapPartsFeedback("保存するパーツを選択してください", "error");
    return;
  }
  const data = collectMapPartsEditData();
  data.partsId = mapPartsState.selectedPartId;

  setMapPartsFeedback("保存中...", null);
  try {
    const { response, data: result } = await fetchJson("/api/maps/parts", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data)
    });
    if (!response.ok) {
      const errMsg = result && result.error ? result.error : `HTTP ${response.status}`;
      setMapPartsFeedback(`保存に失敗しました: ${errMsg}`, "error");
      return;
    }
    setMapPartsFeedback("保存しました", "success");
    await loadMapPartsData(true);
    if (mapPartsState.selectedPartId) {
      populateMapPartsEditForm(mapPartsState.selectedPartId);
    }
  } catch (error) {
    setMapPartsFeedback("保存中にエラーが発生しました", "error");
  }
}

async function handleMapPartsCreate() {
  const data = collectMapPartsEditData();

  setMapPartsFeedback("追加中...", null);
  try {
    const { response, data: result } = await fetchJson("/api/maps/parts", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data)
    });
    if (!response.ok) {
      const errMsg = result && result.error ? result.error : `HTTP ${response.status}`;
      setMapPartsFeedback(`追加に失敗しました: ${errMsg}`, "error");
      return;
    }
    const newId = result && result.partsId ? result.partsId : null;
    setMapPartsFeedback(newId ? `パーツ ${newId} を追加しました` : "パーツを追加しました", "success");
    await loadMapPartsData(true);
    if (newId) {
      selectMapPart(newId);
    }
  } catch (error) {
    setMapPartsFeedback("追加中にエラーが発生しました", "error");
  }
}

async function handleMapPartsDelete() {
  if (!mapPartsState.selectedPartId) {
    setMapPartsFeedback("削除するパーツを選択してください", "error");
    return;
  }
  if (!confirm(`パーツ ${mapPartsState.selectedPartId} を削除しますか？\nマップ上の使用箇所もクリアされます。`)) {
    return;
  }

  setMapPartsFeedback("削除中...", null);
  try {
    const { response, data: result } = await fetchJson("/api/maps/parts", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ partsId: mapPartsState.selectedPartId })
    });
    if (!response.ok) {
      const errMsg = result && result.error ? result.error : `HTTP ${response.status}`;
      setMapPartsFeedback(`削除に失敗しました: ${errMsg}`, "error");
      return;
    }
    setMapPartsFeedback("パーツを削除しました", "success");
    mapPartsState.selectedPartId = null;
    await loadMapPartsData(true);
  } catch (error) {
    setMapPartsFeedback("削除中にエラーが発生しました", "error");
  }
}

/**
 * パーツカタログ（mapPartsState.parts）のみを確保する。
 * map-parts 画面の DOM が存在しない状況でも呼べる軽量版。
 * すでに読み込み済みの場合は何もしない（二重 fetch 回避）。
 */
async function ensureMapPartsCatalog() {
  if (mapPartsState.isLoading) {
    // 別タブでロード中の場合は完了まで少し待つ
    await new Promise((resolve) => {
      const interval = setInterval(() => {
        if (!mapPartsState.isLoading) {
          clearInterval(interval);
          resolve();
        }
      }, 50);
    });
    return;
  }
  if (mapPartsState.parts.length && !mapPartsState.loadError) {
    return; // すでに読み込み済み
  }

  mapPartsState.isLoading = true;
  mapPartsState.loadError = null;
  try {
    const { response, data } = await fetchJson("/api/maps/parts");
    if (!response.ok || !data || !Array.isArray(data.parts)) {
      throw new Error("invalid_response");
    }
    mapPartsState.tileSize = Number(data.tileSize) || mapPartsState.tileSize;
    mapPartsState.sheetTileWidth = Number(data.sheetTileWidth) || mapPartsState.sheetTileWidth;
    mapPartsState.sheetTileHeight = Number(data.sheetTileHeight) || mapPartsState.sheetTileHeight;
    if (typeof data.sheetBaseUrl === "string" && data.sheetBaseUrl.length) {
      mapPartsState.sheetBaseUrl = data.sheetBaseUrl;
    }
    mapPartsState.sheetCount = Number(data.sheetCount) || 0;
    mapPartsState.sheets = Array.isArray(data.sheets)
      ? data.sheets.slice().sort((a, b) => a - b)
      : [];
    mapPartsState.parts = data.parts
      .map((item) => normalizeMapPart(item))
      .filter(Boolean)
      .sort((a, b) => a.partsId - b.partsId);
    mapPartsState.filtered = mapPartsState.parts.slice();
  } catch (error) {
    console.error("map-window: パーツカタログの取得に失敗しました", error);
    mapPartsState.parts = [];
    mapPartsState.filtered = [];
    mapPartsState.loadError = "マップパーツの取得に失敗しました";
  } finally {
    mapPartsState.isLoading = false;
  }
}

async function loadMapPartsData(forceReload = false) {
  if (!mapPartsGallery) {
    return;
  }
  if (mapPartsState.isLoading) {
    return;
  }
  if (!forceReload && mapPartsState.parts.length && !mapPartsState.loadError) {
    return;
  }

  mapPartsState.isLoading = true;
  mapPartsState.loadError = null;
  setMapPartsSummary("マップパーツ情報を読み込み中です...");
  setMapPartsFeedback("マップパーツ情報を読み込み中です...", null);
  renderMapPartsGallery();

  try {
    const { response, data } = await fetchJson("/api/maps/parts");
    if (!response.ok || !data || !Array.isArray(data.parts)) {
      throw new Error("invalid_response");
    }

    mapPartsState.tileSize = Number(data.tileSize) || mapPartsState.tileSize;
    mapPartsState.sheetTileWidth = Number(data.sheetTileWidth) || mapPartsState.sheetTileWidth;
    mapPartsState.sheetTileHeight = Number(data.sheetTileHeight) || mapPartsState.sheetTileHeight;
    if (typeof data.sheetBaseUrl === "string" && data.sheetBaseUrl.length) {
      mapPartsState.sheetBaseUrl = data.sheetBaseUrl;
    }
    mapPartsState.sheetCount = Number(data.sheetCount) || 0;
    mapPartsState.sheets = Array.isArray(data.sheets)
      ? data.sheets.slice().sort((a, b) => a - b)
      : [];

    mapPartsState.parts = data.parts
      .map((item) => normalizeMapPart(item))
      .filter(Boolean)
      .sort((a, b) => a.partsId - b.partsId);

    mapPartsState.isLoading = false;
    applyMapPartsFilters();
    if (mapPartsState.parts.length) {
      setMapPartsFeedback(`パーツ ${mapPartsState.parts.length} 件を読み込みました`, "success");
    } else {
      setMapPartsFeedback("表示できるマップパーツがありません", "error");
    }
  } catch (error) {
    console.error("Failed to load map parts", error);
    mapPartsState.parts = [];
    mapPartsState.filtered = [];
    mapPartsState.selectedPartId = null;
    mapPartsState.loadError = "マップパーツの取得に失敗しました";
    mapPartsState.isLoading = false;
    setMapPartsSummary(mapPartsState.loadError);
    setMapPartsFeedback(mapPartsState.loadError, "error");
    renderMapPartsGallery();
    renderMapPartsPreview();
  } finally {
    updateMapPartsSummary();
  }
}

function initializeMapPartsView() {
  if (!mapPartsGallery) {
    return;
  }
  updateMapPartsSummary();
  renderMapPartsGallery();
  renderMapPartsPreview();
}

function handleMapPartsFlagFilterChange(event) {
  const value = event && event.target ? event.target.value : "all";
  mapPartsState.flagFilter = value;
  applyMapPartsFilters();
}

/* =====================================================
 * マップ情報編集画面 (map-info)
 * GET /api/maps でマップ一覧を取得し、選択したマップの
 * 基本情報（名前・BGM・天候・戦闘設定・暗さ）を編集する。
 * ===================================================== */

/** マップ情報のサマリー文字列を更新する */
function setMapInfoSummary(message) {
  if (mapInfoSummaryEl) {
    mapInfoSummaryEl.textContent = message || "";
  }
}

/** マップ情報フィードバックを表示する */
function setMapInfoFeedback(message, type) {
  if (!mapInfoFeedbackEl) {
    return;
  }
  mapInfoFeedbackEl.textContent = message || "";
  mapInfoFeedbackEl.className = "form-feedback";
  if (type === "success") {
    mapInfoFeedbackEl.classList.add("form-feedback--success");
  } else if (type === "error") {
    mapInfoFeedbackEl.classList.add("form-feedback--error");
  }
}

/** マップ選択 select を再描画する */
function renderMapInfoSelect() {
  if (!mapInfoSelect) {
    return;
  }
  const prevId = mapInfoState.selectedMapId;
  mapInfoSelect.innerHTML = "";

  if (!mapInfoState.maps.length) {
    const opt = document.createElement("option");
    opt.value = "";
    opt.textContent = "（マップなし）";
    mapInfoSelect.appendChild(opt);
    return;
  }

  mapInfoState.maps.forEach((map) => {
    const opt = document.createElement("option");
    opt.value = String(map.id);
    opt.textContent = `[${map.id}] ${map.name || "（名前なし）"}`;
    if (map.id === prevId) {
      opt.selected = true;
    }
    mapInfoSelect.appendChild(opt);
  });

  // 先頭を選択状態にする（前回選択が消えた場合）
  if (!mapInfoState.maps.some((m) => m.id === mapInfoState.selectedMapId)) {
    mapInfoState.selectedMapId = mapInfoState.maps[0].id;
    mapInfoSelect.value = String(mapInfoState.selectedMapId);
  }
}

/** 選択中マップのフォームを更新する */
function renderMapInfoForm() {
  if (!mapInfoEditArea) {
    return;
  }

  const map = mapInfoState.maps.find((m) => m.id === mapInfoState.selectedMapId);
  if (!map) {
    mapInfoEditArea.style.display = "none";
    return;
  }

  mapInfoEditArea.style.display = "";

  if (mapInfoNameInput) { mapInfoNameInput.value = map.name || ""; }
  if (mapInfoBgmInput) { mapInfoBgmInput.value = typeof map.bgmId === "number" ? map.bgmId : 0; }
  if (mapInfoWeatherSelect) { mapInfoWeatherSelect.value = String(typeof map.weatherType === "number" ? map.weatherType : 0); }
  if (mapInfoDarknessInput) { mapInfoDarknessInput.value = typeof map.darknessLevel === "number" ? map.darknessLevel : 0; }
  if (mapInfoBattleCheck) { mapInfoBattleCheck.checked = !!map.battleEnabled; }
  if (mapInfoRecoveryCheck) { mapInfoRecoveryCheck.checked = !!map.recoveryEnabled; }
}

/** マップ情報一覧を API から取得する */
async function loadMapInfoData(forceReload = false) {
  if (mapInfoState.isLoading) {
    return;
  }
  if (!forceReload && mapInfoState.maps.length && !mapInfoState.loadError) {
    renderMapInfoSelect();
    renderMapInfoForm();
    return;
  }

  mapInfoState.isLoading = true;
  mapInfoState.loadError = null;
  setMapInfoSummary("マップ情報を読み込み中...");
  setMapInfoFeedback("", null);

  try {
    const { response, data } = await fetchJson("/api/maps");
    if (!response.ok || !data || !Array.isArray(data.maps)) {
      throw new Error("invalid_response");
    }

    mapInfoState.maps = data.maps.slice().sort((a, b) => a.id - b.id);
    mapInfoState.isLoading = false;

    setMapInfoSummary(`マップ ${mapInfoState.maps.length} 件`);
    renderMapInfoSelect();
    renderMapInfoForm();
  } catch (err) {
    console.error("Failed to load map info", err);
    mapInfoState.maps = [];
    mapInfoState.loadError = "マップ情報の取得に失敗しました";
    mapInfoState.isLoading = false;
    setMapInfoSummary(mapInfoState.loadError);
    setMapInfoFeedback(mapInfoState.loadError, "error");
    renderMapInfoSelect();
    renderMapInfoForm();
  }
}

/** マップ情報を PUT で保存する */
async function saveMapInfo(event) {
  if (event) {
    event.preventDefault();
  }

  const map = mapInfoState.maps.find((m) => m.id === mapInfoState.selectedMapId);
  if (!map) {
    setMapInfoFeedback("保存対象のマップが選択されていません", "error");
    return;
  }

  const payload = { id: map.id };

  if (mapInfoNameInput) { payload.name = mapInfoNameInput.value; }
  if (mapInfoBgmInput) { payload.bgmId = parseInt(mapInfoBgmInput.value, 10) || 0; }
  if (mapInfoWeatherSelect) { payload.weatherType = parseInt(mapInfoWeatherSelect.value, 10) || 0; }
  if (mapInfoDarknessInput) {
    const v = parseInt(mapInfoDarknessInput.value, 10);
    payload.darknessLevel = isNaN(v) ? 0 : Math.min(255, Math.max(0, v));
  }
  if (mapInfoBattleCheck) { payload.battleEnabled = mapInfoBattleCheck.checked; }
  if (mapInfoRecoveryCheck) { payload.recoveryEnabled = mapInfoRecoveryCheck.checked; }

  setMapInfoFeedback("保存中...", null);

  try {
    const { response, data } = await fetchJson("/api/maps", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });

    if (!response.ok || !data || !data.id) {
      const errMsg = (data && data.error) ? data.error : `HTTP ${response.status}`;
      throw new Error(errMsg);
    }

    // ローカルキャッシュを更新する
    const idx = mapInfoState.maps.findIndex((m) => m.id === data.id);
    if (idx >= 0) {
      mapInfoState.maps[idx] = data;
    }

    setMapInfoFeedback("保存しました", "success");
    renderMapInfoSelect();
    renderMapInfoForm();
  } catch (err) {
    console.error("Failed to save map info", err);
    setMapInfoFeedback(`保存に失敗しました: ${err.message}`, "error");
  }
}

/** マップ追加ダイアログのコピー元セレクトを更新する */
function refreshMapInfoCopySelect() {
  if (!mapInfoCopySelect) {
    return;
  }
  mapInfoCopySelect.innerHTML = "";
  const emptyOpt = document.createElement("option");
  emptyOpt.value = "0";
  emptyOpt.textContent = "（空マップを新規作成）";
  mapInfoCopySelect.appendChild(emptyOpt);
  mapInfoState.maps.forEach((map) => {
    const opt = document.createElement("option");
    opt.value = String(map.id);
    opt.textContent = `[${map.id}] ${map.name || "（名前なし）"}`;
    mapInfoCopySelect.appendChild(opt);
  });
}

/** POST /api/maps でマップを新規作成する */
async function createMapInfo() {
  const copyFromMapId = mapInfoCopySelect ? parseInt(mapInfoCopySelect.value, 10) || 0 : 0;
  const payload = { copyFromMapId };

  setMapInfoFeedback("作成中...", null);

  try {
    const { response, data } = await fetchJson("/api/maps", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });

    if (!response.ok || !data || !data.id) {
      const errMsg = (data && data.error) ? data.error : `HTTP ${response.status}`;
      throw new Error(errMsg);
    }

    // 一覧を再取得し、新規作成マップを選択状態にする
    mapInfoState.maps = [];
    mapInfoState.loadError = null;
    await loadMapInfoData(true);
    mapInfoState.selectedMapId = data.id;
    if (mapInfoSelect) {
      mapInfoSelect.value = String(data.id);
    }
    renderMapInfoForm();

    // 追加エリアを閉じる
    if (mapInfoAddArea) {
      mapInfoAddArea.style.display = "none";
    }

    setMapInfoFeedback(`マップ ID[${data.id}] を作成しました`, "success");
  } catch (err) {
    console.error("Failed to create map", err);
    setMapInfoFeedback(`作成に失敗しました: ${err.message}`, "error");
  }
}

/* =====================================================
 * マップウィンドウ画面 (map-window)
 * map-objects 画面の renderMapPreview() を土台に、
 * オブジェクトオーバーレイなしのパーツ描画と
 * セル選択・ハイライト機能を実装する。
 * ===================================================== */

/** マップウィンドウのサマリー文字列を更新する。 */
function updateMapWindowSummary() {
  if (!mapWindowSummaryEl) {
    return;
  }
  if (mapWindowState.isLoading) {
    mapWindowSummaryEl.textContent = "マップ情報を読み込み中...";
    return;
  }
  if (mapWindowState.loadError) {
    mapWindowSummaryEl.textContent = mapWindowState.loadError;
    return;
  }
  const map = mapWindowState.maps.find((m) => m.id === mapWindowState.selectedMapId);
  if (!map) {
    mapWindowSummaryEl.textContent = mapWindowState.maps.length ? "マップを選択してください" : "マップデータなし";
    return;
  }
  mapWindowSummaryEl.textContent = `${map.name}  (${map.width} × ${map.height})`;
}

/** マップウィンドウ用マップ選択 <select> を再描画する。 */
function populateMapWindowOptions() {
  if (!mapWindowMapSelect) {
    return;
  }
  mapWindowMapSelect.innerHTML = "";
  if (!mapWindowState.maps.length) {
    const opt = document.createElement("option");
    opt.value = "";
    opt.textContent = mapWindowState.isLoading ? "読み込み中..." : "マップデータなし";
    mapWindowMapSelect.appendChild(opt);
    mapWindowMapSelect.disabled = true;
    return;
  }
  mapWindowState.maps.forEach((map) => {
    const opt = document.createElement("option");
    opt.value = map.id;
    opt.textContent = map.id ? `${map.name} (ID: ${map.id})` : map.name;
    mapWindowMapSelect.appendChild(opt);
  });
  mapWindowMapSelect.disabled = mapWindowState.isLoading;
  if (mapWindowState.selectedMapId) {
    mapWindowMapSelect.value = mapWindowState.selectedMapId;
  }
}

/**
 * 選択中セル情報テキストを更新する。
 * selectedCell が null の場合は「なし」を表示する。
 */
function updateMapWindowSelectedInfo() {
  if (!mapWindowSelectedInfoEl) {
    return;
  }
  if (!mapWindowState.selectedCell) {
    mapWindowSelectedInfoEl.textContent = "選択中のセル: なし";
    return;
  }
  const { x, y } = mapWindowState.selectedCell;
  mapWindowSelectedInfoEl.textContent = `選択中のセル: X ${x}、Y ${y}`;
}

/**
 * マップグリッドを描画する。
 * map-objects の renderMapPreview() を参考に、
 * オブジェクトオーバーレイなし・セル選択ハイライトのみを実装。
 */
function renderMapWindowGrid() {
  if (!mapWindowGridEl) {
    return;
  }
  mapWindowGridEl.innerHTML = "";

  const showMessage = (message) => {
    const empty = document.createElement("div");
    empty.className = "empty-message";
    empty.textContent = message;
    mapWindowGridEl.style.gridTemplateColumns = "repeat(1, minmax(80px, 1fr))";
    mapWindowGridEl.appendChild(empty);
  };

  if (mapWindowState.isLoading) {
    showMessage("マップ情報を読み込み中です");
    return;
  }
  if (mapWindowState.loadError) {
    showMessage(mapWindowState.loadError);
    return;
  }

  const map = mapWindowState.maps.find((m) => m.id === mapWindowState.selectedMapId);
  if (!map) {
    showMessage(mapWindowState.maps.length ? "マップが選択されていません" : "表示できるマップがありません");
    return;
  }

  const hasTiles = Array.isArray(map.tiles) && map.tiles.length > 0;
  const tileSize = mapWindowState.tileSize || 16;
  const cellPx = tileSize * 2;
  // sheetTileWidth/Height は grpId → tileX/tileY 変換（tile % width, tile / width）に使用する
  const sheetTileWidth = mapWindowState.sheetTileWidth || 32;
  const sel = mapWindowState.selectedCell;

  if (hasTiles) {
    mapWindowGridEl.style.gridTemplateColumns = `repeat(${Math.max(map.width, 1)}, ${cellPx}px)`;
    mapWindowGridEl.classList.add("has-tiles");
  } else {
    mapWindowGridEl.style.gridTemplateColumns = `repeat(${Math.max(map.width, 1)}, minmax(28px, 1fr))`;
    mapWindowGridEl.classList.remove("has-tiles");
  }

  for (let y = 0; y < map.height; y += 1) {
    for (let x = 0; x < map.width; x += 1) {
      const cellButton = document.createElement("button");
      cellButton.type = "button";
      cellButton.className = "map-object-cell";
      cellButton.dataset.x = x.toString();
      cellButton.dataset.y = y.toString();
      cellButton.setAttribute("role", "gridcell");
      cellButton.setAttribute("aria-label", `X: ${x}, Y: ${y}`);

      if (hasTiles) {
        const tileIndex = y * map.width + x;
        cellButton.style.width = `${cellPx}px`;
        cellButton.style.height = `${cellPx}px`;

        // grpId → sprite オブジェクト変換ヘルパー（0 は null = 空セル扱い）
        const grpIdToSprite = (grpId) => {
          if (!grpId) return null;
          const tile = grpId % 1024;
          return {
            sheet: Math.floor(grpId / 1024),
            tile,
            tileX: tile % sheetTileWidth,
            tileY: Math.floor(tile / sheetTileWidth),
          };
        };

        // 4 層描画: DOM 順が z-index 順になるので CSS 変更不要。
        //   層 1: base        (m_pwMap → m_wGrpIDBase)
        //   層 2: base の pile (m_pwMap → m_wGrpIDPile)
        //   層 3: 独立重ねの base  (m_pwMapPile → m_wGrpIDBase)
        //   層 4: 独立重ねの pile  (m_pwMapPile → m_wGrpIDPile)
        const layerDefs = [
          // --- 層 1: base 層 (m_wGrpIDBase) ---
          // grpId が 0 の場合は空セル扱い（pile 側と同じく null を渡す）。
          { grpId: Number(map.tilesBase[tileIndex]) || 0, className: "map-parts-tile" },
          // --- 層 2: base パーツの内蔵 pile 絵 (m_wGrpIDPile) ---
          // パーツカタログ逆引きは使わず、セルが直接持つ pile grpId をそのまま描画する。
          { grpId: Number(map.tilesPile[tileIndex]) || 0, className: "map-parts-tile overlay" },
          // --- 層 3: 独立重ねレイヤの base 絵 (m_pwMapPile → m_wGrpIDBase) ---
          // m_pwMapPile が持つパーツのベース絵。0 の場合は描画なし。
          { grpId: Number(map.tilesMapPileBase[tileIndex]) || 0, className: "map-parts-tile overlay" },
          // --- 層 4: 独立重ねレイヤの pile 絵 (m_pwMapPile → m_wGrpIDPile) ---
          // m_pwMapPile が持つパーツの内蔵重ね絵。0 の場合は描画なし。
          { grpId: Number(map.tilesMapPilePile[tileIndex]) || 0, className: "map-parts-tile overlay" },
        ];

        for (const { grpId, className } of layerDefs) {
          const sprite = grpIdToSprite(grpId);
          const layerDiv = document.createElement("div");
          layerDiv.className = className;
          layerDiv.dataset.tilePixelSize = String(cellPx);
          updateSpriteLayer(layerDiv, sprite);
          if (sprite) {
            layerDiv.style.backgroundRepeat = "no-repeat";
          }
          cellButton.appendChild(layerDiv);
        }
      }

      // 選択中セルのハイライト
      if (sel && sel.x === x && sel.y === y) {
        cellButton.classList.add("is-selected");
      }

      cellButton.addEventListener("click", () => {
        handleMapWindowCellClick(x, y);
      });

      mapWindowGridEl.appendChild(cellButton);
    }
  }
}

/** 現在選択中の pick ターゲット種別値を返す（未取得時は "cell"）。 */
function getMapWindowPickTarget() {
  const checked = document.querySelector('input[name="map-window-pick-target"]:checked');
  return checked ? checked.value : "cell";
}

/** pick ターゲット値を REST API 用 type 文字列へ変換する。 */
const PICK_TARGET_TYPE_MAP = {
  cell: "map_cell",
  placement: "placement",
  event: "event",
  character: "char"
};

/**
 * グリッドのセル選択ハイライトのみ更新する（再描画なし）。
 */
function refreshMapWindowCellHighlight() {
  if (!mapWindowGridEl) return;
  mapWindowGridEl.querySelectorAll(".map-object-cell").forEach((btn) => {
    const bx = Number(btn.dataset.x);
    const by = Number(btn.dataset.y);
    const isNowSelected =
      mapWindowState.selectedCell &&
      mapWindowState.selectedCell.x === bx &&
      mapWindowState.selectedCell.y === by;
    btn.classList.toggle("is-selected", isNowSelected);
  });
}

/** セルクリック時の処理。pick REST と連動して選択状態を更新する。 */
async function handleMapWindowCellClick(x, y) {
  const pickTarget = getMapWindowPickTarget();

  // cell 以外は未対応
  if (pickTarget !== "cell") {
    if (mapWindowSummaryEl) {
      mapWindowSummaryEl.textContent = "このピックタイプは未対応です（現在 cell のみ対応）";
    }
    return;
  }

  const mapId = mapWindowState.selectedMapId;
  const prev = mapWindowState.selectedCell;
  const isSameCell = prev && prev.x === x && prev.y === y;

  try {
    if (isSameCell) {
      // 同じセル再クリック → 選択解除
      const { response } = await fetchJson("/api/selection", { method: "DELETE" });
      if (!response.ok) {
        throw new Error(`DELETE /api/selection: ${response.status}`);
      }
      mapWindowState.selectedCell = null;
    } else {
      // 別セルクリック → 選択更新
      const { response } = await fetchJson("/api/selection/pick", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ type: "map_cell", mapId, x, y })
      });
      if (!response.ok) {
        throw new Error(`POST /api/selection/pick: ${response.status}`);
      }
      mapWindowState.selectedCell = { x, y };
    }
  } catch (error) {
    console.warn("[map-window] 選択の同期に失敗しました", error);
    if (mapWindowSummaryEl) {
      mapWindowSummaryEl.textContent = "選択の同期に失敗しました";
    }
    return;
  }

  refreshMapWindowCellHighlight();
  updateMapWindowSelectedInfo();
}

/**
 * WS の selection_changed ペイロードをマップウィンドウ画面に即時反映する。
 * payload が null（選択解除）、または type が "map_cell" 以外の場合は selectedCell をクリアする。
 * payload の mapId が現在表示中マップと異なる場合は無視する。
 * @param {object|null} payload - Selection JSON ({type, mapId, x, y, ...}) または null
 */
function applySelectionPayload(payload) {
  // マップウィンドウが表示されていない場合は何もしない
  if (!mapWindowState.selectedMapId) return;

  if (
    payload &&
    payload.type === "map_cell" &&
    String(payload.mapId) === String(mapWindowState.selectedMapId) &&
    typeof payload.x === "number" &&
    typeof payload.y === "number"
  ) {
    // 現在表示中マップのセル選択を更新
    mapWindowState.selectedCell = { x: payload.x, y: payload.y };
  } else if (!payload) {
    // 選択解除
    mapWindowState.selectedCell = null;
  } else {
    // 別マップへの選択 or 非対応 type → 無視
    return;
  }

  refreshMapWindowCellHighlight();
  updateMapWindowSelectedInfo();
}

/**
 * GET /api/selection で現在の選択状態を取得し、
 * type が "map_cell" かつ現在表示中マップと一致する場合のみ selectedCell をセットする。
 * それ以外の場合は selectedCell を変更しない。
 */
async function syncMapWindowSelectionFromServer() {
  if (!mapWindowState.selectedMapId) return;
  try {
    const { response, data } = await fetchJson("/api/selection");
    if (!response.ok) {
      throw new Error(`GET /api/selection: ${response.status}`);
    }
    if (
      data &&
      data.type === "map_cell" &&
      String(data.mapId) === String(mapWindowState.selectedMapId) &&
      typeof data.x === "number" &&
      typeof data.y === "number"
    ) {
      mapWindowState.selectedCell = { x: data.x, y: data.y };
    }
    // 他マップや他種別は変更しない
  } catch (error) {
    console.warn("[map-window] 選択の同期に失敗しました", error);
    if (mapWindowSummaryEl) {
      mapWindowSummaryEl.textContent = "選択の同期に失敗しました";
    }
  }
}

/** マップウィンドウ用マップ選択 change ハンドラ。 */
async function handleMapWindowSelectChange(event) {
  const newId = event.target.value;
  mapWindowState.selectedMapId = newId || null;
  mapWindowState.selectedCell = null;
  updateMapWindowSummary();
  updateMapWindowSelectedInfo();
  renderMapWindowGrid();
  // マップ変更後に選択状態を復元
  if (mapWindowState.selectedMapId) {
    await syncMapWindowSelectionFromServer();
    refreshMapWindowCellHighlight();
    updateMapWindowSelectedInfo();
  }
}

/**
 * マップウィンドウ用データロード。
 * /api/maps/objects エンドポイントを map-objects 画面と共用する。
 * 既に mapObjectState にデータがある場合はそちらを流用して二重 fetch を避ける。
 */
async function loadMapWindowData(forceReload = false) {
  if (!mapWindowMapSelect) {
    return;
  }
  if (mapWindowState.isLoading) {
    return;
  }

  // map-objects 画面がすでにデータを持っている場合は流用する
  if (!forceReload && mapObjectState.maps.length && !mapObjectState.loadError) {
    mapWindowState.maps = mapObjectState.maps;
    mapWindowState.tileSize = mapObjectState.tileSize;
    mapWindowState.sheetTileWidth = mapObjectState.sheetTileWidth;
    mapWindowState.sheetTileHeight = mapObjectState.sheetTileHeight;
    mapWindowState.sheetBaseUrl = mapObjectState.sheetBaseUrl;
    mapWindowState.loadError = null;
    if (!mapWindowState.selectedMapId || !mapWindowState.maps.some((m) => m.id === mapWindowState.selectedMapId)) {
      mapWindowState.selectedMapId = mapWindowState.maps.length ? mapWindowState.maps[0].id : null;
    }
    populateMapWindowOptions();
    updateMapWindowSummary();
    // overlay 描画に必要なパーツカタログを確保してからグリッドを描画する
    await ensureMapPartsCatalog();
    // サーバーの選択状態を復元してからグリッドを描画する
    await syncMapWindowSelectionFromServer();
    renderMapWindowGrid();
    updateMapWindowSelectedInfo();
    return;
  }

  // 独自にフェッチする
  mapWindowState.isLoading = true;
  mapWindowState.loadError = null;
  updateMapWindowSummary();
  renderMapWindowGrid();

  try {
    const { response, data } = await fetchJson("/api/maps/objects");
    if (!response.ok || !data || !Array.isArray(data.maps)) {
      throw new Error("invalid_response");
    }

    mapWindowState.tileSize = Number(data.tileSize) || mapWindowState.tileSize;
    mapWindowState.sheetTileWidth = Number(data.sheetTileWidth) || mapWindowState.sheetTileWidth;
    mapWindowState.sheetTileHeight = Number(data.sheetTileHeight) || mapWindowState.sheetTileHeight;
    if (typeof data.sheetBaseUrl === "string" && data.sheetBaseUrl.length) {
      mapWindowState.sheetBaseUrl = data.sheetBaseUrl;
    }

    mapWindowState.maps = data.maps.map((item) => normalizeMapEntry(item));

    if (mapWindowState.maps.length) {
      if (!mapWindowState.selectedMapId || !mapWindowState.maps.some((m) => m.id === mapWindowState.selectedMapId)) {
        mapWindowState.selectedMapId = mapWindowState.maps[0].id;
      }
    } else {
      mapWindowState.selectedMapId = null;
    }
    mapWindowState.selectedCell = null;
  } catch (error) {
    console.error("map-window: マップデータの取得に失敗しました", error);
    mapWindowState.maps = [];
    mapWindowState.selectedMapId = null;
    mapWindowState.selectedCell = null;
    mapWindowState.loadError = "マップデータの取得に失敗しました";
  } finally {
    mapWindowState.isLoading = false;
    populateMapWindowOptions();
    updateMapWindowSummary();
    // overlay 描画に必要なパーツカタログを確保してからグリッドを描画する
    await ensureMapPartsCatalog();
    // サーバーの選択状態を復元してからグリッドを描画する
    if (!mapWindowState.loadError) {
      await syncMapWindowSelectionFromServer();
    }
    updateMapWindowSelectedInfo();
    renderMapWindowGrid();
  }
}

/** map-window ビューが初めて表示されたときの初期化処理。 */
function initializeMapWindowView() {
  if (!mapWindowMapSelect) {
    return;
  }
  if (mapWindowState.isInitialized) {
    return;
  }
  mapWindowState.isInitialized = true;
  populateMapWindowOptions();
  updateMapWindowSummary();
  updateMapWindowSelectedInfo();
  renderMapWindowGrid();
  loadMapWindowData(false);
}

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

  if (normalized === "server-dashboard") {
    const shouldReload = options.initial || options.forceReload || currentRoute !== "server-dashboard";
    if (shouldReload) {
      loadServerInfo(true);
    }
    startServerPolling();
  } else {
    stopServerPolling();
  }

  if (normalized === "map-info") {
    if (options.forceReload) {
      loadMapInfoData(true);
    } else if (!mapInfoState.maps.length && !mapInfoState.isLoading && !mapInfoState.loadError) {
      loadMapInfoData(false);
    }
  }

  if (normalized === "map-window") {
    if (options.forceReload) {
      loadMapWindowData(true);
    } else if (!mapWindowState.maps.length && !mapWindowState.isLoading && !mapWindowState.loadError) {
      initializeMapWindowView();
    }
  }

  if (normalized === "map-objects") {
    if (options.forceReload) {
      loadMapObjectData(true);
    } else if (!mapObjectState.maps.length && !mapObjectState.isLoading && !mapObjectState.loadError) {
      loadMapObjectData(false);
    }
  }

  if (normalized === "map-object-templates") {
    if (options.forceReload || (!mapObjectTemplateState.templates.length && !mapObjectTemplateState.isLoading)) {
      loadObjectTemplates();
    }
  }

  if (normalized === "map-events") {
    if (options.forceReload || (!mapEventState.maps.length && !mapEventState.isLoading)) {
      initMapEventView();
    }
  }

  if (normalized === "talk-events") {
    if (options.forceReload || (!talkEventState.loaded && !talkEventState.isLoading)) {
      loadTalkEventList();
    }
  }

  if (normalized === "map-parts") {
    if (options.forceReload) {
      loadMapPartsData(true);
    } else if (!mapPartsState.parts.length && !mapPartsState.isLoading && !mapPartsState.loadError) {
      loadMapPartsData(false);
    }
  }

  if (normalized === "map-parts-place") {
    initializeMapPartsPlaceView();
  }

  if (normalized === "map-shadows") {
    if (options.forceReload || (!mapShadowState.shadows.length && !mapShadowState.isLoading && !mapShadowState.loadError)) {
      loadMapShadowList();
    }
  }

  if (normalized === "character-list") {
    if (options.forceReload) {
      charListState.offset = 0;
      loadCharacterList();
    } else if (!charListState.total && !charListState.isLoading) {
      loadCharacterList();
    }
  }

  if (normalized === "item-types") {
    if (options.forceReload || (!itemTypeState.items.length && !itemTypeState.isLoading && !itemTypeState.loadError)) {
      loadItemTypeList(options.forceReload === true);
    }
  }

  if (normalized === "item-list") {
    if (options.forceReload || (!itemState.items.length && !itemState.isLoading && !itemState.loadError)) {
      loadItemList(options.forceReload === true);
    }
  }

  if (normalized === "weapon-list") {
    if (options.forceReload || (!weaponState.items.length && !weaponState.isLoading && !weaponState.loadError)) {
      loadWeaponList(options.forceReload === true);
    }
  }

  if (normalized === "effect-balloon") {
    if (options.forceReload || (!balloonState.items.length && !balloonState.isLoading && !balloonState.loadError)) {
      loadBalloonList(options.forceReload === true);
    }
  }

  if (normalized === "effect-library") {
    if (options.forceReload || (!effectState.items.length && !effectState.isLoading && !effectState.loadError)) {
      loadEffectList(options.forceReload === true);
    }
  }

  if (normalized === "operation-history") {
    if (options.forceReload || !auditLogState.loaded) {
      loadAuditLogList();
    }
  }

  // ゲーム iframe へ Web管理モードを通知
  if (adminGameFrame && adminGameFrame.contentWindow) {
    let adminMode = 0;
    if (normalized === "character-overview") {
      adminMode = 1;
    } else if (normalized === "map-parts-place") {
      adminMode = 2;
    }
    adminGameFrame.contentWindow.postMessage({ kind: "sbop2_set_admin_mode", mode: adminMode }, "*");
  }

  currentRoute = normalized;
}

// ============================================================
// 監査ログ（操作履歴）表示
// ============================================================
// サーバー側の GET /api/audit-logs を叩いてテーブル表示する。
// 記録はメモリ上のリングバッファ（最大 500 件）。
// 詳細展開は行クリックで requestBody 要約を展開表示。
// TODO: before/after の diff 表示、CSV エクスポート、SQLite 永続化は未対応。
const auditLogState = {
  entries: [],
  loaded: false,
  isLoading: false,
  loadError: null,
  expandedId: null
};

function escapeHtml(text) {
  if (text == null) return "";
  return String(text)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#39;");
}

function buildAuditLogQuery() {
  const actor  = (document.getElementById("auditLogFilterActor") || {}).value || "";
  const method = (document.getElementById("auditLogFilterMethod") || {}).value || "";
  const path   = (document.getElementById("auditLogFilterPath") || {}).value || "";
  const limit  = (document.getElementById("auditLogFilterLimit") || {}).value || "100";
  const params = new URLSearchParams();
  if (actor.trim())  params.set("actor", actor.trim());
  if (method)        params.set("method", method);
  if (path.trim())   params.set("path", path.trim());
  if (limit)         params.set("limit", limit);
  const qs = params.toString();
  return qs ? `/api/audit-logs?${qs}` : "/api/audit-logs";
}

async function loadAuditLogList() {
  const bodyEl    = document.getElementById("auditLogListBody");
  const summaryEl = document.getElementById("auditLogSummary");
  if (!bodyEl) return;
  if (auditLogState.isLoading) return;
  auditLogState.isLoading = true;
  auditLogState.loadError = null;
  bodyEl.innerHTML = '<tr><td colspan="7" class="empty-row">読み込み中...</td></tr>';
  if (summaryEl) summaryEl.textContent = "";

  try {
    const { response, data } = await fetchJson(buildAuditLogQuery());
    if (!response.ok || !data) {
      throw new Error((data && data.error) || `HTTP ${response.status}`);
    }
    auditLogState.entries = Array.isArray(data.entries) ? data.entries : [];
    auditLogState.loaded = true;
    if (summaryEl) {
      summaryEl.textContent =
        `取得 ${data.count ?? auditLogState.entries.length} 件 / 全 ${data.total ?? "?"} 件（容量 ${data.capacity ?? "?"}）`;
    }
    renderAuditLogList();
  } catch (err) {
    auditLogState.loadError = err.message || String(err);
    bodyEl.innerHTML = `<tr><td colspan="7" class="empty-row">取得エラー: ${escapeHtml(auditLogState.loadError)}</td></tr>`;
  } finally {
    auditLogState.isLoading = false;
  }
}

function renderAuditLogList() {
  const bodyEl = document.getElementById("auditLogListBody");
  if (!bodyEl) return;
  const entries = auditLogState.entries;
  if (!entries.length) {
    bodyEl.innerHTML = '<tr><td colspan="7" class="empty-row">該当する履歴はありません。</td></tr>';
    return;
  }
  const rows = [];
  for (const e of entries) {
    const isExpanded = auditLogState.expandedId === e.id;
    rows.push(
      `<tr class="audit-log-row" data-audit-id="${e.id}">` +
        `<td>${e.id}</td>` +
        `<td>${escapeHtml(e.timestamp)}</td>` +
        `<td>${escapeHtml(e.actor)}${e.adminLevel ? ` <span class="badge">Lv${e.adminLevel}</span>` : ""}</td>` +
        `<td><span class="method method-${escapeHtml(e.method)}">${escapeHtml(e.method)}</span></td>` +
        `<td class="audit-log-path">${escapeHtml(e.path)}</td>` +
        `<td>${e.statusCode}</td>` +
        `<td class="audit-log-body-cell">${escapeHtml((e.bodyDigest || "").slice(0, 80))}${(e.bodyDigest || "").length > 80 ? "…" : ""}</td>` +
      `</tr>`
    );
    if (isExpanded) {
      // 整形表示: JSON として parse できればインデント、できなければ生文字列
      let pretty = e.bodyDigest || "(本文なし)";
      try {
        if (e.bodyDigest) {
          const parsed = JSON.parse(e.bodyDigest);
          pretty = JSON.stringify(parsed, null, 2);
        }
      } catch (_) { /* 非 JSON はそのまま */ }
      rows.push(
        `<tr class="audit-log-detail-row"><td colspan="7">` +
          `<div class="audit-log-detail">` +
            `<div><strong>accountId:</strong> ${escapeHtml(e.accountId || "-")}</div>` +
            `<div><strong>bodySize:</strong> ${e.bodySize} bytes${e.bodySize > (e.bodyDigest || "").length ? "（要約のため末尾省略）" : ""}</div>` +
            `<pre class="audit-log-body-pre">${escapeHtml(pretty)}</pre>` +
          `</div>` +
        `</td></tr>`
      );
    }
  }
  bodyEl.innerHTML = rows.join("");
}

function initAuditLogView() {
  const reloadBtn = document.getElementById("auditLogReloadBtn");
  const clearBtn  = document.getElementById("auditLogClearBtn");
  const bodyEl    = document.getElementById("auditLogListBody");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", () => loadAuditLogList());
  }
  if (clearBtn) {
    clearBtn.addEventListener("click", () => {
      const ids = ["auditLogFilterActor", "auditLogFilterMethod", "auditLogFilterPath"];
      for (const id of ids) {
        const el = document.getElementById(id);
        if (el) el.value = "";
      }
      const limitEl = document.getElementById("auditLogFilterLimit");
      if (limitEl) limitEl.value = "100";
      loadAuditLogList();
    });
  }
  if (bodyEl) {
    bodyEl.addEventListener("click", (ev) => {
      const tr = ev.target.closest("tr.audit-log-row");
      if (!tr) return;
      const id = Number(tr.dataset.auditId);
      if (!Number.isFinite(id)) return;
      auditLogState.expandedId = (auditLogState.expandedId === id) ? null : id;
      renderAuditLogList();
    });
  }
}

function initializeAdminWorkspace() {
  if (adminWorkspaceInitialized) {
    return;
  }
  adminWorkspaceInitialized = true;
  ensureAdminWebSocket();
  const initialRoute = window.location.hash ? window.location.hash.replace(/^#/, "") : DEFAULT_ROUTE;
  activateRoute(initialRoute, { initial: true, forceReload: true });
  loadRoles();
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

  loadRoles();
  initAuditLogView();

  /* マップウィンドウ */
  if (mapWindowMapSelect) {
    mapWindowMapSelect.addEventListener("change", handleMapWindowSelectChange);
    initializeMapWindowView();
  }

  /* マップパーツ配置 */
  if (mapPartsPlaceMapSelect) {
    mapPartsPlaceMapSelect.addEventListener("change", () => {
      const id = parseInt(mapPartsPlaceMapSelect.value, 10);
      if (Number.isFinite(id)) {
        mapPartsPlaceState.selectedMapId = id;
      }
    });
  }
  if (mapPartsPlaceEditMasterBtn) {
    mapPartsPlaceEditMasterBtn.addEventListener("click", () => {
      navigateTo("map-parts");
    });
  }

  if (mapObjectMapSelect) {
    initializeMapObjectView();
    mapObjectMapSelect.addEventListener("change", handleMapSelectChange);
  }
  if (mapObjectFilterSelect) {
    mapObjectFilterSelect.addEventListener("change", handleMapFilterChange);
  }
  if (mapObjectForm) {
    mapObjectForm.addEventListener("submit", handleMapObjectFormSubmit);
  }
  if (mapObjectClearButton) {
    mapObjectClearButton.addEventListener("click", handleMapObjectClear);
  }
  if (mapObjectDeleteButton) {
    mapObjectDeleteButton.addEventListener("click", handleMapObjectDelete);
  }

  /* テンプレート管理 */
  if (mapObjectTemplateForm) {
    applyTemplateToForm(null);
    mapObjectTemplateForm.addEventListener("submit", handleTemplateSave);
  }
  if (mapObjectTemplateNewBtn) {
    mapObjectTemplateNewBtn.addEventListener("click", handleTemplateNew);
  }
  if (mapObjectTemplateDeleteBtn) {
    mapObjectTemplateDeleteBtn.addEventListener("click", handleTemplateDelete);
  }

  if (reloadButton) {
    reloadButton.addEventListener("click", () => {
      loadServerInfo(true);
    });
  }
  if (accountForm) {
    accountForm.addEventListener("submit", handleAccountSubmit);
  }
  if (accountResetButton) {
    accountResetButton.addEventListener("click", handleAccountReset);
  }
  if (roleForm) {
    roleForm.addEventListener("submit", handleRoleSubmit);
  }
  if (roleResetButton) {
    roleResetButton.addEventListener("click", handleRoleReset);
  }

  /* マップ情報編集イベント登録 */
  if (mapInfoSelect) {
    mapInfoSelect.addEventListener("change", function () {
      const id = parseInt(mapInfoSelect.value, 10);
      if (!isNaN(id)) {
        mapInfoState.selectedMapId = id;
        renderMapInfoForm();
        setMapInfoFeedback("", null);
      }
    });
  }
  if (mapInfoEditForm) {
    mapInfoEditForm.addEventListener("submit", saveMapInfo);
  }
  if (mapInfoReloadBtn) {
    mapInfoReloadBtn.addEventListener("click", function () {
      loadMapInfoData(true);
    });
  }

  // マップ追加ボタン：追加エリアを表示してコピー元セレクトを更新する
  if (mapInfoAddBtn) {
    mapInfoAddBtn.addEventListener("click", function () {
      if (!mapInfoAddArea) {
        return;
      }
      const isVisible = mapInfoAddArea.style.display !== "none";
      if (isVisible) {
        mapInfoAddArea.style.display = "none";
      } else {
        refreshMapInfoCopySelect();
        mapInfoAddArea.style.display = "";
      }
    });
  }
  if (mapInfoAddConfirmBtn) {
    mapInfoAddConfirmBtn.addEventListener("click", createMapInfo);
  }
  if (mapInfoAddCancelBtn) {
    mapInfoAddCancelBtn.addEventListener("click", function () {
      if (mapInfoAddArea) {
        mapInfoAddArea.style.display = "none";
      }
    });
  }

  if (mapPartsGallery) {
    initializeMapPartsView();
    if (mapPartsFlagFilter) {
      mapPartsFlagFilter.addEventListener("change", handleMapPartsFlagFilterChange);
    }
    // パーツID 指定入力（picker と連動）: 値が変わったら該当パーツへジャンプ
    const mapPartsJumpInput = document.getElementById("map-parts-jump-id");
    if (mapPartsJumpInput) {
      mapPartsJumpInput.addEventListener("change", function () {
        const v = Number(mapPartsJumpInput.value);
        if (!Number.isFinite(v)) { return; }
        // ギャラリーに該当パーツが無ければフィルタを「すべて」に戻してから選択
        const exists = mapPartsState.parts.some(function (p) { return p.partsId === v; });
        if (!exists) { return; }
        const shown = mapPartsState.filtered.some(function (p) { return p.partsId === v; });
        if (!shown && mapPartsFlagFilter) {
          mapPartsFlagFilter.value = "all";
          mapPartsState.flagFilter = "all";
          applyMapPartsFilters();
        }
        selectMapPart(v);
      });
    }
  }

  if (mapPartsEditForm) {
    mapPartsEditForm.addEventListener("submit", handleMapPartsSave);
    if (mapPartsEditForm.style) {
      mapPartsEditForm.style.display = "none";
    }
  }
  if (mapPartsCreateBtn) {
    mapPartsCreateBtn.addEventListener("click", handleMapPartsCreate);
  }
  if (mapPartsDeleteBtn) {
    mapPartsDeleteBtn.addEventListener("click", handleMapPartsDelete);
  }
  if (mapPartsEditLevel) {
    mapPartsEditLevel.addEventListener("input", function () {
      if (mapPartsEditLevelValue) {
        mapPartsEditLevelValue.textContent = mapPartsEditLevel.value;
      }
    });
  }

  // マップイベント画面の初期化
  if (mapEventMapSelect) {
    mapEventMapSelect.addEventListener("change", function () {
      const id = parseInt(mapEventMapSelect.value, 10);
      if (!isNaN(id)) {
        mapEventState.selectedMapId = id;
        mapEventState.selectedEventId = null;
        loadMapEventList();
      }
    });
  }
  if (mapEventNewBtn) {
    mapEventNewBtn.addEventListener("click", function () {
      mapEventState.selectedEventId = null;
      renderMapEventForm(null);
    });
  }
  if (mapEventEditForm) {
    mapEventEditForm.addEventListener("submit", saveMapEvent);
  }
});

/* =====================================================
 * マップイベント編集画面 (map-events)
 * ===================================================== */

const MAP_EVENT_TYPE_LABELS = {
  0: "なし (NONE)",
  1: "マップ内移動 (MOVE)",
  2: "マップ間移動 (MAPMOVE)",
  3: "ゴミ箱 (TRASHBOX)",
  4: "ステータス初期化 (INITSTATUS)",
  5: "一時画像設定 (GRPIDTMP)",
  6: "灯り (LIGHT)"
};

const MAP_EVENT_HIT_TYPE_LABELS = {
  0: "MAPPOS（マップ座標）",
  1: "CHARPOS（キャラ座標）",
  2: "AREA（範囲）",
  3: "MAPPOS2（マップ座標完全一致）"
};

function setMapEventFeedback(message, type) {
  if (!mapEventFeedbackEl) { return; }
  mapEventFeedbackEl.textContent = message || "";
  mapEventFeedbackEl.className = "form-feedback" + (type ? " form-feedback--" + type : "");
}

function setMapEventSummary(message) {
  if (mapEventSummaryEl) {
    mapEventSummaryEl.textContent = message || "";
  }
}

/** マップ選択 select を描画する */
function renderMapEventMapSelect() {
  if (!mapEventMapSelect) { return; }
  const prevId = mapEventState.selectedMapId;
  mapEventMapSelect.innerHTML = "";

  if (!mapEventState.maps.length) {
    const opt = document.createElement("option");
    opt.value = "";
    opt.textContent = "（マップなし）";
    mapEventMapSelect.appendChild(opt);
    return;
  }

  mapEventState.maps.forEach(function (map) {
    const opt = document.createElement("option");
    opt.value = String(map.id);
    opt.textContent = "[" + map.id + "] " + (map.name || "（名前なし）");
    if (map.id === prevId) { opt.selected = true; }
    mapEventMapSelect.appendChild(opt);
  });

  if (!mapEventState.maps.some(function (m) { return m.id === mapEventState.selectedMapId; })) {
    mapEventState.selectedMapId = mapEventState.maps[0].id;
    mapEventMapSelect.value = String(mapEventState.selectedMapId);
  }
}

/** イベント一覧テーブルを描画する */
function renderMapEventTable() {
  if (!mapEventTableBody) { return; }
  mapEventTableBody.innerHTML = "";

  if (!mapEventState.events.length) {
    const tr = document.createElement("tr");
    const td = document.createElement("td");
    td.colSpan = 5;
    td.textContent = "イベントなし";
    td.style.textAlign = "center";
    tr.appendChild(td);
    mapEventTableBody.appendChild(tr);
    return;
  }

  mapEventState.events.forEach(function (ev) {
    const tr = document.createElement("tr");
    if (ev.id === mapEventState.selectedEventId) {
      tr.classList.add("selected-row");
    }

    function td(text) {
      const el = document.createElement("td");
      el.textContent = text;
      return el;
    }

    tr.appendChild(td(ev.id));
    tr.appendChild(td(ev.typeLabel || String(ev.type)));
    tr.appendChild(td("(" + ev.pos.x + ", " + ev.pos.y + ")"));
    tr.appendChild(td(ev.hitTypeLabel || String(ev.hitType)));

    const tdBtn = document.createElement("td");
    const btn = document.createElement("button");
    btn.type = "button";
    btn.className = "btn btn-secondary btn-sm";
    btn.textContent = "編集";
    btn.addEventListener("click", function () {
      mapEventState.selectedEventId = ev.id;
      renderMapEventTable();
      renderMapEventForm(ev);
    });
    tdBtn.appendChild(btn);
    tr.appendChild(tdBtn);

    mapEventTableBody.appendChild(tr);
  });
}

/** 種別依存フィールドの HTML を返す */
function buildDetailFieldsHtml(type, detail) {
  detail = detail || {};
  switch (Number(type)) {
  case 1: // MOVE
    return '<label class="form-field"><span>移動先 X</span><input type="number" name="detail.destX" value="' + (detail.destX || 0) + '"></label>' +
           '<label class="form-field"><span>移動先 Y</span><input type="number" name="detail.destY" value="' + (detail.destY || 0) + '"></label>' +
           '<label class="form-field"><span>向き</span><input type="number" name="detail.direction" value="' + (detail.direction || 0) + '"></label>';
  case 2: // MAPMOVE
    return '<label class="form-field"><span>移動先マップID</span><input type="number" name="detail.destMapId" value="' + (detail.destMapId || 0) + '"></label>' +
           '<label class="form-field"><span>移動先 X</span><input type="number" name="detail.destX" value="' + (detail.destX || 0) + '"></label>' +
           '<label class="form-field"><span>移動先 Y</span><input type="number" name="detail.destY" value="' + (detail.destY || 0) + '"></label>' +
           '<label class="form-field"><span>向き</span><input type="number" name="detail.direction" value="' + (detail.direction || 0) + '"></label>';
  case 3: // TRASHBOX
    return '<p class="field-note">固有フィールドなし</p>';
  case 4: // INITSTATUS
    return '<label class="form-field"><span>エフェクトID</span><input type="number" name="detail.effectId" value="' + (detail.effectId || 0) + '"></label>';
  case 5: // GRPIDTMP
    return '<label class="form-field"><span>設定種別 (0=解除 1=設定)</span><input type="number" name="detail.setType" value="' + (detail.setType || 0) + '"></label>' +
           '<label class="form-field"><span>画像IDメイン</span><input type="number" name="detail.idMain" value="' + (detail.idMain || 0) + '"></label>' +
           '<label class="form-field"><span>画像IDサブ</span><input type="number" name="detail.idSub" value="' + (detail.idSub || 0) + '"></label>';
  case 6: // LIGHT
    return '<label class="form-field"><span>灯りON</span><input type="checkbox" name="detail.lightOn"' + (detail.lightOn ? ' checked' : '') + '></label>' +
           '<label class="form-field"><span>持続時間</span><input type="number" name="detail.time" value="' + (detail.time || 0) + '"></label>';
  default:
    return '';
  }
}

/** 編集フォームを描画する (ev が null なら新規) */
function renderMapEventForm(ev) {
  if (!mapEventEditArea) { return; }
  mapEventEditArea.style.display = "";

  const isNew = (ev === null || ev === undefined);
  const eventId = isNew ? 0 : ev.id;
  const type    = isNew ? 1 : ev.type;
  const detail  = isNew ? {} : (ev.detail || {});

  // hitType select オプション生成
  let hitTypeOptions = "";
  Object.keys(MAP_EVENT_HIT_TYPE_LABELS).forEach(function (k) {
    const sel = (!isNew && ev.hitType === Number(k)) ? " selected" : "";
    hitTypeOptions += '<option value="' + k + '"' + sel + '>' + MAP_EVENT_HIT_TYPE_LABELS[k] + '</option>';
  });

  // type select オプション生成
  let typeOptions = "";
  Object.keys(MAP_EVENT_TYPE_LABELS).forEach(function (k) {
    if (k === "0") { return; } // NONE は選択肢から除く
    const sel = (Number(k) === type) ? " selected" : "";
    typeOptions += '<option value="' + k + '"' + sel + '>' + MAP_EVENT_TYPE_LABELS[k] + '</option>';
  });

  const pos  = isNew ? { x: 0, y: 0 } : ev.pos;
  const pos2 = isNew ? { x: 0, y: 0 } : ev.pos2;

  mapEventEditArea.innerHTML =
    '<form id="map-event-edit-form" class="edit-form">' +
    '<input type="hidden" name="id" value="' + eventId + '">' +
    '<h3>' + (isNew ? '新規イベント' : 'イベント ID: ' + eventId) + '</h3>' +
    '<label class="form-field"><span>種別</span>' +
      '<select name="type" id="map-event-type-select">' + typeOptions + '</select>' +
    '</label>' +
    '<label class="form-field"><span>効果音ID</span><input type="number" name="soundId" value="' + (isNew ? 0 : ev.soundId) + '"></label>' +
    '<label class="form-field"><span>当たり判定種別</span>' +
      '<select name="hitType">' + hitTypeOptions + '</select>' +
    '</label>' +
    '<label class="form-field"><span>判定向き</span><input type="number" name="hitDirection" value="' + (isNew ? 0 : ev.hitDirection) + '"></label>' +
    '<label class="form-field"><span>座標1 X</span><input type="number" name="posX" value="' + pos.x + '"></label>' +
    '<label class="form-field"><span>座標1 Y</span><input type="number" name="posY" value="' + pos.y + '"></label>' +
    '<label class="form-field"><span>座標2 X</span><input type="number" name="pos2X" value="' + pos2.x + '"></label>' +
    '<label class="form-field"><span>座標2 Y</span><input type="number" name="pos2Y" value="' + pos2.y + '"></label>' +
    '<div id="map-event-detail-fields">' + buildDetailFieldsHtml(type, detail) + '</div>' +
    '<div class="form-actions">' +
      '<button type="submit" class="btn btn-primary">保存</button>' +
      (!isNew ? '<button type="button" class="btn btn-danger" id="map-event-delete-btn">削除</button>' : '') +
      '<button type="button" class="btn btn-secondary" id="map-event-cancel-btn">キャンセル</button>' +
    '</div>' +
    '</form>' +
    '<p id="map-event-feedback" class="form-feedback" aria-live="polite"></p>';

  // 種別変更で detail フィールドを切り替える
  const typeSelect = document.getElementById("map-event-type-select");
  const detailArea = document.getElementById("map-event-detail-fields");
  if (typeSelect && detailArea) {
    typeSelect.addEventListener("change", function () {
      detailArea.innerHTML = buildDetailFieldsHtml(typeSelect.value, {});
    });
  }

  // フォーム submit
  const form = document.getElementById("map-event-edit-form");
  if (form) {
    form.addEventListener("submit", saveMapEvent);
  }

  // 削除ボタン
  const deleteBtn = document.getElementById("map-event-delete-btn");
  if (deleteBtn) {
    deleteBtn.addEventListener("click", function () {
      deleteMapEvent(eventId);
    });
  }

  // キャンセルボタン
  const cancelBtn = document.getElementById("map-event-cancel-btn");
  if (cancelBtn) {
    cancelBtn.addEventListener("click", function () {
      mapEventState.selectedEventId = null;
      mapEventEditArea.style.display = "none";
      mapEventEditArea.innerHTML = "";
    });
  }
}

/** フォームデータから保存用ペイロードを組み立てる */
function collectMapEventPayload(form) {
  const data = new FormData(form);
  const typeVal = parseInt(data.get("type") || "1", 10);
  const payload = {
    mapId:         mapEventState.selectedMapId,
    id:            parseInt(data.get("id") || "0", 10),
    type:          typeVal,
    soundId:       parseInt(data.get("soundId") || "0", 10),
    hitType:       parseInt(data.get("hitType") || "0", 10),
    hitDirection:  parseInt(data.get("hitDirection") || "0", 10),
    pos:  { x: parseInt(data.get("posX")  || "0", 10), y: parseInt(data.get("posY")  || "0", 10) },
    pos2: { x: parseInt(data.get("pos2X") || "0", 10), y: parseInt(data.get("pos2Y") || "0", 10) },
    detail: {}
  };

  switch (typeVal) {
  case 1: // MOVE
    payload.detail = {
      destX: parseInt(data.get("detail.destX") || "0", 10),
      destY: parseInt(data.get("detail.destY") || "0", 10),
      direction: parseInt(data.get("detail.direction") || "0", 10)
    };
    break;
  case 2: // MAPMOVE
    payload.detail = {
      destMapId: parseInt(data.get("detail.destMapId") || "0", 10),
      destX: parseInt(data.get("detail.destX") || "0", 10),
      destY: parseInt(data.get("detail.destY") || "0", 10),
      direction: parseInt(data.get("detail.direction") || "0", 10)
    };
    break;
  case 4: // INITSTATUS
    payload.detail = {
      effectId: parseInt(data.get("detail.effectId") || "0", 10)
    };
    break;
  case 5: // GRPIDTMP
    payload.detail = {
      setType: parseInt(data.get("detail.setType") || "0", 10),
      idMain:  parseInt(data.get("detail.idMain")  || "0", 10),
      idSub:   parseInt(data.get("detail.idSub")   || "0", 10)
    };
    break;
  case 6: // LIGHT
    payload.detail = {
      lightOn: form.querySelector('[name="detail.lightOn"]') ? form.querySelector('[name="detail.lightOn"]').checked : false,
      time:    parseInt(data.get("detail.time") || "0", 10)
    };
    break;
  default:
    break;
  }
  return payload;
}

/** イベントを保存する (POST または PUT) */
async function saveMapEvent(event) {
  if (event) { event.preventDefault(); }
  const form = event ? event.target : document.getElementById("map-event-edit-form");
  if (!form) { return; }

  const payload = collectMapEventPayload(form);
  const isNew   = (payload.id === 0);
  const method  = isNew ? "POST" : "PUT";

  const feedbackEl = document.getElementById("map-event-feedback") || mapEventFeedbackEl;
  if (feedbackEl) { feedbackEl.textContent = "保存中..."; feedbackEl.className = "form-feedback"; }

  try {
    const { response, data } = await fetchJson("/api/maps/events", {
      method: method,
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });
    if (!response.ok || !data || !data.id) {
      const msg = (data && data.error) ? data.error : "HTTP " + response.status;
      throw new Error(msg);
    }

    if (isNew) {
      mapEventState.events.push(data);
    } else {
      const idx = mapEventState.events.findIndex(function (e) { return e.id === data.id; });
      if (idx >= 0) { mapEventState.events[idx] = data; }
    }

    mapEventState.selectedEventId = data.id;
    setMapEventSummary("イベント " + mapEventState.events.length + " 件");
    if (feedbackEl) { feedbackEl.textContent = "保存しました"; feedbackEl.className = "form-feedback form-feedback--success"; }
    renderMapEventTable();
    renderMapEventForm(data);
  } catch (err) {
    console.error("Failed to save map event", err);
    if (feedbackEl) { feedbackEl.textContent = "保存に失敗しました: " + err.message; feedbackEl.className = "form-feedback form-feedback--error"; }
  }
}

/** イベントを削除する */
async function deleteMapEvent(eventId) {
  if (!mapEventState.selectedMapId || !eventId) { return; }

  const feedbackEl = document.getElementById("map-event-feedback") || mapEventFeedbackEl;
  if (feedbackEl) { feedbackEl.textContent = "削除中..."; feedbackEl.className = "form-feedback"; }

  try {
    const { response, data } = await fetchJson(
      "/api/maps/events?mapId=" + mapEventState.selectedMapId + "&id=" + eventId,
      { method: "DELETE" }
    );
    if (!response.ok) {
      const msg = (data && data.error) ? data.error : "HTTP " + response.status;
      throw new Error(msg);
    }

    mapEventState.events = mapEventState.events.filter(function (e) { return e.id !== eventId; });
    mapEventState.selectedEventId = null;
    setMapEventSummary("イベント " + mapEventState.events.length + " 件");
    renderMapEventTable();
    if (mapEventEditArea) {
      mapEventEditArea.style.display = "none";
      mapEventEditArea.innerHTML = "";
    }
  } catch (err) {
    console.error("Failed to delete map event", err);
    if (feedbackEl) { feedbackEl.textContent = "削除に失敗しました: " + err.message; feedbackEl.className = "form-feedback form-feedback--error"; }
  }
}

/** 選択中マップのイベント一覧を API から取得する */
async function loadMapEventList() {
  if (!mapEventState.selectedMapId) { return; }
  mapEventState.isLoading = true;
  mapEventState.loadError = null;
  setMapEventSummary("読み込み中...");

  try {
    const { response, data } = await fetchJson("/api/maps/events?mapId=" + mapEventState.selectedMapId);
    if (!response.ok || !data || !Array.isArray(data.events)) {
      throw new Error("invalid_response");
    }
    mapEventState.events = data.events;
    mapEventState.isLoading = false;
    setMapEventSummary("イベント " + mapEventState.events.length + " 件");
    renderMapEventTable();
    if (mapEventEditArea) {
      mapEventEditArea.style.display = "none";
      mapEventEditArea.innerHTML = "";
    }
  } catch (err) {
    console.error("Failed to load map events", err);
    mapEventState.events = [];
    mapEventState.isLoading = false;
    mapEventState.loadError = "イベント一覧の取得に失敗しました";
    setMapEventSummary(mapEventState.loadError);
    renderMapEventTable();
  }
}

/** マップイベント画面の初期化（マップ一覧を map-info API から流用して選択 select に入れる） */
async function initMapEventView() {
  if (!mapEventMapSelect) { return; }

  // mapInfoState にマップ一覧が既にあればそのまま流用する
  if (mapInfoState.maps.length) {
    mapEventState.maps = mapInfoState.maps.slice().sort(function (a, b) { return a.id - b.id; });
  } else {
    try {
      const { response, data } = await fetchJson("/api/maps");
      if (response.ok && data && Array.isArray(data.maps)) {
        mapEventState.maps = data.maps.slice().sort(function (a, b) { return a.id - b.id; });
      }
    } catch (err) {
      console.error("Failed to load maps for event view", err);
    }
  }

  renderMapEventMapSelect();

  if (mapEventState.maps.length && !mapEventState.selectedMapId) {
    mapEventState.selectedMapId = mapEventState.maps[0].id;
    mapEventMapSelect.value = String(mapEventState.selectedMapId);
  }

  if (mapEventState.selectedMapId) {
    loadMapEventList();
  }
}

// =============================================================================
// マップ影カタログ編集画面 (map-shadows)
// =============================================================================

const mapShadowNewBtn       = document.getElementById("map-shadow-new-btn");
const mapShadowTableBody    = document.getElementById("map-shadow-table-body");
const mapShadowSummaryEl    = document.getElementById("map-shadow-summary");
const mapShadowFeedbackEl   = document.getElementById("map-shadow-feedback");
const mapShadowDetailSection = document.getElementById("map-shadow-detail");
const mapShadowDetailTitle  = document.getElementById("map-shadow-detail-title");
const mapShadowEditForm     = document.getElementById("map-shadow-edit-form");
const mapShadowEditGrpId    = document.getElementById("map-shadow-edit-grp-id");
const mapShadowEditViewType = document.getElementById("map-shadow-edit-view-type");
const mapShadowEditAnimeType = document.getElementById("map-shadow-edit-anime-type");
const mapShadowEditAnimeCount = document.getElementById("map-shadow-edit-anime-count");
const mapShadowEditLevel    = document.getElementById("map-shadow-edit-level");
const mapShadowEditLevelValue = document.getElementById("map-shadow-edit-level-value");
const mapShadowEditLight    = document.getElementById("map-shadow-edit-light");
const mapShadowEditPosX     = document.getElementById("map-shadow-edit-pos-x");
const mapShadowEditPosY     = document.getElementById("map-shadow-edit-pos-y");
const mapShadowFramesList   = document.getElementById("map-shadow-anime-frames-list");
const mapShadowAddFrameBtn  = document.getElementById("map-shadow-add-frame-btn");
const mapShadowSaveBtn      = document.getElementById("map-shadow-save-btn");
const mapShadowCancelBtn    = document.getElementById("map-shadow-cancel-btn");
const mapShadowEditFeedback = document.getElementById("map-shadow-edit-feedback");

/** 影一覧をサーバーから取得して表示する */
async function loadMapShadowList() {
  if (!mapShadowTableBody) { return; }
  mapShadowState.isLoading = true;
  mapShadowState.loadError = null;
  if (mapShadowSummaryEl) { mapShadowSummaryEl.textContent = "読み込み中..."; }

  try {
    const { response, data } = await fetchJson("/api/maps/shadows");
    if (response.ok && data && Array.isArray(data.shadows)) {
      mapShadowState.shadows = data.shadows;
      renderMapShadowTable();
      if (mapShadowSummaryEl) {
        mapShadowSummaryEl.textContent = `${data.shadows.length} 件`;
      }
    } else {
      mapShadowState.loadError = "読み込みに失敗しました";
      if (mapShadowSummaryEl) { mapShadowSummaryEl.textContent = mapShadowState.loadError; }
    }
  } catch (err) {
    mapShadowState.loadError = "通信エラーが発生しました";
    if (mapShadowSummaryEl) { mapShadowSummaryEl.textContent = mapShadowState.loadError; }
  } finally {
    mapShadowState.isLoading = false;
  }
}

/** 影一覧テーブルを描画する */
function renderMapShadowTable() {
  if (!mapShadowTableBody) { return; }
  mapShadowTableBody.innerHTML = "";

  if (!mapShadowState.shadows.length) {
    const tr = document.createElement("tr");
    const td = document.createElement("td");
    td.colSpan = 8;
    td.textContent = "影データがありません";
    tr.appendChild(td);
    mapShadowTableBody.appendChild(tr);
    return;
  }

  mapShadowState.shadows.forEach(function (shadow) {
    const tr = document.createElement("tr");
    if (shadow.id === mapShadowState.selectedId) {
      tr.classList.add("is-selected");
    }

    [
      shadow.id,
      shadow.grpId,
      shadow.viewType,
      shadow.animeType,
      shadow.animeCount,
      shadow.level,
      shadow.light ? "明度" : "透明度"
    ].forEach(function (val) {
      const td = document.createElement("td");
      td.textContent = val;
      tr.appendChild(td);
    });

    const tdOp = document.createElement("td");
    const editBtn = document.createElement("button");
    editBtn.type = "button";
    editBtn.className = "button small";
    editBtn.textContent = "編集";
    editBtn.addEventListener("click", function () {
      openMapShadowEdit(shadow.id);
    });
    const delBtn = document.createElement("button");
    delBtn.type = "button";
    delBtn.className = "button small danger";
    delBtn.textContent = "削除";
    delBtn.addEventListener("click", function () {
      deleteMapShadow(shadow.id);
    });
    tdOp.appendChild(editBtn);
    tdOp.appendChild(delBtn);
    tr.appendChild(tdOp);

    mapShadowTableBody.appendChild(tr);
  });
}

/** アニメーションコマ入力行を 1 件生成して framesList に追加する */
function addMapShadowFrameRow(frame) {
  if (!mapShadowFramesList) { return; }
  const row = document.createElement("div");
  row.className = "anime-frame-row";

  const fields = [
    { label: "待ち(×10ms)", name: "wait",     value: frame ? frame.wait     : 0, min: 0, max: 255 },
    { label: "透明度",       name: "level",    value: frame ? frame.level    : 0, min: 0, max: 255 },
    { label: "下地GrpID",    name: "grpIdBase",value: frame ? frame.grpIdBase: 0, min: 0, max: 65535 },
    { label: "重ねGrpID",    name: "grpIdPile",value: frame ? frame.grpIdPile: 0, min: 0, max: 65535 }
  ];

  fields.forEach(function (f) {
    const label = document.createElement("label");
    label.className = "form-field compact";
    const span = document.createElement("span");
    span.textContent = f.label;
    const input = document.createElement("input");
    input.type = "number";
    input.dataset.frameField = f.name;
    input.value = f.value;
    input.min = f.min;
    input.max = f.max;
    label.appendChild(span);
    label.appendChild(input);
    row.appendChild(label);
  });

  const removeBtn = document.createElement("button");
  removeBtn.type = "button";
  removeBtn.className = "button small danger";
  removeBtn.textContent = "削除";
  removeBtn.addEventListener("click", function () {
    row.parentNode.removeChild(row);
  });
  row.appendChild(removeBtn);

  mapShadowFramesList.appendChild(row);
}

/** フォームにコマ一覧を設定する */
function populateMapShadowFrames(animeFrames) {
  if (!mapShadowFramesList) { return; }
  mapShadowFramesList.innerHTML = "";
  if (Array.isArray(animeFrames)) {
    animeFrames.forEach(function (frame) {
      addMapShadowFrameRow(frame);
    });
  }
}

/** フォームから animeFrames 配列を収集する */
function collectMapShadowFrames() {
  if (!mapShadowFramesList) { return []; }
  const frames = [];
  const rows = mapShadowFramesList.querySelectorAll(".anime-frame-row");
  rows.forEach(function (row) {
    const frame = {};
    row.querySelectorAll("[data-frame-field]").forEach(function (input) {
      frame[input.dataset.frameField] = Number(input.value) || 0;
    });
    frames.push(frame);
  });
  return frames;
}

/** 指定 ID の影を編集フォームに開く（null = 新規） */
function openMapShadowEdit(id) {
  if (!mapShadowDetailSection) { return; }
  mapShadowDetailSection.style.display = "";
  mapShadowState.selectedId = id;
  renderMapShadowTable();

  if (mapShadowEditFeedback) { mapShadowEditFeedback.textContent = ""; }

  if (id === null) {
    if (mapShadowDetailTitle) { mapShadowDetailTitle.textContent = "新規影を追加"; }
    if (mapShadowEditGrpId)   { mapShadowEditGrpId.value   = "0"; }
    if (mapShadowEditViewType){ mapShadowEditViewType.value = "0"; }
    if (mapShadowEditAnimeType){ mapShadowEditAnimeType.value = "0"; }
    if (mapShadowEditAnimeCount){ mapShadowEditAnimeCount.value = "0"; }
    if (mapShadowEditLevel)   { mapShadowEditLevel.value   = "0"; }
    if (mapShadowEditLevelValue){ mapShadowEditLevelValue.textContent = "0"; }
    if (mapShadowEditLight)   { mapShadowEditLight.checked  = false; }
    if (mapShadowEditPosX)    { mapShadowEditPosX.value    = "0"; }
    if (mapShadowEditPosY)    { mapShadowEditPosY.value    = "0"; }
    populateMapShadowFrames([]);
    return;
  }

  const shadow = mapShadowState.shadows.find(function (s) { return s.id === id; });
  if (!shadow) { return; }

  if (mapShadowDetailTitle) { mapShadowDetailTitle.textContent = `影ID ${shadow.id} を編集`; }
  if (mapShadowEditGrpId)    { mapShadowEditGrpId.value    = shadow.grpId; }
  if (mapShadowEditViewType) { mapShadowEditViewType.value  = shadow.viewType; }
  if (mapShadowEditAnimeType){ mapShadowEditAnimeType.value = shadow.animeType; }
  if (mapShadowEditAnimeCount){ mapShadowEditAnimeCount.value = shadow.animeCount; }
  if (mapShadowEditLevel)    { mapShadowEditLevel.value    = shadow.level; }
  if (mapShadowEditLevelValue){ mapShadowEditLevelValue.textContent = shadow.level; }
  if (mapShadowEditLight)    { mapShadowEditLight.checked  = !!shadow.light; }
  if (mapShadowEditPosX)     { mapShadowEditPosX.value     = shadow.viewPos ? shadow.viewPos.x : 0; }
  if (mapShadowEditPosY)     { mapShadowEditPosY.value     = shadow.viewPos ? shadow.viewPos.y : 0; }
  populateMapShadowFrames(shadow.animeFrames || []);
}

/** フォームから JSON ペイロードを組み立てる */
function buildMapShadowPayload(id) {
  return {
    id:         id || 0,
    grpId:      Number(mapShadowEditGrpId ? mapShadowEditGrpId.value : 0),
    viewType:   Number(mapShadowEditViewType ? mapShadowEditViewType.value : 0),
    animeType:  Number(mapShadowEditAnimeType ? mapShadowEditAnimeType.value : 0),
    animeCount: Number(mapShadowEditAnimeCount ? mapShadowEditAnimeCount.value : 0),
    level:      Number(mapShadowEditLevel ? mapShadowEditLevel.value : 0),
    light:      !!(mapShadowEditLight && mapShadowEditLight.checked),
    viewPos: {
      x: Number(mapShadowEditPosX ? mapShadowEditPosX.value : 0),
      y: Number(mapShadowEditPosY ? mapShadowEditPosY.value : 0)
    },
    animeFrames: collectMapShadowFrames()
  };
}

/** 影を保存（新規 or 更新）する */
async function saveMapShadow(evt) {
  if (evt) { evt.preventDefault(); }
  if (mapShadowEditFeedback) { mapShadowEditFeedback.textContent = ""; }

  const isNew = (mapShadowState.selectedId === null);
  const payload = buildMapShadowPayload(mapShadowState.selectedId);

  try {
    const { response, data } = await fetchJson(
      "/api/maps/shadows",
      { method: isNew ? "POST" : "PUT", body: JSON.stringify(payload) }
    );

    if (response.ok) {
      if (mapShadowEditFeedback) { mapShadowEditFeedback.textContent = isNew ? "追加しました" : "更新しました"; }
      if (mapShadowFeedbackEl)   { mapShadowFeedbackEl.textContent = ""; }
      await loadMapShadowList();
      if (data && data.id) {
        mapShadowState.selectedId = data.id;
        renderMapShadowTable();
      }
    } else {
      const msg = (data && data.error) ? data.error : "保存に失敗しました";
      if (mapShadowEditFeedback) { mapShadowEditFeedback.textContent = msg; }
    }
  } catch (err) {
    if (mapShadowEditFeedback) { mapShadowEditFeedback.textContent = "通信エラーが発生しました"; }
  }
}

/** 影を削除する */
async function deleteMapShadow(id) {
  if (!window.confirm(`影ID ${id} を削除しますか？`)) { return; }

  try {
    const { response, data } = await fetchJson(
      `/api/maps/shadows?id=${id}`,
      { method: "DELETE" }
    );

    if (response.ok) {
      if (mapShadowFeedbackEl) { mapShadowFeedbackEl.textContent = `影ID ${id} を削除しました`; }
      if (mapShadowState.selectedId === id) {
        mapShadowState.selectedId = null;
        if (mapShadowDetailSection) { mapShadowDetailSection.style.display = "none"; }
      }
      await loadMapShadowList();
    } else {
      const msg = (data && data.error) ? data.error : "削除に失敗しました";
      if (mapShadowFeedbackEl) { mapShadowFeedbackEl.textContent = msg; }
    }
  } catch (err) {
    if (mapShadowFeedbackEl) { mapShadowFeedbackEl.textContent = "通信エラーが発生しました"; }
  }
}

// イベントリスナー登録（要素が存在する場合のみ）
if (mapShadowNewBtn) {
  mapShadowNewBtn.addEventListener("click", function () {
    openMapShadowEdit(null);
  });
}

// 影ID 指定入力（picker と連動）: 値が変わったら該当影の編集画面を開く
const mapShadowJumpInput = document.getElementById("map-shadow-jump-id");
if (mapShadowJumpInput) {
  mapShadowJumpInput.addEventListener("change", function () {
    const v = Number(mapShadowJumpInput.value);
    if (!Number.isFinite(v)) { return; }
    const exists = mapShadowState.shadows.some(function (s) { return s.id === v; });
    if (!exists) { return; }
    openMapShadowEdit(v);
  });
}

if (mapShadowEditLevel) {
  mapShadowEditLevel.addEventListener("input", function () {
    if (mapShadowEditLevelValue) {
      mapShadowEditLevelValue.textContent = mapShadowEditLevel.value;
    }
  });
}

if (mapShadowAddFrameBtn) {
  mapShadowAddFrameBtn.addEventListener("click", function () {
    addMapShadowFrameRow(null);
  });
}

if (mapShadowEditForm) {
  mapShadowEditForm.addEventListener("submit", saveMapShadow);
}

if (mapShadowCancelBtn) {
  mapShadowCancelBtn.addEventListener("click", function () {
    mapShadowState.selectedId = null;
    if (mapShadowDetailSection) { mapShadowDetailSection.style.display = "none"; }
    renderMapShadowTable();
  });
}

/* =====================================================
 * キャラクター一覧画面 (character-list)
 * GET /api/characters?name=&accountId=&mapId=&isNpc=&limit=20&offset=N
 * レスポンス: { total, limit, offset, characters: [...] }
 * ===================================================== */
const charFilterName    = document.getElementById("char-filter-name");
const charFilterAccId   = document.getElementById("char-filter-account-id");
const charFilterMapId   = document.getElementById("char-filter-map-id");
const charFilterIsNpc   = document.getElementById("char-filter-is-npc");
const charSearchBtn     = document.getElementById("char-search-btn");
const charResetBtn      = document.getElementById("char-reset-btn");
const charListSummary   = document.getElementById("char-list-summary");
const charListFeedback  = document.getElementById("char-list-feedback");
const charListTableBody = document.getElementById("char-list-table-body");
const charPrevBtn       = document.getElementById("char-prev-btn");
const charNextBtn       = document.getElementById("char-next-btn");
const charPageInfo      = document.getElementById("char-page-info");

const CHAR_LIST_LIMIT = 20;

const charListState = {
  offset: 0,
  total: 0,
  isLoading: false,
};

/** クエリパラメータを組み立てて /api/characters を呼ぶ */
async function loadCharacterList() {
  if (!charListTableBody) { return; }
  if (charListState.isLoading) { return; }

  charListState.isLoading = true;
  if (charListFeedback) { charListFeedback.textContent = "読み込み中..."; }
  if (charListTableBody) { charListTableBody.innerHTML = '<tr><td colspan="7">読み込み中...</td></tr>'; }
  updateCharPagination();

  const params = new URLSearchParams();
  if (charFilterName && charFilterName.value.trim()) {
    params.set("name", charFilterName.value.trim());
  }
  if (charFilterAccId && charFilterAccId.value.trim()) {
    params.set("accountId", charFilterAccId.value.trim());
  }
  if (charFilterMapId && charFilterMapId.value.trim()) {
    params.set("mapId", charFilterMapId.value.trim());
  }
  if (charFilterIsNpc && charFilterIsNpc.value !== "") {
    params.set("isNpc", charFilterIsNpc.value);
  }
  params.set("limit", String(CHAR_LIST_LIMIT));
  params.set("offset", String(charListState.offset));

  try {
    const { response, data } = await fetchJson("/api/characters?" + params.toString());
    if (!response.ok || !data || !Array.isArray(data.characters)) {
      const msg = (data && data.error) ? data.error : "キャラクター一覧の取得に失敗しました";
      if (charListFeedback) { charListFeedback.textContent = msg; }
      charListTableBody.innerHTML = '<tr><td colspan="7">取得に失敗しました</td></tr>';
      charListState.total = 0;
      updateCharPagination();
      return;
    }

    charListState.total = typeof data.total === "number" ? data.total : 0;
    if (charListFeedback) { charListFeedback.textContent = ""; }
    if (charListSummary) {
      charListSummary.textContent = charListState.total > 0
        ? `${charListState.total} 件中 ${charListState.offset + 1}〜${Math.min(charListState.offset + data.characters.length, charListState.total)} 件を表示`
        : "該当するキャラクターがありません";
    }

    renderCharacterList(data.characters);
    updateCharPagination();
  } catch (err) {
    console.error("loadCharacterList error", err);
    if (charListFeedback) { charListFeedback.textContent = "通信エラーが発生しました"; }
    charListTableBody.innerHTML = '<tr><td colspan="7">通信エラーが発生しました</td></tr>';
    charListState.total = 0;
    updateCharPagination();
  } finally {
    charListState.isLoading = false;
  }
}

/** テーブル本体を描画する */
function renderCharacterList(characters) {
  if (!charListTableBody) { return; }
  if (!characters.length) {
    charListTableBody.innerHTML = '<tr><td colspan="7">データがありません</td></tr>';
    return;
  }

  const fragment = document.createDocumentFragment();
  characters.forEach(function (c) {
    const tr = document.createElement("tr");
    tr.style.cursor = "pointer";
    tr.innerHTML =
      "<td>" + escapeHtml(String(c.charId ?? "")) + "</td>" +
      "<td>" + escapeHtml(String(c.charName ?? "")) + "</td>" +
      "<td>" + escapeHtml(String(c.level ?? "")) + "</td>" +
      "<td>" + escapeHtml(String(c.mapId ?? "")) + "</td>" +
      "<td>" + escapeHtml(String(c.x ?? "")) + ", " + escapeHtml(String(c.y ?? "")) + "</td>" +
      "<td>" + escapeHtml(String(c.accountId ?? "")) + "</td>" +
      "<td>" + (c.isNpc ? "NPC" : "PC") + "</td>";
    tr.addEventListener("click", function () {
      // character-overview 画面へ遷移して詳細を取得する
      navigateTo("character-overview");
      fetchCharacterDetail(c.charId);
    });
    fragment.appendChild(tr);
  });
  charListTableBody.innerHTML = "";
  charListTableBody.appendChild(fragment);
}

/** ページングボタンの有効/無効を更新する */
function updateCharPagination() {
  const total  = charListState.total;
  const offset = charListState.offset;
  if (charPrevBtn) { charPrevBtn.disabled = offset <= 0; }
  if (charNextBtn) { charNextBtn.disabled = offset + CHAR_LIST_LIMIT >= total; }
  if (charPageInfo) {
    if (total > 0) {
      const page    = Math.floor(offset / CHAR_LIST_LIMIT) + 1;
      const maxPage = Math.ceil(total / CHAR_LIST_LIMIT);
      charPageInfo.textContent = page + " / " + maxPage + " ページ";
    } else {
      charPageInfo.textContent = "";
    }
  }
}

/** HTML エスケープ（キャラ一覧用）*/
function escapeHtml(str) {
  return String(str)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}

// 検索ボタン
if (charSearchBtn) {
  charSearchBtn.addEventListener("click", function () {
    charListState.offset = 0;
    loadCharacterList();
  });
}

// リセットボタン
if (charResetBtn) {
  charResetBtn.addEventListener("click", function () {
    if (charFilterName)  { charFilterName.value  = ""; }
    if (charFilterAccId) { charFilterAccId.value = ""; }
    if (charFilterMapId) { charFilterMapId.value = ""; }
    if (charFilterIsNpc) { charFilterIsNpc.value = ""; }
    charListState.offset = 0;
    charListState.total  = 0;
    if (charListTableBody) { charListTableBody.innerHTML = ""; }
    if (charListSummary)   { charListSummary.textContent  = ""; }
    if (charListFeedback)  { charListFeedback.textContent = ""; }
    updateCharPagination();
  });
}

// 前へボタン
if (charPrevBtn) {
  charPrevBtn.addEventListener("click", function () {
    if (charListState.offset <= 0) { return; }
    charListState.offset = Math.max(0, charListState.offset - CHAR_LIST_LIMIT);
    loadCharacterList();
  });
}

// 次へボタン
if (charNextBtn) {
  charNextBtn.addEventListener("click", function () {
    if (charListState.offset + CHAR_LIST_LIMIT >= charListState.total) { return; }
    charListState.offset += CHAR_LIST_LIMIT;
    loadCharacterList();
  });
}

/* =====================================================
 * キャラクター詳細・編集 (character-overview)
 * GET  /api/characters/{charId}
 * PUT  /api/characters/{charId}
 * PUT  /api/characters/{charId}/status
 * PUT  /api/characters/{charId}/equipment
 * PUT  /api/characters/{charId}/graphics
 * ===================================================== */

const charDetailIdInput  = document.getElementById("char-detail-id-input");
const charDetailLoadBtn  = document.getElementById("char-detail-load-btn");
const charDetailFeedback = document.getElementById("char-detail-feedback");
const charDetailBody     = document.getElementById("char-detail-body");

/** 現在表示中のキャラID */
var currentCharId = 0;

/** GET /api/characters/{charId} を呼んで詳細を取得する */
async function fetchCharacterDetail(charId) {
  if (!charDetailFeedback) { return; }
  charDetailFeedback.textContent = "読み込み中...";
  if (charDetailBody) { charDetailBody.style.display = "none"; }

  try {
    const { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(charId));
    if (!response.ok || !data) {
      const msg = (data && data.error) ? data.error : "キャラクター詳細の取得に失敗しました";
      charDetailFeedback.textContent = "エラー: " + msg;
      return;
    }
    charDetailFeedback.textContent = "";
    currentCharId = charId;
    renderCharacterDetail(data);
  } catch (err) {
    console.error("fetchCharacterDetail error", err);
    charDetailFeedback.textContent = "通信エラーが発生しました";
  }
}

/** フィードバックメッセージを設定する（成功/エラー兼用） */
function setFeedback(id, msg, isError) {
  var el = document.getElementById(id);
  if (!el) { return; }
  el.textContent = msg;
  el.style.color = isError ? "var(--color-danger, #f87171)" : "var(--color-success, #4ade80)";
}

/** input[type=number] の値を数値として取得する（空欄は null） */
function getNumVal(id) {
  var el = document.getElementById(id);
  if (!el) { return null; }
  var s = el.value.trim();
  if (s === "") { return null; }
  var n = parseInt(s, 10);
  return isNaN(n) ? null : n;
}

/** input[type=text] の値を取得する（空欄は null） */
function getStrVal(id) {
  var el = document.getElementById(id);
  if (!el) { return null; }
  var s = el.value;
  return s;
}

/** input[type=checkbox] の値を真偽値で取得する */
function getBoolVal(id) {
  var el = document.getElementById(id);
  if (!el) { return null; }
  return el.checked;
}

/** PUT リクエストを送る汎用関数 */
async function putJson(url, body) {
  const res = await fetch(url, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
    credentials: "same-origin"
  });
  var data = null;
  try { data = await res.json(); } catch (_) {}
  return { response: res, data: data };
}

/** レスポンスデータを詳細フォームに反映する */
function renderCharacterDetail(d) {
  function setText(id, val) {
    var el = document.getElementById(id);
    if (el) { el.textContent = (val === null || val === undefined) ? "-" : String(val); }
  }
  function setInput(id, val) {
    var el = document.getElementById(id);
    if (!el) { return; }
    if (el.type === "checkbox") {
      el.checked = !!val;
    } else {
      el.value = (val === null || val === undefined) ? "" : String(val);
    }
  }

  // 基本情報（表示 span）
  setText("cd-charId",    d.charId);
  setText("cd-accountId", d.accountId);
  setText("cd-moveState", d.moveState);
  setText("cd-isNpc",     d.isNpc ? "NPC" : "PC");

  // 基本情報（フォーム入力値）
  setInput("cdf-charName",    d.charName);
  setInput("cdf-mapId",       d.mapId);
  setInput("cdf-x",           d.x);
  setInput("cdf-y",           d.y);
  setInput("cdf-direction",   d.direction);
  setInput("cdf-moveType",    d.moveType);
  setInput("cdf-motionTypeId",d.motionTypeId);
  setInput("cdf-familyId",    d.familyId);
  setInput("cdf-grpSize",     d.grpSize);
  setInput("cdf-sex",         d.sex);
  setInput("cdf-block",       d.block);
  setInput("cdf-push",        d.push);

  // ステータス
  if (d.status) {
    var s = d.status;
    setInput("cdf-hp",               s.hp);
    setInput("cdf-maxHp",            s.maxHp);
    setInput("cdf-sp",               s.sp);
    setInput("cdf-maxSp",            s.maxSp);
    setInput("cdf-exp",              s.exp);
    setInput("cdf-level",            s.level);
    setInput("cdf-stamina",          s.stamina);
    setInput("cdf-power",            s.power);
    setInput("cdf-strength",         s.strength);
    setInput("cdf-magic",            s.magic);
    setInput("cdf-skillful",         s.skillful);
    setInput("cdf-abilityAT",        s.abilityAT);
    setInput("cdf-abilityDF",        s.abilityDF);
    setInput("cdf-pAtack",           s.pAtack);
    setInput("cdf-pDefense",         s.pDefense);
    setInput("cdf-pMagic",           s.pMagic);
    setInput("cdf-pMagicDefense",    s.pMagicDefense);
    setInput("cdf-pHitAverage",      s.pHitAverage);
    setInput("cdf-pAvoidAverage",    s.pAvoidAverage);
    setInput("cdf-pCriticalAverage", s.pCriticalAverage);
    setInput("cdf-attrFire",         s.attrFire);
    setInput("cdf-attrWind",         s.attrWind);
    setInput("cdf-attrWater",        s.attrWater);
    setInput("cdf-attrEarth",        s.attrEarth);
    setInput("cdf-attrLight",        s.attrLight);
    setInput("cdf-attrDark",         s.attrDark);
  }

  // 装備
  if (d.equipment) {
    var e = d.equipment;
    setInput("cdf-eqCloth",      e.cloth);
    setInput("cdf-eqAcce1",      e.accesory1);
    setInput("cdf-eqAcce2",      e.accesory2);
    setInput("cdf-eqArmsRight",  e.armsRight);
    setInput("cdf-eqArmsLeft",   e.armsLeft);
    setInput("cdf-eqHead",       e.head);
  }

  // グラフィック
  if (d.graphics) {
    var g = d.graphics;
    setInput("cdf-grpNpc",          g.npc);
    setInput("cdf-grpCloth",        g.cloth);
    setInput("cdf-grpEye",          g.eye);
    setInput("cdf-grpEyeColor",     g.eyeColor);
    setInput("cdf-grpHairType",     g.hairType);
    setInput("cdf-grpHairColor",    g.hairColor);
    setInput("cdf-grpSp",           g.sp);
    setInput("cdf-grpTmpMain",      g.tmpMain);
    setInput("cdf-grpTmpSub",       g.tmpSub);
    setInput("cdf-grpAcce",         g.acce);
    setInput("cdf-grpArmsMain",     g.armsMain);
    setInput("cdf-grpArmsSub",      g.armsSub);
    setInput("cdf-grpArmsLeftMain", g.armsLeftMain);
    setInput("cdf-grpArmsLeftSub",  g.armsLeftSub);
    setInput("cdf-grpInitNpc",      g.initNpc);
    setInput("cdf-grpInitCloth",    g.initCloth);
    setInput("cdf-grpInitEye",      g.initEye);
    setInput("cdf-grpInitEyeColor", g.initEyeColor);
    setInput("cdf-grpInitHairType", g.initHairType);
    setInput("cdf-grpInitHairColor",g.initHairColor);
    setInput("cdf-grpInitSp",       g.initSp);
  }

  // 移動（読み取り専用）
  if (d.movement) {
    var mv = d.movement;
    setText("cd-maxItemCount",      mv.maxItemCount);
    setText("cd-dropItemAverage",   mv.dropItemAverage);
    setText("cd-moveAverage",       mv.moveAverage);
    setText("cd-moveAverageBattle", mv.moveAverageBattle);
    setText("cd-moveWait",          mv.moveWait);
    setText("cd-moveWaitBattle",    mv.moveWaitBattle);
    setText("cd-searchCX",          mv.searchDistanceCX);
    setText("cd-searchCY",          mv.searchDistanceCY);
    setText("cd-motionTypeId",      mv.motionTypeId);
  }

  // NPC 発生（読み取り専用）
  if (d.npcSpawn) {
    var ns = d.npcSpawn;
    setText("cd-putCycle",     ns.putCycle);
    setText("cd-putMoveType",  ns.putMoveType);
    setText("cd-maxPutCount",  ns.maxPutCount);
    setText("cd-putAverage",   ns.putAverage);
    setText("cd-putAreaX",     ns.putAreaX);
    setText("cd-putAreaY",     ns.putAreaY);
  }

  if (charDetailBody) { charDetailBody.style.display = ""; }
  if (charDetailIdInput) { charDetailIdInput.value = String(d.charId); }
}

// 「表示」ボタン
if (charDetailLoadBtn) {
  charDetailLoadBtn.addEventListener("click", function () {
    if (!charDetailIdInput) { return; }
    var val = parseInt(charDetailIdInput.value, 10);
    if (!val || val <= 0) {
      if (charDetailFeedback) { charDetailFeedback.textContent = "有効な CharID を入力してください"; }
      return;
    }
    fetchCharacterDetail(val);
  });
}

/* -------------------------------------------------------
 * タブ切り替え
 * ------------------------------------------------------- */
var charTabNav = document.getElementById("char-tab-nav");
if (charTabNav) {
  charTabNav.addEventListener("click", function (ev) {
    var btn = ev.target.closest(".tab-btn");
    if (!btn) { return; }
    var tabId = btn.dataset.tab;
    if (!tabId) { return; }

    // タブボタンのアクティブ切り替え
    charTabNav.querySelectorAll(".tab-btn").forEach(function (b) {
      b.classList.toggle("is-active", b === btn);
    });

    // タブパネルの表示切り替え
    document.querySelectorAll("#char-detail-body .tab-panel").forEach(function (panel) {
      panel.style.display = (panel.id === tabId) ? "" : "none";
    });
  });
}

/* -------------------------------------------------------
 * 基本情報 保存  PUT /api/characters/{charId}
 * ------------------------------------------------------- */
var cdBasicForm = document.getElementById("cd-basic-form");
if (cdBasicForm) {
  cdBasicForm.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) {
      setFeedback("cd-basic-feedback", "先にキャラクターを表示してください", true);
      return;
    }
    setFeedback("cd-basic-feedback", "保存中...", false);

    // number 型フィールドの簡易バリデーション
    var numFields = ["cdf-mapId","cdf-x","cdf-y","cdf-direction","cdf-moveType",
                     "cdf-motionTypeId","cdf-familyId","cdf-grpSize","cdf-sex"];
    for (var i = 0; i < numFields.length; i++) {
      var el = document.getElementById(numFields[i]);
      if (el && el.value.trim() !== "" && isNaN(parseInt(el.value, 10))) {
        setFeedback("cd-basic-feedback", numFields[i] + " は数値で入力してください", true);
        return;
      }
    }

    var body = {};
    var charName = getStrVal("cdf-charName");
    if (charName !== null) { body.charName = charName; }
    var mapId    = getNumVal("cdf-mapId");       if (mapId    !== null) { body.mapId    = mapId; }
    var x        = getNumVal("cdf-x");           if (x        !== null) { body.x        = x; }
    var y        = getNumVal("cdf-y");           if (y        !== null) { body.y        = y; }
    var dir      = getNumVal("cdf-direction");   if (dir      !== null) { body.direction = dir; }
    var moveType = getNumVal("cdf-moveType");    if (moveType !== null) { body.moveType = moveType; }
    var motionId = getNumVal("cdf-motionTypeId");if (motionId !== null) { body.motionTypeId = motionId; }
    var familyId = getNumVal("cdf-familyId");    if (familyId !== null) { body.familyId = familyId; }
    var grpSize  = getNumVal("cdf-grpSize");     if (grpSize  !== null) { body.grpSize  = grpSize; }
    var sex      = getNumVal("cdf-sex");         if (sex      !== null) { body.sex      = sex; }
    body.block = getBoolVal("cdf-block");
    body.push  = getBoolVal("cdf-push");

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId, body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFeedback("cd-basic-feedback", "エラー: " + msg, true);
        return;
      }
      setFeedback("cd-basic-feedback", "保存しました", false);
      // 最新データを再取得して表示を更新する
      fetchCharacterDetail(currentCharId);
    } catch (err) {
      console.error("cd-basic-form save error", err);
      setFeedback("cd-basic-feedback", "通信エラーが発生しました", true);
    }
  });
}

/* -------------------------------------------------------
 * ステータス 保存  PUT /api/characters/{charId}/status
 * ------------------------------------------------------- */
var cdStatusForm = document.getElementById("cd-status-form");
if (cdStatusForm) {
  cdStatusForm.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) {
      setFeedback("cd-status-feedback", "先にキャラクターを表示してください", true);
      return;
    }
    setFeedback("cd-status-feedback", "保存中...", false);

    var statusFields = [
      ["cdf-hp","hp"],["cdf-maxHp","maxHp"],["cdf-sp","sp"],["cdf-maxSp","maxSp"],
      ["cdf-exp","exp"],["cdf-level","level"],["cdf-stamina","stamina"],
      ["cdf-power","power"],["cdf-strength","strength"],["cdf-magic","magic"],
      ["cdf-skillful","skillful"],["cdf-abilityAT","abilityAT"],["cdf-abilityDF","abilityDF"],
      ["cdf-pAtack","pAtack"],["cdf-pDefense","pDefense"],["cdf-pMagic","pMagic"],
      ["cdf-pMagicDefense","pMagicDefense"],["cdf-pHitAverage","pHitAverage"],
      ["cdf-pAvoidAverage","pAvoidAverage"],["cdf-pCriticalAverage","pCriticalAverage"],
      ["cdf-attrFire","attrFire"],["cdf-attrWind","attrWind"],["cdf-attrWater","attrWater"],
      ["cdf-attrEarth","attrEarth"],["cdf-attrLight","attrLight"],["cdf-attrDark","attrDark"]
    ];

    var body = {};
    for (var i = 0; i < statusFields.length; i++) {
      var pair = statusFields[i];
      var el = document.getElementById(pair[0]);
      if (!el) { continue; }
      var s = el.value.trim();
      if (s === "") { continue; }
      var n = parseInt(s, 10);
      if (isNaN(n)) {
        setFeedback("cd-status-feedback", pair[0] + " は数値で入力してください", true);
        return;
      }
      body[pair[1]] = n;
    }

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/status", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFeedback("cd-status-feedback", "エラー: " + msg, true);
        return;
      }
      setFeedback("cd-status-feedback", "保存しました", false);
      fetchCharacterDetail(currentCharId);
    } catch (err) {
      console.error("cd-status-form save error", err);
      setFeedback("cd-status-feedback", "通信エラーが発生しました", true);
    }
  });
}

/* -------------------------------------------------------
 * 装備 保存  PUT /api/characters/{charId}/equipment
 * ------------------------------------------------------- */
var cdEquipForm = document.getElementById("cd-equip-form");
if (cdEquipForm) {
  cdEquipForm.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) {
      setFeedback("cd-equip-feedback", "先にキャラクターを表示してください", true);
      return;
    }
    setFeedback("cd-equip-feedback", "保存中...", false);

    var equipFields = [
      ["cdf-eqCloth","cloth"],["cdf-eqAcce1","accesory1"],["cdf-eqAcce2","accesory2"],
      ["cdf-eqArmsRight","armsRight"],["cdf-eqArmsLeft","armsLeft"],["cdf-eqHead","head"]
    ];

    var body = {};
    for (var i = 0; i < equipFields.length; i++) {
      var pair = equipFields[i];
      var n = getNumVal(pair[0]);
      if (n !== null) { body[pair[1]] = n; }
    }

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/equipment", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFeedback("cd-equip-feedback", "エラー: " + msg, true);
        return;
      }
      setFeedback("cd-equip-feedback", "保存しました", false);
      fetchCharacterDetail(currentCharId);
    } catch (err) {
      console.error("cd-equip-form save error", err);
      setFeedback("cd-equip-feedback", "通信エラーが発生しました", true);
    }
  });
}

/* -------------------------------------------------------
 * グラフィック 保存  PUT /api/characters/{charId}/graphics
 * ------------------------------------------------------- */
var cdGraphicsForm = document.getElementById("cd-graphics-form");
if (cdGraphicsForm) {
  cdGraphicsForm.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) {
      setFeedback("cd-graphics-feedback", "先にキャラクターを表示してください", true);
      return;
    }
    setFeedback("cd-graphics-feedback", "保存中...", false);

    var grpFields = [
      ["cdf-grpNpc","npc"],["cdf-grpCloth","cloth"],["cdf-grpEye","eye"],
      ["cdf-grpEyeColor","eyeColor"],["cdf-grpHairType","hairType"],["cdf-grpHairColor","hairColor"],
      ["cdf-grpSp","sp"],["cdf-grpTmpMain","tmpMain"],["cdf-grpTmpSub","tmpSub"],
      ["cdf-grpAcce","acce"],["cdf-grpArmsMain","armsMain"],["cdf-grpArmsSub","armsSub"],
      ["cdf-grpArmsLeftMain","armsLeftMain"],["cdf-grpArmsLeftSub","armsLeftSub"],
      ["cdf-grpInitNpc","initNpc"],["cdf-grpInitCloth","initCloth"],["cdf-grpInitEye","initEye"],
      ["cdf-grpInitEyeColor","initEyeColor"],["cdf-grpInitHairType","initHairType"],
      ["cdf-grpInitHairColor","initHairColor"],["cdf-grpInitSp","initSp"]
    ];

    var body = {};
    for (var i = 0; i < grpFields.length; i++) {
      var pair = grpFields[i];
      var n = getNumVal(pair[0]);
      if (n !== null) { body[pair[1]] = n; }
    }

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/graphics", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFeedback("cd-graphics-feedback", "エラー: " + msg, true);
        return;
      }
      setFeedback("cd-graphics-feedback", "保存しました", false);
      fetchCharacterDetail(currentCharId);
    } catch (err) {
      console.error("cd-graphics-form save error", err);
      setFeedback("cd-graphics-feedback", "通信エラーが発生しました", true);
    }
  });
}

// character-overview 画面が直接ナビから開かれたときは ID 入力フォームを表示するだけでよい
// （一覧からクリックで来た場合は fetchCharacterDetail が自動的に呼ばれる）

/* =======================================================
 * 所持アイテム  GET/POST/DELETE /api/characters/{id}/items
 * ======================================================= */

/** GET /api/characters/{charId}/items → [{slot, itemId}] */
async function fetchCharacterItems(charId) {
  setFeedback("cd-items-feedback", "読み込み中...", false);
  try {
    var { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(charId) + "/items");
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "アイテム取得に失敗しました";
      setFeedback("cd-items-feedback", "エラー: " + msg, true);
      return;
    }
    setFeedback("cd-items-feedback", "", false);
    renderCharacterItems(data || []);
  } catch (err) {
    console.error("fetchCharacterItems error", err);
    setFeedback("cd-items-feedback", "通信エラーが発生しました", true);
  }
}

/** アイテム一覧テーブルを描画する */
function renderCharacterItems(items) {
  var tbody = document.getElementById("cd-items-table-body");
  if (!tbody) { return; }
  if (!items || items.length === 0) {
    tbody.innerHTML = "<tr><td colspan=\"3\">（データなし）</td></tr>";
    return;
  }
  var html = "";
  for (var i = 0; i < items.length; i++) {
    var item = items[i];
    html += "<tr>" +
      "<td>" + item.slot + "</td>" +
      "<td>" + item.itemId + "</td>" +
      "<td><button type=\"button\" class=\"button danger btn-sm cd-item-del-btn\" data-slot=\"" + item.slot + "\">削除</button></td>" +
      "</tr>";
  }
  tbody.innerHTML = html;

  // 削除ボタンにイベントを付与
  tbody.querySelectorAll(".cd-item-del-btn").forEach(function (btn) {
    btn.addEventListener("click", function () {
      var slot = parseInt(btn.dataset.slot, 10);
      if (!currentCharId) { return; }
      deleteCharacterItem(currentCharId, slot);
    });
  });
}

/** POST /api/characters/{charId}/items  body: {itemId} */
async function addCharacterItem(charId, itemId) {
  setFeedback("cd-items-feedback", "追加中...", false);
  try {
    var res = await fetch("/api/characters/" + encodeURIComponent(charId) + "/items", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ itemId: itemId }),
      credentials: "same-origin"
    });
    var data = null;
    try { data = await res.json(); } catch (_) {}
    if (!res.ok) {
      var errCode = (data && data.error) ? data.error : "エラー";
      if (errCode === "item_slots_full") {
        alert("アイテムスロットが満杯です（上限に達しています）");
      } else if (errCode === "not_found") {
        alert("キャラクターが見つかりません");
      } else {
        setFeedback("cd-items-feedback", "エラー: " + errCode, true);
      }
      return;
    }
    setFeedback("cd-items-feedback", "追加しました（スロット " + data.slot + "）", false);
    fetchCharacterItems(charId);
  } catch (err) {
    console.error("addCharacterItem error", err);
    setFeedback("cd-items-feedback", "通信エラーが発生しました", true);
  }
}

/** DELETE /api/characters/{charId}/items/{slot} */
async function deleteCharacterItem(charId, slot) {
  setFeedback("cd-items-feedback", "削除中...", false);
  try {
    var res = await fetch("/api/characters/" + encodeURIComponent(charId) + "/items/" + slot, {
      method: "DELETE",
      credentials: "same-origin"
    });
    if (res.status === 204) {
      setFeedback("cd-items-feedback", "削除しました（スロット " + slot + "）", false);
      fetchCharacterItems(charId);
      return;
    }
    var data = null;
    try { data = await res.json(); } catch (_) {}
    var errCode = (data && data.error) ? data.error : "エラー";
    if (errCode === "not_found" || errCode === "slot_not_found") {
      alert("指定スロットが見つかりません（slot: " + slot + "）");
    } else {
      setFeedback("cd-items-feedback", "エラー: " + errCode, true);
    }
  } catch (err) {
    console.error("deleteCharacterItem error", err);
    setFeedback("cd-items-feedback", "通信エラーが発生しました", true);
  }
}

// アイテム追加ボタン
var cdItemAddBtn = document.getElementById("cd-item-add-btn");
if (cdItemAddBtn) {
  cdItemAddBtn.addEventListener("click", function () {
    if (!currentCharId) {
      alert("先にキャラクターを表示してください");
      return;
    }
    var input = document.getElementById("cd-item-id-input");
    if (!input) { return; }
    var val = parseInt(input.value, 10);
    if (!val || val <= 0) {
      alert("有効なアイテムID（1以上の整数）を入力してください");
      return;
    }
    addCharacterItem(currentCharId, val);
  });
}

/* =======================================================
 * 所持スキル  GET/POST/DELETE /api/characters/{id}/skills
 * ======================================================= */

/** GET /api/characters/{charId}/skills → [{slot, skillId}] */
async function fetchCharacterSkills(charId) {
  setFeedback("cd-skills-feedback", "読み込み中...", false);
  try {
    var { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(charId) + "/skills");
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "スキル取得に失敗しました";
      setFeedback("cd-skills-feedback", "エラー: " + msg, true);
      return;
    }
    setFeedback("cd-skills-feedback", "", false);
    renderCharacterSkills(data || []);
  } catch (err) {
    console.error("fetchCharacterSkills error", err);
    setFeedback("cd-skills-feedback", "通信エラーが発生しました", true);
  }
}

/** スキル一覧テーブルを描画する */
function renderCharacterSkills(skills) {
  var tbody = document.getElementById("cd-skills-table-body");
  if (!tbody) { return; }
  if (!skills || skills.length === 0) {
    tbody.innerHTML = "<tr><td colspan=\"3\">（データなし）</td></tr>";
    return;
  }
  var html = "";
  for (var i = 0; i < skills.length; i++) {
    var skill = skills[i];
    html += "<tr>" +
      "<td>" + skill.slot + "</td>" +
      "<td>" + skill.skillId + "</td>" +
      "<td><button type=\"button\" class=\"button danger btn-sm cd-skill-del-btn\" data-slot=\"" + skill.slot + "\">削除</button></td>" +
      "</tr>";
  }
  tbody.innerHTML = html;

  // 削除ボタンにイベントを付与
  tbody.querySelectorAll(".cd-skill-del-btn").forEach(function (btn) {
    btn.addEventListener("click", function () {
      var slot = parseInt(btn.dataset.slot, 10);
      if (!currentCharId) { return; }
      deleteCharacterSkill(currentCharId, slot);
    });
  });
}

/** POST /api/characters/{charId}/skills  body: {skillId} */
async function addCharacterSkill(charId, skillId) {
  setFeedback("cd-skills-feedback", "追加中...", false);
  try {
    var res = await fetch("/api/characters/" + encodeURIComponent(charId) + "/skills", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ skillId: skillId }),
      credentials: "same-origin"
    });
    var data = null;
    try { data = await res.json(); } catch (_) {}
    if (!res.ok) {
      var errCode = (data && data.error) ? data.error : "エラー";
      if (errCode === "skill_slots_full") {
        alert("スキルスロットが満杯です（上限 64 スロットに達しています）");
      } else if (errCode === "not_found") {
        alert("キャラクターが見つかりません");
      } else {
        setFeedback("cd-skills-feedback", "エラー: " + errCode, true);
      }
      return;
    }
    setFeedback("cd-skills-feedback", "追加しました（スロット " + data.slot + "）", false);
    fetchCharacterSkills(charId);
  } catch (err) {
    console.error("addCharacterSkill error", err);
    setFeedback("cd-skills-feedback", "通信エラーが発生しました", true);
  }
}

/** DELETE /api/characters/{charId}/skills/{slot} */
async function deleteCharacterSkill(charId, slot) {
  setFeedback("cd-skills-feedback", "削除中...", false);
  try {
    var res = await fetch("/api/characters/" + encodeURIComponent(charId) + "/skills/" + slot, {
      method: "DELETE",
      credentials: "same-origin"
    });
    if (res.status === 204) {
      setFeedback("cd-skills-feedback", "削除しました（スロット " + slot + "）", false);
      fetchCharacterSkills(charId);
      return;
    }
    var data = null;
    try { data = await res.json(); } catch (_) {}
    var errCode = (data && data.error) ? data.error : "エラー";
    if (errCode === "not_found" || errCode === "slot_not_found") {
      alert("指定スロットが見つかりません（slot: " + slot + "）");
    } else {
      setFeedback("cd-skills-feedback", "エラー: " + errCode, true);
    }
  } catch (err) {
    console.error("deleteCharacterSkill error", err);
    setFeedback("cd-skills-feedback", "通信エラーが発生しました", true);
  }
}

// スキル追加ボタン
var cdSkillAddBtn = document.getElementById("cd-skill-add-btn");
if (cdSkillAddBtn) {
  cdSkillAddBtn.addEventListener("click", function () {
    if (!currentCharId) {
      alert("先にキャラクターを表示してください");
      return;
    }
    var input = document.getElementById("cd-skill-id-input");
    if (!input) { return; }
    var val = parseInt(input.value, 10);
    if (!val || val <= 0) {
      alert("有効なスキルID（1以上の整数）を入力してください");
      return;
    }
    addCharacterSkill(currentCharId, val);
  });
}

/* -------------------------------------------------------
 * タブ切り替え時にアイテム/スキル/アカウントを自動ロードする
 * ------------------------------------------------------- */
(function () {
  var charTabNav2 = document.getElementById("char-tab-nav");
  if (!charTabNav2) { return; }
  charTabNav2.addEventListener("click", function (ev) {
    var btn = ev.target.closest(".tab-btn");
    if (!btn) { return; }
    var tabId = btn.dataset.tab;
    if (!currentCharId) { return; }
    if (tabId === "cd-tab-items") {
      fetchCharacterItems(currentCharId);
    } else if (tabId === "cd-tab-skills") {
      fetchCharacterSkills(currentCharId);
    } else if (tabId === "cd-tab-account") {
      fetchCharacterAccount(currentCharId);
    }
  });
}());

/* -------------------------------------------------------
 * アカウント情報タブ (character-overview 内 cd-tab-account)
 * ------------------------------------------------------- */

/** GET /api/characters/{charId}/account を呼んでアカウント情報を取得する */
async function fetchCharacterAccount(charId) {
  var feedbackEl = document.getElementById("cd-account-feedback");
  if (feedbackEl) { feedbackEl.textContent = ""; }

  try {
    var url = "/api/characters/" + encodeURIComponent(charId) + "/account";
    var { response, data } = await fetchJson(url);
    if (!response.ok || !data) {
      var msg = (data && data.error) ? data.error : "アカウント情報の取得に失敗しました";
      if (feedbackEl) { setFeedback("cd-account-feedback", "エラー: " + msg, true); }
      return;
    }
    renderAccountInfo(data, "cd-acct-");
    // 無効化状態の表示（disabled フィールドがあれば表示、なければ「情報なし」）
    var disabledEl = document.getElementById("cd-disabled-status");
    if (disabledEl) { disabledEl.textContent = "（disabled 情報はアカウントAPIから取得）"; }
  } catch (err) {
    console.error("fetchCharacterAccount error", err);
    if (feedbackEl) { setFeedback("cd-account-feedback", "通信エラーが発生しました", true); }
  }
}

/** アカウント情報を指定プレフィックスの要素に反映する */
function renderAccountInfo(d, prefix) {
  function setText(id, val) {
    var el = document.getElementById(id);
    if (el) { el.textContent = (val === null || val === undefined || val === "") ? "-" : String(val); }
  }
  setText(prefix + "accountId", d.accountId);
  setText(prefix + "loginId",   d.loginId);
  setText(prefix + "ip",        d.ip);
  setText(prefix + "mac",       d.mac);
}

/* アカウントタブ内の各ボタンイベント */
(function () {
  // 再読み込みボタン
  var reloadBtn = document.getElementById("cd-acct-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () {
      if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
      fetchCharacterAccount(currentCharId);
    });
  }

  // アカウント紐付け変更ボタン
  var saveBtn = document.getElementById("cd-acct-save-btn");
  if (saveBtn) {
    saveBtn.addEventListener("click", function () {
      if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
      var val = getNumVal("cd-acct-id-input");
      if (val === null || val < 0) { alert("有効なアカウントID（0以上の整数）を入力してください"); return; }
      putCharacterAccount(currentCharId, val, "cd-account-feedback");
    });
  }

  // 解除ボタン（accountId=0）
  var unlinkBtn = document.getElementById("cd-acct-unlink-btn");
  if (unlinkBtn) {
    unlinkBtn.addEventListener("click", function () {
      if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
      if (!confirm("アカウント紐付けを解除しますか？")) { return; }
      putCharacterAccount(currentCharId, 0, "cd-account-feedback");
    });
  }

  // 管理者権限レベル保存ボタン
  var adminSaveBtn = document.getElementById("cd-admin-save-btn");
  if (adminSaveBtn) {
    adminSaveBtn.addEventListener("click", function () {
      if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
      var val = getNumVal("cd-admin-level-input");
      if (val === null || val < 0 || val > 255) { alert("権限レベルは 0〜255 の整数で入力してください"); return; }
      putCharacterAdmin(currentCharId, val, "cd-account-feedback");
    });
  }

  // BAN ボタン
  var banBtn = document.getElementById("cd-ban-btn");
  if (banBtn) {
    banBtn.addEventListener("click", function () {
      if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
      if (!confirm("このキャラクターのアカウントを BAN（無効化）しますか？")) { return; }
      putCharacterDisabled(currentCharId, true, "cd-account-feedback");
    });
  }

  // BAN 解除ボタン
  var unbanBtn = document.getElementById("cd-unban-btn");
  if (unbanBtn) {
    unbanBtn.addEventListener("click", function () {
      if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
      putCharacterDisabled(currentCharId, false, "cd-account-feedback");
    });
  }
}());

/** PUT /api/characters/{charId}/account */
async function putCharacterAccount(charId, accountId, feedbackId) {
  try {
    var { response, data } = await putJson("/api/characters/" + charId + "/account", { accountId: accountId });
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "アカウント紐付けの変更に失敗しました";
      setFeedback(feedbackId, "エラー: " + msg, true);
      return;
    }
    setFeedback(feedbackId, "アカウント紐付けを変更しました（accountId=" + accountId + "）", false);
    fetchCharacterAccount(charId);
  } catch (err) {
    console.error("putCharacterAccount error", err);
    setFeedback(feedbackId, "通信エラーが発生しました", true);
  }
}

/** PUT /api/characters/{charId}/admin */
async function putCharacterAdmin(charId, adminLevel, feedbackId) {
  try {
    var { response, data } = await putJson("/api/characters/" + charId + "/admin", { adminLevel: adminLevel });
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "管理者権限の設定に失敗しました";
      setFeedback(feedbackId, "エラー: " + msg, true);
      return;
    }
    setFeedback(feedbackId, "管理者権限レベルを " + adminLevel + " に設定しました", false);
  } catch (err) {
    console.error("putCharacterAdmin error", err);
    setFeedback(feedbackId, "通信エラーが発生しました", true);
  }
}

/** PUT /api/characters/{charId}/disabled */
async function putCharacterDisabled(charId, disabled, feedbackId) {
  try {
    var { response, data } = await putJson("/api/characters/" + charId + "/disabled", { disabled: disabled });
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "無効化状態の変更に失敗しました";
      setFeedback(feedbackId, "エラー: " + msg, true);
      return;
    }
    var stateStr = disabled ? "BAN（無効化）" : "BAN 解除（有効化）";
    setFeedback(feedbackId, stateStr + " しました", false);
    var disabledEl = document.getElementById("cd-disabled-status");
    if (disabledEl) { disabledEl.textContent = disabled ? "BAN中" : "有効"; }
  } catch (err) {
    console.error("putCharacterDisabled error", err);
    setFeedback(feedbackId, "通信エラーが発生しました", true);
  }
}

/* -------------------------------------------------------
 * character-account ビュー（スタンドアロン画面）
 * ------------------------------------------------------- */

(function () {
  var caLoadBtn = document.getElementById("ca-load-btn");
  if (!caLoadBtn) { return; }

  // 表示ボタン
  caLoadBtn.addEventListener("click", function () {
    var val = getNumVal("ca-char-id-input");
    if (!val || val <= 0) { alert("有効な CharID（1以上の整数）を入力してください"); return; }
    loadCaView(val);
  });

  // Enter キーで表示
  var caInput = document.getElementById("ca-char-id-input");
  if (caInput) {
    caInput.addEventListener("keydown", function (ev) {
      if (ev.key === "Enter") { caLoadBtn.click(); }
    });
  }

  // アカウント紐付け変更
  var caLinkBtn = document.getElementById("ca-link-btn");
  if (caLinkBtn) {
    caLinkBtn.addEventListener("click", function () {
      var charId = getNumVal("ca-char-id-input");
      if (!charId) { return; }
      var val = getNumVal("ca-new-account-id");
      if (val === null || val < 0) { alert("有効なアカウントID（0以上）を入力してください"); return; }
      putCaAccount(charId, val);
    });
  }

  // 解除ボタン（accountId=0）
  var caUnlinkBtn = document.getElementById("ca-unlink-btn");
  if (caUnlinkBtn) {
    caUnlinkBtn.addEventListener("click", function () {
      var charId = getNumVal("ca-char-id-input");
      if (!charId) { return; }
      if (!confirm("アカウント紐付けを解除しますか？")) { return; }
      putCaAccount(charId, 0);
    });
  }

  // 管理者権限レベル保存
  var caAdminBtn = document.getElementById("ca-admin-btn");
  if (caAdminBtn) {
    caAdminBtn.addEventListener("click", function () {
      var charId = getNumVal("ca-char-id-input");
      if (!charId) { return; }
      var val = getNumVal("ca-admin-level");
      if (val === null || val < 0 || val > 255) { alert("権限レベルは 0〜255 の整数で入力してください"); return; }
      putCaAdmin(charId, val);
    });
  }

  // BAN ボタン
  var caBanBtn = document.getElementById("ca-ban-btn");
  if (caBanBtn) {
    caBanBtn.addEventListener("click", function () {
      var charId = getNumVal("ca-char-id-input");
      if (!charId) { return; }
      if (!confirm("このキャラクターのアカウントを BAN しますか？")) { return; }
      putCaDisabled(charId, true);
    });
  }

  // BAN 解除ボタン
  var caUnbanBtn = document.getElementById("ca-unban-btn");
  if (caUnbanBtn) {
    caUnbanBtn.addEventListener("click", function () {
      var charId = getNumVal("ca-char-id-input");
      if (!charId) { return; }
      putCaDisabled(charId, false);
    });
  }
}());

/** character-account ビューのキャラ情報とアカウント情報をロードする */
async function loadCaView(charId) {
  var feedbackEl = document.getElementById("ca-feedback");
  var bodyEl     = document.getElementById("ca-body");
  if (feedbackEl) { feedbackEl.textContent = "読み込み中..."; }
  if (bodyEl) { bodyEl.style.display = "none"; }

  try {
    // まずキャラ詳細（charName を表示するため）
    var detailRes = await fetchJson("/api/characters/" + encodeURIComponent(charId));
    if (!detailRes.response.ok || !detailRes.data) {
      var msg = (detailRes.data && detailRes.data.error) ? detailRes.data.error : "キャラクターが見つかりません";
      if (feedbackEl) { setFeedback("ca-feedback", "エラー: " + msg, true); }
      return;
    }
    var d = detailRes.data;
    var charNameEl = document.getElementById("ca-charName");
    var charIdEl   = document.getElementById("ca-charId");
    if (charIdEl)   { charIdEl.textContent   = String(d.charId); }
    if (charNameEl) { charNameEl.textContent = d.charName || "-"; }

    // アカウント情報
    var acctRes = await fetchJson("/api/characters/" + encodeURIComponent(charId) + "/account");
    if (!acctRes.response.ok || !acctRes.data) {
      var acctMsg = (acctRes.data && acctRes.data.error) ? acctRes.data.error : "アカウント情報の取得に失敗しました";
      if (feedbackEl) { setFeedback("ca-feedback", "エラー: " + acctMsg, true); }
      return;
    }
    renderAccountInfo(acctRes.data, "ca-acct-");

    // 無効化状態は現在 API から返らないため表示なし
    var caDisabledEl = document.getElementById("ca-disabled-status");
    if (caDisabledEl) { caDisabledEl.textContent = "（APIから取得不可 — BAN 操作は実行可能）"; }

    if (feedbackEl) { feedbackEl.textContent = ""; }
    if (bodyEl) { bodyEl.style.display = ""; }
  } catch (err) {
    console.error("loadCaView error", err);
    if (feedbackEl) { setFeedback("ca-feedback", "通信エラーが発生しました", true); }
  }
}

/** character-account ビュー: アカウント紐付け変更 */
async function putCaAccount(charId, accountId) {
  try {
    var { response, data } = await putJson("/api/characters/" + charId + "/account", { accountId: accountId });
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "アカウント紐付けの変更に失敗しました";
      setFeedback("ca-link-feedback", "エラー: " + msg, true);
      return;
    }
    setFeedback("ca-link-feedback", "アカウント紐付けを変更しました（accountId=" + accountId + "）", false);
    // アカウント情報を再取得して表示更新
    var acctRes = await fetchJson("/api/characters/" + charId + "/account");
    if (acctRes.response.ok && acctRes.data) { renderAccountInfo(acctRes.data, "ca-acct-"); }
  } catch (err) {
    console.error("putCaAccount error", err);
    setFeedback("ca-link-feedback", "通信エラーが発生しました", true);
  }
}

/** character-account ビュー: 管理者権限レベル設定 */
async function putCaAdmin(charId, adminLevel) {
  try {
    var { response, data } = await putJson("/api/characters/" + charId + "/admin", { adminLevel: adminLevel });
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "管理者権限の設定に失敗しました";
      setFeedback("ca-admin-feedback", "エラー: " + msg, true);
      return;
    }
    setFeedback("ca-admin-feedback", "管理者権限レベルを " + adminLevel + " に設定しました", false);
  } catch (err) {
    console.error("putCaAdmin error", err);
    setFeedback("ca-admin-feedback", "通信エラーが発生しました", true);
  }
}

/** character-account ビュー: 無効化/解除 */
async function putCaDisabled(charId, disabled) {
  try {
    var { response, data } = await putJson("/api/characters/" + charId + "/disabled", { disabled: disabled });
    if (!response.ok) {
      var msg = (data && data.error) ? data.error : "無効化状態の変更に失敗しました";
      setFeedback("ca-ban-feedback", "エラー: " + msg, true);
      return;
    }
    var stateStr = disabled ? "BAN（無効化）" : "BAN 解除（有効化）";
    setFeedback("ca-ban-feedback", stateStr + " しました", false);
    var caDisabledEl = document.getElementById("ca-disabled-status");
    if (caDisabledEl) { caDisabledEl.textContent = disabled ? "BAN 中" : "有効"; }
  } catch (err) {
    console.error("putCaDisabled error", err);
    setFeedback("ca-ban-feedback", "通信エラーが発生しました", true);
  }
}

// ---------------------------------------------------------------------------
// NPC 追加
// ---------------------------------------------------------------------------

async function submitAddNpc(event) {
  event.preventDefault();

  var form = document.getElementById("npc-add-form");
  if (!form) { return; }

  // 必須項目
  var charName = document.getElementById("npc-charName").value.trim();
  var familyId = parseInt(document.getElementById("npc-familyId").value, 10);
  var mapId    = parseInt(document.getElementById("npc-mapId").value, 10);
  var x        = parseInt(document.getElementById("npc-x").value, 10);
  var y        = parseInt(document.getElementById("npc-y").value, 10);

  if (!charName) {
    setFeedback("npc-add-feedback", "キャラ名は必須です", true);
    return;
  }
  if (isNaN(familyId) || familyId <= 0) {
    setFeedback("npc-add-feedback", "種族IDは1以上の整数を入力してください", true);
    return;
  }
  if (isNaN(mapId) || mapId <= 0) {
    setFeedback("npc-add-feedback", "マップIDは1以上の整数を入力してください", true);
    return;
  }
  if (isNaN(x) || isNaN(y)) {
    setFeedback("npc-add-feedback", "座標 X/Y を正しく入力してください", true);
    return;
  }

  // 任意項目
  var moveType    = parseInt(document.getElementById("npc-moveType").value, 10);
  var sex         = parseInt(document.getElementById("npc-sex").value, 10) || 0;
  var grpIdNpc    = parseInt(document.getElementById("npc-grpIdNpc").value, 10) || 0;
  var grpIdInitNpc = parseInt(document.getElementById("npc-grpIdInitNpc").value, 10) || 0;
  var motionTypeId = parseInt(document.getElementById("npc-motionTypeId").value, 10) || 0;
  var block       = document.getElementById("npc-block").checked ? 1 : 0;
  var push        = document.getElementById("npc-push").checked  ? 1 : 0;

  var body = {
    charName:     charName,
    familyId:     familyId,
    mapId:        mapId,
    x:            x,
    y:            y,
    moveType:     isNaN(moveType) ? 1 : moveType,
    sex:          sex,
    grpIdNpc:     grpIdNpc,
    grpIdInitNpc: grpIdInitNpc,
    motionTypeId: motionTypeId,
    block:        block,
    push:         push
  };

  setFeedback("npc-add-feedback", "追加中...", false);

  try {
    var response = await fetch("/api/characters/npc", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body)
    });
    var data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }

    if (!response.ok) {
      var errMsg = (data && data.error) ? data.error : "追加に失敗しました (HTTP " + response.status + ")";
      setFeedback("npc-add-feedback", "エラー: " + errMsg, true);
      return;
    }

    var charId = data && data.charId ? data.charId : "?";
    setFeedback("npc-add-feedback",
      "NPC を追加しました (charId=" + charId + ", mapId=" + mapId + ", x=" + x + ", y=" + y + ")",
      false);

    // フォームをリセットして再利用しやすくする
    form.reset();
    // moveType を default に戻す
    document.getElementById("npc-moveType").value = "1";

    // キャラクター一覧画面へ遷移する選択肢を提供
    if (confirm("キャラクター一覧でこの NPC を確認しますか？")) {
      navigateTo("character-search");
    }
  } catch (err) {
    console.error("submitAddNpc error", err);
    setFeedback("npc-add-feedback", "通信エラーが発生しました", true);
  }
}

// ---------------------------------------------------------------------------
// アイテム種別管理（Wave 2D）
// ---------------------------------------------------------------------------

const itemTypeState = {
  items: [],          // サーバー取得済みアイテム種別一覧
  isLoading: false,
  loadError: null,
  editing: null       // 編集中の typeId（新規は 0）
};

const ITEM_TYPE_LABELS = {
  0: "効果無し",
  1: "服",
  2: "アクセサリ",
  3: "持ち物",
  4: "盾",
  5: "HP増減",
  6: "灯り"
};

function itemTypeLabel(id) {
  return ITEM_TYPE_LABELS[id] || ("不明(" + id + ")");
}

function setItemTypeFeedback(msg, isError) {
  setFeedback("item-type-feedback", msg || "", !!isError);
}

function setItemTypeEditFeedback(msg, isError) {
  setFeedback("item-type-edit-feedback", msg || "", !!isError);
}

async function loadItemTypeList(forceReload) {
  if (itemTypeState.isLoading) { return; }
  itemTypeState.isLoading = true;
  itemTypeState.loadError = null;
  if (forceReload) {
    setItemTypeFeedback("読み込み中...", false);
  }
  try {
    const { response, data } = await fetchJson("/api/item-types");
    if (!response.ok || !data || !Array.isArray(data.items)) {
      throw new Error("item-types load failed: HTTP " + response.status);
    }
    itemTypeState.items = data.items;
    renderItemTypeTable();
    setItemTypeFeedback("", false);
  } catch (err) {
    console.error("loadItemTypeList error", err);
    itemTypeState.loadError = err.message || String(err);
    setItemTypeFeedback("読み込みに失敗しました", true);
  } finally {
    itemTypeState.isLoading = false;
  }
}

function renderItemTypeTable() {
  const tbody = document.getElementById("item-type-table-body");
  const summary = document.getElementById("item-type-summary");
  if (!tbody) { return; }
  tbody.innerHTML = "";

  const items = itemTypeState.items || [];
  if (summary) {
    summary.textContent = items.length + " 件";
  }

  items.forEach(function (it) {
    const tr = document.createElement("tr");

    const tdId = document.createElement("td");
    tdId.textContent = it.typeId;
    tr.appendChild(tdId);

    const tdName = document.createElement("td");
    tdName.textContent = it.name || "";
    tr.appendChild(tdName);

    const tdType = document.createElement("td");
    tdType.textContent = itemTypeLabel(it.itemTypeId);
    tr.appendChild(tdType);

    const tdGrp = document.createElement("td");
    tdGrp.textContent = it.grpId;
    tr.appendChild(tdGrp);

    const tdIcon = document.createElement("td");
    tdIcon.textContent = it.iconGrpId;
    tr.appendChild(tdIcon);

    const tdOps = document.createElement("td");
    const btnEdit = document.createElement("button");
    btnEdit.type = "button";
    btnEdit.className = "btn-secondary";
    btnEdit.textContent = "編集";
    btnEdit.addEventListener("click", function () { openItemTypeEdit(it); });
    tdOps.appendChild(btnEdit);

    const btnDel = document.createElement("button");
    btnDel.type = "button";
    btnDel.className = "btn-secondary";
    btnDel.textContent = "削除";
    btnDel.style.marginLeft = "0.5rem";
    btnDel.addEventListener("click", function () { deleteItemType(it); });
    tdOps.appendChild(btnDel);

    tr.appendChild(tdOps);
    tbody.appendChild(tr);
  });
}

function openItemTypeEdit(item) {
  const card = document.getElementById("item-type-edit-card");
  const title = document.getElementById("item-type-edit-title");
  if (!card) { return; }
  card.style.display = "";

  const isNew = !item;
  itemTypeState.editing = isNew ? 0 : item.typeId;

  if (title) {
    title.textContent = isNew ? "アイテム種別 新規追加" : ("アイテム種別 編集 (ID=" + item.typeId + ")");
  }

  const src = item || {
    typeId: 0, itemTypeId: 0, name: "",
    delAverage: 0, putOn: false,
    grpId: 0, iconGrpId: 0, grpIdMain: 0, grpIdSub: 0,
    dropSoundId: 0, useEffectId: 0, useSoundId: 0,
    weaponInfoId: 0, value: 0, value2: 0,
    moveWait: 0, moveCount: 0, target: 0, area: 0
  };

  document.getElementById("item-type-edit-typeId").value      = isNew ? "" : String(src.typeId);
  document.getElementById("item-type-edit-itemTypeId").value  = String(src.itemTypeId);
  document.getElementById("item-type-edit-name").value        = src.name || "";
  document.getElementById("item-type-edit-delAverage").value  = String(src.delAverage || 0);
  document.getElementById("item-type-edit-putOn").checked     = !!src.putOn;
  document.getElementById("item-type-edit-grpId").value       = String(src.grpId || 0);
  document.getElementById("item-type-edit-iconGrpId").value   = String(src.iconGrpId || 0);
  document.getElementById("item-type-edit-grpIdMain").value   = String(src.grpIdMain || 0);
  document.getElementById("item-type-edit-grpIdSub").value    = String(src.grpIdSub || 0);
  document.getElementById("item-type-edit-dropSoundId").value = String(src.dropSoundId || 0);
  document.getElementById("item-type-edit-useEffectId").value = String(src.useEffectId || 0);
  document.getElementById("item-type-edit-useSoundId").value  = String(src.useSoundId || 0);
  document.getElementById("item-type-edit-weaponInfoId").value = String(src.weaponInfoId || 0);
  document.getElementById("item-type-edit-value").value       = String(src.value || 0);
  document.getElementById("item-type-edit-value2").value      = String(src.value2 || 0);
  document.getElementById("item-type-edit-moveWait").value    = String(src.moveWait || 0);
  document.getElementById("item-type-edit-moveCount").value   = String(src.moveCount || 0);
  document.getElementById("item-type-edit-target").value      = String(src.target || 0);
  document.getElementById("item-type-edit-area").value        = String(src.area || 0);

  setItemTypeEditFeedback("", false);

  // スクロールしてフォームを見せる
  card.scrollIntoView({ behavior: "smooth", block: "start" });
}

function closeItemTypeEdit() {
  const card = document.getElementById("item-type-edit-card");
  if (card) { card.style.display = "none"; }
  itemTypeState.editing = null;
  setItemTypeEditFeedback("", false);
}

function buildItemTypeBodyFromForm() {
  function intVal(id) {
    const v = parseInt(document.getElementById(id).value, 10);
    return isNaN(v) ? 0 : v;
  }
  return {
    itemTypeId:   intVal("item-type-edit-itemTypeId"),
    name:         document.getElementById("item-type-edit-name").value,
    delAverage:   intVal("item-type-edit-delAverage"),
    putOn:        document.getElementById("item-type-edit-putOn").checked,
    grpId:        intVal("item-type-edit-grpId"),
    iconGrpId:    intVal("item-type-edit-iconGrpId"),
    grpIdMain:    intVal("item-type-edit-grpIdMain"),
    grpIdSub:     intVal("item-type-edit-grpIdSub"),
    dropSoundId:  intVal("item-type-edit-dropSoundId"),
    useEffectId:  intVal("item-type-edit-useEffectId"),
    useSoundId:   intVal("item-type-edit-useSoundId"),
    weaponInfoId: intVal("item-type-edit-weaponInfoId"),
    value:        intVal("item-type-edit-value"),
    value2:       intVal("item-type-edit-value2"),
    moveWait:     intVal("item-type-edit-moveWait"),
    moveCount:    intVal("item-type-edit-moveCount"),
    target:       intVal("item-type-edit-target"),
    area:         intVal("item-type-edit-area")
  };
}

async function submitItemTypeForm(event) {
  event.preventDefault();

  const editing = itemTypeState.editing;
  if (editing === null) { return; }

  const body = buildItemTypeBodyFromForm();
  const isNew = (editing === 0);
  const method = isNew ? "POST" : "PUT";
  if (!isNew) {
    body.typeId = editing;
  }

  setItemTypeEditFeedback(isNew ? "追加中..." : "保存中...", false);

  try {
    const response = await fetch("/api/item-types", {
      method: method,
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify(body)
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setItemTypeEditFeedback("エラー: " + errMsg, true);
      return;
    }
    setItemTypeEditFeedback(isNew ? "追加しました" : "保存しました", false);
    await loadItemTypeList(false);
    if (isNew && data && data.typeId) {
      // 新規作成後は作成された項目の編集フォームに切り替え
      const created = itemTypeState.items.find(function (x) { return x.typeId === data.typeId; });
      if (created) {
        openItemTypeEdit(created);
      }
    }
  } catch (err) {
    console.error("submitItemTypeForm error", err);
    setItemTypeEditFeedback("通信エラーが発生しました", true);
  }
}

async function deleteItemType(item) {
  if (!item || !item.typeId) { return; }
  const msg = "アイテム種別 [" + (item.name || "") + "] (ID=" + item.typeId + ") を削除しますか？";
  if (!confirm(msg)) { return; }

  setItemTypeFeedback("削除中...", false);
  try {
    const response = await fetch("/api/item-types", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify({ typeId: item.typeId })
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setItemTypeFeedback("削除エラー: " + errMsg, true);
      return;
    }
    setItemTypeFeedback("削除しました (ID=" + item.typeId + ")", false);
    // 編集フォームが同じ ID を開いていたら閉じる
    if (itemTypeState.editing === item.typeId) {
      closeItemTypeEdit();
    }
    await loadItemTypeList(false);
  } catch (err) {
    console.error("deleteItemType error", err);
    setItemTypeFeedback("通信エラーが発生しました", true);
  }
}

function setupItemTypeView() {
  const reloadBtn = document.getElementById("item-type-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () { loadItemTypeList(true); });
  }
  const newBtn = document.getElementById("item-type-new-btn");
  if (newBtn) {
    newBtn.addEventListener("click", function () { openItemTypeEdit(null); });
  }
  const form = document.getElementById("item-type-edit-form");
  if (form) {
    form.addEventListener("submit", submitItemTypeForm);
  }
  const cancelBtn = document.getElementById("item-type-cancel-btn");
  if (cancelBtn) {
    cancelBtn.addEventListener("click", function () { closeItemTypeEdit(); });
  }
}

// 初期化ブートストラップ（load イベント後に1度だけ実行する）
window.addEventListener("load", function () {
  setupItemTypeView();
  setupItemView();
  setupWeaponView();
  setupBalloonView();
  setupEffectView();
  setupInitialStatusView();
});

// ---------------------------------------------------------------------------
// アイテム一覧管理（Wave 2D）
// ---------------------------------------------------------------------------

const itemState = {
  items: [],           // サーバー取得済みアイテム一覧
  isLoading: false,
  loadError: null,
  editing: null,       // 編集中の itemId（新規は 0）
  filter: { drop: true, charId: null, mapId: null }
};

function setItemFeedback(msg, isError) {
  setFeedback("item-feedback", msg || "", !!isError);
}

function setItemEditFeedback(msg, isError) {
  setFeedback("item-edit-feedback", msg || "", !!isError);
}

function buildItemQueryString() {
  const params = [];
  if (itemState.filter.drop) { params.push("drop=1"); }
  if (itemState.filter.charId !== null && itemState.filter.charId >= 0) {
    params.push("charId=" + itemState.filter.charId);
  }
  if (itemState.filter.mapId !== null && itemState.filter.mapId >= 0) {
    params.push("mapId=" + itemState.filter.mapId);
  }
  return params.length ? ("?" + params.join("&")) : "";
}

async function loadItemList(forceReload) {
  if (itemState.isLoading) { return; }
  itemState.isLoading = true;
  itemState.loadError = null;
  if (forceReload) {
    setItemFeedback("読み込み中...", false);
  }
  try {
    const url = "/api/items" + buildItemQueryString();
    const { response, data } = await fetchJson(url);
    if (!response.ok || !data || !Array.isArray(data.items)) {
      throw new Error("items load failed: HTTP " + response.status);
    }
    itemState.items = data.items;
    renderItemTable();
    setItemFeedback("", false);
  } catch (err) {
    console.error("loadItemList error", err);
    itemState.loadError = err.message || String(err);
    setItemFeedback("読み込みに失敗しました", true);
  } finally {
    itemState.isLoading = false;
  }
}

function renderItemTable() {
  const tbody = document.getElementById("item-table-body");
  const summary = document.getElementById("item-summary");
  if (!tbody) { return; }
  tbody.innerHTML = "";

  const items = itemState.items || [];
  if (summary) {
    summary.textContent = items.length + " 件";
  }

  items.forEach(function (it) {
    const tr = document.createElement("tr");

    const tdId = document.createElement("td");
    tdId.textContent = it.itemId;
    tr.appendChild(tdId);

    const tdName = document.createElement("td");
    tdName.textContent = it.name || "";
    tr.appendChild(tdName);

    const tdType = document.createElement("td");
    const typeLabel = it.itemTypeName ? it.itemTypeName : "";
    tdType.textContent = "[" + it.itemTypeId + "] " + typeLabel;
    tr.appendChild(tdType);

    const tdChar = document.createElement("td");
    tdChar.textContent = it.charId ? ("[" + it.charId + "]") : "";
    tr.appendChild(tdChar);

    const tdPos = document.createElement("td");
    if (it.mapId) {
      tdPos.textContent = "MAP:" + it.mapId + " (" + it.posX + "," + it.posY + ")";
    } else {
      tdPos.textContent = "";
    }
    tr.appendChild(tdPos);

    const tdGrp = document.createElement("td");
    tdGrp.textContent = it.grpId + " / " + it.iconGrpId;
    tr.appendChild(tdGrp);

    const tdOps = document.createElement("td");
    const btnEdit = document.createElement("button");
    btnEdit.type = "button";
    btnEdit.className = "btn-secondary";
    btnEdit.textContent = "編集";
    btnEdit.addEventListener("click", function () { openItemEdit(it); });
    tdOps.appendChild(btnEdit);

    const btnDel = document.createElement("button");
    btnDel.type = "button";
    btnDel.className = "btn-secondary";
    btnDel.textContent = "削除";
    btnDel.style.marginLeft = "0.5rem";
    btnDel.addEventListener("click", function () { deleteItem(it); });
    tdOps.appendChild(btnDel);

    tr.appendChild(tdOps);
    tbody.appendChild(tr);
  });
}

function openItemEdit(item) {
  const card = document.getElementById("item-edit-card");
  const title = document.getElementById("item-edit-title");
  if (!card) { return; }
  card.style.display = "";

  const isNew = !item;
  itemState.editing = isNew ? 0 : item.itemId;

  if (title) {
    title.textContent = isNew ? "アイテム 新規追加" : ("アイテム 編集 (ID=" + item.itemId + ")");
  }

  const src = item || {
    itemId: 0, itemTypeId: 1, name: "",
    grpId: 0, iconGrpId: 0, dropSoundId: 0,
    putOn: false,
    mapId: 0, posX: 0, posY: 0, posZ: 0,
    charId: 0, backPackX: 0, backPackY: 0,
    itemTypeName: ""
  };

  document.getElementById("item-edit-itemId").value     = isNew ? "" : String(src.itemId);
  document.getElementById("item-edit-itemTypeId").value = String(src.itemTypeId || 1);
  document.getElementById("item-edit-itemTypeName").textContent =
      src.itemTypeName ? ("（" + src.itemTypeName + "）") : "";
  document.getElementById("item-edit-name").value       = src.name || "";
  document.getElementById("item-edit-putOn").checked    = !!src.putOn;
  document.getElementById("item-edit-grpId").value      = String(src.grpId || 0);
  document.getElementById("item-edit-iconGrpId").value  = String(src.iconGrpId || 0);
  document.getElementById("item-edit-dropSoundId").value = String(src.dropSoundId || 0);
  document.getElementById("item-edit-mapId").value      = String(src.mapId || 0);
  document.getElementById("item-edit-posX").value       = String(src.posX || 0);
  document.getElementById("item-edit-posY").value       = String(src.posY || 0);
  document.getElementById("item-edit-posZ").value       = String(src.posZ || 0);
  document.getElementById("item-edit-charId").value     = String(src.charId || 0);
  document.getElementById("item-edit-backPackX").value  = String(src.backPackX || 0);
  document.getElementById("item-edit-backPackY").value  = String(src.backPackY || 0);

  setItemEditFeedback("", false);
  card.scrollIntoView({ behavior: "smooth", block: "start" });
}

function closeItemEdit() {
  const card = document.getElementById("item-edit-card");
  if (card) { card.style.display = "none"; }
  itemState.editing = null;
  setItemEditFeedback("", false);
}

function buildItemBodyFromForm() {
  function intVal(id) {
    const v = parseInt(document.getElementById(id).value, 10);
    return isNaN(v) ? 0 : v;
  }
  return {
    itemTypeId:  intVal("item-edit-itemTypeId"),
    name:        document.getElementById("item-edit-name").value,
    putOn:       document.getElementById("item-edit-putOn").checked,
    grpId:       intVal("item-edit-grpId"),
    iconGrpId:   intVal("item-edit-iconGrpId"),
    dropSoundId: intVal("item-edit-dropSoundId"),
    mapId:       intVal("item-edit-mapId"),
    posX:        intVal("item-edit-posX"),
    posY:        intVal("item-edit-posY"),
    posZ:        intVal("item-edit-posZ"),
    charId:      intVal("item-edit-charId"),
    backPackX:   intVal("item-edit-backPackX"),
    backPackY:   intVal("item-edit-backPackY")
  };
}

async function submitItemForm(event) {
  event.preventDefault();

  const editing = itemState.editing;
  if (editing === null) { return; }

  const body = buildItemBodyFromForm();
  const isNew = (editing === 0);
  const method = isNew ? "POST" : "PUT";
  if (!isNew) {
    body.itemId = editing;
  }

  setItemEditFeedback(isNew ? "追加中..." : "保存中...", false);

  try {
    const response = await fetch("/api/items", {
      method: method,
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify(body)
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setItemEditFeedback("エラー: " + errMsg, true);
      return;
    }
    setItemEditFeedback(isNew ? "追加しました" : "保存しました", false);
    await loadItemList(false);
    if (isNew && data && data.itemId) {
      const created = itemState.items.find(function (x) { return x.itemId === data.itemId; });
      if (created) {
        openItemEdit(created);
      }
    }
  } catch (err) {
    console.error("submitItemForm error", err);
    setItemEditFeedback("通信エラーが発生しました", true);
  }
}

async function deleteItem(item) {
  if (!item || !item.itemId) { return; }
  const msg = "アイテム [" + (item.name || "") + "] (ID=" + item.itemId + ") を削除しますか？";
  if (!confirm(msg)) { return; }

  setItemFeedback("削除中...", false);
  try {
    const response = await fetch("/api/items", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify({ itemId: item.itemId })
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setItemFeedback("削除エラー: " + errMsg, true);
      return;
    }
    setItemFeedback("削除しました (ID=" + item.itemId + ")", false);
    if (itemState.editing === item.itemId) {
      closeItemEdit();
    }
    await loadItemList(false);
  } catch (err) {
    console.error("deleteItem error", err);
    setItemFeedback("通信エラーが発生しました", true);
  }
}

function applyItemFilterFromUi() {
  const drop = document.getElementById("item-filter-drop").checked;
  const charIdRaw = document.getElementById("item-filter-charid").value;
  const mapIdRaw = document.getElementById("item-filter-mapid").value;
  itemState.filter.drop = drop;
  itemState.filter.charId = charIdRaw === "" ? null : parseInt(charIdRaw, 10);
  itemState.filter.mapId = mapIdRaw === "" ? null : parseInt(mapIdRaw, 10);
  if (itemState.filter.charId !== null && isNaN(itemState.filter.charId)) { itemState.filter.charId = null; }
  if (itemState.filter.mapId !== null && isNaN(itemState.filter.mapId)) { itemState.filter.mapId = null; }
}

function clearItemFilter() {
  document.getElementById("item-filter-drop").checked = true;
  document.getElementById("item-filter-charid").value = "";
  document.getElementById("item-filter-mapid").value = "";
  itemState.filter = { drop: true, charId: null, mapId: null };
}

function setupItemView() {
  const reloadBtn = document.getElementById("item-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () { loadItemList(true); });
  }
  const newBtn = document.getElementById("item-new-btn");
  if (newBtn) {
    newBtn.addEventListener("click", function () { openItemEdit(null); });
  }
  const form = document.getElementById("item-edit-form");
  if (form) {
    form.addEventListener("submit", submitItemForm);
  }
  const cancelBtn = document.getElementById("item-cancel-btn");
  if (cancelBtn) {
    cancelBtn.addEventListener("click", function () { closeItemEdit(); });
  }
  const applyBtn = document.getElementById("item-filter-apply-btn");
  if (applyBtn) {
    applyBtn.addEventListener("click", function () {
      applyItemFilterFromUi();
      loadItemList(true);
    });
  }
  const clearBtn = document.getElementById("item-filter-clear-btn");
  if (clearBtn) {
    clearBtn.addEventListener("click", function () {
      clearItemFilter();
      loadItemList(true);
    });
  }
}

// ---------------------------------------------------------------------------
// 武器一覧管理（Wave 2D）
// ---------------------------------------------------------------------------

// 使用可能な攻撃モーションのビットフラグ（InfoItemArms.h の INFOITEMARMS_MOTION_*）
const WEAPON_MOTION_SWING   = 0x01;
const WEAPON_MOTION_POKE    = 0x02;
const WEAPON_MOTION_BOW     = 0x04;
const WEAPON_MOTION_BLOW    = 0x08;
const WEAPON_MOTION_FISHING = 0x10;

const weaponState = {
  items: [],           // サーバー取得済み武器情報一覧
  isLoading: false,
  loadError: null,
  editing: null        // 編集中の weaponInfoId（新規は 0）
};

function setWeaponFeedback(msg, isError) {
  setFeedback("weapon-feedback", msg || "", !!isError);
}

function setWeaponEditFeedback(msg, isError) {
  setFeedback("weapon-edit-feedback", msg || "", !!isError);
}

async function loadWeaponList(forceReload) {
  if (weaponState.isLoading) { return; }
  weaponState.isLoading = true;
  weaponState.loadError = null;
  if (forceReload) {
    setWeaponFeedback("読み込み中...", false);
  }
  try {
    const { response, data } = await fetchJson("/api/weapons");
    if (!response.ok || !data || !Array.isArray(data.items)) {
      throw new Error("weapons load failed: HTTP " + response.status);
    }
    weaponState.items = data.items;
    renderWeaponTable();
    setWeaponFeedback("", false);
  } catch (err) {
    console.error("loadWeaponList error", err);
    weaponState.loadError = err.message || String(err);
    setWeaponFeedback("読み込みに失敗しました", true);
  } finally {
    weaponState.isLoading = false;
  }
}

function formatMotionType(motionType) {
  const labels = [];
  if (motionType & WEAPON_MOTION_SWING)   { labels.push("振る"); }
  if (motionType & WEAPON_MOTION_POKE)    { labels.push("突く"); }
  if (motionType & WEAPON_MOTION_BOW)     { labels.push("弓"); }
  if (motionType & WEAPON_MOTION_BLOW)    { labels.push("打撃"); }
  if (motionType & WEAPON_MOTION_FISHING) { labels.push("釣り"); }
  return labels.length ? labels.join(",") : "-";
}

function renderWeaponTable() {
  const tbody = document.getElementById("weapon-table-body");
  const summary = document.getElementById("weapon-summary");
  if (!tbody) { return; }
  tbody.innerHTML = "";

  const items = weaponState.items || [];
  if (summary) {
    summary.textContent = items.length + " 件";
  }

  items.forEach(function (w) {
    const tr = document.createElement("tr");

    const tdId = document.createElement("td");
    tdId.textContent = w.weaponInfoId;
    tr.appendChild(tdId);

    const tdName = document.createElement("td");
    tdName.textContent = w.name || "";
    tr.appendChild(tdName);

    const tdMotion = document.createElement("td");
    tdMotion.textContent = formatMotionType(w.motionType || 0);
    tr.appendChild(tdMotion);

    const tdAtk = document.createElement("td");
    tdAtk.textContent = String((w.effectIdAtack || []).length);
    tr.appendChild(tdAtk);

    const tdCri = document.createElement("td");
    tdCri.textContent = String((w.effectIdCritical || []).length);
    tr.appendChild(tdCri);

    const tdOps = document.createElement("td");
    const btnEdit = document.createElement("button");
    btnEdit.type = "button";
    btnEdit.className = "btn-secondary";
    btnEdit.textContent = "編集";
    btnEdit.addEventListener("click", function () { openWeaponEdit(w); });
    tdOps.appendChild(btnEdit);

    const btnDel = document.createElement("button");
    btnDel.type = "button";
    btnDel.className = "btn-secondary";
    btnDel.textContent = "削除";
    btnDel.style.marginLeft = "0.5rem";
    btnDel.addEventListener("click", function () { deleteWeapon(w); });
    tdOps.appendChild(btnDel);

    tr.appendChild(tdOps);
    tbody.appendChild(tr);
  });
}

function openWeaponEdit(weapon) {
  const card = document.getElementById("weapon-edit-card");
  const title = document.getElementById("weapon-edit-title");
  if (!card) { return; }
  card.style.display = "";

  const isNew = !weapon;
  weaponState.editing = isNew ? 0 : weapon.weaponInfoId;

  if (title) {
    title.textContent = isNew
        ? "武器情報 新規追加"
        : ("武器情報 編集 (ID=" + weapon.weaponInfoId + ")");
  }

  const src = weapon || {
    weaponInfoId: 0, name: "",
    motionType: 0, motionTypeStand: 0, motionTypeWalk: 0,
    effectIdAtack: [], effectIdCritical: []
  };

  document.getElementById("weapon-edit-weaponInfoId").value =
      isNew ? "" : String(src.weaponInfoId);
  document.getElementById("weapon-edit-name").value = src.name || "";

  const mt = src.motionType || 0;
  document.getElementById("weapon-edit-motion-swing").checked   = !!(mt & WEAPON_MOTION_SWING);
  document.getElementById("weapon-edit-motion-poke").checked    = !!(mt & WEAPON_MOTION_POKE);
  document.getElementById("weapon-edit-motion-bow").checked     = !!(mt & WEAPON_MOTION_BOW);
  document.getElementById("weapon-edit-motion-blow").checked    = !!(mt & WEAPON_MOTION_BLOW);
  document.getElementById("weapon-edit-motion-fishing").checked = !!(mt & WEAPON_MOTION_FISHING);

  document.getElementById("weapon-edit-motionTypeStand").value = String(src.motionTypeStand || 0);
  document.getElementById("weapon-edit-motionTypeWalk").value  = String(src.motionTypeWalk  || 0);

  document.getElementById("weapon-edit-effectIdAtack").value    = (src.effectIdAtack    || []).join(",");
  document.getElementById("weapon-edit-effectIdCritical").value = (src.effectIdCritical || []).join(",");

  setWeaponEditFeedback("", false);
  card.scrollIntoView({ behavior: "smooth", block: "start" });
}

function closeWeaponEdit() {
  const card = document.getElementById("weapon-edit-card");
  if (card) { card.style.display = "none"; }
  weaponState.editing = null;
  setWeaponEditFeedback("", false);
}

function parseIdListFromCsv(text) {
  if (!text) { return []; }
  const parts = String(text).split(",");
  const out = [];
  for (let i = 0; i < parts.length; ++i) {
    const s = parts[i].trim();
    if (s === "") { continue; }
    const n = parseInt(s, 10);
    if (!isNaN(n) && n >= 0) { out.push(n); }
  }
  return out;
}

function buildWeaponBodyFromForm() {
  function intVal(id) {
    const v = parseInt(document.getElementById(id).value, 10);
    return isNaN(v) ? 0 : v;
  }
  let motionType = 0;
  if (document.getElementById("weapon-edit-motion-swing").checked)   { motionType |= WEAPON_MOTION_SWING; }
  if (document.getElementById("weapon-edit-motion-poke").checked)    { motionType |= WEAPON_MOTION_POKE; }
  if (document.getElementById("weapon-edit-motion-bow").checked)     { motionType |= WEAPON_MOTION_BOW; }
  if (document.getElementById("weapon-edit-motion-blow").checked)    { motionType |= WEAPON_MOTION_BLOW; }
  if (document.getElementById("weapon-edit-motion-fishing").checked) { motionType |= WEAPON_MOTION_FISHING; }

  return {
    name: document.getElementById("weapon-edit-name").value,
    motionType: motionType,
    motionTypeStand: intVal("weapon-edit-motionTypeStand"),
    motionTypeWalk:  intVal("weapon-edit-motionTypeWalk"),
    effectIdAtack:    parseIdListFromCsv(document.getElementById("weapon-edit-effectIdAtack").value),
    effectIdCritical: parseIdListFromCsv(document.getElementById("weapon-edit-effectIdCritical").value)
  };
}

async function submitWeaponForm(event) {
  event.preventDefault();

  const editing = weaponState.editing;
  if (editing === null) { return; }

  const body = buildWeaponBodyFromForm();
  const isNew = (editing === 0);
  const method = isNew ? "POST" : "PUT";
  if (!isNew) {
    body.weaponInfoId = editing;
  }

  setWeaponEditFeedback(isNew ? "追加中..." : "保存中...", false);

  try {
    const response = await fetch("/api/weapons", {
      method: method,
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify(body)
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setWeaponEditFeedback("エラー: " + errMsg, true);
      return;
    }
    setWeaponEditFeedback(isNew ? "追加しました" : "保存しました", false);
    await loadWeaponList(false);
    if (isNew && data && data.weaponInfoId) {
      const created = weaponState.items.find(function (x) {
        return x.weaponInfoId === data.weaponInfoId;
      });
      if (created) {
        openWeaponEdit(created);
      }
    }
  } catch (err) {
    console.error("submitWeaponForm error", err);
    setWeaponEditFeedback("通信エラーが発生しました", true);
  }
}

async function deleteWeapon(weapon) {
  if (!weapon || !weapon.weaponInfoId) { return; }
  const msg = "武器情報 [" + (weapon.name || "") + "] (ID=" + weapon.weaponInfoId + ") を削除しますか？";
  if (!confirm(msg)) { return; }

  setWeaponFeedback("削除中...", false);
  try {
    const response = await fetch("/api/weapons", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify({ weaponInfoId: weapon.weaponInfoId })
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setWeaponFeedback("削除エラー: " + errMsg, true);
      return;
    }
    setWeaponFeedback("削除しました (ID=" + weapon.weaponInfoId + ")", false);
    if (weaponState.editing === weapon.weaponInfoId) {
      closeWeaponEdit();
    }
    await loadWeaponList(false);
  } catch (err) {
    console.error("deleteWeapon error", err);
    setWeaponFeedback("通信エラーが発生しました", true);
  }
}

function setupWeaponView() {
  const reloadBtn = document.getElementById("weapon-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () { loadWeaponList(true); });
  }
  const newBtn = document.getElementById("weapon-new-btn");
  if (newBtn) {
    newBtn.addEventListener("click", function () { openWeaponEdit(null); });
  }
  const form = document.getElementById("weapon-edit-form");
  if (form) {
    form.addEventListener("submit", submitWeaponForm);
  }
  const cancelBtn = document.getElementById("weapon-cancel-btn");
  if (cancelBtn) {
    cancelBtn.addEventListener("click", function () { closeWeaponEdit(); });
  }
}

// ---------------------------------------------------------------------------
// 噴出し（吹き出し）情報 /api/efc-balloons
// ---------------------------------------------------------------------------

const balloonState = {
  items: [],           // サーバー取得済みコマ一覧
  isLoading: false,
  loadError: null,
  editing: null        // 編集中の efcBalloonId（新規は 0）
};

function setBalloonFeedback(msg, isError) {
  setFeedback("balloon-feedback", msg || "", !!isError);
}

function setBalloonEditFeedback(msg, isError) {
  setFeedback("balloon-edit-feedback", msg || "", !!isError);
}

async function loadBalloonList(forceReload) {
  if (balloonState.isLoading) { return; }
  balloonState.isLoading = true;
  balloonState.loadError = null;
  if (forceReload) {
    setBalloonFeedback("読み込み中...", false);
  }
  try {
    const { response, data } = await fetchJson("/api/efc-balloons");
    if (!response.ok || !data || !Array.isArray(data.items)) {
      throw new Error("balloons load failed: HTTP " + response.status);
    }
    balloonState.items = data.items;
    renderBalloonTable();
    setBalloonFeedback("", false);
  } catch (err) {
    console.error("loadBalloonList error", err);
    balloonState.loadError = err.message || String(err);
    setBalloonFeedback("読み込みに失敗しました", true);
  } finally {
    balloonState.isLoading = false;
  }
}

function renderBalloonTable() {
  const tbody = document.getElementById("balloon-table-body");
  const summary = document.getElementById("balloon-summary");
  if (!tbody) { return; }
  tbody.innerHTML = "";

  // listId → コマ数 をサマリに
  const items = balloonState.items || [];
  if (summary) {
    const listIds = {};
    items.forEach(function (b) { listIds[b.listId] = (listIds[b.listId] || 0) + 1; });
    const listCount = Object.keys(listIds).length;
    summary.textContent = items.length + " コマ / " + listCount + " 種別";
  }

  items.forEach(function (b) {
    const tr = document.createElement("tr");

    const tdId = document.createElement("td");
    tdId.textContent = b.efcBalloonId;
    tr.appendChild(tdId);

    const tdListId = document.createElement("td");
    tdListId.textContent = b.listId;
    tr.appendChild(tdListId);

    const tdAnime = document.createElement("td");
    tdAnime.textContent = b.animeId;
    tr.appendChild(tdAnime);

    const tdName = document.createElement("td");
    tdName.textContent = b.name || "";
    tr.appendChild(tdName);

    const tdGrp = document.createElement("td");
    tdGrp.textContent = b.grpId;
    tr.appendChild(tdGrp);

    const tdSound = document.createElement("td");
    tdSound.textContent = b.soundId;
    tr.appendChild(tdSound);

    const tdWait = document.createElement("td");
    tdWait.textContent = b.wait;
    tr.appendChild(tdWait);

    const tdLoop = document.createElement("td");
    tdLoop.textContent = b.loop ? "◯" : "";
    tr.appendChild(tdLoop);

    const tdOps = document.createElement("td");
    const btnEdit = document.createElement("button");
    btnEdit.type = "button";
    btnEdit.className = "btn-secondary";
    btnEdit.textContent = "編集";
    btnEdit.addEventListener("click", function () { openBalloonEdit(b); });
    tdOps.appendChild(btnEdit);

    const btnDel = document.createElement("button");
    btnDel.type = "button";
    btnDel.className = "btn-secondary";
    btnDel.textContent = "削除";
    btnDel.style.marginLeft = "0.5rem";
    btnDel.addEventListener("click", function () { deleteBalloon(b); });
    tdOps.appendChild(btnDel);

    tr.appendChild(tdOps);
    tbody.appendChild(tr);
  });
}

function openBalloonEdit(balloon) {
  const card = document.getElementById("balloon-edit-card");
  const title = document.getElementById("balloon-edit-title");
  if (!card) { return; }
  card.style.display = "";

  const isNew = !balloon;
  balloonState.editing = isNew ? 0 : balloon.efcBalloonId;

  if (title) {
    title.textContent = isNew
        ? "噴出し コマ新規追加"
        : ("噴出し コマ編集 (ID=" + balloon.efcBalloonId + ")");
  }

  const src = balloon || {
    efcBalloonId: 0, listId: 1, animeId: 0,
    name: "", grpId: 0, soundId: 0, wait: 0, loop: false
  };

  document.getElementById("balloon-edit-efcBalloonId").value =
      isNew ? "" : String(src.efcBalloonId);
  document.getElementById("balloon-edit-listId").value  = String(src.listId || 1);
  document.getElementById("balloon-edit-animeId").value = String(src.animeId || 0);
  document.getElementById("balloon-edit-name").value    = src.name || "";
  document.getElementById("balloon-edit-grpId").value   = String(src.grpId || 0);
  document.getElementById("balloon-edit-soundId").value = String(src.soundId || 0);
  document.getElementById("balloon-edit-wait").value    = String(src.wait || 0);
  document.getElementById("balloon-edit-loop").checked  = !!src.loop;

  setBalloonEditFeedback("", false);
  card.scrollIntoView({ behavior: "smooth", block: "start" });
}

function closeBalloonEdit() {
  const card = document.getElementById("balloon-edit-card");
  if (card) { card.style.display = "none"; }
  balloonState.editing = null;
  setBalloonEditFeedback("", false);
}

function buildBalloonBodyFromForm() {
  function intVal(id) {
    const v = parseInt(document.getElementById(id).value, 10);
    return isNaN(v) ? 0 : v;
  }
  return {
    listId:  intVal("balloon-edit-listId"),
    animeId: intVal("balloon-edit-animeId"),
    name:    document.getElementById("balloon-edit-name").value,
    grpId:   intVal("balloon-edit-grpId"),
    soundId: intVal("balloon-edit-soundId"),
    wait:    intVal("balloon-edit-wait"),
    loop:    document.getElementById("balloon-edit-loop").checked
  };
}

async function submitBalloonForm(event) {
  event.preventDefault();

  const editing = balloonState.editing;
  if (editing === null) { return; }

  const body = buildBalloonBodyFromForm();
  const isNew = (editing === 0);
  const method = isNew ? "POST" : "PUT";
  if (!isNew) {
    body.efcBalloonId = editing;
  }

  if (isNew && (!body.listId || body.listId <= 0)) {
    setBalloonEditFeedback("種別ID は 1 以上を指定してください", true);
    return;
  }

  setBalloonEditFeedback(isNew ? "追加中..." : "保存中...", false);

  try {
    const response = await fetch("/api/efc-balloons", {
      method: method,
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify(body)
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setBalloonEditFeedback("エラー: " + errMsg, true);
      return;
    }
    setBalloonEditFeedback(isNew ? "追加しました" : "保存しました", false);
    await loadBalloonList(false);
    if (isNew && data && data.efcBalloonId) {
      const created = balloonState.items.find(function (x) {
        return x.efcBalloonId === data.efcBalloonId;
      });
      if (created) {
        openBalloonEdit(created);
      }
    }
  } catch (err) {
    console.error("submitBalloonForm error", err);
    setBalloonEditFeedback("通信エラーが発生しました", true);
  }
}

async function deleteBalloon(balloon) {
  if (!balloon || !balloon.efcBalloonId) { return; }
  const label = (balloon.name || "") +
                " (listId=" + balloon.listId + ", animeId=" + balloon.animeId + ")";
  const msg = "噴出しコマ [" + label + "] (ID=" + balloon.efcBalloonId + ") を削除しますか？";
  if (!confirm(msg)) { return; }

  setBalloonFeedback("削除中...", false);
  try {
    const response = await fetch("/api/efc-balloons", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify({ efcBalloonId: balloon.efcBalloonId })
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setBalloonFeedback("削除エラー: " + errMsg, true);
      return;
    }
    setBalloonFeedback("削除しました (ID=" + balloon.efcBalloonId + ")", false);
    if (balloonState.editing === balloon.efcBalloonId) {
      closeBalloonEdit();
    }
    await loadBalloonList(false);
  } catch (err) {
    console.error("deleteBalloon error", err);
    setBalloonFeedback("通信エラーが発生しました", true);
  }
}

function setupBalloonView() {
  const reloadBtn = document.getElementById("balloon-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () { loadBalloonList(true); });
  }
  const newBtn = document.getElementById("balloon-new-btn");
  if (newBtn) {
    newBtn.addEventListener("click", function () { openBalloonEdit(null); });
  }
  const form = document.getElementById("balloon-edit-form");
  if (form) {
    form.addEventListener("submit", submitBalloonForm);
  }
  const cancelBtn = document.getElementById("balloon-cancel-btn");
  if (cancelBtn) {
    cancelBtn.addEventListener("click", function () { closeBalloonEdit(); });
  }
}

// ---------------------------------------------------------------------------
// エフェクト情報 /api/effects
// ---------------------------------------------------------------------------

const effectState = {
  items: [],            // サーバー取得済みエフェクト一覧
  isLoading: false,
  loadError: null,
  editing: null,        // 編集中の effectId（新規は 0）
  editAnimes: []        // 編集中の animes ワーキングコピー
};

function setEffectFeedback(msg, isError) {
  setFeedback("effect-feedback", msg || "", !!isError);
}

function setEffectEditFeedback(msg, isError) {
  setFeedback("effect-edit-feedback", msg || "", !!isError);
}

async function loadEffectList(forceReload) {
  if (effectState.isLoading) { return; }
  effectState.isLoading = true;
  effectState.loadError = null;
  if (forceReload) {
    setEffectFeedback("読み込み中...", false);
  }
  try {
    const { response, data } = await fetchJson("/api/effects");
    if (!response.ok || !data || !Array.isArray(data.items)) {
      throw new Error("effects load failed: HTTP " + response.status);
    }
    effectState.items = data.items;
    renderEffectTable();
    setEffectFeedback("", false);
  } catch (err) {
    console.error("loadEffectList error", err);
    effectState.loadError = err.message || String(err);
    setEffectFeedback("読み込みに失敗しました", true);
  } finally {
    effectState.isLoading = false;
  }
}

function renderEffectTable() {
  const tbody = document.getElementById("effect-table-body");
  const summary = document.getElementById("effect-summary");
  if (!tbody) { return; }
  tbody.innerHTML = "";

  const items = effectState.items || [];
  if (summary) {
    summary.textContent = items.length + " 件";
  }

  items.forEach(function (e) {
    const tr = document.createElement("tr");

    const tdId = document.createElement("td");
    tdId.textContent = e.effectId;
    tr.appendChild(tdId);

    const tdName = document.createElement("td");
    tdName.textContent = e.name || "";
    tr.appendChild(tdName);

    const tdGrpMain = document.createElement("td");
    tdGrpMain.textContent = e.grpIdMain;
    tr.appendChild(tdGrpMain);

    const tdSound = document.createElement("td");
    tdSound.textContent = e.soundId;
    tr.appendChild(tdSound);

    const tdAnime = document.createElement("td");
    tdAnime.textContent = Array.isArray(e.animes) ? e.animes.length : (e.animeCount || 0);
    tr.appendChild(tdAnime);

    const tdLoop = document.createElement("td");
    tdLoop.textContent = e.loop ? "◯" : "";
    tr.appendChild(tdLoop);

    const tdLoopSound = document.createElement("td");
    tdLoopSound.textContent = e.loopSound ? "◯" : "";
    tr.appendChild(tdLoopSound);

    const tdOps = document.createElement("td");
    const btnEdit = document.createElement("button");
    btnEdit.type = "button";
    btnEdit.className = "btn-secondary";
    btnEdit.textContent = "編集";
    btnEdit.addEventListener("click", function () { openEffectEdit(e); });
    tdOps.appendChild(btnEdit);

    const btnDel = document.createElement("button");
    btnDel.type = "button";
    btnDel.className = "btn-secondary";
    btnDel.textContent = "削除";
    btnDel.style.marginLeft = "0.5rem";
    btnDel.addEventListener("click", function () { deleteEffect(e); });
    tdOps.appendChild(btnDel);

    tr.appendChild(tdOps);
    tbody.appendChild(tr);
  });
}

function openEffectEdit(effect) {
  const card = document.getElementById("effect-edit-card");
  const title = document.getElementById("effect-edit-title");
  if (!card) { return; }
  card.style.display = "";

  const isNew = !effect;
  effectState.editing = isNew ? 0 : effect.effectId;

  if (title) {
    title.textContent = isNew
        ? "エフェクト 新規追加"
        : ("エフェクト編集 (ID=" + effect.effectId + ")");
  }

  const src = effect || {
    effectId: 0, name: "", grpIdMain: 0, soundId: 0,
    loop: false, loopSound: false, animes: []
  };

  document.getElementById("effect-edit-effectId").value =
      isNew ? "" : String(src.effectId);
  document.getElementById("effect-edit-name").value      = src.name || "";
  document.getElementById("effect-edit-grpIdMain").value = String(src.grpIdMain || 0);
  document.getElementById("effect-edit-soundId").value   = String(src.soundId || 0);
  document.getElementById("effect-edit-loop").checked      = !!src.loop;
  document.getElementById("effect-edit-loopSound").checked = !!src.loopSound;

  // animes のワーキングコピー（深いコピー）
  effectState.editAnimes = (src.animes || []).map(function (a) {
    return {
      wait:      a.wait      || 0,
      level:     a.level     || 0,
      grpIdBase: a.grpIdBase || 0,
      grpIdPile: a.grpIdPile || 0
    };
  });
  renderEffectAnimeTable();

  setEffectEditFeedback("", false);
  card.scrollIntoView({ behavior: "smooth", block: "start" });
}

function closeEffectEdit() {
  const card = document.getElementById("effect-edit-card");
  if (card) { card.style.display = "none"; }
  effectState.editing = null;
  effectState.editAnimes = [];
  setEffectEditFeedback("", false);
}

function renderEffectAnimeTable() {
  const tbody = document.getElementById("effect-anime-table-body");
  if (!tbody) { return; }
  tbody.innerHTML = "";

  const animes = effectState.editAnimes || [];
  animes.forEach(function (a, idx) {
    const tr = document.createElement("tr");

    const tdIdx = document.createElement("td");
    tdIdx.textContent = String(idx);
    tr.appendChild(tdIdx);

    function makeNumCell(value, max, onChange) {
      const td = document.createElement("td");
      const inp = document.createElement("input");
      inp.type = "number";
      inp.min = "0";
      if (max !== null) { inp.max = String(max); }
      inp.value = String(value || 0);
      inp.style.width = "6em";
      inp.addEventListener("input", function () {
        let v = parseInt(inp.value, 10);
        if (isNaN(v) || v < 0) { v = 0; }
        if (max !== null && v > max) { v = max; }
        onChange(v);
      });
      td.appendChild(inp);
      return td;
    }

    tr.appendChild(makeNumCell(a.wait, 255, function (v) { a.wait = v; }));
    tr.appendChild(makeNumCell(a.level, 255, function (v) { a.level = v; }));
    tr.appendChild(makeNumCell(a.grpIdBase, 65535, function (v) { a.grpIdBase = v; }));
    tr.appendChild(makeNumCell(a.grpIdPile, 65535, function (v) { a.grpIdPile = v; }));

    const tdOps = document.createElement("td");
    const btnUp = document.createElement("button");
    btnUp.type = "button";
    btnUp.className = "btn-secondary";
    btnUp.textContent = "↑";
    btnUp.disabled = (idx === 0);
    btnUp.addEventListener("click", function () {
      const arr = effectState.editAnimes;
      if (idx > 0) {
        const t = arr[idx - 1]; arr[idx - 1] = arr[idx]; arr[idx] = t;
        renderEffectAnimeTable();
      }
    });
    tdOps.appendChild(btnUp);

    const btnDown = document.createElement("button");
    btnDown.type = "button";
    btnDown.className = "btn-secondary";
    btnDown.textContent = "↓";
    btnDown.style.marginLeft = "0.25rem";
    btnDown.disabled = (idx === animes.length - 1);
    btnDown.addEventListener("click", function () {
      const arr = effectState.editAnimes;
      if (idx < arr.length - 1) {
        const t = arr[idx + 1]; arr[idx + 1] = arr[idx]; arr[idx] = t;
        renderEffectAnimeTable();
      }
    });
    tdOps.appendChild(btnDown);

    const btnDel = document.createElement("button");
    btnDel.type = "button";
    btnDel.className = "btn-secondary";
    btnDel.textContent = "削除";
    btnDel.style.marginLeft = "0.5rem";
    btnDel.addEventListener("click", function () {
      effectState.editAnimes.splice(idx, 1);
      renderEffectAnimeTable();
    });
    tdOps.appendChild(btnDel);

    tr.appendChild(tdOps);
    tbody.appendChild(tr);
  });
}

function addEffectAnime() {
  const arr = effectState.editAnimes || [];
  if (arr.length >= 255) {
    setEffectEditFeedback("コマ数は最大 255 です", true);
    return;
  }
  arr.push({ wait: 0, level: 255, grpIdBase: 0, grpIdPile: 0 });
  effectState.editAnimes = arr;
  renderEffectAnimeTable();
}

function buildEffectBodyFromForm() {
  function intVal(id) {
    const v = parseInt(document.getElementById(id).value, 10);
    return isNaN(v) ? 0 : v;
  }
  return {
    name:      document.getElementById("effect-edit-name").value,
    grpIdMain: intVal("effect-edit-grpIdMain"),
    soundId:   intVal("effect-edit-soundId"),
    loop:      document.getElementById("effect-edit-loop").checked,
    loopSound: document.getElementById("effect-edit-loopSound").checked,
    animes:    (effectState.editAnimes || []).map(function (a) {
      return {
        wait:      a.wait      || 0,
        level:     a.level     || 0,
        grpIdBase: a.grpIdBase || 0,
        grpIdPile: a.grpIdPile || 0
      };
    })
  };
}

async function submitEffectForm(event) {
  event.preventDefault();

  const editing = effectState.editing;
  if (editing === null) { return; }

  const body = buildEffectBodyFromForm();
  const isNew = (editing === 0);
  const method = isNew ? "POST" : "PUT";
  if (!isNew) {
    body.effectId = editing;
  }

  setEffectEditFeedback(isNew ? "追加中..." : "保存中...", false);

  try {
    const response = await fetch("/api/effects", {
      method: method,
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify(body)
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setEffectEditFeedback("エラー: " + errMsg, true);
      return;
    }
    setEffectEditFeedback(isNew ? "追加しました" : "保存しました", false);
    await loadEffectList(false);
    if (isNew && data && data.effectId) {
      const created = effectState.items.find(function (x) {
        return x.effectId === data.effectId;
      });
      if (created) {
        openEffectEdit(created);
      }
    } else if (!isNew && data) {
      // 既存編集: サーバーが返した内容で編集中の animes を更新
      effectState.editAnimes = (data.animes || []).map(function (a) {
        return {
          wait:      a.wait      || 0,
          level:     a.level     || 0,
          grpIdBase: a.grpIdBase || 0,
          grpIdPile: a.grpIdPile || 0
        };
      });
      renderEffectAnimeTable();
    }
  } catch (err) {
    console.error("submitEffectForm error", err);
    setEffectEditFeedback("通信エラーが発生しました", true);
  }
}

async function deleteEffect(effect) {
  if (!effect || !effect.effectId) { return; }
  const label = effect.name || "";
  const msg = "エフェクト [" + label + "] (ID=" + effect.effectId + ") を削除しますか？";
  if (!confirm(msg)) { return; }

  setEffectFeedback("削除中...", false);
  try {
    const response = await fetch("/api/effects", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      credentials: "include",
      body: JSON.stringify({ effectId: effect.effectId })
    });
    let data = null;
    try { data = await response.json(); } catch (e) { /* 無視 */ }
    if (!response.ok) {
      const errMsg = (data && data.error) ? data.error : ("HTTP " + response.status);
      setEffectFeedback("削除エラー: " + errMsg, true);
      return;
    }
    setEffectFeedback("削除しました (ID=" + effect.effectId + ")", false);
    if (effectState.editing === effect.effectId) {
      closeEffectEdit();
    }
    await loadEffectList(false);
  } catch (err) {
    console.error("deleteEffect error", err);
    setEffectFeedback("通信エラーが発生しました", true);
  }
}

function setupEffectView() {
  const reloadBtn = document.getElementById("effect-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () { loadEffectList(true); });
  }
  const newBtn = document.getElementById("effect-new-btn");
  if (newBtn) {
    newBtn.addEventListener("click", function () { openEffectEdit(null); });
  }
  const addAnimeBtn = document.getElementById("effect-anime-add-btn");
  if (addAnimeBtn) {
    addAnimeBtn.addEventListener("click", function () { addEffectAnime(); });
  }
  const form = document.getElementById("effect-edit-form");
  if (form) {
    form.addEventListener("submit", submitEffectForm);
  }
  const cancelBtn = document.getElementById("effect-cancel-btn");
  if (cancelBtn) {
    cancelBtn.addEventListener("click", function () { closeEffectEdit(); });
  }
}

// ---------------------------------------------------------------------------
// 初期ステータス設定 /api/initial-status（Wave 2D）
// 単一レコード設定のため GET / PUT のみで運用する。
// ---------------------------------------------------------------------------

const initialStatusState = {
  current: null,    // 最後にサーバーから取得した値（フォーム「元に戻す」の基準）
  isLoading: false
};

// フォーム上のフィールドID（HTML側のID接頭辞 init- を除いたキー一覧）
const INITIAL_STATUS_FIELDS = [
  "mapId", "posX", "posY",
  "maxHp", "maxSp",
  "stamina", "power", "strength", "magic", "skillful",
  "abilityAt", "abilityDf",
  "pAtack", "pDefense", "pMagic", "pMagicDefense",
  "pHitAverage", "pAvoidAverage", "pCriticalAverage",
  "attrFire", "attrWind", "attrWater", "attrEarth", "attrLight", "attrDark"
];

function setInitialStatusFeedback(msg, isError) {
  setFeedback("initial-status-feedback", msg || "", !!isError);
}

function applyInitialStatusToForm(data) {
  if (!data) { return; }
  INITIAL_STATUS_FIELDS.forEach(function (key) {
    const el = document.getElementById("init-" + key);
    if (!el) { return; }
    const v = data[key];
    el.value = (v === undefined || v === null) ? "" : String(v);
  });
}

async function loadInitialStatus(forceReload) {
  if (initialStatusState.isLoading) { return; }
  initialStatusState.isLoading = true;
  if (forceReload) {
    setInitialStatusFeedback("読み込み中...", false);
  }
  try {
    const { response, data } = await fetchJson("/api/initial-status");
    if (!response.ok || !data) {
      throw new Error("initial-status load failed: HTTP " + response.status);
    }
    initialStatusState.current = data;
    applyInitialStatusToForm(data);
    setInitialStatusFeedback(forceReload ? "読み込みました" : "", false);
  } catch (err) {
    console.error("loadInitialStatus error", err);
    setInitialStatusFeedback("読み込みに失敗しました", true);
  } finally {
    initialStatusState.isLoading = false;
  }
}

function collectInitialStatusForm() {
  const body = {};
  for (let i = 0; i < INITIAL_STATUS_FIELDS.length; i++) {
    const key = INITIAL_STATUS_FIELDS[i];
    const el = document.getElementById("init-" + key);
    if (!el) { continue; }
    const raw = el.value;
    if (raw === "" || raw === null || raw === undefined) {
      return { error: key + " が空です" };
    }
    const n = Number(raw);
    if (!Number.isFinite(n) || !Number.isInteger(n)) {
      return { error: key + " に整数を指定してください" };
    }
    // posX / posY は負値も許容（その他は 0 以上）
    if (key !== "posX" && key !== "posY" && n < 0) {
      return { error: key + " は 0 以上で指定してください" };
    }
    body[key] = n;
  }
  return { body: body };
}

async function submitInitialStatusForm(ev) {
  if (ev) { ev.preventDefault(); }
  const collected = collectInitialStatusForm();
  if (collected.error) {
    setInitialStatusFeedback(collected.error, true);
    return;
  }
  setInitialStatusFeedback("保存中...", false);
  try {
    const response = await fetch("/api/initial-status", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      credentials: "same-origin",
      body: JSON.stringify(collected.body)
    });
    const text = await response.text();
    let data = null;
    try { data = text ? JSON.parse(text) : null; } catch (_e) { data = null; }
    if (!response.ok || !data) {
      const reason = (data && data.error) ? data.error : ("HTTP " + response.status);
      setInitialStatusFeedback("保存に失敗しました: " + reason, true);
      return;
    }
    initialStatusState.current = data;
    applyInitialStatusToForm(data);
    setInitialStatusFeedback("保存しました", false);
  } catch (err) {
    console.error("submitInitialStatusForm error", err);
    setInitialStatusFeedback("通信エラーが発生しました", true);
  }
}

function setupInitialStatusView() {
  const reloadBtn = document.getElementById("initial-status-reload-btn");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", function () { loadInitialStatus(true); });
  }
  const form = document.getElementById("initial-status-form");
  if (form) {
    form.addEventListener("submit", submitInitialStatusForm);
  }
  const revertBtn = document.getElementById("initial-status-revert-btn");
  if (revertBtn) {
    revertBtn.addEventListener("click", function () {
      if (initialStatusState.current) {
        applyInitialStatusToForm(initialStatusState.current);
        setInitialStatusFeedback("フォームを前回値に戻しました", false);
      } else {
        loadInitialStatus(true);
      }
    });
  }
  // 初回自動読み込み（画面を開くタイミングに依らず、他ビューと同様ロード時に1回取得）
  loadInitialStatus(false);
}

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

// =============================================================================
// 会話イベント editor（共通コンポーネント）
//   /api/talk-events を読み書きし、CInfoTalkEvent の events[] を行単位で編集する。
//   外部 API: openTalkEventEditor({ talkEventId, onSaved })
//   - 一覧画面 (view="talk-events") からも、将来的にマップイベント / NPC から
//     data-talk-event ボタンでも呼べる想定。
// =============================================================================

// 会話イベントコマンド種別（CInfoTalkEventBase.h の enum と一致）
const TALK_EVENT_TYPE_NONE     = 0;
const TALK_EVENT_TYPE_PAGE     = 1;
const TALK_EVENT_TYPE_MSG      = 2;
const TALK_EVENT_TYPE_MENU     = 3;
const TALK_EVENT_TYPE_ADDSKILL = 4;

const TALK_EVENT_TYPE_LABELS = {
  0: "NONE（空）",
  1: "PAGE（ページ切替）",
  2: "MSG（メッセージ）",
  3: "MENU（選択肢）",
  4: "ADDSKILL（スキル追加）",
};

// ページ切替条件（CInfoTalkEventPAGE.h の enum と一致）
const TALK_EVENT_PAGE_COND_LABELS = {
  0: "条件なし",
  1: "アイテムあり",
  2: "アイテムなし",
};

const talkEventState = {
  loaded: false,
  isLoading: false,
  items: [],             // [{id, pageCount, eventCount}]
  selectedId: null,
  // editor 用
  editorOpen: false,
  editorTalkEventId: null,
  editorRows: [],        // {type, page, data, text, pageChgCondition, pageJump, menuItems:[{page,name}]}
  editorOnSaved: null,
};

function getTalkEventEls() {
  return {
    tableBody: document.getElementById("talk-event-table-body"),
    summary: document.getElementById("talk-event-summary"),
    feedback: document.getElementById("talk-event-feedback"),
    select: document.getElementById("talk-event-select"),
    reloadBtn: document.getElementById("talk-event-reload-btn"),
    newBtn: document.getElementById("talk-event-new-btn"),
    modal: document.getElementById("talk-event-modal"),
    modalId: document.getElementById("talk-event-modal-id"),
    modalFeedback: document.getElementById("talk-event-modal-feedback"),
    modalSummary: document.getElementById("talk-event-modal-summary"),
    rowsArea: document.getElementById("talk-event-rows-area"),
    rowCount: document.getElementById("talk-event-row-count"),
    addRowBtn: document.getElementById("talk-event-add-row-btn"),
    saveBtn: document.getElementById("talk-event-save-btn"),
  };
}

function setTalkEventFeedback(message, isError) {
  const els = getTalkEventEls();
  if (!els.feedback) { return; }
  els.feedback.textContent = message || "";
  els.feedback.className = "form-feedback" + (isError ? " form-feedback--error" : "");
}

function setTalkEventModalFeedback(message, isError) {
  const els = getTalkEventEls();
  if (!els.modalFeedback) { return; }
  els.modalFeedback.textContent = message || "";
  els.modalFeedback.style.color = isError ? "#f87171" : "";
}

/** 一覧を API から取得して表示 */
async function loadTalkEventList() {
  const els = getTalkEventEls();
  talkEventState.isLoading = true;
  if (els.summary) { els.summary.textContent = "読み込み中..."; }

  try {
    const { response, data } = await fetchJson("/api/talk-events");
    if (!response.ok || !data || !Array.isArray(data.talkEvents)) {
      throw new Error("HTTP " + response.status);
    }
    talkEventState.items = data.talkEvents.slice().sort(function (a, b) { return (a.id || 0) - (b.id || 0); });
    talkEventState.loaded = true;
    talkEventState.isLoading = false;
    if (els.summary) { els.summary.textContent = "会話イベント " + talkEventState.items.length + " 件"; }
    renderTalkEventSelect();
    renderTalkEventTable();
  } catch (err) {
    console.error("会話イベント一覧取得に失敗:", err);
    talkEventState.isLoading = false;
    if (els.summary) { els.summary.textContent = "取得失敗: " + err.message; }
    setTalkEventFeedback("取得に失敗しました: " + err.message, true);
  }
}

function renderTalkEventSelect() {
  const els = getTalkEventEls();
  if (!els.select) { return; }
  els.select.innerHTML = "";
  if (!talkEventState.items.length) {
    const opt = document.createElement("option");
    opt.value = "";
    opt.textContent = "（会話イベントなし）";
    els.select.appendChild(opt);
    return;
  }
  talkEventState.items.forEach(function (it) {
    const opt = document.createElement("option");
    opt.value = String(it.id);
    opt.textContent = "ID " + it.id + "（" + it.pageCount + "ページ / " + it.eventCount + "行）";
    els.select.appendChild(opt);
  });
}

function renderTalkEventTable() {
  const els = getTalkEventEls();
  if (!els.tableBody) { return; }
  els.tableBody.innerHTML = "";
  if (!talkEventState.items.length) {
    const tr = document.createElement("tr");
    tr.innerHTML = '<td colspan="4" class="empty-cell">会話イベントがありません</td>';
    els.tableBody.appendChild(tr);
    return;
  }
  talkEventState.items.forEach(function (it) {
    const tr = document.createElement("tr");
    tr.innerHTML =
      '<td>' + it.id + '</td>' +
      '<td>' + it.pageCount + '</td>' +
      '<td>' + it.eventCount + '</td>' +
      '<td>' +
        '<button type="button" class="btn btn-secondary" data-talk-event-edit="' + it.id + '">編集</button> ' +
        '<button type="button" class="btn btn-danger" data-talk-event-delete="' + it.id + '">削除</button>' +
      '</td>';
    els.tableBody.appendChild(tr);
  });
}

/** 削除（確認ダイアログ付き） */
async function deleteTalkEvent(id) {
  if (!confirm("会話イベント ID " + id + " を削除します。よろしいですか？")) { return; }
  try {
    const { response, data } = await fetchJson("/api/talk-events?id=" + id, { method: "DELETE" });
    if (!response.ok) {
      const msg = (data && data.error) ? data.error : "HTTP " + response.status;
      throw new Error(msg);
    }
    setTalkEventFeedback("削除しました: ID " + id, false);
    loadTalkEventList();
  } catch (err) {
    console.error("会話イベント削除失敗:", err);
    setTalkEventFeedback("削除失敗: " + err.message, true);
  }
}

// ---------------------------------------------------------------------------
// editor（モーダル）
// ---------------------------------------------------------------------------

/**
 * 会話イベント editor を開く（共通 API）
 * @param {Object} opts
 *   - talkEventId: number 必須
 *   - onSaved: (updated) => void 保存成功時に呼ばれる
 */
async function openTalkEventEditor(opts) {
  const options = opts || {};
  const talkEventId = Number(options.talkEventId);
  if (!Number.isFinite(talkEventId) || talkEventId <= 0) {
    alert("会話イベント ID が不正です");
    return;
  }
  talkEventState.editorTalkEventId = talkEventId;
  talkEventState.editorOnSaved = typeof options.onSaved === "function" ? options.onSaved : null;
  talkEventState.editorRows = [];

  const els = getTalkEventEls();
  if (!els.modal) { return; }
  if (els.modalId) { els.modalId.textContent = String(talkEventId); }
  setTalkEventModalFeedback("読み込み中...", false);

  els.modal.hidden = false;
  els.modal.setAttribute("aria-hidden", "false");
  talkEventState.editorOpen = true;

  // 既存データを取得（なければ空の新規として扱う）
  try {
    const { response, data } = await fetchJson("/api/talk-events?id=" + talkEventId);
    if (response.ok && data && Array.isArray(data.events)) {
      talkEventState.editorRows = data.events.map(normalizeTalkEventRow);
      setTalkEventModalFeedback("既存データ " + data.events.length + " 行を読み込みました", false);
    } else if (response.status === 404) {
      talkEventState.editorRows = [];
      setTalkEventModalFeedback("新規会話イベント（既存なし）", false);
    } else {
      throw new Error("HTTP " + response.status);
    }
  } catch (err) {
    console.error("会話イベント取得失敗:", err);
    setTalkEventModalFeedback("取得失敗: " + err.message, true);
  }

  renderTalkEventEditorRows();
}

function closeTalkEventEditor() {
  const els = getTalkEventEls();
  if (!els.modal) { return; }
  els.modal.hidden = true;
  els.modal.setAttribute("aria-hidden", "true");
  talkEventState.editorOpen = false;
  talkEventState.editorOnSaved = null;
}

/** サーバーから来た 1 行を editorRows 用に正規化する */
function normalizeTalkEventRow(ev) {
  return {
    type: Number(ev.type) || 0,
    page: Number(ev.page) || 0,
    data: Number(ev.data) || 0,
    text: String(ev.text || ""),
    pageChgCondition: Number(ev.pageChgCondition) || 0,
    pageJump: Number(ev.pageJump) || 0,
    menuItems: Array.isArray(ev.menuItems)
      ? ev.menuItems.map(function (m) { return { page: Number(m.page) || 0, name: String(m.name || "") }; })
      : [],
  };
}

/** 編集中の行一覧を DOM に描画 */
function renderTalkEventEditorRows() {
  const els = getTalkEventEls();
  if (!els.rowsArea) { return; }
  els.rowsArea.innerHTML = "";

  if (els.rowCount) { els.rowCount.textContent = String(talkEventState.editorRows.length); }
  if (els.modalSummary) {
    els.modalSummary.textContent = talkEventState.editorRows.length + " 行";
  }

  if (!talkEventState.editorRows.length) {
    const empty = document.createElement("p");
    empty.className = "field-note";
    empty.textContent = "行がありません。右上の「行を追加」で追加してください。";
    els.rowsArea.appendChild(empty);
    return;
  }

  talkEventState.editorRows.forEach(function (row, idx) {
    els.rowsArea.appendChild(buildTalkEventRowEl(row, idx));
  });
}

/** 1 行分の編集 UI を組み立てる */
function buildTalkEventRowEl(row, idx) {
  const wrap = document.createElement("div");
  wrap.className = "card";
  wrap.style.padding = "0.5rem";
  wrap.style.borderLeft = "3px solid #4b5563";

  // 種別プルダウン
  let typeOptions = "";
  Object.keys(TALK_EVENT_TYPE_LABELS).forEach(function (k) {
    const sel = (Number(k) === row.type) ? " selected" : "";
    typeOptions += '<option value="' + k + '"' + sel + '>' + TALK_EVENT_TYPE_LABELS[k] + '</option>';
  });

  // 共通ヘッダ（行番号 / 種別 / ページ / 操作）
  wrap.innerHTML =
    '<div style="display:flex; gap:0.5rem; align-items:center; flex-wrap:wrap;">' +
      '<strong style="min-width:3rem;">#' + idx + '</strong>' +
      '<label class="form-field"><span>種別</span><select data-talk-event-row-field="type">' + typeOptions + '</select></label>' +
      '<label class="form-field"><span>所属ページ</span><input type="number" data-talk-event-row-field="page" value="' + row.page + '"></label>' +
      '<div style="margin-left:auto; display:flex; gap:0.25rem;">' +
        '<button type="button" class="btn btn-secondary" data-talk-event-row-up="' + idx + '">↑</button>' +
        '<button type="button" class="btn btn-secondary" data-talk-event-row-down="' + idx + '">↓</button>' +
        '<button type="button" class="btn btn-danger" data-talk-event-row-delete="' + idx + '">削除</button>' +
      '</div>' +
    '</div>' +
    '<div data-talk-event-row-body style="margin-top:0.4rem;"></div>';

  const body = wrap.querySelector("[data-talk-event-row-body]");
  renderTalkEventRowBody(body, row, idx);

  // 種別変更で body を更新
  const typeSel = wrap.querySelector('[data-talk-event-row-field="type"]');
  if (typeSel) {
    typeSel.addEventListener("change", function () {
      row.type = Number(typeSel.value);
      // 種別切替時に他派生フィールドは保持したまま UI だけ更新
      renderTalkEventRowBody(body, row, idx);
    });
  }

  const pageInp = wrap.querySelector('[data-talk-event-row-field="page"]');
  if (pageInp) {
    pageInp.addEventListener("input", function () { row.page = parseInt(pageInp.value || "0", 10) || 0; });
  }

  return wrap;
}

/** 行本体（種別依存フィールド）を body 要素に描画 */
function renderTalkEventRowBody(body, row, idx) {
  if (!body) { return; }
  body.innerHTML = "";

  if (row.type === TALK_EVENT_TYPE_NONE) {
    const p = document.createElement("p");
    p.className = "field-note";
    p.textContent = "固有フィールドなし（NONE）";
    body.appendChild(p);
    return;
  }

  if (row.type === TALK_EVENT_TYPE_MSG) {
    // メッセージ本文（複数行 textarea）
    const label = document.createElement("label");
    label.className = "form-field";
    label.innerHTML =
      '<span>メッセージ本文</span>' +
      '<textarea rows="3" data-talk-event-row-field="text" style="width:100%;"></textarea>';
    body.appendChild(label);
    const ta = label.querySelector("textarea");
    ta.value = row.text || "";
    ta.addEventListener("input", function () { row.text = ta.value; });
    return;
  }

  if (row.type === TALK_EVENT_TYPE_ADDSKILL) {
    // スキル ID（m_dwData）
    const label = document.createElement("label");
    label.className = "form-field";
    label.innerHTML =
      '<span>スキル ID</span>' +
      '<input type="number" data-talk-event-row-field="data" min="0">';
    body.appendChild(label);
    const inp = label.querySelector("input");
    inp.value = String(row.data || 0);
    inp.addEventListener("input", function () { row.data = parseInt(inp.value || "0", 10) || 0; });
    return;
  }

  if (row.type === TALK_EVENT_TYPE_PAGE) {
    // 条件 + ジャンプ先ページ
    let condOptions = "";
    Object.keys(TALK_EVENT_PAGE_COND_LABELS).forEach(function (k) {
      const sel = (Number(k) === row.pageChgCondition) ? " selected" : "";
      condOptions += '<option value="' + k + '"' + sel + '>' + TALK_EVENT_PAGE_COND_LABELS[k] + '</option>';
    });
    body.innerHTML =
      '<label class="form-field"><span>切替条件</span><select data-talk-event-row-field="pageChgCondition">' + condOptions + '</select></label>' +
      '<label class="form-field"><span>ジャンプ先ページ</span><input type="number" data-talk-event-row-field="pageJump" value="' + (row.pageJump || 0) + '"></label>' +
      '<label class="form-field"><span>条件用データ (アイテム ID 等)</span><input type="number" data-talk-event-row-field="data" value="' + (row.data || 0) + '"></label>';
    const condSel = body.querySelector('[data-talk-event-row-field="pageChgCondition"]');
    const jumpInp = body.querySelector('[data-talk-event-row-field="pageJump"]');
    const dataInp = body.querySelector('[data-talk-event-row-field="data"]');
    if (condSel) { condSel.addEventListener("change", function () { row.pageChgCondition = Number(condSel.value); }); }
    if (jumpInp) { jumpInp.addEventListener("input", function () { row.pageJump = parseInt(jumpInp.value || "0", 10) || 0; }); }
    if (dataInp) { dataInp.addEventListener("input", function () { row.data = parseInt(dataInp.value || "0", 10) || 0; }); }
    return;
  }

  if (row.type === TALK_EVENT_TYPE_MENU) {
    // 選択肢一覧
    const title = document.createElement("p");
    title.className = "field-note";
    title.textContent = "選択肢（ジャンプ先ページ番号 + 項目名）";
    body.appendChild(title);

    const listDiv = document.createElement("div");
    listDiv.style.display = "flex";
    listDiv.style.flexDirection = "column";
    listDiv.style.gap = "0.25rem";
    body.appendChild(listDiv);

    function renderMenuItems() {
      listDiv.innerHTML = "";
      row.menuItems.forEach(function (mi, midx) {
        const itemWrap = document.createElement("div");
        itemWrap.style.display = "flex";
        itemWrap.style.gap = "0.5rem";
        itemWrap.style.alignItems = "center";
        itemWrap.innerHTML =
          '<span>#' + midx + '</span>' +
          '<label class="form-field"><span>ページ</span><input type="number" value="' + mi.page + '" style="width:6rem;"></label>' +
          '<label class="form-field" style="flex:1;"><span>項目名</span><input type="text" value=""></label>' +
          '<button type="button" class="btn btn-danger">削除</button>';
        const inps = itemWrap.querySelectorAll("input");
        inps[0].addEventListener("input", function () { mi.page = parseInt(inps[0].value || "0", 10) || 0; });
        inps[1].value = mi.name || "";
        inps[1].addEventListener("input", function () { mi.name = inps[1].value; });
        const delBtn = itemWrap.querySelector("button");
        delBtn.addEventListener("click", function () {
          row.menuItems.splice(midx, 1);
          renderMenuItems();
        });
        listDiv.appendChild(itemWrap);
      });
    }

    renderMenuItems();

    const addBtn = document.createElement("button");
    addBtn.type = "button";
    addBtn.className = "btn btn-secondary";
    addBtn.textContent = "+ 選択肢を追加";
    addBtn.style.marginTop = "0.25rem";
    addBtn.addEventListener("click", function () {
      row.menuItems.push({ page: 0, name: "" });
      renderMenuItems();
    });
    body.appendChild(addBtn);
    return;
  }
}

/** editor の全操作を 1 箇所に集約（イベント委譲） */
function bindTalkEventEditor() {
  const els = getTalkEventEls();

  // 一覧画面のボタン（再読み込み / 新規 / 編集 / 削除）
  if (els.reloadBtn) {
    els.reloadBtn.addEventListener("click", function () { loadTalkEventList(); });
  }
  if (els.newBtn) {
    els.newBtn.addEventListener("click", function () {
      const raw = prompt("新規会話イベント ID (キャラクター ID) を入力してください。");
      if (!raw) { return; }
      const id = parseInt(raw, 10);
      if (!Number.isFinite(id) || id <= 0) {
        alert("不正な ID です");
        return;
      }
      openTalkEventEditor({
        talkEventId: id,
        onSaved: function () { loadTalkEventList(); }
      });
    });
  }
  // 一覧テーブルの編集 / 削除（委譲）
  const tableBody = els.tableBody;
  if (tableBody) {
    tableBody.addEventListener("click", function (ev) {
      const editBtn = ev.target.closest("[data-talk-event-edit]");
      if (editBtn) {
        const id = Number(editBtn.getAttribute("data-talk-event-edit"));
        openTalkEventEditor({
          talkEventId: id,
          onSaved: function () { loadTalkEventList(); }
        });
        return;
      }
      const delBtn = ev.target.closest("[data-talk-event-delete]");
      if (delBtn) {
        const id = Number(delBtn.getAttribute("data-talk-event-delete"));
        deleteTalkEvent(id);
      }
    });
  }

  // モーダル dismiss
  if (els.modal) {
    const dismissEls = els.modal.querySelectorAll("[data-talk-event-dismiss]");
    dismissEls.forEach(function (el) { el.addEventListener("click", closeTalkEventEditor); });
  }

  // 行追加
  if (els.addRowBtn) {
    els.addRowBtn.addEventListener("click", function () {
      talkEventState.editorRows.push(normalizeTalkEventRow({ type: TALK_EVENT_TYPE_MSG }));
      renderTalkEventEditorRows();
    });
  }

  // 行本体のイベント委譲（上下移動 / 削除）
  if (els.rowsArea) {
    els.rowsArea.addEventListener("click", function (ev) {
      const up = ev.target.closest("[data-talk-event-row-up]");
      if (up) {
        const i = Number(up.getAttribute("data-talk-event-row-up"));
        if (i > 0) {
          const tmp = talkEventState.editorRows[i - 1];
          talkEventState.editorRows[i - 1] = talkEventState.editorRows[i];
          talkEventState.editorRows[i] = tmp;
          renderTalkEventEditorRows();
        }
        return;
      }
      const dn = ev.target.closest("[data-talk-event-row-down]");
      if (dn) {
        const i = Number(dn.getAttribute("data-talk-event-row-down"));
        if (i < talkEventState.editorRows.length - 1) {
          const tmp = talkEventState.editorRows[i + 1];
          talkEventState.editorRows[i + 1] = talkEventState.editorRows[i];
          talkEventState.editorRows[i] = tmp;
          renderTalkEventEditorRows();
        }
        return;
      }
      const del = ev.target.closest("[data-talk-event-row-delete]");
      if (del) {
        const i = Number(del.getAttribute("data-talk-event-row-delete"));
        talkEventState.editorRows.splice(i, 1);
        renderTalkEventEditorRows();
      }
    });
  }

  // 保存
  if (els.saveBtn) {
    els.saveBtn.addEventListener("click", saveTalkEventEditor);
  }

  // ESC で閉じる
  document.addEventListener("keydown", function (ev) {
    if (!talkEventState.editorOpen) { return; }
    if (ev.key === "Escape") { closeTalkEventEditor(); }
  });

  // data-talk-event 属性を持つ任意のボタンから editor を開けるようにする
  //   <button data-talk-event="<id>"> または data-talk-event-target="<input id>">
  //   のどちらでも動く（後者は input.value を ID として使う）
  document.addEventListener("click", function (ev) {
    const btn = ev.target.closest("button[data-talk-event]");
    if (!btn) { return; }
    let id = Number(btn.getAttribute("data-talk-event"));
    if (!Number.isFinite(id) || id <= 0) {
      const targetId = btn.getAttribute("data-talk-event-target");
      if (targetId) {
        const inp = document.getElementById(targetId);
        if (inp) { id = Number(inp.value); }
      }
    }
    if (!Number.isFinite(id) || id <= 0) {
      alert("会話イベント ID が未指定です");
      return;
    }
    openTalkEventEditor({ talkEventId: id });
  });
}

/** 保存ボタン: 現在の editorRows を PUT /api/talk-events で送信 */
async function saveTalkEventEditor() {
  if (!talkEventState.editorOpen) { return; }
  const id = talkEventState.editorTalkEventId;
  if (!Number.isFinite(id) || id <= 0) { return; }

  const payload = {
    id: id,
    events: talkEventState.editorRows.map(function (r) {
      const base = {
        type: r.type,
        page: r.page,
        data: r.data,
        text: r.text,
      };
      if (r.type === TALK_EVENT_TYPE_PAGE) {
        base.pageChgCondition = r.pageChgCondition;
        base.pageJump = r.pageJump;
      }
      if (r.type === TALK_EVENT_TYPE_MENU) {
        base.menuItems = r.menuItems.slice();
      }
      return base;
    }),
  };

  setTalkEventModalFeedback("保存中...", false);
  try {
    const { response, data } = await fetchJson("/api/talk-events", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    if (!response.ok) {
      const msg = (data && data.error) ? data.error : "HTTP " + response.status;
      throw new Error(msg);
    }
    setTalkEventModalFeedback("保存しました", false);
    const cb = talkEventState.editorOnSaved;
    // 少し間を置いて閉じる（メッセージを読んでもらう）
    setTimeout(function () {
      closeTalkEventEditor();
      if (cb) { cb(data); }
    }, 400);
  } catch (err) {
    console.error("会話イベント保存失敗:", err);
    setTalkEventModalFeedback("保存失敗: " + err.message, true);
  }
}

// TODO: 未実装機能（必要になり次第追加）
//   - ADDSKILL でのスキル picker 連携（現状は ID 直接入力）
//   - PAGE 条件の「アイテムあり/なし」で data にアイテム picker を出す
//   - MSG テキストのプレビュー / 改行コード整形
//   - ページ単位でのグルーピング表示（現状は events[] 線形リスト）
//   - 行の Undo / 変更差分表示
//   - コミット前バリデーション（MENU のページ番号が存在するか等）

// 初期化（picker と同じく DOMContentLoaded / 即時の両対応）
if (document.readyState === "loading") {
  document.addEventListener("DOMContentLoaded", bindTalkEventEditor);
} else {
  bindTalkEventEditor();
}

// 管理画面全体で右クリックコンテキストメニューを抑止する
document.addEventListener("contextmenu", function (ev) {
  ev.preventDefault();
});

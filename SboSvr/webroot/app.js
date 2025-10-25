const serverStatusEl = document.getElementById("server-status");
const onlineEl = document.getElementById("server-online");
const versionEl = document.getElementById("server-version");
const cpuEl = document.getElementById("server-cpu");
const memoryEl = document.getElementById("server-memory");
const latencyEl = document.getElementById("server-latency");
const updatedEl = document.getElementById("server-updated");
const reloadButton = document.getElementById("reload-server");

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
const mapObjectNameInput = document.getElementById("map-object-name");
const mapObjectTypeSelect = document.getElementById("map-object-type");
const mapObjectLayerSelect = document.getElementById("map-object-layer");
const mapObjectXInput = document.getElementById("map-object-x");
const mapObjectYInput = document.getElementById("map-object-y");
const mapObjectRespawnInput = document.getElementById("map-object-respawn");
const mapObjectNotesInput = document.getElementById("map-object-notes");
const mapObjectClearButton = document.getElementById("map-object-clear");
const mapObjectDeleteButton = document.getElementById("map-object-delete");
const mapObjectSaveButton = document.getElementById("map-object-save");
const mapObjectFeedbackEl = document.getElementById("map-object-feedback");

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
  loadError: null
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

const MAP_PARTS_GALLERY_SCALE = 4;
const MAP_PARTS_DETAIL_SCALE = 6;

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
    objects
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



const DEFAULT_ROUTE = "server-dashboard";
const views = document.querySelectorAll("[data-view]");
const navLinks = document.querySelectorAll("[data-route]");
let currentRoute = null;

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

function initializeMapObjectView() {
  if (!mapObjectMapSelect) {
    return;
  }
  populateMapObjectTypeSelect(mapObjectFilterSelect, true);
  populateMapObjectTypeSelect(mapObjectTypeSelect, false);
  enforceMapObjectReadOnly();
  populateMapOptions();
  if (mapObjectFilterSelect) {
    mapObjectFilterSelect.value = mapObjectState.filterType;
    mapObjectFilterSelect.disabled = true;
  }

  applyObjectToForm(null, getSelectedMapData());
  renderMapComponents();
  setMapObjectFeedback("現在は閲覧専用モードです。", null);
  loadMapObjectData(false);
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

  mapObjectGridEl.style.gridTemplateColumns = `repeat(${Math.max(map.width, 1)}, minmax(28px, 1fr))`;

  const occupantMap = new Map();
  map.objects.forEach((object) => {
    occupantMap.set(`${object.x},${object.y}`, object);
  });

  const preview = mapObjectState.previewSelection;

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
  if (!mapObjectForm) {
    return;
  }
  if (mapObjectNameInput) {
    mapObjectNameInput.readOnly = true;
  }
  if (mapObjectTypeSelect) {
    mapObjectTypeSelect.disabled = true;
  }
  if (mapObjectLayerSelect) {
    mapObjectLayerSelect.disabled = true;
  }
  if (mapObjectXInput) {
    mapObjectXInput.readOnly = true;
  }
  if (mapObjectYInput) {
    mapObjectYInput.readOnly = true;
  }
  if (mapObjectRespawnInput) {
    mapObjectRespawnInput.disabled = true;
  }
  if (mapObjectNotesInput) {
    mapObjectNotesInput.readOnly = true;
  }
  if (mapObjectSaveButton) {
    mapObjectSaveButton.disabled = true;
    mapObjectSaveButton.title = "現在は閲覧専用です";
  }
  if (mapObjectDeleteButton) {
    mapObjectDeleteButton.disabled = true;
  }
}

function applyObjectToForm(object, map) {
  if (!mapObjectForm) {
    return;
  }

  const typeDefault = MAP_OBJECT_TYPE_OPTIONS[0]?.value || "";
  const selectedMap = map || getSelectedMapData();
  const maxX = selectedMap ? selectedMap.width - 1 : 0;
  const maxY = selectedMap ? selectedMap.height - 1 : 0;

  if (object) {
    if (mapObjectNameInput) {
      mapObjectNameInput.value = object.name || "";
    }
    if (mapObjectTypeSelect) {
      mapObjectTypeSelect.value = object.type || typeDefault;
    }
    if (mapObjectLayerSelect) {
      mapObjectLayerSelect.value = mapObjectLayerSelect.options.length ? mapObjectLayerSelect.options[0].value : "ground";
      mapObjectLayerSelect.title = object.attributeHex ? `属性: ${object.attributeHex}` : "属性情報なし";
    }
    if (mapObjectXInput) {
      mapObjectXInput.value = clampCoordinate(object.x, maxX);
    }
    if (mapObjectYInput) {
      mapObjectYInput.value = clampCoordinate(object.y, maxY);
    }
    if (mapObjectRespawnInput) {
      mapObjectRespawnInput.value = "";
    }
    if (mapObjectNotesInput) {
      mapObjectNotesInput.value = object.notes || "";
    }
    const idLabels = [];
    if (object.placementId !== null) {
      idLabels.push(`配置ID: ${object.placementId}`);
    }
    if (object.objectIdHex) {
      idLabels.push(`オブジェクトID: ${object.objectIdHex}`);
    }
    mapObjectSelectedLabel.textContent = `選択中: ${object.name}${idLabels.length ? ` (${idLabels.join(" / ")})` : ""}`;
    if (mapObjectDeleteButton) {
      mapObjectDeleteButton.disabled = true;
    }
  } else {
    const preview = mapObjectState.previewSelection;
    if (mapObjectNameInput) {
      mapObjectNameInput.value = "";
    }
    if (mapObjectTypeSelect) {
      mapObjectTypeSelect.value = mapObjectTypeSelect.options.length ? mapObjectTypeSelect.options[0].value : typeDefault;
    }
    if (mapObjectLayerSelect) {
      mapObjectLayerSelect.value = mapObjectLayerSelect.options.length ? mapObjectLayerSelect.options[0].value : "ground";
      mapObjectLayerSelect.title = "属性情報なし";
    }
    if (mapObjectXInput) {
      mapObjectXInput.value = clampCoordinate(preview ? preview.x : 0, maxX);
    }
    if (mapObjectYInput) {
      mapObjectYInput.value = clampCoordinate(preview ? preview.y : 0, maxY);
    }
    if (mapObjectRespawnInput) {
      mapObjectRespawnInput.value = "";
    }
    if (mapObjectNotesInput) {
      mapObjectNotesInput.value = "";
    }
    mapObjectSelectedLabel.textContent = "選択中: 未選択 (閲覧のみ)";
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
  setMapObjectFeedback(`座標 (${x}, ${y}) を選択しました (閲覧のみ)`, null);
}

function handleMapObjectFormSubmit(event) {
  event.preventDefault();
  const map = getSelectedMapData();
  if (!map) {
    setMapObjectFeedback("マップを選択してください", "error");
    return;
  }
  setMapObjectFeedback("現在は閲覧専用モードのため保存できません。", "error");
}

function handleMapObjectClear() {
  const map = getSelectedMapData();
  mapObjectState.selectedObjectId = null;
  mapObjectState.previewSelection = null;
  applyObjectToForm(null, map);
  renderMapComponents();
  setMapObjectFeedback("選択をクリアしました", null);
}

function handleMapObjectDelete() {
  const map = getSelectedMapData();
  if (!map) {
    setMapObjectFeedback("マップを選択してください", "error");
    return;
  }
  if (!mapObjectState.selectedObjectId) {
    setMapObjectFeedback("削除するオブジェクトを選択してください", "error");
    return;
  }
  setMapObjectFeedback("現在は閲覧専用モードのため削除できません。", "error");
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

  if (normalized === "map-objects") {
    if (options.forceReload) {
      loadMapObjectData(true);
    } else if (!mapObjectState.maps.length && !mapObjectState.isLoading && !mapObjectState.loadError) {
      loadMapObjectData(false);
    }
  }

  if (normalized === "map-parts") {
    if (options.forceReload) {
      loadMapPartsData(true);
    } else if (!mapPartsState.parts.length && !mapPartsState.isLoading && !mapPartsState.loadError) {
      loadMapPartsData(false);
    }
  }

  currentRoute = normalized;
}

window.addEventListener("load", () => {
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

  if (mapPartsGallery) {
    initializeMapPartsView();
    if (mapPartsFlagFilter) {
      mapPartsFlagFilter.addEventListener("change", handleMapPartsFlagFilterChange);
    }
  }
});

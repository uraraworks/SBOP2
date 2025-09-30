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
const mapObjectFeedbackEl = document.getElementById("map-object-feedback");

const POLL_INTERVAL_MS = 30000;
let serverTimerId = null;
let cachedRoles = [];

const MAP_OBJECT_TYPE_OPTIONS = [
  { value: "interactable", label: "インタラクト" },
  { value: "trigger", label: "トリガー" },
  { value: "decoration", label: "装飾" },
  { value: "spawn", label: "スポーン" }
];

const mockMapObjectDataset = [
  {
    id: "fld_001",
    name: "初心者の草原",
    width: 18,
    height: 12,
    weather: "晴れ",
    recommendedLevel: "1-5",
    safeArea: true,
    description: "チュートリアルとイベントガイド用に利用するフィールドマップです。",
    objects: [
      {
        id: "fld_001_obj_001",
        name: "復帰ポータル",
        type: "interactable",
        layer: "interaction",
        x: 3,
        y: 5,
        respawn: 0,
        notes: "クリックで拠点に戻る"
      },
      {
        id: "fld_001_obj_002",
        name: "補給商人ソラ",
        type: "spawn",
        layer: "ground",
        x: 10,
        y: 6,
        respawn: 600,
        notes: "夜間は出現しない"
      },
      {
        id: "fld_001_obj_003",
        name: "古びた宝箱",
        type: "trigger",
        layer: "ground",
        x: 14,
        y: 2,
        respawn: 1800,
        notes: "チュートリアル完了後に開封可能"
      }
    ]
  },
  {
    id: "dng_010",
    name: "忘却の洞窟 B1",
    width: 14,
    height: 10,
    weather: "地下（暗闇）",
    recommendedLevel: "25-30",
    safeArea: false,
    description: "中盤ダンジョンの入り口層。トリガー系の罠が多数配置されます。",
    objects: [
      {
        id: "dng_010_obj_004",
        name: "起動装置:封印門",
        type: "trigger",
        layer: "interaction",
        x: 6,
        y: 4,
        respawn: 0,
        notes: "対応スイッチを押すまで開かない"
      },
      {
        id: "dng_010_obj_005",
        name: "採掘ポイント",
        type: "interactable",
        layer: "ground",
        x: 2,
        y: 8,
        respawn: 900,
        notes: "鉱石を採取すると 15 分後に復活"
      },
      {
        id: "dng_010_obj_006",
        name: "監視スプライト",
        type: "decoration",
        layer: "sky",
        x: 11,
        y: 3,
        respawn: null,
        notes: "エフェクト用の飾り"
      }
    ]
  },
  {
    id: "tow_201",
    name: "天空庭園",
    width: 16,
    height: 12,
    weather: "快晴",
    recommendedLevel: "40",
    safeArea: true,
    description: "高レベル帯の休憩エリア。季節イベントの展示物を配置します。",
    objects: [
      {
        id: "tow_201_obj_007",
        name: "祝福の像",
        type: "decoration",
        layer: "ground",
        x: 7,
        y: 5,
        respawn: null,
        notes: "シーズン限定の装飾"
      },
      {
        id: "tow_201_obj_008",
        name: "転送装置",
        type: "interactable",
        layer: "interaction",
        x: 4,
        y: 9,
        respawn: 0,
        notes: "首都へ移動"
      },
      {
        id: "tow_201_obj_009",
        name: "イベント案内人リナ",
        type: "spawn",
        layer: "ground",
        x: 12,
        y: 7,
        respawn: 300,
        notes: "条件: イベントフラグ ON"
      }
    ]
  }
];

const mapObjectState = {
  maps: mockMapObjectDataset,
  selectedMapId: mockMapObjectDataset.length ? mockMapObjectDataset[0].id : null,
  selectedObjectId: null,
  filterType: "all",
  previewSelection: null
};



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
  populateMapOptions();
  populateMapObjectTypeSelect(mapObjectFilterSelect, true);
  populateMapObjectTypeSelect(mapObjectTypeSelect, false);

  if (mapObjectState.selectedMapId) {
    mapObjectMapSelect.value = mapObjectState.selectedMapId;
  } else if (mapObjectMapSelect.options.length) {
    mapObjectState.selectedMapId = mapObjectMapSelect.options[0].value;
  }

  if (mapObjectFilterSelect) {
    mapObjectFilterSelect.value = mapObjectState.filterType;
  }

  applyObjectToForm(null, getSelectedMapData());
  renderMapComponents();
  setMapObjectFeedback("", null);
}

function populateMapOptions() {
  if (!mapObjectMapSelect) {
    return;
  }
  mapObjectMapSelect.innerHTML = "";
  if (!mapObjectState.maps.length) {
    const option = document.createElement("option");
    option.value = "";
    option.textContent = "マップデータなし";
    mapObjectMapSelect.appendChild(option);
    return;
  }
  mapObjectState.maps.forEach((map) => {
    const option = document.createElement("option");
    option.value = map.id;
    option.textContent = `${map.name} (ID: ${map.id})`;
    mapObjectMapSelect.appendChild(option);
  });
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
  MAP_OBJECT_TYPE_OPTIONS.forEach((item, index) => {
    const option = document.createElement("option");
    option.value = item.value;
    option.textContent = item.label;
    if (!includeAll && index === 0) {
      option.selected = true;
    }
    select.appendChild(option);
  });
}

function getSelectedMapData() {
  return mapObjectState.maps.find((map) => map.id === mapObjectState.selectedMapId) || null;
}

function getMapObjectTypeLabel(type) {
  const found = MAP_OBJECT_TYPE_OPTIONS.find((item) => item.value === type);
  return found ? found.label : type;
}

function formatRespawnValue(value) {
  if (value === null || value === undefined) {
    return "-";
  }
  if (value === 0) {
    return "即時";
  }
  if (!Number.isFinite(value) || value < 0) {
    return "-";
  }
  if (value % 60 === 0) {
    const minutes = value / 60;
    return `${minutes}分`;
  }
  return `${value}秒`;
}

function renderMapSummary(map) {
  if (!mapObjectSummaryEl) {
    return;
  }
  if (!map) {
    mapObjectSummaryEl.textContent = "管理対象のマップが選択されていません";
    mapObjectSummaryEl.title = "";
    return;
  }
  const safety = map.safeArea ? "安全エリア" : "PvP 可能";
  mapObjectSummaryEl.textContent = `${map.name} / ID: ${map.id} / サイズ: ${map.width}×${map.height} / ${safety} / 推奨レベル: ${map.recommendedLevel} / オブジェクト数: ${map.objects.length}`;
  mapObjectSummaryEl.title = map.description || map.name;
}

function renderMapObjectTable(map) {
  if (!mapObjectTableBody) {
    return;
  }
  mapObjectTableBody.innerHTML = "";
  if (!map) {
    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = 6;
    cell.textContent = "マップが選択されていません";
    row.appendChild(cell);
    mapObjectTableBody.appendChild(row);
    return;
  }

  const filtered = map.objects.filter((object) => {
    if (mapObjectState.filterType === "all") {
      return true;
    }
    return object.type === mapObjectState.filterType;
  });

  if (!filtered.length) {
    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = 6;
    cell.textContent = "表示できるオブジェクトがありません";
    row.appendChild(cell);
    mapObjectTableBody.appendChild(row);
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

      const nameCell = document.createElement("td");
      nameCell.textContent = object.name;
      const typeCell = document.createElement("td");
      typeCell.textContent = getMapObjectTypeLabel(object.type);
      const layerCell = document.createElement("td");
      layerCell.textContent = object.layer;
      const xCell = document.createElement("td");
      xCell.textContent = object.x.toString();
      const yCell = document.createElement("td");
      yCell.textContent = object.y.toString();
      const respawnCell = document.createElement("td");
      respawnCell.textContent = formatRespawnValue(object.respawn);

      row.appendChild(nameCell);
      row.appendChild(typeCell);
      row.appendChild(layerCell);
      row.appendChild(xCell);
      row.appendChild(yCell);
      row.appendChild(respawnCell);

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
  if (!map) {
    const empty = document.createElement("div");
    empty.className = "empty-message";
    empty.textContent = "マップが選択されていません";
    mapObjectGridEl.style.gridTemplateColumns = "repeat(1, minmax(80px, 1fr))";
    mapObjectGridEl.appendChild(empty);
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
        cellButton.title = `${occupant.name} (${getMapObjectTypeLabel(occupant.type)})`;
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

function applyObjectToForm(object, map) {
  if (!mapObjectForm) {
    return;
  }

  const typeDefault = MAP_OBJECT_TYPE_OPTIONS[0]?.value || "";
  const selectedMap = map || getSelectedMapData();
  const maxX = selectedMap ? selectedMap.width - 1 : 0;
  const maxY = selectedMap ? selectedMap.height - 1 : 0;

  if (object) {
    mapObjectNameInput.value = object.name || "";
    mapObjectTypeSelect.value = object.type || typeDefault;
    mapObjectLayerSelect.value = object.layer || "ground";
    mapObjectXInput.value = clampCoordinate(object.x, maxX);
    mapObjectYInput.value = clampCoordinate(object.y, maxY);
    mapObjectRespawnInput.value = object.respawn === null || object.respawn === undefined ? "" : object.respawn;
    mapObjectNotesInput.value = object.notes || "";
    mapObjectSelectedLabel.textContent = `選択中: ${object.name} (ID: ${object.id})`;
    if (mapObjectDeleteButton) {
      mapObjectDeleteButton.disabled = false;
    }
  } else {
    const preview = mapObjectState.previewSelection;
    mapObjectNameInput.value = "";
    mapObjectTypeSelect.value = mapObjectTypeSelect.options.length ? mapObjectTypeSelect.options[0].value : typeDefault;
    mapObjectLayerSelect.value = "ground";
    mapObjectXInput.value = clampCoordinate(preview ? preview.x : 0, maxX);
    mapObjectYInput.value = clampCoordinate(preview ? preview.y : 0, maxY);
    mapObjectRespawnInput.value = "";
    mapObjectNotesInput.value = "";
    mapObjectSelectedLabel.textContent = "選択中: 新規配置";
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
  setMapObjectFeedback(`座標 (${x}, ${y}) を選択しました`, null);
}

function generateMapObjectId(mapId) {
  const random = Math.floor(Math.random() * 46656).toString(36).padStart(3, "0");
  const stamp = Date.now().toString(36).slice(-4);
  return `${mapId}_${stamp}${random}`;
}

function handleMapObjectFormSubmit(event) {
  event.preventDefault();
  const map = getSelectedMapData();
  if (!map) {
    setMapObjectFeedback("マップを選択してください", "error");
    return;
  }

  const name = mapObjectNameInput.value.trim();
  const type = mapObjectTypeSelect.value;
  const layer = mapObjectLayerSelect.value;
  const x = Number(mapObjectXInput.value);
  const y = Number(mapObjectYInput.value);
  const respawnRaw = mapObjectRespawnInput.value.trim();
  const respawn = respawnRaw === "" ? null : Number(respawnRaw);
  const notes = mapObjectNotesInput.value.trim();

  if (!name) {
    setMapObjectFeedback("表示名を入力してください", "error");
    return;
  }
  if (Number.isNaN(x) || Number.isNaN(y)) {
    setMapObjectFeedback("座標には数値を入力してください", "error");
    return;
  }
  if (x < 0 || y < 0 || x >= map.width || y >= map.height) {
    setMapObjectFeedback("座標がマップの範囲外です", "error");
    return;
  }
  if (respawn !== null && (Number.isNaN(respawn) || respawn < 0)) {
    setMapObjectFeedback("リスポーン時間が不正です", "error");
    return;
  }

  const existingIndex = map.objects.findIndex((item) => item.id === mapObjectState.selectedObjectId);

  if (existingIndex >= 0) {
    const target = map.objects[existingIndex];
    target.name = name;
    target.type = type;
    target.layer = layer;
    target.x = x;
    target.y = y;
    target.respawn = respawn;
    target.notes = notes;
    mapObjectState.previewSelection = { x, y };
    setMapObjectFeedback("オブジェクト情報を更新しました", "success");
  } else {
    const newId = generateMapObjectId(map.id);
    map.objects.push({
      id: newId,
      name,
      type,
      layer,
      x,
      y,
      respawn,
      notes
    });
    mapObjectState.selectedObjectId = newId;
    mapObjectState.previewSelection = { x, y };
    setMapObjectFeedback("新しいオブジェクトを配置しました", "success");
  }

  renderMapComponents();
  if (mapObjectState.selectedObjectId) {
    selectMapObject(mapObjectState.selectedObjectId);
  } else {
    applyObjectToForm(null, map);
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
  const index = map.objects.findIndex((item) => item.id === mapObjectState.selectedObjectId);
  if (index === -1) {
    setMapObjectFeedback("対象のオブジェクトが見つかりません", "error");
    return;
  }
  const removed = map.objects.splice(index, 1)[0];
  mapObjectState.selectedObjectId = null;
  mapObjectState.previewSelection = null;
  applyObjectToForm(null, map);
  renderMapComponents();
  setMapObjectFeedback(`オブジェクト「${removed.name}」を削除しました`, "success");
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
});

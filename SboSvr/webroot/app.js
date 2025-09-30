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

const mapPartsCategorySelect = document.getElementById("map-parts-category");
const mapPartsSearchInput = document.getElementById("map-parts-search");
const mapPartsGridEl = document.getElementById("map-parts-grid");
const mapPartsSummaryEl = document.getElementById("map-parts-summary");
const mapPartsReloadButton = document.getElementById("map-parts-reload");
const mapPartsPreviewCanvas = document.getElementById("map-parts-preview-canvas");
const mapPartsPlaceholderEl = document.getElementById("map-parts-placeholder");
const mapPartsPreviewTitle = document.getElementById("map-parts-preview-title");
const mapPartsPreviewMeta = document.getElementById("map-parts-preview-meta");
const mapPartsPreviewNotes = document.getElementById("map-parts-preview-notes");
const mapPartsTagsContainer = document.getElementById("map-parts-tags");
const mapPartsDetailCategory = document.getElementById("map-parts-detail-category");
const mapPartsDetailSize = document.getElementById("map-parts-detail-size");
const mapPartsDetailAttribute = document.getElementById("map-parts-detail-attribute");
const mapPartsDetailUpdated = document.getElementById("map-parts-detail-updated");
const mapPartsClearButton = document.getElementById("map-parts-clear");
const mapPartsApplyButton = document.getElementById("map-parts-apply");
const mapPartsFeedbackEl = document.getElementById("map-parts-feedback");

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

const POLL_INTERVAL_MS = 30000;
let serverTimerId = null;
let cachedRoles = [];

const MAP_PARTS_DEFAULT_CATEGORY = "all";

const MAP_OBJECT_TYPE_OPTIONS = [
  { value: "collision", label: "当たり判定あり" },
  { value: "no-collision", label: "当たり判定なし" }
];

const mapPartsState = {
  categories: [],
  parts: [],
  filteredParts: [],
  selectedCategoryId: MAP_PARTS_DEFAULT_CATEGORY,
  searchQuery: "",
  selectedPartId: null,
  isLoading: false,
  loadError: null,
  loadWarning: null,
  usingSampleData: false,
  lastLoadedAt: null
};

let mapPartsInitialized = false;

const mapObjectState = {
  maps: [],
  selectedMapId: null,
  selectedObjectId: null,
  filterType: "all",
  previewSelection: null,
  isLoading: false,
  loadError: null
};

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

function normalizeMapPartsCategory(rawCategory, index) {
  const rawId = rawCategory?.id ?? rawCategory?.categoryId ?? null;
  const id = rawId !== null && rawId !== undefined ? String(rawId) : `category-${index + 1}`;
  const nameCandidates = [rawCategory?.name, rawCategory?.label, rawCategory?.displayName];
  const label = nameCandidates.find((value) => typeof value === "string" && value.trim())?.trim() || id;
  return { id, label };
}

function normalizeMapPartEntry(rawPart, index) {
  const rawId = rawPart?.id ?? rawPart?.partsId ?? rawPart?.mapPartsId ?? null;
  const id = rawId !== null && rawId !== undefined ? String(rawId) : `part-${index + 1}`;
  const rawName = typeof rawPart?.name === "string" && rawPart.name.trim()
    ? rawPart.name.trim()
    : typeof rawPart?.displayName === "string" && rawPart.displayName.trim()
      ? rawPart.displayName.trim()
      : null;
  const name = rawName || `パーツ ${id}`;
  const width = Math.max(1, Math.round(Number(rawPart?.width ?? rawPart?.size?.width ?? 32)) || 32);
  const height = Math.max(1, Math.round(Number(rawPart?.height ?? rawPart?.size?.height ?? 32)) || 32);
  const attribute = Number.isFinite(Number(rawPart?.attribute)) ? Number(rawPart.attribute) : null;
  const attributeHex = typeof rawPart?.attributeHex === "string" && rawPart.attributeHex
    ? rawPart.attributeHex
    : attribute !== null
      ? formatHex(attribute)
      : null;
  const categoryId = rawPart?.categoryId !== undefined && rawPart?.categoryId !== null
    ? String(rawPart.categoryId)
    : null;
  const tags = Array.isArray(rawPart?.tags)
    ? rawPart.tags
        .map((tag) => (typeof tag === "string" ? tag.trim() : String(tag || "").trim()))
        .filter((tag) => tag.length)
    : [];
  const description = typeof rawPart?.description === "string" && rawPart.description.trim()
    ? rawPart.description.trim()
    : "";
  const updatedAtRaw = rawPart?.updatedAt;
  const updatedAt = typeof updatedAtRaw === "string" && updatedAtRaw.length ? updatedAtRaw : null;
  const imagePath = typeof rawPart?.imagePath === "string" && rawPart.imagePath.length ? rawPart.imagePath : null;
  const imageUrl = typeof rawPart?.imageUrl === "string" && rawPart.imageUrl.length
    ? rawPart.imageUrl
    : imagePath
      ? `/assets/map-parts/${imagePath}`
      : null;
  const updatedLabel = updatedAt ? formatTimestampLabel(updatedAt) : null;

  return {
    id,
    name,
    width,
    height,
    attribute,
    attributeHex,
    categoryId,
    tags,
    description,
    imagePath,
    imageUrl,
    updatedAt,
    updatedLabel,
    cachedImage: null,
    cachedImagePromise: null
  };
}

function getMapPartCategoryLabel(categoryId) {
  if (!categoryId) {
    return "未分類";
  }
  const match = mapPartsState.categories.find((category) => category.id === categoryId);
  return match ? match.label : "未分類";
}

function setMapPartsLoading(isLoading, message) {
  mapPartsState.isLoading = isLoading;
  if (mapPartsReloadButton) {
    mapPartsReloadButton.disabled = isLoading;
  }
  if (mapPartsCategorySelect) {
    mapPartsCategorySelect.disabled = isLoading || mapPartsState.categories.length === 0;
  }
  if (mapPartsSearchInput) {
    mapPartsSearchInput.disabled = isLoading;
  }
  if (mapPartsSummaryEl && isLoading) {
    mapPartsSummaryEl.textContent = message || "パーツ情報を読み込み中...";
  }
}

function setMapPartsError(message) {
  mapPartsState.loadError = message || null;
}

function setMapPartsWarning(message) {
  mapPartsState.loadWarning = message || null;
}

function populateMapPartsCategories() {
  if (!mapPartsCategorySelect) {
    return;
  }
  const previousValue = mapPartsCategorySelect.value || MAP_PARTS_DEFAULT_CATEGORY;
  mapPartsCategorySelect.innerHTML = "";
  const allOption = document.createElement("option");
  allOption.value = MAP_PARTS_DEFAULT_CATEGORY;
  allOption.textContent = "すべて";
  mapPartsCategorySelect.appendChild(allOption);
  mapPartsState.categories.forEach((category) => {
    const option = document.createElement("option");
    option.value = category.id;
    option.textContent = category.label;
    mapPartsCategorySelect.appendChild(option);
  });
  const shouldRestore = previousValue !== MAP_PARTS_DEFAULT_CATEGORY
    ? mapPartsState.categories.some((category) => category.id === previousValue)
    : true;
  mapPartsCategorySelect.value = shouldRestore ? previousValue : MAP_PARTS_DEFAULT_CATEGORY;
  mapPartsState.selectedCategoryId = mapPartsCategorySelect.value;
  mapPartsCategorySelect.disabled = mapPartsState.isLoading || mapPartsState.categories.length === 0;
}

function renderMapPartsSummary() {
  if (!mapPartsSummaryEl) {
    return;
  }
  if (mapPartsState.isLoading) {
    mapPartsSummaryEl.textContent = "パーツ情報を読み込み中...";
    return;
  }
  if (mapPartsState.loadError) {
    mapPartsSummaryEl.textContent = mapPartsState.loadError;
    return;
  }
  const total = mapPartsState.parts.length;
  const filtered = mapPartsState.filteredParts.length;
  const sourceLabel = mapPartsState.usingSampleData ? "サンプルデータ" : "API";
  const updatedLabel = mapPartsState.lastLoadedAt
    ? `最終更新: ${mapPartsState.lastLoadedAt}`
    : "最終更新: 不明";
  const warning = mapPartsState.loadWarning ? ` / 注意: ${mapPartsState.loadWarning}` : "";
  mapPartsSummaryEl.textContent = `ライブラリ総数: ${total} / 表示中: ${filtered} / ソース: ${sourceLabel} / ${updatedLabel}${warning}`;
}

function createMapPartsMessage(text) {
  const messageEl = document.createElement("div");
  messageEl.className = "map-parts-message";
  messageEl.textContent = text;
  return messageEl;
}

function computeMapPartScale(part, maxDisplaySize) {
  const width = Math.max(1, Number(part?.width) || 1);
  const height = Math.max(1, Number(part?.height) || 1);
  const maxDimension = Math.max(width, height);
  if (maxDimension <= maxDisplaySize) {
    return Math.max(1, Math.floor(maxDisplaySize / maxDimension)) || 1;
  }
  return maxDisplaySize / maxDimension;
}

function prepareCanvas(canvas, displayWidth, displayHeight) {
  const pixelRatio = window.devicePixelRatio || 1;
  canvas.width = Math.max(1, Math.round(displayWidth * pixelRatio));
  canvas.height = Math.max(1, Math.round(displayHeight * pixelRatio));
  canvas.style.width = `${Math.max(1, Math.round(displayWidth))}px`;
  canvas.style.height = `${Math.max(1, Math.round(displayHeight))}px`;
  const context = canvas.getContext("2d");
  if (!context) {
    return null;
  }
  context.setTransform(pixelRatio, 0, 0, pixelRatio, 0, 0);
  context.clearRect(0, 0, displayWidth, displayHeight);
  return context;
}

function drawCheckerboardBackground(context, width, height, cellSize = 8) {
  context.fillStyle = "rgba(15, 23, 42, 0.9)";
  context.fillRect(0, 0, width, height);
  context.fillStyle = "rgba(51, 65, 85, 0.65)";
  for (let y = 0; y < height; y += cellSize) {
    for (let x = (Math.floor(y / cellSize) % 2) * cellSize; x < width; x += cellSize * 2) {
      context.fillRect(x, y, cellSize, cellSize);
    }
  }
}

function drawMapPartFallback(context, width, height) {
  const stroke = Math.max(1, Math.round(Math.min(width, height) / 14));
  context.fillStyle = "rgba(59, 130, 246, 0.2)";
  context.fillRect(0, 0, width, height);
  context.strokeStyle = "rgba(59, 130, 246, 0.9)";
  context.lineWidth = stroke;
  context.beginPath();
  context.moveTo(0, 0);
  context.lineTo(width, height);
  context.moveTo(width, 0);
  context.lineTo(0, height);
  context.stroke();
}

function formatTimestampLabel(value) {
  if (!value) {
    return null;
  }
  const date = value instanceof Date ? value : new Date(value);
  if (Number.isNaN(date.getTime())) {
    return typeof value === "string" ? value : null;
  }
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, "0");
  const day = String(date.getDate()).padStart(2, "0");
  const hours = String(date.getHours()).padStart(2, "0");
  const minutes = String(date.getMinutes()).padStart(2, "0");
  const seconds = String(date.getSeconds()).padStart(2, "0");
  return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
}

function getMapPartImage(part) {
  if (part.cachedImage) {
    return Promise.resolve(part.cachedImage);
  }
  if (!part.imageUrl) {
    return Promise.reject(new Error("image_not_defined"));
  }
  if (!part.cachedImagePromise) {
    part.cachedImagePromise = new Promise((resolve, reject) => {
      const image = new Image();
      image.decoding = "async";
      image.onload = () => {
        part.cachedImage = image;
        resolve(image);
      };
      image.onerror = () => {
        part.cachedImagePromise = null;
        reject(new Error("image_load_failed"));
      };
      image.src = part.imageUrl;
    });
  }
  return part.cachedImagePromise;
}

function drawMapPartOnCanvas(canvas, part, scale) {
  if (!canvas || !part) {
    return;
  }
  const width = Math.max(1, Number(part.width) || 1);
  const height = Math.max(1, Number(part.height) || 1);
  const scaleFactor = Number.isFinite(scale) && scale > 0 ? scale : 1;
  const displayWidth = Math.max(1, width * scaleFactor);
  const displayHeight = Math.max(1, height * scaleFactor);
  const context = prepareCanvas(canvas, displayWidth, displayHeight);
  if (!context) {
    return;
  }
  drawCheckerboardBackground(context, displayWidth, displayHeight);
  if (!part.imageUrl) {
    drawMapPartFallback(context, displayWidth, displayHeight);
    return;
  }
  const requestToken = `${part.id}-${Date.now()}-${Math.random().toString(16).slice(2)}`;
  canvas.dataset.requestToken = requestToken;
  getMapPartImage(part)
    .then((image) => {
      if (canvas.dataset.requestToken !== requestToken) {
        return;
      }
      drawCheckerboardBackground(context, displayWidth, displayHeight);
      context.imageSmoothingEnabled = scaleFactor < 1;
      context.drawImage(image, 0, 0, width, height, 0, 0, displayWidth, displayHeight);
    })
    .catch(() => {
      if (canvas.dataset.requestToken !== requestToken) {
        return;
      }
      drawMapPartFallback(context, displayWidth, displayHeight);
    });
}

function renderMapPartsGrid(parts) {
  if (!mapPartsGridEl) {
    return;
  }
  mapPartsGridEl.innerHTML = "";
  if (mapPartsState.isLoading) {
    mapPartsGridEl.appendChild(createMapPartsMessage("パーツ情報を読み込み中です..."));
    return;
  }
  if (mapPartsState.loadError && !parts.length) {
    mapPartsGridEl.appendChild(createMapPartsMessage(mapPartsState.loadError));
    return;
  }
  if (!parts.length) {
    const fallbackText = mapPartsState.parts.length
      ? "条件に合致するマップパーツがありません"
      : "表示できるマップパーツがありません";
    mapPartsGridEl.appendChild(createMapPartsMessage(fallbackText));
    return;
  }
  parts.forEach((part) => {
    const button = document.createElement("button");
    button.type = "button";
    button.className = "map-parts-item";
    button.dataset.partId = part.id;
    button.setAttribute("role", "listitem");
    button.title = `${part.name} / サイズ: ${part.width}×${part.height}`;
    if (mapPartsState.selectedPartId === part.id) {
      button.classList.add("is-selected");
    }

    const canvasWrapper = document.createElement("div");
    canvasWrapper.className = "map-parts-item-canvas-wrapper";
    const canvas = document.createElement("canvas");
    canvas.className = "map-parts-item-canvas";
    canvasWrapper.appendChild(canvas);

    const label = document.createElement("span");
    label.className = "map-parts-item-label";
    label.textContent = part.name;

    button.appendChild(canvasWrapper);
    button.appendChild(label);

    if (part.tags.length) {
      const tagsWrapper = document.createElement("div");
      tagsWrapper.className = "map-parts-item-tags";
      part.tags.forEach((tag) => {
        const tagEl = document.createElement("span");
        tagEl.className = "map-parts-tag";
        tagEl.textContent = tag;
        tagsWrapper.appendChild(tagEl);
      });
      button.appendChild(tagsWrapper);
    }

    mapPartsGridEl.appendChild(button);

    const scale = computeMapPartScale(part, 110);
    drawMapPartOnCanvas(canvas, part, scale);
  });
}

function clearMapPartsPreviewCanvas() {
  if (!mapPartsPreviewCanvas) {
    return;
  }
  const context = mapPartsPreviewCanvas.getContext("2d");
  if (!context) {
    return;
  }
  context.setTransform(1, 0, 0, 1, 0, 0);
  context.clearRect(0, 0, mapPartsPreviewCanvas.width, mapPartsPreviewCanvas.height);
}

function renderMapPartsPreview(part) {
  if (!mapPartsPreviewCanvas || !mapPartsPreviewTitle || !mapPartsPreviewMeta || !mapPartsPreviewNotes) {
    return;
  }
  const hasSelection = Boolean(part);
  if (mapPartsPlaceholderEl) {
    mapPartsPlaceholderEl.style.display = hasSelection ? "none" : "flex";
    mapPartsPlaceholderEl.textContent = mapPartsState.isLoading ? "読み込み中..." : "パーツが選択されていません";
  }
  mapPartsPreviewCanvas.style.display = hasSelection ? "block" : "none";
  if (!hasSelection) {
    clearMapPartsPreviewCanvas();
    mapPartsPreviewTitle.textContent = "選択中: なし";
    mapPartsPreviewMeta.textContent = mapPartsState.isLoading ? "ライブラリを読み込み中です。" : "左の一覧からパーツを選択してください。";
    mapPartsPreviewNotes.textContent = "選択中のパーツに関するメタデータがここに表示されます。";
    if (mapPartsTagsContainer) {
      mapPartsTagsContainer.innerHTML = "";
    }
    if (mapPartsDetailCategory) {
      mapPartsDetailCategory.textContent = "-";
    }
    if (mapPartsDetailSize) {
      mapPartsDetailSize.textContent = "-";
    }
    if (mapPartsDetailAttribute) {
      mapPartsDetailAttribute.textContent = "-";
    }
    if (mapPartsDetailUpdated) {
      mapPartsDetailUpdated.textContent = mapPartsState.lastLoadedAt ? mapPartsState.lastLoadedAt : "-";
    }
    if (mapPartsClearButton) {
      mapPartsClearButton.disabled = true;
    }
    if (mapPartsApplyButton) {
      mapPartsApplyButton.disabled = true;
    }
    return;
  }

  mapPartsPreviewTitle.textContent = `選択中: ${part.name}`;
  const categoryLabel = getMapPartCategoryLabel(part.categoryId);
  mapPartsPreviewMeta.textContent = `ID: ${part.id} / カテゴリ: ${categoryLabel}`;
  mapPartsPreviewNotes.textContent = part.description || "説明は登録されていません。";

  if (mapPartsTagsContainer) {
    mapPartsTagsContainer.innerHTML = "";
    if (part.tags.length) {
      part.tags.forEach((tag) => {
        const tagEl = document.createElement("span");
        tagEl.className = "map-parts-tag";
        tagEl.textContent = tag;
        mapPartsTagsContainer.appendChild(tagEl);
      });
    }
  }

  if (mapPartsDetailCategory) {
    mapPartsDetailCategory.textContent = categoryLabel;
  }
  if (mapPartsDetailSize) {
    mapPartsDetailSize.textContent = `${part.width}×${part.height}`;
  }
  if (mapPartsDetailAttribute) {
    mapPartsDetailAttribute.textContent = part.attributeHex || (part.attribute !== null ? String(part.attribute) : "-");
  }
  if (mapPartsDetailUpdated) {
    mapPartsDetailUpdated.textContent = part.updatedLabel || part.updatedAt || (mapPartsState.lastLoadedAt ? mapPartsState.lastLoadedAt : "-");
  }
  if (mapPartsClearButton) {
    mapPartsClearButton.disabled = false;
  }
  if (mapPartsApplyButton) {
    mapPartsApplyButton.disabled = true;
  }

  const previewScale = computeMapPartScale(part, 220);
  drawMapPartOnCanvas(mapPartsPreviewCanvas, part, previewScale);
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

function selectMapPart(partId) {
  if (!partId) {
    mapPartsState.selectedPartId = null;
    renderMapPartsGrid(mapPartsState.filteredParts);
    renderMapPartsPreview(null);
    setMapPartsFeedback("パーツの選択を解除しました", null);
    return;
  }
  const target = mapPartsState.parts.find((part) => part.id === partId);
  if (!target) {
    mapPartsState.selectedPartId = null;
    renderMapPartsGrid(mapPartsState.filteredParts);
    renderMapPartsPreview(null);
    setMapPartsFeedback("指定したパーツが見つかりません", "error");
    return;
  }
  mapPartsState.selectedPartId = partId;
  renderMapPartsGrid(mapPartsState.filteredParts);
  renderMapPartsPreview(target);
  setMapPartsFeedback("現在は閲覧専用モードのため配置は行えません", "info");
}

function applyMapPartsFilters() {
  const query = mapPartsState.searchQuery.trim().toLowerCase();
  const filtered = mapPartsState.parts.filter((part) => {
    if (mapPartsState.selectedCategoryId !== MAP_PARTS_DEFAULT_CATEGORY && part.categoryId !== mapPartsState.selectedCategoryId) {
      return false;
    }
    if (!query) {
      return true;
    }
    const haystack = [part.name, part.id, part.tags.join(" ")].join(" ").toLowerCase();
    return haystack.includes(query);
  });
  mapPartsState.filteredParts = filtered;
  if (mapPartsState.selectedPartId && !filtered.some((part) => part.id === mapPartsState.selectedPartId)) {
    mapPartsState.selectedPartId = null;
  }
  renderMapPartsGrid(filtered);
  renderMapPartsSummary();
  if (mapPartsState.selectedPartId) {
    const selected = mapPartsState.parts.find((part) => part.id === mapPartsState.selectedPartId) || null;
    renderMapPartsPreview(selected);
  } else {
    renderMapPartsPreview(null);
  }
}

function handleMapPartsGridClick(event) {
  const button = event.target.closest(".map-parts-item");
  if (!button || !mapPartsGridEl || !mapPartsGridEl.contains(button)) {
    return;
  }
  const partId = button.dataset.partId;
  if (!partId) {
    return;
  }
  if (mapPartsState.selectedPartId === partId) {
    selectMapPart(null);
    return;
  }
  selectMapPart(partId);
}

function handleMapPartsCategoryChange() {
  if (!mapPartsCategorySelect) {
    return;
  }
  mapPartsState.selectedCategoryId = mapPartsCategorySelect.value || MAP_PARTS_DEFAULT_CATEGORY;
  applyMapPartsFilters();
}

function handleMapPartsSearchInput() {
  if (!mapPartsSearchInput) {
    return;
  }
  mapPartsState.searchQuery = mapPartsSearchInput.value || "";
  applyMapPartsFilters();
}

function handleMapPartsReload() {
  loadMapPartsData(true);
}

function handleMapPartsClear() {
  selectMapPart(null);
}

function handleMapPartsApply() {
  setMapPartsFeedback("現在は閲覧専用モードのため Web からの配置は行えません。", "error");
}

function initializeMapPartsView() {
  if (mapPartsInitialized) {
    return;
  }
  mapPartsInitialized = true;
  mapPartsState.isLoading = true;
  if (mapPartsSearchInput) {
    mapPartsSearchInput.value = "";
    mapPartsSearchInput.disabled = true;
  }
  if (mapPartsCategorySelect) {
    mapPartsCategorySelect.disabled = true;
  }
  renderMapPartsSummary();
  renderMapPartsGrid([]);
  renderMapPartsPreview(null);
  loadMapPartsData(false);
}

function buildSampleMapPartsPayload() {
  const now = new Date().toISOString();
  const categories = [
    { id: "terrain", label: "地形" },
    { id: "structure", label: "建造物" },
    { id: "decoration", label: "装飾" }
  ];
  const parts = [
    {
      id: "terrain-001",
      name: "石畳",
      width: 32,
      height: 32,
      categoryId: "terrain",
      tags: ["地面", "ベース"],
      description: "石畳の床タイル。テスト用のサンプルデータです。",
      imagePath: "terrain/stone_floor.png",
      attribute: 0x10,
      updatedAt: now
    },
    {
      id: "structure-001",
      name: "木製の扉",
      width: 32,
      height: 48,
      categoryId: "structure",
      tags: ["建物", "扉"],
      description: "MFC 版でよく利用する標準的な扉パーツ。",
      imagePath: "structure/wooden_door.png",
      attribute: 0x24,
      updatedAt: now
    },
    {
      id: "decoration-001",
      name: "街灯",
      width: 32,
      height: 64,
      categoryId: "decoration",
      tags: ["装飾", "照明"],
      description: "夜間マップで使用する街灯のサンプルです。",
      imagePath: "decoration/street_lamp.png",
      attribute: 0x30,
      updatedAt: now
    }
  ];
  return {
    categories: categories.map((category, index) => normalizeMapPartsCategory(category, index)),
    parts: parts.map((part, index) => normalizeMapPartEntry(part, index)),
    updatedAt: now
  };
}

async function loadMapPartsData(forceReload = false) {
  if (!mapPartsGridEl) {
    return;
  }
  if (mapPartsState.isLoading) {
    return;
  }
  if (!forceReload && mapPartsState.parts.length && !mapPartsState.loadError) {
    applyMapPartsFilters();
    return;
  }

  setMapPartsLoading(true, "パーツ情報を読み込み中...");
  setMapPartsError(null);
  setMapPartsWarning(null);
  setMapPartsFeedback("", null);

  try {
    const { response, data } = await fetchJson("/api/maps/parts");
    if (!response.ok || !data) {
      throw new Error("invalid_response");
    }
    const normalizedCategories = Array.isArray(data.categories)
      ? data.categories.map((item, index) => normalizeMapPartsCategory(item, index))
      : [];
    const normalizedParts = Array.isArray(data.parts)
      ? data.parts.map((item, index) => normalizeMapPartEntry(item, index))
      : [];

    const validCategoryIds = new Set(normalizedCategories.map((category) => category.id));
    normalizedParts.forEach((part) => {
      if (part.categoryId && !validCategoryIds.has(part.categoryId)) {
        part.categoryId = null;
      }
    });

    mapPartsState.categories = normalizedCategories;
    mapPartsState.parts = normalizedParts;
    mapPartsState.usingSampleData = false;
    mapPartsState.lastLoadedAt = formatTimestampLabel(data.updatedAt || new Date());
  } catch (error) {
    console.warn("マップパーツ情報の取得に失敗したためサンプルデータを利用します", error);
    const fallback = buildSampleMapPartsPayload();
    mapPartsState.categories = fallback.categories;
    mapPartsState.parts = fallback.parts;
    mapPartsState.usingSampleData = true;
    mapPartsState.lastLoadedAt = formatTimestampLabel(fallback.updatedAt);
    setMapPartsWarning("API 応答に失敗したためサンプルを表示中");
    if (!fallback.parts.length) {
      setMapPartsError("マップパーツ情報を取得できませんでした");
    }
  } finally {
    setMapPartsLoading(false);
    populateMapPartsCategories();
    if (mapPartsSearchInput) {
      mapPartsSearchInput.disabled = false;
    }
    applyMapPartsFilters();
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

  if (normalized === "map-parts") {
    if (!mapPartsInitialized) {
      initializeMapPartsView();
    } else if (options.forceReload) {
      loadMapPartsData(true);
    } else if (!mapPartsState.parts.length && !mapPartsState.isLoading && !mapPartsState.loadError) {
      loadMapPartsData(false);
    }
  }

  if (normalized === "map-objects") {
    if (options.forceReload) {
      loadMapObjectData(true);
    } else if (!mapObjectState.maps.length && !mapObjectState.isLoading && !mapObjectState.loadError) {
      loadMapObjectData(false);
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

  if (mapPartsGridEl) {
    mapPartsGridEl.addEventListener("click", handleMapPartsGridClick);
  }
  if (mapPartsCategorySelect) {
    mapPartsCategorySelect.addEventListener("change", handleMapPartsCategoryChange);
  }
  if (mapPartsSearchInput) {
    mapPartsSearchInput.addEventListener("input", handleMapPartsSearchInput);
  }
  if (mapPartsReloadButton) {
    mapPartsReloadButton.addEventListener("click", handleMapPartsReload);
  }
  if (mapPartsClearButton) {
    mapPartsClearButton.addEventListener("click", handleMapPartsClear);
  }
  if (mapPartsApplyButton) {
    mapPartsApplyButton.addEventListener("click", handleMapPartsApply);
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

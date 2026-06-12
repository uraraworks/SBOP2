/**
 * views/map-window.js
 * マップウィンドウ画面 (route: map-window)
 *
 * API:
 *   GET /api/maps/objects → マップ一覧（タイル込み）
 *   GET /api/maps/parts   → パーツカタログ（スプライト描画用）
 *   GET /api/selection    → 現在の選択状態
 *   POST /api/selection/pick → セル選択
 *   DELETE /api/selection    → 選択解除
 *
 * iframe/postMessage 連携はそのままモジュール化。機能変更なし。
 * WS の selection_changed はグローバル applySelectionPayload 経由で
 * app.js から呼ばれる。このビュー mount 後に window._mapWindowApplySelection を公開。
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// normalizeMapObject / normalizeMapEntry (app.js から移植)
// ----------------------------------------------------------------

function formatHex(value) {
  if (value === null || value === undefined) { return null; }
  const numeric = Number(value);
  if (!Number.isFinite(numeric)) { return null; }
  return "0x" + (numeric >>> 0).toString(16).toUpperCase().padStart(8, "0");
}

function normalizeMapObject(rawObject, mapId, index) {
  const placementId = typeof rawObject?.placementId === "number" ? rawObject.placementId : null;
  const objectId    = typeof rawObject?.objectId === "number"    ? rawObject.objectId    : null;
  const x = Number(rawObject?.x ?? 0);
  const y = Number(rawObject?.y ?? 0);
  const hasCollision = Boolean(rawObject?.hasCollision);
  const attribute    = typeof rawObject?.attribute === "number" ? rawObject.attribute : null;
  const attributeHex = (typeof rawObject?.attributeHex === "string" && rawObject.attributeHex) ? rawObject.attributeHex : formatHex(attribute);
  const objectIdHex  = (typeof rawObject?.objectIdHex === "string" && rawObject.objectIdHex)  ? rawObject.objectIdHex  : formatHex(objectId);
  const uniqueId = placementId !== null
    ? "placement-" + placementId
    : "map" + mapId + "-obj" + (objectId !== null ? objectId : "idx" + index) + "-" + x + "-" + y;
  return {
    id: uniqueId, placementId, objectId, objectIdHex,
    name: (typeof rawObject?.name === "string" && rawObject.name.length) ? rawObject.name : objectId !== null ? `オブジェクトID ${objectId}` : `配置${index + 1}`,
    type: hasCollision ? "collision" : "no-collision",
    attribute, attributeHex, x: Number.isFinite(x) ? x : 0, y: Number.isFinite(y) ? y : 0,
    hasCollision,
  };
}

function normalizeMapEntry(rawMap) {
  const rawId = rawMap?.id;
  const id    = rawId !== null && rawId !== undefined ? String(rawId) : "";
  const width  = Number(rawMap?.width  ?? 0);
  const height = Number(rawMap?.height ?? 0);
  const weatherType = typeof rawMap?.weatherType === "number" ? rawMap.weatherType : null;
  const objects = Array.isArray(rawMap?.objects) ? rawMap.objects.map((item, index) => normalizeMapObject(item, id, index)) : [];
  const tiles          = Array.isArray(rawMap?.tiles)          ? rawMap.tiles          : [];
  const tilesBase      = Array.isArray(rawMap?.tilesBase)      ? rawMap.tilesBase      : tiles;
  const tilesPile      = Array.isArray(rawMap?.tilesPile)      ? rawMap.tilesPile      : [];
  const tilesMapPileBase = Array.isArray(rawMap?.tilesMapPileBase) ? rawMap.tilesMapPileBase : [];
  const tilesMapPilePile = Array.isArray(rawMap?.tilesMapPilePile) ? rawMap.tilesMapPilePile : [];
  return {
    id,
    name: typeof rawMap?.name === "string" && rawMap.name.length ? rawMap.name : (id ? `マップID ${id}` : "名称未設定"),
    width: Number.isFinite(width) ? width : 0,
    height: Number.isFinite(height) ? height : 0,
    weatherType,
    battleEnabled: Boolean(rawMap?.battleEnabled),
    recoveryEnabled: Boolean(rawMap?.recoveryEnabled),
    objects, tiles, tilesBase, tilesPile, tilesMapPileBase, tilesMapPilePile
  };
}

// ----------------------------------------------------------------
// スプライト描画
// ----------------------------------------------------------------

function updateSpriteLayer(element, sprite, sheetBaseUrl, tileSize, sheetTileWidth, sheetTileHeight) {
  if (!element) { return; }
  const cellPx = tileSize * 2;
  const sheetWidthPx  = sheetTileWidth  * cellPx;
  const sheetHeightPx = sheetTileHeight * cellPx;
  if (!sprite) {
    element.classList.add("is-empty");
    element.style.backgroundImage  = "none";
    element.style.backgroundSize   = `${sheetWidthPx}px ${sheetHeightPx}px`;
    element.style.backgroundPosition = "0 0";
    return;
  }
  element.classList.remove("is-empty");
  const base    = sheetBaseUrl.endsWith("/") ? sheetBaseUrl.slice(0, -1) : sheetBaseUrl;
  const sheetUrl = `${base}/${sprite.sheet}.png`;
  element.style.backgroundImage    = `url(${sheetUrl})`;
  element.style.backgroundSize     = `${sheetWidthPx}px ${sheetHeightPx}px`;
  element.style.backgroundPosition = `${-(sprite.tileX * cellPx)}px ${-(sprite.tileY * cellPx)}px`;
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  const mapWindowMapSelect  = container.querySelector("#map-window-map");
  const mapWindowSummaryEl  = container.querySelector("#map-window-summary");
  const mapWindowGridEl     = document.getElementById("map-window-grid");         // body レベルに置かれている場合
  const mapWindowSelectedEl = document.getElementById("map-window-selected-info");

  const state = {
    maps: [],
    selectedMapId: null,
    selectedCell: null,
    isInitialized: false,
    isLoading: false,
    loadError: null,
    tileSize: 16,
    sheetTileWidth: 32,
    sheetTileHeight: 32,
    sheetBaseUrl: "/api/assets/map-parts/sheets",
    // パーツカタログ（スプライト描画用）
    partsLoaded: false,
  };

  // app.js から WS ペイロードを受け取れるようにグローバル公開
  window._mapWindowApplySelection = applySelectionPayload;
  window._mapWindowState = state;

  function updateSummary() {
    if (!mapWindowSummaryEl) { return; }
    if (state.isLoading)  { mapWindowSummaryEl.textContent = "マップ情報を読み込み中..."; return; }
    if (state.loadError)  { mapWindowSummaryEl.textContent = state.loadError; return; }
    const map = state.maps.find((m) => m.id === state.selectedMapId);
    if (!map) { mapWindowSummaryEl.textContent = state.maps.length ? "マップを選択してください" : "マップデータなし"; return; }
    mapWindowSummaryEl.textContent = `${map.name}  (${map.width} × ${map.height})`;
  }

  function updateSelectedInfo() {
    if (!mapWindowSelectedEl) { return; }
    if (!state.selectedCell) { mapWindowSelectedEl.textContent = "選択中のセル: なし"; return; }
    const { x, y } = state.selectedCell;
    mapWindowSelectedEl.textContent = `選択中のセル: X ${x}、Y ${y}`;
  }

  function populateOptions() {
    if (!mapWindowMapSelect) { return; }
    mapWindowMapSelect.innerHTML = "";
    if (!state.maps.length) {
      const opt = document.createElement("option");
      opt.value = "";
      opt.textContent = state.isLoading ? "読み込み中..." : "マップデータなし";
      mapWindowMapSelect.appendChild(opt);
      mapWindowMapSelect.disabled = true;
      return;
    }
    state.maps.forEach((map) => {
      const opt = document.createElement("option");
      opt.value = map.id;
      opt.textContent = map.id ? `${map.name} (ID: ${map.id})` : map.name;
      mapWindowMapSelect.appendChild(opt);
    });
    mapWindowMapSelect.disabled = state.isLoading;
    if (state.selectedMapId) { mapWindowMapSelect.value = state.selectedMapId; }
  }

  function refreshCellHighlight() {
    if (!mapWindowGridEl) { return; }
    mapWindowGridEl.querySelectorAll(".map-object-cell").forEach((btn) => {
      const bx = Number(btn.dataset.x);
      const by = Number(btn.dataset.y);
      const isNowSelected = state.selectedCell && state.selectedCell.x === bx && state.selectedCell.y === by;
      btn.classList.toggle("is-selected", isNowSelected);
    });
  }

  function grpIdToSprite(grpId, sheetTileWidth) {
    if (!grpId) { return null; }
    const tile = grpId % 1024;
    return { sheet: Math.floor(grpId / 1024), tile, tileX: tile % sheetTileWidth, tileY: Math.floor(tile / sheetTileWidth) };
  }

  function renderGrid() {
    if (!mapWindowGridEl) { return; }
    mapWindowGridEl.innerHTML = "";
    const showMessage = (msg) => {
      const el = document.createElement("div");
      el.className = "empty-message";
      el.textContent = msg;
      mapWindowGridEl.style.gridTemplateColumns = "repeat(1, minmax(80px, 1fr))";
      mapWindowGridEl.appendChild(el);
    };
    if (state.isLoading) { showMessage("マップ情報を読み込み中です"); return; }
    if (state.loadError) { showMessage(state.loadError); return; }
    const map = state.maps.find((m) => m.id === state.selectedMapId);
    if (!map) { showMessage(state.maps.length ? "マップが選択されていません" : "表示できるマップがありません"); return; }

    const hasTiles = Array.isArray(map.tiles) && map.tiles.length > 0;
    const cellPx   = state.tileSize * 2;
    const sheetTileWidth = state.sheetTileWidth;
    const sel = state.selectedCell;

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
          cellButton.style.width  = `${cellPx}px`;
          cellButton.style.height = `${cellPx}px`;
          const layerDefs = [
            { grpId: Number(map.tilesBase[tileIndex])      || 0, className: "map-parts-tile" },
            { grpId: Number(map.tilesPile[tileIndex])      || 0, className: "map-parts-tile overlay" },
            { grpId: Number(map.tilesMapPileBase[tileIndex]) || 0, className: "map-parts-tile overlay" },
            { grpId: Number(map.tilesMapPilePile[tileIndex]) || 0, className: "map-parts-tile overlay" },
          ];
          for (const { grpId, className } of layerDefs) {
            const sprite   = grpIdToSprite(grpId, sheetTileWidth);
            const layerDiv = document.createElement("div");
            layerDiv.className = className;
            layerDiv.dataset.tilePixelSize = String(cellPx);
            updateSpriteLayer(layerDiv, sprite, state.sheetBaseUrl, state.tileSize, state.sheetTileWidth, state.sheetTileHeight);
            if (sprite) { layerDiv.style.backgroundRepeat = "no-repeat"; }
            cellButton.appendChild(layerDiv);
          }
        }

        if (sel && sel.x === x && sel.y === y) { cellButton.classList.add("is-selected"); }
        cellButton.addEventListener("click", () => { handleCellClick(x, y); });
        mapWindowGridEl.appendChild(cellButton);
      }
    }
  }

  function getPickTarget() {
    const checked = document.querySelector('input[name="map-window-pick-target"]:checked');
    return checked ? checked.value : "cell";
  }

  async function handleCellClick(x, y) {
    const pickTarget = getPickTarget();
    if (pickTarget !== "cell") {
      if (mapWindowSummaryEl) { mapWindowSummaryEl.textContent = "このピックタイプは未対応です（現在 cell のみ対応）"; }
      return;
    }
    const mapId = state.selectedMapId;
    const prev  = state.selectedCell;
    const isSameCell = prev && prev.x === x && prev.y === y;
    try {
      if (isSameCell) {
        const { response } = await fetchJson("/api/selection", { method: "DELETE" });
        if (!response.ok) { throw new Error("DELETE /api/selection: " + response.status); }
        state.selectedCell = null;
      } else {
        const { response } = await fetchJson("/api/selection/pick", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ type: "map_cell", mapId, x, y })
        });
        if (!response.ok) { throw new Error("POST /api/selection/pick: " + response.status); }
        state.selectedCell = { x, y };
      }
    } catch (error) {
      console.warn("[map-window] 選択の同期に失敗しました", error);
      if (mapWindowSummaryEl) { mapWindowSummaryEl.textContent = "選択の同期に失敗しました"; }
      return;
    }
    refreshCellHighlight();
    updateSelectedInfo();
  }

  function applySelectionPayload(payload) {
    if (!state.selectedMapId) { return; }
    if (
      payload &&
      payload.type === "map_cell" &&
      String(payload.mapId) === String(state.selectedMapId) &&
      typeof payload.x === "number" &&
      typeof payload.y === "number"
    ) {
      state.selectedCell = { x: payload.x, y: payload.y };
    } else if (!payload) {
      state.selectedCell = null;
    } else {
      return; // 別マップ or 非対応 type
    }
    refreshCellHighlight();
    updateSelectedInfo();
  }

  async function syncSelectionFromServer() {
    if (!state.selectedMapId) { return; }
    try {
      const { response, data } = await fetchJson("/api/selection");
      if (!response.ok) { throw new Error("GET /api/selection: " + response.status); }
      if (data && data.type === "map_cell" && String(data.mapId) === String(state.selectedMapId) && typeof data.x === "number" && typeof data.y === "number") {
        state.selectedCell = { x: data.x, y: data.y };
      }
    } catch (error) {
      console.warn("[map-window] 選択の同期に失敗しました", error);
    }
  }

  async function handleSelectChange(event) {
    const newId = event.target.value;
    state.selectedMapId  = newId || null;
    state.selectedCell   = null;
    updateSummary();
    updateSelectedInfo();
    renderGrid();
    if (state.selectedMapId) {
      await syncSelectionFromServer();
      refreshCellHighlight();
      updateSelectedInfo();
    }
  }

  async function loadData() {
    if (!mapWindowMapSelect) { return; }
    if (state.isLoading) { return; }
    state.isLoading = true;
    state.loadError = null;
    updateSummary();
    renderGrid();

    try {
      const { response, data } = await fetchJson("/api/maps/objects");
      if (!response.ok || !data || !Array.isArray(data.maps)) { throw new Error("invalid_response"); }

      state.tileSize       = Number(data.tileSize)       || state.tileSize;
      state.sheetTileWidth = Number(data.sheetTileWidth) || state.sheetTileWidth;
      state.sheetTileHeight= Number(data.sheetTileHeight)|| state.sheetTileHeight;
      if (typeof data.sheetBaseUrl === "string" && data.sheetBaseUrl.length) { state.sheetBaseUrl = data.sheetBaseUrl; }
      state.maps = data.maps.map((item) => normalizeMapEntry(item));

      if (state.maps.length) {
        if (!state.selectedMapId || !state.maps.some((m) => m.id === state.selectedMapId)) {
          state.selectedMapId = state.maps[0].id;
        }
      } else {
        state.selectedMapId = null;
      }
      state.selectedCell = null;
    } catch (error) {
      console.error("map-window: マップデータの取得に失敗しました", error);
      state.maps = [];
      state.selectedMapId = null;
      state.selectedCell  = null;
      state.loadError = "マップデータの取得に失敗しました";
    } finally {
      state.isLoading = false;
      populateOptions();
      updateSummary();
      if (!state.loadError) { await syncSelectionFromServer(); }
      updateSelectedInfo();
      renderGrid();
    }
  }

  // イベント登録
  if (mapWindowMapSelect) {
    mapWindowMapSelect.addEventListener("change", handleSelectChange);
  }

  // 初期化
  populateOptions();
  updateSummary();
  updateSelectedInfo();
  renderGrid();
  loadData();

  return {
    unmount() {
      delete window._mapWindowApplySelection;
      delete window._mapWindowState;
    }
  };
}

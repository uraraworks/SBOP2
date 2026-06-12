/**
 * views/map-paint.js
 * RPGツクール風グリッドペイントエディタ共通部品
 *
 * createMapPaintEditor(options) → { el, destroy }
 *
 * options:
 *   container       {HTMLElement}   親要素
 *   maps            {Array}         [{id, name, width, height, tilesBase, tilesPile, ...}]
 *   parts           {Array}         normalizeMapPart() 済みのパーツ一覧
 *   sheetBaseUrl    {string}        スプライトシート基底URL
 *   tileSize        {number}        1タイルのゲーム内px (デフォルト16)
 *   sheetTileWidth  {number}        シート横タイル数 (デフォルト32)
 *   sheetTileHeight {number}        シート縦タイル数 (デフォルト32)
 *   onSave          {Function}      async (mapId, tilesBase, tilesPile) => void
 *   onMapChange     {Function}      (mapId) => void   マップ選択変更通知
 *   notifyFrame     {Function}      (partsId) => void  ブラシ変更をiframeへ通知
 *   pickFromFrame   {Function}      外部から呼ぶ: pickFromFrame(partsId) でブラシを上書き
 *   layerMode       {string}        "base"|"pile" (デフォルト"base")
 *   usePickPile     {boolean}       右クリックスポイトで pile レイヤも参照するか
 *   adminMode       {number}        sbop2_set_admin_mode の mode 値 (省略可)
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// スプライトシート URL 解決
// ----------------------------------------------------------------

function resolveSheetUrl(baseUrl, sheetIndex) {
  const base = baseUrl.endsWith("/") ? baseUrl.slice(0, -1) : baseUrl;
  return `${base}/${sheetIndex}.png`;
}

// grpId (sheet*1024+tile) → { sheet, tileX, tileY } 変換
function grpIdToCoord(grpId, sheetTileWidth) {
  if (!grpId) return null;
  const sheet = Math.floor(grpId / 1024);
  const tile = grpId % 1024;
  return {
    sheet,
    tileX: tile % sheetTileWidth,
    tileY: Math.floor(tile / sheetTileWidth),
  };
}

// ----------------------------------------------------------------
// パレット（左ペイン）
// ----------------------------------------------------------------

/**
 * createPalette(options) → { el, getSelectedId, setSelectedId }
 *
 * options:
 *   parts          {Array}   normalizeMapPart() 済み一覧
 *   sheetBaseUrl   {string}
 *   tileSize       {number}
 *   sheetTileWidth {number}
 *   sheetTileHeight {number}
 *   onSelect       {Function}  (partsId) => void
 */
export function createPalette(options) {
  const {
    parts,
    sheetBaseUrl,
    tileSize,
    sheetTileWidth,
    sheetTileHeight,
    onSelect,
  } = options;

  let selectedId = null;
  const PALETTE_SCALE = 2;
  const cellPx = tileSize * PALETTE_SCALE;

  // ---- ルート ----
  const root = document.createElement("div");
  root.className = "mp-palette";

  // ---- ブラシ表示バー ----
  const brushBar = document.createElement("div");
  brushBar.className = "mp-palette-brush-bar";
  const brushThumb = document.createElement("canvas");
  brushThumb.width = cellPx;
  brushThumb.height = cellPx;
  brushThumb.className = "mp-palette-brush-thumb";
  const brushLabel = document.createElement("span");
  brushLabel.className = "mp-palette-brush-label";
  brushLabel.textContent = "ブラシ: なし";
  brushBar.appendChild(brushThumb);
  brushBar.appendChild(brushLabel);
  root.appendChild(brushBar);

  // ---- ギャラリー ----
  const gallery = document.createElement("div");
  gallery.className = "mp-palette-gallery";
  root.appendChild(gallery);

  // パーツのサムネを描画するキャンバス
  function drawThumb(canvas, part) {
    const ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, cellPx, cellPx);
    // base, pile の順に描画
    for (const sprite of [part.sprites.base, part.sprites.overlay]) {
      if (!sprite) continue;
      const img = getOrLoadImage(resolveSheetUrl(sheetBaseUrl, sprite.sheet));
      if (img && img.complete && img.naturalWidth) {
        const sx = sprite.tileX * tileSize;
        const sy = sprite.tileY * tileSize;
        ctx.drawImage(img, sx, sy, tileSize, tileSize, 0, 0, cellPx, cellPx);
      }
    }
  }

  // ---- 画像キャッシュ ----
  const imgCache = new Map();
  function getOrLoadImage(url) {
    if (imgCache.has(url)) return imgCache.get(url);
    const img = new Image();
    img.src = url;
    img.onload = () => {
      // 読み込み後にギャラリーの該当サムネを再描画
      gallery.querySelectorAll("canvas[data-parts-id]").forEach((c) => {
        const pid = Number(c.dataset.partsId);
        const p = parts.find((x) => x.partsId === pid);
        if (p) drawThumb(c, p);
      });
      drawActiveBrush();
    };
    imgCache.set(url, img);
    return img;
  }

  function drawActiveBrush() {
    const ctx = brushThumb.getContext("2d");
    ctx.clearRect(0, 0, cellPx, cellPx);
    if (selectedId === null) return;
    const part = parts.find((p) => p.partsId === selectedId);
    if (!part) return;
    drawThumb(brushThumb, part);
  }

  // ---- 選択 ----
  function selectPart(partsId) {
    selectedId = partsId;
    // ハイライト更新
    gallery.querySelectorAll(".mp-palette-item").forEach((el) => {
      el.classList.toggle("is-selected", Number(el.dataset.partsId) === partsId);
    });
    const part = parts.find((p) => p.partsId === partsId);
    brushLabel.textContent = part
      ? `ブラシ: パーツ ${partsId}`
      : "ブラシ: なし";
    drawActiveBrush();
    if (onSelect) onSelect(partsId);
  }

  // ---- ギャラリー描画 ----
  function buildGallery() {
    gallery.innerHTML = "";
    if (!parts.length) {
      const msg = document.createElement("div");
      msg.className = "mp-palette-empty";
      msg.textContent = "パーツがありません";
      gallery.appendChild(msg);
      return;
    }
    parts.forEach((part) => {
      const item = document.createElement("button");
      item.type = "button";
      item.className = "mp-palette-item";
      item.dataset.partsId = String(part.partsId);
      item.title = `パーツID: ${part.partsId}`;
      if (part.partsId === selectedId) item.classList.add("is-selected");

      const canvas = document.createElement("canvas");
      canvas.width = cellPx;
      canvas.height = cellPx;
      canvas.className = "mp-palette-item-thumb";
      canvas.dataset.partsId = String(part.partsId);
      drawThumb(canvas, part);
      item.appendChild(canvas);

      const label = document.createElement("span");
      label.className = "mp-palette-item-label";
      label.textContent = String(part.partsId);
      item.appendChild(label);

      item.addEventListener("click", () => selectPart(part.partsId));
      gallery.appendChild(item);
    });
  }

  buildGallery();

  return {
    el: root,
    getSelectedId() { return selectedId; },
    setSelectedId(id) {
      selectPart(id);
    },
  };
}

// ----------------------------------------------------------------
// メイングリッド Canvas エディタ
// ----------------------------------------------------------------

/**
 * createGridCanvas(options) → {
 *   el, setMap(map), getGrid(), setZoom(n), toggleGrid(bool),
 *   setBrush(partsId), applyExternalPick(x, y)
 * }
 *
 * options:
 *   sheetBaseUrl    {string}
 *   tileSize        {number}
 *   sheetTileWidth  {number}
 *   sheetTileHeight {number}
 *   parts           {Array}
 *   layerMode       {string} "base"|"pile"
 *   onDirty         {Function}  () => void  変更発生通知
 *   onRightClick    {Function}  (partsId) => void  スポイト
 */
function createGridCanvas(options) {
  const {
    sheetBaseUrl,
    tileSize,
    sheetTileWidth,
    sheetTileHeight,
    parts,
    layerMode = "base",
    onDirty,
    onRightClick,
  } = options;

  let zoom = 2;
  let showGrid = true;
  let map = null;          // 現在のマップデータ
  let tilesBase = [];      // 編集中の base タイル配列 (grpId)
  let tilesPile = [];      // 編集中の pile タイル配列
  let brushId = null;      // 選択中パーツID
  let isDragging = false;
  let isRightDragging = false;
  let undoStack = [];      // [{tilesBase: [...], tilesPile: [...]}]
  const UNDO_MAX = 50;

  const partsMap = new Map(); // partsId → part
  parts.forEach((p) => partsMap.set(p.partsId, p));

  // ---- DOM ----
  const root = document.createElement("div");
  root.className = "mp-grid-root";

  const canvasWrap = document.createElement("div");
  canvasWrap.className = "mp-grid-canvas-wrap";
  root.appendChild(canvasWrap);

  const canvas = document.createElement("canvas");
  canvas.className = "mp-grid-canvas";
  canvasWrap.appendChild(canvas);
  const ctx = canvas.getContext("2d");

  // ---- 画像キャッシュ ----
  const imgCache = new Map();
  function getOrLoadImage(url) {
    if (imgCache.has(url)) return imgCache.get(url);
    const img = new Image();
    img.src = url;
    img.onload = () => { requestDraw(); };
    imgCache.set(url, img);
    return img;
  }

  // ---- レンダリング ----
  let rafPending = false;
  function requestDraw() {
    if (!rafPending) {
      rafPending = true;
      requestAnimationFrame(() => { rafPending = false; draw(); });
    }
  }

  function cellPx() { return tileSize * zoom; }

  function draw() {
    if (!map) {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      return;
    }
    const cp = cellPx();
    canvas.width  = map.width  * cp;
    canvas.height = map.height * cp;

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.imageSmoothingEnabled = false;

    // タイル描画 (base → pile)
    for (const [layerTiles, zIndex] of [[tilesBase, 0], [tilesPile, 1]]) {
      for (let row = 0; row < map.height; row++) {
        for (let col = 0; col < map.width; col++) {
          const idx = row * map.width + col;
          const grpId = layerTiles[idx] || 0;
          if (!grpId) continue;
          const coord = grpIdToCoord(grpId, sheetTileWidth);
          if (!coord) continue;
          const url = resolveSheetUrl(sheetBaseUrl, coord.sheet);
          const img = getOrLoadImage(url);
          if (img && img.complete && img.naturalWidth) {
            ctx.drawImage(
              img,
              coord.tileX * tileSize, coord.tileY * tileSize,
              tileSize, tileSize,
              col * cp, row * cp,
              cp, cp
            );
          }
        }
      }
    }

    // グリッド線
    if (showGrid) {
      ctx.strokeStyle = "rgba(0,0,0,0.18)";
      ctx.lineWidth = 1;
      for (let col = 0; col <= map.width; col++) {
        ctx.beginPath();
        ctx.moveTo(col * cp, 0);
        ctx.lineTo(col * cp, canvas.height);
        ctx.stroke();
      }
      for (let row = 0; row <= map.height; row++) {
        ctx.beginPath();
        ctx.moveTo(0, row * cp);
        ctx.lineTo(canvas.width, row * cp);
        ctx.stroke();
      }
    }
  }

  // ---- ブラシ塗り ----
  function getLayerTiles() {
    return layerMode === "pile" ? tilesPile : tilesBase;
  }

  function grpIdFromPart(part) {
    if (!part) return 0;
    const sp = layerMode === "pile" ? part.sprites.overlay : part.sprites.base;
    if (!sp) return 0;
    return sp.sheet * 1024 + sp.tileX + sp.tileY * sheetTileWidth;
  }

  function paintCell(cx, cy) {
    if (!map || brushId === null) return;
    if (cx < 0 || cx >= map.width || cy < 0 || cy >= map.height) return;
    const idx = cy * map.width + cx;
    const arr = getLayerTiles();
    const part = partsMap.get(brushId);
    const newGrpId = part ? grpIdFromPart(part) : 0;
    if (arr[idx] !== newGrpId) {
      arr[idx] = newGrpId;
      if (onDirty) onDirty();
      requestDraw();
    }
  }

  function canvasToCell(ev) {
    const rect = canvas.getBoundingClientRect();
    const cp = cellPx();
    const cx = Math.floor((ev.clientX - rect.left) / cp);
    const cy = Math.floor((ev.clientY - rect.top) / cp);
    return { cx, cy };
  }

  // ---- Undo ----
  function pushUndo() {
    undoStack.push({ tilesBase: tilesBase.slice(), tilesPile: tilesPile.slice() });
    if (undoStack.length > UNDO_MAX) undoStack.shift();
  }

  function undo() {
    if (!undoStack.length) return;
    const prev = undoStack.pop();
    tilesBase = prev.tilesBase;
    tilesPile = prev.tilesPile;
    if (onDirty) onDirty();
    requestDraw();
  }

  // ---- Shift+ドラッグ 矩形塗り ----
  let rectStart = null;

  function paintRect(x1, y1, x2, y2) {
    const lx = Math.min(x1, x2), rx = Math.max(x1, x2);
    const ty = Math.min(y1, y2), by = Math.max(y1, y2);
    for (let row = ty; row <= by; row++) {
      for (let col = lx; col <= rx; col++) {
        paintCell(col, row);
      }
    }
  }

  // ---- マウスイベント ----
  canvas.addEventListener("contextmenu", (ev) => { ev.preventDefault(); });

  canvas.addEventListener("mousedown", (ev) => {
    ev.preventDefault();
    const { cx, cy } = canvasToCell(ev);
    if (ev.button === 2) {
      // 右クリック = スポイト
      isRightDragging = true;
      doEyedropper(cx, cy);
      return;
    }
    // 左クリック = 塗る
    pushUndo();
    isDragging = true;
    if (ev.shiftKey) {
      rectStart = { cx, cy };
    } else {
      rectStart = null;
      paintCell(cx, cy);
    }
  });

  canvas.addEventListener("mousemove", (ev) => {
    if (isDragging && !rectStart) {
      const { cx, cy } = canvasToCell(ev);
      paintCell(cx, cy);
    }
    if (isRightDragging) {
      const { cx, cy } = canvasToCell(ev);
      doEyedropper(cx, cy);
    }
  });

  canvas.addEventListener("mouseup", (ev) => {
    if (isDragging && rectStart) {
      const { cx, cy } = canvasToCell(ev);
      paintRect(rectStart.cx, rectStart.cy, cx, cy);
      rectStart = null;
    }
    isDragging = false;
    isRightDragging = false;
  });

  canvas.addEventListener("mouseleave", () => {
    isDragging = false;
    isRightDragging = false;
    rectStart = null;
  });

  // ---- スポイト ----
  function doEyedropper(cx, cy) {
    if (!map) return;
    if (cx < 0 || cx >= map.width || cy < 0 || cy >= map.height) return;
    const idx = cy * map.width + cx;
    // base と pile 両方から検索
    const grpId = tilesBase[idx] || tilesPile[idx] || 0;
    if (!grpId) return;
    // grpId からパーツを探す
    const found = findPartByGrpId(grpId);
    if (found && onRightClick) {
      onRightClick(found.partsId);
    }
  }

  function findPartByGrpId(grpId) {
    // base.sheet*1024 + base.tileX + base.tileY*sheetTileWidth で照合
    for (const part of partsMap.values()) {
      for (const sp of [part.sprites.base, part.sprites.overlay]) {
        if (!sp) continue;
        const gid = sp.sheet * 1024 + sp.tileX + sp.tileY * sheetTileWidth;
        if (gid === grpId) return part;
      }
    }
    return null;
  }

  // ---- キーボードショートカット ----
  function onKeyDown(ev) {
    if ((ev.ctrlKey || ev.metaKey) && ev.key === "z") {
      ev.preventDefault();
      undo();
    }
  }
  window.addEventListener("keydown", onKeyDown);

  // ---- 公開API ----
  function setMap(newMap) {
    map = newMap;
    undoStack = [];
    if (map) {
      const len = map.width * map.height;
      tilesBase = Array.isArray(map.tilesBase) && map.tilesBase.length === len
        ? map.tilesBase.slice()
        : new Array(len).fill(0);
      tilesPile = Array.isArray(map.tilesPile) && map.tilesPile.length === len
        ? map.tilesPile.slice()
        : new Array(len).fill(0);
    } else {
      tilesBase = [];
      tilesPile = [];
    }
    requestDraw();
  }

  function getGrid() {
    return { tilesBase: tilesBase.slice(), tilesPile: tilesPile.slice() };
  }

  function setZoom(z) {
    zoom = z;
    requestDraw();
  }

  function setShowGrid(v) {
    showGrid = v;
    requestDraw();
  }

  function setBrush(partsId) {
    brushId = partsId;
  }

  function applyExternalPick(x, y) {
    const idx = y * (map ? map.width : 0) + x;
    if (!map || idx < 0 || idx >= tilesBase.length) return;
    const grpId = tilesBase[idx] || tilesPile[idx] || 0;
    if (!grpId) return;
    const found = findPartByGrpId(grpId);
    if (found && onRightClick) {
      onRightClick(found.partsId);
    }
  }

  function destroy() {
    window.removeEventListener("keydown", onKeyDown);
  }

  return {
    el: canvasWrap,
    setMap,
    getGrid,
    setZoom,
    setShowGrid,
    setBrush,
    applyExternalPick,
    destroy,
  };
}

// ----------------------------------------------------------------
// createMapPaintEditor — メインファサード
// ----------------------------------------------------------------

export function createMapPaintEditor(options) {
  const {
    maps = [],
    parts = [],
    sheetBaseUrl = "/api/assets/map-parts/sheets",
    tileSize = 16,
    sheetTileWidth = 32,
    sheetTileHeight = 32,
    onSave,
    onMapChange,
    notifyFrame,
    layerMode = "base",
    initialMapId = null,
  } = options;

  let selectedMapId = initialMapId || (maps.length ? maps[0].id : null);
  let isDirty = false;

  // ---- ルートレイアウト ----
  const root = document.createElement("div");
  root.className = "mp-editor";

  // ---- 上部バー ----
  const topBar = document.createElement("div");
  topBar.className = "mp-editor-topbar";

  // マップ選択
  const mapLabel = document.createElement("label");
  mapLabel.className = "mp-topbar-label";
  mapLabel.textContent = "マップ:";
  const mapSelect = document.createElement("select");
  mapSelect.className = "mp-topbar-select";
  maps.forEach((m) => {
    const opt = document.createElement("option");
    opt.value = String(m.id);
    opt.textContent = `[${m.id}] ${m.name || "（名前なし）"}`;
    if (String(m.id) === String(selectedMapId)) opt.selected = true;
    mapSelect.appendChild(opt);
  });
  mapLabel.appendChild(mapSelect);
  topBar.appendChild(mapLabel);

  // 保存ボタン
  const saveBtn = document.createElement("button");
  saveBtn.type = "button";
  saveBtn.className = "button primary mp-topbar-save";
  saveBtn.textContent = "保存";
  topBar.appendChild(saveBtn);

  // Undo ボタン
  const undoBtn = document.createElement("button");
  undoBtn.type = "button";
  undoBtn.className = "button secondary mp-topbar-undo";
  undoBtn.textContent = "元に戻す (Ctrl+Z)";
  topBar.appendChild(undoBtn);

  // ズームボタン
  const zoom1Btn = document.createElement("button");
  zoom1Btn.type = "button";
  zoom1Btn.className = "button secondary mp-topbar-zoom";
  zoom1Btn.textContent = "1x";
  const zoom2Btn = document.createElement("button");
  zoom2Btn.type = "button";
  zoom2Btn.className = "button secondary mp-topbar-zoom is-active";
  zoom2Btn.textContent = "2x";
  topBar.appendChild(zoom1Btn);
  topBar.appendChild(zoom2Btn);

  // グリッドトグル
  const gridToggle = document.createElement("button");
  gridToggle.type = "button";
  gridToggle.className = "button secondary mp-topbar-grid is-active";
  gridToggle.textContent = "グリッド: ON";
  topBar.appendChild(gridToggle);

  // フィードバック
  const feedbackEl = document.createElement("span");
  feedbackEl.className = "mp-topbar-feedback";
  topBar.appendChild(feedbackEl);

  root.appendChild(topBar);

  // ---- ボディ (左ペイン + 中央) ----
  const body = document.createElement("div");
  body.className = "mp-editor-body";

  // パレット
  const palette = createPalette({
    parts,
    sheetBaseUrl,
    tileSize,
    sheetTileWidth,
    sheetTileHeight,
    onSelect(partsId) {
      gridEditor.setBrush(partsId);
      if (notifyFrame) notifyFrame(partsId);
    },
  });
  body.appendChild(palette.el);

  // グリッドエディタ
  const gridEditor = createGridCanvas({
    sheetBaseUrl,
    tileSize,
    sheetTileWidth,
    sheetTileHeight,
    parts,
    layerMode,
    onDirty() {
      isDirty = true;
      saveBtn.classList.add("has-changes");
    },
    onRightClick(partsId) {
      palette.setSelectedId(partsId);
      gridEditor.setBrush(partsId);
      if (notifyFrame) notifyFrame(partsId);
    },
  });
  body.appendChild(gridEditor.el);

  root.appendChild(body);

  // ---- 初期マップ表示 ----
  function loadMap(mapId) {
    const map = maps.find((m) => String(m.id) === String(mapId));
    gridEditor.setMap(map || null);
    isDirty = false;
    saveBtn.classList.remove("has-changes");
    setFeedback("", "");
    selectedMapId = mapId;
    if (onMapChange) onMapChange(mapId);
  }

  if (selectedMapId) {
    loadMap(selectedMapId);
  }

  // ---- イベント ----
  mapSelect.addEventListener("change", () => {
    const newId = mapSelect.value;
    if (isDirty) {
      if (!window.confirm("保存されていない変更があります。マップを切り替えますか？")) {
        mapSelect.value = String(selectedMapId);
        return;
      }
    }
    loadMap(newId);
  });

  function setFeedback(msg, type) {
    feedbackEl.textContent = msg;
    feedbackEl.className = "mp-topbar-feedback";
    if (type) feedbackEl.classList.add(type);
  }

  saveBtn.addEventListener("click", async () => {
    if (!selectedMapId) {
      setFeedback("マップを選択してください", "error");
      return;
    }
    if (!onSave) return;
    saveBtn.disabled = true;
    setFeedback("保存中...", "");
    try {
      const { tilesBase, tilesPile } = gridEditor.getGrid();
      await onSave(selectedMapId, tilesBase, tilesPile);
      isDirty = false;
      saveBtn.classList.remove("has-changes");
      setFeedback("保存しました", "success");
    } catch (err) {
      setFeedback(`保存失敗: ${err.message || err}`, "error");
    } finally {
      saveBtn.disabled = false;
    }
  });

  undoBtn.addEventListener("click", () => {
    // Ctrl+Z のキーハンドラと同じ処理 (内部ではキーボードイベント経由)
    window.dispatchEvent(new KeyboardEvent("keydown", { key: "z", ctrlKey: true, bubbles: true }));
  });

  let currentZoom = 2;
  zoom1Btn.addEventListener("click", () => {
    currentZoom = 1;
    zoom1Btn.classList.add("is-active");
    zoom2Btn.classList.remove("is-active");
    gridEditor.setZoom(1);
  });
  zoom2Btn.addEventListener("click", () => {
    currentZoom = 2;
    zoom2Btn.classList.add("is-active");
    zoom1Btn.classList.remove("is-active");
    gridEditor.setZoom(2);
  });

  let gridVisible = true;
  gridToggle.addEventListener("click", () => {
    gridVisible = !gridVisible;
    gridToggle.textContent = gridVisible ? "グリッド: ON" : "グリッド: OFF";
    gridToggle.classList.toggle("is-active", gridVisible);
    gridEditor.setShowGrid(gridVisible);
  });

  // ---- 公開 API ----

  /** ゲーム iframe からスポイトで拾ったパーツIDを反映 */
  function pickFromFrame(partsId) {
    palette.setSelectedId(partsId);
    gridEditor.setBrush(partsId);
  }

  function destroy() {
    gridEditor.destroy();
    root.remove();
  }

  return {
    el: root,
    pickFromFrame,
    destroy,
    getSelectedMapId() { return selectedMapId; },
  };
}

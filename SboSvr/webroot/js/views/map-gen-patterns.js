/**
 * views/map-gen-patterns.js
 * 自動生成マップ設定 一覧・編集画面 (route: map-gen-patterns)
 *
 * API: /api/map-gen-patterns
 *   GET    → { items: [{ patternId, patternName, algoType, paramsJson, roleMapJson }] }
 *   POST   → 新規 (patternName 必須)
 *   PUT    → 更新 (patternId 必須)
 *   DELETE → { patternId }
 *
 * paramsJson / roleMapJson は JSON 文字列として往復する。
 * 保存時: JSON.stringify した文字列を body に含める。
 * 読込時: JSON.parse してフォームへ反映（空・不正なら既定値にフォールバック）。
 *
 * ※ プレビュー（お試し生成）は S2 で追加予定。
 *
 * レイアウト: list-detail 骨格（item-types.js と同じ構造）
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField, openSpritePicker } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ================================================================
// プレビュー描画定数（map-paint.js と同じ規約）
// ================================================================

/** 1タイルのゲーム内ピクセルサイズ（map-paint.js と同値） */
const PREVIEW_TILE_SIZE = 16;
/** シート横タイル数（map-paint.js と同値） */
const PREVIEW_SHEET_TILE_WIDTH = 32;
/** マップパーツシート画像の基底URL */
const PREVIEW_SHEET_BASE_URL = "/api/assets/map-parts/sheets";

// ================================================================
// プレビュー描画ユーティリティ
// ================================================================

/**
 * grpId → { sheet, tileX, tileY } に分解する（map-paint.js の grpIdToCoord と同じ規約）。
 * grpId = sheet*1024 + tileX + tileY*sheetTileWidth
 * @param {number} grpId
 * @returns {{ sheet:number, tileX:number, tileY:number }|null}
 */
function previewGrpIdToCoord(grpId) {
  if (!grpId) return null;
  const sheet = Math.floor(grpId / 1024);
  const tile  = grpId % 1024;
  return {
    sheet,
    tileX: tile % PREVIEW_SHEET_TILE_WIDTH,
    tileY: Math.floor(tile / PREVIEW_SHEET_TILE_WIDTH),
  };
}

/**
 * シート画像をキャッシュしながら返す（Map<url, HTMLImageElement>）。
 * 読み込み完了時に onLoad コールバックを呼ぶ。
 */
function createPreviewImageCache() {
  const cache = new Map(); // url → HTMLImageElement

  function getOrLoad(sheetIndex, onLoad) {
    const url = `${PREVIEW_SHEET_BASE_URL}/${sheetIndex}.png`;
    if (cache.has(url)) return cache.get(url);
    const img = new Image();
    img.src = url;
    img.onload = onLoad;
    cache.set(url, img);
    return img;
  }

  return { getOrLoad };
}

/**
 * プレビュー canvas を描画するクロージャを生成する。
 * @param {HTMLCanvasElement} canvas
 * @returns {{ draw(result: {width,height,tilesBase:number[]}): void }}
 */
function createPreviewDrawer(canvas) {
  const imgCache = createPreviewImageCache();
  const ctx = canvas.getContext("2d");
  let _lastResult = null;

  /** raf ごとに1回だけ draw() を実行する */
  let rafPending = false;
  function requestDraw() {
    if (!rafPending) {
      rafPending = true;
      requestAnimationFrame(() => { rafPending = false; doDraw(); });
    }
  }

  function doDraw() {
    if (!_lastResult) return;
    const { width, height, tilesBase } = _lastResult;
    const ts = PREVIEW_TILE_SIZE; // 1タイルのpx

    canvas.width  = width  * ts;
    canvas.height = height * ts;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.imageSmoothingEnabled = false;

    for (let row = 0; row < height; row++) {
      for (let col = 0; col < width; col++) {
        const grpId = tilesBase[row * width + col] || 0;
        if (!grpId) continue; // 0=透明、スキップ
        const coord = previewGrpIdToCoord(grpId);
        if (!coord) continue;
        const img = imgCache.getOrLoad(coord.sheet, requestDraw);
        if (img && img.complete && img.naturalWidth) {
          ctx.drawImage(
            img,
            coord.tileX * ts, coord.tileY * ts, ts, ts,
            col * ts,         row * ts,         ts, ts
          );
        }
      }
    }
  }

  /**
   * 生成結果をセットして描画を開始する。
   * @param {{ width:number, height:number, tilesBase:number[] }} result
   */
  function draw(result) {
    _lastResult = result;
    requestDraw();
  }

  return { draw };
}

// ================================================================
// 定数
// ================================================================

/** アルゴリズム種別の選択肢 */
const ALGO_TYPE_LABELS = {
  0: "洞窟 (L3)",
  1: "聖堂 (L1)",
  2: "地下墓地 (L2)",
  3: "地獄 (L4)",
};

/** アルゴリズム種別ごとの補足説明文 */
const ALGO_TYPE_DESC = {
  0: "中央から再帰ブロック分割、有機的な洞窟形状を生成",
  1: "格子状部屋配置、神聖な聖堂・教会風レイアウト",
  2: "迷路状通路と小部屋の組み合わせ、地下墓地風構造",
  3: "溶岩地形を模した大空洞・多層分岐レイアウト",
};

/** paramsJson の既定値（algoType=0 用） */
const DEFAULT_PARAMS = {
  width: 40,
  height: 40,
  floorAreaMin: 150,
  blockMin: 3,
  blockMax: 4,
  cutoffPercent: 25,
};

/**
 * アルゴ種別ごとの推奨パラメータ。
 * floorAreaMin は「グリッド面積 × fillPct」で算出する（マップサイズに比例）。
 * fillPct は各アルゴが確実に成功する保守的な値（全サイズ50/50実測）。
 *   L3洞窟/L1聖堂は密、L2墓地/L4地獄は構造上どうしても疎。
 */
const ALGO_PRESETS = {
  0: { fillPct: 0.08,  blockMin: 3, blockMax: 4,  cutoffPercent: 25 }, // L3洞窟
  1: { fillPct: 0.18,  blockMin: 4, blockMax: 6,  cutoffPercent: 25 }, // L1聖堂
  2: { fillPct: 0.02,  blockMin: 5, blockMax: 10, cutoffPercent: 25 }, // L2地下墓地
  3: { fillPct: 0.012, blockMin: 6, blockMax: 12, cutoffPercent: 25 }, // L4地獄
};

/** roleMapJson の既定値 */
const DEFAULT_ROLE_MAP = {
  floor: 0,
  wall: 0,
  door: 0,
  hall: 0,
  stairs: 0,
};

// ================================================================
// 縁取りセット（edge13）関連の定数・ユーティリティ
// ================================================================

/**
 * 縁取りセットの13スロット定義。
 * key: RoleMapJson v2 の tiles オブジェクトのキー
 * label: UI表示用の短いラベル
 * grid: 3×3配置での [col,row]（0-2）。内角4個はグリッド外表示のため null。
 */
const EDGE13_SLOTS = [
  { key: "nw", label: "外角(左上)", grid: [0, 0] },
  { key: "n",  label: "辺(上)",     grid: [1, 0] },
  { key: "ne", label: "外角(右上)", grid: [2, 0] },
  { key: "w",  label: "辺(左)",     grid: [0, 1] },
  { key: "c",  label: "中央",       grid: [1, 1] },
  { key: "e",  label: "辺(右)",     grid: [2, 1] },
  { key: "sw", label: "外角(左下)", grid: [0, 2] },
  { key: "s",  label: "辺(下)",     grid: [1, 2] },
  { key: "se", label: "外角(右下)", grid: [2, 2] },
  { key: "inNW", label: "内角(左上)", grid: null },
  { key: "inNE", label: "内角(右上)", grid: null },
  { key: "inSW", label: "内角(左下)", grid: null },
  { key: "inSE", label: "内角(右下)", grid: null },
];

/** アンカー（nwスロット）からの自動充填対象キー一覧 */
const EDGE13_ANCHOR_FILL_KEYS = ["nw", "n", "ne", "w", "c", "e", "sw", "s", "se"];

/** アンカー（同一シート内の grpId offset）: nw=+0 を基準とした相対 offset */
const EDGE13_ANCHOR_OFFSETS = {
  nw: 0,  n: 1,  ne: 2,
  w: 32,  c: 33, e: 34,
  sw: 64, s: 65, se: 66,
};

/** 内角4スロットのキー（アンカー充填対象外・既定0） */
const EDGE13_INNER_KEYS = ["inNW", "inNE", "inSW", "inSE"];

/** 縁取りセットのシート幅（map-paint.js の規約と同じ、固定32タイル） */
const EDGE13_SHEET_TILE_WIDTH = 32;

/**
 * 値が縁取りセット（オブジェクト形式）かどうかを判定する。
 * @param {number|object|undefined} v
 */
function isEdgeSetValue(v) {
  return v != null && typeof v === "object" && v.mode === "edge13";
}

/**
 * 縁取りセットの既定オブジェクトを生成する（13スロットすべて0）。
 * @returns {{ mode: "edge13", tiles: object }}
 */
function makeDefaultEdgeSet() {
  const tiles = {};
  EDGE13_SLOTS.forEach(({ key }) => { tiles[key] = 0; });
  return { mode: "edge13", tiles };
}

/**
 * roleMapJson の1ロール分の値を { mode:"single"|"edge13", single, tiles } に正規化する。
 * 後方互換: 数値ならsingle、edge13オブジェクトならそのまま、それ以外は既定(single 0)。
 * @param {number|object|undefined} v
 */
function normalizeRoleValue(v) {
  if (isEdgeSetValue(v)) {
    const def = makeDefaultEdgeSet();
    return { mode: "edge13", single: 0, tiles: { ...def.tiles, ...(v.tiles || {}) } };
  }
  const single = typeof v === "number" ? v : 0;
  return { mode: "single", single, tiles: makeDefaultEdgeSet().tiles };
}

/**
 * grpId からシート内 (tileX, tileY) を算出する（map-paint.js の grpIdToCoord と同じ規約）。
 * @param {number} grpId
 */
function edge13GrpIdToXY(grpId) {
  const tile = grpId % 1024;
  return {
    sheet: Math.floor(grpId / 1024),
    tileX: tile % EDGE13_SHEET_TILE_WIDTH,
    tileY: Math.floor(tile / EDGE13_SHEET_TILE_WIDTH),
  };
}

/**
 * アンカー grpId（nwスロットの値）から3×3ブロック9スロット分の grpId を算出する。
 * シート右端・下端をはみ出す場合は null を返す（呼び出し側で警告表示）。
 * @param {number} anchorGrpId
 * @returns {object|null} { nw, n, ne, w, c, e, sw, s, se } の grpId マップ、または null
 */
function fillEdge13FromAnchor(anchorGrpId) {
  const { sheet, tileX, tileY } = edge13GrpIdToXY(anchorGrpId);
  // nw を左上として3列3行を使うため、tileX <= 29 かつ tileY <= 29 が必要
  if (tileX > EDGE13_SHEET_TILE_WIDTH - 3 || tileY > EDGE13_SHEET_TILE_WIDTH - 3) {
    return null;
  }
  const result = {};
  EDGE13_ANCHOR_FILL_KEYS.forEach((key) => {
    result[key] = anchorGrpId + EDGE13_ANCHOR_OFFSETS[key];
  });
  return result;
}

// ================================================================
// ユーティリティ
// ================================================================

/** フィードバックメッセージ表示 */
function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "it-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

/** label+span のフォームフィールド雛形 */
function makeFormField(labelText) {
  const lbl = document.createElement("label");
  lbl.className = "form-field";
  const span = document.createElement("span");
  span.textContent = labelText;
  lbl.appendChild(span);
  return lbl;
}

/**
 * JSON 文字列を安全にパースし、パース失敗または null なら fallback を返す。
 * @param {string|null|undefined} jsonStr
 * @param {object} fallback
 */
function safeParseJson(jsonStr, fallback) {
  if (!jsonStr) return { ...fallback };
  try {
    const parsed = JSON.parse(jsonStr);
    if (typeof parsed !== "object" || parsed === null) return { ...fallback };
    // 既定値で抜けているキーを補完
    return { ...fallback, ...parsed };
  } catch {
    return { ...fallback };
  }
}

// ================================================================
// 右ペイン: 詳細フォーム
// ================================================================

function buildDetailPane({ feedbackEl }) {
  const pane = document.createElement("div");
  pane.className = "ee-right";

  // フィードバック表示エリア
  pane.appendChild(feedbackEl);

  // 保存バー
  const actionBar = document.createElement("div");
  actionBar.className = "me-action-bar";
  const saveBtn = document.createElement("button");
  saveBtn.type = "button";
  saveBtn.className = "button primary";
  saveBtn.textContent = "保存";
  const cancelBtn = document.createElement("button");
  cancelBtn.type = "button";
  cancelBtn.className = "button";
  cancelBtn.textContent = "キャンセル / 新規";
  actionBar.append(saveBtn, cancelBtn);
  pane.appendChild(actionBar);

  // ================================================================
  // 基本情報セクション
  // ================================================================
  const basicSec = document.createElement("section");
  basicSec.className = "detail-section";
  const basicH3 = document.createElement("h3");
  basicH3.textContent = "基本情報";
  basicSec.appendChild(basicH3);

  const basicGrid = document.createElement("div");
  basicGrid.className = "form-grid compact";

  // パターン名
  const nameLbl = makeFormField("パターン名");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 64;
  nameInput.placeholder = "パターン名（必須）";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  // アルゴリズム種別
  const algoLbl = makeFormField("アルゴリズム種別");
  const algoSelect = document.createElement("select");
  Object.entries(ALGO_TYPE_LABELS).forEach(([v, t]) => {
    const opt = document.createElement("option");
    opt.value = v;
    opt.textContent = v + ": " + t;
    algoSelect.appendChild(opt);
  });
  algoLbl.appendChild(algoSelect);
  basicGrid.appendChild(algoLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // ================================================================
  // 生成パラメータセクション
  // ================================================================
  const paramSec = document.createElement("section");
  paramSec.className = "detail-section";
  const paramH3 = document.createElement("h3");
  paramH3.textContent = "生成パラメータ";
  paramSec.appendChild(paramH3);

  // アルゴ別補足説明文（1行）
  const algoDescEl = document.createElement("p");
  algoDescEl.className = "muted";
  algoDescEl.style.fontSize = "0.85em";
  algoDescEl.style.margin = "0 0 8px";
  paramSec.appendChild(algoDescEl);

  const paramGrid = document.createElement("div");
  paramGrid.className = "form-grid compact";

  /** スピナー付きフォームフィールドを追加するヘルパ。label要素を返す */
  function addParamSpinner(labelText, min, max, defaultVal) {
    const lbl = makeFormField(labelText);
    const spin = createNumberSpinner({ value: defaultVal, min, max, step: 1 });
    lbl.appendChild(spin.el);
    paramGrid.appendChild(lbl);
    return { spin, lbl };
  }

  const { spin: widthSpin }        = addParamSpinner("マップ幅 (width)",              1, 9999, DEFAULT_PARAMS.width);
  const { spin: heightSpin }       = addParamSpinner("マップ高さ (height)",            1, 9999, DEFAULT_PARAMS.height);
  const { spin: floorAreaMinSpin } = addParamSpinner("最小床面積 (floorAreaMin)",       0, 9999, DEFAULT_PARAMS.floorAreaMin);
  // blockMin/blockMax はアルゴ別にラベルが変わるため label 要素も保持する
  const { spin: blockMinSpin, lbl: blockMinLbl } = addParamSpinner("ブロック最小サイズ (blockMin)", 1, 9999, DEFAULT_PARAMS.blockMin);
  const { spin: blockMaxSpin, lbl: blockMaxLbl } = addParamSpinner("ブロック最大サイズ (blockMax)", 1, 9999, DEFAULT_PARAMS.blockMax);
  // cutoffPercent は L3(0) 専用。非L3では display:none にする
  const { spin: cutoffSpin, lbl: cutoffLbl }     = addParamSpinner("カットオフ割合% (cutoffPercent)", 0, 100, DEFAULT_PARAMS.cutoffPercent);

  paramSec.appendChild(paramGrid);
  pane.appendChild(paramSec);

  /**
   * algoType に合わせてパラメータUIを更新する。
   * 値は保持したまま label/visibility だけ切り替える。
   * @param {number} type
   */
  function applyAlgoType(type) {
    // 補足説明文
    algoDescEl.textContent = ALGO_TYPE_DESC[type] ?? "";

    if (type === 0) {
      // L3(洞窟): blockMin/blockMax はブロックサイズ、cutoffPercent を表示
      blockMinLbl.querySelector("span").textContent = "ブロック最小サイズ (blockMin)";
      blockMaxLbl.querySelector("span").textContent = "ブロック最大サイズ (blockMax)";
      cutoffLbl.style.display = "";
    } else {
      // L1/L2/L4(聖堂・地下墓地・地獄): blockMin/blockMax を「部屋サイズ範囲」として流用
      blockMinLbl.querySelector("span").textContent = "部屋サイズ下限 (blockMin)";
      blockMaxLbl.querySelector("span").textContent = "部屋サイズ上限 (blockMax)";
      cutoffLbl.style.display = "none";
    }
  }

  /**
   * アルゴ種別の推奨パラメータを各スピナーに流し込む。
   * floorAreaMin は現在の width×height からスケール算出する。
   * 注意: パターン読込時は呼ばない（保存値を上書きしないため）。algo変更イベント専用。
   * @param {number} type
   */
  function applyAlgoPresets(type) {
    const preset = ALGO_PRESETS[type];
    if (!preset) return;
    const w = widthSpin.getValue();
    const h = heightSpin.getValue();
    let fam = Math.round(w * h * preset.fillPct);
    if (fam < 50) fam = 50;
    floorAreaMinSpin.setValue(fam);
    blockMinSpin.setValue(preset.blockMin);
    blockMaxSpin.setValue(preset.blockMax);
    cutoffSpin.setValue(preset.cutoffPercent);
  }

  // アルゴ種別変更イベント
  algoSelect.addEventListener("change", () => {
    const t = Number(algoSelect.value);
    applyAlgoType(t);
    applyAlgoPresets(t);
  });

  // ================================================================
  // ロール→マップパーツ割り当てセクション
  // ================================================================
  const roleSec = document.createElement("section");
  roleSec.className = "detail-section";
  const roleH3 = document.createElement("h3");
  roleH3.textContent = "ロール → マップパーツ割り当て";
  roleSec.appendChild(roleH3);

  const roleGrid = document.createElement("div");
  roleGrid.className = "form-grid compact";

  /** ロール名 → ラベルのマッピング（roleMapJson のキー順） */
  const ROLE_LABELS = [
    { key: "floor",  label: "床" },
    { key: "wall",   label: "壁" },
    { key: "door",   label: "ドア" },
    { key: "hall",   label: "廊下" },
    { key: "stairs", label: "階段" },
  ];

  /**
   * ロール1件分の「単一 / 縁取りセット」割り当てUIを生成する。
   * 単一用 sprite-field と 縁取りセット用13スロットグリッドの両方を作り、
   * モード切替セレクトで表示/非表示を切り替える。
   * @param {string} label
   */
  function createRoleAssignField(label) {
    const wrap = document.createElement("div");
    wrap.className = "role-assign-field";

    const headRow = document.createElement("div");
    headRow.className = "role-assign-head";
    const lbl = document.createElement("span");
    lbl.className = "sprite-field-label";
    lbl.textContent = label;
    headRow.appendChild(lbl);

    // モード切替セレクト
    const modeSelect = document.createElement("select");
    const optSingle = document.createElement("option");
    optSingle.value = "single";
    optSingle.textContent = "単一";
    const optEdge = document.createElement("option");
    optEdge.value = "edge13";
    optEdge.textContent = "縁取りセット";
    modeSelect.append(optSingle, optEdge);
    headRow.appendChild(modeSelect);
    wrap.appendChild(headRow);

    // --- 単一モード用 sprite-field ---
    const singleField = createSpriteField({
      categoryKey: "mapParts",
      value: 0,
    });
    wrap.appendChild(singleField.el);

    // --- 縁取りセットモード用 UI ---
    const edgeWrap = document.createElement("div");
    edgeWrap.className = "edge13-wrap";

    // アンカー選択ボタン + 警告表示
    const anchorRow = document.createElement("div");
    anchorRow.className = "edge13-anchor-row";
    const anchorBtn = document.createElement("button");
    anchorBtn.type = "button";
    anchorBtn.className = "button small";
    anchorBtn.textContent = "アンカーから自動充填";
    const anchorWarn = document.createElement("span");
    anchorWarn.className = "edge13-anchor-warn muted";
    anchorWarn.style.display = "none";
    anchorWarn.textContent = "アンカーがシート端に近すぎるため自動充填できません（3×3が収まる位置を選んでください）";
    anchorRow.append(anchorBtn, anchorWarn);
    edgeWrap.appendChild(anchorRow);

    // 3×3グリッド（外角/辺/中央）
    const mainGrid = document.createElement("div");
    mainGrid.className = "edge13-grid";

    // 内角4個（3×3の外側の別枠）
    const innerGrid = document.createElement("div");
    innerGrid.className = "edge13-grid edge13-grid-inner";
    const innerCaption = document.createElement("p");
    innerCaption.className = "edge13-inner-caption muted";
    innerCaption.textContent = "内角（アンカー充填対象外）";

    /** key → { thumb, slotEl, grpId } */
    const slotEntries = {};

    EDGE13_SLOTS.forEach(({ key, label: slotLabel, grid }) => {
      const slotEl = document.createElement("div");
      slotEl.className = "edge13-slot" + (grid ? "" : " edge13-slot-inner");
      if (grid) {
        slotEl.style.gridColumn = String(grid[0] + 1);
        slotEl.style.gridRow = String(grid[1] + 1);
      }
      slotEl.title = slotLabel;

      const thumb = createSpriteThumb({ categoryKey: "mapParts", sub: 0, size: 32 });
      thumb.el.style.cursor = "pointer";
      thumb.el.title = slotLabel + "（クリックして選択）";
      thumb.el.setAttribute("role", "button");
      thumb.el.setAttribute("tabindex", "0");

      const caption = document.createElement("span");
      caption.className = "edge13-slot-caption";
      caption.textContent = slotLabel;

      slotEl.append(thumb.el, caption);

      // スロットクリックで個別差し替え
      function openSlotPicker() {
        openSpritePicker({
          categoryKey: "mapParts",
          current: slotEntries[key].grpId,
          onSelect: ({ sub }) => {
            slotEntries[key].grpId = sub;
            thumb.update({ categoryKey: "mapParts", sub });
          },
        });
      }
      thumb.el.addEventListener("click", openSlotPicker);
      thumb.el.addEventListener("keydown", (e) => {
        if (e.key === " " || e.key === "Enter") { e.preventDefault(); openSlotPicker(); }
      });

      (grid ? mainGrid : innerGrid).appendChild(slotEl);
      slotEntries[key] = { thumb, slotEl, grpId: 0 };
    });

    edgeWrap.append(mainGrid, innerCaption, innerGrid);
    wrap.appendChild(edgeWrap);

    // --- アンカー選択ボタンのイベント ---
    anchorBtn.addEventListener("click", () => {
      openSpritePicker({
        categoryKey: "mapParts",
        current: slotEntries.nw.grpId,
        onSelect: ({ sub }) => {
          const filled = fillEdge13FromAnchor(sub);
          if (!filled) {
            anchorWarn.style.display = "";
            return;
          }
          anchorWarn.style.display = "none";
          EDGE13_ANCHOR_FILL_KEYS.forEach((key) => {
            slotEntries[key].grpId = filled[key];
            slotEntries[key].thumb.update({ categoryKey: "mapParts", sub: filled[key] });
          });
        },
      });
    });

    /** モード表示切替（値は保持したまま見た目だけ切り替える） */
    function applyMode(mode) {
      modeSelect.value = mode;
      singleField.el.style.display = mode === "single" ? "" : "none";
      edgeWrap.style.display = mode === "edge13" ? "" : "none";
      if (mode !== "edge13") anchorWarn.style.display = "none";
    }

    modeSelect.addEventListener("change", () => applyMode(modeSelect.value));

    /** 現在値を取得する（数値 or { mode:"edge13", tiles }） */
    function getValue() {
      if (modeSelect.value === "single") {
        return singleField.getValue();
      }
      const tiles = {};
      EDGE13_SLOTS.forEach(({ key }) => { tiles[key] = slotEntries[key].grpId; });
      return { mode: "edge13", tiles };
    }

    /** 値をセットする（数値 or { mode:"edge13", tiles }。後方互換フォールバックあり） */
    function setValue(v) {
      const norm = normalizeRoleValue(v);
      singleField.setValue(norm.single);
      EDGE13_SLOTS.forEach(({ key }) => {
        const grpId = norm.tiles[key] ?? 0;
        slotEntries[key].grpId = grpId;
        slotEntries[key].thumb.update({ categoryKey: "mapParts", sub: grpId });
      });
      applyMode(norm.mode);
    }

    // 初期表示は単一モード
    applyMode("single");

    return { el: wrap, getValue, setValue };
  }

  // 各ロールの割り当てフィールドを生成
  const roleFields = {};
  ROLE_LABELS.forEach(({ key, label }) => {
    const rf = createRoleAssignField(label);
    roleGrid.appendChild(rf.el);
    roleFields[key] = rf;
  });

  roleSec.appendChild(roleGrid);
  pane.appendChild(roleSec);

  // ================================================================
  // プレビューセクション（S2 追加）
  // ================================================================
  const previewSec = document.createElement("section");
  previewSec.className = "detail-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "お試し生成プレビュー";
  previewSec.appendChild(previewH3);

  // --- シード入力行 ---
  const seedRow = document.createElement("div");
  seedRow.className = "form-grid compact";

  const seedLbl = makeFormField("シード（空=ランダム）");
  const seedInput = document.createElement("input");
  seedInput.type = "number";
  seedInput.min = "0";
  seedInput.placeholder = "空欄でランダム";
  seedInput.className = "form-input";
  seedLbl.appendChild(seedInput);
  seedRow.appendChild(seedLbl);
  previewSec.appendChild(seedRow);

  // --- ボタン行 ---
  const previewBtnBar = document.createElement("div");
  previewBtnBar.className = "me-action-bar";

  const tryGenBtn = document.createElement("button");
  tryGenBtn.type = "button";
  tryGenBtn.className = "button";
  tryGenBtn.textContent = "お試し生成";

  const regenBtn = document.createElement("button");
  regenBtn.type = "button";
  regenBtn.className = "button";
  regenBtn.textContent = "再生成（別シード）";

  previewBtnBar.append(tryGenBtn, regenBtn);
  previewSec.appendChild(previewBtnBar);

  // --- プレビューフィードバック行 ---
  const previewFeedback = document.createElement("p");
  previewFeedback.className = "it-feedback result-message";
  previewFeedback.style.display = "none";
  previewSec.appendChild(previewFeedback);

  // --- プレビュー canvas ---
  const previewCanvas = document.createElement("canvas");
  previewCanvas.style.display = "none";
  previewCanvas.style.maxWidth = "100%";
  previewCanvas.style.border = "1px solid var(--border-color, #ccc)";
  previewCanvas.style.marginTop = "8px";
  previewSec.appendChild(previewCanvas);

  pane.appendChild(previewSec);

  // --- プレビュー描画インスタンス ---
  const previewDrawer = createPreviewDrawer(previewCanvas);

  /**
   * プレビュー API を呼び出してcanvasに描画する。
   * @param {number|null} seed  null = 未指定（ランダム）
   */
  async function runPreview(seed) {
    // フォームの現在値を収集
    const paramsObj = {
      width:          widthSpin.getValue(),
      height:         heightSpin.getValue(),
      floorAreaMin:   floorAreaMinSpin.getValue(),
      blockMin:       blockMinSpin.getValue(),
      blockMax:       blockMaxSpin.getValue(),
      cutoffPercent:  cutoffSpin.getValue(),
    };
    const roleMapObj = {};
    ROLE_LABELS.forEach(({ key }) => {
      roleMapObj[key] = roleFields[key].getValue();
    });

    // リクエストボディ組み立て（既存パターン編集中なら patternId を付与）
    // algoType は DB取得経路でも分岐するため常に body に乗せる
    const body = {
      algoType: Number(algoSelect.value),
    };
    if (_current?.patternId) {
      body.patternId = _current.patternId;
    } else {
      body.params  = paramsObj;
      body.roleMap = roleMapObj;
    }
    if (seed != null) body.seed = seed;

    // フィードバック: 生成中
    previewFeedback.textContent = "生成中…";
    previewFeedback.className = "it-feedback result-message";
    previewFeedback.style.display = "";
    tryGenBtn.disabled = true;
    regenBtn.disabled  = true;

    try {
      const { response, data } = await fetchJson("/api/map-gen-patterns/preview", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });

      if (!response.ok) {
        const errMsg = data?.error ?? data?.message ?? "HTTP " + response.status;
        previewFeedback.textContent = "生成失敗: " + errMsg;
        previewFeedback.className = "it-feedback result-message error";
        previewCanvas.style.display = "none";
        return;
      }

      // 成功: canvas 描画
      const { width, height, seed: usedSeed, tilesBase } = data;
      const floorCount = tilesBase.filter(g => g !== 0).length;
      previewFeedback.textContent =
        `生成成功 (seed=${usedSeed}, 床=${floorCount}タイル, ${width}×${height})`;
      previewFeedback.className = "it-feedback result-message success";

      // シード入力欄に使用シードを反映（次回「お試し生成」で同じ結果を再現できるように）
      seedInput.value = String(usedSeed);

      previewCanvas.style.display = "";
      previewDrawer.draw({ width, height, tilesBase });

    } catch (e) {
      previewFeedback.textContent = "通信エラー: " + e.message;
      previewFeedback.className = "it-feedback result-message error";
      previewCanvas.style.display = "none";
    } finally {
      tryGenBtn.disabled = false;
      regenBtn.disabled  = false;
    }
  }

  // --- ボタンイベント ---

  // お試し生成: シード入力欄の値を使用（空ならランダム）
  tryGenBtn.addEventListener("click", () => {
    const rawSeed = seedInput.value.trim();
    const seed = rawSeed !== "" ? Number(rawSeed) : null;
    runPreview(seed);
  });

  // 再生成（別シード）: seed を未指定で投げる（毎回ランダム）
  regenBtn.addEventListener("click", () => {
    runPreview(null);
  });

  // ================================================================
  // 公開 API
  // ================================================================

  let _current = null;

  /** レコードをフォームに反映する */
  function setItem(it) {
    _current = it || null;

    nameInput.value = it ? (it.patternName || "") : "";
    const algoType = it ? (it.algoType ?? 0) : 0;
    algoSelect.value = String(algoType);
    applyAlgoType(algoType);

    // paramsJson: JSON 文字列 → オブジェクト（既定値フォールバック付き）
    const params = safeParseJson(it ? it.paramsJson : null, DEFAULT_PARAMS);
    widthSpin.setValue(params.width);
    heightSpin.setValue(params.height);
    floorAreaMinSpin.setValue(params.floorAreaMin);
    blockMinSpin.setValue(params.blockMin);
    blockMaxSpin.setValue(params.blockMax);
    cutoffSpin.setValue(params.cutoffPercent);

    // roleMapJson: JSON 文字列 → オブジェクト（既定値フォールバック付き）
    const roleMap = safeParseJson(it ? it.roleMapJson : null, DEFAULT_ROLE_MAP);
    ROLE_LABELS.forEach(({ key }) => {
      roleFields[key].setValue(roleMap[key] ?? 0);
    });
  }

  /** フォームの現在値を API 送信用オブジェクトに変換する */
  function collectData() {
    // paramsJson: オブジェクト → JSON 文字列
    const paramsObj = {
      width:          widthSpin.getValue(),
      height:         heightSpin.getValue(),
      floorAreaMin:   floorAreaMinSpin.getValue(),
      blockMin:       blockMinSpin.getValue(),
      blockMax:       blockMaxSpin.getValue(),
      cutoffPercent:  cutoffSpin.getValue(),
    };

    // roleMapJson: オブジェクト → JSON 文字列
    const roleMapObj = {};
    ROLE_LABELS.forEach(({ key }) => {
      roleMapObj[key] = roleFields[key].getValue();
    });

    return {
      patternName: nameInput.value,
      algoType:    Number(algoSelect.value),
      paramsJson:  JSON.stringify(paramsObj),
      roleMapJson: JSON.stringify(roleMapObj),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(it) { _current = it; }

  return { el: pane, saveBtn, cancelBtn, setItem, collectData, getCurrent, setCurrent };
}

// ================================================================
// 左ペイン: 一覧
// ================================================================

function buildLeftPane({ onSelect, onNew, onDelete }) {
  const pane = document.createElement("div");
  pane.className = "ee-left";

  // 検索ボックス
  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "ID または 名前で検索…";
  searchInput.className = "ld-search";
  pane.appendChild(searchInput);

  // ツールバー（新規追加ボタン）
  const toolbar = document.createElement("div");
  toolbar.className = "me-list-toolbar";
  const newBtn = document.createElement("button");
  newBtn.type = "button";
  newBtn.className = "button small";
  newBtn.textContent = "+ 新規追加";
  newBtn.addEventListener("click", onNew);
  toolbar.appendChild(newBtn);
  pane.appendChild(toolbar);

  // 一覧リスト
  const listEl = document.createElement("ul");
  listEl.className = "ld-list";
  pane.appendChild(listEl);

  // 件数表示
  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let _allItems = [];
  let _selectedId = null;

  /** API から一覧を再取得してレンダリング */
  async function loadList() {
    const { response, data } = await fetchJson("/api/map-gen-patterns");
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("map-gen-patterns load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  /** 検索フィルタを適用してリストを描画する */
  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((it) => {
      if (!q) return true;
      return String(it.patternId).includes(q) || (it.patternName || "").toLowerCase().includes(q);
    });

    listEl.innerHTML = "";
    filtered.forEach((it) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (it.patternId === _selectedId ? " selected" : "");

      const label = document.createElement("span");
      label.textContent = "[" + it.patternId + "] " + (it.patternName || "(名前なし)");
      li.appendChild(label);

      li.addEventListener("click", () => {
        _selectedId = it.patternId;
        renderList();
        onSelect(it);
      });

      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); onDelete(it); });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });

    summary.textContent = filtered.length + " 件";
  }

  searchInput.addEventListener("input", renderList);
  loadList();

  return { el: pane, reload: loadList };
}

// ================================================================
// mount（画面エントリポイント）
// ================================================================

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const shell = document.createElement("div");
  shell.className = "me-shell";
  container.appendChild(shell);

  const feedbackEl = document.createElement("p");
  feedbackEl.className = "it-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  const detail = buildDetailPane({ feedbackEl });

  // 詳細ペイン先頭に「← 戻る」ボタンを追加
  const backBar = document.createElement("div");
  backBar.className = "me-action-bar";
  const backBtn = document.createElement("button");
  backBtn.type = "button";
  backBtn.className = "button small";
  backBtn.textContent = "← 戻る";
  backBar.appendChild(backBtn);
  detail.el.insertBefore(backBar, detail.el.firstChild);

  // 一覧↔詳細の表示切替ヘルパ
  function showDetail() {
    leftApi.el.style.display = "none";
    detail.el.style.display = "";
  }
  function showList() {
    detail.el.style.display = "none";
    leftApi.el.style.display = "";
  }

  backBtn.addEventListener("click", showList);

  // 保存ボタン
  detail.saveBtn.addEventListener("click", async () => {
    const payload = detail.collectData();
    const current = detail.getCurrent();
    const isNew = !current?.patternId;

    if (!payload.patternName.trim()) {
      showFeedback(feedbackEl, "パターン名は必須です", "error");
      return;
    }

    if (!isNew) { payload.patternId = current.patternId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");

    try {
      const { response, data } = await fetchJson("/api/map-gen-patterns", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.patternId) {
        detail.setCurrent({ ...payload, patternId: data.patternId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規ボタン → フォームクリア + 一覧に戻る
  detail.cancelBtn.addEventListener("click", () => {
    detail.setItem(null);
    showFeedback(feedbackEl, "", "");
    showList();
  });

  const leftApi = buildLeftPane({
    onSelect: (it) => {
      detail.setItem(it);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onNew: () => {
      detail.setItem(null);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onDelete: async (it) => {
      if (!confirm("パターン [" + (it.patternName || "") + "] (ID=" + it.patternId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/map-gen-patterns", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ patternId: it.patternId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + it.patternId + ")", "success");
        if (detail.getCurrent()?.patternId === it.patternId) {
          detail.setItem(null);
        }
        await leftApi.reload();
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    },
  });

  // 初期状態: 一覧のみ表示
  detail.el.style.display = "none";

  shell.appendChild(leftApi.el);
  shell.appendChild(detail.el);

  _destroyFn = () => {
    container.innerHTML = "";
  };
}

/**
 * data/assets.js
 * /api/image-categories と /api/sounds のキャッシュ取得、
 * grpSub ⇔ シート/座標変換、シート画像の共有キャッシュを提供する。
 */

import { fetchJson } from "../core/api.js";

/** @type {Array<import('../types.js').ImageCategory>|null} */
let _catalogCache = null;

/** @type {Array<unknown>|null} */
let _soundsCache = null;

/** @type {Map<string, HTMLImageElement>} URL → Image オブジェクト */
const _imageCache = new Map();

// ----------------------------------------------------------------
// カタログ取得
// ----------------------------------------------------------------

/**
 * 画像カテゴリ一覧を取得する(2回目以降はキャッシュを返す)。
 * @returns {Promise<Array>}
 */
export async function loadCatalog() {
  if (_catalogCache !== null) {
    return _catalogCache;
  }
  const { response, data } = await fetchJson("/api/image-categories");
  // API は {"categories":[...]} 形式で返す
  const categories = data && Array.isArray(data.categories) ? data.categories : data;
  if (!response.ok || !Array.isArray(categories)) {
    throw new Error(`/api/image-categories の取得に失敗しました (${response.status})`);
  }
  _catalogCache = categories;
  return _catalogCache;
}

/**
 * サウンド一覧を取得する(2回目以降はキャッシュを返す)。
 * @returns {Promise<Array>}
 */
export async function loadSounds() {
  if (_soundsCache !== null) {
    return _soundsCache;
  }
  const { response, data } = await fetchJson("/api/sounds");
  if (!response.ok || !Array.isArray(data)) {
    throw new Error(`/api/sounds の取得に失敗しました (${response.status})`);
  }
  _soundsCache = data;
  return _soundsCache;
}

/**
 * キャッシュを強制クリアする(テスト・再ロード用)。
 */
export function clearCache() {
  _catalogCache = null;
  _soundsCache = null;
  _imageCache.clear();
}

// ----------------------------------------------------------------
// grpSub ⇔ シート/座標変換
// ----------------------------------------------------------------

/**
 * カテゴリオブジェクトと grpSub 値からスプライト座標情報を計算する。
 *
 * マップパーツは sub/1024 がシートインデックスと一致する特殊カテゴリだが、
 * countX*countY = 1024 に設定されていれば同じ式で算出できる。
 *
 * @param {{ countX: number, countY: number, cellSize: number, sheetUrl: string }} category
 * @param {number} sub  grpSub 値
 * @returns {{ sheetIndex: number, cellIndex: number, x: number, y: number, sheetUrl: string }}
 */
export function calcSpriteCoord(category, sub) {
  const { countX, countY, cellSize, sheetUrl } = category;
  const cellsPerSheet = countX * countY;
  const sheetIndex = Math.floor(sub / cellsPerSheet);
  const cellIndex = sub % cellsPerSheet;
  const x = (cellIndex % countX) * cellSize;
  const y = Math.floor(cellIndex / countX) % countY * cellSize;
  return { sheetIndex, cellIndex, x, y, sheetUrl: `${sheetUrl}/${sheetIndex}` };
}

// ----------------------------------------------------------------
// 画像キャッシュ
// ----------------------------------------------------------------

/**
 * シート画像を取得する。同 URL は共有キャッシュから返す。
 * @param {string} url  完全な画像 URL (sheetUrl + "/" + sheetIndex 形式)
 * @returns {Promise<HTMLImageElement>}
 */
export function loadSheetImage(url) {
  if (_imageCache.has(url)) {
    return Promise.resolve(_imageCache.get(url));
  }
  return new Promise((resolve, reject) => {
    const img = new Image();
    img.onload = () => {
      _imageCache.set(url, img);
      resolve(img);
    };
    img.onerror = () => reject(new Error(`画像の読み込みに失敗しました: ${url}`));
    img.src = url;
  });
}

/**
 * calcSpriteCoord で得た sheetUrl を渡してシート画像を取得するショートカット。
 * @param {{ countX: number, countY: number, cellSize: number, sheetUrl: string }} category
 * @param {number} sub
 * @returns {Promise<{ img: HTMLImageElement, coord: ReturnType<typeof calcSpriteCoord> }>}
 */
export async function loadSpriteImage(category, sub) {
  const coord = calcSpriteCoord(category, sub);
  const img = await loadSheetImage(coord.sheetUrl);
  return { img, coord };
}

/**
 * data/map-parts-data.js
 * GET /api/maps/parts のモジュールレベルキャッシュを提供する。
 * 変更系操作の後は invalidateMapPartsData() を呼んでキャッシュを破棄すること。
 */

import { fetchJson } from "../core/api.js";

/** @type {Promise<object>|null} */
let _cachePromise = null;

/**
 * マップパーツデータを取得する（キャッシュあり）。
 * @param {{ force?: boolean }} [opts]
 * @returns {Promise<object>} レスポンスの data オブジェクト（data.parts を含む）
 */
export function fetchMapPartsData({ force = false } = {}) {
  if (force || !_cachePromise) {
    _cachePromise = fetchJson("/api/maps/parts").then(({ response, data }) => {
      if (!response.ok || !Array.isArray(data?.parts)) {
        _cachePromise = null;
        throw new Error("マップパーツの取得に失敗しました");
      }
      return data;
    }).catch((err) => {
      _cachePromise = null;
      throw err;
    });
  }
  return _cachePromise;
}

/**
 * キャッシュを破棄する。次回 fetchMapPartsData() 呼び出し時にサーバーから再取得される。
 */
export function invalidateMapPartsData() {
  _cachePromise = null;
}

/**
 * components/sprite-thumb.js
 * シートから該当セルを canvas に描画するサムネイルコンポーネント。
 * assets.js の共有 Image キャッシュを利用する。
 */

import { loadCatalog, calcSpriteCoord, loadSheetImage } from "../data/assets.js";

/**
 * スプライトサムネイルを生成する。
 *
 * @param {{
 *   categoryKey: string,
 *   sub: number,
 *   size?: number,
 * }} options
 * @returns {{ el: HTMLElement, update: (sub: number) => void, destroy: () => void }}
 */
export function createSpriteThumb({ categoryKey, sub, size = 32 }) {
  const wrap = document.createElement("span");
  wrap.className = "sprite-thumb";
  wrap.style.width = size + "px";
  wrap.style.height = size + "px";

  const canvas = document.createElement("canvas");
  canvas.width = size;
  canvas.height = size;
  wrap.appendChild(canvas);

  let _currentSub = sub;
  let _category = null;
  let _destroyed = false;
  let _pendingDraw = false;

  // カタログ取得(非同期)
  loadCatalog().then((catalog) => {
    _category = catalog.find((c) => c.key === categoryKey) ?? null;
    if (!_destroyed) {
      _draw(_currentSub);
    }
  }).catch(() => {
    // カタログ取得失敗時はプレースホルダのまま
  });

  function _draw(s) {
    if (!_category) return;
    const coord = calcSpriteCoord(_category, s);
    loadSheetImage(coord.sheetUrl)
      .then((img) => {
        if (_destroyed || _currentSub !== s) return;
        const ctx = canvas.getContext("2d");
        ctx.clearRect(0, 0, size, size);
        ctx.imageSmoothingEnabled = false;
        const cs = _category.cellSize;
        ctx.drawImage(img, coord.x, coord.y, cs, cs, 0, 0, size, size);
      })
      .catch(() => {
        // 画像取得失敗時はクリアのまま
      });
  }

  /** sub 値を変更して再描画する */
  function update(newSub) {
    _currentSub = newSub;
    if (_category) {
      _draw(newSub);
    }
  }

  /** リソースを解放する */
  function destroy() {
    _destroyed = true;
  }

  return { el: wrap, update, destroy };
}

/**
 * IntersectionObserver を使った遅延描画版サムネイルを生成する。
 * 一覧テーブルのサムネ列など大量生成時向け。
 *
 * @param {{ categoryKey: string, sub: number, size?: number }} options
 * @returns {{ el: HTMLElement, update: (sub: number) => void, destroy: () => void }}
 */
export function createSpriteThumbLazy({ categoryKey, sub, size = 32 }) {
  const wrap = document.createElement("span");
  wrap.className = "sprite-thumb";
  wrap.style.width = size + "px";
  wrap.style.height = size + "px";

  const canvas = document.createElement("canvas");
  canvas.width = size;
  canvas.height = size;
  wrap.appendChild(canvas);

  let _currentSub = sub;
  let _category = null;
  let _visible = false;
  let _destroyed = false;
  let _drawn = false;

  loadCatalog().then((catalog) => {
    _category = catalog.find((c) => c.key === categoryKey) ?? null;
    if (!_destroyed && _visible) {
      _draw(_currentSub);
    }
  }).catch(() => {});

  function _draw(s) {
    if (!_category || _destroyed) return;
    _drawn = true;
    const coord = calcSpriteCoord(_category, s);
    loadSheetImage(coord.sheetUrl)
      .then((img) => {
        if (_destroyed || _currentSub !== s) return;
        const ctx = canvas.getContext("2d");
        ctx.clearRect(0, 0, size, size);
        ctx.imageSmoothingEnabled = false;
        const cs = _category.cellSize;
        ctx.drawImage(img, coord.x, coord.y, cs, cs, 0, 0, size, size);
      })
      .catch(() => {});
  }

  const observer = new IntersectionObserver((entries) => {
    if (entries[0].isIntersecting) {
      _visible = true;
      if (_category && !_drawn) _draw(_currentSub);
      observer.disconnect();
    }
  });
  observer.observe(wrap);

  function update(newSub) {
    _currentSub = newSub;
    _drawn = false;
    if (_category && _visible) _draw(newSub);
  }

  function destroy() {
    _destroyed = true;
    observer.disconnect();
  }

  return { el: wrap, update, destroy };
}

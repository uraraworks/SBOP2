/**
 * components/anime-preview.js
 * アニメーションプレビューコンポーネント。
 * 多層スプライトを合成してアニメーション再生する canvas + コントロールバー。
 */

import { loadCatalog, calcSpriteCoord, loadSheetImage } from "../data/assets.js";

/**
 * アニメーションプレビューを生成する。
 *
 * @param {{
 *   width?: number,
 *   height?: number,
 *   scale?: number,
 * }} options
 * @returns {{
 *   el: HTMLElement,
 *   setFrames: (frames: Array<{ layers: Array<{ categoryKey: string, sub: number, offsetX?: number, offsetY?: number, alpha?: number }>, wait: number }>) => void,
 *   destroy: () => void,
 * }}
 */
export function createAnimePreview({ width = 64, height = 64, scale = 2 } = {}) {
  const dispW = width * scale;
  const dispH = height * scale;

  const wrap = document.createElement("div");
  wrap.className = "anime-preview";

  // キャンバス領域
  const canvasWrap = document.createElement("div");
  canvasWrap.className = "anime-preview-canvas-wrap";
  canvasWrap.style.width = dispW + "px";
  canvasWrap.style.height = dispH + "px";

  const canvas = document.createElement("canvas");
  canvas.width = dispW;
  canvas.height = dispH;
  canvasWrap.appendChild(canvas);
  wrap.appendChild(canvasWrap);

  const ctx = canvas.getContext("2d");
  ctx.imageSmoothingEnabled = false;

  // コントロールバー
  const controls = document.createElement("div");
  controls.className = "anime-controls";

  const btnPlay = document.createElement("button");
  btnPlay.type = "button";
  btnPlay.textContent = "▶";
  btnPlay.setAttribute("aria-label", "再生");

  const loopLabel = document.createElement("label");
  const loopCheck = document.createElement("input");
  loopCheck.type = "checkbox";
  loopCheck.checked = true;
  loopLabel.append(loopCheck, "ループ");

  const slider = document.createElement("input");
  slider.type = "range";
  slider.min = "0";
  slider.value = "0";
  slider.className = "anime-frame-slider";

  const counter = document.createElement("span");
  counter.className = "anime-frame-counter";
  counter.textContent = "0 / 0";

  controls.append(btnPlay, loopLabel, slider, counter);
  wrap.appendChild(controls);

  // ----------------------------------------------------------------
  // 内部状態
  // ----------------------------------------------------------------

  /** @type {Array<{ layers: Array<{ categoryKey: string, sub: number, offsetX: number, offsetY: number, alpha: number }>, wait: number }>} */
  let _frames = [];
  let _catalog = null;
  let _frameIndex = 0;
  let _playing = false;
  let _rafId = null;
  let _lastTime = 0;
  let _elapsed = 0;
  let _destroyed = false;

  loadCatalog().then((c) => { _catalog = c; }).catch(() => {});

  // ----------------------------------------------------------------
  // 描画
  // ----------------------------------------------------------------

  async function _drawFrame(index) {
    if (!_frames.length || !_catalog) return;
    const frame = _frames[index];
    if (!frame) return;

    ctx.clearRect(0, 0, dispW, dispH);

    for (const layer of frame.layers) {
      const cat = _catalog.find((c) => c.key === layer.categoryKey);
      if (!cat) continue;
      try {
        const coord = calcSpriteCoord(cat, layer.sub);
        const img = await loadSheetImage(coord.sheetUrl);
        if (_destroyed) return;
        const prevAlpha = ctx.globalAlpha;
        ctx.globalAlpha = layer.alpha ?? 1;
        ctx.drawImage(
          img,
          coord.x, coord.y, cat.cellSize, cat.cellSize,
          (layer.offsetX ?? 0) * scale,
          (layer.offsetY ?? 0) * scale,
          cat.cellSize * scale,
          cat.cellSize * scale
        );
        ctx.globalAlpha = prevAlpha;
      } catch {
        // 描画失敗は無視
      }
    }
  }

  function _updateCounter() {
    counter.textContent = `${_frameIndex + 1} / ${Math.max(1, _frames.length)}`;
    slider.value = String(_frameIndex);
  }

  function _tick(ts) {
    if (_destroyed || !_playing) return;

    if (_lastTime === 0) _lastTime = ts;
    const dt = ts - _lastTime;
    _lastTime = ts;
    _elapsed += dt;

    const frame = _frames[_frameIndex];
    const waitMs = frame ? (frame.wait * 10) : 100;

    if (_elapsed >= waitMs) {
      _elapsed -= waitMs;
      _frameIndex++;
      if (_frameIndex >= _frames.length) {
        if (loopCheck.checked) {
          _frameIndex = 0;
        } else {
          _frameIndex = _frames.length - 1;
          _stopPlay();
          return;
        }
      }
      _updateCounter();
      _drawFrame(_frameIndex);
    }

    _rafId = requestAnimationFrame(_tick);
  }

  function _startPlay() {
    if (_playing || !_frames.length) return;
    _playing = true;
    _lastTime = 0;
    _elapsed = 0;
    btnPlay.textContent = "⏸";
    slider.disabled = true;
    _rafId = requestAnimationFrame(_tick);
  }

  function _stopPlay() {
    if (!_playing) return;
    _playing = false;
    btnPlay.textContent = "▶";
    slider.disabled = false;
    if (_rafId !== null) {
      cancelAnimationFrame(_rafId);
      _rafId = null;
    }
  }

  // ----------------------------------------------------------------
  // イベント
  // ----------------------------------------------------------------

  btnPlay.addEventListener("click", () => {
    if (_playing) { _stopPlay(); } else { _startPlay(); }
  });

  slider.addEventListener("input", () => {
    _frameIndex = parseInt(slider.value, 10);
    _updateCounter();
    _drawFrame(_frameIndex);
  });

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  /**
   * フレームデータをセットする。
   * @param {Array<{ layers: Array<{ categoryKey: string, sub: number, offsetX?: number, offsetY?: number, alpha?: number }>, wait: number }>} frames
   */
  function setFrames(frames) {
    _stopPlay();
    _frames = frames ?? [];
    _frameIndex = 0;
    slider.max = String(Math.max(0, _frames.length - 1));
    slider.value = "0";
    _updateCounter();
    ctx.clearRect(0, 0, dispW, dispH);
    if (_frames.length > 0) _drawFrame(0);
  }

  function destroy() {
    _destroyed = true;
    _stopPlay();
  }

  return { el: wrap, setFrames, destroy };
}

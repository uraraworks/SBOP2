/**
 * views/map-parts-place.js
 * マップパーツ配置画面 (route: map-parts-place)
 *
 * パーツパレットのみ表示。ゲーム画面 iframe 上でクリック配置・スポイトが行える。
 * 「編集」ボタンでパーツ詳細編集をビュー内(右側)に表示する。
 */

import { fetchJson } from "../core/api.js";
import { fetchMapPartsData } from "../data/map-parts-data.js";
import { createPalette } from "./map-paint.js";
import { openPartsDetail } from "./map-parts-edit.js";

// ----------------------------------------------------------------
// データ正規化
// ----------------------------------------------------------------

function normalizeMapPart(raw) {
  if (!raw || typeof raw !== "object") return null;
  const partsId = Number(raw.partsId ?? raw.partsID ?? 0);
  const tileWidth = 32;

  function normSprite(sp) {
    if (!sp || typeof sp.sheet !== "number") return null;
    const sheet = Number(sp.sheet);
    const tileX = Number(sp.tileX ?? 0);
    const tileY = Number(sp.tileY ?? 0);
    const tile = Number.isFinite(Number(sp.tile)) ? Number(sp.tile) : (tileY * tileWidth + tileX);
    return { sheet, tile, tileX, tileY };
  }

  return {
    partsId,
    sprites: {
      base:    normSprite(raw?.sprites?.base),
      overlay: normSprite(raw?.sprites?.overlay),
    },
  };
}

// ----------------------------------------------------------------
// データ取得
// ----------------------------------------------------------------

async function fetchParts() {
  let parts = [];
  let sheetBaseUrl = "/api/assets/map-parts/sheets";
  let tileSize = 16;
  let sheetTileWidth = 32;
  let sheetTileHeight = 32;

  try {
    const data = await fetchMapPartsData();
    parts = data.parts.map(normalizeMapPart).filter(Boolean).sort((a, b) => a.partsId - b.partsId);
    if (data.sheetBaseUrl) sheetBaseUrl = data.sheetBaseUrl;
    if (data.tileSize) tileSize = Number(data.tileSize) || tileSize;
    if (data.sheetTileWidth) sheetTileWidth = Number(data.sheetTileWidth) || sheetTileWidth;
    if (data.sheetTileHeight) sheetTileHeight = Number(data.sheetTileHeight) || sheetTileHeight;
  } catch {
    // parts=[] フォールバック
  }

  return { parts, sheetBaseUrl, tileSize, sheetTileWidth, sheetTileHeight };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;
let _mountToken = 0;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  const token = ++_mountToken;
  container.innerHTML = "";

  // ---- ローディング表示 ----
  const loadingEl = document.createElement("p");
  loadingEl.className = "mp-loading";
  loadingEl.textContent = "パーツデータを読み込み中...";
  container.appendChild(loadingEl);

  // ---- ゲームiframe参照 ----
  const adminGameFrame = document.getElementById("admin-game-frame");

  function notifyFrame(partsId) {
    if (adminGameFrame && adminGameFrame.contentWindow) {
      adminGameFrame.contentWindow.postMessage(
        { kind: "sbop2_set_admin_parts_id", partsId },
        "*"
      );
      adminGameFrame.contentWindow.postMessage(
        { kind: "sbop2_set_admin_mode", mode: 2 },
        "*"
      );
    }
  }

  let palette = null;

  fetchParts().then(({ parts, sheetBaseUrl, tileSize, sheetTileWidth, sheetTileHeight }) => {
    if (token !== _mountToken) return;
    container.innerHTML = "";

    // ---- ツールバー(上部): 見出し + 編集ボタン ----
    const toolbar = document.createElement("div");
    toolbar.className = "mpp-toolbar";
    toolbar.style.cssText =
      "display:flex;align-items:center;gap:8px;flex-shrink:0;" +
      "padding:8px 12px;border-bottom:1px solid var(--color-border,#333);";

    const h2 = document.createElement("h2");
    h2.textContent = "マップパーツ配置";
    h2.style.cssText = "margin:0;font-size:1rem;flex:1;";
    toolbar.appendChild(h2);

    // 編集ボタン（パーツ選択中のみ有効）
    const editBtn = document.createElement("button");
    editBtn.type = "button";
    editBtn.className = "button secondary";
    editBtn.textContent = "編集";
    editBtn.disabled = true;
    editBtn.title = "パーツを選択してから押してください";
    toolbar.appendChild(editBtn);

    container.appendChild(toolbar);

    // ---- パレット領域(残り全体を占有) ----
    const paletteSection = document.createElement("div");
    paletteSection.className = "mpp-palette-section";
    paletteSection.style.cssText = "flex:1;overflow-y:auto;padding:8px;";

    // ---- 詳細パネル(パレットと同じ位置に重ねる相対コンテナ) ----
    const viewBody = document.createElement("div");
    viewBody.className = "mpp-view-body";
    viewBody.style.cssText = "position:relative;flex:1;overflow:hidden;";

    paletteSection.style.cssText = "position:absolute;inset:0;overflow-y:auto;padding:8px;";

    const detailContainer = document.createElement("div");
    detailContainer.className = "mpp-detail-container";
    detailContainer.style.cssText =
      "position:absolute;inset:0;overflow:hidden;display:none;" +
      "background:var(--color-bg,#1a1a2e);";

    viewBody.appendChild(paletteSection);
    viewBody.appendChild(detailContainer);
    container.appendChild(viewBody);

    // ---- パレット構築 ----
    function buildPalette(pList, baseUrl, ts, stw, sth) {
      palette = createPalette({
        parts: pList,
        sheetBaseUrl: baseUrl,
        tileSize: ts,
        sheetTileWidth: stw,
        sheetTileHeight: sth,
        onSelect(id) {
          editBtn.disabled = false;
          editBtn.title = "";
          notifyFrame(id);
        },
      });
      return palette.el;
    }

    function rebuildPalette(pList, baseUrl, ts, stw, sth, restoreId) {
      paletteSection.innerHTML = "";
      const el = buildPalette(pList, baseUrl, ts, stw, sth);
      paletteSection.appendChild(el);
      if (restoreId) {
        palette.setSelectedId(restoreId);
        editBtn.disabled = false;
      }
    }

    paletteSection.appendChild(buildPalette(parts, sheetBaseUrl, tileSize, sheetTileWidth, sheetTileHeight));

    // ---- 詳細表示/非表示 ----
    function showDetail() {
      paletteSection.style.display = "none";
      detailContainer.style.display = "";
    }
    function hideDetail() {
      detailContainer.style.display = "none";
      detailContainer.innerHTML = "";
      paletteSection.style.display = "";
    }

    // ---- 編集ボタン ----
    editBtn.addEventListener("click", async () => {
      const partsId = palette ? palette.getSelectedId() : null;
      if (!partsId) return;
      showDetail();
      try {
        const result = await openPartsDetail(partsId, { mountIn: detailContainer });
        hideDetail();
        if (result.saved) {
          // サムネ再描画のためパレットを再構築
          const { parts: newParts, sheetBaseUrl: newBase, tileSize: newTs, sheetTileWidth: newStw, sheetTileHeight: newSth }
            = await fetchParts();
          rebuildPalette(newParts, newBase, newTs, newStw, newSth, partsId);
        }
      } catch (err) {
        hideDetail();
        // openPartsDetail が reject した場合のみここに来る
        const errMsg = document.createElement("p");
        errMsg.className = "result-message error";
        errMsg.textContent = `編集画面を開けませんでした: ${err.message || err}`;
        toolbar.appendChild(errMsg);
        setTimeout(() => errMsg.remove(), 4000);
      }
    });

    // ---- 管理モード通知(初期) ----
    if (adminGameFrame && adminGameFrame.contentWindow) {
      adminGameFrame.contentWindow.postMessage(
        { kind: "sbop2_set_admin_mode", mode: 2 },
        "*"
      );
    }

    // ---- iframe からスポイト pick を受け取る ----
    function onPickupParts(ev) {
      const partsId = ev.detail?.partsId;
      if (typeof partsId === "number" && partsId > 0 && palette) {
        palette.setSelectedId(partsId);
        editBtn.disabled = false;
      }
    }
    window.addEventListener("sbop2_pickup_parts", onPickupParts);

    _destroyFn = () => {
      palette = null;
      window.removeEventListener("sbop2_pickup_parts", onPickupParts);
      container.innerHTML = "";
      container.style.cssText = "";
    };
  }).catch((err) => {
    if (token !== _mountToken) return;
    container.innerHTML = "";
    const errEl = document.createElement("p");
    errEl.className = "result-message error";
    errEl.textContent = `データの取得に失敗しました: ${err.message || err}`;
    container.appendChild(errEl);
  });

  _destroyFn = () => { container.innerHTML = ""; container.style.cssText = ""; };
}

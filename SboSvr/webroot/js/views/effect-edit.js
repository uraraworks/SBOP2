/**
 * views/effect-edit.js
 * エフェクト一覧・編集画面 (route: effect-library)
 *
 * API: /api/effects
 *   GET  → { items: [{ effectId, name, grpIdMain, soundId, loop, loopSound,
 *                       animes: [{wait, level, grpIdBase, grpIdPile}] }] }
 *   POST → 新規 (name, grpIdMain, soundId, loop, loopSound, animes)
 *   PUT  → 更新 (effectId 必須 + 上記任意)
 *   DELETE → { effectId }
 *
 * レイアウト: list-detail 骨格
 *   左: エフェクト一覧 (ID+名前+先頭フレームサムネ、検索)
 *   右: 名前 / 画像種別(effect32|64) / 効果音 / loop / loopSound
 *       + フレームテーブル (行=コマ: サムネ+grpIdBase+grpIdPile+wait+alpha)
 *       + anime-preview リアルタイム再生
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createNumberSpinner } from "../components/number-spinner.js";
import { createDragList } from "../components/drag-list.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { createSoundPicker } from "../components/sound-picker.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const CATEGORY_32 = "effect32";
const CATEGORY_64 = "effect64";

// ----------------------------------------------------------------
// フィードバック
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "ee-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function makeFormField(labelText) {
  const lbl = document.createElement("label");
  lbl.className = "form-field";
  const span = document.createElement("span");
  span.textContent = labelText;
  lbl.appendChild(span);
  return lbl;
}

// ----------------------------------------------------------------
// フレームテーブル (dragList 版)
// ----------------------------------------------------------------

/**
 * フレームテーブルを buildDragList で構築する。
 * @returns {{ el, getFrames, setFrames }}
 */
function buildFrameTable({ getCategoryKey, onFrameChange }) {
  let _frames = []; // [{ grpIdBase, grpIdPile, wait, level }]
  let _dragList = null;
  const wrap = document.createElement("div");
  wrap.className = "ee-frame-table-wrap";

  function renderFrame(frame) {
    const row = document.createElement("div");
    row.className = "ee-frame-row";

    // サムネ
    const thumbWrap = document.createElement("div");
    thumbWrap.className = "ee-frame-thumb";
    const thumb = createSpriteThumb({ categoryKey: getCategoryKey(), sub: frame.grpIdBase, size: 32 });
    thumbWrap.appendChild(thumb.el);
    row.appendChild(thumbWrap);

    // grpIdBase ピッカー
    const sfBase = createSpriteField({
      categoryKey: getCategoryKey(),
      value: frame.grpIdBase,
      label: "下地",
      allowCategorySwitch: false,
      onChange: (v) => {
        frame.grpIdBase = v;
        thumb.update({ categoryKey: getCategoryKey(), sub: v });
        onFrameChange();
      },
    });
    row.appendChild(sfBase.el);

    // grpIdPile ピッカー
    const sfPile = createSpriteField({
      categoryKey: getCategoryKey(),
      value: frame.grpIdPile,
      label: "重ね",
      allowCategorySwitch: false,
      onChange: (v) => {
        frame.grpIdPile = v;
        onFrameChange();
      },
    });
    row.appendChild(sfPile.el);

    // wait
    const waitLbl = makeFormField("表示 wait (×10ms)");
    const waitSpin = createNumberSpinner({ value: frame.wait, min: 0, max: 255, step: 1,
      onChange: (v) => { frame.wait = v; onFrameChange(); } });
    waitLbl.appendChild(waitSpin.el);
    row.appendChild(waitLbl);

    // alpha (level)
    const alphaLbl = makeFormField("透明度 (0-255)");
    const alphaSpin = createNumberSpinner({ value: frame.level, min: 0, max: 255, step: 1,
      onChange: (v) => { frame.level = v; onFrameChange(); } });
    alphaLbl.appendChild(alphaSpin.el);
    row.appendChild(alphaLbl);

    return row;
  }

  function rebuild() {
    if (_dragList) { _dragList.el.remove(); _dragList = null; }
    _dragList = createDragList({
      items: _frames,
      renderItem: renderFrame,
      onReorder: (newItems) => { _frames = newItems; onFrameChange(); },
    });
    wrap.appendChild(_dragList.el);
  }

  // 追加ボタン
  const addBtn = document.createElement("button");
  addBtn.type = "button";
  addBtn.className = "button small";
  addBtn.textContent = "+ コマ追加";
  addBtn.addEventListener("click", () => {
    if (_frames.length >= 255) { return; }
    _frames.push({ grpIdBase: 0, grpIdPile: 0, wait: 10, level: 255 });
    rebuild();
    onFrameChange();
  });
  wrap.appendChild(addBtn);

  rebuild();

  return {
    el: wrap,
    getFrames: () => _frames.map((f) => ({ ...f })),
    setFrames: (frames) => {
      _frames = (frames || []).map((f) => ({
        grpIdBase: f.grpIdBase || 0,
        grpIdPile: f.grpIdPile || 0,
        wait:      f.wait      || 0,
        level:     f.level     != null ? f.level : 255,
      }));
      rebuild();
    },
    refreshCategory: () => { rebuild(); },
  };
}

// ----------------------------------------------------------------
// 右ペイン: 詳細フォーム
// ----------------------------------------------------------------

function buildDetailPane({ feedbackEl }) {
  const pane = document.createElement("div");
  pane.className = "ee-right";

  // --- プレビュー ---
  const previewSec = document.createElement("section");
  previewSec.className = "detail-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "アニメプレビュー";
  previewSec.appendChild(previewH3);
  const preview = createAnimePreview({ width: 64, height: 64, scale: 2 });
  previewSec.appendChild(preview.el);
  pane.appendChild(previewSec);

  // --- 保存バー ---
  pane.appendChild(feedbackEl);
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

  // --- 基本情報 ---
  const basicSec = document.createElement("section");
  basicSec.className = "detail-section";
  const basicH3 = document.createElement("h3");
  basicH3.textContent = "基本情報";
  basicSec.appendChild(basicH3);

  const basicGrid = document.createElement("div");
  basicGrid.className = "form-grid compact";

  // 名前
  const nameLbl = makeFormField("エフェクト名");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 32;
  nameInput.placeholder = "エフェクト名";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  // 画像種別 select
  const catLbl = makeFormField("画像種別");
  const catSelect = document.createElement("select");
  [["effect32", "effect32 (32×32)"], ["effect64", "effect64 (64×64)"]].forEach(([v, t]) => {
    const opt = document.createElement("option");
    opt.value = v;
    opt.textContent = t;
    catSelect.appendChild(opt);
  });
  catLbl.appendChild(catSelect);
  basicGrid.appendChild(catLbl);

  // ループ
  const loopLbl = document.createElement("label");
  loopLbl.className = "form-field";
  const loopCb = document.createElement("input");
  loopCb.type = "checkbox";
  loopLbl.append(loopCb, " ループ再生");
  basicGrid.appendChild(loopLbl);

  // loopSound
  const loopSoundLbl = document.createElement("label");
  loopSoundLbl.className = "form-field";
  const loopSoundCb = document.createElement("input");
  loopSoundCb.type = "checkbox";
  loopSoundLbl.append(loopSoundCb, " ループ時に効果音再生");
  basicGrid.appendChild(loopSoundLbl);

  // 効果音
  const soundLbl = makeFormField("効果音");
  const soundPicker = createSoundPicker({ value: 0 });
  soundLbl.appendChild(soundPicker.el);
  basicGrid.appendChild(soundLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // --- フレームテーブル ---
  const frameSec = document.createElement("section");
  frameSec.className = "detail-section";
  const frameH3 = document.createElement("h3");
  frameH3.textContent = "アニメーションコマ (最大 255)";
  frameSec.appendChild(frameH3);

  let _currentCategory = CATEGORY_32;
  const getCategoryKey = () => _currentCategory;

  let _previewTimer = null;
  function schedulePreview() {
    if (_previewTimer) { clearTimeout(_previewTimer); }
    _previewTimer = setTimeout(doPreview, 80);
  }

  const frameTable = buildFrameTable({
    getCategoryKey,
    onFrameChange: schedulePreview,
  });
  frameSec.appendChild(frameTable.el);
  pane.appendChild(frameSec);

  // カテゴリ切替でフレームテーブルを更新
  catSelect.addEventListener("change", () => {
    _currentCategory = catSelect.value;
    frameTable.refreshCategory();
    schedulePreview();
  });

  // --- プレビュー更新 ---
  function doPreview() {
    const frames = frameTable.getFrames();
    if (!frames.length) {
      preview.setFrames([]);
      return;
    }
    const previewFrames = frames.map((f) => ({
      wait: (f.wait || 1) * 10,
      layers: [
        { categoryKey: getCategoryKey(), sub: f.grpIdBase, offsetX: 0, offsetY: 0, alpha: f.level / 255 },
        { categoryKey: getCategoryKey(), sub: f.grpIdPile,  offsetX: 0, offsetY: 0, alpha: 1 },
      ],
    }));
    preview.setFrames(previewFrames);
  }

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  let _currentEffect = null; // null=新規

  function setEffect(eff) {
    _currentEffect = eff || null;
    nameInput.value = eff ? (eff.name || "") : "";
    // 画像種別: grpIdMain が 0 以外なら effect64 か effect32 は判定できないので
    // grpIdMain そのまま保持し、表示はデフォルト effect32
    catSelect.value = CATEGORY_32;
    _currentCategory = CATEGORY_32;
    loopCb.checked = eff ? !!eff.loop : false;
    loopSoundCb.checked = eff ? !!eff.loopSound : false;
    soundPicker.setValue(eff ? (eff.soundId || 0) : 0);
    frameTable.setFrames(eff ? (eff.animes || []) : []);
    schedulePreview();
  }

  function collectData() {
    return {
      name:      nameInput.value,
      grpIdMain: catSelect.value === CATEGORY_64 ? 1 : 0, // 種別フラグとして保持
      soundId:   soundPicker.getValue(),
      loop:      loopCb.checked,
      loopSound: loopSoundCb.checked,
      animes:    frameTable.getFrames(),
    };
  }

  function getCurrentEffect() { return _currentEffect; }
  function setCurrentEffect(eff) { _currentEffect = eff; }

  return {
    el: pane,
    saveBtn,
    cancelBtn,
    setEffect,
    collectData,
    getCurrentEffect,
    setCurrentEffect,
    destroy: () => { preview.destroy(); },
  };
}

// ----------------------------------------------------------------
// 左ペイン: 一覧
// ----------------------------------------------------------------

function buildLeftPane({ onSelect, onNew, onDelete }) {
  const pane = document.createElement("div");
  pane.className = "ee-left";

  // 検索
  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "ID または 名前で検索…";
  searchInput.className = "ld-search";
  pane.appendChild(searchInput);

  // ツールバー
  const toolbar = document.createElement("div");
  toolbar.className = "me-list-toolbar";
  const newBtn = document.createElement("button");
  newBtn.type = "button";
  newBtn.className = "button small";
  newBtn.textContent = "+ 新規追加";
  newBtn.addEventListener("click", onNew);
  toolbar.appendChild(newBtn);
  pane.appendChild(toolbar);

  // リスト
  const listEl = document.createElement("ul");
  listEl.className = "ld-list";
  pane.appendChild(listEl);

  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let _allItems = [];
  let _selectedId = null;

  async function loadList() {
    const { response, data } = await fetchJson("/api/effects");
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("effects load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((e) => {
      if (!q) return true;
      return String(e.effectId).includes(q) || (e.name || "").toLowerCase().includes(q);
    });
    listEl.innerHTML = "";
    filtered.forEach((e) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (e.effectId === _selectedId ? " selected" : "");

      // サムネ (先頭フレーム grpIdBase)
      const firstFrame = Array.isArray(e.animes) && e.animes.length ? e.animes[0] : null;
      if (firstFrame) {
        const thumb = createSpriteThumb({ categoryKey: CATEGORY_32, sub: firstFrame.grpIdBase, size: 24 });
        thumb.el.className = "ld-item-thumb";
        li.appendChild(thumb.el);
      }

      const label = document.createElement("span");
      label.textContent = "[" + e.effectId + "] " + (e.name || "(名前なし)");
      li.appendChild(label);

      li.addEventListener("click", () => {
        _selectedId = e.effectId;
        renderList();
        onSelect(e);
      });

      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); onDelete(e); });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });
    summary.textContent = filtered.length + " 件";
  }

  searchInput.addEventListener("input", renderList);
  loadList();

  return { el: pane, reload: loadList };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const shell = document.createElement("div");
  shell.className = "me-shell";
  container.appendChild(shell);

  const feedbackEl = document.createElement("p");
  feedbackEl.className = "ee-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  const detail = buildDetailPane({ feedbackEl });

  // 詳細ペインの先頭に「← 戻る」ボタンを追加
  const backBar = document.createElement("div");
  backBar.className = "me-action-bar";
  const backBtn = document.createElement("button");
  backBtn.type = "button";
  backBtn.className = "button small";
  backBtn.textContent = "← 戻る";
  backBar.appendChild(backBtn);
  detail.el.insertBefore(backBar, detail.el.firstChild);

  // 画面切替ヘルパー
  function showDetail() {
    leftApi.el.style.display = "none";
    detail.el.style.display = "";
  }
  function showList() {
    detail.el.style.display = "none";
    leftApi.el.style.display = "";
  }

  backBtn.addEventListener("click", showList);

  // 保存
  detail.saveBtn.addEventListener("click", async () => {
    const payload = detail.collectData();
    const current = detail.getCurrentEffect();
    const isNew = !current?.effectId;
    if (!isNew) { payload.effectId = current.effectId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      const { response, data } = await fetchJson("/api/effects", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.effectId) {
        detail.setCurrentEffect({ ...payload, effectId: data.effectId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規 → フォームクリア + 一覧に戻る
  detail.cancelBtn.addEventListener("click", () => {
    detail.setEffect(null);
    showFeedback(feedbackEl, "", "");
    showList();
  });

  const leftApi = buildLeftPane({
    onSelect: (eff) => {
      detail.setEffect(eff);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onNew: () => {
      detail.setEffect(null);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onDelete: async (eff) => {
      if (!confirm("エフェクト [" + (eff.name || "") + "] (ID=" + eff.effectId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/effects", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ effectId: eff.effectId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + eff.effectId + ")", "success");
        if (detail.getCurrentEffect()?.effectId === eff.effectId) {
          detail.setEffect(null);
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
    detail.destroy();
    container.innerHTML = "";
  };
}

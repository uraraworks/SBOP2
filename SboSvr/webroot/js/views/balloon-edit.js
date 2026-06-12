/**
 * views/balloon-edit.js
 * 噴出し一覧・編集画面 (route: effect-balloon)
 *
 * API: /api/efc-balloons
 *   GET  → { items: [{ efcBalloonId, listId, animeId, name, grpId, wait, loop, soundId }] }
 *   POST → 新規 (listId必須, animeId, name, grpId, wait, loop, soundId)
 *   PUT  → 更新 (efcBalloonId 必須 + 任意)
 *   DELETE → { efcBalloonId }
 *
 * データ構造: flat (コマ単位でレコードが存在)
 *   listId  = 噴出し種別 ID
 *   animeId = コマ番号
 *
 * レイアウト: list-detail 骨格
 *   左: 噴出しコマ一覧 (ID+種別+コマ番号+名前+サムネ、検索)
 *   右: 種別ID / コマ番号 / 名前 / 画像(efcBalloon ピッカー) / 効果音 / wait / loop
 *       + anime-preview
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createNumberSpinner } from "../components/number-spinner.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { createSoundPicker } from "../components/sound-picker.js";

const BALLOON_CATEGORY = "efcBalloon";

// ----------------------------------------------------------------
// フィードバック
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "be-feedback result-message" + (type ? " " + type : "");
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
// 右ペイン: 詳細フォーム
// ----------------------------------------------------------------

function buildDetailPane({ feedbackEl }) {
  const pane = document.createElement("div");
  pane.className = "ee-right";

  // --- プレビュー ---
  const previewSec = document.createElement("section");
  previewSec.className = "detail-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "プレビュー";
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

  // 種別ID (listId)
  const listIdLbl = makeFormField("種別ID (listId)");
  const listIdSpin = createNumberSpinner({ value: 1, min: 1, max: 9999, step: 1 });
  listIdLbl.appendChild(listIdSpin.el);
  basicGrid.appendChild(listIdLbl);

  // コマ番号 (animeId)
  const animeIdLbl = makeFormField("コマ番号 (animeId)");
  const animeIdSpin = createNumberSpinner({ value: 0, min: 0, max: 255, step: 1 });
  animeIdLbl.appendChild(animeIdSpin.el);
  basicGrid.appendChild(animeIdLbl);

  // 名前
  const nameLbl = makeFormField("噴出し名");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 32;
  nameInput.placeholder = "噴出し名";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // --- 表示設定 ---
  const dispSec = document.createElement("section");
  dispSec.className = "detail-section";
  const dispH3 = document.createElement("h3");
  dispH3.textContent = "表示設定";
  dispSec.appendChild(dispH3);

  const dispGrid = document.createElement("div");
  dispGrid.className = "form-grid compact";

  // 画像 (grpId)
  let _previewTimer = null;
  function schedulePreview() {
    if (_previewTimer) { clearTimeout(_previewTimer); }
    _previewTimer = setTimeout(doPreview, 80);
  }

  const sfGrp = createSpriteField({
    categoryKey: BALLOON_CATEGORY,
    value: 0,
    label: "画像 (grpId)",
    allowCategorySwitch: false,
    onChange: schedulePreview,
  });
  dispGrid.appendChild(sfGrp.el);

  // wait
  const waitLbl = makeFormField("表示時間 wait (×10ms)");
  const waitSpin = createNumberSpinner({ value: 0, min: 0, max: 255, step: 1, onChange: schedulePreview });
  waitLbl.appendChild(waitSpin.el);
  dispGrid.appendChild(waitLbl);

  // loop
  const loopLbl = document.createElement("label");
  loopLbl.className = "form-field";
  const loopCb = document.createElement("input");
  loopCb.type = "checkbox";
  loopCb.addEventListener("change", schedulePreview);
  loopLbl.append(loopCb, " ループ再生");
  dispGrid.appendChild(loopLbl);

  // 効果音
  const soundLbl = makeFormField("効果音");
  const soundPicker = createSoundPicker({ value: 0 });
  soundLbl.appendChild(soundPicker.el);
  dispGrid.appendChild(soundLbl);

  dispSec.appendChild(dispGrid);
  pane.appendChild(dispSec);

  // --- プレビュー更新 ---
  function doPreview() {
    const grpId = sfGrp.getValue();
    const wait  = (waitSpin.getValue() || 1) * 10;
    preview.setFrames([{
      wait,
      layers: [{ categoryKey: BALLOON_CATEGORY, sub: grpId, offsetX: 0, offsetY: 0, alpha: 1 }],
    }]);
  }

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  let _currentBalloon = null;

  function setBalloon(b) {
    _currentBalloon = b || null;
    listIdSpin.setValue(b ? (b.listId || 1) : 1);
    animeIdSpin.setValue(b ? (b.animeId || 0) : 0);
    nameInput.value = b ? (b.name || "") : "";
    sfGrp.setValue(b ? (b.grpId || 0) : 0);
    waitSpin.setValue(b ? (b.wait || 0) : 0);
    loopCb.checked = b ? !!b.loop : false;
    soundPicker.setValue(b ? (b.soundId || 0) : 0);
    schedulePreview();
  }

  function collectData() {
    return {
      listId:  listIdSpin.getValue(),
      animeId: animeIdSpin.getValue(),
      name:    nameInput.value,
      grpId:   sfGrp.getValue(),
      wait:    waitSpin.getValue(),
      loop:    loopCb.checked,
      soundId: soundPicker.getValue(),
    };
  }

  function getCurrentBalloon() { return _currentBalloon; }
  function setCurrentBalloon(b) { _currentBalloon = b; }

  return {
    el: pane,
    saveBtn,
    cancelBtn,
    setBalloon,
    collectData,
    getCurrentBalloon,
    setCurrentBalloon,
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
  searchInput.placeholder = "ID / 種別ID / 名前で検索…";
  searchInput.className = "ld-search";
  pane.appendChild(searchInput);

  // ツールバー
  const toolbar = document.createElement("div");
  toolbar.className = "me-list-toolbar";
  const newBtn = document.createElement("button");
  newBtn.type = "button";
  newBtn.className = "button small";
  newBtn.textContent = "+ コマ追加";
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
    const { response, data } = await fetchJson("/api/efc-balloons");
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("efc-balloons load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((b) => {
      if (!q) return true;
      return String(b.efcBalloonId).includes(q)
        || String(b.listId).includes(q)
        || (b.name || "").toLowerCase().includes(q);
    });
    listEl.innerHTML = "";
    filtered.forEach((b) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (b.efcBalloonId === _selectedId ? " selected" : "");

      // サムネ
      const thumb = createSpriteThumb({ categoryKey: BALLOON_CATEGORY, sub: b.grpId || 0, size: 24 });
      thumb.el.className = "ld-item-thumb";
      li.appendChild(thumb.el);

      const label = document.createElement("span");
      label.textContent = "[" + b.efcBalloonId + "] list=" + b.listId + " #" + b.animeId
        + (b.name ? " " + b.name : "");
      li.appendChild(label);

      li.addEventListener("click", () => {
        _selectedId = b.efcBalloonId;
        renderList();
        onSelect(b);
      });

      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); onDelete(b); });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });
    summary.textContent = filtered.length + " コマ";
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
  feedbackEl.className = "be-feedback result-message";
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
    const current = detail.getCurrentBalloon();
    const isNew = !current?.efcBalloonId;
    if (!isNew) { payload.efcBalloonId = current.efcBalloonId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      const { response, data } = await fetchJson("/api/efc-balloons", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.efcBalloonId) {
        detail.setCurrentBalloon({ ...payload, efcBalloonId: data.efcBalloonId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規 → フォームクリア + 一覧に戻る
  detail.cancelBtn.addEventListener("click", () => {
    detail.setBalloon(null);
    showFeedback(feedbackEl, "", "");
    showList();
  });

  const leftApi = buildLeftPane({
    onSelect: (b) => {
      detail.setBalloon(b);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onNew: () => {
      detail.setBalloon(null);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onDelete: async (b) => {
      const label = (b.name || "") + " (listId=" + b.listId + ", animeId=" + b.animeId + ")";
      if (!confirm("噴出しコマ [" + label + "] (ID=" + b.efcBalloonId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/efc-balloons", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ efcBalloonId: b.efcBalloonId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + b.efcBalloonId + ")", "success");
        if (detail.getCurrentBalloon()?.efcBalloonId === b.efcBalloonId) {
          detail.setBalloon(null);
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

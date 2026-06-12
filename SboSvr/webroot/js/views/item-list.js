/**
 * views/item-list.js
 * アイテム一覧・編集画面 (route: item-list)
 *
 * API: /api/items
 *   GET    → { items: [{ itemId, itemTypeId, itemTypeName, name, grpId, iconGrpId,
 *                         dropSoundId, putOn, mapId, posX, posY, posZ,
 *                         charId, backPackX, backPackY }] }
 *   POST   → 新規
 *   PUT    → 更新 (itemId 必須)
 *   DELETE → { itemId }
 *
 * クエリ: ?drop=1 &charId=N &mapId=N
 *
 * レイアウト: list-detail 骨格
 *   左: アイテム一覧 (ID+名前+種別+アイコンサムネ、検索)
 *   右: 基本情報 / 画像(createSpriteField) / 効果音(sound-picker) / 配置・所有
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createSoundPicker } from "../components/sound-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "il-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

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

  // 保存バー
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

  // アイテム種別ID
  const typeIdLbl = makeFormField("アイテム種別ID");
  const typeIdWrap = document.createElement("div");
  typeIdWrap.style.display = "flex";
  typeIdWrap.style.alignItems = "center";
  typeIdWrap.style.gap = "0.5rem";
  const typeIdSpin = createNumberSpinner({ value: 1, min: 0, max: 9999, step: 1 });
  const typeNameSpan = document.createElement("span");
  typeNameSpan.className = "muted";
  typeIdWrap.append(typeIdSpin.el, typeNameSpan);
  typeIdLbl.appendChild(typeIdWrap);
  basicGrid.appendChild(typeIdLbl);

  // 名前
  const nameLbl = makeFormField("アイテム名（省略時は種別名）");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 32;
  nameInput.placeholder = "表示名";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  // 上に置けるか
  const putOnLbl = document.createElement("label");
  putOnLbl.className = "form-field";
  const putOnCb = document.createElement("input");
  putOnCb.type = "checkbox";
  putOnLbl.append(putOnCb, " 上に置けるか");
  basicGrid.appendChild(putOnLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // --- 画像・効果音 ---
  const imgSec = document.createElement("section");
  imgSec.className = "detail-section";
  const imgH3 = document.createElement("h3");
  imgH3.textContent = "画像・効果音";
  imgSec.appendChild(imgH3);

  const imgGrid = document.createElement("div");
  imgGrid.className = "form-grid compact";

  // grpId: 地面画像
  const sfGrpId = createSpriteField({
    categoryKey: "item",
    value: 0,
    label: "地面画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfGrpId.el);

  // iconGrpId: バックパック内画像
  const sfIconGrpId = createSpriteField({
    categoryKey: "icon32",
    value: 0,
    label: "バックパック内画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfIconGrpId.el);

  // dropSoundId
  const dropSoundLbl = makeFormField("落下音");
  const dropSoundPicker = createSoundPicker({ value: 0 });
  dropSoundLbl.appendChild(dropSoundPicker.el);
  imgGrid.appendChild(dropSoundLbl);

  imgSec.appendChild(imgGrid);
  pane.appendChild(imgSec);

  // --- 配置・所有 ---
  const placeSec = document.createElement("section");
  placeSec.className = "detail-section";
  const placeH3 = document.createElement("h3");
  placeH3.textContent = "配置・所有";
  placeSec.appendChild(placeH3);

  const placeGrid = document.createElement("div");
  placeGrid.className = "form-grid compact";

  function addSpinField(labelText, min, max) {
    const lbl = makeFormField(labelText);
    const spin = createNumberSpinner({ value: 0, min, max, step: 1 });
    lbl.appendChild(spin.el);
    placeGrid.appendChild(lbl);
    return spin;
  }

  const mapIdSpin    = addSpinField("配置マップID（0=未配置）", 0, 9999);
  const posXSpin     = addSpinField("X座標", -9999, 9999);
  const posYSpin     = addSpinField("Y座標", -9999, 9999);
  const posZSpin     = addSpinField("Z座標（高さ）", -9999, 9999);
  const charIdSpin   = addSpinField("所持キャラID（0=未所持）", 0, 9999);
  const backPackXSpin = addSpinField("バックパックX", 0, 9999);
  const backPackYSpin = addSpinField("バックパックY", 0, 9999);

  placeSec.appendChild(placeGrid);
  pane.appendChild(placeSec);

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  let _current = null;

  function setItem(it) {
    _current = it || null;
    typeIdSpin.setValue(it ? (it.itemTypeId || 1) : 1);
    typeNameSpan.textContent = it?.itemTypeName ? ("（" + it.itemTypeName + "）") : "";
    nameInput.value = it ? (it.name || "") : "";
    putOnCb.checked = it ? !!it.putOn : false;
    sfGrpId.setValue(it ? (it.grpId || 0) : 0);
    sfIconGrpId.setValue(it ? (it.iconGrpId || 0) : 0);
    dropSoundPicker.setValue(it ? (it.dropSoundId || 0) : 0);
    mapIdSpin.setValue(it ? (it.mapId || 0) : 0);
    posXSpin.setValue(it ? (it.posX || 0) : 0);
    posYSpin.setValue(it ? (it.posY || 0) : 0);
    posZSpin.setValue(it ? (it.posZ || 0) : 0);
    charIdSpin.setValue(it ? (it.charId || 0) : 0);
    backPackXSpin.setValue(it ? (it.backPackX || 0) : 0);
    backPackYSpin.setValue(it ? (it.backPackY || 0) : 0);
  }

  function collectData() {
    return {
      itemTypeId:  typeIdSpin.getValue(),
      name:        nameInput.value,
      putOn:       putOnCb.checked,
      grpId:       sfGrpId.getValue(),
      iconGrpId:   sfIconGrpId.getValue(),
      dropSoundId: dropSoundPicker.getValue(),
      mapId:       mapIdSpin.getValue(),
      posX:        posXSpin.getValue(),
      posY:        posYSpin.getValue(),
      posZ:        posZSpin.getValue(),
      charId:      charIdSpin.getValue(),
      backPackX:   backPackXSpin.getValue(),
      backPackY:   backPackYSpin.getValue(),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(it) { _current = it; }

  return { el: pane, saveBtn, cancelBtn, setItem, collectData, getCurrent, setCurrent };
}

// ----------------------------------------------------------------
// 左ペイン: 一覧 + フィルター
// ----------------------------------------------------------------

function buildLeftPane({ onSelect, onNew, onDelete }) {
  const pane = document.createElement("div");
  pane.className = "ee-left";

  // フィルター
  const filterWrap = document.createElement("div");
  filterWrap.className = "it-filter-wrap";
  filterWrap.style.cssText = "display:flex;flex-direction:column;gap:0.25rem;margin-bottom:0.5rem;";

  const dropLbl = document.createElement("label");
  const dropCb = document.createElement("input");
  dropCb.type = "checkbox";
  dropCb.checked = true;
  dropLbl.append(dropCb, " 落ちているもののみ");
  filterWrap.appendChild(dropLbl);

  const charIdFilterLbl = makeFormField("所持キャラID");
  const charIdFilterInput = document.createElement("input");
  charIdFilterInput.type = "number";
  charIdFilterInput.min = "0";
  charIdFilterInput.style.width = "6rem";
  charIdFilterLbl.appendChild(charIdFilterInput);
  filterWrap.appendChild(charIdFilterLbl);

  const mapIdFilterLbl = makeFormField("マップID");
  const mapIdFilterInput = document.createElement("input");
  mapIdFilterInput.type = "number";
  mapIdFilterInput.min = "0";
  mapIdFilterInput.style.width = "6rem";
  mapIdFilterLbl.appendChild(mapIdFilterInput);
  filterWrap.appendChild(mapIdFilterLbl);

  const filterBtnWrap = document.createElement("div");
  filterBtnWrap.style.display = "flex";
  filterBtnWrap.style.gap = "0.5rem";
  const applyBtn = document.createElement("button");
  applyBtn.type = "button";
  applyBtn.className = "button small";
  applyBtn.textContent = "フィルター適用";
  const clearBtn = document.createElement("button");
  clearBtn.type = "button";
  clearBtn.className = "button small";
  clearBtn.textContent = "クリア";
  filterBtnWrap.append(applyBtn, clearBtn);
  filterWrap.appendChild(filterBtnWrap);

  pane.appendChild(filterWrap);

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

  const listEl = document.createElement("ul");
  listEl.className = "ld-list";
  pane.appendChild(listEl);

  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let _allItems = [];
  let _selectedId = null;

  function buildQuery() {
    const params = [];
    if (dropCb.checked) { params.push("drop=1"); }
    const cid = charIdFilterInput.value.trim();
    if (cid !== "" && !isNaN(Number(cid))) { params.push("charId=" + cid); }
    const mid = mapIdFilterInput.value.trim();
    if (mid !== "" && !isNaN(Number(mid))) { params.push("mapId=" + mid); }
    return params.length ? ("?" + params.join("&")) : "";
  }

  async function loadList() {
    const { response, data } = await fetchJson("/api/items" + buildQuery());
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("items load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((it) => {
      if (!q) return true;
      return String(it.itemId).includes(q) || (it.name || "").toLowerCase().includes(q);
    });
    listEl.innerHTML = "";
    filtered.forEach((it) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (it.itemId === _selectedId ? " selected" : "");

      // アイコンサムネ
      if (it.grpId) {
        const thumb = createSpriteThumb({ categoryKey: "item", sub: it.grpId, size: 24 });
        thumb.el.className = "ld-item-thumb";
        li.appendChild(thumb.el);
      }

      const labelEl = document.createElement("span");
      const typePart = it.itemTypeName ? ("[" + it.itemTypeId + "] " + it.itemTypeName) : ("[" + it.itemTypeId + "]");
      labelEl.textContent = "[" + it.itemId + "] " + (it.name || "(名前なし)") + " / " + typePart;
      li.appendChild(labelEl);

      li.addEventListener("click", () => {
        _selectedId = it.itemId;
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

  applyBtn.addEventListener("click", () => loadList());
  clearBtn.addEventListener("click", () => {
    dropCb.checked = true;
    charIdFilterInput.value = "";
    mapIdFilterInput.value = "";
    loadList();
  });
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
  feedbackEl.className = "il-feedback result-message";
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
    const current = detail.getCurrent();
    const isNew = !current?.itemId;
    if (!isNew) { payload.itemId = current.itemId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      const { response, data } = await fetchJson("/api/items", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.itemId) {
        detail.setCurrent({ ...payload, itemId: data.itemId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規 → フォームクリア + 一覧に戻る
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
      if (!confirm("アイテム [" + (it.name || "") + "] (ID=" + it.itemId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/items", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ itemId: it.itemId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + it.itemId + ")", "success");
        if (detail.getCurrent()?.itemId === it.itemId) {
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

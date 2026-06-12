/**
 * views/item-types.js
 * アイテム種別一覧・編集画面 (route: item-types)
 *
 * API: /api/item-types
 *   GET    → { items: [{ typeId, itemTypeId, name, delAverage, putOn,
 *                         grpId, iconGrpId, grpIdMain, grpIdSub,
 *                         dropSoundId, useEffectId, useSoundId,
 *                         weaponInfoId, value, value2, moveWait, moveCount,
 *                         target, area }] }
 *   POST   → 新規
 *   PUT    → 更新 (typeId 必須)
 *   DELETE → { typeId }
 *
 * レイアウト: list-detail 骨格
 *   左: 種別一覧 (ID+名前+アイコンサムネ、検索)
 *   右: 基本情報 / 効果音3種(sound-picker) / 画像4種(createSpriteField) / 詳細パラメータ
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createSoundPicker } from "../components/sound-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const ITEM_TYPE_LABELS = {
  0: "効果無し",
  1: "服",
  2: "アクセサリ",
  3: "持ち物",
  4: "盾",
  5: "HP増減",
  6: "灯り"
};

function itemTypeLabel(id) {
  return ITEM_TYPE_LABELS[id] != null ? ITEM_TYPE_LABELS[id] : ("不明(" + id + ")");
}

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "it-feedback result-message" + (type ? " " + type : "");
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

  // 名前
  const nameLbl = makeFormField("アイテム名");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 32;
  nameInput.placeholder = "表示名";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  // アイテム種別 select
  const typeLbl = makeFormField("アイテム種別");
  const typeSelect = document.createElement("select");
  Object.entries(ITEM_TYPE_LABELS).forEach(([v, t]) => {
    const opt = document.createElement("option");
    opt.value = v;
    opt.textContent = v + ": " + t;
    typeSelect.appendChild(opt);
  });
  typeLbl.appendChild(typeSelect);
  basicGrid.appendChild(typeLbl);

  // 使用消失確率
  const delAvgLbl = makeFormField("使用消失確率 (0-255)");
  const delAvgSpin = createNumberSpinner({ value: 0, min: 0, max: 255, step: 1 });
  delAvgLbl.appendChild(delAvgSpin.el);
  basicGrid.appendChild(delAvgLbl);

  // 上に置けるか
  const putOnLbl = document.createElement("label");
  putOnLbl.className = "form-field";
  const putOnCb = document.createElement("input");
  putOnCb.type = "checkbox";
  putOnLbl.append(putOnCb, " 上に置けるか");
  basicGrid.appendChild(putOnLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // --- 画像 ---
  const imgSec = document.createElement("section");
  imgSec.className = "detail-section";
  const imgH3 = document.createElement("h3");
  imgH3.textContent = "画像";
  imgSec.appendChild(imgH3);

  const imgGrid = document.createElement("div");
  imgGrid.className = "form-grid compact";

  // grpId: 地面画像 (allowCategorySwitch: true)
  const sfGrpId = createSpriteField({
    categoryKey: "item",
    value: 0,
    label: "地面画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfGrpId.el);

  // iconGrpId: バックパック内画像 (icon32)
  const sfIconGrpId = createSpriteField({
    categoryKey: "icon32",
    value: 0,
    label: "バックパック内画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfIconGrpId.el);

  // grpIdMain: 装備メイン画像
  const sfGrpIdMain = createSpriteField({
    categoryKey: "weapon",
    value: 0,
    label: "装備メイン画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfGrpIdMain.el);

  // grpIdSub: 装備サブ画像
  const sfGrpIdSub = createSpriteField({
    categoryKey: "weapon",
    value: 0,
    label: "装備サブ画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfGrpIdSub.el);

  imgSec.appendChild(imgGrid);
  pane.appendChild(imgSec);

  // --- 効果音 ---
  const soundSec = document.createElement("section");
  soundSec.className = "detail-section";
  const soundH3 = document.createElement("h3");
  soundH3.textContent = "効果音";
  soundSec.appendChild(soundH3);

  const soundGrid = document.createElement("div");
  soundGrid.className = "form-grid compact";

  const dropSoundLbl = makeFormField("落とす時の音");
  const dropSoundPicker = createSoundPicker({ value: 0 });
  dropSoundLbl.appendChild(dropSoundPicker.el);
  soundGrid.appendChild(dropSoundLbl);

  const useSoundLbl = makeFormField("使用時の音");
  const useSoundPicker = createSoundPicker({ value: 0 });
  useSoundLbl.appendChild(useSoundPicker.el);
  soundGrid.appendChild(useSoundLbl);

  // 使用エフェクト (数値入力 + effectId 表示)
  const useEffectLbl = makeFormField("使用時エフェクトID");
  const useEffectSpin = createNumberSpinner({ value: 0, min: 0, max: 9999, step: 1 });
  useEffectLbl.appendChild(useEffectSpin.el);
  soundGrid.appendChild(useEffectLbl);

  soundSec.appendChild(soundGrid);
  pane.appendChild(soundSec);

  // --- 詳細パラメータ ---
  const detailSec = document.createElement("section");
  detailSec.className = "detail-section";
  const detailH3 = document.createElement("h3");
  detailH3.textContent = "詳細パラメータ（種別に応じて使用）";
  detailSec.appendChild(detailH3);

  const detailGrid = document.createElement("div");
  detailGrid.className = "form-grid compact";

  function addSpinField(labelText, min, max) {
    const lbl = makeFormField(labelText);
    const spin = createNumberSpinner({ value: 0, min, max, step: 1 });
    lbl.appendChild(spin.el);
    detailGrid.appendChild(lbl);
    return spin;
  }

  const weaponInfoIdSpin = addSpinField("武器情報ID（持ち物）", 0, 9999);
  const valueSpin        = addSpinField("効果値(最小)/灯りレベル等", 0, 9999);
  const value2Spin       = addSpinField("効果値(最大)/持続時間等", 0, 9999);
  const moveWaitSpin     = addSpinField("速度（持ち物）", 0, 9999);
  const moveCountSpin    = addSpinField("飛距離（持ち物）", 0, 9999);
  const targetSpin       = addSpinField("使用対象 (0-255)", 0, 255);
  const areaSpin         = addSpinField("使用範囲 (0-255)", 0, 255);

  detailSec.appendChild(detailGrid);
  pane.appendChild(detailSec);

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  let _current = null;

  function setItem(it) {
    _current = it || null;
    nameInput.value = it ? (it.name || "") : "";
    typeSelect.value = String(it ? (it.itemTypeId || 0) : 0);
    delAvgSpin.setValue(it ? (it.delAverage || 0) : 0);
    putOnCb.checked = it ? !!it.putOn : false;

    sfGrpId.setValue(it ? (it.grpId || 0) : 0);
    sfIconGrpId.setValue(it ? (it.iconGrpId || 0) : 0);
    sfGrpIdMain.setValue(it ? (it.grpIdMain || 0) : 0);
    sfGrpIdSub.setValue(it ? (it.grpIdSub || 0) : 0);

    dropSoundPicker.setValue(it ? (it.dropSoundId || 0) : 0);
    useSoundPicker.setValue(it ? (it.useSoundId || 0) : 0);
    useEffectSpin.setValue(it ? (it.useEffectId || 0) : 0);

    weaponInfoIdSpin.setValue(it ? (it.weaponInfoId || 0) : 0);
    valueSpin.setValue(it ? (it.value || 0) : 0);
    value2Spin.setValue(it ? (it.value2 || 0) : 0);
    moveWaitSpin.setValue(it ? (it.moveWait || 0) : 0);
    moveCountSpin.setValue(it ? (it.moveCount || 0) : 0);
    targetSpin.setValue(it ? (it.target || 0) : 0);
    areaSpin.setValue(it ? (it.area || 0) : 0);
  }

  function collectData() {
    return {
      itemTypeId:   Number(typeSelect.value),
      name:         nameInput.value,
      delAverage:   delAvgSpin.getValue(),
      putOn:        putOnCb.checked,
      grpId:        sfGrpId.getValue(),
      iconGrpId:    sfIconGrpId.getValue(),
      grpIdMain:    sfGrpIdMain.getValue(),
      grpIdSub:     sfGrpIdSub.getValue(),
      dropSoundId:  dropSoundPicker.getValue(),
      useSoundId:   useSoundPicker.getValue(),
      useEffectId:  useEffectSpin.getValue(),
      weaponInfoId: weaponInfoIdSpin.getValue(),
      value:        valueSpin.getValue(),
      value2:       value2Spin.getValue(),
      moveWait:     moveWaitSpin.getValue(),
      moveCount:    moveCountSpin.getValue(),
      target:       targetSpin.getValue(),
      area:         areaSpin.getValue(),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(it) { _current = it; }

  return { el: pane, saveBtn, cancelBtn, setItem, collectData, getCurrent, setCurrent };
}

// ----------------------------------------------------------------
// 左ペイン: 一覧
// ----------------------------------------------------------------

function buildLeftPane({ onSelect, onNew, onDelete }) {
  const pane = document.createElement("div");
  pane.className = "ee-left";

  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "ID または 名前で検索…";
  searchInput.className = "ld-search";
  pane.appendChild(searchInput);

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

  async function loadList() {
    const { response, data } = await fetchJson("/api/item-types");
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("item-types load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((it) => {
      if (!q) return true;
      return String(it.typeId).includes(q) || (it.name || "").toLowerCase().includes(q);
    });
    listEl.innerHTML = "";
    filtered.forEach((it) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (it.typeId === _selectedId ? " selected" : "");

      // アイコンサムネ (iconGrpId / icon32)
      if (it.iconGrpId) {
        const thumb = createSpriteThumb({ categoryKey: "icon32", sub: it.iconGrpId, size: 24 });
        thumb.el.className = "ld-item-thumb";
        li.appendChild(thumb.el);
      }

      const label = document.createElement("span");
      label.textContent = "[" + it.typeId + "] " + (it.name || "(名前なし)");
      li.appendChild(label);

      li.addEventListener("click", () => {
        _selectedId = it.typeId;
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
  feedbackEl.className = "it-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  const detail = buildDetailPane({ feedbackEl });

  // 保存
  detail.saveBtn.addEventListener("click", async () => {
    const payload = detail.collectData();
    const current = detail.getCurrent();
    const isNew = !current?.typeId;
    if (!isNew) { payload.typeId = current.typeId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      const { response, data } = await fetchJson("/api/item-types", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.typeId) {
        detail.setCurrent({ ...payload, typeId: data.typeId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規
  detail.cancelBtn.addEventListener("click", () => {
    detail.setItem(null);
    showFeedback(feedbackEl, "", "");
  });

  const leftApi = buildLeftPane({
    onSelect: (it) => {
      detail.setItem(it);
      showFeedback(feedbackEl, "", "");
    },
    onNew: () => {
      detail.setItem(null);
      showFeedback(feedbackEl, "", "");
    },
    onDelete: async (it) => {
      if (!confirm("アイテム種別 [" + (it.name || "") + "] (ID=" + it.typeId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/item-types", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ typeId: it.typeId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + it.typeId + ")", "success");
        if (detail.getCurrent()?.typeId === it.typeId) {
          detail.setItem(null);
        }
        await leftApi.reload();
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    },
  });

  shell.appendChild(leftApi.el);
  shell.appendChild(detail.el);

  _destroyFn = () => {
    container.innerHTML = "";
  };
}

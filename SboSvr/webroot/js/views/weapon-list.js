/**
 * views/weapon-list.js
 * 武器一覧・編集画面 (route: weapon-list)
 *
 * API: /api/weapons
 *   GET    → { items: [{ weaponInfoId, name, motionType, motionTypeStand,
 *                         motionTypeWalk, effectIdAtack, effectIdCritical }] }
 *   POST   → 新規
 *   PUT    → 更新 (weaponInfoId 必須)
 *   DELETE → { weaponInfoId }
 *
 * レイアウト: list-detail 骨格
 *   左: 武器一覧 (ID+名前+攻撃モーション、検索)
 *   右: 基本情報 / 攻撃モーション(チェックボックス) / モーションID / エフェクトID
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const WEAPON_MOTION_SWING   = 0x01;
const WEAPON_MOTION_POKE    = 0x02;
const WEAPON_MOTION_BOW     = 0x04;
const WEAPON_MOTION_BLOW    = 0x08;
const WEAPON_MOTION_FISHING = 0x10;

const MOTION_FLAGS = [
  { bit: WEAPON_MOTION_SWING,   label: "振る" },
  { bit: WEAPON_MOTION_POKE,    label: "突く" },
  { bit: WEAPON_MOTION_BOW,     label: "弓" },
  { bit: WEAPON_MOTION_BLOW,    label: "打撃" },
  { bit: WEAPON_MOTION_FISHING, label: "釣り" },
];

function formatMotionType(motionType) {
  const labels = MOTION_FLAGS.filter((f) => motionType & f.bit).map((f) => f.label);
  return labels.length ? labels.join(",") : "-";
}

function parseIdListFromCsv(text) {
  if (!text) { return []; }
  const out = [];
  String(text).split(",").forEach((s) => {
    const n = parseInt(s.trim(), 10);
    if (!isNaN(n) && n >= 0) { out.push(n); }
  });
  return out;
}

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "wl-feedback result-message" + (type ? " " + type : "");
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

  const nameLbl = makeFormField("武器種別名");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 32;
  nameInput.placeholder = "武器種別名";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // --- 攻撃モーション ---
  const motionSec = document.createElement("section");
  motionSec.className = "detail-section";
  const motionH3 = document.createElement("h3");
  motionH3.textContent = "使用可能な攻撃モーション（複数選択可）";
  motionSec.appendChild(motionH3);

  const motionFlagWrap = document.createElement("div");
  motionFlagWrap.style.cssText = "display:flex;gap:1rem;flex-wrap:wrap;";
  const motionCbs = MOTION_FLAGS.map((f) => {
    const lbl = document.createElement("label");
    const cb = document.createElement("input");
    cb.type = "checkbox";
    lbl.append(cb, " " + f.label);
    motionFlagWrap.appendChild(lbl);
    return { bit: f.bit, cb };
  });
  motionSec.appendChild(motionFlagWrap);

  // モーションID(スピナー)
  const motionGrid = document.createElement("div");
  motionGrid.className = "form-grid compact";
  motionGrid.style.marginTop = "0.5rem";

  const standLbl = makeFormField("立ちモーションID");
  const standSpin = createNumberSpinner({ value: 0, min: 0, max: 9999, step: 1 });
  standLbl.appendChild(standSpin.el);
  motionGrid.appendChild(standLbl);

  const walkLbl = makeFormField("すり足モーションID");
  const walkSpin = createNumberSpinner({ value: 0, min: 0, max: 9999, step: 1 });
  walkLbl.appendChild(walkSpin.el);
  motionGrid.appendChild(walkLbl);

  motionSec.appendChild(motionGrid);
  pane.appendChild(motionSec);

  // --- 武器画像 ---
  const imgSec = document.createElement("section");
  imgSec.className = "detail-section";
  const imgH3 = document.createElement("h3");
  imgH3.textContent = "武器画像";
  imgSec.appendChild(imgH3);

  const imgGrid = document.createElement("div");
  imgGrid.className = "form-grid compact";

  const sfWeapon = createSpriteField({
    categoryKey: "weapon",
    value: 0,
    label: "武器メイン画像",
    allowCategorySwitch: true,
  });
  imgGrid.appendChild(sfWeapon.el);

  imgSec.appendChild(imgGrid);
  pane.appendChild(imgSec);

  // --- エフェクトID ---
  const effectSec = document.createElement("section");
  effectSec.className = "detail-section";
  const effectH3 = document.createElement("h3");
  effectH3.textContent = "エフェクトID";
  effectSec.appendChild(effectH3);

  const effectGrid = document.createElement("div");
  effectGrid.className = "form-grid compact";

  const atkLbl = makeFormField("通常攻撃時（カンマ区切り）");
  const atkInput = document.createElement("input");
  atkInput.type = "text";
  atkInput.className = "form-input";
  atkInput.placeholder = "例: 1,2,3";
  atkLbl.appendChild(atkInput);
  effectGrid.appendChild(atkLbl);

  const criLbl = makeFormField("クリティカル時（カンマ区切り）");
  const criInput = document.createElement("input");
  criInput.type = "text";
  criInput.className = "form-input";
  criInput.placeholder = "例: 10,11";
  criLbl.appendChild(criInput);
  effectGrid.appendChild(criLbl);

  effectSec.appendChild(effectGrid);
  pane.appendChild(effectSec);

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  let _current = null;

  // grpId は武器画像として保持（weaponInfoId とは別）
  // 武器データに grpId フィールドがある場合は sfWeapon に反映
  function setWeapon(w) {
    _current = w || null;
    nameInput.value = w ? (w.name || "") : "";

    const mt = w ? (w.motionType || 0) : 0;
    motionCbs.forEach(({ bit, cb }) => { cb.checked = !!(mt & bit); });

    standSpin.setValue(w ? (w.motionTypeStand || 0) : 0);
    walkSpin.setValue(w ? (w.motionTypeWalk || 0) : 0);
    sfWeapon.setValue(w ? (w.grpId || 0) : 0);

    atkInput.value = w ? ((w.effectIdAtack || []).join(",")) : "";
    criInput.value = w ? ((w.effectIdCritical || []).join(",")) : "";
  }

  function collectData() {
    let motionType = 0;
    motionCbs.forEach(({ bit, cb }) => { if (cb.checked) { motionType |= bit; } });
    return {
      name:             nameInput.value,
      motionType:       motionType,
      motionTypeStand:  standSpin.getValue(),
      motionTypeWalk:   walkSpin.getValue(),
      grpId:            sfWeapon.getValue(),
      effectIdAtack:    parseIdListFromCsv(atkInput.value),
      effectIdCritical: parseIdListFromCsv(criInput.value),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(w) { _current = w; }

  return { el: pane, saveBtn, cancelBtn, setWeapon, collectData, getCurrent, setCurrent };
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
    const { response, data } = await fetchJson("/api/weapons");
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("weapons load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((w) => {
      if (!q) return true;
      return String(w.weaponInfoId).includes(q) || (w.name || "").toLowerCase().includes(q);
    });
    listEl.innerHTML = "";
    filtered.forEach((w) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (w.weaponInfoId === _selectedId ? " selected" : "");

      // 武器画像サムネ
      if (w.grpId) {
        const thumb = createSpriteThumb({ categoryKey: "weapon", sub: w.grpId, size: 24 });
        thumb.el.className = "ld-item-thumb";
        li.appendChild(thumb.el);
      }

      const label = document.createElement("span");
      const motionStr = formatMotionType(w.motionType || 0);
      label.textContent = "[" + w.weaponInfoId + "] " + (w.name || "(名前なし)") + " / " + motionStr;
      li.appendChild(label);

      li.addEventListener("click", () => {
        _selectedId = w.weaponInfoId;
        renderList();
        onSelect(w);
      });

      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); onDelete(w); });
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
  feedbackEl.className = "wl-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  const detail = buildDetailPane({ feedbackEl });

  // 保存
  detail.saveBtn.addEventListener("click", async () => {
    const payload = detail.collectData();
    const current = detail.getCurrent();
    const isNew = !current?.weaponInfoId;
    if (!isNew) { payload.weaponInfoId = current.weaponInfoId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      const { response, data } = await fetchJson("/api/weapons", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.weaponInfoId) {
        detail.setCurrent({ ...payload, weaponInfoId: data.weaponInfoId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規
  detail.cancelBtn.addEventListener("click", () => {
    detail.setWeapon(null);
    showFeedback(feedbackEl, "", "");
  });

  const leftApi = buildLeftPane({
    onSelect: (w) => {
      detail.setWeapon(w);
      showFeedback(feedbackEl, "", "");
    },
    onNew: () => {
      detail.setWeapon(null);
      showFeedback(feedbackEl, "", "");
    },
    onDelete: async (w) => {
      if (!confirm("武器情報 [" + (w.name || "") + "] (ID=" + w.weaponInfoId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/weapons", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ weaponInfoId: w.weaponInfoId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + w.weaponInfoId + ")", "success");
        if (detail.getCurrent()?.weaponInfoId === w.weaponInfoId) {
          detail.setWeapon(null);
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

/**
 * views/skill-edit.js
 * スキル一覧・編集画面 (route: skill-management)
 *
 * API: /api/skills
 *   GET    → { skills: [{ skillId, name, sp, iconId, typeMain, typeSub, classType, use, ... }] }
 *   POST   → 新規
 *   PUT    → 更新 (skillId 必須)
 *   DELETE → { skillId }
 *
 * レイアウト: list-detail (左リスト + 右フォーム)
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const SKILL_TYPE_SUB_OPTIONS = {
  0: [{ value: 0, label: "0: NONE" }],
  1: [
    { value: 0, label: "0: NONE" },
    { value: 1, label: "1: 移動して攻撃" },
    { value: 2, label: "2: 回復" },
  ],
  2: [
    { value: 0, label: "0: NONE" },
    { value: 1, label: "1: 釣り" },
  ],
};

const SKILL_TYPEMAIN_LABELS = { 0: "能力", 1: "戦闘", 2: "生活" };
const SKILL_USE_LABELS = { 0: "制限無し", 1: "通常時", 2: "戦闘モード" };

const SKILL_CLASS_MOVEATACK = 1;
const SKILL_CLASS_HEAL      = 2;

function skillClassType(typeMain, typeSub) {
  if (typeMain === 1) {
    if (typeSub === 1) { return SKILL_CLASS_MOVEATACK; }
    if (typeSub === 2) { return SKILL_CLASS_HEAL; }
  }
  return 0;
}

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  if (!el) { return; }
  el.textContent = message || "";
  el.className = "it-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

function mkEl(tag, cls, text) {
  var el = document.createElement(tag);
  if (cls) { el.className = cls; }
  if (text !== undefined) { el.textContent = text; }
  return el;
}

function mkInput(type, cls) {
  var inp = document.createElement("input");
  inp.type = type || "text";
  if (cls) { inp.className = cls; }
  return inp;
}

function mkField(labelText) {
  var lbl = mkEl("label", "form-field");
  var span = mkEl("span", "", labelText);
  lbl.appendChild(span);
  return lbl;
}

function mkSelect(options, cls) {
  var sel = document.createElement("select");
  if (cls) { sel.className = cls; }
  options.forEach(function (o) {
    var opt = document.createElement("option");
    opt.value = String(o.value);
    opt.textContent = o.label;
    sel.appendChild(opt);
  });
  return sel;
}

// ----------------------------------------------------------------
// 右ペイン: 詳細フォーム
// ----------------------------------------------------------------

function buildDetailPane(feedbackEl) {
  var pane = mkEl("div", "ee-right");

  pane.appendChild(feedbackEl);

  var actionBar = mkEl("div", "me-action-bar");
  var saveBtn = mkEl("button", "button primary", "保存");
  saveBtn.type = "button";
  var cancelBtn = mkEl("button", "button", "キャンセル / 新規");
  cancelBtn.type = "button";
  actionBar.append(saveBtn, cancelBtn);
  pane.appendChild(actionBar);

  // ---- 基本情報 ----
  var basicSec = mkEl("section", "detail-section");
  basicSec.appendChild(mkEl("h3", "", "基本情報"));
  var basicGrid = mkEl("div", "form-grid compact");

  // スキルID (readonly)
  var idLbl = mkField("スキルID");
  var idInp = mkInput("text", "form-input");
  idInp.readOnly = true;
  idInp.placeholder = "（新規は自動採番）";
  idLbl.appendChild(idInp);
  basicGrid.appendChild(idLbl);

  // スキル名
  var nameLbl = mkField("スキル名");
  var nameInp = mkInput("text", "form-input");
  nameInp.maxLength = 32;
  nameInp.placeholder = "スキル名";
  nameLbl.appendChild(nameInp);
  basicGrid.appendChild(nameLbl);

  // 種別(メイン) select
  var typeMainLbl = mkField("種別(メイン)");
  var typeMainSel = mkSelect([
    { value: 0, label: "0: 能力" },
    { value: 1, label: "1: 戦闘" },
    { value: 2, label: "2: 生活" },
  ], "form-input");
  typeMainLbl.appendChild(typeMainSel);
  basicGrid.appendChild(typeMainLbl);

  // 種別(サブ) select（動的切替）
  var typeSubLbl = mkField("種別(サブ)");
  var typeSubSel = document.createElement("select");
  typeSubSel.className = "form-input";
  typeSubLbl.appendChild(typeSubSel);
  basicGrid.appendChild(typeSubLbl);

  // 消費SP
  var spSpin = createNumberSpinner({ value: 0, min: 0, max: 9999, step: 1 });
  var spLbl = mkField("消費SP");
  spLbl.appendChild(spSpin.el);
  basicGrid.appendChild(spLbl);

  // アイコン画像ID (sprite-picker: icon32)
  var sfIcon = createSpriteField({
    categoryKey: "icon32",
    value: 0,
    label: "アイコン画像",
    allowCategorySwitch: true,
  });
  basicGrid.appendChild(sfIcon.el);

  // 使用制限 select
  var useLbl = mkField("使用制限");
  var useSel = mkSelect([
    { value: 0, label: "0: 制限無し" },
    { value: 1, label: "1: 通常時のみ" },
    { value: 2, label: "2: 戦闘モード時のみ" },
  ], "form-input");
  useLbl.appendChild(useSel);
  basicGrid.appendChild(useLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // ---- 移動攻撃フィールド ----
  var maSec = mkEl("section", "detail-section");
  maSec.appendChild(mkEl("h3", "", "移動攻撃パラメータ"));
  var maGrid = mkEl("div", "form-grid compact");

  function addSpinLbl(grid, labelText, max) {
    var lbl = mkField(labelText);
    var spin = createNumberSpinner({ value: 0, min: 0, max: max || 9999, step: 1 });
    lbl.appendChild(spin.el);
    grid.appendChild(lbl);
    return spin;
  }
  function addSelectLbl(grid, labelText, opts) {
    var lbl = mkField(labelText);
    var sel = mkSelect(opts, "form-input");
    lbl.appendChild(sel);
    grid.appendChild(lbl);
    return sel;
  }
  function addCheckLbl(grid, labelText) {
    var lbl = mkEl("label", "form-field form-field--checkbox");
    var cb = mkInput("checkbox");
    lbl.append(cb, " " + labelText);
    grid.appendChild(lbl);
    return cb;
  }

  var targetTypeSpin = addSpinLbl(maGrid, "攻撃対象", 255);
  var hitEffectIdSpin = addSpinLbl(maGrid, "ヒットエフェクトID");
  var effectId0Spin = addSpinLbl(maGrid, "エフェクトID(下)");
  var effectId1Spin = addSpinLbl(maGrid, "エフェクトID(左)");
  var effectId2Spin = addSpinLbl(maGrid, "エフェクトID(右)");
  var effectId3Spin = addSpinLbl(maGrid, "エフェクトID(上)");
  var putTypeSel = addSelectLbl(maGrid, "発射種別", [
    { value: 0, label: "0: 未設定" },
    { value: 1, label: "1: 前方" },
    { value: 2, label: "2: 上下左右" },
  ]);
  var aliveTimeSpin = addSpinLbl(maGrid, "耐久時間");
  var waitTimeSpin  = addSpinLbl(maGrid, "移動速度");
  var maValue1Spin  = addSpinLbl(maGrid, "効果値1");
  var maValue2Spin  = addSpinLbl(maGrid, "効果値2");
  var maDistSpin    = addSpinLbl(maGrid, "射程距離");
  var hitQuitCb     = addCheckLbl(maGrid, "ヒット時消滅");
  var distDelCb     = addCheckLbl(maGrid, "射程到達で消える");

  maSec.appendChild(maGrid);
  pane.appendChild(maSec);

  // ---- 回復フィールド ----
  var healSec = mkEl("section", "detail-section");
  healSec.appendChild(mkEl("h3", "", "回復パラメータ"));
  var healGrid = mkEl("div", "form-grid compact");

  var healAreaSel  = addSelectLbl(healGrid, "範囲", [
    { value: 0, label: "0: 自分位置" },
    { value: 1, label: "1: 前方" },
    { value: 2, label: "2: 周囲" },
  ]);
  var healTypeSel  = addSelectLbl(healGrid, "回復種別", [
    { value: 0, label: "0: 未設定" },
    { value: 1, label: "1: HP" },
    { value: 2, label: "2: SP" },
  ]);
  var healHitEffSpin = addSpinLbl(healGrid, "ヒットエフェクトID");
  var healVal1Spin   = addSpinLbl(healGrid, "効果値1");
  var healVal2Spin   = addSpinLbl(healGrid, "効果値2");
  var healDistSpin   = addSpinLbl(healGrid, "射程距離");

  healSec.appendChild(healGrid);
  pane.appendChild(healSec);

  // ----------------------------------------------------------------
  // typeSub 選択肢動的切替
  // ----------------------------------------------------------------
  function rebuildTypeSub(typeMain, currentSub) {
    var opts = SKILL_TYPE_SUB_OPTIONS[typeMain] || SKILL_TYPE_SUB_OPTIONS[0];
    typeSubSel.innerHTML = "";
    opts.forEach(function (o) {
      var opt = document.createElement("option");
      opt.value = String(o.value);
      opt.textContent = o.label;
      typeSubSel.appendChild(opt);
    });
    typeSubSel.value = String(currentSub || 0);
  }

  function updateDerivedFields(classType) {
    maSec.style.display   = (classType === SKILL_CLASS_MOVEATACK) ? "" : "none";
    healSec.style.display = (classType === SKILL_CLASS_HEAL)      ? "" : "none";
  }

  typeMainSel.addEventListener("change", function () {
    var typeMain = parseInt(this.value, 10) || 0;
    rebuildTypeSub(typeMain, 0);
    updateDerivedFields(skillClassType(typeMain, 0));
  });

  typeSubSel.addEventListener("change", function () {
    var typeMain = parseInt(typeMainSel.value, 10) || 0;
    var typeSub  = parseInt(this.value, 10) || 0;
    updateDerivedFields(skillClassType(typeMain, typeSub));
  });

  // 初期状態
  rebuildTypeSub(0, 0);
  updateDerivedFields(0);

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------
  var _current = null;

  function setItem(sk) {
    _current = sk || null;
    idInp.value = sk ? String(sk.skillId) : "";
    nameInp.value = sk ? (sk.name || "") : "";
    typeMainSel.value = String(sk ? (sk.typeMain || 0) : 0);
    rebuildTypeSub(sk ? (sk.typeMain || 0) : 0, sk ? (sk.typeSub || 0) : 0);
    spSpin.setValue(sk ? (sk.sp || 0) : 0);
    sfIcon.setValue(sk ? (sk.iconId || 0) : 0);
    useSel.value = String(sk ? (sk.use || 0) : 0);

    var ct = sk
      ? (sk.classType !== undefined ? sk.classType : skillClassType(sk.typeMain || 0, sk.typeSub || 0))
      : 0;
    updateDerivedFields(ct);

    // MOVEATACK
    targetTypeSpin.setValue(sk ? (sk.targetType  || 0) : 0);
    hitEffectIdSpin.setValue(sk ? (sk.hitEffectId || 0) : 0);
    effectId0Spin.setValue(sk ? (sk.effectId0 || 0) : 0);
    effectId1Spin.setValue(sk ? (sk.effectId1 || 0) : 0);
    effectId2Spin.setValue(sk ? (sk.effectId2 || 0) : 0);
    effectId3Spin.setValue(sk ? (sk.effectId3 || 0) : 0);
    putTypeSel.value = String(sk ? (sk.putType || 0) : 0);
    aliveTimeSpin.setValue(sk ? (sk.aliveTime || 0) : 0);
    waitTimeSpin.setValue(sk ? (sk.waitTime || 0) : 0);
    maValue1Spin.setValue(sk ? (sk.value1 || 0) : 0);
    maValue2Spin.setValue(sk ? (sk.value2 || 0) : 0);
    maDistSpin.setValue(sk ? (sk.distance || 0) : 0);
    hitQuitCb.checked = sk ? !!sk.hitQuit : false;
    distDelCb.checked = sk ? !!sk.distanceDelete : false;

    // HEAL
    healAreaSel.value  = String(sk ? (sk.area     || 0) : 0);
    healTypeSel.value  = String(sk ? (sk.healType || 0) : 0);
    healHitEffSpin.setValue(sk ? (sk.hitEffectId || 0) : 0);
    healVal1Spin.setValue(sk ? (sk.value1 || 0) : 0);
    healVal2Spin.setValue(sk ? (sk.value2 || 0) : 0);
    healDistSpin.setValue(sk ? (sk.distance || 0) : 0);
  }

  function collectData() {
    var typeMain = parseInt(typeMainSel.value, 10) || 0;
    var typeSub  = parseInt(typeSubSel.value, 10) || 0;
    var ct = skillClassType(typeMain, typeSub);

    var body = {
      name:     nameInp.value,
      typeMain: typeMain,
      typeSub:  typeSub,
      sp:       spSpin.getValue(),
      iconId:   sfIcon.getValue(),
      use:      parseInt(useSel.value, 10) || 0,
    };

    if (ct === SKILL_CLASS_MOVEATACK) {
      body.targetType     = targetTypeSpin.getValue();
      body.hitEffectId    = hitEffectIdSpin.getValue();
      body.effectId0      = effectId0Spin.getValue();
      body.effectId1      = effectId1Spin.getValue();
      body.effectId2      = effectId2Spin.getValue();
      body.effectId3      = effectId3Spin.getValue();
      body.putType        = parseInt(putTypeSel.value, 10) || 0;
      body.aliveTime      = aliveTimeSpin.getValue();
      body.waitTime       = waitTimeSpin.getValue();
      body.value1         = maValue1Spin.getValue();
      body.value2         = maValue2Spin.getValue();
      body.distance       = maDistSpin.getValue();
      body.hitQuit        = hitQuitCb.checked;
      body.distanceDelete = distDelCb.checked;
    } else if (ct === SKILL_CLASS_HEAL) {
      body.area        = parseInt(healAreaSel.value, 10) || 0;
      body.healType    = parseInt(healTypeSel.value, 10) || 0;
      body.hitEffectId = healHitEffSpin.getValue();
      body.value1      = healVal1Spin.getValue();
      body.value2      = healVal2Spin.getValue();
      body.distance    = healDistSpin.getValue();
    }

    return body;
  }

  return {
    el: pane, saveBtn, cancelBtn,
    setItem, collectData,
    getCurrent: function () { return _current; },
    setCurrent: function (sk) { _current = sk; },
  };
}

// ----------------------------------------------------------------
// 左ペイン: 一覧
// ----------------------------------------------------------------

function buildLeftPane({ onSelect, onNew, onDelete }) {
  var pane = mkEl("div", "ee-left");

  var searchInp = document.createElement("input");
  searchInp.type = "search";
  searchInp.placeholder = "ID または 名前で検索…";
  searchInp.className = "ld-search";
  pane.appendChild(searchInp);

  var toolbar = mkEl("div", "me-list-toolbar");
  var newBtn = mkEl("button", "button small", "+ 新規追加");
  newBtn.type = "button";
  newBtn.addEventListener("click", onNew);
  toolbar.appendChild(newBtn);
  pane.appendChild(toolbar);

  var listEl = mkEl("ul", "ld-list");
  pane.appendChild(listEl);

  var summary = mkEl("p", "muted me-list-summary");
  pane.appendChild(summary);

  var _allItems = [];
  var _selectedId = null;

  async function loadList() {
    var { response, data } = await fetchJson("/api/skills");
    if (!response.ok || !Array.isArray(data && data.skills)) {
      console.error("skill-edit list load error");
      return;
    }
    _allItems = data.skills;
    renderList();
  }

  function renderList() {
    var q = searchInp.value.trim().toLowerCase();
    var filtered = _allItems.filter(function (sk) {
      if (!q) { return true; }
      return String(sk.skillId).includes(q) || (sk.name || "").toLowerCase().includes(q);
    });

    listEl.innerHTML = "";
    filtered.forEach(function (sk) {
      var li = mkEl("li", "ld-list-item" + (sk.skillId === _selectedId ? " selected" : ""));

      // アイコンサムネ
      if (sk.iconId) {
        var thumb = createSpriteThumb({ categoryKey: "icon32", sub: sk.iconId, size: 24 });
        thumb.el.className = "ld-item-thumb";
        li.appendChild(thumb.el);
      }

      var mainLbl = SKILL_TYPEMAIN_LABELS[sk.typeMain] || String(sk.typeMain);
      var label = mkEl("span", "", "[" + sk.skillId + "] " + (sk.name || "(名前なし)") + " (" + mainLbl + ")");
      li.appendChild(label);

      li.addEventListener("click", function () {
        _selectedId = sk.skillId;
        renderList();
        onSelect(sk);
      });

      var delBtn = mkEl("button", "ld-item-del button small", "削除");
      delBtn.type = "button";
      delBtn.addEventListener("click", function (ev) { ev.stopPropagation(); onDelete(sk); });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });
    summary.textContent = filtered.length + " 件";
  }

  searchInp.addEventListener("input", renderList);
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

  var shell = mkEl("div", "me-shell");
  container.appendChild(shell);

  var feedbackEl = mkEl("p", "it-feedback result-message");
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  var detail = buildDetailPane(feedbackEl);

  // 保存
  detail.saveBtn.addEventListener("click", async function () {
    var payload = detail.collectData();
    var current = detail.getCurrent();
    var isNew = !current || !current.skillId;
    if (!isNew) { payload.skillId = current.skillId; }

    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      var { response, data } = await fetchJson("/api/skills", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data && data.error ? data.error : "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      await leftApi.reload();
      if (isNew && data && data.skillId) {
        detail.setCurrent(Object.assign({}, payload, { skillId: data.skillId }));
        detail.setItem(Object.assign({}, payload, { skillId: data.skillId }));
      }
    } catch (err) {
      showFeedback(feedbackEl, "通信エラー: " + err.message, "error");
    }
  });

  // キャンセル/新規
  detail.cancelBtn.addEventListener("click", function () {
    detail.setItem(null);
    showFeedback(feedbackEl, "", "");
  });

  var leftApi = buildLeftPane({
    onSelect: function (sk) {
      detail.setItem(sk);
      showFeedback(feedbackEl, "", "");
    },
    onNew: function () {
      detail.setItem(null);
      showFeedback(feedbackEl, "", "");
    },
    onDelete: async function (sk) {
      if (!confirm("スキル [" + (sk.name || "") + "] (ID=" + sk.skillId + ") を削除しますか？")) { return; }
      try {
        var { response, data } = await fetchJson("/api/skills", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ skillId: sk.skillId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data && data.error ? data.error : "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + sk.skillId + ")", "success");
        var cur = detail.getCurrent();
        if (cur && cur.skillId === sk.skillId) {
          detail.setItem(null);
        }
        await leftApi.reload();
      } catch (err) {
        showFeedback(feedbackEl, "通信エラー: " + err.message, "error");
      }
    },
  });

  shell.appendChild(leftApi.el);
  shell.appendChild(detail.el);

  _destroyFn = function () {
    container.innerHTML = "";
  };
}

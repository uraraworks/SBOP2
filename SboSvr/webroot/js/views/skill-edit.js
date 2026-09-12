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
import { withBusy, armConfirmButton } from "../core/dom.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createNumberSpinner } from "../components/number-spinner.js";
import { createEntityField, invalidateEntityCache } from "../components/entity-picker.js";
import { createListToolbar } from "../components/list-toolbar.js";
import { getRouteParams, setRouteParams } from "../core/router.js";
import { registerSaveHandler, unregisterSaveHandler } from "../core/save-shortcut.js";

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
  var dupBtn = mkEl("button", "button small", "複製して新規");
  dupBtn.type = "button";
  actionBar.append(saveBtn, dupBtn, cancelBtn);
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
  var hitEffectField = createEntityField({ type: "effect", value: 0, label: "ヒットエフェクトID" });
  maGrid.appendChild(hitEffectField.element);

  var effectId0Field = createEntityField({ type: "effect", value: 0, label: "エフェクトID(下)" });
  var effectId1Field = createEntityField({ type: "effect", value: 0, label: "エフェクトID(左)" });
  var effectId2Field = createEntityField({ type: "effect", value: 0, label: "エフェクトID(右)" });
  var effectId3Field = createEntityField({ type: "effect", value: 0, label: "エフェクトID(上)" });
  maGrid.appendChild(effectId0Field.element);
  maGrid.appendChild(effectId1Field.element);
  maGrid.appendChild(effectId2Field.element);
  maGrid.appendChild(effectId3Field.element);

  var sameAsAboveBtn = mkEl("button", "button small", "↑ 上と同じにする（4方向）");
  sameAsAboveBtn.type = "button";
  sameAsAboveBtn.addEventListener("click", function () {
    var v = effectId0Field.getValue();
    effectId1Field.setValue(v);
    effectId2Field.setValue(v);
    effectId3Field.setValue(v);
  });
  maGrid.appendChild(sameAsAboveBtn);

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
  var healHitEffField = createEntityField({ type: "effect", value: 0, label: "ヒットエフェクトID" });
  healGrid.appendChild(healHitEffField.element);
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
    hitEffectField.setValue(sk ? (sk.hitEffectId || 0) : 0);
    effectId0Field.setValue(sk ? (sk.effectId0 || 0) : 0);
    effectId1Field.setValue(sk ? (sk.effectId1 || 0) : 0);
    effectId2Field.setValue(sk ? (sk.effectId2 || 0) : 0);
    effectId3Field.setValue(sk ? (sk.effectId3 || 0) : 0);
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
    healHitEffField.setValue(sk ? (sk.hitEffectId || 0) : 0);
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
      body.hitEffectId    = hitEffectField.getValue();
      body.effectId0      = effectId0Field.getValue();
      body.effectId1      = effectId1Field.getValue();
      body.effectId2      = effectId2Field.getValue();
      body.effectId3      = effectId3Field.getValue();
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
      body.hitEffectId = healHitEffField.getValue();
      body.value1      = healVal1Spin.getValue();
      body.value2      = healVal2Spin.getValue();
      body.distance    = healDistSpin.getValue();
    }

    return body;
  }

  return {
    el: pane, saveBtn, dupBtn, cancelBtn,
    setItem, collectData,
    getCurrent: function () { return _current; },
    setCurrent: function (sk) { _current = sk; },
  };
}

// ----------------------------------------------------------------
// 左ペイン: 一覧
// ----------------------------------------------------------------

function buildLeftPane({ onSelect, onNew, onDelete, routeParams }) {
  var pane = mkEl("div", "ee-left");

  var toolbar = createListToolbar({
    placeholder: "ID・名前で検索",
    sortOptions: [
      { value: "id", label: "ID順" },
      { value: "name", label: "名前順" },
      { value: "type", label: "種別順" },
    ],
    pageSizes: [20, 50, 100],
    initial: {
      q: routeParams.get("q") || "",
      sort: routeParams.get("sort") || "id",
      page: Number(routeParams.get("page")) || 1,
    },
    onChange: function (s) {
      setRouteParams({
        q: s.q || null,
        sort: s.sort && s.sort !== "id" ? s.sort : null,
        page: s.page && s.page !== 1 ? s.page : null,
      });
      renderList();
    },
  });
  pane.appendChild(toolbar.element);

  var actionsBar = mkEl("div", "me-list-toolbar");
  var newBtn = mkEl("button", "button small", "+ 新規追加");
  newBtn.type = "button";
  newBtn.addEventListener("click", onNew);
  actionsBar.appendChild(newBtn);
  pane.appendChild(actionsBar);

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
    var result = toolbar.applyToRows(_allItems, {
      searchFields: ["name", function (sk) { return String(sk.skillId); }],
      sorters: {
        id: function (a, b) { return a.skillId - b.skillId; },
        name: function (a, b) { return String(a.name || "").localeCompare(String(b.name || "")); },
        type: function (a, b) { return (a.typeMain || 0) - (b.typeMain || 0) || (a.typeSub || 0) - (b.typeSub || 0); },
      },
    });
    var pageRows = result.pageRows;

    listEl.innerHTML = "";
    pageRows.forEach(function (sk) {
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
      delBtn.addEventListener("click", function (ev) { ev.stopPropagation(); });
      // 参照件数はキャラクター単位（/api/characters/:id/skills）でしか
      // 取得できず一覧APIが無いため全件集計は不可能。二度押し確認のみ行う。
      armConfirmButton(delBtn, {
        armedLabel: "本当に削除？",
        onConfirm: function () { onDelete(sk); },
      });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });
    summary.textContent = result.total + " 件";
  }

  var ready = loadList();

  return {
    el: pane,
    reload: loadList,
    ready: ready,
    selectById: function (id) {
      var sk = _allItems.find(function (x) { return x.skillId === id; });
      if (sk) {
        _selectedId = id;
        renderList();
        onSelect(sk);
      }
      return sk || null;
    },
  };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

const ROUTE = "skill-management";

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  var routeParams = getRouteParams();

  var shell = mkEl("div", "me-shell");
  container.appendChild(shell);

  var feedbackEl = mkEl("p", "it-feedback result-message");
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  var detail = buildDetailPane(feedbackEl);

  // 保存（Ctrl+S からも呼べるよう名前付き関数にしてある）
  async function saveSkill() {
    var payload = detail.collectData();
    var current = detail.getCurrent();
    var isNew = !current || !current.skillId;
    if (!isNew) { payload.skillId = current.skillId; }

    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    await withBusy(detail.saveBtn, async function () {
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
        if (isNew && data && data.skillId) {
          detail.setCurrent(Object.assign({}, payload, { skillId: data.skillId }));
          detail.setItem(Object.assign({}, payload, { skillId: data.skillId }));
          setRouteParams({ id: data.skillId });
        }
        invalidateEntityCache("skill");
        await leftApi.reload();
      } catch (err) {
        showFeedback(feedbackEl, "通信エラー: " + err.message, "error");
      }
    });
  }
  detail.saveBtn.addEventListener("click", saveSkill);

  // Ctrl+S: 詳細ペインを開いている時だけ保存ボタンと同じ処理を呼ぶ
  registerSaveHandler(ROUTE, function () {
    if (detail.el.style.display !== "none") { saveSkill(); }
  });

  // 複製して新規 → 表示中のスキルをコピーして別レコードとして保存
  // (POST は常にサーバー側で新規 skillId を採番する。SkillHandler.cpp:386
  //  で Add() 直前に m_dwSkillID = 0 に強制しているため、body に skillId を
  //  含めても既存レコードを上書きする心配は無い)
  detail.dupBtn.addEventListener("click", function () {
    withBusy(detail.dupBtn, async function () {
      var current = detail.getCurrent();
      if (!current) {
        showFeedback(feedbackEl, "複製元のスキルを選択してください", "error");
        return;
      }
      var payload = detail.collectData();
      payload.name = (payload.name || "") + "のコピー";
      showFeedback(feedbackEl, "複製中…", "");
      try {
        var { response, data } = await fetchJson("/api/skills", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "エラー: " + (data && data.error ? data.error : "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "複製しました (ID=" + data.skillId + ")", "success");
        var created = Object.assign({}, payload, { skillId: data.skillId });
        detail.setCurrent(created);
        detail.setItem(created);
        setRouteParams({ id: data.skillId });
        invalidateEntityCache("skill");
        await leftApi.reload();
      } catch (err) {
        showFeedback(feedbackEl, "通信エラー: " + err.message, "error");
      }
    }, { busyText: "複製中…" });
  });

  // キャンセル/新規 → フォームクリア + 一覧に戻る
  detail.cancelBtn.addEventListener("click", function () {
    detail.setItem(null);
    showFeedback(feedbackEl, "", "");
    showList();
  });

  var leftApi = buildLeftPane({
    routeParams: routeParams,
    onSelect: function (sk) {
      detail.setItem(sk);
      showFeedback(feedbackEl, "", "");
      showDetail();
      setRouteParams({ id: sk.skillId });
    },
    onNew: function () {
      detail.setItem(null);
      showFeedback(feedbackEl, "", "");
      showDetail();
      setRouteParams({ id: null });
    },
    onDelete: async function (sk) {
      // 削除確認は armConfirmButton（一覧の削除ボタン自体を二度押しで確定）で
      // 済んでいるため、ここでは confirm() を使わない。
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
          setRouteParams({ id: null });
        }
        invalidateEntityCache("skill");
        await leftApi.reload();
      } catch (err) {
        showFeedback(feedbackEl, "通信エラー: " + err.message, "error");
      }
    },
  });

  // 詳細ペインの先頭に「← 戻る」ボタンを追加
  var backBar = mkEl("div", "me-action-bar");
  var backBtn = mkEl("button", "button small");
  backBtn.type = "button";
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
    setRouteParams({ id: null });
  }

  backBtn.addEventListener("click", showList);

  // 初期状態: 一覧のみ表示
  detail.el.style.display = "none";

  shell.appendChild(leftApi.el);
  shell.appendChild(detail.el);

  // URL の id パラメータから選択状態を復元（一覧読み込み完了後）
  leftApi.ready.then(function () {
    var idParam = parseInt(routeParams.get("id"), 10);
    if (!isNaN(idParam)) {
      leftApi.selectById(idParam);
    }
  });

  _destroyFn = function () {
    unregisterSaveHandler(ROUTE);
    container.innerHTML = "";
  };
}

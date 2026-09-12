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
import { withBusy, armConfirmButton } from "../core/dom.js";
import { openEntityPicker, createEntityField, invalidateEntityCache } from "../components/entity-picker.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { loadCatalog } from "../data/assets.js";

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

// ----------------------------------------------------------------
// 立ち/すり足モーションID の実体（CHARMOTIONLISTID、Common/Info/InfoMotion.h）
//
// SboCliAdminMfc/src/Migrated/Dialog/DlgAdminItemWeaponNew.cpp の
// OnInitDialog() が m_ctlStandMotion / m_ctlWalkMotion に積んでいる選択肢と
// 完全に同じ固定リスト（"モーション種別ID" でも自由な "モーションID" でも
// なく、CHARMOTIONLISTID enum の固定値＝モーションリストIDの一種）。
// 実際の描画は LibInfoCharCli.cpp が
//   pLibInfoItem->GetMotionIDBattleStand()/GetMotionIDBattleWalk() で
//   この値を取得し、キャラ自身の motionTypeId と組み合わせて
//   pLibInfoMotion->GetMotionInfo(motionTypeId, dwListID + 方向) を引く。
// つまり「武器を戦闘モードで構えた時、どの動作系統（素手/弓/打撃/釣り）の
// 立ち・すり足を使うか」を選ぶフィールドであり、キャラ本体の見た目
// （motionTypeId）とは独立。
// ----------------------------------------------------------------

// CHARMOTIONLISTID_* の値（Common/Info/InfoMotion.h の enum 順で採番）
const STAND_MOTION_OPTIONS = [
  { value: 13, label: "戦闘立ち(上)" },        // CHARMOTIONLISTID_BATTLESTAND_UP
  { value: 57, label: "弓用戦闘立ち(上)" },     // CHARMOTIONLISTID_BOWBATTLESTAND_UP
  { value: 61, label: "打撃用戦闘立ち(上)" },   // CHARMOTIONLISTID_GLOVEBATTLESTAND_UP
  { value: 65, label: "釣り用戦闘立ち(上)" },   // CHARMOTIONLISTID_FISHINGBATTLESTAND_UP
];

const WALK_MOTION_OPTIONS = [
  { value: 1,  label: "立ち(上)" },             // CHARMOTIONLISTID_STAND_UP
  { value: 17, label: "すり足(上)" },           // CHARMOTIONLISTID_BATTLEWALK_UP
  { value: 29, label: "弓用すり足(上)" },       // CHARMOTIONLISTID_BOWWALK_UP
  { value: 37, label: "打撃用すり足(上)" },     // CHARMOTIONLISTID_GLOVEWALK_UP
  { value: 45, label: "釣り用すり足(上)" },     // CHARMOTIONLISTID_FISHINGWALK_UP
];

// プレビューでは正面向き（下方向 = UP値 + 1）を表示する
const MOTION_LIST_DOWN_OFFSET = 1;

function motionOptionLabel(options, value) {
  const found = options.find((o) => o.value === Number(value));
  return found ? found.label : "#" + value;
}

// 固定選択肢の <select>。保存済みデータが選択肢に無い値（旧データや手動編集分）
// の場合は「不明な値」として選択肢に追加し、無断で別の値に書き換えないようにする。
function createMotionListSelect(options) {
  const select = document.createElement("select");
  select.className = "form-input";

  let _unknownOpt = null;

  function rebuildOptions(current) {
    select.innerHTML = "";
    options.forEach((o) => {
      const opt = document.createElement("option");
      opt.value = String(o.value);
      opt.textContent = o.label + "（#" + o.value + "）";
      select.appendChild(opt);
    });
    _unknownOpt = null;
    const known = options.some((o) => o.value === Number(current));
    if (!known) {
      _unknownOpt = document.createElement("option");
      _unknownOpt.value = String(current ?? 0);
      _unknownOpt.textContent = "#" + (current ?? 0) + "（不明な値）";
      select.appendChild(_unknownOpt);
    }
    select.value = String(current ?? options[0].value);
  }

  rebuildOptions(options[0].value);

  return {
    el: select,
    getValue: () => parseInt(select.value, 10) || 0,
    setValue: (v) => rebuildOptions(v),
  };
}

// ----------------------------------------------------------------
// モーションプレビュー用データ取得・合成（motion-edit.js と同じ考え方を
// 武器プレビュー向けに最小限で再実装したもの）
// ----------------------------------------------------------------

const PREVIEW_LAYERS = [
  { mainField: "grpIdMainBase",  subField: "grpIdSubBase",  dpx: "drawPosPile0x", dpy: "drawPosPile0y", levelField: null,     fallbackKey: "char" },
  { mainField: "grpIdMainPile1", subField: "grpIdSubPile1", dpx: "drawPosPile1x", dpy: "drawPosPile1y", levelField: "level1", fallbackKey: "weapon" },
  { mainField: "grpIdMainPile2", subField: "grpIdSubPile2", dpx: "drawPosPile2x", dpy: "drawPosPile2y", levelField: "level2", fallbackKey: "weapon" },
  { mainField: "grpIdMainPile3", subField: "grpIdSubPile3", dpx: "drawPosPile3x", dpy: "drawPosPile3y", levelField: "level3", fallbackKey: "weapon" },
];

const MOTION_TYPE_SHEET_CATEGORY_KEYS = new Set(["npc", "npc2x2"]);

function mainIdToCategory(catalog, idMain, fallbackKey) {
  return catalog.find((c) => Number(c.idMain) === Number(idMain))
    ?? catalog.find((c) => c.key === fallbackKey)
    ?? catalog[0];
}

function cellsPerSheet(cat) {
  return Math.max(1, Number(cat?.countX ?? 1) * Number(cat?.countY ?? 1));
}

function storedSubToContextSub(value, cat, motionType) {
  const cell = Math.max(0, (Number(value) || 0) - 1);
  if (!MOTION_TYPE_SHEET_CATEGORY_KEYS.has(cat?.key)) return cell;
  const sheetIndex = Math.max(0, Number(motionType?.grpIdSub ?? 0) || 0);
  return sheetIndex * cellsPerSheet(cat) + (cell % cellsPerSheet(cat));
}

// CInfoMotion(1フレーム分) → createAnimePreview 用レイヤー配列
function motionFrameToPreviewLayers(frame, catalog, motionType) {
  const drawList = Array.isArray(frame.drawList) && frame.drawList.length ? frame.drawList : [0, 1, 2, 3];
  return drawList.map((idx) => {
    const layer = PREVIEW_LAYERS[idx];
    if (!layer) return null;
    const sub = Number(frame[layer.subField] ?? 0);
    if (sub <= 0) return null;
    const cat = mainIdToCategory(catalog, frame[layer.mainField], layer.fallbackKey);
    const alpha = layer.levelField ? (Number(frame[layer.levelField] ?? 255) / 255) : 1;
    return {
      categoryKey: cat.key,
      sub: storedSubToContextSub(sub, cat, motionType),
      offsetX: Math.round(Number(frame[layer.dpx] ?? 0) / 2),
      offsetY: Math.round(Number(frame[layer.dpy] ?? 0) / 2),
      alpha,
    };
  }).filter(Boolean);
}

const _motionsByTypeCache = new Map();

function loadMotionsForType(motionTypeId) {
  if (!motionTypeId) return Promise.resolve([]);
  if (_motionsByTypeCache.has(motionTypeId)) return _motionsByTypeCache.get(motionTypeId);
  const promise = fetchJson("/api/motions?motionTypeId=" + motionTypeId)
    .then(({ response, data }) => {
      if (!response.ok || !Array.isArray(data?.motions)) return [];
      return data.motions;
    })
    .catch(() => []);
  _motionsByTypeCache.set(motionTypeId, promise);
  return promise;
}

// ----------------------------------------------------------------
// エフェクト名解決（/api/effects を一度だけ取得して Map 化）
// ----------------------------------------------------------------

let _effectNameMapPromise = null;

function loadEffectNameMap() {
  if (!_effectNameMapPromise) {
    _effectNameMapPromise = fetchJson("/api/effects")
      .then(({ response, data }) => {
        const map = new Map();
        if (response.ok && Array.isArray(data?.items)) {
          data.items.forEach((e) => { map.set(e.effectId, e.name); });
        }
        return map;
      })
      .catch(() => new Map());
  }
  return _effectNameMapPromise;
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

// エフェクト ID の配列を「チップ（名前＋削除×）＋追加ボタン」で編集する部品。
// カンマ区切り文字列入力の置き換え。内部表現は従来どおり配列のまま。
function createEffectChipList() {
  const wrap = document.createElement("div");
  wrap.className = "form-field";

  const chipWrap = document.createElement("div");
  chipWrap.style.cssText = "display:flex;gap:0.45rem;flex-wrap:wrap;margin-bottom:0.35rem;min-height:2rem;";
  wrap.appendChild(chipWrap);

  const addBtn = document.createElement("button");
  addBtn.type = "button";
  addBtn.className = "button small";
  addBtn.textContent = "+ エフェクトを追加";
  wrap.appendChild(addBtn);

  let _ids = [];
  const _nameCache = new Map(); // id -> name（picker で選んだ時に上書き解決）

  // 全エフェクト一覧から名前を解決し、届いたら再描画する。
  loadEffectNameMap().then((map) => {
    map.forEach((name, id) => { if (!_nameCache.has(id)) { _nameCache.set(id, name); } });
    render();
  });

  function render() {
    chipWrap.innerHTML = "";
    if (_ids.length === 0) {
      const empty = document.createElement("span");
      empty.className = "muted";
      empty.textContent = "（未設定）";
      chipWrap.appendChild(empty);
    }
    _ids.forEach((id) => {
      const chip = document.createElement("span");
      chip.className = "me-frame-chip";
      chip.style.cssText = "display:inline-flex;align-items:center;gap:0.35rem;cursor:default;";
      const text = document.createElement("span");
      const label = _nameCache.get(id);
      text.textContent = label ? "#" + id + " " + label : "#" + id;
      chip.appendChild(text);
      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.textContent = "×";
      delBtn.title = "削除";
      delBtn.style.cssText = "border:none;background:transparent;cursor:pointer;font-weight:bold;line-height:1;padding:0;";
      delBtn.addEventListener("click", () => {
        _ids = _ids.filter((x) => x !== id);
        render();
      });
      chip.appendChild(delBtn);
      chipWrap.appendChild(chip);
    });
  }

  addBtn.addEventListener("click", () => {
    openEntityPicker({
      type: "effect",
      ownerDocument: addBtn.ownerDocument,
      onSelect: (id, row) => {
        if (row && row.name) { _nameCache.set(id, row.name); }
        if (!_ids.includes(id)) { _ids.push(id); }
        render();
      },
    });
  });

  render();

  return {
    element: wrap,
    getValue: () => _ids.slice(),
    setValue: (ids) => { _ids = Array.isArray(ids) ? ids.slice() : []; render(); },
  };
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
  const dupBtn = document.createElement("button");
  dupBtn.type = "button";
  dupBtn.className = "button small";
  dupBtn.textContent = "複製して新規";
  actionBar.append(saveBtn, dupBtn, cancelBtn);
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

  // 立ち/すり足モーションID（実体は CHARMOTIONLISTID の固定値。上部の解説コメント参照）
  const motionGrid = document.createElement("div");
  motionGrid.className = "form-grid compact";
  motionGrid.style.marginTop = "0.5rem";

  const standLbl = makeFormField("立ちモーションID（戦闘モード時）");
  const standSelect = createMotionListSelect(STAND_MOTION_OPTIONS);
  standLbl.appendChild(standSelect.el);
  motionGrid.appendChild(standLbl);

  const walkLbl = makeFormField("すり足モーションID（戦闘モード時）");
  const walkSelect = createMotionListSelect(WALK_MOTION_OPTIONS);
  walkLbl.appendChild(walkSelect.el);
  motionGrid.appendChild(walkLbl);

  motionSec.appendChild(motionGrid);
  pane.appendChild(motionSec);

  // --- モーションプレビュー ---
  const previewSec = document.createElement("section");
  previewSec.className = "detail-section me-preview-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "モーションプレビュー";
  previewSec.appendChild(previewH3);

  const previewNote = document.createElement("p");
  previewNote.className = "muted";
  previewNote.textContent = "立ち/すり足モーションIDは「戦闘モード中にどの動作系統を使うか」の指定で、実際の見た目はキャラクター自身のモーション種別と組み合わさって決まります。下でプレビュー用のモーション種別（キャラ見た目）を選ぶと、正面向きの動きを確認できます。";
  previewSec.appendChild(previewNote);

  const previewMotionTypeField = createEntityField({
    type: "motionType",
    label: "プレビュー用モーション種別",
    value: 0,
    onChange: () => updatePreview(),
  });
  previewSec.appendChild(previewMotionTypeField.element);

  const previewToggleWrap = document.createElement("div");
  previewToggleWrap.style.cssText = "display:flex;gap:0.5rem;margin:0.5rem 0;";
  const standToggleBtn = document.createElement("button");
  standToggleBtn.type = "button";
  standToggleBtn.className = "button small primary";
  standToggleBtn.textContent = "立ちを再生";
  const walkToggleBtn = document.createElement("button");
  walkToggleBtn.type = "button";
  walkToggleBtn.className = "button small";
  walkToggleBtn.textContent = "すり足を再生";
  previewToggleWrap.append(standToggleBtn, walkToggleBtn);
  previewSec.appendChild(previewToggleWrap);

  const previewAnime = createAnimePreview({ width: 64, height: 64, scale: 2 });
  previewSec.appendChild(previewAnime.el);

  const previewStatus = document.createElement("p");
  previewStatus.className = "muted";
  previewStatus.style.marginTop = "0.35rem";
  previewSec.appendChild(previewStatus);

  pane.appendChild(previewSec);

  let _catalog = null;
  loadCatalog().then((c) => { _catalog = c; updatePreview(); }).catch(() => {});

  let _previewWhich = "stand"; // "stand" | "walk"

  function setPreviewWhich(which) {
    _previewWhich = which;
    standToggleBtn.classList.toggle("primary", which === "stand");
    walkToggleBtn.classList.toggle("primary", which === "walk");
    updatePreview();
  }
  standToggleBtn.addEventListener("click", () => setPreviewWhich("stand"));
  walkToggleBtn.addEventListener("click", () => setPreviewWhich("walk"));

  let _updateSeq = 0;

  async function updatePreview() {
    const seq = ++_updateSeq;
    const motionTypeId = previewMotionTypeField.getValue();
    const listId = _previewWhich === "stand" ? standSelect.getValue() : walkSelect.getValue();

    if (!_catalog || !motionTypeId || !listId) {
      previewAnime.setFrames([]);
      previewStatus.textContent = motionTypeId ? "" : "プレビュー用モーション種別を選択してください。";
      return;
    }

    previewStatus.textContent = "読み込み中…";
    const motions = await loadMotionsForType(motionTypeId);
    if (seq !== _updateSeq) return; // 途中で別の選択に切り替わった

    const motionType = { motionTypeId }; // grpIdSub 等は一覧APIに含まれないため未指定（char系では未使用）
    const targetListId = listId + MOTION_LIST_DOWN_OFFSET; // 正面向き（下）
    const frames = motions
      .filter((m) => Number(m.motionListId) === targetListId)
      .sort((a, b) => a.motionId - b.motionId);

    if (frames.length === 0) {
      previewAnime.setFrames([]);
      previewStatus.textContent =
        "選択中のモーション種別には「" + motionOptionLabel(
          _previewWhich === "stand" ? STAND_MOTION_OPTIONS : WALK_MOTION_OPTIONS, listId
        ) + "」に対応するモーション（正面向き）が未設定です。";
      return;
    }

    previewAnime.setFrames(frames.map((f) => ({
      wait: Number(f.wait ?? 10),
      layers: motionFrameToPreviewLayers(f, _catalog, motionType),
    })));
    previewStatus.textContent = frames.length + " フレーム";
  }

  standSelect.el.addEventListener("change", updatePreview);
  walkSelect.el.addEventListener("change", updatePreview);

  // --- エフェクトID ---
  const effectSec = document.createElement("section");
  effectSec.className = "detail-section";
  const effectH3 = document.createElement("h3");
  effectH3.textContent = "エフェクトID";
  effectSec.appendChild(effectH3);

  const effectGrid = document.createElement("div");
  effectGrid.className = "form-grid compact";

  const atkLbl = makeFormField("通常攻撃時");
  const atkChips = createEffectChipList();
  atkLbl.appendChild(atkChips.element);
  effectGrid.appendChild(atkLbl);

  const criLbl = makeFormField("クリティカル時");
  const criChips = createEffectChipList();
  criLbl.appendChild(criChips.element);
  effectGrid.appendChild(criLbl);

  effectSec.appendChild(effectGrid);
  pane.appendChild(effectSec);

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  let _current = null;

  function setWeapon(w) {
    _current = w || null;
    nameInput.value = w ? (w.name || "") : "";

    const mt = w ? (w.motionType || 0) : 0;
    motionCbs.forEach(({ bit, cb }) => { cb.checked = !!(mt & bit); });

    standSelect.setValue(w ? (w.motionTypeStand || 0) : STAND_MOTION_OPTIONS[0].value);
    walkSelect.setValue(w ? (w.motionTypeWalk || 0) : WALK_MOTION_OPTIONS[0].value);

    atkChips.setValue(w ? (w.effectIdAtack || []) : []);
    criChips.setValue(w ? (w.effectIdCritical || []) : []);

    updatePreview();
  }

  function collectData() {
    let motionType = 0;
    motionCbs.forEach(({ bit, cb }) => { if (cb.checked) { motionType |= bit; } });
    return {
      name:             nameInput.value,
      motionType:       motionType,
      motionTypeStand:  standSelect.getValue(),
      motionTypeWalk:   walkSelect.getValue(),
      effectIdAtack:    atkChips.getValue(),
      effectIdCritical: criChips.getValue(),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(w) { _current = w; }

  return { el: pane, saveBtn, dupBtn, cancelBtn, setWeapon, collectData, getCurrent, setCurrent };
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
  // weaponInfoId -> それを参照している /api/item-types の件数。削除前警告に使う。
  let _refCounts = new Map();

  async function loadList() {
    const [weaponsResult, itemTypesResult] = await Promise.all([
      fetchJson("/api/weapons"),
      fetchJson("/api/item-types"),
    ]);
    const { response, data } = weaponsResult;
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("weapons load error");
      return;
    }
    _allItems = data.items;

    _refCounts = new Map();
    if (itemTypesResult.response.ok && Array.isArray(itemTypesResult.data?.items)) {
      itemTypesResult.data.items.forEach((it) => {
        const wid = it.weaponInfoId;
        if (wid) {
          _refCounts.set(wid, (_refCounts.get(wid) || 0) + 1);
        }
      });
    }

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
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); });
      const refCount = _refCounts.get(w.weaponInfoId) || 0;
      armConfirmButton(delBtn, {
        armedLabel: "本当に削除？",
        message: refCount > 0 ? refCount + " 件のアイテム種別がこの武器情報を参照しています" : undefined,
        onConfirm: () => onDelete(w),
      });
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
      invalidateEntityCache("weapon");
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // 複製して新規 → 表示中のレコードをコピーして別レコードとして保存
  // (POST は weaponInfoId を受け付けず、Add 時に常に新規採番されるため
  //  既存レコードを上書きする心配は無い。WeaponHandler.cpp:328 参照)
  detail.dupBtn.addEventListener("click", () => {
    withBusy(detail.dupBtn, async () => {
      const current = detail.getCurrent();
      if (!current) {
        showFeedback(feedbackEl, "複製元の武器情報を選択してください", "error");
        return;
      }
      const payload = detail.collectData();
      payload.name = (payload.name || "") + "のコピー";
      showFeedback(feedbackEl, "複製中…", "");
      try {
        const { response, data } = await fetchJson("/api/weapons", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "複製しました (ID=" + data.weaponInfoId + ")", "success");
        const created = { ...payload, weaponInfoId: data.weaponInfoId };
        detail.setCurrent(created);
        detail.setWeapon(created);
        invalidateEntityCache("weapon");
        await leftApi.reload();
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    }, { busyText: "複製中…" });
  });

  // キャンセル/新規 → フォームクリア + 一覧に戻る
  detail.cancelBtn.addEventListener("click", () => {
    detail.setWeapon(null);
    showFeedback(feedbackEl, "", "");
    showList();
  });

  const leftApi = buildLeftPane({
    onSelect: (w) => {
      detail.setWeapon(w);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onNew: () => {
      detail.setWeapon(null);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onDelete: async (w) => {
      // 削除確認は armConfirmButton（一覧の削除ボタン自体を二度押しで確定）で
      // 済んでいるため、ここでは confirm() を使わない。
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
        invalidateEntityCache("weapon");
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

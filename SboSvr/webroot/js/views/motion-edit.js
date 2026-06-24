/**
 * views/motion-edit.js
 * モーション編集画面 (route: motion-edit)
 */

import { fetchJson } from "../core/api.js";
import { loadCatalog } from "../data/assets.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";
import { createDragList } from "../components/drag-list.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { createSoundPicker } from "../components/sound-picker.js";

const LAYERS = [
  { key: "Base",  label: "下地", mainField: "grpIdMainBase",  subField: "grpIdSubBase",  dpx: "drawPosPile0x", dpy: "drawPosPile0y", levelField: null },
  { key: "Pile1", label: "重ね1", mainField: "grpIdMainPile1", subField: "grpIdSubPile1", dpx: "drawPosPile1x", dpy: "drawPosPile1y", levelField: "level1" },
  { key: "Pile2", label: "重ね2", mainField: "grpIdMainPile2", subField: "grpIdSubPile2", dpx: "drawPosPile2x", dpy: "drawPosPile2y", levelField: "level2" },
  { key: "Pile3", label: "重ね3", mainField: "grpIdMainPile3", subField: "grpIdSubPile3", dpx: "drawPosPile3x", dpy: "drawPosPile3y", levelField: "level3" },
];

const BASE_CATEGORY_KEYS = new Set(["char", "npc", "char2x2", "npc2x2"]);
const MOTION_TYPE_SHEET_CATEGORY_KEYS = new Set(["npc", "npc2x2"]);
const PILE_CATEGORY_KEYS = new Set([
  "none", "weapon", "weaponBow", "effect32", "effect64", "weaponGlove", "weaponEtc",
  "arms2x2", "shield2x2", "armsSp2x2", "bow2x2",
]);

const MOTION_LIST_NAMES = {
  1: "立ち(上)", 2: "立ち(下)", 3: "立ち(左)", 4: "立ち(右)",
  5: "歩き(上)", 6: "歩き(下)", 7: "歩き(左)", 8: "歩き(右)",
  9: "座り(上)", 10: "座り(下)", 11: "座り(左)", 12: "座り(右)",
  13: "戦闘立ち(上)", 14: "戦闘立ち(下)", 15: "戦闘立ち(左)", 16: "戦闘立ち(右)",
  17: "すり足(上)", 18: "すり足(下)", 19: "すり足(左)", 20: "すり足(右)",
  21: "振り(上)", 22: "振り(下)", 23: "振り(左)", 24: "振り(右)",
  25: "突き(上)", 26: "突き(下)", 27: "突き(左)", 28: "突き(右)",
  29: "弓用すり足(上)", 30: "弓用すり足(下)", 31: "弓用すり足(左)", 32: "弓用すり足(右)",
  33: "弓(上)", 34: "弓(下)", 35: "弓(左)", 36: "弓(右)",
  37: "打撃用すり足(上)", 38: "打撃用すり足(下)", 39: "打撃用すり足(左)", 40: "打撃用すり足(右)",
  41: "打撃(上)", 42: "打撃(下)", 43: "打撃(左)", 44: "打撃(右)",
  45: "釣り用すり足(上)", 46: "釣り用すり足(下)", 47: "釣り用すり足(左)", 48: "釣り用すり足(右)",
  49: "釣り(上)", 50: "釣り(下)", 51: "釣り(左)", 52: "釣り(右)",
  53: "釣りヒット(上)", 54: "釣りヒット(下)", 55: "釣りヒット(左)", 56: "釣りヒット(右)",
  57: "弓用戦闘立ち(上)", 58: "弓用戦闘立ち(下)", 59: "弓用戦闘立ち(左)", 60: "弓用戦闘立ち(右)",
  61: "打撃用戦闘立ち(上)", 62: "打撃用戦闘立ち(下)", 63: "打撃用戦闘立ち(左)", 64: "打撃用戦闘立ち(右)",
  65: "釣り用戦闘立ち(上)", 66: "釣り用戦闘立ち(下)", 67: "釣り用戦闘立ち(左)", 68: "釣り用戦闘立ち(右)",
  69: "立ち", 70: "立ち(上下左右1コマずつ)", 71: "歩き(方向無し)", 72: "破壊", 73: "16コマ連続",
  74: "防御(上)", 75: "防御(下)", 76: "防御(左)", 77: "防御(右)",
  78: "防御成功(上)", 79: "防御成功(下)", 80: "防御成功(左)", 81: "防御成功(右)",
  82: "弓用防御(上)", 83: "弓用防御(下)", 84: "弓用防御(左)", 85: "弓用防御(右)",
  86: "ダメージ(上)", 87: "ダメージ(下)", 88: "ダメージ(左)", 89: "ダメージ(右)",
  90: "死亡(上)", 91: "死亡(下)", 92: "死亡(左)", 93: "死亡(右)",
};

async function fetchMotionTypes() {
  const { response, data } = await fetchJson("/api/motion-types");
  if (!response.ok || !Array.isArray(data?.motionTypes)) {
    throw new Error("モーション種別の取得に失敗しました");
  }
  return data.motionTypes.slice().sort((a, b) => a.motionTypeId - b.motionTypeId);
}

async function fetchMotions(motionTypeId) {
  const url = motionTypeId ? "/api/motions?motionTypeId=" + motionTypeId : "/api/motions";
  const { response, data } = await fetchJson(url);
  if (!response.ok || !Array.isArray(data?.motions)) {
    throw new Error("モーションの取得に失敗しました");
  }
  return data.motions;
}

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "me-feedback result-message" + (type ? " " + type : "");
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

function mainIdToCategory(catalog, idMain, fallbackKey) {
  return catalog.find((c) => Number(c.idMain) === Number(idMain)) ?? catalog.find((c) => c.key === fallbackKey) ?? catalog[0];
}

function categoryKeyToMainId(catalog, key) {
  const cat = catalog.find((c) => c.key === key);
  return Number(cat?.idMain ?? 0);
}

function thumbCategoryKey(key) {
  return key === "none" ? "weapon" : key;
}

function storedSubToPickerSub(value) {
  return Math.max(0, (Number(value) || 0) - 1);
}

function pickerSubToStoredSub(value) {
  return (Number(value) || 0) + 1;
}

function cellsPerSheet(cat) {
  return Math.max(1, Number(cat?.countX ?? 1) * Number(cat?.countY ?? 1));
}

function usesMotionTypeSheet(cat) {
  return MOTION_TYPE_SHEET_CATEGORY_KEYS.has(cat?.key);
}

function motionTypeSheetIndex(motionType) {
  return Math.max(0, Number(motionType?.grpIdSub ?? 0) || 0);
}

function storedSubToContextSub(value, cat, motionType) {
  const cell = storedSubToPickerSub(value);
  if (!usesMotionTypeSheet(cat)) return cell;
  return motionTypeSheetIndex(motionType) * cellsPerSheet(cat) + (cell % cellsPerSheet(cat));
}

function contextSubToStoredSub(sub, cat) {
  if (!usesMotionTypeSheet(cat)) return pickerSubToStoredSub(sub);
  return (Math.max(0, Number(sub) || 0) % cellsPerSheet(cat)) + 1;
}

function frameToPreviewLayers(frame, catalog, motionType, drawListOverride) {
  const drawList = Array.isArray(drawListOverride) ? drawListOverride
    : (Array.isArray(frame.drawList) && frame.drawList.length ? frame.drawList : [0, 1, 2, 3]);

  return drawList.map((idx) => {
    const layer = LAYERS[idx];
    if (!layer) return null;
    const sub = Number(frame[layer.subField] ?? 0);
    if (sub <= 0) return null;
    const cat = mainIdToCategory(catalog, frame[layer.mainField], idx === 0 ? "char" : "weapon");
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

function hasMotionContent(frame) {
  return Boolean(
    Number(frame.wait ?? 0) ||
    Number(frame.grpIdMainBase ?? 0) ||
    Number(frame.grpIdSubBase ?? 0) ||
    Number(frame.grpIdMainPile1 ?? 0) ||
    Number(frame.grpIdSubPile1 ?? 0) ||
    Number(frame.grpIdMainPile2 ?? 0) ||
    Number(frame.grpIdSubPile2 ?? 0) ||
    Number(frame.grpIdMainPile3 ?? 0) ||
    Number(frame.grpIdSubPile3 ?? 0) ||
    Number(frame.soundId ?? 0) ||
    Number(frame.procId ?? 0)
  );
}

function hasMotionListContent(frames) {
  return frames.some((frame) => hasMotionContent(frame));
}

function formatMotionList(listId) {
  return (MOTION_LIST_NAMES[listId] ? MOTION_LIST_NAMES[listId] + " " : "") + "#" + listId;
}

function makeBlankFrame(typeId, listId) {
  return {
    motionTypeId: typeId,
    motionListId: listId || 1,
    wait: 10,
    level1: 255,
    level2: 255,
    level3: 255,
    pile: false,
    redrawHand: false,
    loop: true,
    grpIdMainBase: categoryKeyToMainId(_catalogRef, "char"),
    grpIdMainPile1: 0,
    grpIdMainPile2: 0,
    grpIdMainPile3: 0,
    grpIdSubBase: 1,
    grpIdSubPile1: 0,
    grpIdSubPile2: 0,
    grpIdSubPile3: 0,
    soundId: 0,
    procId: 0,
    drawPosPile0x: 0,
    drawPosPile0y: 0,
    drawPosPile1x: 0,
    drawPosPile1y: 0,
    drawPosPile2x: 0,
    drawPosPile2y: 0,
    drawPosPile3x: 0,
    drawPosPile3y: 0,
    drawList: [0, 1, 2, 3],
  };
}

let _catalogRef = [];

function buildMotionForm({ catalog, preview, motionType, frame, allFrames, onChange, onCollectFrame }) {
  const wrap = document.createElement("div");
  wrap.className = "me-form-wrap";

  const topSection = document.createElement("section");
  topSection.className = "detail-section me-editor-top";
  const topTitle = document.createElement("h3");
  topTitle.textContent = "フレーム設定";
  topSection.appendChild(topTitle);

  const basicGrid = document.createElement("div");
  basicGrid.className = "form-grid compact";

  const listIdLabel = makeFormField("モーションリストID");
  const listIdSpin = createNumberSpinner({ value: frame.motionListId || 1, min: 1, max: 9999, step: 1, onChange });
  listIdLabel.appendChild(listIdSpin.el);
  basicGrid.appendChild(listIdLabel);

  const waitLabel = makeFormField("表示時間 wait (x10ms)");
  const waitSpin = createNumberSpinner({ value: frame.wait ?? 10, min: 0, max: 255, step: 1, onChange });
  waitLabel.appendChild(waitSpin.el);
  basicGrid.appendChild(waitLabel);

  const procLabel = makeFormField("行動ID");
  const procSpin = createNumberSpinner({ value: frame.procId ?? 0, min: 0, max: 99999, step: 1, onChange });
  procLabel.appendChild(procSpin.el);
  basicGrid.appendChild(procLabel);

  const soundLabel = makeFormField("効果音");
  const soundPicker = createSoundPicker({ value: frame.soundId ?? 0 });
  soundLabel.appendChild(soundPicker.el);
  basicGrid.appendChild(soundLabel);

  const flagRow = document.createElement("div");
  flagRow.className = "me-flag-row";
  const loopCb = makeCheck("ループ再生", !!frame.loop);
  const pileCb = makeCheck("重ね画像を先に描画", !!frame.pile);
  const redrawCb = makeCheck("手を手前に描画", !!frame.redrawHand);
  flagRow.append(loopCb.el, pileCb.el, redrawCb.el);

  topSection.append(basicGrid, flagRow);
  wrap.appendChild(topSection);

  const layerSection = document.createElement("section");
  layerSection.className = "detail-section me-layer-editor";
  const layerTitle = document.createElement("h3");
  layerTitle.textContent = "レイヤー";
  layerSection.appendChild(layerTitle);

  const layerFields = LAYERS.map((layer, index) => {
    const block = document.createElement("div");
    block.className = "me-layer-block";

    const h4 = document.createElement("h4");
    h4.className = "me-layer-title";
    h4.textContent = layer.label;
    block.appendChild(h4);

    const grid = document.createElement("div");
    grid.className = "form-grid compact";

    const allowedKeys = index === 0 ? BASE_CATEGORY_KEYS : PILE_CATEGORY_KEYS;
    const categories = catalog.filter((c) => allowedKeys.has(c.key) && (c.key === "none" || c.sheetUrl));
    const mainCat = mainIdToCategory(catalog, frame[layer.mainField], index === 0 ? "char" : "none");
    const mainLabel = makeFormField("カテゴリ");
    const mainSelect = document.createElement("select");
    categories.forEach((cat) => {
      const opt = document.createElement("option");
      opt.value = cat.key;
      opt.textContent = cat.label || cat.key;
      if (cat.key === mainCat.key) opt.selected = true;
      mainSelect.appendChild(opt);
    });
    mainLabel.appendChild(mainSelect);
    grid.appendChild(mainLabel);

    const subCategory = () => catalog.find((c) => c.key === thumbCategoryKey(mainSelect.value)) ?? mainCat;
    const sfSub = createSpriteField({
      categoryKey: thumbCategoryKey(mainCat.key),
      value: frame[layer.subField] ?? 0,
      label: "Sub",
      allowCategorySwitch: false,
      valueToSub: (value) => storedSubToContextSub(value, subCategory(), motionType),
      subToValue: (sub) => contextSubToStoredSub(sub, subCategory()),
      onChange,
    });
    grid.appendChild(sfSub.el);

    const xLabel = makeFormField("X");
    const xSpin = createNumberSpinner({ value: frame[layer.dpx] ?? 0, min: -128, max: 128, step: 1, onChange });
    xLabel.appendChild(xSpin.el);
    grid.appendChild(xLabel);

    const yLabel = makeFormField("Y");
    const ySpin = createNumberSpinner({ value: frame[layer.dpy] ?? 0, min: -128, max: 128, step: 1, onChange });
    yLabel.appendChild(ySpin.el);
    grid.appendChild(yLabel);

    let levelSpin = null;
    if (layer.levelField) {
      const levelLabel = makeFormField("透明度");
      levelSpin = createNumberSpinner({ value: frame[layer.levelField] ?? 255, min: 0, max: 255, step: 1, onChange });
      levelLabel.appendChild(levelSpin.el);
      grid.appendChild(levelLabel);
    }

    mainSelect.addEventListener("change", () => {
      const nextCat = subCategory();
      sfSub.setThumb({
        categoryKey: thumbCategoryKey(mainSelect.value),
        sub: storedSubToContextSub(sfSub.getValue(), nextCat, motionType),
      });
      onChange();
    });

    block.appendChild(grid);
    layerSection.appendChild(block);
    return { layer, mainSelect, sfSub, xSpin, ySpin, levelSpin };
  });

  wrap.appendChild(layerSection);

  const drawSection = document.createElement("section");
  drawSection.className = "detail-section";
  const drawTitle = document.createElement("h3");
  drawTitle.textContent = "描画順";
  drawSection.appendChild(drawTitle);

  let drawItems = Array.isArray(frame.drawList) && frame.drawList.length ? frame.drawList.slice() : [0, 1, 2, 3];
  let dragList = null;
  function rebuildDrawList() {
    if (dragList) dragList.el.remove();
    dragList = createDragList({
      items: drawItems,
      renderItem: (idx) => {
        const span = document.createElement("span");
        span.textContent = (LAYERS[idx]?.label ?? "?") + " (" + idx + ")";
        return span;
      },
      onReorder: (items) => {
        drawItems = items;
        onChange();
      },
    });
    drawSection.appendChild(dragList.el);
  }
  rebuildDrawList();
  wrap.appendChild(drawSection);

  function makeCheck(label, checked) {
    const lbl = document.createElement("label");
    lbl.className = "me-check";
    const cb = document.createElement("input");
    cb.type = "checkbox";
    cb.checked = checked;
    cb.addEventListener("change", onChange);
    lbl.append(cb, label);
    return { el: lbl, cb };
  }

  function collectData() {
    const payload = {
      motionTypeId: frame.motionTypeId,
      motionListId: listIdSpin.getValue(),
      wait: waitSpin.getValue(),
      soundId: soundPicker.getValue(),
      procId: procSpin.getValue(),
      loop: loopCb.cb.checked,
      pile: pileCb.cb.checked,
      redrawHand: redrawCb.cb.checked,
      drawList: drawItems.slice(),
    };
    layerFields.forEach(({ layer, mainSelect, sfSub, xSpin, ySpin, levelSpin }) => {
      payload[layer.mainField] = categoryKeyToMainId(catalog, mainSelect.value);
      payload[layer.subField] = sfSub.getValue();
      payload[layer.dpx] = xSpin.getValue();
      payload[layer.dpy] = ySpin.getValue();
      if (layer.levelField) payload[layer.levelField] = levelSpin.getValue();
    });
    return payload;
  }

  function updatePreview() {
    const current = { ...frame, ...collectData() };
    const frames = allFrames.map((f) => f.motionId === frame.motionId ? current : f);
    if (!frames.some((f) => f.motionId === frame.motionId)) frames.push(current);
    preview.setFrames(frames.map((f) => ({
      wait: Number(f.wait ?? 10),
      layers: frameToPreviewLayers(f, catalog, motionType, f.motionId === frame.motionId ? drawItems : undefined),
    })));
    onCollectFrame(current);
  }

  soundPicker.el.addEventListener("change", onChange);
  return { el: wrap, collectData, updatePreview };
}

function buildFrameStrip({ getFrames, getSelectedId, onSelect, onAdd, onDelete }) {
  const wrap = document.createElement("section");
  wrap.className = "detail-section me-frame-strip-section";
  const head = document.createElement("div");
  head.className = "me-section-head";
  const title = document.createElement("h3");
  title.textContent = "フレーム";
  const addBtn = document.createElement("button");
  addBtn.type = "button";
  addBtn.className = "button small";
  addBtn.textContent = "+ フレーム追加";
  addBtn.addEventListener("click", onAdd);
  head.append(title, addBtn);
  wrap.appendChild(head);

  const list = document.createElement("div");
  list.className = "me-frame-strip";
  wrap.appendChild(list);

  function render() {
    list.innerHTML = "";
    getFrames().forEach((frame, index) => {
      const btn = document.createElement("button");
      btn.type = "button";
      btn.className = "me-frame-chip" + (frame.motionId === getSelectedId() ? " is-selected" : "");
      btn.textContent = (index + 1) + " / wait " + (frame.wait ?? 0);
      btn.addEventListener("click", () => onSelect(frame));
      list.appendChild(btn);
    });
    const delBtn = document.createElement("button");
    delBtn.type = "button";
    delBtn.className = "button small danger";
    delBtn.textContent = "選択フレーム削除";
    delBtn.disabled = !getSelectedId();
    delBtn.addEventListener("click", onDelete);
    list.appendChild(delBtn);
  }

  return { el: wrap, render };
}

function buildLeftPane({ catalog, onSelectList, onNewList, onDeleteList }) {
  const pane = document.createElement("div");
  pane.className = "me-left";

  const filter = document.createElement("div");
  filter.className = "me-filter-row";
  const typeField = makeFormField("モーション種別");
  const typeSelect = document.createElement("select");
  typeField.appendChild(typeSelect);
  filter.appendChild(typeField);
  pane.appendChild(filter);

  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "リストID・名前で検索";
  searchInput.className = "ld-search";
  pane.appendChild(searchInput);

  const toolbar = document.createElement("div");
  toolbar.className = "me-list-toolbar";
  const newBtn = document.createElement("button");
  newBtn.type = "button";
  newBtn.className = "button small";
  newBtn.textContent = "+ 新規リスト";
  newBtn.addEventListener("click", () => onNewList(getCurrentTypeId()));
  toolbar.appendChild(newBtn);

  const showEmptyLabel = document.createElement("label");
  showEmptyLabel.className = "me-check";
  const showEmptyCb = document.createElement("input");
  showEmptyCb.type = "checkbox";
  showEmptyLabel.append(showEmptyCb, "未設定リストも表示");
  toolbar.appendChild(showEmptyLabel);
  pane.appendChild(toolbar);

  const listEl = document.createElement("div");
  listEl.className = "me-motion-groups";
  pane.appendChild(listEl);

  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let types = [];
  let motions = [];
  let selectedKey = "";

  async function loadTypes() {
    types = await fetchMotionTypes();
    typeSelect.innerHTML = "";
    types.forEach((mt) => {
      const opt = document.createElement("option");
      opt.value = String(mt.motionTypeId);
      opt.textContent = "[" + mt.motionTypeId + "] " + (mt.name || "(名前なし)");
      typeSelect.appendChild(opt);
    });
  }

  async function loadMotionList() {
    const typeId = getCurrentTypeId();
    motions = typeId ? await fetchMotions(typeId) : [];
    renderList();
  }

  function getCurrentTypeId() {
    return typeSelect.value ? parseInt(typeSelect.value, 10) : 0;
  }

  function getCurrentType() {
    const typeId = getCurrentTypeId();
    return types.find((mt) => mt.motionTypeId === typeId) ?? null;
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const type = getCurrentType();
    const groups = new Map();
    motions.forEach((m) => {
      const listId = Number(m.motionListId || 0);
      const label = formatMotionList(listId);
      if (q && !String(listId).includes(q) && !label.toLowerCase().includes(q)) return;
      if (!groups.has(listId)) groups.set(listId, []);
      groups.get(listId).push(m);
    });

    listEl.innerHTML = "";
    if (!type) {
      listEl.textContent = "モーション種別がありません。";
      summary.textContent = "";
      return;
    }

    const typeCard = document.createElement("div");
    typeCard.className = "me-motion-type-card";
    const title = document.createElement("h3");
    title.textContent = "[" + type.motionTypeId + "] " + (type.name || "(名前なし)");
    const meta = document.createElement("p");
    meta.className = "muted";
    meta.textContent = "GrpSub " + (type.grpIdSub ?? 0) + " / " + groups.size + " リスト";
    typeCard.append(title, meta);

    let visibleCount = 0;
    Array.from(groups.entries()).sort((a, b) => a[0] - b[0]).forEach(([listId, frames]) => {
      const hasContent = hasMotionListContent(frames);
      if (!showEmptyCb.checked && !hasContent) return;
      visibleCount += 1;
      const key = type.motionTypeId + ":" + listId;
      const row = document.createElement("button");
      row.type = "button";
      row.className = "me-motion-list-row"
        + (key === selectedKey ? " is-selected" : "")
        + (!hasContent ? " is-empty" : "");
      const name = document.createElement("span");
      name.textContent = formatMotionList(listId);
      const count = document.createElement("small");
      count.textContent = frames.length + " フレーム" + (hasContent ? "" : " / 未設定");
      row.append(name, count);
      row.addEventListener("click", () => {
        selectedKey = key;
        renderList();
        onSelectList({ type, listId, frames: frames.slice().sort((a, b) => a.motionId - b.motionId) });
      });
      typeCard.appendChild(row);
    });

    const delBtn = document.createElement("button");
    delBtn.type = "button";
    delBtn.className = "button small danger me-delete-list-btn";
    delBtn.textContent = "選択リストを削除";
    delBtn.disabled = !selectedKey;
    delBtn.addEventListener("click", () => onDeleteList());
    typeCard.appendChild(delBtn);

    listEl.appendChild(typeCard);
    summary.textContent = visibleCount + " / " + groups.size + " リスト表示、" + motions.length + " フレーム";
  }

  typeSelect.addEventListener("change", () => {
    selectedKey = "";
    loadMotionList();
  });
  searchInput.addEventListener("input", renderList);
  showEmptyCb.addEventListener("change", renderList);

  return {
    el: pane,
    init: async () => {
      await loadTypes();
      await loadMotionList();
    },
    reload: loadMotionList,
    getCurrentTypeId,
    getCurrentType,
    getSelectedFrames: () => {
      if (!selectedKey) return [];
      const listId = Number(selectedKey.split(":")[1]);
      return motions.filter((m) => Number(m.motionListId) === listId);
    },
    clearSelection: () => { selectedKey = ""; renderList(); },
  };
}

let _destroyFn = null;

export async function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";
  const mountToken = {};
  _destroyFn = () => {
    mountToken.destroyed = true;
    container.innerHTML = "";
  };

  const loading = document.createElement("p");
  loading.className = "muted";
  loading.textContent = "モーション編集画面を読み込んでいます...";
  container.appendChild(loading);

  const catalog = await loadCatalog();
  if (mountToken.destroyed) return;
  _catalogRef = catalog;

  container.innerHTML = "";
  const shell = document.createElement("div");
  shell.className = "me-shell motion-editor-shell";
  container.appendChild(shell);

  const preview = createAnimePreview({ width: 64, height: 64, scale: 2 });
  const feedbackEl = document.createElement("p");
  feedbackEl.className = "me-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  const rightPane = document.createElement("div");
  rightPane.className = "me-right motion-editor-right";

  const backBar = document.createElement("div");
  backBar.className = "me-action-bar";
  const backBtn = document.createElement("button");
  backBtn.type = "button";
  backBtn.className = "button small";
  backBtn.textContent = "← 一覧へ";
  backBar.appendChild(backBtn);
  rightPane.appendChild(backBar);

  const previewSection = document.createElement("section");
  previewSection.className = "detail-section me-preview-section";
  const previewTitle = document.createElement("h3");
  previewTitle.textContent = "合成プレビュー";
  previewSection.append(previewTitle, preview.el);
  rightPane.append(previewSection, feedbackEl);

  const actionBar = document.createElement("div");
  actionBar.className = "me-action-bar";
  const saveBtn = document.createElement("button");
  saveBtn.type = "button";
  saveBtn.className = "button primary";
  saveBtn.textContent = "選択フレームを保存";
  const duplicateBtn = document.createElement("button");
  duplicateBtn.type = "button";
  duplicateBtn.className = "button";
  duplicateBtn.textContent = "複製して追加";
  actionBar.append(saveBtn, duplicateBtn);
  rightPane.appendChild(actionBar);

  let currentType = null;
  let currentListId = 0;
  let frames = [];
  let selectedFrame = null;
  let formApi = null;
  let latestDraft = null;

  const frameStrip = buildFrameStrip({
    getFrames: () => frames,
    getSelectedId: () => selectedFrame?.motionId ?? 0,
    onSelect: (frame) => selectFrame(frame),
    onAdd: () => addFrame(false),
    onDelete: () => deleteSelectedFrame(),
  });
  rightPane.appendChild(frameStrip.el);

  function renderPreview() {
    preview.setFrames(frames.map((f) => ({
      wait: Number(f.wait ?? 10),
      layers: frameToPreviewLayers(f, catalog, currentType),
    })));
  }

  function renderForm() {
    if (formApi?.el?.parentNode) formApi.el.remove();
    if (!selectedFrame) {
      formApi = null;
      return;
    }
    latestDraft = { ...selectedFrame };
    formApi = buildMotionForm({
      catalog,
      preview,
      motionType: currentType,
      frame: selectedFrame,
      allFrames: frames,
      onChange: () => {
        if (!formApi) return;
        formApi.updatePreview();
      },
      onCollectFrame: (draft) => { latestDraft = draft; },
    });
    rightPane.appendChild(formApi.el);
    formApi.updatePreview();
  }

  function selectFrame(frame) {
    selectedFrame = frame;
    frameStrip.render();
    renderForm();
    showDetail();
  }

  function selectList({ type, listId, frames: nextFrames }) {
    currentType = type;
    currentListId = listId;
    frames = nextFrames;
    selectedFrame = frames[0] ?? makeBlankFrame(type.motionTypeId, listId);
    frameStrip.render();
    renderForm();
    renderPreview();
    showFeedback(feedbackEl, "", "");
    showDetail();
  }

  async function saveSelectedFrame() {
    if (!formApi || !selectedFrame) return;
    const payload = formApi.collectData();
    const isNew = !selectedFrame.motionId;
    if (!isNew) payload.motionId = selectedFrame.motionId;
    const { response, data } = await fetchJson("/api/motions", {
      method: isNew ? "POST" : "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    if (!response.ok) {
      showFeedback(feedbackEl, "保存に失敗しました: " + (data?.error ?? response.status), "error");
      return;
    }
    showFeedback(feedbackEl, "保存しました", "success");
    selectedFrame = { ...payload, motionId: data.motionId ?? selectedFrame.motionId };
    frames = frames.map((frame) => frame.motionId === selectedFrame.motionId ? selectedFrame : frame);
    await leftApi.reload();
    frameStrip.render();
    renderForm();
  }

  async function addFrame(copySelected) {
    if (!currentType) {
      const type = leftApi.getCurrentType();
      if (!type) return;
      currentType = type;
      currentListId = 1;
    }
    const source = copySelected ? (latestDraft || selectedFrame) : null;
    const payload = {
      ...(source ? { ...source } : makeBlankFrame(currentType.motionTypeId, currentListId)),
      motionTypeId: currentType.motionTypeId,
      motionListId: currentListId || 1,
    };
    delete payload.motionId;
    const { response, data } = await fetchJson("/api/motions", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    if (!response.ok) {
      showFeedback(feedbackEl, "追加に失敗しました: " + (data?.error ?? response.status), "error");
      return;
    }
    const created = { ...payload, motionId: data.motionId };
    frames.push(created);
    selectedFrame = created;
    frameStrip.render();
    renderForm();
    await leftApi.reload();
    showFeedback(feedbackEl, "フレームを追加しました", "success");
  }

  async function deleteSelectedFrame() {
    if (!selectedFrame?.motionId) return;
    if (!confirm("選択フレーム (ID=" + selectedFrame.motionId + ") を削除しますか？")) return;
    const { response, data } = await fetchJson("/api/motions", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ motionId: selectedFrame.motionId }),
    });
    if (!response.ok) {
      showFeedback(feedbackEl, "削除に失敗しました: " + (data?.error ?? response.status), "error");
      return;
    }
    frames = frames.filter((f) => f.motionId !== selectedFrame.motionId);
    selectedFrame = frames[0] ?? null;
    frameStrip.render();
    renderForm();
    renderPreview();
    await leftApi.reload();
    showFeedback(feedbackEl, "削除しました", "success");
  }

  const leftApi = buildLeftPane({
    catalog,
    onSelectList: selectList,
    onNewList: (typeId) => {
      if (!typeId) return;
      currentType = leftApi.getCurrentType();
      currentListId = 1;
      frames = [];
      selectedFrame = makeBlankFrame(typeId, currentListId);
      frameStrip.render();
      renderForm();
      showDetail();
    },
    onDeleteList: async () => {
      const selected = leftApi.getSelectedFrames();
      if (!selected.length) return;
      if (!confirm("選択中のモーションリスト " + selected.length + " フレームを削除しますか？")) return;
      for (const frame of selected) {
        await fetchJson("/api/motions", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ motionId: frame.motionId }),
        });
      }
      frames = [];
      selectedFrame = null;
      leftApi.clearSelection();
      await leftApi.reload();
      showList();
    },
  });

  saveBtn.addEventListener("click", saveSelectedFrame);
  duplicateBtn.addEventListener("click", () => addFrame(true));
  backBtn.addEventListener("click", showList);

  function showDetail() {
    leftApi.el.style.display = "none";
    rightPane.style.display = "";
  }

  function showList() {
    rightPane.style.display = "none";
    leftApi.el.style.display = "";
  }

  rightPane.style.display = "none";
  shell.append(leftApi.el, rightPane);

  try {
    await leftApi.init();
  } catch (e) {
    showFeedback(feedbackEl, "初期化に失敗しました: " + e.message, "error");
  }

  _destroyFn = () => {
    mountToken.destroyed = true;
    preview.destroy();
    container.innerHTML = "";
  };
}

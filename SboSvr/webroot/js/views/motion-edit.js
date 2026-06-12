/**
 * views/motion-edit.js
 * モーション編集画面 (route: motion-edit)
 *
 * レイアウト:
 *   左: 種別フィルタ + 検索 + モーション一覧
 *   右上: 4層合成リアルタイムプレビュー (anime-preview, scale=2)
 *   右下: フォーム(4層スプライト + drawList + wait/sound/procId 等)
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";
import { createDragList } from "../components/drag-list.js";
import { createAnimePreview } from "../components/anime-preview.js";
import { createSoundPicker } from "../components/sound-picker.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

/** 4層定義 */
const LAYERS = [
  { key: "Base",  label: "下地  (Base)",  mainField: "grpIdMainBase",  subField: "grpIdSubBase",  dpx: "drawPosPile0x", dpy: "drawPosPile0y", levelField: "level1" },
  { key: "Pile1", label: "重ね1 (Pile1)", mainField: "grpIdMainPile1", subField: "grpIdSubPile1", dpx: "drawPosPile1x", dpy: "drawPosPile1y", levelField: "level2" },
  { key: "Pile2", label: "重ね2 (Pile2)", mainField: "grpIdMainPile2", subField: "grpIdSubPile2", dpx: "drawPosPile2x", dpy: "drawPosPile2y", levelField: "level3" },
  { key: "Pile3", label: "重ね3 (Pile3)", mainField: "grpIdMainPile3", subField: "grpIdSubPile3", dpx: "drawPosPile3x", dpy: "drawPosPile3y", levelField: null },
];

// ----------------------------------------------------------------
// API ヘルパ
// ----------------------------------------------------------------

async function fetchMotionTypes() {
  const { response, data } = await fetchJson("/api/motion-types");
  if (!response.ok || !Array.isArray(data?.motionTypes)) {
    throw new Error("モーション種別の取得に失敗しました");
  }
  return data.motionTypes.slice().sort((a, b) => a.motionTypeId - b.motionTypeId);
}

async function fetchMotions(motionTypeId) {
  const url = motionTypeId
    ? "/api/motions?motionTypeId=" + motionTypeId
    : "/api/motions";
  const { response, data } = await fetchJson(url);
  if (!response.ok || !Array.isArray(data?.motions)) {
    throw new Error("モーションの取得に失敗しました");
  }
  return data.motions;
}

// ----------------------------------------------------------------
// フィードバック
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "me-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

// ----------------------------------------------------------------
// 4層フォーム構築
// ----------------------------------------------------------------

/**
 * 4層スプライト入力 + drawList + 各種フィールドを含む右ペインフォームを生成。
 * @returns {{ el, collectData, setMotion, updatePreview }}
 */
function buildMotionForm(preview, typeId) {
  const wrap = document.createElement("div");
  wrap.className = "me-form-wrap";

  // --- 基本情報 ---
  const basicSection = document.createElement("section");
  basicSection.className = "detail-section";
  const basicH3 = document.createElement("h3");
  basicH3.textContent = "基本情報";
  basicSection.appendChild(basicH3);

  const basicGrid = document.createElement("div");
  basicGrid.className = "form-grid compact";

  // 種別ID (非表示)
  const hiddenTypeId = document.createElement("input");
  hiddenTypeId.type = "hidden";
  hiddenTypeId.name = "motionTypeId";
  hiddenTypeId.value = String(typeId || 0);
  basicGrid.appendChild(hiddenTypeId);

  // リストID
  const listIdLabel = makeFormField("モーションリストID");
  const listIdSpin = createNumberSpinner({ value: 1, min: 1, max: 9999, step: 1 });
  listIdLabel.appendChild(listIdSpin.el);
  basicGrid.appendChild(listIdLabel);

  // ループ
  const loopLabel = document.createElement("label");
  loopLabel.className = "form-field";
  const loopCb = document.createElement("input");
  loopCb.type = "checkbox";
  loopCb.name = "loop";
  loopLabel.append(loopCb, " ループ再生");
  basicGrid.appendChild(loopLabel);

  // pile
  const pileLabel = document.createElement("label");
  pileLabel.className = "form-field";
  const pileCb = document.createElement("input");
  pileCb.type = "checkbox";
  pileCb.name = "pile";
  pileLabel.append(pileCb, " 重ね画像を先に描画");
  basicGrid.appendChild(pileLabel);

  // redrawHand
  const redrawLabel = document.createElement("label");
  redrawLabel.className = "form-field";
  const redrawCb = document.createElement("input");
  redrawCb.type = "checkbox";
  redrawCb.name = "redrawHand";
  redrawLabel.append(redrawCb, " 手を手前に描画");
  basicGrid.appendChild(redrawLabel);

  // 行動ID
  const procLabel = makeFormField("行動ID (procId)");
  const procSpin = createNumberSpinner({ value: 0, min: 0, max: 99999, step: 1 });
  procLabel.appendChild(procSpin.el);
  basicGrid.appendChild(procLabel);

  basicSection.appendChild(basicGrid);
  wrap.appendChild(basicSection);

  // --- 4層スプライト ---
  const layerSection = document.createElement("section");
  layerSection.className = "detail-section";
  const layerH3 = document.createElement("h3");
  layerH3.textContent = "4層スプライト";
  layerSection.appendChild(layerH3);

  const layerFields = LAYERS.map((layer) => {
    const block = document.createElement("div");
    block.className = "me-layer-block";

    const h4 = document.createElement("h4");
    h4.textContent = layer.label;
    h4.className = "me-layer-title";
    block.appendChild(h4);

    const innerGrid = document.createElement("div");
    innerGrid.className = "form-grid compact";

    // メインスプライト
    const sfMain = createSpriteField({
      categoryKey: "character",
      value: 0,
      label: "メイン",
      allowCategorySwitch: true,
      onChange: () => schedulePreviewUpdate(),
    });
    innerGrid.appendChild(sfMain.el);

    // サブスプライト
    const sfSub = createSpriteField({
      categoryKey: "character",
      value: 0,
      label: "サブ",
      allowCategorySwitch: true,
      onChange: () => schedulePreviewUpdate(),
    });
    innerGrid.appendChild(sfSub.el);

    // 描画オフセット X
    const dpxLabel = makeFormField("オフセット X");
    const dpxSpin = createNumberSpinner({ value: 0, min: -999, max: 999, step: 1, onChange: schedulePreviewUpdate });
    dpxLabel.appendChild(dpxSpin.el);
    innerGrid.appendChild(dpxLabel);

    // 描画オフセット Y
    const dpyLabel = makeFormField("オフセット Y");
    const dpySpin = createNumberSpinner({ value: 0, min: -999, max: 999, step: 1, onChange: schedulePreviewUpdate });
    dpyLabel.appendChild(dpySpin.el);
    innerGrid.appendChild(dpyLabel);

    // 透明度 (Pile3 は level フィールドなし)
    let levelSpin = null;
    if (layer.levelField) {
      const lvLabel = makeFormField("透明度 (0-255)");
      levelSpin = createNumberSpinner({ value: 0, min: 0, max: 255, step: 1, onChange: schedulePreviewUpdate });
      lvLabel.appendChild(levelSpin.el);
      innerGrid.appendChild(lvLabel);
    }

    block.appendChild(innerGrid);
    layerSection.appendChild(block);

    return { layer, sfMain, sfSub, dpxSpin, dpySpin, levelSpin };
  });

  wrap.appendChild(layerSection);

  // --- 描画順 (drawList) ---
  const drawListSection = document.createElement("section");
  drawListSection.className = "detail-section";
  const drawListH3 = document.createElement("h3");
  drawListH3.textContent = "描画順";
  drawListSection.appendChild(drawListH3);

  let _drawItems = [0, 1, 2, 3]; // default: Base,Pile1,Pile2,Pile3

  const drawListNote = document.createElement("p");
  drawListNote.className = "muted";
  drawListNote.textContent = "数値は層インデックス (0=Base, 1=Pile1, 2=Pile2, 3=Pile3)。上から順に描画。";
  drawListSection.appendChild(drawListNote);

  let _dragList = null;

  function buildDrawDragList() {
    if (_dragList) { _dragList.el.remove(); _dragList = null; }
    _dragList = createDragList({
      items: _drawItems,
      renderItem: (idx) => {
        const span = document.createElement("span");
        span.textContent = idx + " — " + (LAYERS[idx]?.label ?? "?");
        return span;
      },
      onReorder: (newItems) => { _drawItems = newItems; schedulePreviewUpdate(); },
    });
    drawListSection.insertBefore(_dragList.el, drawListSection.querySelector(".me-drawlist-add") ?? null);
  }

  buildDrawDragList();
  wrap.appendChild(drawListSection);

  // --- wait / 効果音 ---
  const timeSection = document.createElement("section");
  timeSection.className = "detail-section";
  const timeH3 = document.createElement("h3");
  timeH3.textContent = "タイミング / 効果音";
  timeSection.appendChild(timeH3);

  const timeGrid = document.createElement("div");
  timeGrid.className = "form-grid compact";

  const waitLabel = makeFormField("表示時間 wait (×10ms)");
  const waitSpin = createNumberSpinner({ value: 0, min: 0, max: 255, step: 1 });
  waitLabel.appendChild(waitSpin.el);
  timeGrid.appendChild(waitLabel);

  const soundLabel = makeFormField("効果音");
  const soundPicker = createSoundPicker({ value: 0 });
  soundLabel.appendChild(soundPicker.el);
  timeGrid.appendChild(soundLabel);

  timeSection.appendChild(timeGrid);
  wrap.appendChild(timeSection);

  // ----------------------------------------------------------------
  // プレビュー更新
  // ----------------------------------------------------------------

  let _previewTimer = null;
  function schedulePreviewUpdate() {
    if (_previewTimer) { clearTimeout(_previewTimer); }
    _previewTimer = setTimeout(doPreviewUpdate, 80);
  }

  function doPreviewUpdate() {
    if (!preview) return;
    const layers = layerFields.map(({ layer, sfMain, sfSub, dpxSpin, dpySpin, levelSpin }) => ({
      categoryKey: "character",
      sub: sfMain.getValue(),
      offsetX: dpxSpin.getValue(),
      offsetY: dpySpin.getValue(),
      alpha: levelSpin ? (levelSpin.getValue() / 255) : 1,
    }));
    preview.setFrames([{
      wait: (waitSpin.getValue() || 10) * 10,
      layers,
    }]);
  }

  // ----------------------------------------------------------------
  // setMotion: モーションデータをフォームに反映
  // ----------------------------------------------------------------

  function setMotion(m) {
    if (!m) return;
    hiddenTypeId.value = String(m.motionTypeId || typeId || 0);
    listIdSpin.setValue(m.motionListId ?? 1);
    loopCb.checked = !!m.loop;
    pileCb.checked = !!m.pile;
    redrawCb.checked = !!m.redrawHand;
    procSpin.setValue(m.procId ?? 0);
    waitSpin.setValue(m.wait ?? 0);
    soundPicker.setValue(m.soundId ?? 0);

    // 4層
    const layerData = [
      { mainVal: m.grpIdMainBase  ?? 0, subVal: m.grpIdSubBase  ?? 0, dpx: m.drawPosPile0x ?? 0, dpy: m.drawPosPile0y ?? 0, lv: m.level1 ?? 0 },
      { mainVal: m.grpIdMainPile1 ?? 0, subVal: m.grpIdSubPile1 ?? 0, dpx: m.drawPosPile1x ?? 0, dpy: m.drawPosPile1y ?? 0, lv: m.level2 ?? 0 },
      { mainVal: m.grpIdMainPile2 ?? 0, subVal: m.grpIdSubPile2 ?? 0, dpx: m.drawPosPile2x ?? 0, dpy: m.drawPosPile2y ?? 0, lv: m.level3 ?? 0 },
      { mainVal: m.grpIdMainPile3 ?? 0, subVal: m.grpIdSubPile3 ?? 0, dpx: m.drawPosPile3x ?? 0, dpy: m.drawPosPile3y ?? 0, lv: 0 },
    ];
    layerFields.forEach(({ sfMain, sfSub, dpxSpin, dpySpin, levelSpin }, i) => {
      sfMain.setValue(layerData[i].mainVal);
      sfSub.setValue(layerData[i].subVal);
      dpxSpin.setValue(layerData[i].dpx);
      dpySpin.setValue(layerData[i].dpy);
      if (levelSpin) levelSpin.setValue(layerData[i].lv);
    });

    // 描画順
    if (Array.isArray(m.drawList) && m.drawList.length > 0) {
      _drawItems = m.drawList.slice();
    } else {
      _drawItems = [0, 1, 2, 3];
    }
    buildDrawDragList();

    doPreviewUpdate();
  }

  // ----------------------------------------------------------------
  // collectData
  // ----------------------------------------------------------------

  function collectData() {
    const payload = {
      motionTypeId:  parseInt(hiddenTypeId.value, 10) || 0,
      motionListId:  listIdSpin.getValue(),
      loop:          loopCb.checked,
      pile:          pileCb.checked,
      redrawHand:    redrawCb.checked,
      procId:        procSpin.getValue(),
      wait:          waitSpin.getValue(),
      soundId:       soundPicker.getValue(),
      grpIdMainBase:  layerFields[0].sfMain.getValue(),
      grpIdMainPile1: layerFields[1].sfMain.getValue(),
      grpIdMainPile2: layerFields[2].sfMain.getValue(),
      grpIdMainPile3: layerFields[3].sfMain.getValue(),
      grpIdSubBase:   layerFields[0].sfSub.getValue(),
      grpIdSubPile1:  layerFields[1].sfSub.getValue(),
      grpIdSubPile2:  layerFields[2].sfSub.getValue(),
      grpIdSubPile3:  layerFields[3].sfSub.getValue(),
      drawPosPile0x: layerFields[0].dpxSpin.getValue(),
      drawPosPile0y: layerFields[0].dpySpin.getValue(),
      drawPosPile1x: layerFields[1].dpxSpin.getValue(),
      drawPosPile1y: layerFields[1].dpySpin.getValue(),
      drawPosPile2x: layerFields[2].dpxSpin.getValue(),
      drawPosPile2y: layerFields[2].dpySpin.getValue(),
      drawPosPile3x: layerFields[3].dpxSpin.getValue(),
      drawPosPile3y: layerFields[3].dpySpin.getValue(),
      level1: layerFields[0].levelSpin?.getValue() ?? 0,
      level2: layerFields[1].levelSpin?.getValue() ?? 0,
      level3: layerFields[2].levelSpin?.getValue() ?? 0,
      drawList: _drawItems.slice(),
    };
    return payload;
  }

  return { el: wrap, collectData, setMotion, updatePreview: doPreviewUpdate };
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
// 左ペイン: 種別フィルタ + 検索 + リスト
// ----------------------------------------------------------------

function buildLeftPane({ onSelect, onNew, onDelete }) {
  const pane = document.createElement("div");
  pane.className = "me-left";

  // 種別フィルタ
  const filterRow = document.createElement("div");
  filterRow.className = "me-filter-row";

  const filterLabel = document.createElement("label");
  filterLabel.className = "form-field";
  const filterSpan = document.createElement("span");
  filterSpan.textContent = "種別";
  const typeSelect = document.createElement("select");
  const allOpt = document.createElement("option");
  allOpt.value = "";
  allOpt.textContent = "— すべて —";
  typeSelect.appendChild(allOpt);
  filterLabel.append(filterSpan, typeSelect);
  filterRow.appendChild(filterLabel);
  pane.appendChild(filterRow);

  // 検索
  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "ID または リストID で検索…";
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
  listEl.className = "ld-list me-motion-list";
  pane.appendChild(listEl);

  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let _allTypes = [];
  let _allMotions = [];
  let _selectedId = null;

  async function loadTypes() {
    try {
      _allTypes = await fetchMotionTypes();
      _allTypes.forEach((mt) => {
        const opt = document.createElement("option");
        opt.value = String(mt.motionTypeId);
        opt.textContent = "[" + mt.motionTypeId + "] " + (mt.name || "(名前なし)");
        typeSelect.appendChild(opt);
      });
    } catch (e) {
      console.error("motion types load error", e);
    }
  }

  async function loadMotionList() {
    const tid = typeSelect.value ? parseInt(typeSelect.value, 10) : 0;
    try {
      _allMotions = await fetchMotions(tid || 0);
      renderList();
    } catch (e) {
      console.error("motions load error", e);
    }
  }

  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allMotions.filter((m) => {
      if (!q) return true;
      return String(m.motionId).includes(q)
        || String(m.motionListId).includes(q);
    });
    listEl.innerHTML = "";
    filtered.forEach((m) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (m.motionId === _selectedId ? " selected" : "");
      li.textContent = "[" + m.motionId + "] list=" + m.motionListId + " wait=" + m.wait;
      li.addEventListener("click", () => {
        _selectedId = m.motionId;
        renderList();
        onSelect(m);
      });

      // 削除ボタン
      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.title = "モーションを削除";
      delBtn.addEventListener("click", (ev) => {
        ev.stopPropagation();
        onDelete(m);
      });
      li.appendChild(delBtn);
      listEl.appendChild(li);
    });
    summary.textContent = filtered.length + " 件";
  }

  typeSelect.addEventListener("change", loadMotionList);
  searchInput.addEventListener("input", renderList);

  // 初期ロード
  loadTypes().then(loadMotionList);

  return {
    el: pane,
    reload: loadMotionList,
    getCurrentTypeId: () => typeSelect.value ? parseInt(typeSelect.value, 10) : 0,
  };
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

  // --- 右ペイン ---
  const rightPane = document.createElement("div");
  rightPane.className = "me-right";

  // プレビュー領域
  const previewSection = document.createElement("section");
  previewSection.className = "detail-section me-preview-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "4層合成プレビュー (2倍)";
  previewSection.appendChild(previewH3);
  const preview = createAnimePreview({ width: 64, height: 64, scale: 2 });
  previewSection.appendChild(preview.el);
  rightPane.appendChild(previewSection);

  // フィードバック
  const feedbackEl = document.createElement("p");
  feedbackEl.className = "me-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";
  rightPane.appendChild(feedbackEl);

  // 保存・キャンセルバー (右上)
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
  rightPane.appendChild(actionBar);

  // フォーム
  let _currentMotion = null; // 編集中モーション (null=新規)
  let _formApi = null;

  function resetForm(typeId) {
    if (_formApi?.el?.parentNode) _formApi.el.remove();
    _formApi = buildMotionForm(preview, typeId);
    rightPane.appendChild(_formApi.el);
    _currentMotion = null;
    showFeedback(feedbackEl, "", "");
  }

  // 初期フォーム
  resetForm(0);

  // --- 保存 ---
  saveBtn.addEventListener("click", async () => {
    if (!_formApi) return;
    const payload = _formApi.collectData();
    const isNew = !_currentMotion?.motionId;
    const method = isNew ? "POST" : "PUT";
    if (!isNew) payload.motionId = _currentMotion.motionId;

    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");
    try {
      const { response, data } = await fetchJson("/api/motions", {
        method,
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        const msg = data?.error ?? "HTTP " + response.status;
        showFeedback(feedbackEl, "エラー: " + msg, "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.motionId) {
        _currentMotion = { ...payload, motionId: data.motionId };
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // --- キャンセル / 新規 ---
  cancelBtn.addEventListener("click", () => {
    resetForm(leftApi.getCurrentTypeId());
    showList();
  });

  // --- 左ペイン ---
  const leftApi = buildLeftPane({
    onSelect: (m) => {
      _currentMotion = m;
      if (_formApi?.el?.parentNode) _formApi.el.remove();
      _formApi = buildMotionForm(preview, m.motionTypeId);
      rightPane.appendChild(_formApi.el);
      _formApi.setMotion(m);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onNew: () => {
      resetForm(leftApi.getCurrentTypeId());
      showDetail();
    },
    onDelete: async (m) => {
      if (!confirm("モーション (ID=" + m.motionId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/motions", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ motionId: m.motionId }),
        });
        if (!response.ok) {
          const msg = data?.error ?? "HTTP " + response.status;
          showFeedback(feedbackEl, "削除エラー: " + msg, "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + m.motionId + ")", "success");
        if (_currentMotion?.motionId === m.motionId) {
          resetForm(leftApi.getCurrentTypeId());
        }
        await leftApi.reload();
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    },
  });

  // 詳細ペインの先頭に「← 戻る」ボタンを追加
  const backBar = document.createElement("div");
  backBar.className = "me-action-bar";
  const backBtn = document.createElement("button");
  backBtn.type = "button";
  backBtn.className = "button small";
  backBtn.textContent = "← 戻る";
  backBar.appendChild(backBtn);
  rightPane.insertBefore(backBar, rightPane.firstChild);

  // 画面切替ヘルパー
  function showDetail() {
    leftApi.el.style.display = "none";
    rightPane.style.display = "";
  }
  function showList() {
    rightPane.style.display = "none";
    leftApi.el.style.display = "";
  }

  backBtn.addEventListener("click", showList);

  // 初期状態: 一覧のみ表示
  rightPane.style.display = "none";

  shell.appendChild(leftApi.el);
  shell.appendChild(rightPane);

  _destroyFn = () => {
    preview.destroy();
    container.innerHTML = "";
  };
}

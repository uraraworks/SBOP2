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
import { withBusy, armConfirmButton } from "../core/dom.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { createSoundPicker } from "../components/sound-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";
import { createEntityField, invalidateEntityCache } from "../components/entity-picker.js";
import { createListToolbar } from "../components/list-toolbar.js";
import { getRouteParams, setRouteParams } from "../core/router.js";
import { registerSaveHandler } from "../core/save-shortcut.js";

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
  const dupBtn = document.createElement("button");
  dupBtn.type = "button";
  dupBtn.className = "button small";
  dupBtn.textContent = "複製";
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

  // アイテム種別ID（itemType picker）
  const typeIdField = createEntityField({ type: "itemType", value: 1, label: "アイテム種別ID" });
  basicGrid.appendChild(typeIdField.element);

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
  // ゲームの DrawItem は item シートから描画するため item カテゴリを参照
  const sfIconGrpId = createSpriteField({
    categoryKey: "item",
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

  const mapIdField = createEntityField({ type: "map", value: 0, label: "配置マップID（0=未配置）" });
  placeGrid.appendChild(mapIdField.element);
  const posXSpin     = addSpinField("X座標", -9999, 9999);
  const posYSpin     = addSpinField("Y座標", -9999, 9999);
  const posZSpin     = addSpinField("Z座標（高さ）", -9999, 9999);
  const charIdField = createEntityField({ type: "character", value: 0, label: "所持キャラID（0=未所持）" });
  placeGrid.appendChild(charIdField.element);
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
    typeIdField.setValue(it ? (it.itemTypeId || 1) : 1);
    nameInput.value = it ? (it.name || "") : "";
    putOnCb.checked = it ? !!it.putOn : false;
    sfGrpId.setValue(it ? (it.grpId || 0) : 0);
    sfIconGrpId.setValue(it ? (it.iconGrpId || 0) : 0);
    dropSoundPicker.setValue(it ? (it.dropSoundId || 0) : 0);
    mapIdField.setValue(it ? (it.mapId || 0) : 0);
    posXSpin.setValue(it ? (it.posX || 0) : 0);
    posYSpin.setValue(it ? (it.posY || 0) : 0);
    posZSpin.setValue(it ? (it.posZ || 0) : 0);
    charIdField.setValue(it ? (it.charId || 0) : 0);
    backPackXSpin.setValue(it ? (it.backPackX || 0) : 0);
    backPackYSpin.setValue(it ? (it.backPackY || 0) : 0);
  }

  function collectData() {
    return {
      itemTypeId:  typeIdField.getValue(),
      name:        nameInput.value,
      putOn:       putOnCb.checked,
      grpId:       sfGrpId.getValue(),
      iconGrpId:   sfIconGrpId.getValue(),
      dropSoundId: dropSoundPicker.getValue(),
      mapId:       mapIdField.getValue(),
      posX:        posXSpin.getValue(),
      posY:        posYSpin.getValue(),
      posZ:        posZSpin.getValue(),
      charId:      charIdField.getValue(),
      backPackX:   backPackXSpin.getValue(),
      backPackY:   backPackYSpin.getValue(),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(it) { _current = it; }

  return { el: pane, saveBtn, dupBtn, cancelBtn, setItem, collectData, getCurrent, setCurrent };
}

// ----------------------------------------------------------------
// 左ペイン: 一覧 + フィルター
// ----------------------------------------------------------------

// アイテム一覧の並び替え（クライアント側 applyToRows で使用）
const ITEM_SORTERS = {
  id:   (a, b) => (a.itemId ?? 0) - (b.itemId ?? 0),
  name: (a, b) => String(a.name ?? "").localeCompare(String(b.name ?? "")),
  type: (a, b) => (a.itemTypeId ?? 0) - (b.itemTypeId ?? 0),
};

function buildLeftPane({ onSelect, onNew, onDelete, initialState, onStateChange }) {
  const pane = document.createElement("div");
  pane.className = "ee-left";

  // フィルター（サーバー側クエリ: drop/charId/mapId）
  const filterWrap = document.createElement("div");
  filterWrap.className = "it-filter-wrap";
  filterWrap.style.cssText = "display:flex;flex-direction:column;gap:0.25rem;margin-bottom:0.5rem;";

  const dropLbl = document.createElement("label");
  const dropCb = document.createElement("input");
  dropCb.type = "checkbox";
  dropCb.checked = initialState.drop !== false;
  dropLbl.append(dropCb, " 落ちているもののみ");
  filterWrap.appendChild(dropLbl);

  // 所持キャラID / マップID フィルター（entity field。0 のままなら未指定扱い）
  const charIdFilterField = createEntityField({ type: "character", value: initialState.charId || 0, label: "所持キャラID" });
  filterWrap.appendChild(charIdFilterField.element);

  const mapIdFilterField = createEntityField({ type: "map", value: initialState.mapId || 0, label: "マップID" });
  filterWrap.appendChild(mapIdFilterField.element);

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

  // 検索/並び替え/表示件数/ページ（クライアント側 applyToRows。名前・ID で検索、
  // ID/名前/種別で並び替え）。状態は q/sort/size/page として URL に保持する。
  const toolbar = createListToolbar({
    placeholder: "ID または 名前で検索…",
    sortOptions: [
      { value: "id",   label: "ID順" },
      { value: "name", label: "名前順" },
      { value: "type", label: "種別順" },
    ],
    pageSizes: [20, 50, 100],
    initial: {
      q: initialState.q || "",
      sort: initialState.sort || "id",
      pageSize: initialState.size || 20,
      page: initialState.page || 1,
    },
    onChange: (s) => {
      onStateChange({ q: s.q, sort: s.sort, size: s.pageSize, page: s.page });
      renderList();
    },
  });
  pane.appendChild(toolbar.element);

  // ツールバー（新規追加）
  const actionsRow = document.createElement("div");
  actionsRow.className = "me-list-toolbar";
  const newBtn = document.createElement("button");
  newBtn.type = "button";
  newBtn.className = "button small";
  newBtn.textContent = "+ 新規追加";
  newBtn.addEventListener("click", onNew);
  actionsRow.appendChild(newBtn);
  pane.appendChild(actionsRow);

  const listEl = document.createElement("ul");
  listEl.className = "ld-list";
  pane.appendChild(listEl);

  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let _allItems = [];
  let _selectedId = initialState.selectedId || null;

  function buildQuery() {
    const params = [];
    if (dropCb.checked) { params.push("drop=1"); }
    const cid = charIdFilterField.getValue();
    if (cid > 0) { params.push("charId=" + cid); }
    const mid = mapIdFilterField.getValue();
    if (mid > 0) { params.push("mapId=" + mid); }
    return params.length ? ("?" + params.join("&")) : "";
  }

  function persistFilterState() {
    onStateChange({
      drop: dropCb.checked,
      charId: charIdFilterField.getValue() || null,
      mapId: mapIdFilterField.getValue() || null,
    });
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
    const { pageRows } = toolbar.applyToRows(_allItems, {
      searchFields: ["name", (it) => String(it.itemId)],
      sorters: ITEM_SORTERS,
    });
    listEl.innerHTML = "";
    pageRows.forEach((it) => {
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
        onStateChange({ selectedId: it.itemId });
        renderList();
        onSelect(it);
      });

      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); });
      armConfirmButton(delBtn, {
        armedLabel: "本当に削除？",
        onConfirm: () => onDelete(it),
      });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });
    summary.textContent = toolbar.getState().q || pageRows.length !== _allItems.length
      ? `${pageRows.length} / ${_allItems.length} 件`
      : `${_allItems.length} 件`;
  }

  applyBtn.addEventListener("click", () => { persistFilterState(); loadList(); });
  clearBtn.addEventListener("click", () => {
    dropCb.checked = true;
    charIdFilterField.setValue(0);
    mapIdFilterField.setValue(0);
    persistFilterState();
    loadList();
  });

  function setSelectedId(id) {
    _selectedId = id;
    onStateChange({ selectedId: id || null });
    renderList();
  }

  function findById(id) {
    return _allItems.find((it) => it.itemId === id) || null;
  }

  return { el: pane, reload: loadList, setSelectedId, findById };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

// item-list ルートの現在の一覧状態。setRouteParams は「値が null/undefined/空文字なら
// キーを削除」する仕様なので、デフォルト値と同じ場合は null を渡して URL を簡潔に保つ。
function persistItemListState(partial) {
  const mapped = {};
  if ("q" in partial)      { mapped.q = partial.q || null; }
  if ("sort" in partial)   { mapped.sort = (partial.sort && partial.sort !== "id") ? partial.sort : null; }
  if ("size" in partial)   { mapped.size = (partial.size && partial.size !== 20) ? partial.size : null; }
  if ("page" in partial)   { mapped.page = (partial.page && partial.page !== 1) ? partial.page : null; }
  if ("drop" in partial)   { mapped.drop = partial.drop === false ? "0" : null; }
  if ("charId" in partial) { mapped.charId = partial.charId || null; }
  if ("mapId" in partial)  { mapped.mapId = partial.mapId || null; }
  if ("selectedId" in partial) { mapped.id = partial.selectedId || null; }
  setRouteParams(mapped);
}

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const shell = document.createElement("div");
  shell.className = "me-shell";
  container.appendChild(shell);

  // URL クエリ(q/sort/size/page/drop/charId/mapId/id)から一覧状態を復元する。
  // drop は既定 true(落下中のみ)なので "0" の時だけ false にする。
  const routeParams = getRouteParams();
  const initialState = {
    q: routeParams.get("q") || "",
    sort: routeParams.get("sort") || "id",
    size: Number(routeParams.get("size")) || 20,
    page: Number(routeParams.get("page")) || 1,
    drop: routeParams.get("drop") !== "0",
    charId: Number(routeParams.get("charId")) || 0,
    mapId: Number(routeParams.get("mapId")) || 0,
    selectedId: Number(routeParams.get("id")) || null,
  };

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

  let _detailOpen = false;

  // 画面切替ヘルパー
  function showDetail() {
    leftApi.el.style.display = "none";
    detail.el.style.display = "";
    _detailOpen = true;
  }
  function showList() {
    detail.el.style.display = "none";
    leftApi.el.style.display = "";
    _detailOpen = false;
    persistItemListState({ selectedId: null });
  }

  backBtn.addEventListener("click", showList);

  // 保存（Ctrl+S からも同じ処理を呼ぶ。withBusy が二重実行を防ぐ）
  async function performSave() {
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
        leftApi.setSelectedId(data.itemId);
      }
      invalidateEntityCache("item");
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  }

  detail.saveBtn.addEventListener("click", () => {
    withBusy(detail.saveBtn, performSave);
  });

  // Ctrl+S: 詳細ペインを開いている時だけ保存ボタンと同じ処理を呼ぶ。
  // performSave は withBusy(detail.saveBtn, ...) 経由なので二重実行防止は既存のまま効く。
  registerSaveHandler("item-list", () => {
    if (_detailOpen) { withBusy(detail.saveBtn, performSave); }
  });

  // 複製 → 表示中のフォーム内容(未保存の編集も含む)をコピーして別レコードとして保存
  // (ItemHandler.cpp の CItemCreateHandler::Handle は POST 時に必ず m_dwItemID=0 にしてから
  //  Add するため、既存レコードを上書きする心配は無い。
  //  ただし座標・マップID・所持キャラIDをそのまま複製すると同じマスや同じキャラの
  //  所持欄に重複して現れてしまうため、複製したアイテムは未配置(mapId=0)・所有者なし
  //  (charId=0)・座標ゼロで作成する。地面には出ないが item-list からいつでも配置し直せる。
  //  MakeItem(mapID=0)が地面に出ない挙動と同じ扱い)
  detail.dupBtn.addEventListener("click", () => {
    withBusy(detail.dupBtn, async () => {
      const current = detail.getCurrent();
      if (!current) {
        showFeedback(feedbackEl, "複製元のアイテムを選択してください", "error");
        return;
      }
      const payload = detail.collectData();
      payload.name = (payload.name || "") + "のコピー";
      payload.mapId = 0;
      payload.charId = 0;
      payload.posX = 0;
      payload.posY = 0;
      payload.posZ = 0;
      payload.backPackX = 0;
      payload.backPackY = 0;
      showFeedback(feedbackEl, "複製中…", "");
      try {
        const { response, data } = await fetchJson("/api/items", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "複製しました (ID=" + data.itemId + ")。未配置・所有者なしで作成しました", "success");
        const created = { ...payload, itemId: data.itemId };
        detail.setCurrent(created);
        detail.setItem(created);
        invalidateEntityCache("item");
        await leftApi.reload();
        leftApi.setSelectedId(data.itemId);
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    }, { busyText: "複製中…" });
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
          leftApi.setSelectedId(null);
        }
        invalidateEntityCache("item");
        await leftApi.reload();
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    },
    initialState,
    onStateChange: persistItemListState,
  });

  // 初期状態: 一覧のみ表示
  detail.el.style.display = "none";

  shell.appendChild(leftApi.el);
  shell.appendChild(detail.el);

  // 初回ロード。URL に選択中アイテム ID (id) が残っていれば、取得後にその詳細を開く。
  leftApi.reload().then(() => {
    if (initialState.selectedId) {
      const found = leftApi.findById(initialState.selectedId);
      if (found) {
        detail.setItem(found);
        showFeedback(feedbackEl, "", "");
        showDetail();
      } else {
        // 該当アイテムが見つからない(削除済み等)場合は URL から id を落とす
        persistItemListState({ selectedId: null });
      }
    }
  });

  _destroyFn = () => {
    container.innerHTML = "";
  };
}

/**
 * views/map-events.js
 * マップイベント編集画面 (route: map-events)
 *
 * API:
 *   GET /api/maps                          → マップ一覧
 *   GET /api/maps/events?mapId=N           → イベント一覧
 *   POST /api/maps/events                  → イベント新規作成
 *   PUT  /api/maps/events                  → イベント更新
 *   DELETE /api/maps/events?mapId=N&id=N   → イベント削除
 *
 * ゲーム iframe 連携: core/game-pick.js に registerPickHandler("map-events", ...)
 * で登録し、セルクリックからイベントを検索・選択する。
 */

import { fetchJson } from "../core/api.js";
import { createEntityField } from "../components/entity-picker.js";
import { registerPickHandler, unregisterPickHandler, requestNextPick } from "../core/game-pick.js";
import { showSuccessToast } from "../components/toast.js";
import { registerSaveHandler, unregisterSaveHandler } from "../core/save-shortcut.js";
import { getRouteParams, setRouteParams } from "../core/router.js";
import { createListToolbar } from "../components/list-toolbar.js";

const MAP_EVENT_TYPE_LABELS = {
  0: "なし (NONE)",
  1: "マップ内移動 (MOVE)",
  2: "マップ間移動 (MAPMOVE)",
  3: "ゴミ箱 (TRASHBOX)",
  4: "ステータス初期化 (INITSTATUS)",
  5: "一時画像設定 (GRPIDTMP)",
  6: "灯り (LIGHT)"
};

const MAP_EVENT_HIT_TYPE_LABELS = {
  0: "MAPPOS（マップ座標）",
  1: "CHARPOS（キャラ座標）",
  2: "AREA（範囲）",
  3: "MAPPOS2（マップ座標完全一致）"
};

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function buildDetailFieldsHtml(type, detail) {
  detail = detail || {};
  switch (Number(type)) {
  case 1: // MOVE
    return `<label class="form-field"><span>移動先 X</span><input type="number" name="detail.destX" value="${detail.destX || 0}"></label>` +
           `<label class="form-field"><span>移動先 Y</span><input type="number" name="detail.destY" value="${detail.destY || 0}"></label>` +
           `<label class="form-field"><span>向き</span><input type="number" name="detail.direction" value="${detail.direction || 0}"></label>`;
  case 2: // MAPMOVE
    return `<div class="form-field" id="map-event-destmapid-wrap" data-detail-destmapid="${detail.destMapId || 0}"></div>` +
           `<label class="form-field"><span>移動先 X</span><input type="number" name="detail.destX" value="${detail.destX || 0}"></label>` +
           `<label class="form-field"><span>移動先 Y</span><input type="number" name="detail.destY" value="${detail.destY || 0}"></label>` +
           `<label class="form-field"><span>向き</span><input type="number" name="detail.direction" value="${detail.direction || 0}"></label>` +
           `<div class="form-actions"><button type="button" class="btn btn-secondary" id="map-event-mapmove-pick-btn">ゲーム画面でクリックして指定</button></div>`;
  case 3: // TRASHBOX
    return '<p class="field-note">固有フィールドなし</p>';
  case 4: // INITSTATUS
    return `<label class="form-field"><span>エフェクトID</span><input type="number" name="detail.effectId" value="${detail.effectId || 0}"></label>`;
  case 5: // GRPIDTMP
    return `<label class="form-field"><span>設定種別 (0=解除 1=設定)</span><input type="number" name="detail.setType" value="${detail.setType || 0}"></label>` +
           `<label class="form-field"><span>画像IDメイン</span><input type="number" name="detail.idMain" value="${detail.idMain || 0}"></label>` +
           `<label class="form-field"><span>画像IDサブ</span><input type="number" name="detail.idSub" value="${detail.idSub || 0}"></label>`;
  case 6: // LIGHT
    return `<label class="form-field"><span>灯りON</span><input type="checkbox" name="detail.lightOn"${detail.lightOn ? " checked" : ""}></label>` +
           `<label class="form-field"><span>持続時間</span><input type="number" name="detail.time" value="${detail.time || 0}"></label>`;
  default:
    return "";
  }
}

// buildDetailFieldsHtml が type=2 (MAPMOVE) 用に置いたプレースホルダに
// マップ picker 付きの entity field を差し込む。FormData で拾えるよう
// input に name="detail.destMapId" を付与する。
function mountMapMoveDestField(container) {
  if (!container) { return null; }
  const wrap = container.querySelector("#map-event-destmapid-wrap");
  if (!wrap) { return null; }
  const initialValue = parseInt(wrap.dataset.detailDestmapid || "0", 10) || 0;
  const field = createEntityField({ type: "map", value: initialValue, label: "移動先マップID" });
  field.input.name = "detail.destMapId";
  wrap.replaceWith(field.element);
  return field;
}

// 「ゲーム画面でクリックして指定」ボタン: 次の1回の pick を移動先マップID/X/Y へ
// 反映する。編集中フォームへの値入力にすぎないため破棄確認(confirmDiscard)は呼ばない。
function mountMapMoveDestPickButton(container, destMapField) {
  if (!container) { return; }
  const btn = container.querySelector("#map-event-mapmove-pick-btn");
  if (!btn) { return; }
  btn.addEventListener("click", () => {
    requestNextPick({
      message: "移動先にするセルをゲーム画面でクリックしてください(Esc で中止)",
      onPick: (pick) => {
        if (destMapField) { destMapField.setValue(pick.mapId); }
        const destXInput = container.querySelector('[name="detail.destX"]');
        const destYInput = container.querySelector('[name="detail.destY"]');
        if (destXInput) { destXInput.value = String(pick.cellX); }
        if (destYInput) { destYInput.value = String(pick.cellY); }
      }
    });
  });
}

function collectMapEventPayload(form, selectedMapId) {
  const data = new FormData(form);
  const typeVal = parseInt(data.get("type") || "1", 10);
  const payload = {
    mapId:        selectedMapId,
    id:           parseInt(data.get("id") || "0", 10),
    type:         typeVal,
    soundId:      parseInt(data.get("soundId") || "0", 10),
    hitType:      parseInt(data.get("hitType") || "0", 10),
    hitDirection: parseInt(data.get("hitDirection") || "0", 10),
    pos:  { x: parseInt(data.get("posX")  || "0", 10), y: parseInt(data.get("posY")  || "0", 10) },
    pos2: { x: parseInt(data.get("pos2X") || "0", 10), y: parseInt(data.get("pos2Y") || "0", 10) },
    detail: {}
  };
  switch (typeVal) {
  case 1: payload.detail = { destX: parseInt(data.get("detail.destX") || "0", 10), destY: parseInt(data.get("detail.destY") || "0", 10), direction: parseInt(data.get("detail.direction") || "0", 10) }; break;
  case 2: payload.detail = { destMapId: parseInt(data.get("detail.destMapId") || "0", 10), destX: parseInt(data.get("detail.destX") || "0", 10), destY: parseInt(data.get("detail.destY") || "0", 10), direction: parseInt(data.get("detail.direction") || "0", 10) }; break;
  case 4: payload.detail = { effectId: parseInt(data.get("detail.effectId") || "0", 10) }; break;
  case 5: payload.detail = { setType: parseInt(data.get("detail.setType") || "0", 10), idMain: parseInt(data.get("detail.idMain") || "0", 10), idSub: parseInt(data.get("detail.idSub") || "0", 10) }; break;
  case 6: {
    const lightOnEl = form.querySelector('[name="detail.lightOn"]');
    payload.detail = { lightOn: lightOnEl ? lightOnEl.checked : false, time: parseInt(data.get("detail.time") || "0", 10) };
    break;
  }
  default: break;
  }
  return payload;
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card" id="map-events-browser">
        <h2>マップイベント編集</h2>
        <p class="card-description">マップを選択してイベントを確認・編集します。</p>

        <!-- 一覧ペイン -->
        <div id="map-event-list-section">
          <div class="map-info-controls">
            <label class="form-field">
              <span>マップ選択</span>
              <select id="map-event-map" name="mapId"></select>
            </label>
            <button type="button" class="btn btn-primary" id="map-event-new-btn">新規追加</button>
          </div>

          <!-- 会話イベント editor への導線（共通コンポーネントの組み込み動作確認用） -->
          <div class="form-actions" style="margin-top:0.5rem;">
            <label class="form-field">
              <span>会話イベント ID (NPC/キャラ ID)</span>
              <input type="number" id="map-event-talk-id-input" min="1" value="1">
            </label>
            <button type="button" class="btn btn-secondary"
                    data-talk-event data-talk-event-target="map-event-talk-id-input">
              会話イベント editor を開く
            </button>
          </div>

          <p id="map-event-summary" class="section-summary" aria-live="polite"></p>

          <div id="map-event-toolbar"></div>

          <div class="table-wrapper">
            <table class="data-table">
              <thead>
                <tr>
                  <th>ID</th>
                  <th>種別</th>
                  <th>座標1</th>
                  <th>当たり判定</th>
                  <th>操作</th>
                </tr>
              </thead>
              <tbody id="map-event-table-body"></tbody>
            </table>
          </div>

          <p id="map-event-feedback" class="form-feedback" aria-live="polite"></p>
          <div class="form-actions" id="map-event-create-here-actions" style="display:none;">
            <button type="button" class="btn btn-primary" id="map-event-create-here-btn">ここに新規作成</button>
          </div>
        </div>

        <!-- 詳細ペイン（編集フォーム） -->
        <div id="map-event-detail-section" style="display:none;">
          <div class="view-detail-toolbar">
            <button type="button" class="btn btn-secondary" id="map-event-back-btn">← 一覧に戻る</button>
          </div>
          <div id="map-event-edit-area"></div>
        </div>
      </section>`;

  const mapEventMapSelect = container.querySelector("#map-event-map");
  const summaryEl         = container.querySelector("#map-event-summary");
  const tableBody         = container.querySelector("#map-event-table-body");
  const editArea          = container.querySelector("#map-event-edit-area");
  const feedbackEl        = container.querySelector("#map-event-feedback");
  const newBtn            = container.querySelector("#map-event-new-btn");
  const listSection       = container.querySelector("#map-event-list-section");
  const detailSection     = container.querySelector("#map-event-detail-section");
  const backBtn           = container.querySelector("#map-event-back-btn");
  const createHereActions = container.querySelector("#map-event-create-here-actions");
  const createHereBtn     = container.querySelector("#map-event-create-here-btn");
  const toolbarHost       = container.querySelector("#map-event-toolbar");

  const state = {
    maps: [],
    selectedMapId: null,
    events: [],
    selectedEventId: null,
    isLoading: false,
    loadError: null,
    pendingCreatePos: null, // { x, y } クリックしたセルにイベントが無かった時の「ここに新規作成」候補座標
  };

  // 一覧の検索/並び替え。選択中マップ/イベントIDと合わせて URL のクエリ(map/id/q/sort/page)に
  // 反映し、リロード時に復元する(setRouteParams は history.replaceState のみで hashchange を
  // 発火させないため、ここでの状態更新が再 mount を招くことはない)。
  const routeParams = getRouteParams();
  const initialMapParam = routeParams.get("map");
  const initialEventIdParam = routeParams.get("id");
  const toolbar = createListToolbar({
    placeholder: "ID・種別・座標で検索",
    sortOptions: [
      { value: "id", label: "ID順" },
      { value: "type", label: "種別順" },
    ],
    pageSizes: [20, 50, 100],
    initial: {
      q: routeParams.get("q") || "",
      sort: routeParams.get("sort") || "id",
      page: Number(routeParams.get("page")) || 1,
    },
    onChange: (s) => {
      setRouteParams({
        q: s.q || null,
        sort: s.sort && s.sort !== "id" ? s.sort : null,
        page: s.page && s.page !== 1 ? s.page : null,
      });
      renderTable();
    },
  });
  if (toolbarHost) { toolbarHost.appendChild(toolbar.element); }

  function hideCreateHereButton() {
    state.pendingCreatePos = null;
    if (createHereActions) { createHereActions.style.display = "none"; }
  }

  function showCreateHereButton(x, y) {
    state.pendingCreatePos = { x, y };
    if (createHereActions) { createHereActions.style.display = ""; }
  }

  function setSummary(msg) { if (summaryEl) { summaryEl.textContent = msg || ""; } }
  function setFeedback(msg, type) {
    if (!feedbackEl) { return; }
    feedbackEl.textContent = msg || "";
    feedbackEl.className = "form-feedback" + (type ? " form-feedback--" + type : "");
  }

  function renderMapSelect() {
    if (!mapEventMapSelect) { return; }
    const prevId = state.selectedMapId;
    mapEventMapSelect.innerHTML = "";
    if (!state.maps.length) {
      const opt = document.createElement("option");
      opt.value = "";
      opt.textContent = "（マップなし）";
      mapEventMapSelect.appendChild(opt);
      return;
    }
    state.maps.forEach((map) => {
      const opt = document.createElement("option");
      opt.value = String(map.id);
      opt.textContent = `[${map.id}] ${map.name || "（名前なし）"}`;
      if (map.id === prevId) { opt.selected = true; }
      mapEventMapSelect.appendChild(opt);
    });
    if (!state.maps.some((m) => m.id === state.selectedMapId)) {
      state.selectedMapId = state.maps[0].id;
      mapEventMapSelect.value = String(state.selectedMapId);
    }
  }

  function renderTable() {
    if (!tableBody) { return; }
    tableBody.innerHTML = "";
    if (!state.events.length) {
      const tr = document.createElement("tr");
      const td = document.createElement("td");
      td.colSpan = 5;
      td.textContent = "イベントなし";
      td.style.textAlign = "center";
      tr.appendChild(td);
      tableBody.appendChild(tr);
      return;
    }
    const { pageRows } = toolbar.applyToRows(state.events, {
      searchFields: [
        (e) => String(e.id),
        (e) => e.typeLabel || String(e.type),
        (e) => `${e.pos.x},${e.pos.y}`,
      ],
      sorters: {
        id: (a, b) => a.id - b.id,
        type: (a, b) => a.type - b.type || a.id - b.id,
      },
    });
    if (!pageRows.length) {
      const tr = document.createElement("tr");
      const td = document.createElement("td");
      td.colSpan = 5;
      td.textContent = "該当するイベントがありません";
      td.style.textAlign = "center";
      tr.appendChild(td);
      tableBody.appendChild(tr);
      return;
    }
    pageRows.forEach((ev) => {
      const tr = document.createElement("tr");
      if (ev.id === state.selectedEventId) { tr.classList.add("selected-row"); }
      function td(text) { const el = document.createElement("td"); el.textContent = text; return el; }
      tr.appendChild(td(ev.id));
      tr.appendChild(td(ev.typeLabel || String(ev.type)));
      tr.appendChild(td(`(${ev.pos.x}, ${ev.pos.y})`));
      tr.appendChild(td(ev.hitTypeLabel || String(ev.hitType)));
      const tdBtn = document.createElement("td");
      const btn = document.createElement("button");
      btn.type = "button";
      btn.className = "btn btn-secondary btn-sm";
      btn.textContent = "編集";
      btn.addEventListener("click", () => {
        state.selectedEventId = ev.id;
        renderTable();
        renderForm(ev);
      });
      tdBtn.appendChild(btn);
      tr.appendChild(tdBtn);
      tableBody.appendChild(tr);
    });
  }

  // 選択中マップ/イベントIDをURLに反映する(map/id)。detailSection非表示時はid無し。
  function syncRouteParams() {
    setRouteParams({
      map: state.selectedMapId != null ? state.selectedMapId : null,
      id: (detailSection && detailSection.style.display !== "none" && state.selectedEventId)
        ? state.selectedEventId : null,
    });
  }

  function showListSection() {
    if (listSection) { listSection.style.display = ""; }
    if (detailSection) { detailSection.style.display = "none"; }
    state.selectedEventId = null;
    if (editArea) { editArea.innerHTML = ""; }
    hideCreateHereButton();
    syncRouteParams();
  }

  function showDetailSection() {
    if (listSection) { listSection.style.display = "none"; }
    if (detailSection) { detailSection.style.display = ""; }
    syncRouteParams();
  }

  function renderForm(ev, presetPos) {
    if (!editArea) { return; }
    hideCreateHereButton();
    showDetailSection();
    const isNew   = (ev === null || ev === undefined);
    const eventId = isNew ? 0 : ev.id;
    const type    = isNew ? 1 : ev.type;
    const detail  = isNew ? {} : (ev.detail || {});

    let hitTypeOptions = "";
    Object.keys(MAP_EVENT_HIT_TYPE_LABELS).forEach((k) => {
      const sel = (!isNew && ev.hitType === Number(k)) ? " selected" : "";
      hitTypeOptions += `<option value="${k}"${sel}>${MAP_EVENT_HIT_TYPE_LABELS[k]}</option>`;
    });
    let typeOptions = "";
    Object.keys(MAP_EVENT_TYPE_LABELS).forEach((k) => {
      if (k === "0") { return; }
      const sel = (Number(k) === type) ? " selected" : "";
      typeOptions += `<option value="${k}"${sel}>${MAP_EVENT_TYPE_LABELS[k]}</option>`;
    });
    const pos  = isNew ? (presetPos || { x: 0, y: 0 }) : ev.pos;
    const pos2 = isNew ? { x: 0, y: 0 } : ev.pos2;

    editArea.innerHTML =
      `<form id="map-event-edit-form-mod" class="edit-form">` +
      `<input type="hidden" name="id" value="${eventId}">` +
      `<h3>${isNew ? "新規イベント" : "イベント ID: " + eventId}</h3>` +
      `<label class="form-field"><span>種別</span><select name="type" id="map-event-type-select-mod">${typeOptions}</select></label>` +
      `<label class="form-field"><span>効果音ID</span><input type="number" name="soundId" value="${isNew ? 0 : ev.soundId}"></label>` +
      `<label class="form-field"><span>当たり判定種別</span><select name="hitType">${hitTypeOptions}</select></label>` +
      `<label class="form-field"><span>判定向き</span><input type="number" name="hitDirection" value="${isNew ? 0 : ev.hitDirection}"></label>` +
      `<label class="form-field"><span>座標1 X</span><input type="number" name="posX" value="${pos.x}"></label>` +
      `<label class="form-field"><span>座標1 Y</span><input type="number" name="posY" value="${pos.y}"></label>` +
      `<label class="form-field"><span>座標2 X</span><input type="number" name="pos2X" value="${pos2.x}"></label>` +
      `<label class="form-field"><span>座標2 Y</span><input type="number" name="pos2Y" value="${pos2.y}"></label>` +
      `<div id="map-event-detail-fields-mod">${buildDetailFieldsHtml(type, detail)}</div>` +
      `<div class="form-actions">` +
        `<button type="submit" class="btn btn-primary">保存</button>` +
        (!isNew ? `<button type="button" class="btn btn-danger" id="map-event-delete-btn-mod">削除</button>` : "") +
        `<button type="button" class="btn btn-secondary" id="map-event-cancel-btn-mod">キャンセル</button>` +
      `</div>` +
      `</form>` +
      `<p id="map-event-form-feedback" class="form-feedback" aria-live="polite"></p>`;

    // 種別変更
    const typeSelect = editArea.querySelector("#map-event-type-select-mod");
    const detailFieldsEl = editArea.querySelector("#map-event-detail-fields-mod");
    mountMapMoveDestPickButton(detailFieldsEl, mountMapMoveDestField(detailFieldsEl));
    if (typeSelect && detailFieldsEl) {
      typeSelect.addEventListener("change", () => {
        detailFieldsEl.innerHTML = buildDetailFieldsHtml(typeSelect.value, {});
        mountMapMoveDestPickButton(detailFieldsEl, mountMapMoveDestField(detailFieldsEl));
      });
    }

    const form = editArea.querySelector("#map-event-edit-form-mod");
    if (form) { form.addEventListener("submit", saveEvent); }

    const deleteBtn = editArea.querySelector("#map-event-delete-btn-mod");
    if (deleteBtn) { deleteBtn.addEventListener("click", () => { deleteEvent(eventId); }); }

    const cancelBtn = editArea.querySelector("#map-event-cancel-btn-mod");
    if (cancelBtn) {
      cancelBtn.addEventListener("click", () => {
        showListSection();
      });
    }
  }

  function getFormFeedbackEl() {
    return (editArea && editArea.querySelector("#map-event-form-feedback")) || feedbackEl;
  }

  async function saveEvent(event) {
    if (event) { event.preventDefault(); }
    const form = event ? event.target : editArea && editArea.querySelector("#map-event-edit-form-mod");
    if (!form) { return; }
    const payload = collectMapEventPayload(form, state.selectedMapId);
    const isNew   = (payload.id === 0);
    const method  = isNew ? "POST" : "PUT";
    const fbEl = getFormFeedbackEl();
    if (fbEl) { fbEl.textContent = "保存中..."; fbEl.className = "form-feedback"; }
    try {
      const { response, data } = await fetchJson("/api/maps/events", {
        method,
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
      });
      if (!response.ok || !data || !data.id) {
        throw new Error((data && data.error) ? data.error : "HTTP " + response.status);
      }
      if (isNew) { state.events.push(data); }
      else {
        const idx = state.events.findIndex((e) => e.id === data.id);
        if (idx >= 0) { state.events[idx] = data; }
      }
      state.selectedEventId = data.id;
      setSummary(`イベント ${state.events.length} 件`);
      renderTable();
      showSuccessToast("イベント " + data.id + " を保存しました");
      // 保存後は一覧に自動遷移せず、その場（編集フォーム）に留まる。
      // 新規作成直後はサーバー採番の id を hidden input へ反映する必要があるため
      // フォームを最新データで再描画する（PUT/POST の取り違えと id=0 の重複作成を防ぐ）。
      renderForm(data);
      const newFbEl = getFormFeedbackEl();
      if (newFbEl) { newFbEl.textContent = "保存しました"; newFbEl.className = "form-feedback form-feedback--success"; }
    } catch (err) {
      if (fbEl) { fbEl.textContent = "保存に失敗しました: " + err.message; fbEl.className = "form-feedback form-feedback--error"; }
    }
  }

  async function deleteEvent(eventId) {
    if (!state.selectedMapId || !eventId) { return; }
    const fbEl = getFormFeedbackEl();
    if (fbEl) { fbEl.textContent = "削除中..."; fbEl.className = "form-feedback"; }
    try {
      const { response, data } = await fetchJson(
        `/api/maps/events?mapId=${state.selectedMapId}&id=${eventId}`,
        { method: "DELETE" }
      );
      if (!response.ok) {
        throw new Error((data && data.error) ? data.error : "HTTP " + response.status);
      }
      state.events = state.events.filter((e) => e.id !== eventId);
      setSummary(`イベント ${state.events.length} 件`);
      renderTable();
      showListSection();
    } catch (err) {
      if (fbEl) { fbEl.textContent = "削除に失敗しました: " + err.message; fbEl.className = "form-feedback form-feedback--error"; }
    }
  }

  async function loadEventList() {
    if (!state.selectedMapId) { return; }
    state.isLoading = true;
    state.loadError = null;
    setSummary("読み込み中...");
    try {
      const { response, data } = await fetchJson("/api/maps/events?mapId=" + state.selectedMapId);
      if (!response.ok || !data || !Array.isArray(data.events)) {
        throw new Error("invalid_response");
      }
      state.events = data.events;
      state.isLoading = false;
      setSummary(`イベント ${state.events.length} 件`);
      renderTable();
      showListSection();
    } catch (err) {
      state.events = [];
      state.isLoading = false;
      state.loadError = "イベント一覧の取得に失敗しました";
      setSummary(state.loadError);
      renderTable();
    }
  }

  async function initView() {
    if (!mapEventMapSelect) { return; }
    // マップ一覧取得
    try {
      const { response, data } = await fetchJson("/api/maps");
      if (response.ok && data && Array.isArray(data.maps)) {
        state.maps = data.maps.slice().sort((a, b) => a.id - b.id);
      }
    } catch { /* ignore */ }

    // URL の map= を優先して復元する(存在するマップIDの場合のみ)
    if (initialMapParam !== null && state.maps.some((m) => String(m.id) === initialMapParam)) {
      state.selectedMapId = parseInt(initialMapParam, 10);
    }

    renderMapSelect();

    if (state.maps.length && !state.selectedMapId) {
      state.selectedMapId = state.maps[0].id;
    }
    if (mapEventMapSelect && state.selectedMapId) {
      mapEventMapSelect.value = String(state.selectedMapId);
    }
    if (state.selectedMapId) {
      await loadEventList();
      // URL の id= を復元(一覧取得後でないと該当イベントを引けない)
      if (initialEventIdParam !== null) {
        const restoredId = parseInt(initialEventIdParam, 10);
        const ev = state.events.find((e) => e.id === restoredId);
        if (ev) {
          state.selectedEventId = ev.id;
          renderTable();
          renderForm(ev);
        }
      }
    }
  }

  // イベント登録
  if (mapEventMapSelect) {
    mapEventMapSelect.addEventListener("change", () => {
      const id = parseInt(mapEventMapSelect.value, 10);
      if (!isNaN(id)) {
        state.selectedMapId = id;
        state.selectedEventId = null;
        loadEventList();
      }
    });
  }
  if (newBtn) {
    newBtn.addEventListener("click", () => {
      state.selectedEventId = null;
      renderForm(null);
    });
  }
  if (backBtn) {
    backBtn.addEventListener("click", () => {
      showListSection();
    });
  }
  if (createHereBtn) {
    createHereBtn.addEventListener("click", () => {
      const pos = state.pendingCreatePos;
      if (!pos) { return; }
      hideCreateHereButton();
      state.selectedEventId = null;
      renderForm(null, pos);
    });
  }

  // data-talk-event 連携（会話イベント editor を開くボタン）は app.js の bindTalkEventEditor が担当
  // このビュー内でバインドは不要

  // ゲーム画面クリック連携: セル座標から該当イベントを検索して選択する。
  // 別マップをクリックした場合はそのマップへ切り替えてからイベント一覧を再取得する。
  function handleGamePick(pick) {
    if (!pick.mapId) { return false; }

    const switchAndFind = function () {
      const ev = state.events.find(function (e) {
        if (e.hitType === 2) {
          const x1 = Math.min(e.pos.x, e.pos2.x);
          const x2 = Math.max(e.pos.x, e.pos2.x);
          const y1 = Math.min(e.pos.y, e.pos2.y);
          const y2 = Math.max(e.pos.y, e.pos2.y);
          return pick.cellX >= x1 && pick.cellX <= x2 && pick.cellY >= y1 && pick.cellY <= y2;
        }
        return e.pos.x === pick.cellX && e.pos.y === pick.cellY;
      });
      if (ev) {
        state.selectedEventId = ev.id;
        renderTable();
        renderForm(ev);
        setFeedback("(" + pick.cellX + "," + pick.cellY + ") のイベントを選択しました", "success");
      } else {
        setFeedback("(" + pick.cellX + "," + pick.cellY + ") にイベントはありません", "");
        showCreateHereButton(pick.cellX, pick.cellY);
      }
    };

    if (state.selectedMapId !== pick.mapId) {
      state.selectedMapId = pick.mapId;
      loadEventList().then(switchAndFind).catch(function () {
        setFeedback("イベント一覧の取得に失敗しました", "error");
      });
    } else {
      switchAndFind();
    }
    return true;
  }

  registerPickHandler("map-events", handleGamePick);

  // Ctrl+S: 詳細ペイン(編集フォーム)を開いている時だけ保存を呼ぶ。
  // saveEvent は非同期だが多重クリック/多重送信対策は既存のまま(フォーム再描画で
  // hidden id が最新化されるため PUT/POST の取り違えは起きない)。
  registerSaveHandler("map-events", () => {
    if (detailSection && detailSection.style.display !== "none") {
      saveEvent();
    }
  });

  initView();

  return {
    unmount() {
      unregisterPickHandler("map-events", handleGamePick);
    }
  };
}

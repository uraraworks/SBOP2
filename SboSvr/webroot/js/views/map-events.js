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
 * ゲーム iframe 連携: app.js の handleAdminGamePick が map-events ルートで
 * このモジュールの state を参照する。window._mapEventsState で公開する。
 */

import { fetchJson } from "../core/api.js";

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
    return `<label class="form-field"><span>移動先マップID</span><input type="number" name="detail.destMapId" value="${detail.destMapId || 0}"></label>` +
           `<label class="form-field"><span>移動先 X</span><input type="number" name="detail.destX" value="${detail.destX || 0}"></label>` +
           `<label class="form-field"><span>移動先 Y</span><input type="number" name="detail.destY" value="${detail.destY || 0}"></label>` +
           `<label class="form-field"><span>向き</span><input type="number" name="detail.direction" value="${detail.direction || 0}"></label>`;
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

  const state = {
    maps: [],
    selectedMapId: null,
    events: [],
    selectedEventId: null,
    isLoading: false,
    loadError: null,
  };

  // app.js の handleAdminGamePick から参照できるように公開
  window._mapEventsState = state;
  // ゲーム連携: イベント一覧再ロードとフォーム表示のコールバック
  window._mapEventsReload = loadEventList;
  window._mapEventsRenderTable = renderTable;
  window._mapEventsRenderForm = renderForm;
  window._mapEventsFeedback = setFeedback;

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
    state.events.forEach((ev) => {
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

  function showListSection() {
    if (listSection) { listSection.style.display = ""; }
    if (detailSection) { detailSection.style.display = "none"; }
    state.selectedEventId = null;
    if (editArea) { editArea.innerHTML = ""; }
  }

  function showDetailSection() {
    if (listSection) { listSection.style.display = "none"; }
    if (detailSection) { detailSection.style.display = ""; }
  }

  function renderForm(ev) {
    if (!editArea) { return; }
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
    const pos  = isNew ? { x: 0, y: 0 } : ev.pos;
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
    if (typeSelect && detailFieldsEl) {
      typeSelect.addEventListener("change", () => {
        detailFieldsEl.innerHTML = buildDetailFieldsHtml(typeSelect.value, {});
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
      if (fbEl) { fbEl.textContent = "保存しました"; fbEl.className = "form-feedback form-feedback--success"; }
      renderTable();
      // 保存後は一覧に戻る（少し遅延してメッセージを見せる）
      setTimeout(() => { showListSection(); }, 600);
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

    renderMapSelect();

    if (state.maps.length && !state.selectedMapId) {
      state.selectedMapId = state.maps[0].id;
      mapEventMapSelect.value = String(state.selectedMapId);
    }
    if (state.selectedMapId) { await loadEventList(); }
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

  // data-talk-event 連携（会話イベント editor を開くボタン）は app.js の bindTalkEventEditor が担当
  // このビュー内でバインドは不要

  initView();

  return {
    unmount() {
      delete window._mapEventsState;
      delete window._mapEventsReload;
      delete window._mapEventsRenderTable;
      delete window._mapEventsRenderForm;
      delete window._mapEventsFeedback;
    }
  };
}

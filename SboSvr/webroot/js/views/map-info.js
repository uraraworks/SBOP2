/**
 * views/map-info.js
 * マップ情報編集画面 (route: map-info)
 *
 * API:
 *   GET  /api/maps → { maps: [...] }
 *   PUT  /api/maps → 更新（payload: {id, name, bgmId, weatherType, darknessLevel, battleEnabled, recoveryEnabled}）
 *   POST /api/maps → 新規作成（payload: {copyFromMapId}）
 */

import { fetchJson } from "../core/api.js";

export function mount(container) {
  container.innerHTML = `
      <section class="card" id="map-info-browser">
        <h2>マップ情報編集</h2>
        <p class="card-description">
          マップ名・BGM・天候・戦闘設定などの基本情報を編集します。
          名前を変更すると全クライアントへ通知されます。
        </p>

        <!-- 一覧ペイン -->
        <div id="map-info-list-section">
          <div class="map-parts-controls">
            <button id="map-info-reload-btn" class="btn btn-secondary btn-sm">再読み込み</button>
            <button id="map-info-add-btn" class="btn btn-primary btn-sm">マップ追加</button>
          </div>

          <div id="map-info-add-area" style="display:none" class="form-grid" style="margin-top:0.5rem;">
            <label class="form-field compact">
              <span class="form-label">コピー元マップ（空で空マップ作成）</span>
              <select id="map-info-copy-select" class="form-select">
                <option value="0">（空マップを新規作成）</option>
              </select>
            </label>
            <div class="form-actions">
              <button id="map-info-add-confirm-btn" class="btn btn-primary btn-sm">作成</button>
              <button id="map-info-add-cancel-btn" class="btn btn-secondary btn-sm">キャンセル</button>
            </div>
          </div>

          <p id="map-info-summary" class="map-parts-summary" aria-live="polite"></p>

          <div class="table-wrapper" id="map-info-table-wrapper">
            <table class="data-table">
              <thead>
                <tr>
                  <th>ID</th>
                  <th>マップ名</th>
                  <th>BGM</th>
                  <th>天候</th>
                  <th>操作</th>
                </tr>
              </thead>
              <tbody id="map-info-table-body"></tbody>
            </table>
          </div>

          <p id="map-info-feedback" class="form-feedback" aria-live="polite"></p>
        </div>

        <!-- 詳細ペイン（編集フォーム） -->
        <div id="map-info-detail-section" style="display:none;">
          <div class="view-detail-toolbar">
            <button type="button" class="btn btn-secondary" id="map-info-back-btn">← 一覧に戻る</button>
          </div>
          <div id="map-info-edit-area">
            <form id="map-info-edit-form" class="form-grid" novalidate>
              <label class="form-field">
                <span class="form-label">マップ名</span>
                <input type="text" id="map-info-name" class="form-input" maxlength="64" />
              </label>
              <label class="form-field">
                <span class="form-label">BGM ID</span>
                <input type="number" id="map-info-bgm" class="form-input" min="0" step="1" />
              </label>
              <label class="form-field">
                <span class="form-label">天候</span>
                <select id="map-info-weather" class="form-select">
                  <option value="0">指定なし (NONE)</option>
                  <option value="1">雲 (CLOUD)</option>
                  <option value="2">霧 (MISTY)</option>
                  <option value="3">雪 (SNOW)</option>
                </select>
              </label>
              <label class="form-field">
                <span class="form-label">暗さレベル (0–255)</span>
                <input type="number" id="map-info-darkness" class="form-input" min="0" max="255" step="1" />
              </label>
              <div class="form-field form-field--inline">
                <label class="form-checkbox">
                  <input type="checkbox" id="map-info-battle" />
                  <span>戦闘許可</span>
                </label>
                <label class="form-checkbox">
                  <input type="checkbox" id="map-info-recovery" />
                  <span>気絶後回復</span>
                </label>
              </div>
              <div class="form-actions">
                <button type="submit" class="btn btn-primary" id="map-info-save-btn">保存</button>
                <button type="button" class="btn btn-secondary" id="map-info-cancel-btn">キャンセル</button>
              </div>
            </form>
          </div>
          <p id="map-info-detail-feedback" class="form-feedback" aria-live="polite"></p>
        </div>
      </section>`;

  const tableBody           = container.querySelector("#map-info-table-body");
  const summaryEl           = container.querySelector("#map-info-summary");
  const editArea            = container.querySelector("#map-info-edit-area");
  const editForm            = container.querySelector("#map-info-edit-form");
  const nameInput           = container.querySelector("#map-info-name");
  const bgmInput            = container.querySelector("#map-info-bgm");
  const weatherSelect       = container.querySelector("#map-info-weather");
  const darknessInput       = container.querySelector("#map-info-darkness");
  const battleCheck         = container.querySelector("#map-info-battle");
  const recoveryCheck       = container.querySelector("#map-info-recovery");
  const feedbackEl          = container.querySelector("#map-info-feedback");
  const detailFeedbackEl    = container.querySelector("#map-info-detail-feedback");
  const reloadBtn           = container.querySelector("#map-info-reload-btn");
  const addBtn              = container.querySelector("#map-info-add-btn");
  const addArea             = container.querySelector("#map-info-add-area");
  const copySelect          = container.querySelector("#map-info-copy-select");
  const addConfirmBtn       = container.querySelector("#map-info-add-confirm-btn");
  const addCancelBtn        = container.querySelector("#map-info-add-cancel-btn");
  const listSection         = container.querySelector("#map-info-list-section");
  const detailSection       = container.querySelector("#map-info-detail-section");
  const backBtn             = container.querySelector("#map-info-back-btn");
  const cancelBtn           = container.querySelector("#map-info-cancel-btn");

  const state = {
    maps: [],
    selectedMapId: null,
    isLoading: false,
    loadError: null,
  };

  function setSummary(msg) {
    if (summaryEl) { summaryEl.textContent = msg || ""; }
  }

  function setFeedback(msg, type) {
    if (!feedbackEl) { return; }
    feedbackEl.textContent = msg || "";
    feedbackEl.className = "form-feedback";
    if (type === "success") { feedbackEl.classList.add("form-feedback--success"); }
    else if (type === "error") { feedbackEl.classList.add("form-feedback--error"); }
  }

  function setDetailFeedback(msg, type) {
    const el = detailFeedbackEl || feedbackEl;
    if (!el) { return; }
    el.textContent = msg || "";
    el.className = "form-feedback";
    if (type === "success") { el.classList.add("form-feedback--success"); }
    else if (type === "error") { el.classList.add("form-feedback--error"); }
  }

  const WEATHER_LABELS = { 0: "なし", 1: "雲", 2: "霧", 3: "雪" };

  function showListSection() {
    if (listSection) { listSection.style.display = ""; }
    if (detailSection) { detailSection.style.display = "none"; }
    state.selectedMapId = null;
  }

  function showDetailSection(mapId) {
    state.selectedMapId = mapId;
    if (listSection) { listSection.style.display = "none"; }
    if (detailSection) { detailSection.style.display = ""; }
    renderForm();
    setDetailFeedback("", null);
  }

  function renderTable() {
    if (!tableBody) { return; }
    tableBody.innerHTML = "";
    if (!state.maps.length) {
      const tr = document.createElement("tr");
      tr.innerHTML = '<td colspan="5" style="text-align:center">マップなし</td>';
      tableBody.appendChild(tr);
      return;
    }
    state.maps.forEach((map) => {
      const tr = document.createElement("tr");
      const weatherLabel = WEATHER_LABELS[map.weatherType] || String(map.weatherType || 0);
      tr.innerHTML =
        `<td>${map.id}</td>` +
        `<td>${map.name || "（名前なし）"}</td>` +
        `<td>${map.bgmId || 0}</td>` +
        `<td>${weatherLabel}</td>` +
        `<td><button type="button" class="btn btn-secondary btn-sm" data-map-edit="${map.id}">編集</button></td>`;
      tableBody.appendChild(tr);
    });
  }

  function renderForm() {
    const map = state.maps.find((m) => m.id === state.selectedMapId);
    if (!map) { return; }
    if (nameInput)     { nameInput.value     = map.name || ""; }
    if (bgmInput)      { bgmInput.value      = typeof map.bgmId === "number" ? map.bgmId : 0; }
    if (weatherSelect) { weatherSelect.value = String(typeof map.weatherType === "number" ? map.weatherType : 0); }
    if (darknessInput) { darknessInput.value = typeof map.darknessLevel === "number" ? map.darknessLevel : 0; }
    if (battleCheck)   { battleCheck.checked  = !!map.battleEnabled; }
    if (recoveryCheck) { recoveryCheck.checked = !!map.recoveryEnabled; }
  }

  async function loadData(forceReload = false) {
    if (state.isLoading) { return; }
    if (!forceReload && state.maps.length && !state.loadError) {
      renderTable();
      return;
    }

    state.isLoading = true;
    state.loadError = null;
    setSummary("マップ情報を読み込み中...");
    setFeedback("", null);

    try {
      const { response, data } = await fetchJson("/api/maps");
      if (!response.ok || !data || !Array.isArray(data.maps)) {
        throw new Error("invalid_response");
      }
      state.maps = data.maps.slice().sort((a, b) => a.id - b.id);
      state.isLoading = false;
      setSummary(`マップ ${state.maps.length} 件`);
      renderTable();
    } catch (err) {
      state.maps = [];
      state.loadError = "マップ情報の取得に失敗しました";
      state.isLoading = false;
      setSummary(state.loadError);
      setFeedback(state.loadError, "error");
      renderTable();
    }
  }

  async function saveMapInfo(event) {
    if (event) { event.preventDefault(); }
    const map = state.maps.find((m) => m.id === state.selectedMapId);
    if (!map) { setDetailFeedback("保存対象のマップが選択されていません", "error"); return; }

    const payload = { id: map.id };
    if (nameInput)     { payload.name         = nameInput.value; }
    if (bgmInput)      { payload.bgmId        = parseInt(bgmInput.value, 10) || 0; }
    if (weatherSelect) { payload.weatherType  = parseInt(weatherSelect.value, 10) || 0; }
    if (darknessInput) {
      const v = parseInt(darknessInput.value, 10);
      payload.darknessLevel = isNaN(v) ? 0 : Math.min(255, Math.max(0, v));
    }
    if (battleCheck)   { payload.battleEnabled   = battleCheck.checked; }
    if (recoveryCheck) { payload.recoveryEnabled = recoveryCheck.checked; }

    setDetailFeedback("保存中...", null);
    try {
      const { response, data } = await fetchJson("/api/maps", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
      });
      if (!response.ok || !data || !data.id) {
        throw new Error((data && data.error) ? data.error : `HTTP ${response.status}`);
      }
      const idx = state.maps.findIndex((m) => m.id === data.id);
      if (idx >= 0) { state.maps[idx] = data; }
      setDetailFeedback("保存しました", "success");
      renderTable();
      // 保存後に一覧に戻る
      setTimeout(() => { showListSection(); }, 600);
    } catch (err) {
      setDetailFeedback(`保存に失敗しました: ${err.message}`, "error");
    }
  }

  function refreshCopySelect() {
    if (!copySelect) { return; }
    copySelect.innerHTML = "";
    const emptyOpt = document.createElement("option");
    emptyOpt.value = "0";
    emptyOpt.textContent = "（空マップを新規作成）";
    copySelect.appendChild(emptyOpt);
    state.maps.forEach((map) => {
      const opt = document.createElement("option");
      opt.value = String(map.id);
      opt.textContent = `[${map.id}] ${map.name || "（名前なし）"}`;
      copySelect.appendChild(opt);
    });
  }

  async function createMap() {
    const copyFromMapId = copySelect ? parseInt(copySelect.value, 10) || 0 : 0;
    setFeedback("作成中...", null);
    try {
      const { response, data } = await fetchJson("/api/maps", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ copyFromMapId })
      });
      if (!response.ok || !data || !data.id) {
        throw new Error((data && data.error) ? data.error : `HTTP ${response.status}`);
      }
      state.maps = [];
      state.loadError = null;
      await loadData(true);
      if (addArea) { addArea.style.display = "none"; }
      setFeedback(`マップ ID[${data.id}] を作成しました`, "success");
      // 作成後、新しいマップの詳細を開く
      showDetailSection(data.id);
    } catch (err) {
      setFeedback(`作成に失敗しました: ${err.message}`, "error");
    }
  }

  // イベント登録

  // テーブルの編集ボタン（委譲）
  if (tableBody) {
    tableBody.addEventListener("click", (ev) => {
      const btn = ev.target.closest("[data-map-edit]");
      if (btn) {
        const id = parseInt(btn.getAttribute("data-map-edit"), 10);
        if (!isNaN(id)) { showDetailSection(id); }
      }
    });
  }

  if (backBtn) { backBtn.addEventListener("click", () => { showListSection(); }); }
  if (cancelBtn) { cancelBtn.addEventListener("click", () => { showListSection(); }); }
  if (editForm) { editForm.addEventListener("submit", saveMapInfo); }
  if (reloadBtn) { reloadBtn.addEventListener("click", () => { loadData(true); }); }
  if (addBtn) {
    addBtn.addEventListener("click", () => {
      if (!addArea) { return; }
      const isVisible = addArea.style.display !== "none";
      if (isVisible) { addArea.style.display = "none"; }
      else { refreshCopySelect(); addArea.style.display = ""; }
    });
  }
  if (addConfirmBtn) { addConfirmBtn.addEventListener("click", createMap); }
  if (addCancelBtn) {
    addCancelBtn.addEventListener("click", () => { if (addArea) { addArea.style.display = "none"; } });
  }

  // 初回ロード
  loadData(false);
}

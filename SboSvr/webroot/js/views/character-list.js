/**
 * views/character-list.js
 * キャラクター一覧画面 (route: character-list)
 *
 * API: GET /api/characters?name=&accountId=&mapId=&isNpc=&limit=20&offset=N
 * レスポンス: { total, limit, offset, characters: [...] }
 *
 * F9 拡張: grpIdMain / grpIdSub から sprite-thumb サムネ列を追加。
 * character-overview へのジャンプは window._charEditMount 連携を維持。
 */

import { fetchJson } from "../core/api.js";
import { createSpriteThumbLazy } from "../components/sprite-thumb.js";

const CHAR_LIST_LIMIT = 20;

// ----------------------------------------------------------------
// escapeHtml
// ----------------------------------------------------------------
function escapeHtml(str) {
  return String(str == null ? "" : str)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card">
        <h2>キャラクター一覧</h2>
        <!-- フィルター入力欄 -->
        <div class="filter-row">
          <label>名前（部分一致）:
            <input type="text" id="char-filter-name" placeholder="キャラ名を入力" />
          </label>
          <label>アカウントID:
            <input type="number" id="char-filter-account-id" placeholder="例: 1001" min="0" />
          </label>
          <label>マップID:
            <input type="number" id="char-filter-map-id" placeholder="例: 1" min="0" />
          </label>
          <label>種別:
            <select id="char-filter-is-npc">
              <option value="">全て</option>
              <option value="0">PC のみ</option>
              <option value="1">NPC のみ</option>
            </select>
          </label>
          <button type="button" id="char-search-btn" class="button">検索</button>
          <button type="button" id="char-reset-btn" class="button secondary">リセット</button>
        </div>
        <p id="char-list-summary" class="result-message"></p>
        <p id="char-list-feedback" class="form-feedback" aria-live="polite"></p>
        <!-- キャラクター一覧テーブル -->
        <table class="data-table" id="char-list-table">
          <thead>
            <tr>
              <th>CharID</th>
              <th>キャラ名</th>
              <th>Lv</th>
              <th>マップID</th>
              <th>座標 (X, Y)</th>
              <th>アカウントID</th>
              <th>NPC</th>
            </tr>
          </thead>
          <tbody id="char-list-table-body"></tbody>
        </table>
        <!-- ページングコントロール -->
        <div class="pagination-row" id="char-pagination">
          <button type="button" id="char-prev-btn" class="button secondary" disabled>前へ</button>
          <span id="char-page-info"></span>
          <button type="button" id="char-next-btn" class="button secondary" disabled>次へ</button>
        </div>
      </section>`;

  const filterName    = container.querySelector("#char-filter-name");
  const filterAccId   = container.querySelector("#char-filter-account-id");
  const filterMapId   = container.querySelector("#char-filter-map-id");
  const filterIsNpc   = container.querySelector("#char-filter-is-npc");
  const searchBtn     = container.querySelector("#char-search-btn");
  const resetBtn      = container.querySelector("#char-reset-btn");
  const summaryEl     = container.querySelector("#char-list-summary");
  const feedbackEl    = container.querySelector("#char-list-feedback");
  const tableEl       = container.querySelector("#char-list-table");
  const tableBody     = container.querySelector("#char-list-table-body");
  const prevBtn       = container.querySelector("#char-prev-btn");
  const nextBtn       = container.querySelector("#char-next-btn");
  const pageInfo      = container.querySelector("#char-page-info");

  // サムネ列をヘッダに追加
  if (tableEl) {
    const thead = tableEl.querySelector("thead tr");
    if (thead && !thead.querySelector("th.th-thumb")) {
      const th = document.createElement("th");
      th.className = "th-thumb";
      th.textContent = "見た目";
      thead.insertBefore(th, thead.firstChild);
    }
  }

  const state = {
    offset: 0,
    total: 0,
    isLoading: false,
  };

  // 前回のサムネリストを保持（destroy 用）
  let _thumbs = [];

  function destroyThumbs() {
    _thumbs.forEach((t) => t.destroy());
    _thumbs = [];
  }

  function updatePagination() {
    const { total, offset } = state;
    if (prevBtn) { prevBtn.disabled = offset <= 0; }
    if (nextBtn) { nextBtn.disabled = offset + CHAR_LIST_LIMIT >= total; }
    if (pageInfo) {
      if (total > 0) {
        const page    = Math.floor(offset / CHAR_LIST_LIMIT) + 1;
        const maxPage = Math.ceil(total / CHAR_LIST_LIMIT);
        pageInfo.textContent = `${page} / ${maxPage} ページ`;
      } else {
        pageInfo.textContent = "";
      }
    }
  }

  function renderList(characters) {
    if (!tableBody) { return; }
    destroyThumbs();
    tableBody.innerHTML = "";

    if (!characters.length) {
      const tr = document.createElement("tr");
      // サムネ列含めて 8 列
      tr.innerHTML = '<td colspan="8">データがありません</td>';
      tableBody.appendChild(tr);
      return;
    }

    const fragment = document.createDocumentFragment();
    characters.forEach((c) => {
      const tr = document.createElement("tr");
      tr.style.cursor = "pointer";

      // サムネ列
      const thumbTd = document.createElement("td");
      thumbTd.className = "td-thumb";
      // grpIdMain (char category key) / grpIdSub から sprite-thumb を生成
      // API が grpIdMain を返す場合のみ表示。返さない場合は空セル。
      if (typeof c.grpIdSub === "number") {
        const categoryKey = typeof c.grpIdMain === "number" ? `char_${c.grpIdMain}` : "char";
        const thumb = createSpriteThumbLazy({ categoryKey, sub: c.grpIdSub, size: 32 });
        _thumbs.push(thumb);
        thumbTd.appendChild(thumb.el);
      }
      tr.appendChild(thumbTd);

      tr.innerHTML +=
        `<td>${escapeHtml(String(c.charId ?? ""))}</td>` +
        `<td>${escapeHtml(String(c.charName ?? ""))}</td>` +
        `<td>${escapeHtml(String(c.level ?? ""))}</td>` +
        `<td>${escapeHtml(String(c.mapId ?? ""))}</td>` +
        `<td>${escapeHtml(String(c.x ?? ""))}, ${escapeHtml(String(c.y ?? ""))}</td>` +
        `<td>${escapeHtml(String(c.accountId ?? ""))}</td>` +
        `<td>${c.isNpc ? "NPC" : "PC"}</td>`;

      tr.addEventListener("click", () => {
        // character-overview (char-edit.js) へ遷移
        window.location.hash = "#character-overview";
        if (typeof window._charEditMount === "function") { window._charEditMount(c.charId); }
      });
      fragment.appendChild(tr);
    });
    tableBody.appendChild(fragment);
  }

  async function load() {
    if (!tableBody || state.isLoading) { return; }
    state.isLoading = true;
    if (feedbackEl) { feedbackEl.textContent = "読み込み中..."; }
    if (tableBody) { tableBody.innerHTML = '<tr><td colspan="8">読み込み中...</td></tr>'; }
    updatePagination();

    const params = new URLSearchParams();
    if (filterName && filterName.value.trim()) { params.set("name", filterName.value.trim()); }
    if (filterAccId && filterAccId.value.trim()) { params.set("accountId", filterAccId.value.trim()); }
    if (filterMapId && filterMapId.value.trim()) { params.set("mapId", filterMapId.value.trim()); }
    if (filterIsNpc && filterIsNpc.value !== "") { params.set("isNpc", filterIsNpc.value); }
    params.set("limit", String(CHAR_LIST_LIMIT));
    params.set("offset", String(state.offset));

    try {
      const { response, data } = await fetchJson("/api/characters?" + params.toString());
      if (!response.ok || !data || !Array.isArray(data.characters)) {
        const msg = (data && data.error) ? data.error : "キャラクター一覧の取得に失敗しました";
        if (feedbackEl) { feedbackEl.textContent = msg; }
        if (tableBody) { tableBody.innerHTML = '<tr><td colspan="8">取得に失敗しました</td></tr>'; }
        state.total = 0;
        updatePagination();
        return;
      }
      state.total = typeof data.total === "number" ? data.total : 0;
      if (feedbackEl) { feedbackEl.textContent = ""; }
      if (summaryEl) {
        summaryEl.textContent = state.total > 0
          ? `${state.total} 件中 ${state.offset + 1}〜${Math.min(state.offset + data.characters.length, state.total)} 件を表示`
          : "該当するキャラクターがありません";
      }
      renderList(data.characters);
      updatePagination();
    } catch {
      if (feedbackEl) { feedbackEl.textContent = "通信エラーが発生しました"; }
      if (tableBody) { tableBody.innerHTML = '<tr><td colspan="8">通信エラーが発生しました</td></tr>'; }
      state.total = 0;
      updatePagination();
    } finally {
      state.isLoading = false;
    }
  }

  // イベント登録
  if (searchBtn) {
    searchBtn.addEventListener("click", () => { state.offset = 0; load(); });
  }
  if (resetBtn) {
    resetBtn.addEventListener("click", () => {
      if (filterName)  { filterName.value  = ""; }
      if (filterAccId) { filterAccId.value = ""; }
      if (filterMapId) { filterMapId.value = ""; }
      if (filterIsNpc) { filterIsNpc.value = ""; }
      state.offset = 0;
      state.total  = 0;
      destroyThumbs();
      if (tableBody)  { tableBody.innerHTML  = ""; }
      if (summaryEl)  { summaryEl.textContent  = ""; }
      if (feedbackEl) { feedbackEl.textContent = ""; }
      updatePagination();
    });
  }
  if (prevBtn) {
    prevBtn.addEventListener("click", () => {
      if (state.offset <= 0) { return; }
      state.offset = Math.max(0, state.offset - CHAR_LIST_LIMIT);
      load();
    });
  }
  if (nextBtn) {
    nextBtn.addEventListener("click", () => {
      if (state.offset + CHAR_LIST_LIMIT >= state.total) { return; }
      state.offset += CHAR_LIST_LIMIT;
      load();
    });
  }

  // 初回ロード
  load();

  return {
    unmount() {
      destroyThumbs();
    }
  };
}

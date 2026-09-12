/**
 * views/character-list.js
 * キャラクター一覧画面 (route: character-list)
 *
 * API: GET /api/characters?name=&accountId=&mapId=&isNpc=&limit=20&offset=N
 * レスポンス: { total, limit, offset, characters: [...] }
 * (CharacterListHandler.cpp:279-305 — name/accountId/mapId/isNpc/limit/offset のみ対応。
 *  sort パラメータはサーバー側に存在しない。並び替えは取得した 1 ページ分の
 *  配列に対してクライアント側で行うに留める)
 *
 * F9 拡張: grpIdMain / grpIdSub から sprite-thumb サムネ列を追加。
 * character-overview へのジャンプは window._charEditMount 連携を維持。
 *
 * 一覧の検索/表示件数/ページ/並び替えは list-toolbar（サーバー側ページングモード。
 * applyToRows は使わず setTotal のみで件数を反映）。状態は q/size/page/sort と
 * 既存フィルタ(accountId/mapId/isNpc)を #character-list?... の URL クエリに保持し、
 * mount 時に復元する。
 */

import { fetchJson } from "../core/api.js";
import { createSpriteThumbLazy } from "../components/sprite-thumb.js";
import { createEntityField } from "../components/entity-picker.js";
import { currentRoute, getRouteParams, setRouteParams } from "../core/router.js";
import { createListToolbar } from "../components/list-toolbar.js";

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

// クライアント側でのみ有効な並び替え（サーバー未対応のため取得ページ内のみ）
const SORTERS = {
  id:    (a, b) => (a.charId ?? 0) - (b.charId ?? 0),
  name:  (a, b) => String(a.charName ?? "").localeCompare(String(b.charName ?? "")),
  level: (a, b) => (a.level ?? 0) - (b.level ?? 0),
  mapId: (a, b) => (a.mapId ?? 0) - (b.mapId ?? 0),
};

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card">
        <h2>キャラクター一覧</h2>
        <!-- フィルター入力欄 -->
        <div class="filter-row">
          <label>アカウントID:
            <input type="number" id="char-filter-account-id" placeholder="例: 1001" min="0" />
          </label>
          <span id="char-filter-map-id-wrap"></span>
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
        <div id="char-list-toolbar"></div>
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
      </section>`;

  const filterAccId   = container.querySelector("#char-filter-account-id");
  const filterMapIdWrap = container.querySelector("#char-filter-map-id-wrap");
  const filterIsNpc   = container.querySelector("#char-filter-is-npc");
  const toolbarHost   = container.querySelector("#char-list-toolbar");

  // URL クエリから初期状態を復元
  const routeParams = getRouteParams();

  // マップID フィルター（map picker。0 のままなら未指定扱い）
  const filterMapIdField = createEntityField({
    type: "map",
    value: Number(routeParams.get("mapId")) || 0,
    label: "マップID:",
    onChange: () => { state.offset = computeOffset(); persistRouteParams(); load(); },
  });
  if (filterMapIdWrap) { filterMapIdWrap.replaceWith(filterMapIdField.element); }

  if (filterAccId) { filterAccId.value = routeParams.get("accountId") || ""; }
  if (filterIsNpc) { filterIsNpc.value = routeParams.get("isNpc") || ""; }

  const searchBtn     = container.querySelector("#char-search-btn");
  const resetBtn      = container.querySelector("#char-reset-btn");
  const summaryEl     = container.querySelector("#char-list-summary");
  const feedbackEl    = container.querySelector("#char-list-feedback");
  const tableEl       = container.querySelector("#char-list-table");
  const tableBody     = container.querySelector("#char-list-table-body");

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

  function computeOffset() {
    const s = toolbar.getState();
    return (s.page - 1) * s.pageSize;
  }

  // 検索/表示件数/ページ/並び替え。サーバー側ページングのため applyToRows は使わず、
  // onChange のたびに API を叩き直す (setTotal でサーバーの総件数を反映)。
  const toolbar = createListToolbar({
    placeholder: "名前で検索",
    sortOptions: [
      { value: "id",    label: "ID順" },
      { value: "name",  label: "名前順" },
      { value: "level", label: "レベル順" },
      { value: "mapId", label: "マップID順" },
    ],
    pageSizes: [20, 50, 100],
    initial: {
      q: routeParams.get("q") || "",
      sort: routeParams.get("sort") || "id",
      pageSize: Number(routeParams.get("size")) || 20,
      page: Number(routeParams.get("page")) || 1,
    },
    onChange: () => {
      state.offset = computeOffset();
      persistRouteParams();
      load();
    },
  });
  if (toolbarHost) { toolbarHost.appendChild(toolbar.element); }
  state.offset = computeOffset();

  function persistRouteParams() {
    const s = toolbar.getState();
    setRouteParams({
      q: s.q || null,
      sort: s.sort && s.sort !== "id" ? s.sort : null,
      size: s.pageSize && s.pageSize !== 20 ? s.pageSize : null,
      page: s.page && s.page !== 1 ? s.page : null,
      accountId: (filterAccId && filterAccId.value.trim()) ? filterAccId.value.trim() : null,
      mapId: filterMapIdField.getValue() > 0 ? filterMapIdField.getValue() : null,
      isNpc: (filterIsNpc && filterIsNpc.value !== "") ? filterIsNpc.value : null,
    });
  }

  // 前回のサムネリストを保持（destroy 用）
  let _thumbs = [];

  function destroyThumbs() {
    _thumbs.forEach((t) => t.destroy());
    _thumbs = [];
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
        // hashchange は非同期発火のため、先に pendingCharId をセットしてから hash を変更する
        if (currentRoute() === "character-overview") {
          // 既に同じ hash の場合は hashchange が発火しないので直接呼び出す
          if (typeof window._charEditMount === "function") { window._charEditMount(c.charId); }
        } else {
          window._charEditPendingCharId = c.charId;
          window.location.hash = "#character-overview";
        }
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

    const s = toolbar.getState();
    const params = new URLSearchParams();
    if (s.q && s.q.trim()) { params.set("name", s.q.trim()); }
    if (filterAccId && filterAccId.value.trim()) { params.set("accountId", filterAccId.value.trim()); }
    const mapIdFilterValue = filterMapIdField.getValue();
    if (mapIdFilterValue > 0) { params.set("mapId", String(mapIdFilterValue)); }
    if (filterIsNpc && filterIsNpc.value !== "") { params.set("isNpc", filterIsNpc.value); }
    params.set("limit", String(s.pageSize));
    params.set("offset", String(state.offset));

    try {
      const { response, data } = await fetchJson("/api/characters?" + params.toString());
      if (!response.ok || !data || !Array.isArray(data.characters)) {
        const msg = (data && data.error) ? data.error : "キャラクター一覧の取得に失敗しました";
        if (feedbackEl) { feedbackEl.textContent = msg; }
        if (tableBody) { tableBody.innerHTML = '<tr><td colspan="8">取得に失敗しました</td></tr>'; }
        state.total = 0;
        toolbar.setTotal(0);
        return;
      }
      state.total = typeof data.total === "number" ? data.total : 0;
      toolbar.setTotal(state.total);
      if (feedbackEl) { feedbackEl.textContent = ""; }
      if (summaryEl) {
        summaryEl.textContent = state.total > 0
          ? `${state.total} 件中 ${state.offset + 1}〜${Math.min(state.offset + data.characters.length, state.total)} 件を表示`
          : "該当するキャラクターがありません";
      }
      // サーバーは sort 未対応のため、取得した 1 ページ分だけをクライアント側で並び替える
      let characters = data.characters.slice();
      const sorter = SORTERS[s.sort];
      if (typeof sorter === "function") { characters.sort(sorter); }
      renderList(characters);
    } catch {
      if (feedbackEl) { feedbackEl.textContent = "通信エラーが発生しました"; }
      if (tableBody) { tableBody.innerHTML = '<tr><td colspan="8">通信エラーが発生しました</td></tr>'; }
      state.total = 0;
      toolbar.setTotal(0);
    } finally {
      state.isLoading = false;
    }
  }

  // イベント登録
  if (searchBtn) {
    searchBtn.addEventListener("click", () => {
      toolbar.setState({ page: 1 });
      state.offset = 0;
      persistRouteParams();
      load();
    });
  }
  if (resetBtn) {
    resetBtn.addEventListener("click", () => {
      if (filterAccId) { filterAccId.value = ""; }
      filterMapIdField.setValue(0);
      if (filterIsNpc) { filterIsNpc.value = ""; }
      toolbar.setState({ q: "", sort: "id", pageSize: 20, page: 1 });
      state.offset = 0;
      state.total  = 0;
      destroyThumbs();
      if (tableBody)  { tableBody.innerHTML  = ""; }
      if (summaryEl)  { summaryEl.textContent  = ""; }
      if (feedbackEl) { feedbackEl.textContent = ""; }
      toolbar.setTotal(0);
      persistRouteParams();
      load();
    });
  }
  if (filterAccId) {
    filterAccId.addEventListener("keydown", (ev) => {
      if (ev.key === "Enter") { searchBtn && searchBtn.click(); }
    });
  }
  if (filterIsNpc) {
    filterIsNpc.addEventListener("change", () => { searchBtn && searchBtn.click(); });
  }

  // 初回ロード
  load();

  return {
    unmount() {
      destroyThumbs();
    }
  };
}

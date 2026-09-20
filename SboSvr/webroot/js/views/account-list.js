/**
 * views/account-list.js
 * アカウント一覧画面 (route: account-list)
 *
 * API: GET /api/accounts?search=&lastLoginBefore=&lastLoginAfter=&
 *      neverLoggedIn=&createdBefore=&createdAfter=&disabled=&admin=&online=&
 *      minChars=&maxChars=&sort=&offset=&limit=
 * search はアカウントID完全一致 / アカウント名部分一致 / 所持キャラ名部分一致 の
 * いずれかにヒットするフリーワード検索(旧 q + アカウントID欄を統合したもの)。
 * レスポンス: { total, offset, limit, items: [...] }
 * (docs/web-admin-account-management-plan.md の S1 節を参照。
 *  API はこの画面と並行実装中のため、まだ動かない前提で実装している)
 *
 * 絞り込み・ソート・ページングはすべてサーバー側で行う(list-toolbar は
 * サーバー側ページングモード。applyToRows は使わず setTotal のみで件数を反映)。
 * 状態は q/size/page/sort と個別フィルタを #account-list?... の URL クエリに
 * 保持し、mount 時に復元する(character-list.js と同じ作法)。
 *
 * 各行の「ロール設定」ボタンで「管理者ロール設定」へアカウントIDを引き継ぐ。role-management.js は
 * アカウントID手入力の作りなので、router.js の setRouteParams と同じ仕組みで
 * #role-management?accountId=12 の形にして遷移する(role-management.js 側で
 * getRouteParams から初期値を読む)。
 */

import { fetchJson } from "../core/api.js";
import { getRouteParams, setRouteParams } from "../core/router.js";
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

// UNIX 秒 → 日本語ローカル表示。未ログイン等は 0/null 想定なので "-" にする。
function formatUnixSeconds(sec) {
  if (!sec) { return "-"; }
  const n = Number(sec);
  if (!Number.isFinite(n) || n <= 0) { return "-"; }
  return new Date(n * 1000).toLocaleString("ja-JP");
}

// 所持キャラ名一覧を表示用に整形する。長い場合は先頭3件のみ表示し「他N件」を付ける。
// 全件は title 属性(ツールチップ)で確認できるようにする。
function formatCharNames(charNames) {
  const names = Array.isArray(charNames) ? charNames.filter((n) => n) : [];
  if (!names.length) { return { text: "-", title: "" }; }
  const MAX_SHOW = 3;
  const shown = names.slice(0, MAX_SHOW).join("、");
  const text = names.length > MAX_SHOW ? `${shown} 他${names.length - MAX_SHOW}件` : shown;
  return { text, title: names.join("、") };
}

// よく使う条件のプリセット。適用時に他のフィルタはリセットする。
const PRESETS = {
  inactive90: {
    label: "90日以上ログイン無し",
    apply(fields) {
      const before = Math.floor(Date.now() / 1000) - 90 * 24 * 60 * 60;
      fields.lastLoginBefore.value = String(before);
      fields.neverLoggedIn.checked = false;
    },
  },
  neverLoggedIn: {
    label: "一度もログイン無し",
    apply(fields) {
      fields.lastLoginBefore.value = "";
      fields.neverLoggedIn.checked = true;
    },
  },
  zeroChars: {
    label: "キャラ0件",
    apply(fields) {
      fields.minChars.value = "0";
      fields.maxChars.value = "0";
    },
  },
};

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card">
        <h2>アカウント一覧</h2>
        <!-- フィルター入力欄 -->
        <div class="filter-row">
          <label>状態:
            <select id="acct-filter-disabled">
              <option value="">すべて</option>
              <option value="0">通常</option>
              <option value="1">ログイン拒否</option>
            </select>
          </label>
          <label><input type="checkbox" id="acct-filter-admin" /> 管理者のみ</label>
          <label><input type="checkbox" id="acct-filter-online" /> 接続中のみ</label>
          <label><input type="checkbox" id="acct-filter-never-login" /> 一度もログイン無し</label>
          <label>最終ログイン(指定日以前):
            <input type="date" id="acct-filter-last-login-before" />
          </label>
          <label>キャラ数:
            <input type="number" id="acct-filter-min-chars" placeholder="下限" min="0" style="width:5em" />
            〜
            <input type="number" id="acct-filter-max-chars" placeholder="上限" min="0" style="width:5em" />
          </label>
          <button type="button" id="acct-search-btn" class="button">検索</button>
          <button type="button" id="acct-reset-btn" class="button secondary">リセット</button>
        </div>
        <div class="filter-row" id="acct-preset-row">
          <span>よく使う条件:</span>
          <button type="button" class="button secondary" data-preset="inactive90">90日以上ログイン無し</button>
          <button type="button" class="button secondary" data-preset="neverLoggedIn">一度もログイン無し</button>
          <button type="button" class="button secondary" data-preset="zeroChars">キャラ0件</button>
        </div>
        <div id="acct-list-toolbar"></div>
        <p id="acct-list-summary" class="result-message"></p>
        <p id="acct-list-feedback" class="form-feedback" aria-live="polite"></p>
        <!-- アカウント一覧テーブル -->
        <table class="data-table" id="acct-list-table">
          <thead>
            <tr>
              <th>ID</th>
              <th>名前</th>
              <th>状態</th>
              <th>キャラ数</th>
              <th>キャラ</th>
              <th>作成日</th>
              <th>最終ログイン</th>
              <th>ログイン回数</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody id="acct-list-table-body"></tbody>
        </table>
      </section>`;

  const filterDisabled    = container.querySelector("#acct-filter-disabled");
  const filterAdmin       = container.querySelector("#acct-filter-admin");
  const filterOnline      = container.querySelector("#acct-filter-online");
  const filterNeverLogin  = container.querySelector("#acct-filter-never-login");
  const filterLoginBefore = container.querySelector("#acct-filter-last-login-before");
  const filterMinChars    = container.querySelector("#acct-filter-min-chars");
  const filterMaxChars    = container.querySelector("#acct-filter-max-chars");
  const toolbarHost       = container.querySelector("#acct-list-toolbar");
  const presetRow         = container.querySelector("#acct-preset-row");

  // URL クエリから初期状態を復元
  const routeParams = getRouteParams();

  if (filterDisabled)   { filterDisabled.value = routeParams.get("disabled") || ""; }
  if (filterAdmin)      { filterAdmin.checked = routeParams.get("admin") === "1"; }
  if (filterOnline)     { filterOnline.checked = routeParams.get("online") === "1"; }
  if (filterNeverLogin) { filterNeverLogin.checked = routeParams.get("neverLoggedIn") === "1"; }
  if (filterLoginBefore && routeParams.get("lastLoginBefore")) {
    const sec = Number(routeParams.get("lastLoginBefore"));
    if (Number.isFinite(sec) && sec > 0) {
      filterLoginBefore.value = new Date(sec * 1000).toISOString().slice(0, 10);
    }
  }
  if (filterMinChars) { filterMinChars.value = routeParams.get("minChars") || ""; }
  if (filterMaxChars) { filterMaxChars.value = routeParams.get("maxChars") || ""; }

  const searchBtn  = container.querySelector("#acct-search-btn");
  const resetBtn   = container.querySelector("#acct-reset-btn");
  const summaryEl  = container.querySelector("#acct-list-summary");
  const feedbackEl = container.querySelector("#acct-list-feedback");
  const tableBody  = container.querySelector("#acct-list-table-body");

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
    placeholder: "アカウントID / アカウント名 / キャラ名",
    sortOptions: [
      { value: "accountId",  label: "ID順" },
      { value: "account",    label: "名前順" },
      { value: "lastLogin",  label: "最終ログイン順" },
      { value: "created",    label: "作成日順" },
      { value: "loginCount", label: "ログイン回数順" },
      { value: "charCount",  label: "キャラ数順" },
    ],
    pageSizes: [20, 50, 100],
    initial: {
      q: routeParams.get("search") || "",
      sort: routeParams.get("sort") || "accountId",
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

  // date input(ローカル日付) → UNIX 秒(その日の 00:00 として扱う)
  function loginBeforeSeconds() {
    if (!filterLoginBefore || !filterLoginBefore.value) { return null; }
    const t = new Date(filterLoginBefore.value + "T00:00:00").getTime();
    return Number.isFinite(t) ? Math.floor(t / 1000) : null;
  }

  function persistRouteParams() {
    const s = toolbar.getState();
    setRouteParams({
      search: s.q || null,
      sort: s.sort && s.sort !== "accountId" ? s.sort : null,
      size: s.pageSize && s.pageSize !== 20 ? s.pageSize : null,
      page: s.page && s.page !== 1 ? s.page : null,
      disabled: (filterDisabled && filterDisabled.value !== "") ? filterDisabled.value : null,
      admin: (filterAdmin && filterAdmin.checked) ? "1" : null,
      online: (filterOnline && filterOnline.checked) ? "1" : null,
      neverLoggedIn: (filterNeverLogin && filterNeverLogin.checked) ? "1" : null,
      lastLoginBefore: loginBeforeSeconds(),
      minChars: (filterMinChars && filterMinChars.value !== "") ? filterMinChars.value : null,
      maxChars: (filterMaxChars && filterMaxChars.value !== "") ? filterMaxChars.value : null,
    });
  }

  function renderList(items) {
    if (!tableBody) { return; }
    tableBody.innerHTML = "";

    if (!items.length) {
      const tr = document.createElement("tr");
      tr.innerHTML = '<td colspan="9">データがありません</td>';
      tableBody.appendChild(tr);
      return;
    }

    const fragment = document.createDocumentFragment();
    items.forEach((a) => {
      const tr = document.createElement("tr");

      const statusParts = [];
      statusParts.push(a.disabled ? "ログイン拒否" : "通常");
      if (a.online) { statusParts.push('<span class="badge">接続中</span>'); }
      if (a.adminLevel) { statusParts.push(`<span class="badge">管理者Lv${escapeHtml(String(a.adminLevel))}</span>`); }

      const charNamesInfo = formatCharNames(a.charNames);

      tr.innerHTML =
        `<td>${escapeHtml(String(a.accountId ?? ""))}</td>` +
        `<td>${escapeHtml(String(a.account ?? ""))}</td>` +
        `<td>${statusParts.join(" ")}</td>` +
        `<td>${escapeHtml(String(a.charCount ?? ""))}</td>` +
        `<td title="${escapeHtml(charNamesInfo.title)}">${escapeHtml(charNamesInfo.text)}</td>` +
        `<td>${escapeHtml(formatUnixSeconds(a.timeMakeAccount))}</td>` +
        `<td>${escapeHtml(formatUnixSeconds(a.timeLastLogin))}</td>` +
        `<td>${escapeHtml(String(a.loginCount ?? ""))}</td>` +
        `<td><button type="button" class="button secondary" data-role-btn="${escapeHtml(String(a.accountId))}">ロール設定</button></td>`;

      const roleBtn = tr.querySelector("[data-role-btn]");
      if (roleBtn) {
        roleBtn.addEventListener("click", () => {
          // 「管理者ロール設定」へアカウントIDを引き継いで遷移
          // (role-management.js のアカウントID手入力を解消する)
          window.location.hash = `#role-management?accountId=${encodeURIComponent(a.accountId)}`;
        });
      }
      fragment.appendChild(tr);
    });
    tableBody.appendChild(fragment);
  }

  async function load() {
    if (!tableBody || state.isLoading) { return; }
    state.isLoading = true;
    if (feedbackEl) { feedbackEl.textContent = "読み込み中..."; }
    if (tableBody) { tableBody.innerHTML = '<tr><td colspan="9">読み込み中...</td></tr>'; }

    const s = toolbar.getState();
    const params = new URLSearchParams();
    if (s.q && s.q.trim()) { params.set("search", s.q.trim()); }
    if (filterDisabled && filterDisabled.value !== "") { params.set("disabled", filterDisabled.value); }
    if (filterAdmin && filterAdmin.checked) { params.set("admin", "1"); }
    if (filterOnline && filterOnline.checked) { params.set("online", "1"); }
    if (filterNeverLogin && filterNeverLogin.checked) { params.set("neverLoggedIn", "1"); }
    const beforeSec = loginBeforeSeconds();
    if (beforeSec != null) { params.set("lastLoginBefore", String(beforeSec)); }
    if (filterMinChars && filterMinChars.value !== "") { params.set("minChars", filterMinChars.value); }
    if (filterMaxChars && filterMaxChars.value !== "") { params.set("maxChars", filterMaxChars.value); }
    params.set("sort", s.sort);
    params.set("limit", String(s.pageSize));
    params.set("offset", String(state.offset));

    try {
      const { response, data } = await fetchJson("/api/accounts?" + params.toString());
      if (!response.ok || !data || !Array.isArray(data.items)) {
        const msg = (data && data.error) ? data.error : "アカウント一覧の取得に失敗しました";
        if (feedbackEl) { feedbackEl.textContent = msg; }
        if (tableBody) { tableBody.innerHTML = '<tr><td colspan="9">取得に失敗しました</td></tr>'; }
        state.total = 0;
        toolbar.setTotal(0);
        return;
      }
      state.total = typeof data.total === "number" ? data.total : 0;
      toolbar.setTotal(state.total);
      if (feedbackEl) { feedbackEl.textContent = ""; }
      if (summaryEl) {
        summaryEl.textContent = state.total > 0
          ? `${state.total} 件中 ${state.offset + 1}〜${Math.min(state.offset + data.items.length, state.total)} 件を表示`
          : "該当するアカウントがありません";
      }
      renderList(data.items);
    } catch {
      if (feedbackEl) { feedbackEl.textContent = "通信エラーが発生しました"; }
      if (tableBody) { tableBody.innerHTML = '<tr><td colspan="9">通信エラーが発生しました</td></tr>'; }
      state.total = 0;
      toolbar.setTotal(0);
    } finally {
      state.isLoading = false;
    }
  }

  function doSearch() {
    toolbar.setState({ page: 1 });
    state.offset = 0;
    persistRouteParams();
    load();
  }

  // イベント登録
  if (searchBtn) { searchBtn.addEventListener("click", doSearch); }
  if (resetBtn) {
    resetBtn.addEventListener("click", () => {
      if (filterDisabled)   { filterDisabled.value = ""; }
      if (filterAdmin)      { filterAdmin.checked = false; }
      if (filterOnline)     { filterOnline.checked = false; }
      if (filterNeverLogin) { filterNeverLogin.checked = false; }
      if (filterLoginBefore){ filterLoginBefore.value = ""; }
      if (filterMinChars)   { filterMinChars.value = ""; }
      if (filterMaxChars)   { filterMaxChars.value = ""; }
      toolbar.setState({ q: "", sort: "accountId", pageSize: 20, page: 1 });
      state.offset = 0;
      state.total  = 0;
      if (tableBody)  { tableBody.innerHTML  = ""; }
      if (summaryEl)  { summaryEl.textContent  = ""; }
      if (feedbackEl) { feedbackEl.textContent = ""; }
      toolbar.setTotal(0);
      persistRouteParams();
      load();
    });
  }
  [filterDisabled, filterAdmin, filterOnline, filterNeverLogin, filterLoginBefore].forEach((el) => {
    if (el) { el.addEventListener("change", doSearch); }
  });

  // プリセットボタン
  if (presetRow) {
    presetRow.querySelectorAll("button[data-preset]").forEach((btn) => {
      btn.addEventListener("click", () => {
        const preset = PRESETS[btn.dataset.preset];
        if (!preset) { return; }
        if (filterDisabled)   { filterDisabled.value = ""; }
        if (filterAdmin)      { filterAdmin.checked = false; }
        if (filterOnline)     { filterOnline.checked = false; }
        if (filterNeverLogin) { filterNeverLogin.checked = false; }
        if (filterLoginBefore){ filterLoginBefore.value = ""; }
        if (filterMinChars)   { filterMinChars.value = ""; }
        if (filterMaxChars)   { filterMaxChars.value = ""; }
        preset.apply({
          lastLoginBefore: filterLoginBefore,
          neverLoggedIn: filterNeverLogin,
          minChars: filterMinChars,
          maxChars: filterMaxChars,
        });
        doSearch();
      });
    });
  }

  // 初回ロード
  load();

  return {
    unmount() {}
  };
}

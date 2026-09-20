/**
 * views/account-list.js
 * アカウント一覧画面 (route: account-list)
 *
 * API: GET /api/accounts?search=&lastLoginBefore=&lastLoginAfter=&
 *      neverLoggedIn=&createdBefore=&createdAfter=&disabled=&admin=&online=&
 *      minChars=&maxChars=&sort=&offset=&limit=&status=
 * search はアカウントID完全一致 / アカウント名部分一致 / 所持キャラ名部分一致 の
 * いずれかにヒットするフリーワード検索(旧 q + アカウントID欄を統合したもの)。
 * status: 未指定ならゴミ箱(status=trashed)を除外。"trashed" でゴミ箱のみ、
 *   "banned" で BAN 中のみ、"all" で全部。既定(未指定)ではゴミ箱を除くだけで
 *   BAN 中のアカウントは通常の一覧に出る。
 *   ゴミ箱/BAN の項目は理由・日時を伴うが、フィールド名は C++ 側で整理され得るため
 *   item.statusReason ?? item.trashReason / item.statusChangedAt ?? item.trashedAt の
 *   ように両対応で読む。
 * レスポンス: { total, offset, limit, items: [...] }
 * 各 item には disabled/online/adminLevel に加えて status("active"/"trashed"/"banned")
 * が付く。
 * (docs/web-admin-account-management-plan.md の S1 節を参照。
 *  ゴミ箱/完全削除/BAN の API はこの画面と並行実装中のため、まだ動かない前提で実装している)
 *
 * 絞り込み・ソート・ページングはすべてサーバー側で行う(list-toolbar は
 * サーバー側ページングモード。applyToRows は使わず setTotal のみで件数を反映)。
 * 状態は q/size/page/sort と個別フィルタを #account-list?... の URL クエリに
 * 保持し、mount 時に復元する(character-list.js と同じ作法)。
 *
 * ゴミ箱/完全削除/BAN:
 *   POST   /api/accounts/{id}/trash    body {reason?} → 204
 *   DELETE /api/accounts/{id}/trash    ゴミ箱から戻す → 204
 *   DELETE /api/accounts/{id}          完全削除 → 204 (失敗時 409/403)
 *   POST   /api/accounts/{id}/ban      body {reason?} → 204 (既にゴミ箱なら 409 already_trashed)
 *   DELETE /api/accounts/{id}/ban      BAN 解除 → 204 (BAN 中でなければ 409 not_banned)
 *   一覧先頭にチェックボックス列を追加し、選択した複数件へまとめて適用する。
 *   確認は confirm()/prompt() を使わず、core/dom.js の armConfirmButton
 *   (二度押し確定)で画面内完結させる。編集ペインを別ウィンドウへポップアウト
 *   している間は confirm() が背後に隠れて「押しても何も起きない」既知の罠がある為。
 *   完全削除はゴミ箱送り/BAN より一段強い確認文言(対象件数を明示)にしている。
 */

import { fetchJson } from "../core/api.js";
import { getRouteParams, setRouteParams } from "../core/router.js";
import { createListToolbar } from "../components/list-toolbar.js";
import { armConfirmButton } from "../core/dom.js";
import { showToast } from "../components/toast.js";

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
          <label>表示:
            <select id="acct-filter-status">
              <option value="">通常(ゴミ箱を除く)</option>
              <option value="trashed">ゴミ箱</option>
              <option value="banned">BAN中</option>
              <option value="all">すべて(ゴミ箱含む)</option>
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
        <!-- 選択操作(ゴミ箱送り/復元/完全削除)ツールバー -->
        <div class="filter-row" id="acct-bulk-toolbar">
          <span id="acct-bulk-count">0 件選択中</span>
          <span id="acct-bulk-normal-actions">
            <input type="text" id="acct-bulk-reason" placeholder="理由（任意・ゴミ箱/BAN共通）" style="width:18em" />
            <button type="button" id="acct-bulk-trash-btn" class="button secondary" disabled>選択したアカウントをゴミ箱へ</button>
            <button type="button" id="acct-bulk-ban-btn" class="button danger" disabled>選択したアカウントをBAN</button>
            <button type="button" id="acct-bulk-unban-btn" class="button secondary" disabled>選択したアカウントのBANを解除</button>
          </span>
          <span id="acct-bulk-trash-actions">
            <button type="button" id="acct-bulk-restore-btn" class="button secondary" disabled>選択したアカウントを元に戻す</button>
            <button type="button" id="acct-bulk-delete-btn" class="button danger" disabled>選択したアカウントを完全に削除</button>
          </span>
        </div>
        <p id="acct-list-summary" class="result-message"></p>
        <p id="acct-list-feedback" class="form-feedback" aria-live="polite"></p>
        <!-- アカウント一覧テーブル -->
        <table class="data-table" id="acct-list-table">
          <thead>
            <tr id="acct-list-thead-row"></tr>
          </thead>
          <tbody id="acct-list-table-body"></tbody>
        </table>
      </section>`;

  const filterDisabled    = container.querySelector("#acct-filter-disabled");
  const filterStatus      = container.querySelector("#acct-filter-status");
  const filterAdmin       = container.querySelector("#acct-filter-admin");
  const filterOnline      = container.querySelector("#acct-filter-online");
  const filterNeverLogin  = container.querySelector("#acct-filter-never-login");
  const filterLoginBefore = container.querySelector("#acct-filter-last-login-before");
  const filterMinChars    = container.querySelector("#acct-filter-min-chars");
  const filterMaxChars    = container.querySelector("#acct-filter-max-chars");
  const toolbarHost       = container.querySelector("#acct-list-toolbar");
  const presetRow         = container.querySelector("#acct-preset-row");
  const theadRow          = container.querySelector("#acct-list-thead-row");

  // 選択操作ツールバーの要素
  const bulkCountEl          = container.querySelector("#acct-bulk-count");
  const bulkNormalActionsEl  = container.querySelector("#acct-bulk-normal-actions");
  const bulkTrashActionsEl   = container.querySelector("#acct-bulk-trash-actions");
  const bulkReasonInput      = container.querySelector("#acct-bulk-reason");
  const bulkTrashBtn         = container.querySelector("#acct-bulk-trash-btn");
  const bulkBanBtn           = container.querySelector("#acct-bulk-ban-btn");
  const bulkUnbanBtn         = container.querySelector("#acct-bulk-unban-btn");
  const bulkRestoreBtn       = container.querySelector("#acct-bulk-restore-btn");
  const bulkDeleteBtn        = container.querySelector("#acct-bulk-delete-btn");

  // URL クエリから初期状態を復元
  const routeParams = getRouteParams();

  if (filterDisabled)   { filterDisabled.value = routeParams.get("disabled") || ""; }
  if (filterStatus)     { filterStatus.value = routeParams.get("status") || ""; }
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

  // 選択中のアカウントID(現在表示中の1ページ分のみ対象。再読み込みでクリアする)
  const selectedIds = new Set();

  function isTrashMode() {
    return !!filterStatus && filterStatus.value === "trashed";
  }

  function isBanMode() {
    return !!filterStatus && filterStatus.value === "banned";
  }

  // ゴミ箱/BAN のどちらかを絞り込み表示している間は理由・日時列を出す
  // (ゴミ箱理由/BAN理由はサーバー側で reason / statusChangedAt に整理され得るため
  // 両方のフィールド名にフォールバックして読む)。
  function showReasonColumns() {
    return isTrashMode() || isBanMode();
  }

  function columnCount() {
    // チェックボックス + ID/名前/状態/キャラ数/キャラ/作成日/最終ログイン/ログイン回数 + 操作
    return 1 + 8 + (showReasonColumns() ? 2 : 0) + 1;
  }

  // ゴミ箱/BAN 表示中かどうかで見出し列とツールバーの見た目を切り替える
  function renderThead() {
    const showReason = showReasonColumns();
    theadRow.innerHTML =
      `<th><input type="checkbox" id="acct-select-all" /></th>` +
      `<th>ID</th>` +
      `<th>名前</th>` +
      `<th>状態</th>` +
      `<th>キャラ数</th>` +
      `<th>キャラ</th>` +
      `<th>作成日</th>` +
      `<th>最終ログイン</th>` +
      `<th>ログイン回数</th>` +
      (showReason ? `<th>理由</th><th>状態変更日時</th>` : "") +
      `<th>操作</th>`;

    const selectAllCb = theadRow.querySelector("#acct-select-all");
    if (selectAllCb) {
      selectAllCb.addEventListener("change", () => {
        const rowChecks = tableBody.querySelectorAll("[data-acct-checkbox]");
        rowChecks.forEach((cb) => {
          cb.checked = selectAllCb.checked;
          const id = Number(cb.dataset.acctCheckbox);
          if (selectAllCb.checked) { selectedIds.add(id); } else { selectedIds.delete(id); }
        });
        updateBulkUi();
      });
    }
  }

  function updateBulkModeVisibility() {
    const trash = isTrashMode();
    if (bulkNormalActionsEl) { bulkNormalActionsEl.style.display = trash ? "none" : ""; }
    if (bulkTrashActionsEl)  { bulkTrashActionsEl.style.display  = trash ? "" : "none"; }
  }

  function clearSelection() {
    selectedIds.clear();
    updateBulkUi();
  }

  function updateBulkUi() {
    if (bulkCountEl) { bulkCountEl.textContent = `${selectedIds.size} 件選択中`; }
    const hasSelection = selectedIds.size > 0;
    if (bulkTrashBtn)   { bulkTrashBtn.disabled   = !hasSelection; }
    if (bulkBanBtn)     { bulkBanBtn.disabled     = !hasSelection; }
    if (bulkUnbanBtn)   { bulkUnbanBtn.disabled   = !hasSelection; }
    if (bulkRestoreBtn) { bulkRestoreBtn.disabled = !hasSelection; }
    if (bulkDeleteBtn)  { bulkDeleteBtn.disabled  = !hasSelection; }
    // 全選択チェックボックスの状態を現在表示中の行に合わせる
    const selectAllCb = theadRow.querySelector("#acct-select-all");
    if (selectAllCb) {
      const rowChecks = Array.from(tableBody.querySelectorAll("[data-acct-checkbox]"));
      selectAllCb.checked = rowChecks.length > 0 && rowChecks.every((cb) => cb.checked);
    }
  }

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
      status: (filterStatus && filterStatus.value !== "") ? filterStatus.value : null,
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
    const showReason = showReasonColumns();
    const colCount = columnCount();

    if (!items.length) {
      const tr = document.createElement("tr");
      tr.innerHTML = `<td colspan="${colCount}">データがありません</td>`;
      tableBody.appendChild(tr);
      updateBulkUi();
      return;
    }

    const fragment = document.createDocumentFragment();
    items.forEach((a) => {
      const tr = document.createElement("tr");

      const statusParts = [];
      statusParts.push(a.disabled ? "ログイン拒否" : "通常");
      if (a.online) { statusParts.push('<span class="badge">接続中</span>'); }
      if (a.adminLevel) { statusParts.push(`<span class="badge">管理者Lv${escapeHtml(String(a.adminLevel))}</span>`); }
      if (a.status === "trashed") { statusParts.push('<span class="badge">ゴミ箱</span>'); }
      if (a.status === "banned")  { statusParts.push('<span class="badge">BAN</span>'); }

      const charNamesInfo = formatCharNames(a.charNames);

      tr.innerHTML =
        `<td><input type="checkbox" data-acct-checkbox="${escapeHtml(String(a.accountId))}" /></td>` +
        `<td>${escapeHtml(String(a.accountId ?? ""))}</td>` +
        `<td>${escapeHtml(String(a.account ?? ""))}</td>` +
        `<td>${statusParts.join(" ")}</td>` +
        `<td>${escapeHtml(String(a.charCount ?? ""))}</td>` +
        `<td title="${escapeHtml(charNamesInfo.title)}">${escapeHtml(charNamesInfo.text)}</td>` +
        `<td>${escapeHtml(formatUnixSeconds(a.timeMakeAccount))}</td>` +
        `<td>${escapeHtml(formatUnixSeconds(a.timeLastLogin))}</td>` +
        `<td>${escapeHtml(String(a.loginCount ?? ""))}</td>` +
        (showReason
          ? `<td>${escapeHtml((a.statusReason ?? a.reason ?? a.trashReason) || "-")}</td><td>${escapeHtml(formatUnixSeconds(a.statusChangedAt ?? a.trashedAt))}</td>`
          : "") +
        `<td><button type="button" class="button secondary" data-role-btn="${escapeHtml(String(a.accountId))}">ロール設定</button></td>`;

      const checkbox = tr.querySelector("[data-acct-checkbox]");
      if (checkbox) {
        checkbox.addEventListener("change", () => {
          const id = Number(checkbox.dataset.acctCheckbox);
          if (checkbox.checked) { selectedIds.add(id); } else { selectedIds.delete(id); }
          updateBulkUi();
        });
      }

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
    updateBulkUi();
  }

  async function load() {
    if (!tableBody || state.isLoading) { return; }
    state.isLoading = true;
    clearSelection();
    renderThead();
    updateBulkModeVisibility();
    const colCount = columnCount();
    if (feedbackEl) { feedbackEl.textContent = "読み込み中..."; }
    if (tableBody) { tableBody.innerHTML = `<tr><td colspan="${colCount}">読み込み中...</td></tr>`; }

    const s = toolbar.getState();
    const params = new URLSearchParams();
    if (s.q && s.q.trim()) { params.set("search", s.q.trim()); }
    if (filterDisabled && filterDisabled.value !== "") { params.set("disabled", filterDisabled.value); }
    if (filterStatus && filterStatus.value !== "") { params.set("status", filterStatus.value); }
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
        if (tableBody) { tableBody.innerHTML = `<tr><td colspan="${colCount}">取得に失敗しました</td></tr>`; }
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
      if (tableBody) { tableBody.innerHTML = `<tr><td colspan="${colCount}">通信エラーが発生しました</td></tr>`; }
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

  // ----------------------------------------------------------------
  // 選択操作(ゴミ箱送り/復元/完全削除)
  // ----------------------------------------------------------------

  // 現在テーブルに描画されているアカウントIDの集合。
  function getVisibleAccountIds() {
    return new Set(
      Array.from(tableBody.querySelectorAll("[data-acct-checkbox]"))
        .map((cb) => Number(cb.dataset.acctCheckbox))
    );
  }

  // 一括操作の実行直前の保険: 選択中のIDのうち、現在画面に表示されている行の
  // IDだけを対象にする。再読み込みで選択が残ってしまった場合でも、
  // 表示されていないアカウント(別の絞り込み結果)には絶対に操作を飛ばさない。
  function getSafeSelectedIds() {
    const visible = getVisibleAccountIds();
    return Array.from(selectedIds).filter((id) => visible.has(id));
  }

  // 選択中の各IDに順番にリクエストを送り、成功/失敗件数をまとめる。
  async function runBulk(ids, requestFn) {
    const failures = [];
    let successCount = 0;
    for (const id of ids) {
      try {
        const { response, data } = await requestFn(id);
        if (response.ok) {
          successCount++;
        } else {
          const reason = (data && data.error) ? data.error : `HTTP ${response.status}`;
          failures.push({ id, reason });
        }
      } catch {
        failures.push({ id, reason: "通信エラー" });
      }
    }
    return { successCount, failures };
  }

  function reportBulkResult(actionLabel, successCount, failures) {
    const total = successCount + failures.length;
    if (!failures.length) {
      showToast(`${actionLabel}: ${successCount}/${total} 件成功`, "success");
      return;
    }
    const detail = failures.map((f) => `ID ${f.id}: ${f.reason}`).join("\n");
    showToast(`${actionLabel}: ${successCount}/${total} 件成功（${failures.length} 件失敗）`, "error", { detail });
  }

  async function executeBulkTrash() {
    const ids = getSafeSelectedIds();
    if (!ids.length) { return; }
    const reason = bulkReasonInput ? bulkReasonInput.value.trim() : "";
    const { successCount, failures } = await runBulk(ids, (id) => fetchJson(`/api/accounts/${id}/trash`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ reason: reason || undefined }),
    }));
    reportBulkResult("ゴミ箱へ移動", successCount, failures);
    if (bulkReasonInput) { bulkReasonInput.value = ""; }
    load();
  }

  async function executeBulkBan() {
    const ids = getSafeSelectedIds();
    if (!ids.length) { return; }
    const reason = bulkReasonInput ? bulkReasonInput.value.trim() : "";
    const { successCount, failures } = await runBulk(ids, (id) => fetchJson(`/api/accounts/${id}/ban`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ reason: reason || undefined }),
    }));
    reportBulkResult("BAN", successCount, failures);
    if (bulkReasonInput) { bulkReasonInput.value = ""; }
    load();
  }

  async function executeBulkUnban() {
    const ids = getSafeSelectedIds();
    if (!ids.length) { return; }
    const { successCount, failures } = await runBulk(ids, (id) => fetchJson(`/api/accounts/${id}/ban`, {
      method: "DELETE",
    }));
    reportBulkResult("BAN解除", successCount, failures);
    load();
  }

  async function executeBulkRestore() {
    const ids = getSafeSelectedIds();
    if (!ids.length) { return; }
    const { successCount, failures } = await runBulk(ids, (id) => fetchJson(`/api/accounts/${id}/trash`, {
      method: "DELETE",
    }));
    reportBulkResult("復元", successCount, failures);
    load();
  }

  async function executeBulkDelete() {
    const ids = getSafeSelectedIds();
    if (!ids.length) { return; }
    const { successCount, failures } = await runBulk(ids, (id) => fetchJson(`/api/accounts/${id}`, {
      method: "DELETE",
    }));
    reportBulkResult("完全削除", successCount, failures);
    load();
  }

  // 確認は confirm()/prompt() を使わず二度押し確定(armConfirmButton)で完結させる。
  // 完全削除はゴミ箱送りより一段強い確認文言(対象件数を明示)にする。
  if (bulkTrashBtn) {
    armConfirmButton(bulkTrashBtn, {
      armedLabel: "本当に実行？（もう一度押す）",
      message: () => `${selectedIds.size} 件をゴミ箱へ移動します。`,
      onConfirm: executeBulkTrash,
    });
  }
  if (bulkBanBtn) {
    armConfirmButton(bulkBanBtn, {
      armedLabel: "本当にBAN？（もう一度押す）",
      message: () => `${selectedIds.size} 件のアカウントをBANします。`,
      onConfirm: executeBulkBan,
    });
  }
  if (bulkUnbanBtn) {
    armConfirmButton(bulkUnbanBtn, {
      armedLabel: "本当にBAN解除？（もう一度押す）",
      message: () => `${selectedIds.size} 件のアカウントのBANを解除します。`,
      onConfirm: executeBulkUnban,
    });
  }
  if (bulkRestoreBtn) {
    armConfirmButton(bulkRestoreBtn, {
      armedLabel: "本当に元に戻す？（もう一度押す）",
      message: () => `${selectedIds.size} 件をゴミ箱から元に戻します。`,
      onConfirm: executeBulkRestore,
    });
  }
  if (bulkDeleteBtn) {
    armConfirmButton(bulkDeleteBtn, {
      armedLabel: "本当に完全削除？（もう一度押す）",
      timeoutMs: 6000,
      message: () => `${selectedIds.size} 件のアカウントを完全に削除します。この操作は元に戻せません。`,
      onConfirm: executeBulkDelete,
    });
  }

  // イベント登録
  if (searchBtn) { searchBtn.addEventListener("click", doSearch); }
  if (resetBtn) {
    resetBtn.addEventListener("click", () => {
      if (filterDisabled)   { filterDisabled.value = ""; }
      if (filterStatus)     { filterStatus.value = ""; }
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
  [filterDisabled, filterStatus, filterAdmin, filterOnline, filterNeverLogin, filterLoginBefore].forEach((el) => {
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

  // 初回描画
  renderThead();
  updateBulkModeVisibility();
  updateBulkUi();

  // 初回ロード
  load();

  return {
    unmount() {}
  };
}

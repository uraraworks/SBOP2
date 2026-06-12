/**
 * views/operation-history.js
 * 監査ログ（操作履歴）画面 (route: operation-history)
 *
 * API: GET /api/audit-logs?actor=&method=&path=&limit=N
 * レスポンス: { entries:[...], count, total, capacity }
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function escapeHtml(text) {
  if (text == null) { return ""; }
  return String(text)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#39;");
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card">
        <header class="card-header">
          <h2>監査ログ（操作履歴）</h2>
          <p class="card-sub">管理画面から実行された変更系 API（POST / PUT / DELETE）の成功履歴を新しい順に表示します。記録はサーバー再起動でリセットされるメモリ上のリングバッファです（最大 500 件）。</p>
        </header>
        <div class="form-row audit-log-filters">
          <label>
            実行者 (loginId)
            <input type="text" id="auditLogFilterActor" placeholder="例: admin" />
          </label>
          <label>
            メソッド
            <select id="auditLogFilterMethod">
              <option value="">すべて</option>
              <option value="POST">POST</option>
              <option value="PUT">PUT</option>
              <option value="DELETE">DELETE</option>
            </select>
          </label>
          <label>
            パス前方一致
            <input type="text" id="auditLogFilterPath" placeholder="例: /api/maps" />
          </label>
          <label>
            最大件数
            <input type="number" id="auditLogFilterLimit" min="1" max="500" value="100" />
          </label>
          <div class="form-actions">
            <button type="button" id="auditLogReloadBtn" class="primary">再読み込み</button>
            <button type="button" id="auditLogClearBtn">フィルタクリア</button>
          </div>
        </div>
        <div id="auditLogSummary" class="status-line"></div>
        <div id="auditLogListContainer">
          <table class="data-table audit-log-table">
            <thead>
              <tr>
                <th>#</th>
                <th>時刻 (UTC)</th>
                <th>実行者</th>
                <th>メソッド</th>
                <th>パス</th>
                <th>ステータス</th>
                <th>本文(要約)</th>
              </tr>
            </thead>
            <tbody id="auditLogListBody">
              <tr><td colspan="7" class="empty-row">読み込み待ち...</td></tr>
            </tbody>
          </table>
        </div>
      </section>`;

  const filterActorEl  = container.querySelector("#auditLogFilterActor");
  const filterMethodEl = container.querySelector("#auditLogFilterMethod");
  const filterPathEl   = container.querySelector("#auditLogFilterPath");
  const filterLimitEl  = container.querySelector("#auditLogFilterLimit");
  const reloadBtn      = container.querySelector("#auditLogReloadBtn");
  const clearBtn       = container.querySelector("#auditLogClearBtn");
  const summaryEl      = container.querySelector("#auditLogSummary");
  const bodyEl         = container.querySelector("#auditLogListBody");

  const state = {
    entries: [],
    loaded: false,
    isLoading: false,
    loadError: null,
    expandedId: null,
  };

  function buildQuery() {
    const actor  = (filterActorEl  && filterActorEl.value.trim())  || "";
    const method = (filterMethodEl && filterMethodEl.value)        || "";
    const path   = (filterPathEl   && filterPathEl.value.trim())   || "";
    const limit  = (filterLimitEl  && filterLimitEl.value)         || "100";
    const params = new URLSearchParams();
    if (actor)  { params.set("actor",  actor); }
    if (method) { params.set("method", method); }
    if (path)   { params.set("path",   path); }
    if (limit)  { params.set("limit",  limit); }
    const qs = params.toString();
    return qs ? `/api/audit-logs?${qs}` : "/api/audit-logs";
  }

  function renderList() {
    if (!bodyEl) { return; }
    const entries = state.entries;
    if (!entries.length) {
      bodyEl.innerHTML = '<tr><td colspan="7" class="empty-row">該当する履歴はありません。</td></tr>';
      return;
    }
    const rows = [];
    for (const e of entries) {
      const isExpanded = state.expandedId === e.id;
      rows.push(
        `<tr class="audit-log-row" data-audit-id="${e.id}">` +
          `<td>${e.id}</td>` +
          `<td>${escapeHtml(e.timestamp)}</td>` +
          `<td>${escapeHtml(e.actor)}${e.adminLevel ? ` <span class="badge">Lv${e.adminLevel}</span>` : ""}</td>` +
          `<td><span class="method method-${escapeHtml(e.method)}">${escapeHtml(e.method)}</span></td>` +
          `<td class="audit-log-path">${escapeHtml(e.path)}</td>` +
          `<td>${e.statusCode}</td>` +
          `<td class="audit-log-body-cell">${escapeHtml((e.bodyDigest || "").slice(0, 80))}${(e.bodyDigest || "").length > 80 ? "…" : ""}</td>` +
        `</tr>`
      );
      if (isExpanded) {
        let pretty = e.bodyDigest || "(本文なし)";
        try {
          if (e.bodyDigest) { pretty = JSON.stringify(JSON.parse(e.bodyDigest), null, 2); }
        } catch (_) { /* 非 JSON はそのまま */ }
        rows.push(
          `<tr class="audit-log-detail-row"><td colspan="7">` +
            `<div class="audit-log-detail">` +
              `<div><strong>accountId:</strong> ${escapeHtml(e.accountId || "-")}</div>` +
              `<div><strong>bodySize:</strong> ${e.bodySize} bytes${e.bodySize > (e.bodyDigest || "").length ? "（要約のため末尾省略）" : ""}</div>` +
              `<pre class="audit-log-body-pre">${escapeHtml(pretty)}</pre>` +
            `</div>` +
          `</td></tr>`
        );
      }
    }
    bodyEl.innerHTML = rows.join("");
  }

  async function loadList() {
    if (!bodyEl)  { return; }
    if (state.isLoading) { return; }
    state.isLoading = true;
    state.loadError = null;
    bodyEl.innerHTML = '<tr><td colspan="7" class="empty-row">読み込み中...</td></tr>';
    if (summaryEl) { summaryEl.textContent = ""; }

    try {
      const { response, data } = await fetchJson(buildQuery());
      if (!response.ok || !data) {
        throw new Error((data && data.error) || `HTTP ${response.status}`);
      }
      state.entries = Array.isArray(data.entries) ? data.entries : [];
      state.loaded  = true;
      if (summaryEl) {
        summaryEl.textContent = `取得 ${data.count ?? state.entries.length} 件 / 全 ${data.total ?? "?"} 件（容量 ${data.capacity ?? "?"}）`;
      }
      renderList();
    } catch (err) {
      state.loadError = err.message || String(err);
      if (bodyEl) {
        bodyEl.innerHTML = `<tr><td colspan="7" class="empty-row">取得エラー: ${escapeHtml(state.loadError)}</td></tr>`;
      }
    } finally {
      state.isLoading = false;
    }
  }

  // イベント登録
  if (reloadBtn) { reloadBtn.addEventListener("click", () => { loadList(); }); }
  if (clearBtn) {
    clearBtn.addEventListener("click", () => {
      if (filterActorEl)  { filterActorEl.value  = ""; }
      if (filterMethodEl) { filterMethodEl.value = ""; }
      if (filterPathEl)   { filterPathEl.value   = ""; }
      if (filterLimitEl)  { filterLimitEl.value  = "100"; }
      loadList();
    });
  }
  if (bodyEl) {
    bodyEl.addEventListener("click", (ev) => {
      const tr = ev.target.closest("tr.audit-log-row");
      if (!tr) { return; }
      const id = Number(tr.dataset.auditId);
      if (!Number.isFinite(id)) { return; }
      state.expandedId = (state.expandedId === id) ? null : id;
      renderList();
    });
  }

  // 初回ロード
  loadList();
}

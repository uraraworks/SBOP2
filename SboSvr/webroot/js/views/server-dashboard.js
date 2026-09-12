/**
 * views/server-dashboard.js
 * サーバー情報ダッシュボード画面 (route: server-dashboard)
 *
 * API:
 *   GET /api/server?includeMetrics=true
 *     → { items:[...], updatedAt }
 *     items[0].metrics.cpuUsage / memoryUsage / latencyMs は実測できない時 null。
 *     items[0].metrics.memoryMB (プロセスのメモリ MB) も null あり。
 *   GET /api/server/sessions
 *     → { updatedAt, count, sessions: [ { accountId, account, charId, charName,
 *         mapId, x, y, ip, lastKeepaliveSec, loginAt } ] }
 *     loginAt は「今回のログイン時刻」(ISO8601, null あり)。
 *     接続時間は loginAt とレスポンスの updatedAt の差で画面側が算出する。
 *     サーバーが対応していない場合 404、権限がない場合 403 を返しうる。
 */

import { fetchJson } from "../core/api.js";
import { openCharacterEditor } from "../core/game-pick.js";
import { getRouteParams, setRouteParams } from "../core/router.js";
import { createListToolbar } from "../components/list-toolbar.js";

const POLL_INTERVAL_MS = 30000;

let _timerId = null;
let _container = null;
let _toolbar = null;
let _sessions = [];
let _sessionsState = "loading"; // loading | ok | unsupported | forbidden | error
let _sessionsUpdatedAt = null; // 接続時間の算出に使う「現在時刻」(loginAtとの差分)
let _mapNames = null; // Map<number,string> | null(未取得) | false(取得失敗)

// ----------------------------------------------------------------
// 内部ユーティリティ
// ----------------------------------------------------------------

function qs(sel) {
  return _container ? _container.querySelector(sel) : null;
}

function setText(sel, text) {
  const el = qs(sel);
  if (el) { el.textContent = text; }
}

function isFiniteNumber(v) {
  return typeof v === "number" && Number.isFinite(v);
}

function formatPercent(v) {
  return isFiniteNumber(v) ? `${v.toFixed(1)} %` : "未計測";
}

function formatMemory(memoryUsage, memoryMB) {
  const parts = [];
  if (isFiniteNumber(memoryUsage)) { parts.push(`${memoryUsage.toFixed(1)} %`); }
  if (isFiniteNumber(memoryMB)) { parts.push(`${memoryMB.toFixed(1)} MB`); }
  if (!parts.length) { return "未計測"; }
  if (parts.length === 1) { return parts[0]; }
  return `${parts[0]}（${parts[1]}）`;
}

function formatLatency(v) {
  return isFiniteNumber(v) ? v.toFixed(1) : "未計測";
}

function formatRelativeSec(sec) {
  if (!isFiniteNumber(sec) || sec < 0) { return "-"; }
  if (sec < 60) { return `${Math.floor(sec)} 秒前`; }
  const minutes = Math.floor(sec / 60);
  if (minutes < 60) { return `${minutes} 分前`; }
  const hours = Math.floor(minutes / 60);
  if (hours < 24) { return `${hours} 時間前`; }
  const days = Math.floor(hours / 24);
  return `${days} 日前`;
}

function formatLocalDateTime(iso) {
  if (!iso) { return "-"; }
  const d = new Date(iso);
  if (isNaN(d.getTime())) { return "-"; }
  return d.toLocaleString("ja-JP");
}

// loginAt と updatedAt(このレスポンスの取得時刻) の差分を接続時間の秒数で返す。
// どちらか欠けている・不正な場合は null。
function connectedDurationSec(loginAt, updatedAt) {
  if (!loginAt || !updatedAt) { return null; }
  const loginMs = new Date(loginAt).getTime();
  const updatedMs = new Date(updatedAt).getTime();
  if (isNaN(loginMs) || isNaN(updatedMs)) { return null; }
  const sec = Math.floor((updatedMs - loginMs) / 1000);
  return sec >= 0 ? sec : null;
}

// 接続時間を「12 分」「1 時間 5 分」形式で表示する。null は "-"。
function formatDuration(sec) {
  if (!isFiniteNumber(sec) || sec < 0) { return "-"; }
  const totalMinutes = Math.floor(sec / 60);
  const hours = Math.floor(totalMinutes / 60);
  const minutes = totalMinutes % 60;
  if (hours <= 0) { return `${minutes} 分`; }
  return `${hours} 時間 ${minutes} 分`;
}

function escapeHtml(s) {
  return String(s ?? "").replace(/[&<>"']/g, (c) => (
    { "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;" }[c]
  ));
}

async function ensureMapNames() {
  if (_mapNames) { return _mapNames; }
  try {
    const { response, data } = await fetchJson("/api/maps");
    if (response.ok && data && Array.isArray(data.maps)) {
      _mapNames = new Map(data.maps.map((m) => [m.id, m.name || ""]));
    } else {
      _mapNames = false;
    }
  } catch {
    _mapNames = false;
  }
  return _mapNames;
}

function mapLabel(mapId) {
  if (mapId === null || mapId === undefined) { return "-"; }
  if (_mapNames && _mapNames.get) {
    const name = _mapNames.get(mapId);
    if (name) { return `${name}（${mapId}）`; }
  }
  return String(mapId);
}

// ----------------------------------------------------------------
// サーバー概要
// ----------------------------------------------------------------

function resetServerSummary(message) {
  setText("#server-status", message);
  setText("#server-online", "-");
  setText("#server-version", "-");
  setText("#server-cpu", "-");
  setText("#server-memory", "-");
  setText("#server-latency", "-");
  setText("#server-updated", "更新時刻: -");
}

function populateServerSummary(item, updatedAt) {
  setText("#server-status", item.status || "不明");
  setText("#server-online", typeof item.onlinePlayers === "number" ? item.onlinePlayers.toString() : "-");
  setText("#server-version", item.lastPatchVersion || "-");
  const metrics = item.metrics || {};
  setText("#server-cpu", formatPercent(metrics.cpuUsage));
  setText("#server-memory", formatMemory(metrics.memoryUsage, metrics.memoryMB));
  setText("#server-latency", formatLatency(metrics.latencyMs));
  setText("#server-updated", `更新時刻: ${updatedAt || "不明"}`);
}

async function loadServerInfo(showLoading = false) {
  if (showLoading) { resetServerSummary("読み込み中..."); }
  try {
    const { response, data } = await fetchJson("/api/server?includeMetrics=true");
    if (!response.ok || !data || !Array.isArray(data.items) || data.items.length === 0) {
      throw new Error("サーバー情報の取得に失敗しました");
    }
    populateServerSummary(data.items[0], data.updatedAt);
  } catch (error) {
    resetServerSummary("エラー");
    setText("#server-updated", `更新時刻: ${error.message}`);
  }
}

// ----------------------------------------------------------------
// 接続中プレイヤー一覧
// ----------------------------------------------------------------

function renderSessionsMessage(message) {
  const wrapper = qs("#server-sessions-table-wrapper");
  const messageEl = qs("#server-sessions-message");
  if (wrapper) { wrapper.style.display = "none"; }
  if (messageEl) {
    messageEl.style.display = "";
    messageEl.textContent = message;
  }
}

function renderSessionsTable() {
  const wrapper = qs("#server-sessions-table-wrapper");
  const messageEl = qs("#server-sessions-message");
  const tbody = qs("#server-sessions-table-body");
  if (!tbody) { return; }

  if (_sessionsState === "unsupported") {
    renderSessionsMessage("この一覧はサーバーが未対応です");
    return;
  }
  if (_sessionsState === "forbidden") {
    renderSessionsMessage("権限がありません");
    return;
  }
  if (_sessionsState === "error") {
    renderSessionsMessage("接続中プレイヤー一覧の取得に失敗しました");
    return;
  }
  if (_sessionsState === "loading" && !_sessions.length) {
    renderSessionsMessage("読み込み中...");
    return;
  }

  if (wrapper) { wrapper.style.display = ""; }
  if (messageEl) { messageEl.style.display = "none"; }

  tbody.innerHTML = "";
  if (!_sessions.length) {
    const tr = document.createElement("tr");
    tr.innerHTML = '<td colspan="8" style="text-align:center">接続中のプレイヤーはいません</td>';
    tbody.appendChild(tr);
    if (_toolbar) { _toolbar.setTotal(0); }
    return;
  }

  const { pageRows } = _toolbar.applyToRows(_sessions, {
    searchFields: ["charName", "account", "ip"],
    sorters: {
      charName: (a, b) => String(a.charName || "").localeCompare(String(b.charName || "")),
      account: (a, b) => String(a.account || "").localeCompare(String(b.account || "")),
      lastKeepalive: (a, b) => {
        const av = isFiniteNumber(a.lastKeepaliveSec) ? a.lastKeepaliveSec : Number.POSITIVE_INFINITY;
        const bv = isFiniteNumber(b.lastKeepaliveSec) ? b.lastKeepaliveSec : Number.POSITIVE_INFINITY;
        return av - bv;
      },
      duration: (a, b) => {
        const av = connectedDurationSec(a.loginAt, _sessionsUpdatedAt);
        const bv = connectedDurationSec(b.loginAt, _sessionsUpdatedAt);
        const avv = isFiniteNumber(av) ? av : -1;
        const bvv = isFiniteNumber(bv) ? bv : -1;
        return bvv - avv; // 接続時間が長い順
      },
    },
  });

  if (!pageRows.length) {
    const tr = document.createElement("tr");
    tr.innerHTML = '<td colspan="8" style="text-align:center">該当するプレイヤーがいません</td>';
    tbody.appendChild(tr);
    return;
  }

  pageRows.forEach((s) => {
    const tr = document.createElement("tr");
    const charNameCell = (s.charId > 0)
      ? `<button type="button" class="btn btn-link btn-sm" data-session-char-edit="${s.charId}">${escapeHtml(s.charName || `#${s.charId}`)}</button>`
      : escapeHtml(s.charName || "-");
    const coord = (isFiniteNumber(s.x) && isFiniteNumber(s.y)) ? `(${s.x}, ${s.y})` : "-";
    const durationSec = connectedDurationSec(s.loginAt, _sessionsUpdatedAt);
    tr.innerHTML =
      `<td>${charNameCell}</td>` +
      `<td>${escapeHtml(s.account || "")}</td>` +
      `<td>${escapeHtml(mapLabel(s.mapId))}</td>` +
      `<td>${coord}</td>` +
      `<td>${escapeHtml(s.ip || "-")}</td>` +
      `<td>${escapeHtml(formatRelativeSec(s.lastKeepaliveSec))}</td>` +
      `<td>${escapeHtml(formatLocalDateTime(s.loginAt))}</td>` +
      `<td>${escapeHtml(formatDuration(durationSec))}</td>`;
    tbody.appendChild(tr);
  });
}

async function loadSessions() {
  try {
    const { response, data } = await fetchJson("/api/server/sessions");
    if (response.status === 404) {
      _sessionsState = "unsupported";
      _sessions = [];
      renderSessionsTable();
      return;
    }
    if (response.status === 403) {
      _sessionsState = "forbidden";
      _sessions = [];
      renderSessionsTable();
      return;
    }
    if (!response.ok || !data || !Array.isArray(data.sessions)) {
      throw new Error("invalid_response");
    }
    _sessions = data.sessions;
    _sessionsUpdatedAt = data.updatedAt || null;
    _sessionsState = "ok";
    await ensureMapNames();
    renderSessionsTable();
  } catch {
    _sessionsState = "error";
    _sessions = [];
    renderSessionsTable();
  }
}

// ----------------------------------------------------------------
// ポーリング
// ----------------------------------------------------------------

function loadAll(showLoading = false) {
  return Promise.all([loadServerInfo(showLoading), loadSessions()]);
}

function startPolling() {
  stopPolling();
  _timerId = setInterval(() => { loadAll(false); }, POLL_INTERVAL_MS);
}

function stopPolling() {
  if (_timerId !== null) {
    clearInterval(_timerId);
    _timerId = null;
  }
}

// ----------------------------------------------------------------
// mount / unmount
// ----------------------------------------------------------------

export function mount(container) {
  _container = container;
  _sessions = [];
  _sessionsState = "loading";

  container.innerHTML = `
      <section class="card" id="server-card">
        <h2>サーバー概要</h2>
        <div class="status-grid">
          <div class="status-row">
            <span class="status-label">状態:</span>
            <span class="status-value" id="server-status">読み込み中...</span>
          </div>
          <div class="status-row">
            <span class="status-label">オンライン人数:</span>
            <span class="status-value" id="server-online">-</span>
          </div>
          <div class="status-row">
            <span class="status-label">最終パッチバージョン:</span>
            <span class="status-value" id="server-version">-</span>
          </div>
          <div class="status-row">
            <span class="status-label">CPU 使用率:</span>
            <span class="status-value" id="server-cpu">-</span>
          </div>
          <div class="status-row">
            <span class="status-label">メモリ使用率:</span>
            <span class="status-value" id="server-memory">-</span>
          </div>
          <div class="status-row">
            <span class="status-label">遅延 (ms):</span>
            <span class="status-value" id="server-latency">-</span>
          </div>
        </div>
        <p class="timestamp" id="server-updated">更新時刻: -</p>
        <div class="server-controls">
          <button type="button" id="reload-server" class="button">再読み込み</button>
        </div>
        <p class="note">30秒ごとに自動更新されます。</p>
      </section>

      <section class="card" id="server-sessions-card">
        <h2>接続中のプレイヤー</h2>
        <div id="server-sessions-toolbar"></div>
        <p id="server-sessions-message" class="note" aria-live="polite"></p>
        <div class="table-wrapper" id="server-sessions-table-wrapper">
          <table class="data-table">
            <thead>
              <tr>
                <th>キャラ名</th>
                <th>アカウント</th>
                <th>マップ</th>
                <th>座標</th>
                <th>IP</th>
                <th>最終通信</th>
                <th>ログイン日時</th>
                <th>接続時間</th>
              </tr>
            </thead>
            <tbody id="server-sessions-table-body"></tbody>
          </table>
        </div>
      </section>`;

  const reloadBtn = container.querySelector("#reload-server");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", () => { loadAll(true); });
  }

  const tbody = container.querySelector("#server-sessions-table-body");
  if (tbody) {
    tbody.addEventListener("click", (ev) => {
      const btn = ev.target.closest("[data-session-char-edit]");
      if (btn) {
        const id = parseInt(btn.getAttribute("data-session-char-edit"), 10);
        if (!isNaN(id) && id > 0) { openCharacterEditor(id); }
      }
    });
  }

  const toolbarHost = container.querySelector("#server-sessions-toolbar");
  const routeParams = getRouteParams();
  _toolbar = createListToolbar({
    placeholder: "キャラ名・アカウント・IP で検索",
    sortOptions: [
      { value: "charName", label: "キャラ名順" },
      { value: "account", label: "アカウント順" },
      { value: "lastKeepalive", label: "最終通信順" },
      { value: "duration", label: "接続時間順" },
    ],
    pageSizes: [20, 50, 100],
    initial: {
      q: routeParams.get("q") || "",
      sort: routeParams.get("sort") || "charName",
      page: Number(routeParams.get("page")) || 1,
    },
    onChange: (s) => {
      // 検索語(q)以外は URL に残さない。IP やアカウント名そのものが
      // URL に載ることを避けるため、検索欄の値だけを保持する。
      setRouteParams({
        q: s.q || null,
      });
      renderSessionsTable();
    },
  });
  if (toolbarHost) { toolbarHost.appendChild(_toolbar.element); }

  loadAll(true);
  startPolling();

  return {
    unmount() {
      stopPolling();
      _container = null;
      _toolbar = null;
    }
  };
}

/**
 * views/server-dashboard.js
 * サーバー情報ダッシュボード画面 (route: server-dashboard)
 *
 * API: /api/server?includeMetrics=true
 *   GET → { items:[...], updatedAt }
 */

import { fetchJson } from "../core/api.js";

const POLL_INTERVAL_MS = 30000;

let _timerId = null;
let _container = null;

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

function resetDisplay(message) {
  setText("#server-status", message);
  setText("#server-online", "-");
  setText("#server-version", "-");
  setText("#server-cpu", "-");
  setText("#server-memory", "-");
  setText("#server-latency", "-");
  setText("#server-updated", "更新時刻: -");
}

function populateDisplay(item, updatedAt) {
  setText("#server-status", item.status || "不明");
  setText("#server-online", typeof item.onlinePlayers === "number" ? item.onlinePlayers.toString() : "-");
  setText("#server-version", item.lastPatchVersion || "-");
  if (item.metrics) {
    setText("#server-cpu", `${item.metrics.cpuUsage.toFixed(1)} %`);
    setText("#server-memory", `${item.metrics.memoryUsage.toFixed(1)} %`);
    setText("#server-latency", `${item.metrics.latencyMs.toFixed(1)}`);
  } else {
    setText("#server-cpu", "-");
    setText("#server-memory", "-");
    setText("#server-latency", "-");
  }
  setText("#server-updated", `更新時刻: ${updatedAt || "不明"}`);
}

async function loadServerInfo(showLoading = false) {
  if (showLoading) { resetDisplay("読み込み中..."); }
  try {
    const { response, data } = await fetchJson("/api/server?includeMetrics=true");
    if (!response.ok || !data || !Array.isArray(data.items) || data.items.length === 0) {
      throw new Error("サーバー情報の取得に失敗しました");
    }
    populateDisplay(data.items[0], data.updatedAt);
  } catch (error) {
    resetDisplay("エラー");
    setText("#server-updated", `更新時刻: ${error.message}`);
  }
}

function startPolling() {
  stopPolling();
  _timerId = setInterval(() => { loadServerInfo(false); }, POLL_INTERVAL_MS);
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
      </section>`;

  const reloadBtn = container.querySelector("#reload-server");
  if (reloadBtn) {
    reloadBtn.addEventListener("click", () => { loadServerInfo(true); });
  }

  loadServerInfo(true);
  startPolling();

  return {
    unmount() {
      stopPolling();
      _container = null;
    }
  };
}

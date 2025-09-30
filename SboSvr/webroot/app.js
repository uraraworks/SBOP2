const serverStatusEl = document.getElementById("server-status");
const onlineEl = document.getElementById("server-online");
const versionEl = document.getElementById("server-version");
const cpuEl = document.getElementById("server-cpu");
const memoryEl = document.getElementById("server-memory");
const latencyEl = document.getElementById("server-latency");
const updatedEl = document.getElementById("server-updated");

async function loadServerInfo() {
  try {
    const response = await fetch("/api/server?includeMetrics=true", {
      headers: {
        "Accept": "application/json"
      },
      credentials: "include"
    });

    if (!response.ok) {
      throw new Error(`サーバー情報の取得に失敗しました (${response.status})`);
    }

    const payload = await response.json();
    const item = payload.items && payload.items.length > 0 ? payload.items[0] : null;
    if (!item) {
      throw new Error("サーバー情報の形式が正しくありません");
    }

    serverStatusEl.textContent = item.status || "不明";
    onlineEl.textContent = typeof item.onlinePlayers === "number" ? item.onlinePlayers.toString() : "-";
    versionEl.textContent = item.lastPatchVersion || "-";

    if (item.metrics) {
      cpuEl.textContent = `${item.metrics.cpuUsage.toFixed(1)} %`;
      memoryEl.textContent = `${item.metrics.memoryUsage.toFixed(1)} %`;
      latencyEl.textContent = `${item.metrics.latencyMs.toFixed(1)}`;
    } else {
      cpuEl.textContent = "-";
      memoryEl.textContent = "-";
      latencyEl.textContent = "-";
    }

    updatedEl.textContent = `更新時刻: ${payload.updatedAt || "不明"}`;
  } catch (error) {
    serverStatusEl.textContent = "エラー";
    onlineEl.textContent = "-";
    versionEl.textContent = "-";
    cpuEl.textContent = "-";
    memoryEl.textContent = "-";
    latencyEl.textContent = "-";
    updatedEl.textContent = error.message;
  }
}

window.addEventListener("load", loadServerInfo);

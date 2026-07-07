/**
 * views/character-account.js
 * アカウント紐付け・権限・BAN 画面 (route: character-account)
 *
 * API:
 *   GET  /api/characters/:id          → キャラ詳細
 *   GET  /api/characters/:id/account  → アカウント情報
 *   PUT  /api/characters/:id/account  → アカウント紐付け変更
 *   PUT  /api/characters/:id/admin    → 権限レベル変更
 *   PUT  /api/characters/:id/disabled → BAN/解除
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function setFeedback(el, msg, isError) {
  if (!el) { return; }
  el.textContent = msg;
  el.style.color = isError
    ? "var(--color-danger, #f87171)"
    : "var(--color-success, #4ade80)";
}

async function putJson(url, body) {
  const res = await fetch(url, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
    credentials: "same-origin"
  });
  let data = null;
  try { data = await res.json(); } catch (_) { /* ignore */ }
  return { response: res, data };
}

function renderAccountInfo(data, container) {
  const fields = {
    "ca-acct-accountId": data.accountId,
    "ca-acct-loginId":   data.loginId,
    "ca-acct-password":  data.password,
    "ca-acct-ip":        data.ip,
    "ca-acct-mac":       data.mac,
  };
  for (const [id, value] of Object.entries(fields)) {
    const el = container.querySelector(`#${id}`);
    if (el) { el.textContent = value != null ? String(value) : "-"; }
  }
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card" id="char-acct-card">
        <h2>アカウント紐付け・権限・BAN</h2>
        <p class="card-description">CharID を入力してキャラクターのアカウント情報を表示・変更します。</p>
        <div class="filter-row">
          <label>CharID:
            <input type="number" id="ca-char-id-input" placeholder="例: 1001" min="1" />
          </label>
          <button type="button" id="ca-load-btn" class="button">表示</button>
        </div>
        <p id="ca-feedback" class="form-feedback" aria-live="polite"></p>

        <div id="ca-body" style="display:none">
          <p style="margin-bottom:1rem">CharID: <strong id="ca-charId">-</strong> / キャラ名: <strong id="ca-charName">-</strong></p>

          <!-- アカウント情報表示 -->
          <section class="card" style="margin-bottom:1rem">
            <h3 style="margin-top:0">アカウント情報</h3>
            <dl class="detail-list">
              <div><dt>アカウントID</dt><dd id="ca-acct-accountId">-</dd></div>
              <div><dt>ログインID</dt><dd id="ca-acct-loginId">-</dd></div>
              <div><dt>パスワード</dt><dd id="ca-acct-password">-</dd></div>
              <div><dt>IP アドレス</dt><dd id="ca-acct-ip">-</dd></div>
              <div><dt>MAC アドレス</dt><dd id="ca-acct-mac">-</dd></div>
            </dl>
          </section>

          <!-- アカウント紐付け変更 -->
          <section class="card" style="margin-bottom:1rem">
            <h3 style="margin-top:0">アカウント紐付け変更</h3>
            <p class="card-description">accountId=0 を指定すると紐付けを解除します。</p>
            <div class="filter-row">
              <label>アカウントID:
                <input type="number" id="ca-new-account-id" placeholder="例: 1001" min="0" />
              </label>
              <button type="button" id="ca-link-btn" class="button primary">変更</button>
              <button type="button" id="ca-unlink-btn" class="button danger">解除（0）</button>
            </div>
            <p id="ca-link-feedback" class="form-feedback" aria-live="polite"></p>
          </section>

          <!-- 管理者権限レベル -->
          <section class="card" style="margin-bottom:1rem">
            <h3 style="margin-top:0">管理者権限レベル</h3>
            <p class="card-description">0 = 一般、1 = 管理者。紐付きアカウントの権限レベルを変更します。</p>
            <div class="filter-row">
              <label>権限レベル (0-255):
                <input type="number" id="ca-admin-level" placeholder="例: 0" min="0" max="255" />
              </label>
              <button type="button" id="ca-admin-btn" class="button primary">保存</button>
            </div>
            <p id="ca-admin-feedback" class="form-feedback" aria-live="polite"></p>
          </section>

          <!-- 無効化（BAN） -->
          <section class="card">
            <h3 style="margin-top:0">アカウント無効化（BAN）</h3>
            <p class="card-description">紐付きアカウントのログイン拒否フラグを設定します。</p>
            <dl class="detail-list" style="margin-bottom:0.75rem">
              <div><dt>現在の状態</dt><dd id="ca-disabled-status">-</dd></div>
            </dl>
            <div class="form-actions">
              <button type="button" id="ca-ban-btn" class="button danger">BAN（無効化）</button>
              <button type="button" id="ca-unban-btn" class="button secondary">BAN 解除</button>
            </div>
            <p id="ca-ban-feedback" class="form-feedback" aria-live="polite"></p>
          </section>
        </div>
      </section>`;

  const loadBtn       = container.querySelector("#ca-load-btn");
  const charIdInput   = container.querySelector("#ca-char-id-input");
  const feedbackEl    = container.querySelector("#ca-feedback");
  const bodyEl        = container.querySelector("#ca-body");
  const linkFbEl      = container.querySelector("#ca-link-feedback");
  const adminFbEl     = container.querySelector("#ca-admin-feedback");
  const banFbEl       = container.querySelector("#ca-ban-feedback");

  function getCharId() {
    const v = parseInt((charIdInput && charIdInput.value) || "", 10);
    return Number.isFinite(v) && v > 0 ? v : null;
  }

  // キャラ情報ロード
  async function loadCaView(charId) {
    if (feedbackEl) { feedbackEl.textContent = "読み込み中..."; feedbackEl.style.color = ""; }
    if (bodyEl) { bodyEl.style.display = "none"; }

    try {
      const detailRes = await fetchJson("/api/characters/" + encodeURIComponent(charId));
      if (!detailRes.response.ok || !detailRes.data) {
        const msg = (detailRes.data && detailRes.data.error) ? detailRes.data.error : "キャラクターが見つかりません";
        setFeedback(feedbackEl, "エラー: " + msg, true);
        return;
      }
      const d = detailRes.data;
      const charNameEl = container.querySelector("#ca-charName");
      const charIdEl   = container.querySelector("#ca-charId");
      if (charIdEl)   { charIdEl.textContent   = String(d.charId); }
      if (charNameEl) { charNameEl.textContent = d.charName || "-"; }

      const acctRes = await fetchJson("/api/characters/" + encodeURIComponent(charId) + "/account");
      if (!acctRes.response.ok || !acctRes.data) {
        const acctMsg = (acctRes.data && acctRes.data.error) ? acctRes.data.error : "アカウント情報の取得に失敗しました";
        setFeedback(feedbackEl, "エラー: " + acctMsg, true);
        return;
      }
      renderAccountInfo(acctRes.data, container);

      const caDisabledEl = container.querySelector("#ca-disabled-status");
      if (caDisabledEl) { caDisabledEl.textContent = "（APIから取得不可 — BAN 操作は実行可能）"; }

      if (feedbackEl) { feedbackEl.textContent = ""; }
      if (bodyEl) { bodyEl.style.display = ""; }
    } catch {
      setFeedback(feedbackEl, "通信エラーが発生しました", true);
    }
  }

  // 表示ボタン
  if (loadBtn) {
    loadBtn.addEventListener("click", () => {
      const charId = getCharId();
      if (!charId) { alert("有効な CharID（1以上の整数）を入力してください"); return; }
      loadCaView(charId);
    });
  }
  if (charIdInput) {
    charIdInput.addEventListener("keydown", (ev) => {
      if (ev.key === "Enter" && loadBtn) { loadBtn.click(); }
    });
  }

  // アカウント紐付け変更
  const linkBtn = container.querySelector("#ca-link-btn");
  if (linkBtn) {
    linkBtn.addEventListener("click", async () => {
      const charId = getCharId();
      if (!charId) { return; }
      const newAcctIdInput = container.querySelector("#ca-new-account-id");
      const val = newAcctIdInput ? parseInt(newAcctIdInput.value, 10) : NaN;
      if (isNaN(val) || val < 0) { alert("有効なアカウントID（0以上）を入力してください"); return; }
      try {
        const { response, data } = await putJson("/api/characters/" + charId + "/account", { accountId: val });
        if (!response.ok) {
          const msg = (data && data.error) ? data.error : "変更に失敗しました";
          setFeedback(linkFbEl, "エラー: " + msg, true);
          return;
        }
        setFeedback(linkFbEl, `アカウント紐付けを変更しました（accountId=${val}）`, false);
        const acctRes = await fetchJson("/api/characters/" + charId + "/account");
        if (acctRes.response.ok && acctRes.data) { renderAccountInfo(acctRes.data, container); }
      } catch {
        setFeedback(linkFbEl, "通信エラーが発生しました", true);
      }
    });
  }

  // 解除ボタン（accountId=0）
  const unlinkBtn = container.querySelector("#ca-unlink-btn");
  if (unlinkBtn) {
    unlinkBtn.addEventListener("click", async () => {
      const charId = getCharId();
      if (!charId) { return; }
      if (!confirm("アカウント紐付けを解除しますか？")) { return; }
      try {
        const { response, data } = await putJson("/api/characters/" + charId + "/account", { accountId: 0 });
        if (!response.ok) {
          const msg = (data && data.error) ? data.error : "解除に失敗しました";
          setFeedback(linkFbEl, "エラー: " + msg, true);
          return;
        }
        setFeedback(linkFbEl, "アカウント紐付けを解除しました", false);
        const acctRes = await fetchJson("/api/characters/" + charId + "/account");
        if (acctRes.response.ok && acctRes.data) { renderAccountInfo(acctRes.data, container); }
      } catch {
        setFeedback(linkFbEl, "通信エラーが発生しました", true);
      }
    });
  }

  // 管理者権限レベル保存
  const adminBtn = container.querySelector("#ca-admin-btn");
  if (adminBtn) {
    adminBtn.addEventListener("click", async () => {
      const charId = getCharId();
      if (!charId) { return; }
      const adminLevelInput = container.querySelector("#ca-admin-level");
      const val = adminLevelInput ? parseInt(adminLevelInput.value, 10) : NaN;
      if (isNaN(val) || val < 0 || val > 255) { alert("権限レベルは 0〜255 の整数で入力してください"); return; }
      try {
        const { response, data } = await putJson("/api/characters/" + charId + "/admin", { adminLevel: val });
        if (!response.ok) {
          const msg = (data && data.error) ? data.error : "設定に失敗しました";
          setFeedback(adminFbEl, "エラー: " + msg, true);
          return;
        }
        setFeedback(adminFbEl, `管理者権限レベルを ${val} に設定しました`, false);
      } catch {
        setFeedback(adminFbEl, "通信エラーが発生しました", true);
      }
    });
  }

  // BAN ボタン
  const banBtn = container.querySelector("#ca-ban-btn");
  if (banBtn) {
    banBtn.addEventListener("click", async () => {
      const charId = getCharId();
      if (!charId) { return; }
      if (!confirm("このキャラクターのアカウントを BAN しますか？")) { return; }
      try {
        const { response, data } = await putJson("/api/characters/" + charId + "/disabled", { disabled: true });
        if (!response.ok) {
          const msg = (data && data.error) ? data.error : "BAN に失敗しました";
          setFeedback(banFbEl, "エラー: " + msg, true);
          return;
        }
        setFeedback(banFbEl, "BAN（無効化）しました", false);
        const caDisabledEl = container.querySelector("#ca-disabled-status");
        if (caDisabledEl) { caDisabledEl.textContent = "BAN 中"; }
      } catch {
        setFeedback(banFbEl, "通信エラーが発生しました", true);
      }
    });
  }

  // BAN 解除ボタン
  const unbanBtn = container.querySelector("#ca-unban-btn");
  if (unbanBtn) {
    unbanBtn.addEventListener("click", async () => {
      const charId = getCharId();
      if (!charId) { return; }
      try {
        const { response, data } = await putJson("/api/characters/" + charId + "/disabled", { disabled: false });
        if (!response.ok) {
          const msg = (data && data.error) ? data.error : "BAN 解除に失敗しました";
          setFeedback(banFbEl, "エラー: " + msg, true);
          return;
        }
        setFeedback(banFbEl, "BAN 解除（有効化）しました", false);
        const caDisabledEl = container.querySelector("#ca-disabled-status");
        if (caDisabledEl) { caDisabledEl.textContent = "有効"; }
      } catch {
        setFeedback(banFbEl, "通信エラーが発生しました", true);
      }
    });
  }
}

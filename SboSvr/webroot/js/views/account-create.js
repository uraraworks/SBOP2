/**
 * views/account-create.js
 * 管理アカウント作成画面 (route: account-create)
 *
 * API:
 *   GET /api/admin/roles → ロール一覧
 *   POST /api/accounts  → アカウント作成
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// ロール UI 共通ヘルパ
// ----------------------------------------------------------------

async function loadRoles() {
  try {
    const { response, data } = await fetchJson("/api/admin/roles");
    if (!response.ok || !Array.isArray(data)) { throw new Error(); }
    return data;
  } catch {
    return [];
  }
}

function createRoleOption(role, prefix) {
  const wrapper = document.createElement("div");
  wrapper.className = "role-option";

  const input = document.createElement("input");
  input.type = "checkbox";
  input.value = role.id;
  const inputId = `${prefix}-${role.id}`;
  input.id = inputId;

  const label = document.createElement("label");
  label.setAttribute("for", inputId);

  const nameSpan = document.createElement("span");
  nameSpan.className = "role-name";
  nameSpan.textContent = role.name || role.id;
  label.appendChild(nameSpan);

  if (role.description) {
    const descSpan = document.createElement("span");
    descSpan.className = "role-description";
    descSpan.textContent = role.description;
    label.appendChild(descSpan);
  }

  if (Array.isArray(role.featureFlags) && role.featureFlags.length) {
    const flags = document.createElement("div");
    flags.className = "role-flags";
    role.featureFlags.forEach((flag) => {
      const flagEl = document.createElement("span");
      flagEl.className = "role-flag";
      flagEl.textContent = flag;
      flags.appendChild(flagEl);
    });
    label.appendChild(flags);
  }

  wrapper.appendChild(input);
  wrapper.appendChild(label);
  return wrapper;
}

function renderRoleOptions(container, roles, prefix) {
  container.innerHTML = "";
  if (!roles.length) {
    const p = document.createElement("p");
    p.textContent = "ロール情報を取得できませんでした";
    p.className = "note";
    container.appendChild(p);
    return;
  }
  roles.forEach((role) => {
    container.appendChild(createRoleOption(role, prefix));
  });
}

function collectSelectedRoles(container) {
  const values = [];
  container.querySelectorAll("input[type=checkbox]:checked").forEach((input) => {
    values.push(input.value);
  });
  return values;
}

function setResultMessage(element, message, type) {
  element.textContent = message;
  element.className = "result-message";
  if (type) { element.classList.add(type); }
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

export async function mount(container) {
  container.innerHTML = `
      <section class="card" id="account-card">
        <h2>管理アカウント作成</h2>
        <p class="card-description">管理者用アカウントを追加する場合は以下のフォームに必要事項を入力してください。</p>
        <form id="account-form" novalidate>
          <div class="form-grid">
            <label class="form-field">
              <span>ログインID</span>
              <input type="text" id="account-login" name="loginId" autocomplete="username" required />
            </label>
            <label class="form-field">
              <span>表示名</span>
              <input type="text" id="account-display" name="displayName" required />
            </label>
            <label class="form-field">
              <span>メールアドレス</span>
              <input type="email" id="account-email" name="email" autocomplete="email" />
            </label>
            <label class="form-field">
              <span>パスワード</span>
              <input type="password" id="account-password" name="password" autocomplete="new-password" required />
            </label>
            <label class="form-field">
              <span>パスワード再入力</span>
              <input type="password" id="account-password-confirm" name="passwordConfirm" autocomplete="new-password" required />
            </label>
          </div>
          <fieldset class="form-field roles-field">
            <legend>付与ロール</legend>
            <div id="account-roles" class="roles-container" aria-live="polite"></div>
          </fieldset>
          <div class="form-actions">
            <button type="submit" class="button primary">アカウントを作成</button>
            <button type="reset" class="button secondary" id="account-reset">リセット</button>
          </div>
        </form>
        <div id="account-result" class="result-message" role="alert"></div>
      </section>`;

  const form = container.querySelector("#account-form");
  const resultEl = container.querySelector("#account-result");
  const rolesContainer = container.querySelector("#account-roles");
  const resetButton = container.querySelector("#account-reset");

  let cachedRoles = [];

  function refreshRoles() {
    renderRoleOptions(rolesContainer, cachedRoles, "account-role");
  }

  // ロール読み込み
  cachedRoles = await loadRoles();
  refreshRoles();

  function buildAccountErrorMessage(status, data, text) {
    if (!data || typeof data !== "object") {
      return `アカウント作成に失敗しました (HTTP ${status})\n${text || ""}`;
    }
    if (status === 400 || status === 422) {
      if (Array.isArray(data.details) && data.details.length) {
        const issues = data.details.map((issue) => `${issue.field}: ${issue.message}`).join("\n");
        return `入力内容に誤りがあります:\n${issues}`;
      }
      return "入力内容に誤りがあります";
    }
    if (status === 409) {
      if (data.error === "role_conflict" && data.conflictRole) {
        return `ロールの排他制約により更新できません: ${data.conflictRole}`;
      }
      return "同じログインIDのアカウントが既に存在します";
    }
    if (status === 401) { return "認証に失敗しました。再度ログインしてください"; }
    if (status === 403) { return "アカウント作成の権限がありません"; }
    return `アカウント作成に失敗しました (HTTP ${status})`;
  }

  async function handleSubmit(event) {
    event.preventDefault();
    setResultMessage(resultEl, "送信中...", null);

    const loginId = form.loginId.value.trim();
    const displayName = form.displayName.value.trim();
    const emailRaw = form.email.value.trim();
    const password = form.password.value;
    const passwordConfirm = form.passwordConfirm.value;
    const selectedRoles = collectSelectedRoles(rolesContainer);

    if (password !== passwordConfirm) {
      setResultMessage(resultEl, "パスワードと再入力が一致しません", "error");
      return;
    }
    if (!selectedRoles.length) {
      setResultMessage(resultEl, "少なくとも1つのロールを選択してください", "error");
      return;
    }

    const payload = { loginId, displayName, password, roles: selectedRoles };
    payload.email = emailRaw || null;

    try {
      const { response, data, text } = await fetchJson("/api/accounts", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
      });

      if (response.ok) {
        form.reset();
        refreshRoles();
        const accountId = data && data.accountId ? data.accountId : "不明";
        setResultMessage(resultEl, `アカウントを作成しました (ID: ${accountId})`, "success");
        return;
      }
      setResultMessage(resultEl, buildAccountErrorMessage(response.status, data, text), "error");
    } catch {
      setResultMessage(resultEl, "アカウント作成中にエラーが発生しました", "error");
    }
  }

  if (form) { form.addEventListener("submit", handleSubmit); }
  if (resetButton) {
    resetButton.addEventListener("click", () => {
      setResultMessage(resultEl, "", null);
      refreshRoles();
    });
  }
}

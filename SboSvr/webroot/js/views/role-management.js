/**
 * views/role-management.js
 * 管理者ロール設定画面 (route: role-management)
 *
 * API:
 *   GET /api/admin/roles           → ロール一覧
 *   PUT /api/admin/roles?accountId → ロール更新
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// ロール UI ヘルパ
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
      <section class="card" id="role-card">
        <h2>ロール割り当て</h2>
        <p class="card-description">既存アカウントの権限ロールを更新するにはアカウントIDを指定し、必要なロールを選択してください。</p>
        <form id="role-form" novalidate>
          <div class="form-grid">
            <label class="form-field">
              <span>アカウントID</span>
              <input type="text" id="role-account-id" name="accountId" autocomplete="off" required />
            </label>
            <label class="form-field">
              <span>コメント (任意)</span>
              <input type="text" id="role-comment" name="comment" maxlength="256" />
            </label>
          </div>
          <fieldset class="form-field roles-field">
            <legend>付与ロール</legend>
            <div id="role-checkboxes" class="roles-container" aria-live="polite"></div>
          </fieldset>
          <div class="form-actions">
            <button type="submit" class="button primary">ロールを更新</button>
            <button type="reset" class="button secondary" id="role-reset">リセット</button>
          </div>
        </form>
        <div id="role-result" class="result-message" role="alert"></div>
      </section>`;

  const form = container.querySelector("#role-form");
  const resultEl = container.querySelector("#role-result");
  const checkboxContainer = container.querySelector("#role-checkboxes");
  const resetButton = container.querySelector("#role-reset");

  let cachedRoles = [];

  function refreshRoles() {
    renderRoleOptions(checkboxContainer, cachedRoles, "update-role");
  }

  cachedRoles = await loadRoles();
  refreshRoles();

  function buildRoleErrorMessage(status, data, text) {
    if (status === 400) { return "リクエスト形式が正しくありません"; }
    if (status === 404) { return "指定したアカウントIDが見つかりません"; }
    if (status === 409) {
      if (data && data.conflictRole) {
        return `ロールの排他制約により更新できません: ${data.conflictRole}`;
      }
      return "ロールの排他制約により更新できません";
    }
    if (status === 401) { return "認証に失敗しました。再度ログインしてください"; }
    if (status === 403) { return "ロール更新の権限がありません"; }
    return `${text || "ロール更新に失敗しました"} (HTTP ${status})`;
  }

  async function handleSubmit(event) {
    event.preventDefault();
    setResultMessage(resultEl, "送信中...", null);

    const accountId = form.accountId.value.trim();
    const comment = form.comment.value.trim();
    const selectedRoles = collectSelectedRoles(checkboxContainer);

    if (!accountId) {
      setResultMessage(resultEl, "アカウントIDを入力してください", "error");
      return;
    }
    if (!selectedRoles.length) {
      setResultMessage(resultEl, "付与するロールを選択してください", "error");
      return;
    }

    const payload = { roles: selectedRoles };
    if (comment) { payload.comment = comment; }

    try {
      const { response, data, text } = await fetchJson(
        `/api/admin/roles?accountId=${encodeURIComponent(accountId)}`,
        {
          method: "PUT",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload)
        }
      );

      if (response.status === 204) {
        setResultMessage(resultEl, "ロールを更新しました", "success");
        return;
      }
      setResultMessage(resultEl, buildRoleErrorMessage(response.status, data, text), "error");
    } catch {
      setResultMessage(resultEl, "ロール更新中にエラーが発生しました", "error");
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

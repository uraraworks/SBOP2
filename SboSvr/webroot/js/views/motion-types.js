/**
 * views/motion-types.js
 * モーション種別管理画面 (route: motion-management)
 * list-detail シェルで左リスト + 右詳細フォームを実装。
 */

import { fetchJson } from "../core/api.js";
import { createListDetail } from "../components/list-detail.js";

// ----------------------------------------------------------------
// API ヘルパ
// ----------------------------------------------------------------

async function fetchMotionTypeList() {
  const { response, data } = await fetchJson("/api/motion-types");
  if (!response.ok || !Array.isArray(data?.motionTypes)) {
    throw new Error("モーション種別の取得に失敗しました (HTTP " + response.status + ")");
  }
  return data.motionTypes.slice().sort((a, b) => a.motionTypeId - b.motionTypeId);
}

// ----------------------------------------------------------------
// リストアイテム表示
// ----------------------------------------------------------------

function renderListItem(mt) {
  return "[" + mt.motionTypeId + "] " + (mt.name || "(名前なし)");
}

// ----------------------------------------------------------------
// 詳細フォーム
// ----------------------------------------------------------------

function renderDetail(mt) {
  const wrap = document.createElement("div");
  wrap.className = "motion-types-detail";

  const grid = document.createElement("div");
  grid.className = "form-grid compact";

  // ID (読み取り専用)
  const idLabel = document.createElement("label");
  idLabel.className = "form-field";
  const idSpan = document.createElement("span");
  idSpan.textContent = "種別ID";
  const idInput = document.createElement("input");
  idInput.type = "number";
  idInput.name = "motionTypeId";
  idInput.readOnly = true;
  idInput.value = mt ? String(mt.motionTypeId) : "";
  idInput.style.opacity = "0.6";
  idLabel.append(idSpan, idInput);
  grid.appendChild(idLabel);

  // 名前
  const nameLabel = document.createElement("label");
  nameLabel.className = "form-field";
  const nameSpan = document.createElement("span");
  nameSpan.textContent = "名前";
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.name = "name";
  nameInput.maxLength = 64;
  nameInput.value = mt ? (mt.name || "") : "";
  nameLabel.append(nameSpan, nameInput);
  grid.appendChild(nameLabel);

  // グラフィックID (サブ)
  const grpLabel = document.createElement("label");
  grpLabel.className = "form-field";
  const grpSpan = document.createElement("span");
  grpSpan.textContent = "グラフィックID (サブ)";
  const grpInput = document.createElement("input");
  grpInput.type = "number";
  grpInput.name = "grpIdSub";
  grpInput.min = "0";
  grpInput.value = mt ? String(mt.grpIdSub ?? 0) : "0";
  grpLabel.append(grpSpan, grpInput);
  grid.appendChild(grpLabel);

  wrap.appendChild(grid);

  // ペイロード収集
  wrap._collectData = function () {
    return {
      name:     nameInput.value,
      grpIdSub: parseInt(grpInput.value, 10) || 0,
    };
  };

  return wrap;
}

// ----------------------------------------------------------------
// フィードバック表示
// ----------------------------------------------------------------

function showFeedback(container, message, type) {
  let fb = container.querySelector(".mt-feedback");
  if (!fb) {
    fb = document.createElement("p");
    fb.className = "mt-feedback result-message";
    fb.setAttribute("role", "status");
    fb.setAttribute("aria-live", "polite");
    container.prepend(fb);
  }
  fb.textContent = message;
  fb.className = "mt-feedback result-message" + (type ? " " + type : "");
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  createListDetail({
    container,
    title: "モーション種別一覧",
    fetchList: fetchMotionTypeList,
    renderListItem,
    searchKeys: ["motionTypeId", "name"],

    renderDetail: (mt) => renderDetail(mt),

    onSave: async (mt) => {
      const detailEl = container.querySelector(".ld-right-body .motion-types-detail");
      if (!detailEl?._collectData) throw new Error("フォームが見つかりません");
      const payload = detailEl._collectData();
      const isNew = !mt?.motionTypeId;
      const method = isNew ? "POST" : "PUT";
      if (!isNew) { payload.motionTypeId = mt.motionTypeId; }
      const { response, data } = await fetchJson("/api/motion-types", {
        method,
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        const msg = data?.error ?? "HTTP " + response.status;
        showFeedback(container, "保存に失敗しました: " + msg, "error");
        throw new Error(msg);
      }
      showFeedback(container, isNew ? "追加しました" : "保存しました", "success");
    },

    onCreate: async () => {
      const { response, data } = await fetchJson("/api/motion-types", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ name: "", grpIdSub: 0 }),
      });
      if (!response.ok) {
        const msg = data?.error ?? "HTTP " + response.status;
        showFeedback(container, "新規追加に失敗しました: " + msg, "error");
        throw new Error(msg);
      }
      showFeedback(container, "種別 " + (data?.motionTypeId ?? "") + " を追加しました", "success");
      return null;
    },

    onDelete: async (mt) => {
      const { response, data } = await fetchJson("/api/motion-types", {
        method: "DELETE",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ motionTypeId: mt.motionTypeId }),
      });
      if (!response.ok) {
        const msg = data?.error ?? "HTTP " + response.status;
        showFeedback(container, "削除に失敗しました: " + msg, "error");
        throw new Error(msg);
      }
      showFeedback(container, "削除しました (ID=" + mt.motionTypeId + ")", "success");
    },
  });

  _destroyFn = () => { container.innerHTML = ""; };
}

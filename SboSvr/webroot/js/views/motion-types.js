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
  const [{ response, data }, motionsResult] = await Promise.all([
    fetchJson("/api/motion-types"),
    fetchJson("/api/motions").catch(() => null),
  ]);
  if (!response.ok || !Array.isArray(data?.motionTypes)) {
    throw new Error("モーション種別の取得に失敗しました (HTTP " + response.status + ")");
  }
  const motions = Array.isArray(motionsResult?.data?.motions) ? motionsResult.data.motions : [];
  const stats = new Map();
  motions.forEach((motion) => {
    const typeId = Number(motion.motionTypeId || 0);
    if (!stats.has(typeId)) {
      stats.set(typeId, { frameCount: 0, listIds: new Set(), configuredListIds: new Set() });
    }
    const stat = stats.get(typeId);
    stat.frameCount += 1;
    stat.listIds.add(Number(motion.motionListId || 0));
    if (hasMotionContent(motion)) {
      stat.configuredListIds.add(Number(motion.motionListId || 0));
    }
  });
  return data.motionTypes
    .map((mt) => {
      const stat = stats.get(Number(mt.motionTypeId)) ?? { frameCount: 0, listIds: new Set(), configuredListIds: new Set() };
      return {
        ...mt,
        frameCount: stat.frameCount,
        motionListCount: stat.listIds.size,
        configuredListCount: stat.configuredListIds.size,
      };
    })
    .sort((a, b) => a.motionTypeId - b.motionTypeId);
}

function hasMotionContent(motion) {
  return Boolean(
    Number(motion.wait ?? 0) ||
    Number(motion.grpIdMainBase ?? 0) ||
    Number(motion.grpIdSubBase ?? 0) ||
    Number(motion.grpIdMainPile1 ?? 0) ||
    Number(motion.grpIdSubPile1 ?? 0) ||
    Number(motion.grpIdMainPile2 ?? 0) ||
    Number(motion.grpIdSubPile2 ?? 0) ||
    Number(motion.grpIdMainPile3 ?? 0) ||
    Number(motion.grpIdSubPile3 ?? 0) ||
    Number(motion.soundId ?? 0) ||
    Number(motion.procId ?? 0)
  );
}

// ----------------------------------------------------------------
// リストアイテム表示
// ----------------------------------------------------------------

function renderListItem(mt) {
  return "[" + mt.motionTypeId + "] " + (mt.name || "(名前なし)")
    + " / 設定あり" + (mt.configuredListCount ?? 0) + "リスト"
    + " / 総" + (mt.motionListCount ?? 0) + "リスト"
    + " / " + (mt.frameCount ?? 0) + "フレーム";
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

  const summary = document.createElement("div");
  summary.className = "motion-type-summary";

  const countText = document.createElement("p");
  countText.className = "muted";
  countText.textContent = "登録済み: 設定あり " + (mt?.configuredListCount ?? 0)
    + " リスト / 総 " + (mt?.motionListCount ?? 0)
    + " リスト / " + (mt?.frameCount ?? 0) + " フレーム";

  const editLink = document.createElement("button");
  editLink.type = "button";
  editLink.className = "button secondary";
  editLink.textContent = "この種別のモーション編集へ";
  editLink.addEventListener("click", () => {
    window.location.hash = "motion-edit";
  });

  summary.append(countText, editLink);
  wrap.appendChild(summary);

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

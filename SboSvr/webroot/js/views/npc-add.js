/**
 * views/npc-add.js
 * NPC 追加画面 (route: npc-management)
 *
 * API: POST /api/characters/npc
 *   body: { charName, familyId, mapId, x, y, moveType, sex, grpIdNpc, grpIdInitNpc, motionTypeId, block, push }
 *   response: { charId }
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ----------------------------------------------------------------
// 定数
// ----------------------------------------------------------------

const MOVE_TYPE_OPTIONS = [
  { value: 1,  label: "1: STAND（移動しない）" },
  { value: 0,  label: "0: PC" },
  { value: 5,  label: "5: ATACKANIME（攻撃アニメ）" },
  { value: 6,  label: "6: MOVEATACK（移動して攻撃）" },
  { value: 8,  label: "8: PUTNPC（NPC発生）" },
  { value: 9,  label: "9: BATTLE1" },
  { value: 10, label: "10: BATTLE2" },
];

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

function makeField(labelText, inputEl) {
  const wrap = document.createElement("label");
  wrap.className = "form-field";
  const span = document.createElement("span");
  span.textContent = labelText;
  wrap.appendChild(span);
  if (inputEl) { wrap.appendChild(inputEl); }
  return wrap;
}

function makeTextInput(placeholder, maxlength) {
  const input = document.createElement("input");
  input.type = "text";
  input.className = "form-input";
  if (placeholder) { input.placeholder = placeholder; }
  if (maxlength) { input.maxLength = maxlength; }
  return input;
}

function makeNumberInput(min, placeholder) {
  const input = document.createElement("input");
  input.type = "number";
  input.className = "form-input";
  if (min !== undefined) { input.min = String(min); }
  if (placeholder !== undefined) { input.placeholder = String(placeholder); }
  return input;
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const card = document.createElement("section");
  card.className = "card";
  container.appendChild(card);

  const h2 = document.createElement("h2");
  h2.textContent = "NPC 追加";
  card.appendChild(h2);

  const feedbackEl = document.createElement("p");
  feedbackEl.className = "result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";
  card.appendChild(feedbackEl);

  // ---- 必須項目セクション ----
  const reqSection = document.createElement("section");
  reqSection.className = "detail-section";
  const reqH3 = document.createElement("h3");
  reqH3.textContent = "必須項目";
  reqSection.appendChild(reqH3);

  const reqGrid = document.createElement("div");
  reqGrid.className = "form-grid compact";

  // キャラ名
  const charNameInput = makeTextInput("NPC名を入力", 32);
  reqGrid.appendChild(makeField("キャラ名", charNameInput));

  // 種族ID
  const familyIdInput = makeNumberInput(1, "1");
  reqGrid.appendChild(makeField("種族ID", familyIdInput));

  // マップID
  const mapIdInput = makeNumberInput(1, "1");
  reqGrid.appendChild(makeField("マップID", mapIdInput));

  // 座標
  const xInput = makeNumberInput(0, "0");
  reqGrid.appendChild(makeField("座標 X", xInput));
  const yInput = makeNumberInput(0, "0");
  reqGrid.appendChild(makeField("座標 Y", yInput));

  reqSection.appendChild(reqGrid);
  card.appendChild(reqSection);

  // ---- 詳細設定セクション（任意） ----
  const optSection = document.createElement("section");
  optSection.className = "detail-section";
  const optH3 = document.createElement("h3");
  optH3.textContent = "詳細設定（任意）";
  optSection.appendChild(optH3);

  const optGrid = document.createElement("div");
  optGrid.className = "form-grid compact";

  // 移動種別 select
  const moveTypeLbl = makeField("移動種別");
  const moveTypeSelect = document.createElement("select");
  moveTypeSelect.className = "form-input";
  MOVE_TYPE_OPTIONS.forEach(function (o) {
    const opt = document.createElement("option");
    opt.value = String(o.value);
    opt.textContent = o.label;
    if (o.value === 1) { opt.selected = true; }
    moveTypeSelect.appendChild(opt);
  });
  moveTypeLbl.appendChild(moveTypeSelect);
  optGrid.appendChild(moveTypeLbl);

  // 性別
  const sexSpin = createNumberSpinner({ value: 0, min: 0, max: 1, step: 1 });
  const sexLbl = makeField("性別 (0/1)");
  sexLbl.appendChild(sexSpin.el);
  optGrid.appendChild(sexLbl);

  // NPC 画像ID（sprite picker: npc/npc2x2 切替可）
  const sfGrpIdNpc = createSpriteField({
    categoryKey: "npc",
    value: 0,
    label: "NPC 画像ID",
    allowCategorySwitch: true,
  });
  optGrid.appendChild(sfGrpIdNpc.el);

  // 初期 NPC 画像ID
  const sfGrpIdInitNpc = createSpriteField({
    categoryKey: "npc",
    value: 0,
    label: "初期 NPC 画像ID",
    allowCategorySwitch: true,
  });
  optGrid.appendChild(sfGrpIdInitNpc.el);

  // モーション種別ID
  const motionTypeSpin = createNumberSpinner({ value: 0, min: 0, max: 9999, step: 1 });
  const motionTypeLbl = makeField("モーション種別ID");
  motionTypeLbl.appendChild(motionTypeSpin.el);
  optGrid.appendChild(motionTypeLbl);

  // ブロック判定
  const blockLbl = document.createElement("label");
  blockLbl.className = "form-field";
  const blockCb = document.createElement("input");
  blockCb.type = "checkbox";
  blockLbl.append(blockCb, " ブロック判定");
  optGrid.appendChild(blockLbl);

  // 押せる判定
  const pushLbl = document.createElement("label");
  pushLbl.className = "form-field";
  const pushCb = document.createElement("input");
  pushCb.type = "checkbox";
  pushLbl.append(pushCb, " 押せる判定");
  optGrid.appendChild(pushLbl);

  optSection.appendChild(optGrid);
  card.appendChild(optSection);

  // ---- アクションバー ----
  const actionBar = document.createElement("div");
  actionBar.className = "form-actions";
  const submitBtn = document.createElement("button");
  submitBtn.type = "button";
  submitBtn.className = "button primary";
  submitBtn.textContent = "NPC を追加";
  const resetBtn = document.createElement("button");
  resetBtn.type = "button";
  resetBtn.className = "button";
  resetBtn.textContent = "リセット";
  actionBar.append(submitBtn, resetBtn);
  card.appendChild(actionBar);

  // ---- フォームリセット ----
  function resetForm() {
    charNameInput.value = "";
    familyIdInput.value = "";
    mapIdInput.value = "";
    xInput.value = "";
    yInput.value = "";
    moveTypeSelect.value = "1";
    sexSpin.setValue(0);
    sfGrpIdNpc.setValue(0);
    sfGrpIdInitNpc.setValue(0);
    motionTypeSpin.setValue(0);
    blockCb.checked = false;
    pushCb.checked = false;
    showFeedback(feedbackEl, "", "");
  }

  resetBtn.addEventListener("click", resetForm);

  // ---- 送信 ----
  submitBtn.addEventListener("click", async function () {
    const charName = charNameInput.value.trim();
    const familyId = parseInt(familyIdInput.value, 10);
    const mapId    = parseInt(mapIdInput.value, 10);
    const x        = parseInt(xInput.value, 10);
    const y        = parseInt(yInput.value, 10);

    if (!charName) {
      showFeedback(feedbackEl, "キャラ名は必須です", "error");
      return;
    }
    if (isNaN(familyId) || familyId <= 0) {
      showFeedback(feedbackEl, "種族IDは1以上の整数を入力してください", "error");
      return;
    }
    if (isNaN(mapId) || mapId <= 0) {
      showFeedback(feedbackEl, "マップIDは1以上の整数を入力してください", "error");
      return;
    }
    if (isNaN(x) || isNaN(y)) {
      showFeedback(feedbackEl, "座標 X/Y を正しく入力してください", "error");
      return;
    }

    const moveType = parseInt(moveTypeSelect.value, 10);
    const body = {
      charName:     charName,
      familyId:     familyId,
      mapId:        mapId,
      x:            x,
      y:            y,
      moveType:     isNaN(moveType) ? 1 : moveType,
      sex:          sexSpin.getValue(),
      grpIdNpc:     sfGrpIdNpc.getValue(),
      grpIdInitNpc: sfGrpIdInitNpc.getValue(),
      motionTypeId: motionTypeSpin.getValue(),
      block:        blockCb.checked ? 1 : 0,
      push:         pushCb.checked  ? 1 : 0,
    };

    showFeedback(feedbackEl, "追加中...", "");

    try {
      const { response, data } = await fetchJson("/api/characters/npc", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      if (!response.ok) {
        const errMsg = (data && data.error) ? data.error : "追加に失敗しました (HTTP " + response.status + ")";
        showFeedback(feedbackEl, "エラー: " + errMsg, "error");
        return;
      }
      const charId = data && data.charId ? data.charId : "?";
      showFeedback(feedbackEl,
        "NPC を追加しました (charId=" + charId + ", mapId=" + mapId + ", x=" + x + ", y=" + y + ")",
        "success");
      resetForm();
      showFeedback(feedbackEl,
        "NPC を追加しました (charId=" + charId + ", mapId=" + mapId + ", x=" + x + ", y=" + y + ")",
        "success");
    } catch (err) {
      console.error("npc-add submit error", err);
      showFeedback(feedbackEl, "通信エラーが発生しました", "error");
    }
  });

  _destroyFn = function () {
    container.innerHTML = "";
  };
}

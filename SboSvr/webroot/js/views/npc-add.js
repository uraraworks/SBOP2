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
import { withBusy } from "../core/dom.js";
import { showSuccessToast, showErrorToast } from "../components/toast.js";
import { MOVE_TYPE_OPTIONS } from "../data/move-types.js";
import { FAMILY_TYPE_OPTIONS } from "../data/family-types.js";
import { createEntityField, fetchCharacterDetail } from "../components/entity-picker.js";

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

  // 種族ID（FAMILYTYPE_* select。body.familyId として送信する）
  const familyLbl = makeField("種族ID");
  const familySelect = document.createElement("select");
  familySelect.className = "form-input";
  FAMILY_TYPE_OPTIONS.forEach(function (o) {
    const opt = document.createElement("option");
    opt.value = String(o.value);
    opt.textContent = o.label;
    familySelect.appendChild(opt);
  });
  familyLbl.appendChild(familySelect);
  reqGrid.appendChild(familyLbl);

  // テンプレート（既存NPC） picker。選ぶと画像ID・モーション種別・移動種別・
  // sex/block/push に加え、テンプレの種族IDをフォームへ丸ごと反映する。
  // この項目自体は送信しない（あくまでコピー元の指定）。
  const templateField = createEntityField({
    type: "npcTemplate",
    value: 0,
    label: "テンプレート（既存NPC）",
    onChange: (id) => { applyTemplateCopy(id); },
  });
  reqGrid.appendChild(templateField.element);

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

  // モーション種別ID（motionType picker。NPC 画像ID に対応する組み合わせが
  // /api/npc-motion-pairs から取れれば、それで絞り込む）
  let _npcMotionPairs = null; // { fileCount, motionTypes: [{motionTypeId, targetGrpIdNpc, ...}] }
  fetchJson("/api/npc-motion-pairs").then(({ response, data }) => {
    if (response.ok && data) { _npcMotionPairs = data; }
  }).catch(() => { /* 絞り込み無しにフォールバック */ });

  function motionTypeFilter(row) {
    if (!_npcMotionPairs || !Array.isArray(_npcMotionPairs.motionTypes)) return true;
    const grpNpc = sfGrpIdNpc.getValue();
    if (!grpNpc) return true;
    const allowed = _npcMotionPairs.motionTypes
      .filter((e) => Number(e.targetGrpIdNpc) === Number(grpNpc))
      .map((e) => Number(e.motionTypeId));
    if (allowed.length === 0) return true; // 対応情報が無ければ絞り込まない
    return allowed.includes(Number(row.motionTypeId));
  }

  const motionTypeField = createEntityField({
    type: "motionType",
    value: 0,
    label: "モーション種別ID",
    filter: motionTypeFilter,
  });
  optGrid.appendChild(motionTypeField.element);

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

  // テンプレート（npcTemplate）を選んだ時、そのキャラの種族ID・画像ID・
  // モーション種別・移動種別をフォームへコピーする。一覧行には charId/charName
  // しか無いため GET /api/characters/{id} で詳細を取得する。
  async function applyTemplateCopy(id) {
    if (!id) return;
    try {
      const detail = await fetchCharacterDetail(id);
      if (!detail) return;
      if (detail.familyId != null) {
        familySelect.value = String(detail.familyId);
      }
      if (detail.graphics) {
        if (detail.graphics.npc != null) { sfGrpIdNpc.setValue(detail.graphics.npc); }
        if (detail.graphics.initNpc != null) { sfGrpIdInitNpc.setValue(detail.graphics.initNpc); }
      }
      if (detail.movement && detail.movement.motionTypeId != null) {
        motionTypeField.setValue(detail.movement.motionTypeId);
      }
      if (detail.moveType != null) {
        moveTypeSelect.value = String(detail.moveType);
      }
      if (detail.sex != null) {
        sexSpin.setValue(detail.sex);
      }
      if (detail.block != null) { blockCb.checked = !!detail.block; }
      if (detail.push != null) { pushCb.checked = !!detail.push; }
    } catch (err) {
      console.error("npc-add: テンプレコピー失敗", err);
      showErrorToast("種族テンプレの取得に失敗しました", String(err && err.message ? err.message : err));
    }
  }

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
    familySelect.value = "0";
    templateField.setValue(0);
    mapIdInput.value = "";
    xInput.value = "";
    yInput.value = "";
    moveTypeSelect.value = "1";
    sexSpin.setValue(0);
    sfGrpIdNpc.setValue(0);
    sfGrpIdInitNpc.setValue(0);
    motionTypeField.setValue(0);
    blockCb.checked = false;
    pushCb.checked = false;
    showFeedback(feedbackEl, "", "");
  }

  resetBtn.addEventListener("click", resetForm);

  // ---- 送信 ----
  submitBtn.addEventListener("click", async function () {
    const charName = charNameInput.value.trim();
    const familyId = parseInt(familySelect.value, 10);
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
      motionTypeId: motionTypeField.getValue(),
      block:        blockCb.checked ? 1 : 0,
      push:         pushCb.checked  ? 1 : 0,
    };

    showFeedback(feedbackEl, "", "");

    await withBusy(submitBtn, async () => {
      try {
        const { response, data } = await fetchJson("/api/characters/npc", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(body),
        });
        if (!response.ok) {
          const errMsg = (data && data.error) ? data.error : "追加に失敗しました (HTTP " + response.status + ")";
          showFeedback(feedbackEl, "エラー: " + errMsg, "error");
          showErrorToast("NPC 追加に失敗しました", errMsg);
          return;
        }
        const charId = data && data.charId ? data.charId : "?";
        const message = "NPC を追加しました (charId=" + charId + ", mapId=" + mapId + ", x=" + x + ", y=" + y + ")";
        resetForm();
        showSuccessToast(message);
      } catch (err) {
        console.error("npc-add submit error", err);
        showFeedback(feedbackEl, "通信エラーが発生しました", "error");
        showErrorToast("通信エラーが発生しました", String(err && err.message ? err.message : err));
      }
    }, { busyText: "追加中…" });
  });

  _destroyFn = function () {
    container.innerHTML = "";
  };
}

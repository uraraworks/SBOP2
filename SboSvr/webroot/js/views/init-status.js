/**
 * views/init-status.js
 * 初期ステータス設定画面 (route: initial-status)
 *
 * API: /api/initial-status
 *   GET → 現在の初期ステータス JSON
 *   PUT → 更新
 *
 * レイアウト: 単一フォーム（セクション別グループ分け + number-spinner）
 */

import { fetchJson } from "../core/api.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ----------------------------------------------------------------
// フィールド定義（キー, ラベル, min, max, 負値許可）
// ----------------------------------------------------------------

const FIELD_GROUPS = [
  {
    title: "初期位置",
    fields: [
      { key: "mapId", label: "マップID",  min: 0,      max: 9999 },
      { key: "posX",  label: "座標 X",    min: -9999,  max: 9999 },
      { key: "posY",  label: "座標 Y",    min: -9999,  max: 9999 },
    ],
  },
  {
    title: "HP / SP",
    fields: [
      { key: "maxHp", label: "最大HP", min: 0, max: 65535 },
      { key: "maxSp", label: "最大SP", min: 0, max: 65535 },
    ],
  },
  {
    title: "基本能力",
    fields: [
      { key: "stamina",  label: "スタミナ", min: 0, max: 65535 },
      { key: "power",    label: "腕力",     min: 0, max: 65535 },
      { key: "strength", label: "体力",     min: 0, max: 65535 },
      { key: "magic",    label: "魔力",     min: 0, max: 65535 },
      { key: "skillful", label: "器用",     min: 0, max: 65535 },
    ],
  },
  {
    title: "技能",
    fields: [
      { key: "abilityAt", label: "攻撃技能", min: 0, max: 65535 },
      { key: "abilityDf", label: "防御技能", min: 0, max: 65535 },
    ],
  },
  {
    title: "戦闘パラメータ",
    fields: [
      { key: "pAtack",           label: "攻撃力",       min: 0, max: 65535 },
      { key: "pDefense",         label: "防御力",       min: 0, max: 65535 },
      { key: "pMagic",           label: "魔法力",       min: 0, max: 65535 },
      { key: "pMagicDefense",    label: "魔法防御力",   min: 0, max: 65535 },
      { key: "pHitAverage",      label: "命中率",       min: 0, max: 65535 },
      { key: "pAvoidAverage",    label: "回避率",       min: 0, max: 65535 },
      { key: "pCriticalAverage", label: "クリティカル率", min: 0, max: 65535 },
    ],
  },
  {
    title: "属性値",
    fields: [
      { key: "attrFire",  label: "火", min: 0, max: 65535 },
      { key: "attrWind",  label: "風", min: 0, max: 65535 },
      { key: "attrWater", label: "水", min: 0, max: 65535 },
      { key: "attrEarth", label: "土", min: 0, max: 65535 },
      { key: "attrLight", label: "光", min: 0, max: 65535 },
      { key: "attrDark",  label: "闇", min: 0, max: 65535 },
    ],
  },
];

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  // ---- カード ----
  var card = document.createElement("section");
  card.className = "card";
  container.appendChild(card);

  // ---- カードヘッド ----
  var cardHead = document.createElement("div");
  cardHead.className = "card-head";
  var h2 = document.createElement("h2");
  h2.textContent = "初期ステータス設定";
  var cardActions = document.createElement("div");
  cardActions.className = "card-actions";
  var reloadBtn = document.createElement("button");
  reloadBtn.type = "button";
  reloadBtn.className = "button";
  reloadBtn.textContent = "再読み込み";
  cardActions.appendChild(reloadBtn);
  cardHead.append(h2, cardActions);
  card.appendChild(cardHead);

  var desc = document.createElement("p");
  desc.className = "muted";
  desc.textContent = "新規キャラクター作成時に適用される初期パラメータです。保存すると即座にゲームサーバーへ反映されます。";
  card.appendChild(desc);

  var feedbackEl = document.createElement("p");
  feedbackEl.className = "it-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";
  card.appendChild(feedbackEl);

  function showFeedback(msg, type) {
    feedbackEl.textContent = msg || "";
    feedbackEl.className = "it-feedback result-message" + (type ? " " + type : "");
    feedbackEl.style.display = msg ? "" : "none";
  }

  // ---- フォームグリッド ----
  var form = document.createElement("form");
  form.className = "form-grid";
  card.appendChild(form);

  // スピナーレジストリ
  var spinners = {};

  FIELD_GROUPS.forEach(function (group) {
    var section = document.createElement("fieldset");
    section.className = "form-section";
    var legend = document.createElement("legend");
    legend.textContent = group.title;
    section.appendChild(legend);

    group.fields.forEach(function (f) {
      var lbl = document.createElement("label");
      lbl.className = "form-field";
      var span = document.createElement("span");
      span.textContent = f.label;
      lbl.appendChild(span);

      var spin = createNumberSpinner({
        value: 0,
        min: f.min,
        max: f.max,
        step: 1,
      });
      lbl.appendChild(spin.el);
      section.appendChild(lbl);
      spinners[f.key] = spin;
    });

    form.appendChild(section);
  });

  // ---- アクション ----
  var actions = document.createElement("div");
  actions.className = "form-actions";
  var saveBtn = document.createElement("button");
  saveBtn.type = "submit";
  saveBtn.className = "button primary";
  saveBtn.textContent = "保存";
  var revertBtn = document.createElement("button");
  revertBtn.type = "button";
  revertBtn.className = "button";
  revertBtn.textContent = "元に戻す";
  actions.append(saveBtn, revertBtn);
  form.appendChild(actions);

  // ----------------------------------------------------------------
  // 状態
  // ----------------------------------------------------------------
  var _current = null;
  var _isLoading = false;

  // ----------------------------------------------------------------
  // データ反映
  // ----------------------------------------------------------------
  function applyToForm(data) {
    if (!data) { return; }
    FIELD_GROUPS.forEach(function (group) {
      group.fields.forEach(function (f) {
        var v = data[f.key];
        if (v !== undefined && v !== null) {
          spinners[f.key].setValue(Number(v));
        }
      });
    });
  }

  function collectForm() {
    var body = {};
    var err = null;
    FIELD_GROUPS.forEach(function (group) {
      if (err) { return; }
      group.fields.forEach(function (f) {
        if (err) { return; }
        var v = spinners[f.key].getValue();
        if (v === null || v === undefined || !Number.isFinite(v) || !Number.isInteger(v)) {
          err = f.key + " に整数を指定してください";
          return;
        }
        if (f.min !== undefined && v < f.min) {
          err = f.label + " は " + f.min + " 以上で指定してください";
          return;
        }
        if (f.max !== undefined && v > f.max) {
          err = f.label + " は " + f.max + " 以下で指定してください";
          return;
        }
        body[f.key] = v;
      });
    });
    return err ? { error: err } : { body };
  }

  // ----------------------------------------------------------------
  // ロード
  // ----------------------------------------------------------------
  async function loadData(forceReload) {
    if (_isLoading) { return; }
    _isLoading = true;
    if (forceReload) { showFeedback("読み込み中...", ""); }
    try {
      var { response, data } = await fetchJson("/api/initial-status");
      if (!response.ok || !data) {
        throw new Error("HTTP " + response.status);
      }
      _current = data;
      applyToForm(data);
      showFeedback(forceReload ? "読み込みました" : "", forceReload ? "success" : "");
    } catch (err) {
      console.error("init-status load error", err);
      showFeedback("読み込みに失敗しました", "error");
    } finally {
      _isLoading = false;
    }
  }

  // ----------------------------------------------------------------
  // 保存
  // ----------------------------------------------------------------
  form.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    var collected = collectForm();
    if (collected.error) {
      showFeedback(collected.error, "error");
      return;
    }
    showFeedback("保存中...", "");
    try {
      var { response, data } = await fetchJson("/api/initial-status", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(collected.body),
      });
      if (!response.ok || !data) {
        var reason = (data && data.error) ? data.error : "HTTP " + response.status;
        showFeedback("保存に失敗しました: " + reason, "error");
        return;
      }
      _current = data;
      applyToForm(data);
      showFeedback("保存しました", "success");
    } catch (err) {
      console.error("init-status save error", err);
      showFeedback("通信エラーが発生しました", "error");
    }
  });

  // ---- ボタン ----
  reloadBtn.addEventListener("click", function () { loadData(true); });
  revertBtn.addEventListener("click", function () {
    if (_current) {
      applyToForm(_current);
      showFeedback("フォームを前回値に戻しました", "");
    } else {
      loadData(true);
    }
  });

  // 初回自動ロード
  loadData(false);

  _destroyFn = function () {
    container.innerHTML = "";
  };
}

/**
 * views/map-gen-patterns.js
 * 自動生成マップ設定 一覧・編集画面 (route: map-gen-patterns)
 *
 * API: /api/map-gen-patterns
 *   GET    → { items: [{ patternId, patternName, algoType, paramsJson, roleMapJson }] }
 *   POST   → 新規 (patternName 必須)
 *   PUT    → 更新 (patternId 必須)
 *   DELETE → { patternId }
 *
 * paramsJson / roleMapJson は JSON 文字列として往復する。
 * 保存時: JSON.stringify した文字列を body に含める。
 * 読込時: JSON.parse してフォームへ反映（空・不正なら既定値にフォールバック）。
 *
 * ※ プレビュー（お試し生成）は S2 で追加予定。
 *
 * レイアウト: list-detail 骨格（item-types.js と同じ構造）
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createNumberSpinner } from "../components/number-spinner.js";

// ================================================================
// 定数
// ================================================================

/** アルゴリズム種別の選択肢 */
const ALGO_TYPE_LABELS = {
  0: "洞窟(L3)",
  // 将来ここに追加する
};

/** paramsJson の既定値（algoType=0 用） */
const DEFAULT_PARAMS = {
  width: 40,
  height: 40,
  floorAreaMin: 600,
  blockMin: 3,
  blockMax: 4,
  cutoffPercent: 25,
};

/** roleMapJson の既定値 */
const DEFAULT_ROLE_MAP = {
  floor: 0,
  wall: 0,
  door: 0,
  hall: 0,
  stairs: 0,
};

// ================================================================
// ユーティリティ
// ================================================================

/** フィードバックメッセージ表示 */
function showFeedback(el, message, type) {
  el.textContent = message;
  el.className = "it-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

/** label+span のフォームフィールド雛形 */
function makeFormField(labelText) {
  const lbl = document.createElement("label");
  lbl.className = "form-field";
  const span = document.createElement("span");
  span.textContent = labelText;
  lbl.appendChild(span);
  return lbl;
}

/**
 * JSON 文字列を安全にパースし、パース失敗または null なら fallback を返す。
 * @param {string|null|undefined} jsonStr
 * @param {object} fallback
 */
function safeParseJson(jsonStr, fallback) {
  if (!jsonStr) return { ...fallback };
  try {
    const parsed = JSON.parse(jsonStr);
    if (typeof parsed !== "object" || parsed === null) return { ...fallback };
    // 既定値で抜けているキーを補完
    return { ...fallback, ...parsed };
  } catch {
    return { ...fallback };
  }
}

// ================================================================
// 右ペイン: 詳細フォーム
// ================================================================

function buildDetailPane({ feedbackEl }) {
  const pane = document.createElement("div");
  pane.className = "ee-right";

  // フィードバック表示エリア
  pane.appendChild(feedbackEl);

  // 保存バー
  const actionBar = document.createElement("div");
  actionBar.className = "me-action-bar";
  const saveBtn = document.createElement("button");
  saveBtn.type = "button";
  saveBtn.className = "button primary";
  saveBtn.textContent = "保存";
  const cancelBtn = document.createElement("button");
  cancelBtn.type = "button";
  cancelBtn.className = "button";
  cancelBtn.textContent = "キャンセル / 新規";
  actionBar.append(saveBtn, cancelBtn);
  pane.appendChild(actionBar);

  // ================================================================
  // 基本情報セクション
  // ================================================================
  const basicSec = document.createElement("section");
  basicSec.className = "detail-section";
  const basicH3 = document.createElement("h3");
  basicH3.textContent = "基本情報";
  basicSec.appendChild(basicH3);

  const basicGrid = document.createElement("div");
  basicGrid.className = "form-grid compact";

  // パターン名
  const nameLbl = makeFormField("パターン名");
  const nameInput = document.createElement("input");
  nameInput.type = "text";
  nameInput.maxLength = 64;
  nameInput.placeholder = "パターン名（必須）";
  nameInput.className = "form-input";
  nameLbl.appendChild(nameInput);
  basicGrid.appendChild(nameLbl);

  // アルゴリズム種別
  const algoLbl = makeFormField("アルゴリズム種別");
  const algoSelect = document.createElement("select");
  Object.entries(ALGO_TYPE_LABELS).forEach(([v, t]) => {
    const opt = document.createElement("option");
    opt.value = v;
    opt.textContent = v + ": " + t;
    algoSelect.appendChild(opt);
  });
  algoLbl.appendChild(algoSelect);
  basicGrid.appendChild(algoLbl);

  basicSec.appendChild(basicGrid);
  pane.appendChild(basicSec);

  // ================================================================
  // 生成パラメータセクション（algoType=0 用）
  // ================================================================
  const paramSec = document.createElement("section");
  paramSec.className = "detail-section";
  const paramH3 = document.createElement("h3");
  paramH3.textContent = "生成パラメータ（algoType=0: 洞窟L3）";
  paramSec.appendChild(paramH3);

  const paramGrid = document.createElement("div");
  paramGrid.className = "form-grid compact";

  /** スピナー付きフォームフィールドを追加するヘルパ */
  function addParamSpinner(labelText, min, max, defaultVal) {
    const lbl = makeFormField(labelText);
    const spin = createNumberSpinner({ value: defaultVal, min, max, step: 1 });
    lbl.appendChild(spin.el);
    paramGrid.appendChild(lbl);
    return spin;
  }

  const widthSpin        = addParamSpinner("マップ幅 (width)",              1, 9999, DEFAULT_PARAMS.width);
  const heightSpin       = addParamSpinner("マップ高さ (height)",            1, 9999, DEFAULT_PARAMS.height);
  const floorAreaMinSpin = addParamSpinner("最小床面積 (floorAreaMin)",       0, 9999, DEFAULT_PARAMS.floorAreaMin);
  const blockMinSpin     = addParamSpinner("ブロック最小サイズ (blockMin)",   1, 9999, DEFAULT_PARAMS.blockMin);
  const blockMaxSpin     = addParamSpinner("ブロック最大サイズ (blockMax)",   1, 9999, DEFAULT_PARAMS.blockMax);
  const cutoffSpin       = addParamSpinner("カットオフ割合% (cutoffPercent)", 0,  100, DEFAULT_PARAMS.cutoffPercent);

  paramSec.appendChild(paramGrid);
  pane.appendChild(paramSec);

  // ================================================================
  // ロール→マップパーツ割り当てセクション
  // ================================================================
  const roleSec = document.createElement("section");
  roleSec.className = "detail-section";
  const roleH3 = document.createElement("h3");
  roleH3.textContent = "ロール → マップパーツ割り当て";
  roleSec.appendChild(roleH3);

  const roleGrid = document.createElement("div");
  roleGrid.className = "form-grid compact";

  /** ロール名 → ラベルのマッピング（roleMapJson のキー順） */
  const ROLE_LABELS = [
    { key: "floor",  label: "床" },
    { key: "wall",   label: "壁" },
    { key: "door",   label: "ドア" },
    { key: "hall",   label: "廊下" },
    { key: "stairs", label: "階段" },
  ];

  // 各ロールの sprite-picker フィールドを生成
  const roleFields = {};
  ROLE_LABELS.forEach(({ key, label }) => {
    const sf = createSpriteField({
      categoryKey: "map-parts",
      label: label,
      value: 0,
    });
    roleGrid.appendChild(sf.el);
    roleFields[key] = sf;
  });

  roleSec.appendChild(roleGrid);
  pane.appendChild(roleSec);

  // ================================================================
  // S2 プレースホルダー
  // ================================================================
  // ※ プレビュー（お試し生成 / canvas 描画）は S2 で追加予定

  // ================================================================
  // 公開 API
  // ================================================================

  let _current = null;

  /** レコードをフォームに反映する */
  function setItem(it) {
    _current = it || null;

    nameInput.value = it ? (it.patternName || "") : "";
    algoSelect.value = String(it ? (it.algoType ?? 0) : 0);

    // paramsJson: JSON 文字列 → オブジェクト（既定値フォールバック付き）
    const params = safeParseJson(it ? it.paramsJson : null, DEFAULT_PARAMS);
    widthSpin.setValue(params.width);
    heightSpin.setValue(params.height);
    floorAreaMinSpin.setValue(params.floorAreaMin);
    blockMinSpin.setValue(params.blockMin);
    blockMaxSpin.setValue(params.blockMax);
    cutoffSpin.setValue(params.cutoffPercent);

    // roleMapJson: JSON 文字列 → オブジェクト（既定値フォールバック付き）
    const roleMap = safeParseJson(it ? it.roleMapJson : null, DEFAULT_ROLE_MAP);
    ROLE_LABELS.forEach(({ key }) => {
      roleFields[key].setValue(roleMap[key] ?? 0);
    });
  }

  /** フォームの現在値を API 送信用オブジェクトに変換する */
  function collectData() {
    // paramsJson: オブジェクト → JSON 文字列
    const paramsObj = {
      width:          widthSpin.getValue(),
      height:         heightSpin.getValue(),
      floorAreaMin:   floorAreaMinSpin.getValue(),
      blockMin:       blockMinSpin.getValue(),
      blockMax:       blockMaxSpin.getValue(),
      cutoffPercent:  cutoffSpin.getValue(),
    };

    // roleMapJson: オブジェクト → JSON 文字列
    const roleMapObj = {};
    ROLE_LABELS.forEach(({ key }) => {
      roleMapObj[key] = roleFields[key].getValue();
    });

    return {
      patternName: nameInput.value,
      algoType:    Number(algoSelect.value),
      paramsJson:  JSON.stringify(paramsObj),
      roleMapJson: JSON.stringify(roleMapObj),
    };
  }

  function getCurrent() { return _current; }
  function setCurrent(it) { _current = it; }

  return { el: pane, saveBtn, cancelBtn, setItem, collectData, getCurrent, setCurrent };
}

// ================================================================
// 左ペイン: 一覧
// ================================================================

function buildLeftPane({ onSelect, onNew, onDelete }) {
  const pane = document.createElement("div");
  pane.className = "ee-left";

  // 検索ボックス
  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "ID または 名前で検索…";
  searchInput.className = "ld-search";
  pane.appendChild(searchInput);

  // ツールバー（新規追加ボタン）
  const toolbar = document.createElement("div");
  toolbar.className = "me-list-toolbar";
  const newBtn = document.createElement("button");
  newBtn.type = "button";
  newBtn.className = "button small";
  newBtn.textContent = "+ 新規追加";
  newBtn.addEventListener("click", onNew);
  toolbar.appendChild(newBtn);
  pane.appendChild(toolbar);

  // 一覧リスト
  const listEl = document.createElement("ul");
  listEl.className = "ld-list";
  pane.appendChild(listEl);

  // 件数表示
  const summary = document.createElement("p");
  summary.className = "muted me-list-summary";
  pane.appendChild(summary);

  let _allItems = [];
  let _selectedId = null;

  /** API から一覧を再取得してレンダリング */
  async function loadList() {
    const { response, data } = await fetchJson("/api/map-gen-patterns");
    if (!response.ok || !Array.isArray(data?.items)) {
      console.error("map-gen-patterns load error");
      return;
    }
    _allItems = data.items;
    renderList();
  }

  /** 検索フィルタを適用してリストを描画する */
  function renderList() {
    const q = searchInput.value.trim().toLowerCase();
    const filtered = _allItems.filter((it) => {
      if (!q) return true;
      return String(it.patternId).includes(q) || (it.patternName || "").toLowerCase().includes(q);
    });

    listEl.innerHTML = "";
    filtered.forEach((it) => {
      const li = document.createElement("li");
      li.className = "ld-list-item" + (it.patternId === _selectedId ? " selected" : "");

      const label = document.createElement("span");
      label.textContent = "[" + it.patternId + "] " + (it.patternName || "(名前なし)");
      li.appendChild(label);

      li.addEventListener("click", () => {
        _selectedId = it.patternId;
        renderList();
        onSelect(it);
      });

      const delBtn = document.createElement("button");
      delBtn.type = "button";
      delBtn.className = "ld-item-del button small";
      delBtn.textContent = "削除";
      delBtn.addEventListener("click", (ev) => { ev.stopPropagation(); onDelete(it); });
      li.appendChild(delBtn);

      listEl.appendChild(li);
    });

    summary.textContent = filtered.length + " 件";
  }

  searchInput.addEventListener("input", renderList);
  loadList();

  return { el: pane, reload: loadList };
}

// ================================================================
// mount（画面エントリポイント）
// ================================================================

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const shell = document.createElement("div");
  shell.className = "me-shell";
  container.appendChild(shell);

  const feedbackEl = document.createElement("p");
  feedbackEl.className = "it-feedback result-message";
  feedbackEl.setAttribute("role", "status");
  feedbackEl.setAttribute("aria-live", "polite");
  feedbackEl.style.display = "none";

  const detail = buildDetailPane({ feedbackEl });

  // 詳細ペイン先頭に「← 戻る」ボタンを追加
  const backBar = document.createElement("div");
  backBar.className = "me-action-bar";
  const backBtn = document.createElement("button");
  backBtn.type = "button";
  backBtn.className = "button small";
  backBtn.textContent = "← 戻る";
  backBar.appendChild(backBtn);
  detail.el.insertBefore(backBar, detail.el.firstChild);

  // 一覧↔詳細の表示切替ヘルパ
  function showDetail() {
    leftApi.el.style.display = "none";
    detail.el.style.display = "";
  }
  function showList() {
    detail.el.style.display = "none";
    leftApi.el.style.display = "";
  }

  backBtn.addEventListener("click", showList);

  // 保存ボタン
  detail.saveBtn.addEventListener("click", async () => {
    const payload = detail.collectData();
    const current = detail.getCurrent();
    const isNew = !current?.patternId;

    if (!payload.patternName.trim()) {
      showFeedback(feedbackEl, "パターン名は必須です", "error");
      return;
    }

    if (!isNew) { payload.patternId = current.patternId; }
    showFeedback(feedbackEl, isNew ? "追加中…" : "保存中…", "");

    try {
      const { response, data } = await fetchJson("/api/map-gen-patterns", {
        method: isNew ? "POST" : "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload),
      });
      if (!response.ok) {
        showFeedback(feedbackEl, "エラー: " + (data?.error ?? "HTTP " + response.status), "error");
        return;
      }
      showFeedback(feedbackEl, isNew ? "追加しました" : "保存しました", "success");
      if (isNew && data?.patternId) {
        detail.setCurrent({ ...payload, patternId: data.patternId });
      }
      await leftApi.reload();
    } catch (e) {
      showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
    }
  });

  // キャンセル/新規ボタン → フォームクリア + 一覧に戻る
  detail.cancelBtn.addEventListener("click", () => {
    detail.setItem(null);
    showFeedback(feedbackEl, "", "");
    showList();
  });

  const leftApi = buildLeftPane({
    onSelect: (it) => {
      detail.setItem(it);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onNew: () => {
      detail.setItem(null);
      showFeedback(feedbackEl, "", "");
      showDetail();
    },
    onDelete: async (it) => {
      if (!confirm("パターン [" + (it.patternName || "") + "] (ID=" + it.patternId + ") を削除しますか？")) return;
      try {
        const { response, data } = await fetchJson("/api/map-gen-patterns", {
          method: "DELETE",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ patternId: it.patternId }),
        });
        if (!response.ok) {
          showFeedback(feedbackEl, "削除エラー: " + (data?.error ?? "HTTP " + response.status), "error");
          return;
        }
        showFeedback(feedbackEl, "削除しました (ID=" + it.patternId + ")", "success");
        if (detail.getCurrent()?.patternId === it.patternId) {
          detail.setItem(null);
        }
        await leftApi.reload();
      } catch (e) {
        showFeedback(feedbackEl, "通信エラー: " + e.message, "error");
      }
    },
  });

  // 初期状態: 一覧のみ表示
  detail.el.style.display = "none";

  shell.appendChild(leftApi.el);
  shell.appendChild(detail.el);

  _destroyFn = () => {
    container.innerHTML = "";
  };
}

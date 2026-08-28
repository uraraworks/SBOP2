/**
 * views/image-editor.js
 * 画像エディタ画面 (route: image-editor)
 *
 * API:
 *   GET    /api/image-categories
 *   GET    /api/assets/sprites/{key}/{index}            (PNG本体)
 *   PUT    /api/assets/sprites/{key}/{index}             (PNG差し替え)
 *   GET    /api/assets/sprites/{key}/{index}/history
 *   POST   /api/assets/sprites/{key}/{index}/revert      { revision }
 *   DELETE /api/assets/sprites/{key}/{index}             (出荷時に戻す)
 *
 * 左: カテゴリ一覧 + シート選択(複数枚のカテゴリのみ)
 * 右: プレビュー(実寸ベースのグリッド) + 状態表示 + アップロード + 履歴 + 出荷時復元
 *
 * 重要: グリッド計算にカタログの countX/countY は使わない(サブID割付け用の論理値で
 * 実画像サイズと一致しないカテゴリがあるため)。実際に読み込んだ画像の
 * naturalWidth/naturalHeight と cellSize からグリッドを算出する。
 */

import { fetchJson } from "../core/api.js";
import { createCharComposer } from "../components/char-composer.js";

const MAX_UPLOAD_BYTES = 2 * 1024 * 1024; // 2MB
const PNG_SIGNATURE = [0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];

// ----------------------------------------------------------------
// フィードバック表示
// ----------------------------------------------------------------

function showFeedback(el, message, type) {
  el.textContent = message || "";
  el.className = "ie-feedback result-message" + (type ? " " + type : "");
  el.style.display = message ? "" : "none";
}

// ----------------------------------------------------------------
// 日時整形 (Unix epoch 秒 → ローカル時刻文字列)
// ----------------------------------------------------------------

function formatEpochSeconds(sec) {
  if (sec == null || !Number.isFinite(Number(sec))) return "-";
  const d = new Date(Number(sec) * 1000);
  if (Number.isNaN(d.getTime())) return "-";
  const pad = (n) => String(n).padStart(2, "0");
  return `${d.getFullYear()}/${pad(d.getMonth() + 1)}/${pad(d.getDate())} ` +
    `${pad(d.getHours())}:${pad(d.getMinutes())}:${pad(d.getSeconds())}`;
}

function formatBytes(bytes) {
  const n = Number(bytes);
  if (!Number.isFinite(n)) return "-";
  if (n < 1024) return `${n} B`;
  return `${(n / 1024).toFixed(1)} KB`;
}

// ----------------------------------------------------------------
// サーバーエラーの日本語メッセージ化
// ----------------------------------------------------------------

function describeUploadError(response, data) {
  if (response.status === 401) {
    return "セッションが切れました。再度ログインしてください";
  }
  if (response.status === 403) {
    return "権限がありません(IMAGE_EDIT ロールが必要です)";
  }
  const err = data?.error;
  if (response.status === 400 && err === "invalid_png") {
    return "PNG として読み込めませんでした";
  }
  if (response.status === 400 && err === "size_mismatch") {
    const exp = data?.expected;
    const act = data?.actual;
    const expStr = exp ? `${exp.width}x${exp.height}` : "?";
    const actStr = act ? `${act.width}x${act.height}` : "?";
    return `画像サイズが一致しません(現在: ${expStr} / 指定: ${actStr})`;
  }
  if (response.status === 409 && err === "sheet_not_found") {
    return data?.message || "新規シートの追加は未対応です";
  }
  if (response.status === 413) {
    return "ファイルサイズが大きすぎます(2MB 以下にしてください)";
  }
  if (response.status === 415) {
    return "PNG 以外のファイルは指定できません";
  }
  return err ? `アップロードに失敗しました: ${err}` : `アップロードに失敗しました(HTTP ${response.status})`;
}

// ----------------------------------------------------------------
// クライアント側バリデーション
// ----------------------------------------------------------------

async function readHeaderBytes(file, len) {
  const slice = file.slice(0, len);
  const buf = await slice.arrayBuffer();
  return new Uint8Array(buf);
}

async function getImageDimensions(file) {
  if (typeof createImageBitmap === "function") {
    try {
      const bmp = await createImageBitmap(file);
      const dims = { width: bmp.width, height: bmp.height };
      if (typeof bmp.close === "function") bmp.close();
      return dims;
    } catch {
      // フォールバックへ
    }
  }
  return new Promise((resolve, reject) => {
    const url = URL.createObjectURL(file);
    const img = new Image();
    img.onload = () => {
      URL.revokeObjectURL(url);
      resolve({ width: img.naturalWidth, height: img.naturalHeight });
    };
    img.onerror = () => {
      URL.revokeObjectURL(url);
      reject(new Error("画像として読み込めませんでした"));
    };
    img.src = url;
  });
}

/**
 * アップロード前検証。
 * @returns {Promise<{ok: true} | {ok: false, message: string}>}
 */
async function validateUploadFile(file, expectedWidth, expectedHeight) {
  if (!file) return { ok: false, message: "ファイルが選択されていません" };

  const nameLower = (file.name || "").toLowerCase();
  const extOk = nameLower.endsWith(".png");
  const mimeOk = !file.type || file.type === "image/png";
  if (!extOk || !mimeOk) {
    return { ok: false, message: "PNG ファイルを指定してください" };
  }

  if (file.size > MAX_UPLOAD_BYTES) {
    return { ok: false, message: `ファイルサイズが 2MB を超えています(${formatBytes(file.size)})` };
  }

  try {
    const header = await readHeaderBytes(file, 8);
    const isPng = header.length === 8 && PNG_SIGNATURE.every((b, i) => header[i] === b);
    if (!isPng) {
      return { ok: false, message: "PNG シグネチャが確認できません(壊れているか PNG ではありません)" };
    }
  } catch {
    return { ok: false, message: "ファイルの読み込みに失敗しました" };
  }

  if (expectedWidth && expectedHeight) {
    try {
      const dims = await getImageDimensions(file);
      if (dims.width !== expectedWidth || dims.height !== expectedHeight) {
        return {
          ok: false,
          message: `現在の画像は ${expectedWidth}x${expectedHeight} です。同じ寸法の PNG を指定してください` +
            `(指定されたファイル: ${dims.width}x${dims.height})`,
        };
      }
    } catch (e) {
      return { ok: false, message: e?.message || "画像の寸法を確認できませんでした" };
    }
  }

  return { ok: true };
}

// ----------------------------------------------------------------
// 左ペイン: カテゴリ一覧
// ----------------------------------------------------------------

function buildCategoryList({ categories, onSelect }) {
  const ul = document.createElement("ul");
  ul.className = "ld-list";

  let _selectedKey = null;
  let _selectedIndex = 0;
  const overriddenMarks = new Map(); // "key/index" -> boolean

  function markKey(key, index) { return `${key}/${index}`; }

  function render() {
    ul.innerHTML = "";
    categories.forEach((cat) => {
      const li = document.createElement("li");
      li.className = "ld-list-item image-editor-cat-item";
      const nameSpan = document.createElement("span");
      nameSpan.className = "image-editor-cat-label";
      nameSpan.textContent = cat.label || cat.key;
      li.appendChild(nameSpan);

      if (cat.sheetCount > 1) {
        const select = document.createElement("select");
        select.className = "image-editor-sheet-select";
        for (let i = 0; i < cat.sheetCount; i++) {
          const opt = document.createElement("option");
          opt.value = String(i);
          opt.textContent = `#${i}`;
          if (overriddenMarks.get(markKey(cat.key, i))) {
            opt.textContent += " *";
          }
          select.appendChild(opt);
        }
        select.addEventListener("click", (e) => e.stopPropagation());
        select.addEventListener("change", () => {
          _selectedKey = cat.key;
          _selectedIndex = Number(select.value) || 0;
          highlight();
          onSelect(cat, _selectedIndex);
        });
        li.appendChild(select);
        li._sheetSelect = select;
      } else {
        const idx0Mark = document.createElement("span");
        idx0Mark.className = "image-editor-overridden-mark";
        idx0Mark.textContent = overriddenMarks.get(markKey(cat.key, 0)) ? "上書き中" : "";
        li.appendChild(idx0Mark);
        li._mark0 = idx0Mark;
      }

      li.addEventListener("click", () => {
        _selectedKey = cat.key;
        _selectedIndex = li._sheetSelect ? (Number(li._sheetSelect.value) || 0) : 0;
        highlight();
        onSelect(cat, _selectedIndex);
      });

      li._catKey = cat.key;
      ul.appendChild(li);
    });
  }

  function highlight() {
    ul.querySelectorAll(".ld-list-item").forEach((li) => {
      li.classList.toggle("is-selected", li._catKey === _selectedKey);
    });
  }

  function setOverriddenMark(key, index, overridden) {
    overriddenMarks.set(markKey(key, index), overridden);
    // 該当行を再描画
    const li = Array.from(ul.querySelectorAll(".ld-list-item")).find((el) => el._catKey === key);
    if (!li) return;
    if (li._mark0) {
      li._mark0.textContent = overridden ? "上書き中" : "";
    } else if (li._sheetSelect) {
      const opt = li._sheetSelect.querySelector(`option[value="${index}"]`);
      if (opt) {
        const base = `#${index}`;
        opt.textContent = overridden ? `${base} *` : base;
      }
    }
  }

  render();

  return { el: ul, setOverriddenMark };
}

// ----------------------------------------------------------------
// 右ペイン: プレビュー + 詳細
// ----------------------------------------------------------------

function buildDetailPane({ onOverriddenChange, categories }) {
  const pane = document.createElement("div");
  pane.className = "ie-right";

  let _cat = null;
  let _index = 0;
  let _naturalWidth = 0;
  let _naturalHeight = 0;
  let _scale = 2;
  let _showGrid = true;
  let _revision = 0;

  // --- フィードバック ---
  const feedback = document.createElement("p");
  feedback.className = "ie-feedback result-message";
  feedback.style.display = "none";
  feedback.setAttribute("role", "status");
  feedback.setAttribute("aria-live", "polite");
  pane.appendChild(feedback);

  // --- プレビューセクション ---
  const previewSec = document.createElement("section");
  previewSec.className = "detail-section";
  const previewH3 = document.createElement("h3");
  previewH3.textContent = "プレビュー";
  previewSec.appendChild(previewH3);

  const previewToolbar = document.createElement("div");
  previewToolbar.className = "ie-preview-toolbar";

  const scaleSelect = document.createElement("select");
  [1, 2, 4, 8].forEach((s) => {
    const opt = document.createElement("option");
    opt.value = String(s);
    opt.textContent = `${s}x`;
    if (s === _scale) opt.selected = true;
    scaleSelect.appendChild(opt);
  });
  scaleSelect.addEventListener("change", () => {
    _scale = Number(scaleSelect.value) || 1;
    applyImageTransform();
  });

  const gridToggleLabel = document.createElement("label");
  gridToggleLabel.className = "ie-grid-toggle";
  const gridToggleCb = document.createElement("input");
  gridToggleCb.type = "checkbox";
  gridToggleCb.checked = _showGrid;
  gridToggleCb.addEventListener("change", () => {
    _showGrid = gridToggleCb.checked;
    applyGridVisibility();
  });
  gridToggleLabel.append(gridToggleCb, " グリッド表示");

  previewToolbar.append(
    (() => { const s = document.createElement("span"); s.textContent = "拡大率:"; return s; })(),
    scaleSelect,
    gridToggleLabel
  );
  previewSec.appendChild(previewToolbar);

  const previewStage = document.createElement("div");
  previewStage.className = "ie-preview-stage";
  const previewInner = document.createElement("div");
  previewInner.className = "ie-preview-inner";
  const previewImg = document.createElement("img");
  previewImg.alt = "スプライトシート プレビュー";
  previewImg.className = "ie-preview-img";
  const gridOverlay = document.createElement("div");
  gridOverlay.className = "ie-grid-overlay";
  previewInner.append(previewImg, gridOverlay);
  previewStage.appendChild(previewInner);
  previewSec.appendChild(previewStage);

  // ドラッグ&ドロップ受付
  previewStage.addEventListener("dragover", (e) => {
    e.preventDefault();
    previewStage.classList.add("is-drag-over");
  });
  previewStage.addEventListener("dragleave", () => {
    previewStage.classList.remove("is-drag-over");
  });
  previewStage.addEventListener("drop", (e) => {
    e.preventDefault();
    previewStage.classList.remove("is-drag-over");
    const file = e.dataTransfer?.files?.[0];
    if (file) handleFileSelected(file);
  });

  const dimInfo = document.createElement("p");
  dimInfo.className = "ie-dim-info";
  previewSec.appendChild(dimInfo);

  pane.appendChild(previewSec);

  // --- 合成プレビュー(S3b) ---
  // 2x2 キャラ系カテゴリのときだけ表示される。setTarget が false を返すと自分で隠れる。
  const composer = createCharComposer({ categories });
  pane.appendChild(composer.el);

  function applyImageTransform() {
    if (!_naturalWidth || !_naturalHeight) return;
    const w = _naturalWidth * _scale;
    const h = _naturalHeight * _scale;
    previewInner.style.width = `${w}px`;
    previewInner.style.height = `${h}px`;
    previewImg.style.width = `${w}px`;
    previewImg.style.height = `${h}px`;
    gridOverlay.style.backgroundSize = `${(_cat?.cellSize || 32) * _scale}px ${(_cat?.cellSize || 32) * _scale}px`;
  }

  function applyGridVisibility() {
    gridOverlay.style.display = _showGrid ? "" : "none";
  }

  function updateDimInfo() {
    if (!_cat || !_naturalWidth || !_naturalHeight) {
      dimInfo.textContent = "";
      return;
    }
    const cellSize = _cat.cellSize || 32;
    const cols = Math.max(1, Math.round(_naturalWidth / cellSize));
    const rows = Math.max(1, Math.round(_naturalHeight / cellSize));
    dimInfo.textContent =
      `実寸: ${_naturalWidth}x${_naturalHeight}px / セルサイズ: ${cellSize}px / ` +
      `セル数(実寸から算出): 横${cols} x 縦${rows}`;
  }

  // --- 状態表示セクション ---
  const stateSec = document.createElement("section");
  stateSec.className = "detail-section";
  const stateH3 = document.createElement("h3");
  stateH3.textContent = "状態";
  stateSec.appendChild(stateH3);
  const stateText = document.createElement("p");
  stateText.className = "ie-state-text";
  stateSec.appendChild(stateText);
  pane.appendChild(stateSec);

  // --- アップロードセクション ---
  const uploadSec = document.createElement("section");
  uploadSec.className = "detail-section";
  const uploadH3 = document.createElement("h3");
  uploadH3.textContent = "差し替え";
  uploadSec.appendChild(uploadH3);

  const uploadHint = document.createElement("p");
  uploadHint.className = "card-description";
  uploadHint.textContent = "PNG ファイルを選択するか、上のプレビューにドラッグ&ドロップしてください(2MB 以下、同じ寸法のみ)";
  uploadSec.appendChild(uploadHint);

  const fileField = document.createElement("label");
  fileField.className = "form-field";
  const fileFieldSpan = document.createElement("span");
  fileFieldSpan.textContent = "PNG ファイル";
  const fileInput = document.createElement("input");
  fileInput.type = "file";
  fileInput.accept = "image/png";
  fileField.append(fileFieldSpan, fileInput);
  uploadSec.appendChild(fileField);

  fileInput.addEventListener("change", () => {
    const file = fileInput.files?.[0];
    if (file) handleFileSelected(file);
    fileInput.value = "";
  });

  pane.appendChild(uploadSec);

  // --- 履歴セクション ---
  const historySec = document.createElement("section");
  historySec.className = "detail-section";
  const historyH3 = document.createElement("h3");
  historyH3.textContent = "履歴";
  historySec.appendChild(historyH3);
  const historyBody = document.createElement("div");
  historySec.appendChild(historyBody);
  pane.appendChild(historySec);

  // --- 出荷時復元セクション ---
  const revertSec = document.createElement("section");
  revertSec.className = "detail-section";
  const revertH3 = document.createElement("h3");
  revertH3.textContent = "出荷時に戻す";
  revertSec.appendChild(revertH3);
  const revertBtn = document.createElement("button");
  revertBtn.type = "button";
  revertBtn.className = "button danger";
  revertBtn.textContent = "出荷時の画像に戻す";
  revertSec.appendChild(revertBtn);
  pane.appendChild(revertSec);

  revertBtn.addEventListener("click", async () => {
    if (!_cat) return;
    if (!confirm("上書きと履歴がすべて削除されます。よろしいですか?")) return;
    try {
      const { response, data } = await fetchJson(
        `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}`,
        { method: "DELETE" }
      );
      if (!response.ok) {
        showFeedback(feedback, describeUploadError(response, data), "error");
        return;
      }
      showFeedback(feedback, "出荷時の画像に戻しました", "success");
      await reload();
    } catch (e) {
      showFeedback(feedback, "通信に失敗しました: " + String(e?.message ?? e), "error");
    }
  });

  async function handleFileSelected(file) {
    if (!_cat) return;
    const validation = await validateUploadFile(file, _naturalWidth, _naturalHeight);
    if (!validation.ok) {
      showFeedback(feedback, validation.message, "error");
      return;
    }
    try {
      const buf = await file.arrayBuffer();
      const response = await fetch(
        `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}`,
        {
          method: "PUT",
          credentials: "include",
          headers: { "Content-Type": "image/png" },
          body: buf,
        }
      );
      let data = null;
      try { data = await response.json(); } catch { data = null; }
      if (!response.ok) {
        showFeedback(feedback, describeUploadError(response, data), "error");
        return;
      }
      showFeedback(feedback, "アップロードしました", "success");
      await reload();
    } catch (e) {
      showFeedback(feedback, "通信に失敗しました(接続がリセットされた場合はファイルサイズをご確認ください): " + String(e?.message ?? e), "error");
    }
  }

  function renderHistory(historyData) {
    historyBody.innerHTML = "";
    const items = historyData?.history ?? [];
    if (!items.length) {
      const p = document.createElement("p");
      p.className = "card-description";
      p.textContent = "履歴はありません";
      historyBody.appendChild(p);
      return;
    }
    const table = document.createElement("table");
    table.className = "data-table";
    const thead = document.createElement("thead");
    thead.innerHTML = "<tr><th>版</th><th>保存日時</th><th>保存者</th><th>サイズ</th><th>操作</th></tr>";
    table.appendChild(thead);
    const tbody = document.createElement("tbody");
    items.slice().sort((a, b) => b.revision - a.revision).forEach((h) => {
      const tr = document.createElement("tr");
      const tdRev = document.createElement("td");
      tdRev.textContent = String(h.revision);
      const tdDate = document.createElement("td");
      tdDate.textContent = formatEpochSeconds(h.savedAt);
      const tdUser = document.createElement("td");
      tdUser.textContent = h.savedBy || "-";
      const tdSize = document.createElement("td");
      tdSize.textContent = formatBytes(h.bytes);
      const tdOp = document.createElement("td");
      const revertBtnRow = document.createElement("button");
      revertBtnRow.type = "button";
      revertBtnRow.className = "button small";
      revertBtnRow.textContent = "この版に戻す";
      revertBtnRow.addEventListener("click", async () => {
        if (!confirm(`版 ${h.revision} に戻しますか?`)) return;
        try {
          const { response, data } = await fetchJson(
            `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}/revert`,
            {
              method: "POST",
              headers: { "Content-Type": "application/json" },
              body: JSON.stringify({ revision: h.revision }),
            }
          );
          if (!response.ok) {
            const msg = response.status === 404 && data?.error === "revision_not_found"
              ? "指定した版が見つかりません"
              : describeUploadError(response, data);
            showFeedback(feedback, msg, "error");
            return;
          }
          showFeedback(feedback, `版 ${h.revision} に戻しました`, "success");
          await reload();
        } catch (e) {
          showFeedback(feedback, "通信に失敗しました: " + String(e?.message ?? e), "error");
        }
      });
      tdOp.appendChild(revertBtnRow);
      tr.append(tdRev, tdDate, tdUser, tdSize, tdOp);
      tbody.appendChild(tr);
    });
    table.appendChild(tbody);
    historyBody.appendChild(table);
  }

  async function reload() {
    if (!_cat) return;
    showFeedback(feedback, "", null);

    // 差し替え/復元の直後も呼ばれるため、合成プレビューの画像キャッシュを捨ててから貼り直す
    composer.invalidate({ redraw: false });
    composer.setTarget(_cat, _index);

    // 画像本体
    const cacheBust = Date.now();
    previewImg.onload = () => {
      _naturalWidth = previewImg.naturalWidth;
      _naturalHeight = previewImg.naturalHeight;
      applyImageTransform();
      applyGridVisibility();
      updateDimInfo();
    };
    previewImg.src = `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}?v=${cacheBust}`;

    // 履歴 + 現在の状態
    try {
      const { response, data } = await fetchJson(
        `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}/history`
      );
      if (!response.ok) {
        stateText.textContent = "状態を取得できませんでした";
        renderHistory(null);
        return;
      }
      const overridden = Boolean(data?.overridden);
      _revision = data?.current?.revision ?? 0;
      if (overridden && data?.current) {
        const cur = data.current;
        stateText.textContent =
          `上書き中 (rev ${cur.revision}, 更新者: ${cur.updatedBy || "-"}, ` +
          `更新日時: ${formatEpochSeconds(cur.updatedAt)})`;
      } else {
        stateText.textContent = "出荷時の画像";
      }
      revertBtn.disabled = !overridden;
      renderHistory(data);
      onOverriddenChange(_cat.key, _index, overridden);
      // revision が変わった場合に備え src へ v= を付け直す(ETag/no-cache でも念のため)
      if (_revision) {
        previewImg.src = `/api/assets/sprites/${encodeURIComponent(_cat.key)}/${_index}?v=${_revision}`;
      }
    } catch (e) {
      stateText.textContent = "状態の取得に失敗しました: " + String(e?.message ?? e);
      renderHistory(null);
    }
  }

  function setTarget(cat, index) {
    _cat = cat;
    _index = index;
    _naturalWidth = 0;
    _naturalHeight = 0;
    reload();
  }

  applyGridVisibility();

  return { el: pane, setTarget, destroy: () => composer.destroy() };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  const shell = document.createElement("div");
  shell.className = "list-detail-shell image-editor-shell";

  const listPane = document.createElement("div");
  listPane.className = "ld-list-pane";
  const listHeader = document.createElement("div");
  listHeader.className = "ld-list-header";
  const listTitle = document.createElement("div");
  listTitle.className = "ld-list-pane-title";
  listTitle.textContent = "カテゴリ";
  listHeader.appendChild(listTitle);
  listPane.appendChild(listHeader);

  const listLoading = document.createElement("p");
  listLoading.className = "card-description";
  listLoading.style.padding = "0.6rem 0.75rem";
  listLoading.textContent = "読み込み中...";
  listPane.appendChild(listLoading);

  const detailPane = document.createElement("div");
  detailPane.className = "ld-detail-pane";
  const emptyMsg = document.createElement("p");
  emptyMsg.className = "card-description";
  emptyMsg.style.padding = "1rem";
  emptyMsg.textContent = "左のカテゴリを選択してください";
  detailPane.appendChild(emptyMsg);

  shell.append(listPane, detailPane);
  container.appendChild(shell);

  let detail = null;

  async function load() {
    try {
      const { response, data } = await fetchJson("/api/image-categories");
      if (!response.ok) {
        listLoading.textContent = "カテゴリ一覧の取得に失敗しました";
        return;
      }
      const categories = (data?.categories ?? []).filter((c) => Number(c.sheetCount) > 0);
      listLoading.remove();

      const listUi = buildCategoryList({
        categories,
        onSelect: (cat, index) => {
          if (!detail) {
            detailPane.innerHTML = "";
            detail = buildDetailPane({
              onOverriddenChange: (key, idx, overridden) => listUi.setOverriddenMark(key, idx, overridden),
              categories,
            });
            detailPane.appendChild(detail.el);
          }
          detail.setTarget(cat, index);
        },
      });
      listPane.appendChild(listUi.el);
    } catch (e) {
      listLoading.textContent = "カテゴリ一覧の取得に失敗しました: " + String(e?.message ?? e);
    }
  }

  load();

  // 画面を離れる時にアニメのタイマーを止める
  _destroyFn = () => {
    if (detail?.destroy) detail.destroy();
    container.innerHTML = "";
    detail = null;
  };
}

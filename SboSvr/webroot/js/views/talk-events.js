/**
 * views/talk-events.js
 * 会話イベント編集画面 (route: talk-events)
 *
 * API:
 *   GET    /api/talk-events         → 一覧 { talkEvents:[{id,pageCount,eventCount}] }
 *   GET    /api/talk-events?id=N    → 詳細 { events:[...] }
 *   PUT    /api/talk-events         → 保存
 *   DELETE /api/talk-events?id=N   → 削除
 *
 * 会話イベント editor モーダル (#talk-event-modal) は index.html に存在する。
 * このビューが mount() された後にモーダルのイベントをバインドする。
 * data-talk-event ボタンによる editor 起動は全画面共通で
 * app.js の bindTalkEventEditor が担当する（重複バインド不要）。
 */

import { fetchJson } from "../core/api.js";

// ----------------------------------------------------------------
// コマンド種別定数
// ----------------------------------------------------------------
const TALK_EVENT_TYPE_NONE     = 0;
const TALK_EVENT_TYPE_PAGE     = 1;
const TALK_EVENT_TYPE_MSG      = 2;
const TALK_EVENT_TYPE_MENU     = 3;
const TALK_EVENT_TYPE_ADDSKILL = 4;

const TALK_EVENT_TYPE_LABELS = {
  0: "NONE（空）",
  1: "PAGE（ページ切替）",
  2: "MSG（メッセージ）",
  3: "MENU（選択肢）",
  4: "ADDSKILL（スキル追加）",
};

const TALK_EVENT_PAGE_COND_LABELS = {
  0: "条件なし",
  1: "アイテムあり",
  2: "アイテムなし",
};

// ----------------------------------------------------------------
// 状態
// ----------------------------------------------------------------
const state = {
  loaded: false,
  isLoading: false,
  items: [],
  selectedId: null,
  editorOpen: false,
  editorTalkEventId: null,
  editorRows: [],
  editorOnSaved: null,
};

// app.js の bindTalkEventEditor と共用するため window に公開
// （app.js は DOMContentLoaded 後に bindTalkEventEditor() を呼んでいるが
//  このモジュールの mount() は後から呼ばれるため、
//  openTalkEventEditor / closeTalkEventEditor は window 経由で渡す）
function exposeGlobals() {
  window._talkEventState = state;
  window._openTalkEventEditor = openTalkEventEditor;
  window._closeTalkEventEditor = closeTalkEventEditor;
}

// ----------------------------------------------------------------
// DOM ヘルパ
// ----------------------------------------------------------------
function getEls() {
  return {
    tableBody:    document.getElementById("talk-event-table-body"),
    summary:      document.getElementById("talk-event-summary"),
    feedback:     document.getElementById("talk-event-feedback"),
    select:       document.getElementById("talk-event-select"),
    reloadBtn:    document.getElementById("talk-event-reload-btn"),
    newBtn:       document.getElementById("talk-event-new-btn"),
    modal:        document.getElementById("talk-event-modal"),
    modalId:      document.getElementById("talk-event-modal-id"),
    modalFb:      document.getElementById("talk-event-modal-feedback"),
    modalSummary: document.getElementById("talk-event-modal-summary"),
    rowsArea:     document.getElementById("talk-event-rows-area"),
    rowCount:     document.getElementById("talk-event-row-count"),
    addRowBtn:    document.getElementById("talk-event-add-row-btn"),
    saveBtn:      document.getElementById("talk-event-save-btn"),
  };
}

function setFeedback(msg, isError) {
  const els = getEls();
  if (!els.feedback) { return; }
  els.feedback.textContent = msg || "";
  els.feedback.className = "form-feedback" + (isError ? " form-feedback--error" : "");
}

function setModalFeedback(msg, isError) {
  const els = getEls();
  if (!els.modalFb) { return; }
  els.modalFb.textContent = msg || "";
  els.modalFb.style.color = isError ? "#f87171" : "";
}

// ----------------------------------------------------------------
// 一覧
// ----------------------------------------------------------------
async function loadTalkEventList() {
  const els = getEls();
  state.isLoading = true;
  if (els.summary) { els.summary.textContent = "読み込み中..."; }
  try {
    const { response, data } = await fetchJson("/api/talk-events");
    if (!response.ok || !data || !Array.isArray(data.talkEvents)) {
      throw new Error("HTTP " + response.status);
    }
    state.items = data.talkEvents.slice().sort((a, b) => (a.id || 0) - (b.id || 0));
    state.loaded = true;
    state.isLoading = false;
    if (els.summary) { els.summary.textContent = `会話イベント ${state.items.length} 件`; }
    renderSelect();
    renderTable();
  } catch (err) {
    state.isLoading = false;
    if (els.summary) { els.summary.textContent = "取得失敗: " + err.message; }
    setFeedback("取得に失敗しました: " + err.message, true);
  }
}

function renderSelect() {
  const els = getEls();
  if (!els.select) { return; }
  els.select.innerHTML = "";
  if (!state.items.length) {
    const opt = document.createElement("option");
    opt.value = "";
    opt.textContent = "（会話イベントなし）";
    els.select.appendChild(opt);
    return;
  }
  state.items.forEach((it) => {
    const opt = document.createElement("option");
    opt.value = String(it.id);
    opt.textContent = `ID ${it.id}（${it.pageCount}ページ / ${it.eventCount}行）`;
    els.select.appendChild(opt);
  });
}

function renderTable() {
  const els = getEls();
  if (!els.tableBody) { return; }
  els.tableBody.innerHTML = "";
  if (!state.items.length) {
    const tr = document.createElement("tr");
    tr.innerHTML = '<td colspan="4" class="empty-cell">会話イベントがありません</td>';
    els.tableBody.appendChild(tr);
    return;
  }
  state.items.forEach((it) => {
    const tr = document.createElement("tr");
    tr.innerHTML =
      `<td>${it.id}</td>` +
      `<td>${it.pageCount}</td>` +
      `<td>${it.eventCount}</td>` +
      `<td>` +
        `<button type="button" class="btn btn-secondary" data-talk-event-edit="${it.id}">編集</button> ` +
        `<button type="button" class="btn btn-danger" data-talk-event-delete="${it.id}">削除</button>` +
      `</td>`;
    els.tableBody.appendChild(tr);
  });
}

async function deleteTalkEvent(id) {
  if (!confirm("会話イベント ID " + id + " を削除します。よろしいですか？")) { return; }
  try {
    const { response, data } = await fetchJson("/api/talk-events?id=" + id, { method: "DELETE" });
    if (!response.ok) {
      const msg = (data && data.error) ? data.error : "HTTP " + response.status;
      throw new Error(msg);
    }
    setFeedback("削除しました: ID " + id, false);
    loadTalkEventList();
  } catch (err) {
    setFeedback("削除失敗: " + err.message, true);
  }
}

// ----------------------------------------------------------------
// editor（モーダル）
// ----------------------------------------------------------------

function normalizeTalkEventRow(ev) {
  return {
    type: Number(ev.type) || 0,
    page: Number(ev.page) || 0,
    data: Number(ev.data) || 0,
    text: String(ev.text || ""),
    pageChgCondition: Number(ev.pageChgCondition) || 0,
    pageJump: Number(ev.pageJump) || 0,
    menuItems: Array.isArray(ev.menuItems)
      ? ev.menuItems.map((m) => ({ page: Number(m.page) || 0, name: String(m.name || "") }))
      : [],
  };
}

async function openTalkEventEditor(opts) {
  const options = opts || {};
  const talkEventId = Number(options.talkEventId);
  if (!Number.isFinite(talkEventId) || talkEventId <= 0) { alert("会話イベント ID が不正です"); return; }

  state.editorTalkEventId = talkEventId;
  state.editorOnSaved = typeof options.onSaved === "function" ? options.onSaved : null;
  state.editorRows = [];

  const els = getEls();
  if (!els.modal) { return; }
  if (els.modalId) { els.modalId.textContent = String(talkEventId); }
  setModalFeedback("読み込み中...", false);
  els.modal.hidden = false;
  els.modal.setAttribute("aria-hidden", "false");
  state.editorOpen = true;

  try {
    const { response, data } = await fetchJson("/api/talk-events?id=" + talkEventId);
    if (response.ok && data && Array.isArray(data.events)) {
      state.editorRows = data.events.map(normalizeTalkEventRow);
      setModalFeedback(`既存データ ${data.events.length} 行を読み込みました`, false);
    } else if (response.status === 404) {
      state.editorRows = [];
      setModalFeedback("新規会話イベント（既存なし）", false);
    } else {
      throw new Error("HTTP " + response.status);
    }
  } catch (err) {
    setModalFeedback("取得失敗: " + err.message, true);
  }
  renderEditorRows();
}

function closeTalkEventEditor() {
  const els = getEls();
  if (!els.modal) { return; }
  els.modal.hidden = true;
  els.modal.setAttribute("aria-hidden", "true");
  state.editorOpen = false;
  state.editorOnSaved = null;
}

function renderEditorRows() {
  const els = getEls();
  if (!els.rowsArea) { return; }
  els.rowsArea.innerHTML = "";
  if (els.rowCount) { els.rowCount.textContent = String(state.editorRows.length); }
  if (els.modalSummary) { els.modalSummary.textContent = state.editorRows.length + " 行"; }

  if (!state.editorRows.length) {
    const empty = document.createElement("p");
    empty.className = "field-note";
    empty.textContent = "行がありません。右上の「行を追加」で追加してください。";
    els.rowsArea.appendChild(empty);
    return;
  }
  state.editorRows.forEach((row, idx) => {
    els.rowsArea.appendChild(buildRowEl(row, idx));
  });
}

function buildRowEl(row, idx) {
  const wrap = document.createElement("div");
  wrap.className = "card";
  wrap.style.padding = "0.5rem";
  wrap.style.borderLeft = "3px solid #4b5563";

  let typeOptions = "";
  Object.keys(TALK_EVENT_TYPE_LABELS).forEach((k) => {
    const sel = (Number(k) === row.type) ? " selected" : "";
    typeOptions += `<option value="${k}"${sel}>${TALK_EVENT_TYPE_LABELS[k]}</option>`;
  });

  wrap.innerHTML =
    `<div style="display:flex; gap:0.5rem; align-items:center; flex-wrap:wrap;">` +
      `<strong style="min-width:3rem;">#${idx}</strong>` +
      `<label class="form-field"><span>種別</span><select data-talk-row-field="type">${typeOptions}</select></label>` +
      `<label class="form-field"><span>所属ページ</span><input type="number" data-talk-row-field="page" value="${row.page}"></label>` +
      `<div style="margin-left:auto; display:flex; gap:0.25rem;">` +
        `<button type="button" class="btn btn-secondary" data-talk-row-up="${idx}">↑</button>` +
        `<button type="button" class="btn btn-secondary" data-talk-row-down="${idx}">↓</button>` +
        `<button type="button" class="btn btn-danger" data-talk-row-delete="${idx}">削除</button>` +
      `</div>` +
    `</div>` +
    `<div data-talk-row-body style="margin-top:0.4rem;"></div>`;

  const body = wrap.querySelector("[data-talk-row-body]");
  renderRowBody(body, row);

  const typeSel = wrap.querySelector('[data-talk-row-field="type"]');
  if (typeSel) {
    typeSel.addEventListener("change", () => {
      row.type = Number(typeSel.value);
      renderRowBody(body, row);
    });
  }
  const pageInp = wrap.querySelector('[data-talk-row-field="page"]');
  if (pageInp) { pageInp.addEventListener("input", () => { row.page = parseInt(pageInp.value || "0", 10) || 0; }); }

  return wrap;
}

function renderRowBody(body, row) {
  if (!body) { return; }
  body.innerHTML = "";

  if (row.type === TALK_EVENT_TYPE_NONE) {
    const p = document.createElement("p");
    p.className = "field-note";
    p.textContent = "固有フィールドなし（NONE）";
    body.appendChild(p);
    return;
  }
  if (row.type === TALK_EVENT_TYPE_MSG) {
    const label = document.createElement("label");
    label.className = "form-field";
    label.innerHTML = '<span>メッセージ本文</span><textarea rows="3" data-talk-row-field="text" style="width:100%;"></textarea>';
    body.appendChild(label);
    const ta = label.querySelector("textarea");
    ta.value = row.text || "";
    ta.addEventListener("input", () => { row.text = ta.value; });
    return;
  }
  if (row.type === TALK_EVENT_TYPE_ADDSKILL) {
    const label = document.createElement("label");
    label.className = "form-field";
    label.innerHTML = '<span>スキル ID</span><input type="number" data-talk-row-field="data" min="0">';
    body.appendChild(label);
    const inp = label.querySelector("input");
    inp.value = String(row.data || 0);
    inp.addEventListener("input", () => { row.data = parseInt(inp.value || "0", 10) || 0; });
    return;
  }
  if (row.type === TALK_EVENT_TYPE_PAGE) {
    let condOptions = "";
    Object.keys(TALK_EVENT_PAGE_COND_LABELS).forEach((k) => {
      const sel = (Number(k) === row.pageChgCondition) ? " selected" : "";
      condOptions += `<option value="${k}"${sel}>${TALK_EVENT_PAGE_COND_LABELS[k]}</option>`;
    });
    body.innerHTML =
      `<label class="form-field"><span>切替条件</span><select data-talk-row-field="pageChgCondition">${condOptions}</select></label>` +
      `<label class="form-field"><span>ジャンプ先ページ</span><input type="number" data-talk-row-field="pageJump" value="${row.pageJump || 0}"></label>` +
      `<label class="form-field"><span>条件用データ (アイテム ID 等)</span><input type="number" data-talk-row-field="data" value="${row.data || 0}"></label>`;
    const condSel = body.querySelector('[data-talk-row-field="pageChgCondition"]');
    const jumpInp = body.querySelector('[data-talk-row-field="pageJump"]');
    const dataInp = body.querySelector('[data-talk-row-field="data"]');
    if (condSel) { condSel.addEventListener("change", () => { row.pageChgCondition = Number(condSel.value); }); }
    if (jumpInp) { jumpInp.addEventListener("input", () => { row.pageJump = parseInt(jumpInp.value || "0", 10) || 0; }); }
    if (dataInp) { dataInp.addEventListener("input", () => { row.data = parseInt(dataInp.value || "0", 10) || 0; }); }
    return;
  }
  if (row.type === TALK_EVENT_TYPE_MENU) {
    const title = document.createElement("p");
    title.className = "field-note";
    title.textContent = "選択肢（ジャンプ先ページ番号 + 項目名）";
    body.appendChild(title);
    const listDiv = document.createElement("div");
    listDiv.style.cssText = "display:flex;flex-direction:column;gap:0.25rem;";
    body.appendChild(listDiv);

    function renderMenuItems() {
      listDiv.innerHTML = "";
      row.menuItems.forEach((mi, midx) => {
        const itemWrap = document.createElement("div");
        itemWrap.style.cssText = "display:flex;gap:0.5rem;align-items:center;";
        itemWrap.innerHTML =
          `<span>#${midx}</span>` +
          `<label class="form-field"><span>ページ</span><input type="number" value="${mi.page}" style="width:6rem;"></label>` +
          `<label class="form-field" style="flex:1;"><span>項目名</span><input type="text" value=""></label>` +
          `<button type="button" class="btn btn-danger">削除</button>`;
        const inps = itemWrap.querySelectorAll("input");
        inps[0].addEventListener("input", () => { mi.page = parseInt(inps[0].value || "0", 10) || 0; });
        inps[1].value = mi.name || "";
        inps[1].addEventListener("input", () => { mi.name = inps[1].value; });
        const delBtn = itemWrap.querySelector("button");
        delBtn.addEventListener("click", () => { row.menuItems.splice(midx, 1); renderMenuItems(); });
        listDiv.appendChild(itemWrap);
      });
    }
    renderMenuItems();
    const addBtn = document.createElement("button");
    addBtn.type = "button";
    addBtn.className = "btn btn-secondary";
    addBtn.textContent = "+ 選択肢を追加";
    addBtn.style.marginTop = "0.25rem";
    addBtn.addEventListener("click", () => { row.menuItems.push({ page: 0, name: "" }); renderMenuItems(); });
    body.appendChild(addBtn);
  }
}

async function saveTalkEventEditor() {
  if (!state.editorOpen) { return; }
  const id = state.editorTalkEventId;
  if (!Number.isFinite(id) || id <= 0) { return; }

  const payload = {
    id,
    events: state.editorRows.map((r) => {
      const base = { type: r.type, page: r.page, data: r.data, text: r.text };
      if (r.type === TALK_EVENT_TYPE_PAGE) { base.pageChgCondition = r.pageChgCondition; base.pageJump = r.pageJump; }
      if (r.type === TALK_EVENT_TYPE_MENU) { base.menuItems = r.menuItems.slice(); }
      return base;
    }),
  };

  setModalFeedback("保存中...", false);
  try {
    const { response, data } = await fetchJson("/api/talk-events", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    if (!response.ok) {
      const msg = (data && data.error) ? data.error : "HTTP " + response.status;
      throw new Error(msg);
    }
    setModalFeedback("保存しました", false);
    const cb = state.editorOnSaved;
    setTimeout(() => {
      closeTalkEventEditor();
      if (cb) { cb(data); }
    }, 400);
  } catch (err) {
    setModalFeedback("保存失敗: " + err.message, true);
  }
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------
export function mount(container) {
  container.innerHTML = `
      <section class="card" id="talk-events-browser">
        <h2>会話イベント編集</h2>
        <p class="card-description">
          NPC / キャラクターに紐づく会話イベント (CInfoTalkEvent) を編集します。
          ID はキャラクター ID と対応しており、コマンド配列 (events[]) を行単位で差し替えます。
        </p>

        <div class="map-info-controls">
          <label class="form-field">
            <span>会話イベント ID</span>
            <select id="talk-event-select"></select>
          </label>
          <button type="button" class="btn btn-primary" id="talk-event-reload-btn">再読み込み</button>
          <button type="button" class="btn btn-primary" id="talk-event-new-btn">ID を指定して新規</button>
        </div>

        <p id="talk-event-summary" class="section-summary" aria-live="polite"></p>

        <div class="table-wrapper">
          <table class="data-table">
            <thead>
              <tr>
                <th>ID</th>
                <th>ページ数</th>
                <th>行数</th>
                <th>操作</th>
              </tr>
            </thead>
            <tbody id="talk-event-table-body"></tbody>
          </table>
        </div>

        <p id="talk-event-feedback" class="form-feedback" aria-live="polite"></p>
      </section>

      <!-- 会話イベント editor モーダル -->
      <div class="picker-modal" id="talk-event-modal" hidden aria-hidden="true" role="dialog" aria-modal="true" aria-labelledby="talk-event-modal-title">
        <div class="picker-modal-backdrop" data-talk-event-dismiss></div>
        <div class="picker-modal-panel" role="document" style="width:min(960px,95vw);">
          <header class="picker-modal-header">
            <h2 id="talk-event-modal-title" class="picker-modal-title">会話イベント editor</h2>
            <button type="button" class="picker-modal-close" data-talk-event-dismiss aria-label="閉じる">&times;</button>
          </header>
          <div class="picker-modal-toolbar">
            <span>ID: <strong id="talk-event-modal-id">-</strong></span>
            <button type="button" class="btn btn-primary" id="talk-event-add-row-btn">行を追加</button>
            <span class="picker-modal-summary" id="talk-event-modal-summary"></span>
          </div>
          <div class="picker-modal-body">
            <div class="picker-modal-feedback" id="talk-event-modal-feedback" aria-live="polite"></div>
            <div id="talk-event-rows-area" style="display:flex; flex-direction:column; gap:0.5rem;"></div>
          </div>
          <footer class="picker-modal-footer">
            <div class="picker-modal-selected">
              編集中の行数: <strong id="talk-event-row-count">0</strong>
            </div>
            <div class="picker-modal-actions">
              <button type="button" class="btn" data-talk-event-dismiss>キャンセル</button>
              <button type="button" class="btn primary" id="talk-event-save-btn">保存</button>
            </div>
          </footer>
        </div>
      </div>`;

  // グローバル公開（app.js の data-talk-event 連携用）
  exposeGlobals();

  // 一覧画面イベント登録（委譲）
  const tableBody = container.querySelector("#talk-event-table-body");
  if (tableBody) {
    tableBody.addEventListener("click", (ev) => {
      const editBtn = ev.target.closest("[data-talk-event-edit]");
      if (editBtn) {
        const id = Number(editBtn.getAttribute("data-talk-event-edit"));
        openTalkEventEditor({ talkEventId: id, onSaved: () => { loadTalkEventList(); } });
        return;
      }
      const delBtn = ev.target.closest("[data-talk-event-delete]");
      if (delBtn) {
        const id = Number(delBtn.getAttribute("data-talk-event-delete"));
        deleteTalkEvent(id);
      }
    });
  }

  const reloadBtn = container.querySelector("#talk-event-reload-btn");
  if (reloadBtn) { reloadBtn.addEventListener("click", () => { loadTalkEventList(); }); }

  const newBtn = container.querySelector("#talk-event-new-btn");
  if (newBtn) {
    newBtn.addEventListener("click", () => {
      const raw = prompt("新規会話イベント ID (キャラクター ID) を入力してください。");
      if (!raw) { return; }
      const id = parseInt(raw, 10);
      if (!Number.isFinite(id) || id <= 0) { alert("不正な ID です"); return; }
      openTalkEventEditor({ talkEventId: id, onSaved: () => { loadTalkEventList(); } });
    });
  }

  // モーダルの dismiss / 行操作 / 保存
  const modal = document.getElementById("talk-event-modal");
  if (modal) {
    modal.querySelectorAll("[data-talk-event-dismiss]").forEach((el) => {
      el.addEventListener("click", closeTalkEventEditor);
    });

    const addRowBtn = document.getElementById("talk-event-add-row-btn");
    if (addRowBtn) {
      addRowBtn.addEventListener("click", () => {
        state.editorRows.push(normalizeTalkEventRow({ type: TALK_EVENT_TYPE_MSG }));
        renderEditorRows();
      });
    }

    const rowsArea = document.getElementById("talk-event-rows-area");
    if (rowsArea) {
      rowsArea.addEventListener("click", (ev) => {
        const up = ev.target.closest("[data-talk-row-up]");
        if (up) {
          const i = Number(up.getAttribute("data-talk-row-up"));
          if (i > 0) { [state.editorRows[i - 1], state.editorRows[i]] = [state.editorRows[i], state.editorRows[i - 1]]; renderEditorRows(); }
          return;
        }
        const dn = ev.target.closest("[data-talk-row-down]");
        if (dn) {
          const i = Number(dn.getAttribute("data-talk-row-down"));
          if (i < state.editorRows.length - 1) { [state.editorRows[i + 1], state.editorRows[i]] = [state.editorRows[i], state.editorRows[i + 1]]; renderEditorRows(); }
          return;
        }
        const del = ev.target.closest("[data-talk-row-delete]");
        if (del) {
          const i = Number(del.getAttribute("data-talk-row-delete"));
          state.editorRows.splice(i, 1);
          renderEditorRows();
        }
      });
    }

    const saveBtn = document.getElementById("talk-event-save-btn");
    if (saveBtn) { saveBtn.addEventListener("click", saveTalkEventEditor); }
  }

  // ESC で閉じる
  document.addEventListener("keydown", (ev) => {
    if (state.editorOpen && ev.key === "Escape") { closeTalkEventEditor(); }
  });

  // 初回ロード
  loadTalkEventList();

  return {
    unmount() {
      delete window._talkEventState;
      delete window._openTalkEventEditor;
      delete window._closeTalkEventEditor;
    }
  };
}

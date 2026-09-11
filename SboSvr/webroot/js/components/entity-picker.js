/**
 * components/entity-picker.js
 * item / itemType / effect / weapon / skill / motionType / map / character /
 * npcTemplate / mapPart / mapShadow を共通の型で選べるモーダル picker。
 *
 * app.js:565-1030 の openPicker（item/effect/mapPart/mapShadow の未使用実装）を
 * 置き換え・拡張したもの。データ源は各 /api/* エンドポイント。
 *
 * openEntityPicker() でモーダルを開き、ユーザーが選択すると
 * onSelect(id, row) が呼ばれる（row は API が返した生レコード。
 * character / npcTemplate ではキャラ全体の情報が入る）。
 *
 * createEntityField() は「(サムネ or 名前表示) + ID input + 選択ボタン」の
 * 横並びフィールドを返す。sprite-picker.js の createSpriteField 相当。
 *
 * 別ウィンドウ対応:
 *   workspace-layout.js は編集ペインの DOM を adoptNode で popup ウィンドウへ
 *   移す。モーダルを document 固定で出すと popup 側での操作時にモーダルが
 *   メインウィンドウ側に出てしまうため、呼び出し元要素の ownerDocument に
 *   モーダルを生成する（openEntityPicker の ownerDocument 引数、
 *   createEntityField は選択ボタンの ownerDocument を自動で渡す）。
 *   popup 側は workspace-layout.js が起動時のスタイルシートを複製済みなので
 *   components.css のスタイルもそのまま効く。
 */

import { fetchJson } from "../core/api.js";
import { createSpriteThumbLazy } from "./sprite-thumb.js";

// ----------------------------------------------------------------
// type ごとの設定
// ----------------------------------------------------------------

// 通常型（一覧を丸ごと取得してクライアント側キャッシュ・検索する型）
const LIST_TYPE_CONFIG = {
  item: {
    title: "アイテムを選択",
    listUrl: "/api/items",
    listKey: "items",
    idField: "itemId",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: (row) => (row.grpId != null ? { categoryKey: "item", sub: row.grpId } : null),
  },
  itemType: {
    title: "アイテム種別を選択",
    listUrl: "/api/item-types",
    listKey: "items",
    idField: "itemTypeId",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: (row) => (row.iconGrpId != null ? { categoryKey: "item", sub: row.iconGrpId } : null),
  },
  effect: {
    title: "エフェクトを選択",
    listUrl: "/api/effects",
    listKey: "items",
    idField: "effectId",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: (row) => {
      const first = Array.isArray(row.animes) ? row.animes[0] : null;
      if (!first) return null;
      return { categoryKey: row.grpIdMain ? "effect64" : "effect32", sub: first.grpIdBase };
    },
  },
  weapon: {
    title: "武器を選択",
    listUrl: "/api/weapons",
    listKey: "items",
    idField: "weaponInfoId",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: null,
  },
  skill: {
    title: "スキルを選択",
    listUrl: "/api/skills",
    listKey: "skills",
    idField: "skillId",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: (row) => (row.iconId != null ? { categoryKey: "icon32", sub: row.iconId } : null),
  },
  motionType: {
    title: "モーション種別を選択",
    listUrl: "/api/motion-types",
    listKey: "motionTypes",
    idField: "motionTypeId",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: null,
  },
  map: {
    title: "マップを選択",
    listUrl: "/api/maps",
    listKey: "maps",
    idField: "id",
    nameField: "name",
    searchPlaceholder: "ID や名前で絞り込み",
    thumb: null,
  },
  mapPart: {
    title: "マップパーツを選択",
    listUrl: "/api/maps/parts",
    listKey: "parts",
    idField: "partsId",
    nameField: null,
    searchPlaceholder: "パーツ ID で絞り込み",
    // 一覧 API は sprites.base に { sheet, tile } しか返さない（生の grpId は
    // 含まれない）。m_wGrpIDBase = sheet*1024+tile なので逆算する
    // （MapPartsHandler.cpp の AppendSpriteJson と対）。
    thumb: (row) => {
      const base = row.sprites && row.sprites.base;
      if (!base) return null;
      return { categoryKey: "mapParts", sub: base.sheet * 1024 + base.tile };
    },
  },
  mapShadow: {
    title: "マップ影を選択",
    listUrl: "/api/maps/shadows",
    listKey: "shadows",
    idField: "id",
    nameField: null,
    searchPlaceholder: "影 ID で絞り込み",
    thumb: (row) => (row.grpId != null ? { categoryKey: "mapShadow", sub: row.grpId } : null),
  },
};

// ページング型（サーバーに検索語を渡す。/api/characters）
const PAGED_TYPE_CONFIG = {
  character: {
    title: "キャラクターを選択",
    searchPlaceholder: "名前で絞り込み",
    isNpc: null, // フィルタなし（PC/NPC 両方）
  },
  npcTemplate: {
    title: "種族（NPCテンプレ）を選択",
    searchPlaceholder: "名前で絞り込み",
    isNpc: 1,
  },
};

function isPagedType(type) {
  return Object.prototype.hasOwnProperty.call(PAGED_TYPE_CONFIG, type);
}

function typeSupportsThumb(type) {
  const cfg = LIST_TYPE_CONFIG[type];
  return !!(cfg && typeof cfg.thumb === "function");
}

function rowId(type, row) {
  if (isPagedType(type)) return row.charId;
  const cfg = LIST_TYPE_CONFIG[type];
  return cfg ? row[cfg.idField] : undefined;
}

function rowLabel(type, row) {
  if (isPagedType(type)) {
    return row.charName || `#${row.charId}`;
  }
  const cfg = LIST_TYPE_CONFIG[type];
  if (!cfg) return String(rowId(type, row));
  if (cfg.nameField && row[cfg.nameField]) return row[cfg.nameField];
  return `#${row[cfg.idField]}`;
}

function rowThumb(type, row) {
  if (isPagedType(type)) return null; // API が grpIdMain/grpIdSub を返さないため対応なし
  const cfg = LIST_TYPE_CONFIG[type];
  if (!cfg || typeof cfg.thumb !== "function") return null;
  try {
    return cfg.thumb(row);
  } catch {
    return null;
  }
}

// ----------------------------------------------------------------
// 一覧キャッシュ（LIST_TYPE_CONFIG の型のみ。character/npcTemplate はページング
// のためキャッシュしない）
// ----------------------------------------------------------------

/** @type {Map<string, Promise<any[]>>} */
const _cache = new Map();

async function loadListRows(type) {
  const cfg = LIST_TYPE_CONFIG[type];
  if (!cfg) return [];
  if (_cache.has(type)) {
    return _cache.get(type);
  }
  const promise = (async () => {
    const { response, data } = await fetchJson(cfg.listUrl);
    if (!response.ok || !data || !Array.isArray(data[cfg.listKey])) {
      throw new Error(`一覧取得に失敗（HTTP ${response.status}）`);
    }
    return data[cfg.listKey];
  })();
  _cache.set(type, promise);
  try {
    return await promise;
  } catch (err) {
    _cache.delete(type); // 失敗はキャッシュしない（再試行できるように）
    throw err;
  }
}

/**
 * type の一覧キャッシュを破棄する。type 省略時は全 type を破棄。
 * 編集画面で一覧に影響する保存をした後に呼ぶ。
 * @param {string} [type]
 */
export function invalidateEntityCache(type) {
  if (type) {
    _cache.delete(type);
    if (isPagedType(type)) {
      // character / npcTemplate は同じ charId 空間を共有するため両方まとめて破棄
      _charDetailCache.clear();
    }
  } else {
    _cache.clear();
    _charDetailCache.clear();
  }
}

// character / npcTemplate 用: 一覧に無い charId を GET /api/characters/{id} で
// 1 件取得して名前解決するための小さなキャッシュ（Promise を保持）。
/** @type {Map<number, Promise<object|null>>} */
const _charDetailCache = new Map();

/**
 * GET /api/characters/{id} を叩いてキャラ詳細を取得する（結果はキャッシュ）。
 * 手入力 ID の名前解決や、npcTemplate 選択時のテンプレコピーに使う。
 * @param {number} id
 * @returns {Promise<object|null>}
 */
export async function fetchCharacterDetail(id) {
  const key = Number(id);
  if (!Number.isFinite(key) || key <= 0) return null; // 未選択（0）は API を叩かない
  if (_charDetailCache.has(key)) {
    return _charDetailCache.get(key);
  }
  const promise = (async () => {
    const { response, data } = await fetchJson(`/api/characters/${key}`);
    if (!response.ok || !data) return null;
    return data;
  })();
  _charDetailCache.set(key, promise);
  try {
    return await promise;
  } catch (err) {
    _charDetailCache.delete(key);
    return null;
  }
}

async function fetchCharacterPage(type, searchText) {
  const cfg = PAGED_TYPE_CONFIG[type];
  const params = new URLSearchParams();
  if (searchText) params.set("name", searchText);
  if (cfg.isNpc != null) params.set("isNpc", String(cfg.isNpc));
  params.set("limit", "50");
  const { response, data } = await fetchJson(`/api/characters?${params.toString()}`);
  if (!response.ok || !data || !Array.isArray(data.characters)) {
    throw new Error(`一覧取得に失敗（HTTP ${response.status}）`);
  }
  return data.characters;
}

// ----------------------------------------------------------------
// openEntityPicker
// ----------------------------------------------------------------

/**
 * エンティティ選択モーダルを開く。
 *
 * @param {{
 *   type: "item"|"itemType"|"effect"|"weapon"|"skill"|"motionType"|"map"|
 *         "character"|"npcTemplate"|"mapPart"|"mapShadow",
 *   initialValue?: number|null,
 *   onSelect: (id: number, row: object) => void,
 *   filter?: (row: object) => boolean,
 *   ownerDocument?: Document,
 * }} options
 */
export function openEntityPicker({ type, initialValue = null, onSelect, filter, ownerDocument }) {
  const doc = ownerDocument || document;
  const paged = isPagedType(type);
  const cfg = paged ? PAGED_TYPE_CONFIG[type] : LIST_TYPE_CONFIG[type];
  if (!cfg) {
    console.warn(`entity-picker: 未知の type "${type}"`);
    return;
  }

  let _rows = [];
  let _selectedId = initialValue != null ? Number(initialValue) : null;
  let _selectedRow = null;
  let _searchDebounce = null;
  let _requestSeq = 0;

  // --- DOM 構築 ---
  const backdrop = doc.createElement("div");
  backdrop.className = "ep-backdrop";
  backdrop.setAttribute("role", "dialog");
  backdrop.setAttribute("aria-modal", "true");

  const dialog = doc.createElement("div");
  dialog.className = "ep-dialog";
  backdrop.appendChild(dialog);

  const header = doc.createElement("div");
  header.className = "ep-header";
  const titleEl = doc.createElement("h3");
  titleEl.className = "ep-title";
  titleEl.textContent = cfg.title;
  const searchInput = doc.createElement("input");
  searchInput.type = "text";
  searchInput.className = "ep-search";
  searchInput.placeholder = cfg.searchPlaceholder || "検索";
  header.append(titleEl, searchInput);
  dialog.appendChild(header);

  const body = doc.createElement("div");
  body.className = "ep-body";
  const listEl = doc.createElement("ul");
  listEl.className = "ep-list";
  body.appendChild(listEl);
  dialog.appendChild(body);

  const feedbackEl = doc.createElement("div");
  feedbackEl.className = "ep-feedback";
  dialog.appendChild(feedbackEl);

  const footer = doc.createElement("div");
  footer.className = "ep-footer";
  const btnCancel = doc.createElement("button");
  btnCancel.type = "button";
  btnCancel.className = "button";
  btnCancel.textContent = "キャンセル";
  const btnOk = doc.createElement("button");
  btnOk.type = "button";
  btnOk.className = "button primary";
  btnOk.textContent = "決定";
  btnOk.disabled = true;
  footer.append(btnCancel, btnOk);
  dialog.appendChild(footer);

  doc.body.appendChild(backdrop);

  // --- 内部処理 ---

  function setFeedback(message, isError) {
    feedbackEl.textContent = message || "";
    feedbackEl.classList.toggle("is-error", !!isError);
  }

  function renderRows() {
    listEl.innerHTML = "";
    const searchText = searchInput.value.trim().toLowerCase();
    let visible = _rows;
    if (typeof filter === "function") {
      visible = visible.filter((r) => {
        try { return filter(r); } catch { return true; }
      });
    }
    if (!paged && searchText) {
      visible = visible.filter((r) => {
        const id = String(rowId(type, r));
        const label = rowLabel(type, r).toLowerCase();
        return id.includes(searchText) || label.includes(searchText);
      });
    }
    if (visible.length === 0) {
      const empty = doc.createElement("li");
      empty.className = "ep-empty";
      empty.textContent = "該当なし";
      listEl.appendChild(empty);
      return;
    }
    visible.forEach((row) => {
      const id = rowId(type, row);
      const li = doc.createElement("li");
      li.className = "ep-row";
      li.dataset.entityId = String(id);
      if (_selectedId != null && Number(id) === Number(_selectedId)) {
        li.classList.add("is-selected");
      }

      if (typeSupportsThumb(type)) {
        const t = rowThumb(type, row);
        const thumb = createSpriteThumbLazy({
          categoryKey: t ? t.categoryKey : "item",
          sub: t ? t.sub : 0,
          size: 28,
        });
        if (!t) thumb.el.classList.add("is-empty");
        li.appendChild(thumb.el);
      }

      const idSpan = doc.createElement("span");
      idSpan.className = "ep-row-id";
      idSpan.textContent = `#${id}`;
      li.appendChild(idSpan);

      const nameSpan = doc.createElement("span");
      nameSpan.className = "ep-row-name";
      nameSpan.textContent = rowLabel(type, row);
      li.appendChild(nameSpan);

      li.addEventListener("click", () => {
        _selectedId = Number(id);
        _selectedRow = row;
        listEl.querySelectorAll(".ep-row.is-selected").forEach((el) => el.classList.remove("is-selected"));
        li.classList.add("is-selected");
        btnOk.disabled = false;
      });
      li.addEventListener("dblclick", () => {
        _selectedId = Number(id);
        _selectedRow = row;
        confirmSelection();
      });

      listEl.appendChild(li);
    });
  }

  async function loadRows() {
    setFeedback("読み込み中...");
    const seq = ++_requestSeq;
    try {
      const rows = paged
        ? await fetchCharacterPage(type, searchInput.value.trim())
        : await loadListRows(type);
      if (seq !== _requestSeq) return; // 途中で次の検索が走った
      _rows = rows;
      if (_selectedId != null && !_selectedRow) {
        _selectedRow = rows.find((r) => Number(rowId(type, r)) === Number(_selectedId)) || null;
        if (!_selectedRow && paged) {
          // 検索結果のページに載っていない初期選択 ID（手入力値など）を
          // 1 件取得で名前解決する。btnOk のラベル用途のみで一覧には出さない。
          fetchCharacterDetail(_selectedId).then((detail) => {
            if (detail && detail.charId != null && _selectedId === Number(detail.charId) && !_selectedRow) {
              _selectedRow = { charId: detail.charId, charName: detail.charName };
            }
          }).catch(() => { /* 無視 */ });
        }
      }
      setFeedback(`${_rows.length} 件`);
      renderRows();
    } catch (err) {
      if (seq !== _requestSeq) return;
      _rows = [];
      renderRows();
      setFeedback(err && err.message ? err.message : String(err), true);
    }
  }

  function close() {
    backdrop.remove();
    doc.removeEventListener("keydown", onKeydown);
  }

  function confirmSelection() {
    if (_selectedId == null) return;
    const id = _selectedId;
    const row = _selectedRow;
    close();
    try {
      onSelect?.(id, row);
    } catch (err) {
      console.error("entity-picker onSelect でエラー:", err);
    }
  }

  function onKeydown(e) {
    if (e.key === "Escape") { e.preventDefault(); close(); }
    else if (e.key === "Enter" && _selectedId != null) { e.preventDefault(); confirmSelection(); }
  }

  backdrop.addEventListener("click", (e) => { if (e.target === backdrop) close(); });
  btnCancel.addEventListener("click", close);
  btnOk.addEventListener("click", confirmSelection);
  doc.addEventListener("keydown", onKeydown);

  searchInput.addEventListener("input", () => {
    if (paged) {
      if (_searchDebounce) clearTimeout(_searchDebounce);
      _searchDebounce = setTimeout(() => loadRows(), 300);
    } else {
      renderRows();
    }
  });

  loadRows();
  setTimeout(() => { try { searchInput.focus(); } catch { /* 無視 */ } }, 0);
}

// ----------------------------------------------------------------
// createEntityField
// ----------------------------------------------------------------

/**
 * 「(サムネ or 名前表示) + ID input + 選択ボタン」の横並びフィールドを返す。
 * createSpriteField (sprite-picker.js) と同じ思想の部品。
 *
 * @param {{
 *   type: string,
 *   value?: number,
 *   onChange?: (id: number, row: object|null) => void,
 *   label?: string,
 *   filter?: (row: object) => boolean,
 * }} options
 * @returns {{ element: HTMLElement, input: HTMLInputElement, getValue: () => number, setValue: (id: number) => void }}
 */
export function createEntityField({ type, value = 0, onChange, label, filter }) {
  const wrap = document.createElement("div");
  wrap.className = "entity-field";

  if (label) {
    const lbl = document.createElement("span");
    lbl.className = "entity-field-label";
    lbl.textContent = label;
    wrap.appendChild(lbl);
  }

  let _value = value;
  let _row = null;

  let thumbCtrl = null;
  if (typeSupportsThumb(type)) {
    thumbCtrl = createSpriteThumbLazy({ categoryKey: "item", sub: 0, size: 28 });
    wrap.appendChild(thumbCtrl.el);
  }

  const nameSpan = document.createElement("span");
  nameSpan.className = "entity-field-name";
  wrap.appendChild(nameSpan);

  const input = document.createElement("input");
  input.type = "number";
  input.min = "0";
  input.value = String(_value);
  wrap.appendChild(input);

  const btn = document.createElement("button");
  btn.type = "button";
  btn.className = "button small";
  btn.textContent = "選択…";
  wrap.appendChild(btn);

  function applyRow(row) {
    _row = row;
    if (row) {
      nameSpan.textContent = rowLabel(type, row);
      if (thumbCtrl) {
        const t = rowThumb(type, row);
        thumbCtrl.update(t || { sub: 0 });
      }
    } else {
      nameSpan.textContent = Number(_value) > 0 ? `#${_value}` : "未選択";
    }
  }

  // 手入力された ID からキャッシュ済み一覧（character/npcTemplate は
  // GET /api/characters/{id}）で名前・サムネを引き直す。
  async function resolveAndApply(id) {
    if (!Number.isFinite(Number(id)) || Number(id) <= 0) {
      applyRow(null); // 未選択（0 以下）は API/一覧取得を行わない
      return;
    }
    if (isPagedType(type)) {
      try {
        const detail = await fetchCharacterDetail(id);
        if (detail && detail.charId != null) {
          applyRow({ charId: detail.charId, charName: detail.charName });
        } else {
          applyRow(null);
        }
      } catch {
        applyRow(null);
      }
      return;
    }
    try {
      const rows = await loadListRows(type);
      const row = rows.find((r) => Number(rowId(type, r)) === Number(id));
      applyRow(row || null);
    } catch {
      applyRow(null);
    }
  }

  function openPickerForField() {
    openEntityPicker({
      type,
      initialValue: _value,
      filter,
      ownerDocument: btn.ownerDocument,
      onSelect: (id, row) => {
        _value = id;
        input.value = String(id);
        applyRow(row);
        onChange?.(id, row);
      },
    });
  }

  btn.addEventListener("click", openPickerForField);

  input.addEventListener("change", () => {
    const v = parseInt(input.value, 10);
    if (!Number.isFinite(v) || v < 0) return;
    _value = v;
    resolveAndApply(v);
    onChange?.(v, _row);
  });

  resolveAndApply(_value);

  return {
    element: wrap,
    input,
    getValue: () => _value,
    setValue: (id) => {
      _value = id;
      input.value = String(id);
      resolveAndApply(id);
    },
  };
}

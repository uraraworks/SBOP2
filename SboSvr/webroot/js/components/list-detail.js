/**
 * components/list-detail.js
 * 画面切替方式の一覧⇔詳細シェル。
 * 通常時は一覧のみ全幅表示。項目クリック/新規で詳細画面に切替。
 * 保存成功 or 「← 戻る」で一覧に復帰（保存後は再読み込み）。
 * dirty 管理・検索・新規作成・削除をサポート。
 */

/**
 * リスト＆詳細シェルを生成してコンテナに mount する。
 *
 * @param {{
 *   container: HTMLElement,
 *   title?: string,
 *   fetchList: () => Promise<Array<unknown>>,
 *   renderListItem: (item: unknown) => string,
 *   renderDetail: (item: unknown) => HTMLElement,
 *   onSave?: (item: unknown) => Promise<unknown>,
 *   onCreate?: () => Promise<unknown>,
 *   onDelete?: (item: unknown) => Promise<void>,
 *   searchKeys?: string[],
 * }} options
 * @returns {{ refresh: () => Promise<void>, getSelected: () => unknown, openDetail: (item: unknown) => void, showList: () => void }}
 */
export function createListDetail({
  container,
  title = "一覧",
  fetchList,
  renderListItem,
  renderDetail,
  onSave,
  onCreate,
  onDelete,
  searchKeys = [],
}) {
  // ----------------------------------------------------------------
  // DOM 構築 — 一覧ペイン
  // ----------------------------------------------------------------

  const shell = document.createElement("div");
  shell.className = "list-detail-shell";

  // === 一覧ペイン ===
  const listPane = document.createElement("div");
  listPane.className = "ld-list-pane";

  const listHeader = document.createElement("div");
  listHeader.className = "ld-list-header";

  const listTitle = document.createElement("div");
  listTitle.className = "ld-list-pane-title";
  listTitle.textContent = title;

  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.placeholder = "検索…";
  searchInput.className = "ld-search";
  searchInput.setAttribute("aria-label", "リスト検索");

  listHeader.append(listTitle, searchInput);

  const listEl = document.createElement("ul");
  listEl.className = "ld-list";
  listEl.setAttribute("role", "listbox");

  const listFooter = document.createElement("div");
  listFooter.className = "ld-list-footer";

  const btnNew = document.createElement("button");
  btnNew.type = "button";
  btnNew.className = "button";
  btnNew.textContent = "+ 新規";

  listFooter.appendChild(btnNew);
  listPane.append(listHeader, listEl, listFooter);

  // === 詳細ペイン ===
  const detailPane = document.createElement("div");
  detailPane.className = "ld-detail-pane";
  detailPane.style.display = "none";

  const detailToolbar = document.createElement("div");
  detailToolbar.className = "ld-detail-toolbar";

  const btnBack = document.createElement("button");
  btnBack.type = "button";
  btnBack.className = "button";
  btnBack.textContent = "← 戻る";

  const btnSave = document.createElement("button");
  btnSave.type = "button";
  btnSave.className = "button primary";
  btnSave.textContent = "保存";
  btnSave.style.display = "none";

  const btnDelete = document.createElement("button");
  btnDelete.type = "button";
  btnDelete.className = "button danger";
  btnDelete.textContent = "削除";
  btnDelete.style.display = "none";

  const dirtyBadge = document.createElement("span");
  dirtyBadge.className = "ld-dirty-badge";
  dirtyBadge.textContent = "未保存";
  dirtyBadge.style.display = "none";

  detailToolbar.append(btnBack, btnSave, btnDelete, dirtyBadge);

  const detailBody = document.createElement("div");
  detailBody.className = "ld-detail-body";

  detailPane.append(detailToolbar, detailBody);

  shell.append(listPane, detailPane);
  container.appendChild(shell);

  // ----------------------------------------------------------------
  // 内部状態
  // ----------------------------------------------------------------

  /** @type {Array<unknown>} */
  let _allItems = [];
  /** @type {Array<unknown>} */
  let _filtered = [];
  let _selectedItem = null;
  let _dirty = false;
  let _listScrollTop = 0;

  // ----------------------------------------------------------------
  // 検索フィルタ
  // ----------------------------------------------------------------

  function _matchSearch(item, q) {
    if (!q) return true;
    const lower = q.toLowerCase();
    if (searchKeys.length === 0) {
      return String(renderListItem(item)).toLowerCase().includes(lower);
    }
    return searchKeys.some((k) => String(item[k] ?? "").toLowerCase().includes(lower));
  }

  function _applyFilter() {
    const q = searchInput.value.trim();
    _filtered = _allItems.filter((item) => _matchSearch(item, q));
    _renderList();
  }

  // ----------------------------------------------------------------
  // 一覧描画
  // ----------------------------------------------------------------

  function _renderList() {
    listEl.innerHTML = "";
    _filtered.forEach((item) => {
      const li = document.createElement("li");
      li.className = "ld-list-item";
      li.setAttribute("role", "option");
      li.setAttribute("aria-selected", item === _selectedItem ? "true" : "false");
      li.textContent = renderListItem(item);
      if (item === _selectedItem) li.classList.add("is-selected");

      li.addEventListener("click", () => _selectItem(item));
      listEl.appendChild(li);
    });
  }

  // ----------------------------------------------------------------
  // 表示切替
  // ----------------------------------------------------------------

  function _showListPane() {
    detailPane.style.display = "none";
    listPane.style.display = "";
    // スクロール位置を復元
    requestAnimationFrame(() => { listEl.scrollTop = _listScrollTop; });
  }

  function _showDetailPane() {
    // スクロール位置を記憶
    _listScrollTop = listEl.scrollTop;
    listPane.style.display = "none";
    detailPane.style.display = "";
  }

  // ----------------------------------------------------------------
  // 詳細ペイン描画
  // ----------------------------------------------------------------

  function _renderDetail(item) {
    detailBody.innerHTML = "";

    if (!item) {
      btnSave.style.display = "none";
      btnDelete.style.display = "none";
      return;
    }

    try {
      const detailEl = renderDetail(item);
      if (detailEl) {
        detailBody.appendChild(detailEl);
      }
    } catch (err) {
      const errMsg = document.createElement("p");
      errMsg.style.color = "rgba(248,113,113,0.9)";
      errMsg.textContent = "詳細の表示中にエラーが発生しました: " + String(err?.message ?? err);
      detailBody.appendChild(errMsg);
    }

    btnSave.style.display = onSave ? "" : "none";
    btnDelete.style.display = onDelete ? "" : "none";
  }

  function _setDirty(flag) {
    _dirty = flag;
    dirtyBadge.style.display = flag ? "" : "none";
  }

  // ----------------------------------------------------------------
  // 選択ロジック
  // ----------------------------------------------------------------

  function _selectItem(item) {
    if (_dirty) {
      if (!confirm("未保存の変更があります。破棄して切り替えますか?")) return;
      _setDirty(false);
    }
    _selectedItem = item;
    _setDirty(false);
    _renderDetail(item);
    _showDetailPane();
  }

  // ----------------------------------------------------------------
  // イベント
  // ----------------------------------------------------------------

  searchInput.addEventListener("input", _applyFilter);

  btnBack.addEventListener("click", () => {
    if (_dirty) {
      if (!confirm("未保存の変更があります。破棄して一覧に戻りますか?")) return;
      _setDirty(false);
    }
    _selectedItem = null;
    _setDirty(false);
    _renderList();
    _showListPane();
  });

  btnNew.addEventListener("click", async () => {
    if (_dirty) {
      if (!confirm("未保存の変更があります。破棄して新規作成しますか?")) return;
    }
    if (!onCreate) return;
    try {
      const newItem = await onCreate();
      await _load();
      if (newItem) {
        const found = _allItems.find((i) => i === newItem) ?? _allItems[_allItems.length - 1];
        if (found) _selectItem(found);
      }
    } catch (err) {
      alert("新規作成に失敗しました: " + String(err?.message ?? err));
    }
  });

  btnSave.addEventListener("click", async () => {
    if (!onSave || !_selectedItem) return;
    try {
      await onSave(_selectedItem);
      _setDirty(false);
      const savedItem = _selectedItem;
      await _load();
      // 保存後も詳細ペインに留まり、同じ項目を再選択
      const found = _allItems.find((i) => i === savedItem) ?? null;
      _selectedItem = found;
      _renderList();
    } catch (err) {
      alert("保存に失敗しました: " + String(err?.message ?? err));
    }
  });

  btnDelete.addEventListener("click", async () => {
    if (!onDelete || !_selectedItem) return;
    if (!confirm("この項目を削除しますか?")) return;
    try {
      await onDelete(_selectedItem);
      _selectedItem = null;
      _setDirty(false);
      await _load();
      _showListPane();
    } catch (err) {
      alert("削除に失敗しました: " + String(err?.message ?? err));
    }
  });

  // detail 内での変更を検知(input/change イベントを委譲)
  detailBody.addEventListener("input", () => {
    if (_selectedItem) _setDirty(true);
  });
  detailBody.addEventListener("change", () => {
    if (_selectedItem) _setDirty(true);
  });

  // ----------------------------------------------------------------
  // データ読み込み
  // ----------------------------------------------------------------

  async function _load() {
    try {
      _allItems = await fetchList();
    } catch (err) {
      _allItems = [];
      console.error("リスト取得失敗:", err);
    }
    _applyFilter();
  }

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  async function refresh() {
    await _load();
  }

  function getSelected() {
    return _selectedItem;
  }

  /** 外部から直接詳細画面を開く（R2-3 フェーズで使用）*/
  function openDetail(item) {
    _selectItem(item);
  }

  /** 外部から一覧画面に戻す */
  function showList() {
    if (_dirty) {
      if (!confirm("未保存の変更があります。破棄して一覧に戻りますか?")) return;
      _setDirty(false);
    }
    _selectedItem = null;
    _setDirty(false);
    _renderList();
    _showListPane();
  }

  // 初期ロード
  _load();

  return { refresh, getSelected, openDetail, showList };
}

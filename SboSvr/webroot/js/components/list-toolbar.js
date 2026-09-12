/**
 * components/list-toolbar.js
 * 一覧画面共通の検索/ソート/ページングツールバー。
 *
 * データの取得元(API から取るか、フロントで持っている全件配列か)は問わない。
 * ここではクライアント側でのフィルタ・ソート・ページングだけを提供する
 * (applyToRows)。状態(q/sort/pageSize/page)は呼び出し元が
 * core/router.js の setRouteParams と組み合わせて URL に残せるよう、
 * getState/setState で単純なプレーンオブジェクトとして出し入れできるようにしてある。
 *
 * 使い方:
 *   const toolbar = createListToolbar({
 *     placeholder: "名前で検索",
 *     sortOptions: [{ value: "id", label: "ID順" }, { value: "name", label: "名前順" }],
 *     pageSizes: [20, 50, 100],
 *     initial: { q: "", sort: "id", page: 1 },
 *     onChange: (state) => { setRouteParams({...}); render(); },
 *   });
 *   host.appendChild(toolbar.element);
 *   // 一覧を描画する際:
 *   const { pageRows } = toolbar.applyToRows(allRows, {
 *     searchFields: ["name", (row) => String(row.id)],
 *     sorters: { id: (a, b) => a.id - b.id, name: (a, b) => a.name.localeCompare(b.name) },
 *   });
 */

const DEFAULT_PAGE_SIZES = [20, 50, 100];

/**
 * @param {Object} options
 * @param {string} [options.placeholder]  検索 input の placeholder
 * @param {{value:string,label:string}[]} [options.sortOptions]  省略時はソート select 自体を出さない
 * @param {number[]} [options.pageSizes]
 * @param {{ q?: string, sort?: string, pageSize?: number, page?: number }} [options.initial]
 * @param {(state: { q: string, sort: string, pageSize: number, page: number }) => void} [options.onChange]
 * @returns {{
 *   element: HTMLElement,
 *   getState: () => { q: string, sort: string, pageSize: number, page: number },
 *   setState: (partial: object, opts?: { silent?: boolean }) => void,
 *   applyToRows: (rows: any[], opts: { searchFields?: (string|((row:any)=>any))[], sorters?: Record<string, (a:any,b:any)=>number> }) => { pageRows: any[], total: number, page: number, pageCount: number },
 *   setTotal: (n: number) => void,
 * }}
 */
export function createListToolbar(options = {}) {
  const {
    placeholder = "検索",
    sortOptions = [],
    pageSizes = DEFAULT_PAGE_SIZES,
    initial = {},
    onChange,
  } = options;

  const state = {
    q: initial.q || "",
    sort: initial.sort || (sortOptions[0] && sortOptions[0].value) || "",
    pageSize: Number(initial.pageSize) || pageSizes[0] || 20,
    page: Number(initial.page) || 1,
  };

  const root = document.createElement("div");
  root.className = "list-toolbar";

  // 検索
  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.className = "list-toolbar-search form-input";
  searchInput.placeholder = placeholder;
  searchInput.value = state.q;
  let debounceTimer = null;
  searchInput.addEventListener("input", () => {
    clearTimeout(debounceTimer);
    debounceTimer = setTimeout(() => {
      state.q = searchInput.value;
      state.page = 1;
      emitChange();
    }, 200);
  });
  root.appendChild(searchInput);

  // 並び替え
  let sortSelect = null;
  if (sortOptions.length) {
    sortSelect = document.createElement("select");
    sortSelect.className = "list-toolbar-sort form-select";
    sortOptions.forEach((opt) => {
      const o = document.createElement("option");
      o.value = opt.value;
      o.textContent = opt.label;
      sortSelect.appendChild(o);
    });
    sortSelect.value = state.sort;
    sortSelect.addEventListener("change", () => {
      state.sort = sortSelect.value;
      state.page = 1;
      emitChange();
    });
    root.appendChild(sortSelect);
  }

  // 表示件数
  const pageSizeSelect = document.createElement("select");
  pageSizeSelect.className = "list-toolbar-pagesize form-select";
  pageSizes.forEach((n) => {
    const o = document.createElement("option");
    o.value = String(n);
    o.textContent = `${n} 件`;
    pageSizeSelect.appendChild(o);
  });
  pageSizeSelect.value = String(state.pageSize);
  pageSizeSelect.addEventListener("change", () => {
    state.pageSize = Number(pageSizeSelect.value) || pageSizes[0] || 20;
    state.page = 1;
    emitChange();
  });
  root.appendChild(pageSizeSelect);

  // ページ送り
  const pager = document.createElement("div");
  pager.className = "list-toolbar-pager";
  const prevBtn = document.createElement("button");
  prevBtn.type = "button";
  prevBtn.className = "btn btn-secondary btn-sm";
  prevBtn.textContent = "← 前へ";
  const pageInfo = document.createElement("span");
  pageInfo.className = "list-toolbar-page-info";
  const nextBtn = document.createElement("button");
  nextBtn.type = "button";
  nextBtn.className = "btn btn-secondary btn-sm";
  nextBtn.textContent = "次へ →";
  prevBtn.addEventListener("click", () => {
    if (state.page > 1) {
      state.page -= 1;
      emitChange();
    }
  });
  nextBtn.addEventListener("click", () => {
    if (state.page < _pageCount) {
      state.page += 1;
      emitChange();
    }
  });
  pager.append(prevBtn, pageInfo, nextBtn);
  root.appendChild(pager);

  let _total = 0;
  let _pageCount = 1;

  function updatePagerUI() {
    pageInfo.textContent = `${state.page} / ${_pageCount} ページ・全 ${_total} 件`;
    prevBtn.disabled = state.page <= 1;
    nextBtn.disabled = state.page >= _pageCount;
  }

  function getState() {
    return { q: state.q, sort: state.sort, pageSize: state.pageSize, page: state.page };
  }

  function emitChange() {
    updatePagerUI();
    if (typeof onChange === "function") {
      onChange(getState());
    }
  }

  /**
   * 外部から状態を書き換える(URL 復元時など)。既定では onChange を呼ばない
   * (呼び出し元がこの後 render するのが通例のため)。
   * @param {Partial<{ q: string, sort: string, pageSize: number, page: number }>} partial
   * @param {{ silent?: boolean }} [opts]
   */
  function setState(partial, opts = {}) {
    const { silent = true } = opts;
    Object.assign(state, partial || {});
    searchInput.value = state.q;
    if (sortSelect) { sortSelect.value = state.sort; }
    pageSizeSelect.value = String(state.pageSize);
    updatePagerUI();
    if (!silent) {
      emitChange();
    }
  }

  /**
   * rows に検索/ソート/ページングを適用する。
   * @param {any[]} rows
   * @param {{ searchFields?: (string|((row:any)=>any))[], sorters?: Record<string, (a:any,b:any)=>number> }} opts
   */
  function applyToRows(rows, opts = {}) {
    const { searchFields = [], sorters = {} } = opts;
    let filtered = rows || [];

    const q = state.q.trim().toLowerCase();
    if (q && searchFields.length) {
      filtered = filtered.filter((row) =>
        searchFields.some((field) => {
          const v = typeof field === "function" ? field(row) : row[field];
          return String(v ?? "").toLowerCase().includes(q);
        })
      );
    }

    const sorter = sorters[state.sort];
    if (typeof sorter === "function") {
      filtered = filtered.slice().sort(sorter);
    }

    _total = filtered.length;
    _pageCount = Math.max(1, Math.ceil(_total / state.pageSize));
    if (state.page > _pageCount) { state.page = _pageCount; }
    if (state.page < 1) { state.page = 1; }

    const start = (state.page - 1) * state.pageSize;
    const pageRows = filtered.slice(start, start + state.pageSize);
    updatePagerUI();
    return { pageRows, total: _total, page: state.page, pageCount: _pageCount };
  }

  /** サーバー側ページングなど、applyToRows を使わない場合に件数だけ反映する。 */
  function setTotal(n) {
    _total = Number(n) || 0;
    _pageCount = Math.max(1, Math.ceil(_total / state.pageSize));
    updatePagerUI();
  }

  updatePagerUI();

  return { element: root, getState, setState, applyToRows, setTotal };
}

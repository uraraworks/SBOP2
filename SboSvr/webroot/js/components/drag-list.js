/**
 * components/drag-list.js
 * HTML5 DnD による並べ替えリスト。↑↓ボタンも併設。
 */

/**
 * 並べ替え可能なリストを生成する。
 *
 * @param {{
 *   items: Array<unknown>,
 *   renderItem: (item: unknown, index: number) => HTMLElement,
 *   onReorder: (newItems: Array<unknown>) => void,
 * }} options
 * @returns {{ el: HTMLElement, setItems: (items: Array<unknown>) => void, getItems: () => Array<unknown> }}
 */
export function createDragList({ items = [], renderItem, onReorder }) {
  let _items = [...items];

  const ul = document.createElement("ul");
  ul.className = "drag-list";

  // ----------------------------------------------------------------
  // 描画
  // ----------------------------------------------------------------

  function _render() {
    ul.innerHTML = "";
    _items.forEach((item, index) => {
      const li = _makeItem(item, index);
      ul.appendChild(li);
    });
  }

  function _makeItem(item, index) {
    const li = document.createElement("li");
    li.className = "drag-list-item";
    li.draggable = true;
    li.dataset.index = String(index);

    // ハンドル
    const handle = document.createElement("span");
    handle.className = "drag-handle";
    handle.textContent = "≡";
    handle.setAttribute("aria-hidden", "true");

    // コンテンツ
    const content = document.createElement("div");
    content.className = "drag-list-content";
    const rendered = renderItem(item, index);
    if (rendered) content.appendChild(rendered);

    // ↑↓ボタン
    const actions = document.createElement("div");
    actions.className = "drag-list-actions";

    const btnUp = document.createElement("button");
    btnUp.type = "button";
    btnUp.textContent = "↑";
    btnUp.setAttribute("aria-label", "上へ移動");
    btnUp.disabled = index === 0;

    const btnDown = document.createElement("button");
    btnDown.type = "button";
    btnDown.textContent = "↓";
    btnDown.setAttribute("aria-label", "下へ移動");
    btnDown.disabled = index === _items.length - 1;

    btnUp.addEventListener("click", () => _move(index, index - 1));
    btnDown.addEventListener("click", () => _move(index, index + 1));

    actions.append(btnUp, btnDown);
    li.append(handle, content, actions);

    // DnD イベント
    li.addEventListener("dragstart", (e) => _onDragStart(e, index));
    li.addEventListener("dragover", (e) => _onDragOver(e, index));
    li.addEventListener("dragleave", () => _onDragLeave(li));
    li.addEventListener("drop", (e) => _onDrop(e, index));
    li.addEventListener("dragend", () => _onDragEnd());

    return li;
  }

  // ----------------------------------------------------------------
  // DnD ロジック
  // ----------------------------------------------------------------

  let _dragIndex = -1;

  function _onDragStart(e, index) {
    _dragIndex = index;
    e.dataTransfer.effectAllowed = "move";
    e.dataTransfer.setData("text/plain", String(index));
    requestAnimationFrame(() => {
      const li = ul.querySelector(`[data-index="${index}"]`);
      if (li) li.classList.add("dragging");
    });
  }

  function _onDragOver(e, index) {
    if (_dragIndex === index) return;
    e.preventDefault();
    e.dataTransfer.dropEffect = "move";
    ul.querySelectorAll(".drag-list-item").forEach((li) => li.classList.remove("drag-over"));
    const target = ul.querySelector(`[data-index="${index}"]`);
    if (target) target.classList.add("drag-over");
  }

  function _onDragLeave(li) {
    li.classList.remove("drag-over");
  }

  function _onDrop(e, toIndex) {
    e.preventDefault();
    if (_dragIndex === toIndex || _dragIndex < 0) return;
    _move(_dragIndex, toIndex);
  }

  function _onDragEnd() {
    ul.querySelectorAll(".drag-list-item").forEach((li) => {
      li.classList.remove("dragging", "drag-over");
    });
    _dragIndex = -1;
  }

  // ----------------------------------------------------------------
  // 並べ替え
  // ----------------------------------------------------------------

  function _move(fromIndex, toIndex) {
    if (toIndex < 0 || toIndex >= _items.length) return;
    const newItems = [..._items];
    const [moved] = newItems.splice(fromIndex, 1);
    newItems.splice(toIndex, 0, moved);
    _items = newItems;
    _render();
    onReorder?.([..._items]);
  }

  // ----------------------------------------------------------------
  // 公開 API
  // ----------------------------------------------------------------

  function setItems(newItems) {
    _items = [...newItems];
    _render();
  }

  function getItems() {
    return [..._items];
  }

  _render();

  return { el: ul, setItems, getItems };
}

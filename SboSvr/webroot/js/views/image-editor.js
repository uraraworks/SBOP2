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
import { createCharComposer, COMPOSABLE_KEYS } from "../components/char-composer.js";
import { createCharContext } from "../components/char-context.js";
import { createSpritePaint } from "../components/sprite-paint.js";
import { icon } from "../components/icons.js";
import { attachTip } from "../components/tooltip.js";

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
  // 401 は core/api.js の共通ハンドラ(setUnauthorizedHandler)がログイン画面へ戻す
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
// 破壊的な操作の確認
//
// confirm() は使わない。編集ペインを別ウィンドウへポップアップしていると、
// ダイアログはスクリプトの所属する「元のウィンドウ」に出るため、背後に隠れて
// 見えず「押しても何も起きない」状態になる。
// 代わりに 1 回目のクリックで文言を変えて確認し、2 回目で実行する。
// ----------------------------------------------------------------

function armConfirmButton(button, { armedLabel, timeoutMs = 4000, onConfirm }) {
  // アイコンを子要素として持つボタンでは button.textContent への代入で
  // アイコン(SVG)ごと消えてしまう。ラベル用の <span class="btn-label"> が
  // あればそちらだけを差し替え、無いボタンでは従来どおり textContent を使う。
  const labelEl = button.querySelector(".btn-label");
  const getLabel = () => (labelEl ? labelEl.textContent : button.textContent);
  const setLabel = (text) => {
    if (labelEl) labelEl.textContent = text;
    else button.textContent = text;
  };

  const normalLabel = getLabel();
  let armed = false;
  let timer = null;

  const disarm = () => {
    armed = false;
    setLabel(normalLabel);
    button.classList.remove("is-armed");
    if (timer) { clearTimeout(timer); timer = null; }
  };

  button.addEventListener("click", () => {
    if (!armed) {
      armed = true;
      setLabel(armedLabel);
      button.classList.add("is-armed");
      timer = setTimeout(disarm, timeoutMs);
      return;
    }
    disarm();
    onConfirm();
  });

  return { disarm };
}

// ----------------------------------------------------------------
// 左ペイン: カテゴリ一覧
// ----------------------------------------------------------------

// カテゴリをグルーピングする際の表示順。定義に無い key は末尾の「その他」に
// まとめる(将来カテゴリが増えても取りこぼさないため)。
const CATEGORY_GROUPS = [
  { label: "キャラクター (2x2)", keys: ["char2x2", "cloth2x2", "hair2x2", "eye2x2", "spCloth2x2", "spHair2x2", "npc2x2"] },
  { label: "装備・持ち物 (2x2)", keys: ["arms2x2", "armsSp2x2", "shield2x2", "bow2x2"] },
  { label: "エフェクト・演出", keys: ["effect32", "effect64", "efcBalloon"] },
  { label: "マップ・アイテム", keys: ["mapParts", "mapShadow", "item"] },
  { label: "UI", keys: ["icon32"] },
  { label: "キャラクター (旧16px)", keys: ["char", "cloth", "hairDown", "hairUp", "eye", "spCloth", "acce", "npcRow", "npc"] },
  { label: "武器 (旧)", keys: ["weapon", "weaponBow", "weaponGlove", "weaponEtc"] },
];

const OTHER_GROUP_LABEL = "その他";
const DEFAULT_OPEN_GROUPS = new Set(["キャラクター (2x2)", "装備・持ち物 (2x2)"]);
const GROUP_OPEN_STORAGE_KEY = "sbop2.imageEditor.groups";

function loadGroupOpenState() {
  try {
    const raw = localStorage.getItem(GROUP_OPEN_STORAGE_KEY);
    const obj = raw ? JSON.parse(raw) : null;
    return obj && typeof obj === "object" ? obj : {};
  } catch (e) {
    return {}; // 破損していても致命的にしない
  }
}

function saveGroupOpenState(obj) {
  try {
    localStorage.setItem(GROUP_OPEN_STORAGE_KEY, JSON.stringify(obj));
  } catch (e) {
    // 保存できなくても動作は継続する
  }
}

// categories を CATEGORY_GROUPS の順にグルーピングする。どのグループにも
// 属さない key は「その他」としてまとめる。
function groupCategories(categories) {
  const used = new Set();
  const groups = CATEGORY_GROUPS.map((g) => {
    const items = g.keys
      .map((k) => categories.find((c) => c.key === k))
      .filter(Boolean);
    items.forEach((c) => used.add(c.key));
    return { label: g.label, items };
  }).filter((g) => g.items.length > 0);
  const rest = categories.filter((c) => !used.has(c.key));
  if (rest.length) groups.push({ label: OTHER_GROUP_LABEL, items: rest });
  return groups;
}

function buildCategoryList({ categories, onSelect }) {
  const searchInput = document.createElement("input");
  searchInput.type = "search";
  searchInput.className = "ie-cat-search";
  searchInput.placeholder = "カテゴリを検索";

  const ul = document.createElement("ul");
  ul.className = "ld-list";

  const groupDefs = groupCategories(categories);
  const keyToGroupLabel = new Map();
  groupDefs.forEach((g) => g.items.forEach((c) => keyToGroupLabel.set(c.key, g.label)));

  const savedOpen = loadGroupOpenState();
  const openState = new Map(); // label -> boolean(開いているか)
  groupDefs.forEach((g) => {
    const open = Object.prototype.hasOwnProperty.call(savedOpen, g.label)
      ? Boolean(savedOpen[g.label])
      : DEFAULT_OPEN_GROUPS.has(g.label);
    openState.set(g.label, open);
  });

  let _selectedKey = null;
  let _selectedIndex = 0;
  let _expandedKey = null; // シート一覧を展開中のカテゴリ(1つだけ)
  const overriddenMarks = new Map(); // "key/index" -> boolean
  const thumbVersions = new Map(); // "key/index" -> number (再描画用キャッシュバスタ)
  const thumbCanvases = new Map(); // "key/index" -> 現在表示中の <canvas>
  const catRowByKey = new Map(); // key -> li
  const groupElByLabel = new Map(); // label -> { li, badge }

  function markKey(key, index) { return `${key}/${index}`; }

  function persistOpenState() {
    const obj = {};
    openState.forEach((v, k) => { obj[k] = v; });
    saveGroupOpenState(obj);
  }

  function matchesQuery(cat, q) {
    if (!q) return true;
    const label = (cat.label || cat.key || "").toLowerCase();
    const key = (cat.key || "").toLowerCase();
    return label.includes(q) || key.includes(q);
  }

  function thumbUrl(cat, index) {
    const v = thumbVersions.get(markKey(cat.key, index));
    const base = `/api/assets/sprites/${encodeURIComponent(cat.key)}/${index}`;
    return v ? `${base}?v=${v}` : base;
  }

  // シートのサムネイル(左上セルを拡大したもの)を1枚だけ読み込んで描画する。
  // カテゴリを開いた時にだけ呼ぶ(全カテゴリ分の先読みはしない)。
  function loadThumb(cat, index, canvas) {
    const ctx = canvas.getContext("2d");
    if (!ctx) return;
    ctx.imageSmoothingEnabled = false;
    const cellSize = cat.cellSize || 32;
    const img = new Image();
    img.onload = () => {
      // 差し替え等で canvas が既に別用途に回っていたら描かない
      if (thumbCanvases.get(markKey(cat.key, index)) !== canvas) return;
      ctx.clearRect(0, 0, 40, 40);
      ctx.drawImage(img, 0, 0, cellSize, cellSize, 0, 0, 40, 40);
    };
    img.onerror = () => { /* 存在しないシートもあるため、失敗時は空のままにする */ };
    img.src = thumbUrl(cat, index);
  }

  function buildSheetRow(cat, index) {
    const li = document.createElement("li");
    li.className = "ie-sheet-item";
    li._catKey = cat.key;
    li._index = index;
    if (_selectedKey === cat.key && _selectedIndex === index) {
      li.classList.add("is-selected");
    }

    const thumbWrap = document.createElement("div");
    thumbWrap.className = "ie-sheet-thumb-wrap";
    const canvas = document.createElement("canvas");
    canvas.width = 40;
    canvas.height = 40;
    canvas.className = "ie-sheet-thumb";
    thumbWrap.appendChild(canvas);
    li.appendChild(thumbWrap);
    thumbCanvases.set(markKey(cat.key, index), canvas);

    const labelSpan = document.createElement("span");
    labelSpan.className = "ie-sheet-label";
    labelSpan.textContent = `#${index}`;
    li.appendChild(labelSpan);

    const mark = document.createElement("span");
    mark.className = "image-editor-overridden-mark";
    mark.textContent = overriddenMarks.get(markKey(cat.key, index)) ? "上書き中" : "";
    li.appendChild(mark);
    li._mark = mark;

    li.addEventListener("click", () => selectCategory(cat, index));

    loadThumb(cat, index, canvas);
    return li;
  }

  function buildSheetList(cat) {
    const sheetUl = document.createElement("ul");
    sheetUl.className = "ie-sheet-list";
    for (let i = 0; i < cat.sheetCount; i++) {
      sheetUl.appendChild(buildSheetRow(cat, i));
    }
    return sheetUl;
  }

  function setChevron(li, open) {
    if (!li._chevron) return;
    const next = icon(open ? "chevronDown" : "chevronRight");
    next.classList.add("ie-cat-chevron");
    li._chevron.replaceWith(next);
    li._chevron = next;
  }

  function collapseExpanded() {
    if (!_expandedKey) return;
    const li = catRowByKey.get(_expandedKey);
    if (li) {
      if (li._sheetListEl) { li._sheetListEl.remove(); li._sheetListEl = null; }
      setChevron(li, false);
    }
    _expandedKey = null;
  }

  function expandCategory(cat) {
    if (_expandedKey === cat.key) return;
    collapseExpanded();
    _expandedKey = cat.key;
    const li = catRowByKey.get(cat.key);
    if (!li) return;
    setChevron(li, true);
    const sheetUl = buildSheetList(cat);
    li.appendChild(sheetUl);
    li._sheetListEl = sheetUl;
    highlight();
  }

  function toggleExpand(cat) {
    if (_expandedKey === cat.key) {
      collapseExpanded();
    } else {
      expandCategory(cat);
    }
  }

  function buildCategoryRow(cat) {
    const li = document.createElement("li");
    li.className = "ld-list-item image-editor-cat-item";
    li._catKey = cat.key;
    li._sheetListEl = null;
    catRowByKey.set(cat.key, li);

    const rowDiv = document.createElement("div");
    rowDiv.className = "ie-cat-row";

    const nameSpan = document.createElement("span");
    nameSpan.className = "image-editor-cat-label";
    nameSpan.textContent = cat.label || cat.key;
    rowDiv.appendChild(nameSpan);

    if (cat.sheetCount > 1) {
      const countBadge = document.createElement("span");
      countBadge.className = "ie-sheet-count";
      countBadge.textContent = `${cat.sheetCount}枚`;
      rowDiv.appendChild(countBadge);

      const chevron = icon(_expandedKey === cat.key ? "chevronDown" : "chevronRight");
      chevron.classList.add("ie-cat-chevron");
      rowDiv.appendChild(chevron);
      li._chevron = chevron;
    } else {
      const mark = document.createElement("span");
      mark.className = "image-editor-overridden-mark";
      mark.textContent = overriddenMarks.get(markKey(cat.key, 0)) ? "上書き中" : "";
      rowDiv.appendChild(mark);
      li._mark = mark;
    }

    li.appendChild(rowDiv);

    rowDiv.addEventListener("click", () => {
      if (cat.sheetCount > 1) {
        toggleExpand(cat);
      } else {
        selectCategory(cat, 0);
      }
    });

    if (_expandedKey === cat.key) {
      const sheetUl = buildSheetList(cat);
      li.appendChild(sheetUl);
      li._sheetListEl = sheetUl;
    }

    return li;
  }

  function render() {
    const q = searchInput.value.trim().toLowerCase();
    ul.innerHTML = "";
    catRowByKey.clear();
    groupElByLabel.clear();
    thumbCanvases.clear();

    let anyMatch = false;

    groupDefs.forEach((g) => {
      const items = q ? g.items.filter((c) => matchesQuery(c, q)) : g.items;
      if (q && items.length === 0) return;
      anyMatch = true;

      const isOpen = q ? true : Boolean(openState.get(g.label));

      const groupLi = document.createElement("li");
      groupLi.className = "ld-list-group";

      const toggleBtn = document.createElement("button");
      toggleBtn.type = "button";
      toggleBtn.className = "ld-list-group-toggle";

      const chevron = icon(isOpen ? "chevronDown" : "chevronRight");
      chevron.classList.add("ld-list-group-chevron");

      const labelSpan = document.createElement("span");
      labelSpan.className = "ld-list-group-label";
      labelSpan.textContent = g.label;

      const badge = document.createElement("span");
      badge.className = "ld-list-group-badge";
      const overriddenCount = g.items.reduce((sum, cat) => {
        let n = 0;
        for (let i = 0; i < (cat.sheetCount || 1); i++) {
          if (overriddenMarks.get(markKey(cat.key, i))) n++;
        }
        return sum + n;
      }, 0);
      badge.textContent = String(overriddenCount);
      badge.style.display = overriddenCount > 0 ? "" : "none";

      toggleBtn.append(chevron, labelSpan, badge);
      toggleBtn.addEventListener("click", () => {
        if (q) return; // 検索中は個別開閉を無視(全ヒットを開いたまま見せる)
        const next = !openState.get(g.label);
        openState.set(g.label, next);
        persistOpenState();
        render();
      });

      groupLi.appendChild(toggleBtn);
      groupElByLabel.set(g.label, { li: groupLi, badge });

      const itemsUl = document.createElement("ul");
      itemsUl.className = "ld-list-group-items";
      itemsUl.hidden = !isOpen;
      items.forEach((cat) => itemsUl.appendChild(buildCategoryRow(cat)));
      groupLi.appendChild(itemsUl);

      ul.appendChild(groupLi);
    });

    if (q && !anyMatch) {
      const empty = document.createElement("li");
      empty.className = "ld-list-empty";
      empty.textContent = "該当するカテゴリがありません";
      ul.appendChild(empty);
    }

    highlight();
  }

  function highlight() {
    catRowByKey.forEach((li, key) => {
      li.classList.toggle("is-selected", key === _selectedKey);
    });
    if (_expandedKey) {
      const li = catRowByKey.get(_expandedKey);
      if (li?._sheetListEl) {
        Array.from(li._sheetListEl.children).forEach((sli) => {
          sli.classList.toggle("is-selected", sli._catKey === _selectedKey && sli._index === _selectedIndex);
        });
      }
    }
  }

  function selectCategory(cat, index) {
    _selectedKey = cat.key;
    _selectedIndex = index;
    highlight();
    onSelect(cat, index);
  }

  function updateGroupBadge(key) {
    const label = keyToGroupLabel.get(key);
    if (!label) return;
    const entry = groupElByLabel.get(label);
    const g = groupDefs.find((gd) => gd.label === label);
    if (!entry || !g) return;
    const count = g.items.reduce((sum, cat) => {
      let n = 0;
      for (let i = 0; i < (cat.sheetCount || 1); i++) {
        if (overriddenMarks.get(markKey(cat.key, i))) n++;
      }
      return sum + n;
    }, 0);
    entry.badge.textContent = String(count);
    entry.badge.style.display = count > 0 ? "" : "none";
  }

  function setOverriddenMark(key, index, overridden) {
    overriddenMarks.set(markKey(key, index), overridden);

    const li = catRowByKey.get(key);
    if (li) {
      if (li._mark) {
        li._mark.textContent = overridden ? "上書き中" : "";
      }
      if (li._sheetListEl) {
        const sli = Array.from(li._sheetListEl.children).find((c) => c._index === index);
        if (sli?._mark) {
          sli._mark.textContent = overridden ? "上書き中" : "";
        }
      }
    }

    // 差し替え・保存・出荷時復元でシートの絵が変わるので、表示中のサムネイルだけ
    // キャッシュバスタ付きで再読み込みする(全カテゴリの先読みはしない)。
    thumbVersions.set(markKey(key, index), (thumbVersions.get(markKey(key, index)) || 0) + 1);
    const canvas = thumbCanvases.get(markKey(key, index));
    if (canvas) {
      const cat = categories.find((c) => c.key === key);
      if (cat) loadThumb(cat, index, canvas);
    }

    updateGroupBadge(key);
  }

  searchInput.addEventListener("input", () => render());
  searchInput.addEventListener("keydown", (e) => {
    if (e.key === "Escape") {
      searchInput.value = "";
      render();
    }
  });

  render();

  // 途中セーブを開く時など、外から対象を指定して選択させる。
  // グループが閉じていても、対象のグループとカテゴリを開いてから選択する。
  function selectByKey(key, index) {
    const cat = categories.find((c) => c.key === key);
    if (!cat) return false;
    const label = keyToGroupLabel.get(key);
    if (label) {
      openState.set(label, true);
      persistOpenState();
    }
    _expandedKey = cat.sheetCount > 1 ? key : null;
    _selectedKey = key;
    _selectedIndex = Number(index) || 0;
    searchInput.value = "";
    render();
    onSelect(cat, _selectedIndex);
    return true;
  }

  return { el: ul, searchEl: searchInput, setOverriddenMark, selectByKey };
}

// ----------------------------------------------------------------
// 右ペイン: プレビュー + 詳細
// ----------------------------------------------------------------

// タブバー + タブパネルの簡易実装。3 カラム化した右カラム(ie-inspector)で使う。
function buildTabbar(tabs) {
  const bar = document.createElement("div");
  bar.className = "ie-tabbar";
  const panelWrap = document.createElement("div");
  panelWrap.className = "ie-tabpanel-wrap";

  const buttons = new Map();
  const panels = new Map();

  tabs.forEach(({ id, label, iconName }, i) => {
    const btn = document.createElement("button");
    btn.type = "button";
    btn.className = "ie-tab";
    if (iconName) btn.appendChild(icon(iconName));
    btn.appendChild(document.createTextNode(label));
    btn.addEventListener("click", () => select(id));
    bar.appendChild(btn);
    buttons.set(id, btn);

    const panel = document.createElement("div");
    panel.className = "ie-tabpanel";
    panel.hidden = i !== 0;
    panelWrap.appendChild(panel);
    panels.set(id, panel);
  });

  function select(id) {
    buttons.forEach((btn, key) => btn.classList.toggle("is-selected", key === id));
    panels.forEach((panel, key) => { panel.hidden = key !== id; });
  }
  if (tabs[0]) select(tabs[0].id);

  return { bar, panelWrap, panels };
}

// アイコンのみのボタン（セル移動用）。ツールチップと aria-label で補う。
function iconOnlyNavButton(iconName, tipText) {
  const btn = document.createElement("button");
  btn.type = "button";
  btn.className = "button small icon-only";
  btn.appendChild(icon(iconName));
  attachTip(btn, tipText);
  return btn;
}

function buildDetailPane({ onOverriddenChange, categories, onRequestTarget }) {
  const center = document.createElement("div");
  center.className = "ie-center";
  const inspector = document.createElement("div");
  inspector.className = "ie-inspector";

  // 「性別/服/髪/目」の合成の見え方は、合成プレビューとペイントの下敷きで
  // 同じ値を共有する必要があるため、この画面で 1 個だけ作って両方へ渡す。
  const context = createCharContext({ categories });

  let _cat = null;
  let _index = 0;
  let _naturalWidth = 0;
  let _naturalHeight = 0;
  let _scale = 2;
  let _showGrid = true;
  let _revision = 0;
  let _markerColForBreadcrumb = -1;
  let _markerRowForBreadcrumb = 0;
  let _sheetCols = 0;
  let _sheetRows = 0;

  // --- フィードバック(中央上部に固定) ---
  const feedback = document.createElement("p");
  feedback.className = "ie-feedback result-message";
  feedback.style.display = "none";
  feedback.setAttribute("role", "status");
  feedback.setAttribute("aria-live", "polite");
  center.appendChild(feedback);

  // --- ヘッダ行(パンくず) ---
  const centerHead = document.createElement("div");
  centerHead.className = "ie-center-head";
  const breadcrumb = document.createElement("p");
  breadcrumb.className = "ie-breadcrumb";

  // セル移動ボタン。中央で描いている最中に隣のコマへ移りやすくする。
  const cellNav = document.createElement("div");
  cellNav.className = "ie-cellnav";
  const cellNavLeft = iconOnlyNavButton("arrowLeft", "左のセル (←)");
  const cellNavUp = iconOnlyNavButton("arrowUp", "上のセル (↑)");
  const cellNavDown = iconOnlyNavButton("arrowDown", "下のセル (↓)");
  const cellNavRight = iconOnlyNavButton("arrowRight", "右のセル (→)");
  cellNavLeft.addEventListener("click", () => paint.selectCell(_markerColForBreadcrumb - 1, _markerRowForBreadcrumb));
  cellNavRight.addEventListener("click", () => paint.selectCell(_markerColForBreadcrumb + 1, _markerRowForBreadcrumb));
  cellNavUp.addEventListener("click", () => paint.selectCell(_markerColForBreadcrumb, _markerRowForBreadcrumb - 1));
  cellNavDown.addEventListener("click", () => paint.selectCell(_markerColForBreadcrumb, _markerRowForBreadcrumb + 1));
  cellNav.append(cellNavLeft, cellNavUp, cellNavDown, cellNavRight);
  // .ie-cellnav は display:flex を持つ CSS クラスなので、hidden 属性ではなく
  // style.display で切り替える（同じ詳細度の [hidden] と .ie-cellnav が競合し、
  // 後勝ちで hidden が効かなくなるのを避けるため）。
  cellNav.style.display = "none";

  // --- ショートカット一覧(?ボタン) ---
  // ツールチップと同じ理由(別ウィンドウへの adoptNode)で、パネル本体は
  // 表示のたびに helpBtn.ownerDocument.body へ置き直す。
  const helpBtn = document.createElement("button");
  helpBtn.type = "button";
  helpBtn.className = "button small icon-only";
  helpBtn.appendChild(icon("help"));
  attachTip(helpBtn, "キーボードショートカット一覧");
  helpBtn.setAttribute("aria-expanded", "false");

  const shortcutsPanel = document.createElement("div");
  shortcutsPanel.className = "ie-shortcuts";
  shortcutsPanel.hidden = true;
  shortcutsPanel.appendChild((() => {
    const h = document.createElement("h4");
    h.textContent = "キーボードショートカット";
    h.style.margin = "0 0 0.5rem";
    return h;
  })());
  const shortcutsDl = document.createElement("dl");
  [
    ["ペン", "B"], ["消しゴム", "E"], ["スポイト", "I"], ["塗りつぶし", "G"],
    ["元に戻す", "Ctrl+Z"], ["やり直す", "Ctrl+Shift+Z / Ctrl+Y"],
    ["前の色 / 次の色", "[ / ]"],
    ["左のセル", "←"], ["右のセル", "→"], ["上のセル", "↑"], ["下のセル", "↓"],
    ["この内容で保存", "Ctrl+S"],
  ].forEach(([label, key]) => {
    const dt = document.createElement("dt");
    dt.textContent = label;
    const dd = document.createElement("dd");
    const kbd = document.createElement("kbd");
    kbd.textContent = key;
    dd.appendChild(kbd);
    shortcutsDl.append(dt, dd);
  });
  shortcutsPanel.appendChild(shortcutsDl);

  let _shortcutsOpen = false;

  function positionShortcutsPanel() {
    const doc = helpBtn.ownerDocument;
    const win = doc.defaultView || window;
    const rect = helpBtn.getBoundingClientRect();
    const panelRect = shortcutsPanel.getBoundingClientRect();
    let top = rect.bottom + 6;
    if (top + panelRect.height > win.innerHeight) top = rect.top - panelRect.height - 6;
    let left = rect.right - panelRect.width;
    left = Math.max(4, Math.min(left, win.innerWidth - panelRect.width - 4));
    shortcutsPanel.style.left = `${left}px`;
    shortcutsPanel.style.top = `${top}px`;
  }

  function onShortcutsKeydown(e) {
    if (e.key === "Escape") closeShortcutsPanel();
  }
  function onShortcutsOutsideClick(e) {
    if (shortcutsPanel.contains(e.target) || helpBtn.contains(e.target)) return;
    closeShortcutsPanel();
  }

  function openShortcutsPanel() {
    const doc = helpBtn.ownerDocument;
    if (shortcutsPanel.parentNode !== doc.body) doc.body.appendChild(shortcutsPanel);
    shortcutsPanel.hidden = false;
    positionShortcutsPanel();
    helpBtn.setAttribute("aria-expanded", "true");
    _shortcutsOpen = true;
    doc.addEventListener("keydown", onShortcutsKeydown);
    doc.addEventListener("pointerdown", onShortcutsOutsideClick, true);
  }

  function closeShortcutsPanel() {
    if (!_shortcutsOpen) return;
    const doc = shortcutsPanel.ownerDocument;
    shortcutsPanel.hidden = true;
    helpBtn.setAttribute("aria-expanded", "false");
    _shortcutsOpen = false;
    doc.removeEventListener("keydown", onShortcutsKeydown);
    doc.removeEventListener("pointerdown", onShortcutsOutsideClick, true);
  }

  helpBtn.addEventListener("click", () => {
    if (_shortcutsOpen) closeShortcutsPanel();
    else openShortcutsPanel();
  });

  // パンくずの右側にセル移動ボタンと「?」ボタンをまとめて置く
  const centerHeadRight = document.createElement("div");
  centerHeadRight.className = "ie-center-head-right";
  centerHeadRight.append(cellNav, helpBtn);
  centerHead.append(breadcrumb, centerHeadRight);

  center.appendChild(centerHead);

  function updateCellNav() {
    const hasCell = _markerColForBreadcrumb >= 0;
    cellNav.style.display = hasCell ? "" : "none";
    if (!hasCell) return;
    cellNavLeft.disabled = _markerColForBreadcrumb <= 0;
    cellNavRight.disabled = _markerColForBreadcrumb >= _sheetCols - 1;
    cellNavUp.disabled = _markerRowForBreadcrumb <= 0;
    cellNavDown.disabled = _markerRowForBreadcrumb >= _sheetRows - 1;
  }

  function updateBreadcrumb() {
    if (!_cat) { breadcrumb.textContent = "カテゴリを選択してください"; return; }
    let text = `${_cat.label || _cat.key} › #${_index}`;
    if (_markerColForBreadcrumb >= 0) {
      text += ` › セル(${_markerColForBreadcrumb}, ${_markerRowForBreadcrumb})`;
    }
    breadcrumb.textContent = text;
  }

  // --- プレビューセクション(タブ「描く」に配置) ---
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
  gridToggleLabel.append(gridToggleCb, icon("grid"), " グリッド表示");

  previewToolbar.append(
    (() => {
      const s = document.createElement("span");
      s.append(icon("zoom"), " 拡大率:");
      return s;
    })(),
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
  // ペイント対象セルを示す枠。プレビューのクリックで移動する。
  const cellMarker = document.createElement("div");
  cellMarker.className = "ie-cell-marker";
  cellMarker.hidden = true;
  previewInner.append(previewImg, gridOverlay, cellMarker);
  previewStage.appendChild(previewInner);
  previewSec.appendChild(previewStage);

  // プレビュー上のクリック位置からセルを求めてペイント対象にする
  previewInner.addEventListener("click", (e) => {
    if (!_cat || !_naturalWidth || !_naturalHeight) return;
    const cellSize = _cat.cellSize || 32;
    const rect = previewInner.getBoundingClientRect();
    const col = Math.floor((e.clientX - rect.left) / (cellSize * _scale));
    const row = Math.floor((e.clientY - rect.top) / (cellSize * _scale));
    const cols = Math.max(1, Math.round(_naturalWidth / cellSize));
    const rows = Math.max(1, Math.round(_naturalHeight / cellSize));
    if (col < 0 || row < 0 || col >= cols || row >= rows) return;
    paint.selectCell(col, row);
  });

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

  // --- ペイント(S4) ---
  // パレット PNG として読めたシートだけ編集可能。読めない場合は自分で無効表示になる。
  const paint = createSpritePaint({
    categories,
    context,
    onFeedback: (message, type) => showFeedback(feedback, message, type),
    onSaved: () => { void reload(); },
    onCellChange: (col, row, dims) => updateCellMarker(col, row, dims),
    // 途中セーブを開く時に、カテゴリ一覧の選択ごと切り替える
    onRequestTarget: (catKey, sheetIndex) => onRequestTarget?.(catKey, sheetIndex),
  });
  const paintParts = paint.parts;

  // --- 合成プレビュー(S3b) ---
  // 2x2 キャラ系カテゴリのときだけ表示される。setTarget が false を返すと自分で隠れる。
  const composer = createCharComposer({ categories, context });

  // ペイント対象セルの枠をプレビュー上に重ねる。パンくずのセル表示も同時に更新する。
  // col が負なら「このシートはペイントできない」ので枠とパンくずのセル部分を隠す。
  let _markerCol = -1;
  let _markerRow = 0;

  function updateCellMarker(col, row, dims) {
    if (col != null) { _markerCol = col; _markerRow = row; }
    if (dims) { _sheetCols = dims.cols; _sheetRows = dims.rows; }
    _markerColForBreadcrumb = _markerCol;
    _markerRowForBreadcrumb = _markerRow;
    updateBreadcrumb();
    updateCellNav();
    if (_markerCol < 0 || !_cat || !_naturalWidth || !_naturalHeight) {
      cellMarker.hidden = true;
      return;
    }
    const cellSize = _cat.cellSize || 32;
    cellMarker.hidden = false;
    cellMarker.style.left   = `${_markerCol * cellSize * _scale}px`;
    cellMarker.style.top    = `${_markerRow * cellSize * _scale}px`;
    cellMarker.style.width  = `${cellSize * _scale}px`;
    cellMarker.style.height = `${cellSize * _scale}px`;
  }

  function applyImageTransform() {
    if (!_naturalWidth || !_naturalHeight) return;
    const w = _naturalWidth * _scale;
    const h = _naturalHeight * _scale;
    previewInner.style.width = `${w}px`;
    previewInner.style.height = `${h}px`;
    previewImg.style.width = `${w}px`;
    previewImg.style.height = `${h}px`;
    gridOverlay.style.backgroundSize = `${(_cat?.cellSize || 32) * _scale}px ${(_cat?.cellSize || 32) * _scale}px`;
    updateCellMarker();
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

  // --- 履歴セクション ---
  const historySec = document.createElement("section");
  historySec.className = "detail-section";
  const historyH3 = document.createElement("h3");
  historyH3.textContent = "履歴";
  historySec.appendChild(historyH3);
  const historyBody = document.createElement("div");
  historySec.appendChild(historyBody);

  // --- 出荷時復元セクション ---
  const revertSec = document.createElement("section");
  revertSec.className = "detail-section";
  const revertH3 = document.createElement("h3");
  revertH3.textContent = "出荷時に戻す";
  revertSec.appendChild(revertH3);
  const revertBtn = document.createElement("button");
  revertBtn.type = "button";
  revertBtn.className = "button danger";
  revertBtn.appendChild(icon("revert"));
  revertBtn.appendChild((() => {
    const span = document.createElement("span");
    span.className = "btn-label";
    span.textContent = "出荷時の画像に戻す";
    return span;
  })());
  revertSec.appendChild(revertBtn);

  const doRevertToShipped = async () => {
    if (!_cat) return;
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
      paint.refresh();
      await reload();
    } catch (e) {
      showFeedback(feedback, "通信に失敗しました: " + String(e?.message ?? e), "error");
    }
  };

  armConfirmButton(revertBtn, {
    armedLabel: "本当に戻す?（上書きと履歴を全削除）",
    onConfirm: () => { void doRevertToShipped(); },
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
      paint.refresh();
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
      revertBtnRow.appendChild(icon("history"));
      const revertBtnRowLabel = document.createElement("span");
      revertBtnRowLabel.className = "btn-label";
      revertBtnRowLabel.textContent = "この版に戻す";
      revertBtnRow.appendChild(revertBtnRowLabel);
      const doRevertToRevision = async () => {
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
          paint.refresh();
          await reload();
        } catch (e) {
          showFeedback(feedback, "通信に失敗しました: " + String(e?.message ?? e), "error");
        }
      };

      armConfirmButton(revertBtnRow, {
        armedLabel: `本当に版 ${h.revision} へ?`,
        onConfirm: () => { void doRevertToRevision(); },
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
    // ここではフィードバックを消さない。保存・復元の直後にも reload() が走るため、
    // 消すと「保存しました」等の結果表示が出た直後に消えてしまう。
    // 表示のクリアは対象を切り替える setTarget() 側で行う。

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
    showFeedback(feedback, "", null);
    updateBreadcrumb();
    // 2x2 キャラ系（npc2x2 除く）以外では「合成の見え方」は意味を持たないので隠す
    context.setVisible(Boolean(cat) && COMPOSABLE_KEYS.has(cat.key) && cat.key !== "npc2x2");
    // ペイントは未保存の変更があると切り替えを断ることがある（その時は自分で通知する）
    paint.setTarget(cat, index);
    reload();
  }

  applyGridVisibility();
  updateBreadcrumb();

  // ----------------------------------------------------------------
  // 中央カラム(ie-center): ヘッダの下にツールバー・キャンバス・パレット・保存バー
  // ----------------------------------------------------------------
  center.append(paintParts.toolBar, paintParts.stage, paintParts.palette, paintParts.colorRow, paintParts.saveBar);

  // ----------------------------------------------------------------
  // 右カラム(ie-inspector): タブ「描く」「確認」「管理」
  // ----------------------------------------------------------------
  const tabs = buildTabbar([
    { id: "draw", label: "描く", iconName: "pen" },
    { id: "check", label: "確認", iconName: "eye" },
    { id: "manage", label: "管理", iconName: "sliders" },
  ]);
  // 「合成の見え方」は「描く」「確認」どちらのタブからも参照するので、
  // タブの外・タブバーより上に常設で置く。
  inspector.append(context.el, tabs.bar, tabs.panelWrap);

  // 「描く」: 編集不可の理由(desc)を先頭に、シートプレビュー、下敷き、セル選択
  tabs.panels.get("draw").append(paintParts.desc, previewSec, paintParts.underlayBar, paintParts.cellBar);

  // 「確認」: 合成プレビュー
  tabs.panels.get("check").append(composer.el);

  // 「管理」: 状態 / 差し替え / 履歴 / 途中セーブ / 出荷時に戻す
  tabs.panels.get("manage").append(stateSec, uploadSec, historySec, paintParts.draftSec, revertSec);

  return {
    center, inspector, setTarget, feedbackEl: feedback,
    handleShortcut: (event) => paint.handleShortcut(event),
    destroy: () => {
      composer.destroy();
      closeShortcutsPanel();
      shortcutsPanel.remove();
    },
  };
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

  // 3 カラム構成: list-pane(左) / ie-center(中央) / ie-inspector(右)。
  // detail(center/inspector) はカテゴリを選ぶまで作らないので、その間は
  // 案内文だけ shell 直下に置いておく。
  const emptyMsg = document.createElement("p");
  emptyMsg.className = "card-description";
  emptyMsg.style.padding = "1rem";
  emptyMsg.textContent = "左のカテゴリを選択してください";

  shell.append(listPane, emptyMsg);
  container.appendChild(shell);

  let detail = null;

  // ----------------------------------------------------------------
  // キーボードショートカット
  //
  // この画面は「編集を別ウィンドウで開く」機能で DOM ごと別ウィンドウへ
  // adoptNode される(core/workspace-layout.js)。mount 時に document へ
  // listener を張ると、ポップアップ側ではキーが効かない。
  // そこで shell にマウスが乗った/フォーカスが入った時点の
  // shell.ownerDocument へ遅延で束縛する(まだ束縛していないドキュメントだけ)。
  // ----------------------------------------------------------------
  const boundDocs = new Set();

  function isEditableTarget(target) {
    if (!target) return false;
    const tag = target.tagName;
    if (tag === "INPUT" || tag === "TEXTAREA" || tag === "SELECT") return true;
    return Boolean(target.isContentEditable);
  }

  function onShellKeydown(event) {
    // shell が既に外れている、あるいは別ドキュメントに移った後に残った
    // listener からの発火なら何もしない。
    if (!shell.isConnected || shell.ownerDocument !== event.currentTarget) return;
    // 名前入力欄やセレクトなど、入力中の操作を邪魔しない。
    if (isEditableTarget(event.target)) return;
    if (event.altKey || event.metaKey) return;
    if (!detail) return;
    if (detail.handleShortcut(event)) {
      event.preventDefault();
    }
  }

  function bindShortcuts(doc) {
    if (!doc || boundDocs.has(doc)) return;
    boundDocs.add(doc);
    doc.addEventListener("keydown", onShellKeydown);
  }

  shell.addEventListener("pointerenter", () => bindShortcuts(shell.ownerDocument));
  shell.addEventListener("focusin", () => bindShortcuts(shell.ownerDocument));

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
            emptyMsg.remove();
            detail = buildDetailPane({
              onOverriddenChange: (key, idx, overridden) => listUi.setOverriddenMark(key, idx, overridden),
              categories,
              // 途中セーブを開く時にカテゴリ一覧の選択ごと切り替える
              onRequestTarget: (catKey, sheetIndex) => {
                if (!listUi.selectByKey(catKey, sheetIndex)) {
                  showFeedback(detail.feedbackEl, `カテゴリ ${catKey} が見つかりません`, "error");
                }
              },
            });
            shell.append(detail.center, detail.inspector);
          }
          detail.setTarget(cat, index);
        },
      });
      listHeader.appendChild(listUi.searchEl);
      listPane.appendChild(listUi.el);
    } catch (e) {
      listLoading.textContent = "カテゴリ一覧の取得に失敗しました: " + String(e?.message ?? e);
    }
  }

  load();

  // 画面を離れる時にアニメのタイマーを止める
  _destroyFn = () => {
    if (detail?.destroy) detail.destroy();
    boundDocs.forEach((doc) => doc.removeEventListener("keydown", onShellKeydown));
    boundDocs.clear();
    container.innerHTML = "";
    detail = null;
  };
}

/**
 * views/char-edit.js
 * キャラクター基本情報編集画面 (route: character-overview)
 *
 * API:
 *   GET  /api/characters/{charId}
 *   PUT  /api/characters/{charId}
 *   PUT  /api/characters/{charId}/status
 *   PUT  /api/characters/{charId}/equipment
 *   PUT  /api/characters/{charId}/graphics
 *   GET/POST/DELETE /api/characters/{charId}/items[/{slot}]
 *   GET/POST/DELETE /api/characters/{charId}/skills[/{slot}]
 *   GET  /api/characters/{charId}/account
 *   PUT  /api/characters/{charId}/account
 *   PUT  /api/characters/{charId}/admin
 *   PUT  /api/characters/{charId}/disabled
 */

import { fetchJson } from "../core/api.js";
import { createSpriteField } from "../components/sprite-picker.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";

// ----------------------------------------------------------------
// アイテム情報マップキャッシュ (itemId → { name, iconGrpId })
// ----------------------------------------------------------------
var _itemInfoMap = null;

async function ensureItemInfoMap() {
  if (_itemInfoMap !== null) { return; }
  try {
    var { response, data } = await fetchJson("/api/items");
    if (!response || !response.ok) {
      _itemInfoMap = new Map();
      return;
    }
    var list = Array.isArray(data) ? data : (data && Array.isArray(data.items) ? data.items : []);
    _itemInfoMap = new Map();
    list.forEach(function (it) {
      _itemInfoMap.set(it.itemId, { name: it.name || "", iconGrpId: it.iconGrpId || null });
    });
  } catch (_) {
    _itemInfoMap = new Map(); // 失敗時は空 Map にして再試行ループを避ける
  }
}

// ----------------------------------------------------------------
// ユーティリティ
// ----------------------------------------------------------------

function setFb(el, msg, type) {
  if (!el) { return; }
  el.textContent = msg || "";
  el.className = "form-feedback" + (type ? " " + type : "");
}

function numVal(input) {
  if (!input) { return null; }
  var s = input.value.trim();
  if (s === "") { return null; }
  var n = parseInt(s, 10);
  return isNaN(n) ? null : n;
}

function mkEl(tag, cls, text) {
  var el = document.createElement(tag);
  if (cls) { el.className = cls; }
  if (text !== undefined) { el.textContent = text; }
  return el;
}

function mkInput(type, cls) {
  var inp = document.createElement("input");
  inp.type = type || "text";
  if (cls) { inp.className = cls; }
  return inp;
}

function mkField(labelText) {
  var div = mkEl("div", "form-field");
  var lbl = mkEl("label", "", labelText);
  div.appendChild(lbl);
  return { wrap: div, lbl };
}

function mkNumField(labelText) {
  var { wrap, lbl } = mkField(labelText);
  var inp = mkInput("number", "form-input");
  lbl.appendChild(inp);
  wrap.appendChild(inp);
  return { wrap, inp };
}

function mkTextFieldEl(labelText, maxlength) {
  var { wrap, lbl } = mkField(labelText);
  var inp = mkInput("text", "form-input");
  if (maxlength) { inp.maxLength = maxlength; }
  lbl.appendChild(inp);
  wrap.appendChild(inp);
  return { wrap, inp };
}

function mkSpan(labelText) {
  var { wrap, lbl } = mkField(labelText);
  var span = mkEl("span", "form-value", "-");
  wrap.appendChild(span);
  return { wrap, span };
}

function mkCheckbox(labelText) {
  var wrap = mkEl("label", "form-field form-field--checkbox");
  var cb = mkInput("checkbox");
  var span = mkEl("span", "", labelText);
  wrap.append(cb, " ", span);
  return { wrap, cb };
}

async function putJson(url, body) {
  var res = await fetch(url, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    credentials: "same-origin",
    body: JSON.stringify(body),
  });
  var data = null;
  try { data = await res.json(); } catch (_) {}
  return { response: res, data };
}

// ----------------------------------------------------------------
// タブ管理
// ----------------------------------------------------------------

function buildTabNav(tabs) {
  var nav = mkEl("div", "tab-nav");
  nav.setAttribute("role", "tablist");
  tabs.forEach(function (t, i) {
    var btn = mkEl("button", "tab-btn" + (i === 0 ? " is-active" : ""), t.label);
    btn.type = "button";
    btn.setAttribute("role", "tab");
    btn.dataset.tab = t.id;
    nav.appendChild(btn);
  });
  return nav;
}

function activateTab(nav, panels, tabId) {
  nav.querySelectorAll(".tab-btn").forEach(function (b) {
    b.classList.toggle("is-active", b.dataset.tab === tabId);
  });
  panels.forEach(function (p) {
    p.style.display = (p.id === tabId) ? "" : "none";
  });
}

// ----------------------------------------------------------------
// タブパネル: 基本情報
// ----------------------------------------------------------------

function buildBasicTab() {
  var panel = mkEl("div", "tab-panel", "");
  panel.id = "ce-tab-basic";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var form = mkEl("form", "edit-form");
  form.id = "ce-basic-form";
  panel.appendChild(form);

  var grid = mkEl("div", "form-grid");
  form.appendChild(grid);

  var charIdSpan = mkSpan("CharID");
  var accountIdSpan = mkSpan("アカウントID");
  var moveStateSpan = mkSpan("移動状態");
  var isNpcSpan = mkSpan("NPC");
  grid.append(charIdSpan.wrap, accountIdSpan.wrap, moveStateSpan.wrap, isNpcSpan.wrap);

  var charName = mkTextFieldEl("キャラ名", 255);
  var mapId    = mkNumField("マップID");
  var x        = mkNumField("座標 X");
  var y        = mkNumField("座標 Y");
  var direction= mkNumField("向き");
  var moveType = mkNumField("移動種別");
  var motionTypeId = mkNumField("モーション種別ID");
  var familyId = mkNumField("種族ID");
  var grpSize  = mkNumField("画像サイズ");
  var sex      = mkNumField("性別");
  grid.append(charName.wrap, mapId.wrap, x.wrap, y.wrap,
    direction.wrap, moveType.wrap, motionTypeId.wrap,
    familyId.wrap, grpSize.wrap, sex.wrap);

  var block = mkCheckbox("ブロック");
  var push  = mkCheckbox("プッシュ");
  grid.append(block.wrap, push.wrap);

  var actions = mkEl("div", "form-actions");
  var saveBtn = mkEl("button", "button primary", "保存");
  saveBtn.type = "submit";
  actions.appendChild(saveBtn);
  form.appendChild(actions);

  var refs = {
    panel, fb, form,
    charIdSpan: charIdSpan.span,
    accountIdSpan: accountIdSpan.span,
    moveStateSpan: moveStateSpan.span,
    isNpcSpan: isNpcSpan.span,
    charNameInp: charName.inp,
    mapIdInp: mapId.inp,
    xInp: x.inp,
    yInp: y.inp,
    directionInp: direction.inp,
    moveTypeInp: moveType.inp,
    motionTypeIdInp: motionTypeId.inp,
    familyIdInp: familyId.inp,
    grpSizeInp: grpSize.inp,
    sexInp: sex.inp,
    blockCb: block.cb,
    pushCb: push.cb,
  };
  return refs;
}

// ----------------------------------------------------------------
// タブパネル: ステータス
// ----------------------------------------------------------------

function buildStatusTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-status";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var form = mkEl("form", "edit-form");
  form.id = "ce-status-form";
  panel.appendChild(form);

  var grid = mkEl("div", "form-grid");
  form.appendChild(grid);

  var fields = [
    ["hp","HP"],["maxHp","最大HP"],["sp","SP"],["maxSp","最大SP"],
    ["exp","経験値"],["level","レベル"],["stamina","スタミナ"],["power","腕力"],
    ["strength","体力"],["magic","魔力"],["skillful","器用"],
    ["abilityAT","攻撃技能"],["abilityDF","防御技能"],
    ["pAtack","攻撃力"],["pDefense","防御力"],["pMagic","魔法力"],
    ["pMagicDefense","魔法防御力"],["pHitAverage","命中率"],
    ["pAvoidAverage","回避率"],["pCriticalAverage","クリティカル率"],
    ["attrFire","属性[火]"],["attrWind","属性[風]"],["attrWater","属性[水]"],
    ["attrEarth","属性[土]"],["attrLight","属性[光]"],["attrDark","属性[闇]"],
  ];

  var inputs = {};
  fields.forEach(function (pair) {
    var key = pair[0];
    var lbl = pair[1];
    var f = mkNumField(lbl);
    grid.appendChild(f.wrap);
    inputs[key] = f.inp;
  });

  var actions = mkEl("div", "form-actions");
  var saveBtn = mkEl("button", "button primary", "保存");
  saveBtn.type = "submit";
  actions.appendChild(saveBtn);
  form.appendChild(actions);

  return { panel, fb, form, inputs };
}

// ----------------------------------------------------------------
// タブパネル: 装備
// ----------------------------------------------------------------

function buildEquipTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-equip";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var form = mkEl("form", "edit-form");
  form.id = "ce-equip-form";
  panel.appendChild(form);

  var grid = mkEl("div", "form-grid");
  form.appendChild(grid);

  var fields = [
    ["cloth","服"],["accesory1","アクセサリ1"],["accesory2","アクセサリ2"],
    ["armsRight","右手"],["armsLeft","左手"],["head","頭"],
  ];

  var inputs = {};
  fields.forEach(function (pair) {
    var f = mkNumField(pair[1]);
    grid.appendChild(f.wrap);
    inputs[pair[0]] = f.inp;
  });

  var actions = mkEl("div", "form-actions");
  var saveBtn = mkEl("button", "button primary", "保存");
  saveBtn.type = "submit";
  actions.appendChild(saveBtn);
  form.appendChild(actions);

  return { panel, fb, form, inputs };
}

// ----------------------------------------------------------------
// タブパネル: グラフィック（sprite-picker 付き）
// ----------------------------------------------------------------

function buildGraphicsTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-graphics";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var form = mkEl("form", "edit-form");
  form.id = "ce-graphics-form";
  panel.appendChild(form);

  var grid = mkEl("div", "form-grid compact");
  form.appendChild(grid);

  // NPC/PC 系は npc カテゴリ。キャラ系は char/char2x2 可。allowCategorySwitch: true
  var graphicFields = [
    { key: "npc",          label: "NPC",           cat: "npc"  },
    { key: "cloth",        label: "服",             cat: "char" },
    { key: "eye",          label: "目",             cat: "char" },
    { key: "eyeColor",     label: "目色",           cat: "char" },
    { key: "hairType",     label: "髪",             cat: "char" },
    { key: "hairColor",    label: "髪色",           cat: "char" },
    { key: "sp",           label: "特殊服",          cat: "char" },
    { key: "tmpMain",      label: "一時服(メイン)",   cat: "char" },
    { key: "tmpSub",       label: "一時服(サブ)",     cat: "char" },
    { key: "acce",         label: "アクセサリ",       cat: "char" },
    { key: "armsMain",     label: "持ち物(メイン)",   cat: "char" },
    { key: "armsSub",      label: "持ち物(サブ)",     cat: "char" },
    { key: "armsLeftMain", label: "盾(メイン)",       cat: "char" },
    { key: "armsLeftSub",  label: "盾(サブ)",         cat: "char" },
    { key: "initNpc",      label: "初期NPC",          cat: "npc"  },
    { key: "initCloth",    label: "初期服",           cat: "char" },
    { key: "initEye",      label: "初期目",           cat: "char" },
    { key: "initEyeColor", label: "初期目色",          cat: "char" },
    { key: "initHairType", label: "初期髪",           cat: "char" },
    { key: "initHairColor",label: "初期髪色",          cat: "char" },
    { key: "initSp",       label: "初期特殊服",        cat: "char" },
  ];

  var spriteFields = {};
  graphicFields.forEach(function (f) {
    var sf = createSpriteField({
      categoryKey: f.cat,
      value: 0,
      label: f.label,
      allowCategorySwitch: true,
    });
    grid.appendChild(sf.el);
    spriteFields[f.key] = sf;
  });

  var actions = mkEl("div", "form-actions");
  var saveBtn = mkEl("button", "button primary", "保存");
  saveBtn.type = "submit";
  actions.appendChild(saveBtn);
  form.appendChild(actions);

  return { panel, fb, form, spriteFields };
}

// ----------------------------------------------------------------
// タブパネル: 移動（読み取り専用）
// ----------------------------------------------------------------

function buildMovementTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-movement";
  panel.style.display = "none";

  var dl = mkEl("dl", "detail-list");
  panel.appendChild(dl);

  var spanMap = {};
  var mvFields = [
    ["maxItemCount","最大アイテム所持数"],["dropItemAverage","ドロップ率"],
    ["moveAverage","移動確率"],["moveAverageBattle","戦闘時移動確率"],
    ["moveWait","移動待ち(ms)"],["moveWaitBattle","戦闘時移動待ち(ms)"],
    ["searchCX","索敵範囲CX"],["searchCY","索敵範囲CY"],
    ["motionTypeId","モーション種別ID"],
  ];
  mvFields.forEach(function (pair) {
    var div = mkEl("div");
    var dt = mkEl("dt", "", pair[1]);
    var dd = mkEl("dd", "", "-");
    div.append(dt, dd);
    dl.appendChild(div);
    spanMap[pair[0]] = dd;
  });

  return { panel, spanMap };
}

// ----------------------------------------------------------------
// タブパネル: NPC発生（読み取り専用）
// ----------------------------------------------------------------

function buildNpcSpawnTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-npcspawn";
  panel.style.display = "none";

  var dl = mkEl("dl", "detail-list");
  panel.appendChild(dl);

  var spanMap = {};
  var spawnFields = [
    ["putCycle","発生周期"],["putMoveType","発生させる移動種別"],
    ["maxPutCount","同時発生数"],["putAverage","発生確率"],
    ["putAreaX","発生範囲X"],["putAreaY","発生範囲Y"],
  ];
  spawnFields.forEach(function (pair) {
    var div = mkEl("div");
    var dt = mkEl("dt", "", pair[1]);
    var dd = mkEl("dd", "", "-");
    div.append(dt, dd);
    dl.appendChild(div);
    spanMap[pair[0]] = dd;
  });

  return { panel, spanMap };
}

// ----------------------------------------------------------------
// タブパネル: 所持アイテム
// ----------------------------------------------------------------

function buildItemsTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-items";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var addRow = mkEl("div", "filter-row");
  var itemIdInp = mkInput("number");
  itemIdInp.placeholder = "例: 1001";
  itemIdInp.min = "1";
  var addBtn = mkEl("button", "button primary", "追加");
  addBtn.type = "button";
  addRow.append(mkEl("label", "", "アイテムID: "), itemIdInp, addBtn);
  panel.appendChild(addRow);

  var table = mkEl("table", "data-table");
  var thead = mkEl("thead");
  thead.innerHTML = "<tr><th>スロット</th><th>アイテム</th><th>操作</th></tr>";
  var tbody = mkEl("tbody");
  tbody.innerHTML = "<tr><td colspan='3'>（データなし）</td></tr>";
  table.append(thead, tbody);
  panel.appendChild(table);

  return { panel, fb, itemIdInp, addBtn, tbody };
}

// ----------------------------------------------------------------
// タブパネル: 所持スキル
// ----------------------------------------------------------------

function buildSkillsTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-skills";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var addRow = mkEl("div", "filter-row");
  var skillIdInp = mkInput("number");
  skillIdInp.placeholder = "例: 101";
  skillIdInp.min = "1";
  var addBtn = mkEl("button", "button primary", "追加");
  addBtn.type = "button";
  addRow.append(mkEl("label", "", "スキルID: "), skillIdInp, addBtn);
  panel.appendChild(addRow);

  var table = mkEl("table", "data-table");
  var thead = mkEl("thead");
  thead.innerHTML = "<tr><th>スロット</th><th>スキルID</th><th>操作</th></tr>";
  var tbody = mkEl("tbody");
  tbody.innerHTML = "<tr><td colspan='3'>（データなし）</td></tr>";
  table.append(thead, tbody);
  panel.appendChild(table);

  return { panel, fb, skillIdInp, addBtn, tbody };
}

// ----------------------------------------------------------------
// タブパネル: アカウント情報
// ----------------------------------------------------------------

function buildAccountTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-account";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  // アカウント情報表示
  var infoCard = mkEl("section", "card");
  infoCard.style.marginBottom = "1rem";
  var infoH3 = mkEl("h3", "", "アカウント情報");
  infoH3.style.marginTop = "0";
  infoCard.appendChild(infoH3);

  var dl = mkEl("dl", "detail-list");
  var acctSpans = {};
  [["accountId","アカウントID"],["loginId","ログインID"],["ip","IPアドレス"],["mac","MACアドレス"]].forEach(function (pair) {
    var div = mkEl("div");
    var dt = mkEl("dt", "", pair[1]);
    var dd = mkEl("dd", "", "-");
    div.append(dt, dd);
    dl.appendChild(div);
    acctSpans[pair[0]] = dd;
  });
  infoCard.appendChild(dl);
  var reloadBtn = mkEl("button", "button secondary", "再読み込み");
  reloadBtn.type = "button";
  infoCard.appendChild(reloadBtn);
  panel.appendChild(infoCard);

  // アカウント紐付け変更
  var linkCard = mkEl("section", "card");
  linkCard.style.marginBottom = "1rem";
  var linkH3 = mkEl("h3", "", "アカウント紐付け変更");
  linkH3.style.marginTop = "0";
  linkCard.appendChild(linkH3);
  var linkRow = mkEl("div", "filter-row");
  var newAcctInp = mkInput("number");
  newAcctInp.placeholder = "例: 1001";
  newAcctInp.min = "0";
  var linkSaveBtn = mkEl("button", "button primary", "変更");
  linkSaveBtn.type = "button";
  var unlinkBtn = mkEl("button", "button danger", "解除（0）");
  unlinkBtn.type = "button";
  linkRow.append(mkEl("label", "", "アカウントID: "), newAcctInp, linkSaveBtn, unlinkBtn);
  linkCard.appendChild(linkRow);
  panel.appendChild(linkCard);

  // 管理者権限レベル
  var adminCard = mkEl("section", "card");
  adminCard.style.marginBottom = "1rem";
  var adminH3 = mkEl("h3", "", "管理者権限レベル");
  adminH3.style.marginTop = "0";
  adminCard.appendChild(adminH3);
  var adminRow = mkEl("div", "filter-row");
  var adminLevelInp = mkInput("number");
  adminLevelInp.placeholder = "例: 0";
  adminLevelInp.min = "0";
  adminLevelInp.max = "255";
  var adminSaveBtn = mkEl("button", "button primary", "保存");
  adminSaveBtn.type = "button";
  adminRow.append(mkEl("label", "", "権限レベル(0-255): "), adminLevelInp, adminSaveBtn);
  adminCard.appendChild(adminRow);
  panel.appendChild(adminCard);

  // BAN
  var banCard = mkEl("section", "card");
  var banH3 = mkEl("h3", "", "アカウント無効化（BAN）");
  banH3.style.marginTop = "0";
  banCard.appendChild(banH3);
  var disabledStatusSpan = mkEl("dd", "", "-");
  var disabledDl = mkEl("dl", "detail-list");
  var disabledDiv = mkEl("div");
  disabledDiv.append(mkEl("dt", "", "現在の状態"), disabledStatusSpan);
  disabledDl.appendChild(disabledDiv);
  disabledDl.style.marginBottom = "0.75rem";
  banCard.appendChild(disabledDl);
  var banActions = mkEl("div", "form-actions");
  var banBtn = mkEl("button", "button danger", "BAN（無効化）");
  banBtn.type = "button";
  var unbanBtn = mkEl("button", "button secondary", "BAN 解除");
  unbanBtn.type = "button";
  banActions.append(banBtn, unbanBtn);
  banCard.appendChild(banActions);
  panel.appendChild(banCard);

  return { panel, fb, acctSpans, reloadBtn, newAcctInp, linkSaveBtn, unlinkBtn,
    adminLevelInp, adminSaveBtn, banBtn, unbanBtn, disabledStatusSpan };
}

// ----------------------------------------------------------------
// mount
// ----------------------------------------------------------------

let _destroyFn = null;

export function mount(container) {
  if (_destroyFn) { _destroyFn(); _destroyFn = null; }
  container.innerHTML = "";

  var card = mkEl("section", "card");
  card.id = "char-detail-card";
  container.appendChild(card);

  var h2 = mkEl("h2", "", "キャラクター編集");
  card.appendChild(h2);

  var desc = mkEl("p", "card-description", "CharID を入力するか、キャラクター一覧の行をクリックすると詳細が表示されます。");
  card.appendChild(desc);

  // CharID 入力行
  var idRow = mkEl("div", "filter-row");
  var idInp = mkInput("number");
  idInp.placeholder = "例: 1001";
  idInp.min = "1";
  var loadBtn = mkEl("button", "button", "表示");
  loadBtn.type = "button";
  idRow.append(mkEl("label", "", "CharID: "), idInp, loadBtn);
  card.appendChild(idRow);

  var mainFb = mkEl("p", "form-feedback");
  mainFb.setAttribute("aria-live", "polite");
  card.appendChild(mainFb);

  // 詳細エリア（最初は非表示）
  var detailArea = mkEl("div");
  detailArea.id = "char-detail-body";
  detailArea.style.display = "none";
  card.appendChild(detailArea);

  // タブ定義
  var TABS = [
    { id: "ce-tab-basic",    label: "基本" },
    { id: "ce-tab-status",   label: "ステータス" },
    { id: "ce-tab-equip",    label: "装備" },
    { id: "ce-tab-graphics", label: "グラフィック" },
    { id: "ce-tab-movement", label: "移動" },
    { id: "ce-tab-npcspawn", label: "NPC発生" },
    { id: "ce-tab-items",    label: "アイテム" },
    { id: "ce-tab-skills",   label: "スキル" },
    { id: "ce-tab-account",  label: "アカウント" },
  ];

  var tabNav = buildTabNav(TABS);
  detailArea.appendChild(tabNav);

  // タブパネルを構築
  var basicTab      = buildBasicTab();
  var statusTab     = buildStatusTab();
  var equipTab      = buildEquipTab();
  var graphicsTab   = buildGraphicsTab();
  var movementTab   = buildMovementTab();
  var npcSpawnTab   = buildNpcSpawnTab();
  var itemsTab      = buildItemsTab();
  var skillsTab     = buildSkillsTab();
  var accountTab    = buildAccountTab();

  var allPanels = [
    basicTab.panel, statusTab.panel, equipTab.panel, graphicsTab.panel,
    movementTab.panel, npcSpawnTab.panel, itemsTab.panel, skillsTab.panel,
    accountTab.panel,
  ];
  allPanels.forEach(function (p) { detailArea.appendChild(p); });

  // タブクリック
  tabNav.addEventListener("click", function (ev) {
    var btn = ev.target.closest(".tab-btn");
    if (!btn) { return; }
    var tabId = btn.dataset.tab;
    activateTab(tabNav, allPanels, tabId);
    if (!currentCharId) { return; }
    if (tabId === "ce-tab-items")   { doFetchItems(); }
    if (tabId === "ce-tab-skills")  { doFetchSkills(); }
    if (tabId === "ce-tab-account") { doFetchAccount(); }
  });

  // ----------------------------------------------------------------
  // 状態
  // ----------------------------------------------------------------
  var currentCharId = 0;

  // ----------------------------------------------------------------
  // データロード
  // ----------------------------------------------------------------
  async function doFetchChar(charId) {
    setFb(mainFb, "読み込み中...", "");
    detailArea.style.display = "none";
    try {
      var { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(charId));
      if (!response.ok || !data) {
        var msg = (data && data.error) ? data.error : "キャラクター詳細の取得に失敗しました";
        setFb(mainFb, "エラー: " + msg, "error");
        return;
      }
      setFb(mainFb, "", "");
      currentCharId = charId;
      renderCharDetail(data);
      detailArea.style.display = "";
      idInp.value = String(charId);
    } catch (err) {
      console.error("char-edit fetchChar error", err);
      setFb(mainFb, "通信エラーが発生しました", "error");
    }
  }

  function renderCharDetail(d) {
    function setText(span, val) {
      if (span) { span.textContent = (val === null || val === undefined) ? "-" : String(val); }
    }
    function setNumInp(inp, val) {
      if (inp) { inp.value = (val === null || val === undefined) ? "" : String(val); }
    }
    function setCbInp(cb, val) {
      if (cb) { cb.checked = !!val; }
    }

    // 基本
    setText(basicTab.charIdSpan,    d.charId);
    setText(basicTab.accountIdSpan, d.accountId);
    setText(basicTab.moveStateSpan, d.moveState);
    setText(basicTab.isNpcSpan,     d.isNpc ? "NPC" : "PC");
    if (basicTab.charNameInp) { basicTab.charNameInp.value = d.charName || ""; }
    setNumInp(basicTab.mapIdInp,      d.mapId);
    setNumInp(basicTab.xInp,          d.x);
    setNumInp(basicTab.yInp,          d.y);
    setNumInp(basicTab.directionInp,  d.direction);
    setNumInp(basicTab.moveTypeInp,   d.moveType);
    setNumInp(basicTab.motionTypeIdInp, d.motionTypeId);
    setNumInp(basicTab.familyIdInp,   d.familyId);
    setNumInp(basicTab.grpSizeInp,    d.grpSize);
    setNumInp(basicTab.sexInp,        d.sex);
    setCbInp(basicTab.blockCb, d.block);
    setCbInp(basicTab.pushCb,  d.push);

    // ステータス
    if (d.status) {
      var s = d.status;
      Object.keys(statusTab.inputs).forEach(function (k) { setNumInp(statusTab.inputs[k], s[k]); });
    }

    // 装備
    if (d.equipment) {
      var e = d.equipment;
      Object.keys(equipTab.inputs).forEach(function (k) { setNumInp(equipTab.inputs[k], e[k]); });
    }

    // グラフィック
    if (d.graphics) {
      var g = d.graphics;
      Object.keys(graphicsTab.spriteFields).forEach(function (k) {
        var sf = graphicsTab.spriteFields[k];
        if (sf && g[k] !== undefined) { sf.setValue(g[k] || 0); }
      });
    }

    // 移動
    if (d.movement) {
      var mv = d.movement;
      var sm = movementTab.spanMap;
      if (sm.maxItemCount)      { sm.maxItemCount.textContent = String(mv.maxItemCount ?? "-"); }
      if (sm.dropItemAverage)   { sm.dropItemAverage.textContent = String(mv.dropItemAverage ?? "-"); }
      if (sm.moveAverage)       { sm.moveAverage.textContent = String(mv.moveAverage ?? "-"); }
      if (sm.moveAverageBattle) { sm.moveAverageBattle.textContent = String(mv.moveAverageBattle ?? "-"); }
      if (sm.moveWait)          { sm.moveWait.textContent = String(mv.moveWait ?? "-"); }
      if (sm.moveWaitBattle)    { sm.moveWaitBattle.textContent = String(mv.moveWaitBattle ?? "-"); }
      if (sm.searchCX)          { sm.searchCX.textContent = String(mv.searchDistanceCX ?? "-"); }
      if (sm.searchCY)          { sm.searchCY.textContent = String(mv.searchDistanceCY ?? "-"); }
      if (sm.motionTypeId)      { sm.motionTypeId.textContent = String(mv.motionTypeId ?? "-"); }
    }

    // NPC発生
    if (d.npcSpawn) {
      var ns = d.npcSpawn;
      var ss = npcSpawnTab.spanMap;
      if (ss.putCycle)    { ss.putCycle.textContent = String(ns.putCycle ?? "-"); }
      if (ss.putMoveType) { ss.putMoveType.textContent = String(ns.putMoveType ?? "-"); }
      if (ss.maxPutCount) { ss.maxPutCount.textContent = String(ns.maxPutCount ?? "-"); }
      if (ss.putAverage)  { ss.putAverage.textContent = String(ns.putAverage ?? "-"); }
      if (ss.putAreaX)    { ss.putAreaX.textContent = String(ns.putAreaX ?? "-"); }
      if (ss.putAreaY)    { ss.putAreaY.textContent = String(ns.putAreaY ?? "-"); }
    }
  }

  // ----------------------------------------------------------------
  // 基本情報 保存
  // ----------------------------------------------------------------
  basicTab.form.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) { setFb(basicTab.fb, "先にキャラクターを表示してください", "error"); return; }
    setFb(basicTab.fb, "保存中...", "");

    var body = {};
    body.charName    = basicTab.charNameInp.value;
    var mapId2 = numVal(basicTab.mapIdInp);      if (mapId2 !== null) { body.mapId = mapId2; }
    var x2 = numVal(basicTab.xInp);             if (x2 !== null) { body.x = x2; }
    var y2 = numVal(basicTab.yInp);             if (y2 !== null) { body.y = y2; }
    var dir = numVal(basicTab.directionInp);    if (dir !== null) { body.direction = dir; }
    var mt = numVal(basicTab.moveTypeInp);      if (mt !== null) { body.moveType = mt; }
    var moti = numVal(basicTab.motionTypeIdInp);if (moti !== null) { body.motionTypeId = moti; }
    var fam = numVal(basicTab.familyIdInp);     if (fam !== null) { body.familyId = fam; }
    var gs = numVal(basicTab.grpSizeInp);       if (gs !== null) { body.grpSize = gs; }
    var sx = numVal(basicTab.sexInp);           if (sx !== null) { body.sex = sx; }
    body.block = basicTab.blockCb.checked;
    body.push  = basicTab.pushCb.checked;

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId, body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFb(basicTab.fb, "エラー: " + msg, "error");
        return;
      }
      setFb(basicTab.fb, "保存しました", "success");
      await doFetchChar(currentCharId);
    } catch (err) {
      console.error("char-edit basic save error", err);
      setFb(basicTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // ステータス 保存
  // ----------------------------------------------------------------
  statusTab.form.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) { setFb(statusTab.fb, "先にキャラクターを表示してください", "error"); return; }
    setFb(statusTab.fb, "保存中...", "");

    var body = {};
    Object.keys(statusTab.inputs).forEach(function (k) {
      var v = numVal(statusTab.inputs[k]);
      if (v !== null) { body[k] = v; }
    });

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/status", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFb(statusTab.fb, "エラー: " + msg, "error");
        return;
      }
      setFb(statusTab.fb, "保存しました", "success");
      await doFetchChar(currentCharId);
    } catch (err) {
      console.error("char-edit status save error", err);
      setFb(statusTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // 装備 保存
  // ----------------------------------------------------------------
  equipTab.form.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) { setFb(equipTab.fb, "先にキャラクターを表示してください", "error"); return; }
    setFb(equipTab.fb, "保存中...", "");

    var body = {};
    Object.keys(equipTab.inputs).forEach(function (k) {
      var v = numVal(equipTab.inputs[k]);
      if (v !== null) { body[k] = v; }
    });

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/equipment", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFb(equipTab.fb, "エラー: " + msg, "error");
        return;
      }
      setFb(equipTab.fb, "保存しました", "success");
      await doFetchChar(currentCharId);
    } catch (err) {
      console.error("char-edit equip save error", err);
      setFb(equipTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // グラフィック 保存
  // ----------------------------------------------------------------
  graphicsTab.form.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) { setFb(graphicsTab.fb, "先にキャラクターを表示してください", "error"); return; }
    setFb(graphicsTab.fb, "保存中...", "");

    var body = {};
    Object.keys(graphicsTab.spriteFields).forEach(function (k) {
      body[k] = graphicsTab.spriteFields[k].getValue();
    });

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/graphics", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFb(graphicsTab.fb, "エラー: " + msg, "error");
        return;
      }
      setFb(graphicsTab.fb, "保存しました", "success");
      await doFetchChar(currentCharId);
    } catch (err) {
      console.error("char-edit graphics save error", err);
      setFb(graphicsTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // 所持アイテム
  // ----------------------------------------------------------------
  async function doFetchItems() {
    if (!currentCharId) { return; }
    setFb(itemsTab.fb, "読み込み中...", "");
    try {
      var { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(currentCharId) + "/items");
      if (!response.ok) {
        setFb(itemsTab.fb, "エラー: " + ((data && data.error) || "取得失敗"), "error");
        return;
      }
      setFb(itemsTab.fb, "", "");
      await ensureItemInfoMap();
      renderItems(data || []);
    } catch (err) {
      setFb(itemsTab.fb, "通信エラーが発生しました", "error");
    }
  }

  function renderItems(items) {
    if (!items.length) {
      itemsTab.tbody.innerHTML = "<tr><td colspan='3'>（データなし）</td></tr>";
      return;
    }
    itemsTab.tbody.innerHTML = "";
    items.forEach(function (item) {
      var info = (_itemInfoMap && _itemInfoMap.get(item.itemId)) || null;
      var tr = mkEl("tr");
      var tdSlot = mkEl("td", "", String(item.slot));

      // アイテム列: アイコン + [ID] 名前 を横並び
      var tdItem = mkEl("td");
      tdItem.style.display = "flex";
      tdItem.style.alignItems = "center";
      tdItem.style.gap = "6px";
      if (info && info.iconGrpId) {
        var thumb = createSpriteThumb({ categoryKey: "item", sub: info.iconGrpId, size: 20 });
        tdItem.appendChild(thumb.el);
      }
      var itemName = (info && info.name) ? info.name : "(名前なし)";
      var spanLabel = mkEl("span", "", "[" + String(item.itemId) + "] " + itemName);
      tdItem.appendChild(spanLabel);

      var tdOp   = mkEl("td");
      var delBtn = mkEl("button", "button danger btn-sm", "削除");
      delBtn.type = "button";
      delBtn.addEventListener("click", function () { doDeleteItem(item.slot); });
      tdOp.appendChild(delBtn);
      tr.append(tdSlot, tdItem, tdOp);
      itemsTab.tbody.appendChild(tr);
    });
  }

  async function doDeleteItem(slot) {
    setFb(itemsTab.fb, "削除中...", "");
    try {
      var res = await fetch("/api/characters/" + encodeURIComponent(currentCharId) + "/items/" + slot, {
        method: "DELETE", credentials: "same-origin"
      });
      if (res.status === 204) {
        setFb(itemsTab.fb, "削除しました（スロット " + slot + "）", "success");
        doFetchItems();
        return;
      }
      var data = null;
      try { data = await res.json(); } catch (_) {}
      setFb(itemsTab.fb, "エラー: " + ((data && data.error) || "HTTP " + res.status), "error");
    } catch (err) {
      setFb(itemsTab.fb, "通信エラーが発生しました", "error");
    }
  }

  itemsTab.addBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    var val = parseInt(itemsTab.itemIdInp.value, 10);
    if (!val || val <= 0) { alert("有効なアイテムID（1以上）を入力してください"); return; }
    setFb(itemsTab.fb, "追加中...", "");
    try {
      var res = await fetch("/api/characters/" + encodeURIComponent(currentCharId) + "/items", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        credentials: "same-origin",
        body: JSON.stringify({ itemId: val }),
      });
      var data = null;
      try { data = await res.json(); } catch (_) {}
      if (!res.ok) {
        var errCode = (data && data.error) || "エラー";
        if (errCode === "item_slots_full") { alert("アイテムスロットが満杯です"); }
        else { setFb(itemsTab.fb, "エラー: " + errCode, "error"); }
        return;
      }
      setFb(itemsTab.fb, "追加しました（スロット " + data.slot + "）", "success");
      doFetchItems();
    } catch (err) {
      setFb(itemsTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // 所持スキル
  // ----------------------------------------------------------------
  async function doFetchSkills() {
    if (!currentCharId) { return; }
    setFb(skillsTab.fb, "読み込み中...", "");
    try {
      var { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(currentCharId) + "/skills");
      if (!response.ok) {
        setFb(skillsTab.fb, "エラー: " + ((data && data.error) || "取得失敗"), "error");
        return;
      }
      setFb(skillsTab.fb, "", "");
      renderSkills(data || []);
    } catch (err) {
      setFb(skillsTab.fb, "通信エラーが発生しました", "error");
    }
  }

  function renderSkills(skills) {
    if (!skills.length) {
      skillsTab.tbody.innerHTML = "<tr><td colspan='3'>（データなし）</td></tr>";
      return;
    }
    skillsTab.tbody.innerHTML = "";
    skills.forEach(function (skill) {
      var tr = mkEl("tr");
      var tdSlot = mkEl("td", "", String(skill.slot));
      var tdId   = mkEl("td", "", String(skill.skillId));
      var tdOp   = mkEl("td");
      var delBtn = mkEl("button", "button danger btn-sm", "削除");
      delBtn.type = "button";
      delBtn.addEventListener("click", function () { doDeleteSkill(skill.slot); });
      tdOp.appendChild(delBtn);
      tr.append(tdSlot, tdId, tdOp);
      skillsTab.tbody.appendChild(tr);
    });
  }

  async function doDeleteSkill(slot) {
    setFb(skillsTab.fb, "削除中...", "");
    try {
      var res = await fetch("/api/characters/" + encodeURIComponent(currentCharId) + "/skills/" + slot, {
        method: "DELETE", credentials: "same-origin"
      });
      if (res.status === 204) {
        setFb(skillsTab.fb, "削除しました（スロット " + slot + "）", "success");
        doFetchSkills();
        return;
      }
      var data = null;
      try { data = await res.json(); } catch (_) {}
      setFb(skillsTab.fb, "エラー: " + ((data && data.error) || "HTTP " + res.status), "error");
    } catch (err) {
      setFb(skillsTab.fb, "通信エラーが発生しました", "error");
    }
  }

  skillsTab.addBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    var val = parseInt(skillsTab.skillIdInp.value, 10);
    if (!val || val <= 0) { alert("有効なスキルID（1以上）を入力してください"); return; }
    setFb(skillsTab.fb, "追加中...", "");
    try {
      var res = await fetch("/api/characters/" + encodeURIComponent(currentCharId) + "/skills", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        credentials: "same-origin",
        body: JSON.stringify({ skillId: val }),
      });
      var data = null;
      try { data = await res.json(); } catch (_) {}
      if (!res.ok) {
        var errCode = (data && data.error) || "エラー";
        if (errCode === "skill_slots_full") { alert("スキルスロットが満杯です（上限 64）"); }
        else { setFb(skillsTab.fb, "エラー: " + errCode, "error"); }
        return;
      }
      setFb(skillsTab.fb, "追加しました（スロット " + data.slot + "）", "success");
      doFetchSkills();
    } catch (err) {
      setFb(skillsTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // アカウント情報
  // ----------------------------------------------------------------
  async function doFetchAccount() {
    if (!currentCharId) { return; }
    setFb(accountTab.fb, "", "");
    try {
      var { response, data } = await fetchJson("/api/characters/" + encodeURIComponent(currentCharId) + "/account");
      if (!response.ok || !data) {
        setFb(accountTab.fb, "エラー: " + ((data && data.error) || "取得失敗"), "error");
        return;
      }
      var sp = accountTab.acctSpans;
      if (sp.accountId) { sp.accountId.textContent = String(data.accountId ?? "-"); }
      if (sp.loginId)   { sp.loginId.textContent = String(data.loginId ?? "-"); }
      if (sp.ip)        { sp.ip.textContent = String(data.ip ?? "-"); }
      if (sp.mac)       { sp.mac.textContent = String(data.mac ?? "-"); }
    } catch (err) {
      setFb(accountTab.fb, "通信エラーが発生しました", "error");
    }
  }

  accountTab.reloadBtn.addEventListener("click", function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    doFetchAccount();
  });

  accountTab.linkSaveBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    var val = parseInt(accountTab.newAcctInp.value, 10);
    if (isNaN(val) || val < 0) { alert("有効なアカウントID（0以上）を入力してください"); return; }
    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/account", { accountId: val });
      if (!response.ok) { setFb(accountTab.fb, "エラー: " + ((data && data.error) || "失敗"), "error"); return; }
      setFb(accountTab.fb, "アカウント紐付けを変更しました（accountId=" + val + "）", "success");
      doFetchAccount();
    } catch (err) {
      setFb(accountTab.fb, "通信エラーが発生しました", "error");
    }
  });

  accountTab.unlinkBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    if (!confirm("アカウント紐付けを解除しますか？")) { return; }
    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/account", { accountId: 0 });
      if (!response.ok) { setFb(accountTab.fb, "エラー: " + ((data && data.error) || "失敗"), "error"); return; }
      setFb(accountTab.fb, "アカウント紐付けを解除しました", "success");
      doFetchAccount();
    } catch (err) {
      setFb(accountTab.fb, "通信エラーが発生しました", "error");
    }
  });

  accountTab.adminSaveBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    var val = parseInt(accountTab.adminLevelInp.value, 10);
    if (isNaN(val) || val < 0 || val > 255) { alert("権限レベルは 0〜255 で入力してください"); return; }
    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/admin", { adminLevel: val });
      if (!response.ok) { setFb(accountTab.fb, "エラー: " + ((data && data.error) || "失敗"), "error"); return; }
      setFb(accountTab.fb, "管理者権限レベルを " + val + " に設定しました", "success");
    } catch (err) {
      setFb(accountTab.fb, "通信エラーが発生しました", "error");
    }
  });

  accountTab.banBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    if (!confirm("このキャラクターのアカウントを BAN しますか？")) { return; }
    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/disabled", { disabled: true });
      if (!response.ok) { setFb(accountTab.fb, "エラー: " + ((data && data.error) || "失敗"), "error"); return; }
      setFb(accountTab.fb, "BAN しました", "success");
      accountTab.disabledStatusSpan.textContent = "BAN中";
    } catch (err) {
      setFb(accountTab.fb, "通信エラーが発生しました", "error");
    }
  });

  accountTab.unbanBtn.addEventListener("click", async function () {
    if (!currentCharId) { alert("先にキャラクターを表示してください"); return; }
    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/disabled", { disabled: false });
      if (!response.ok) { setFb(accountTab.fb, "エラー: " + ((data && data.error) || "失敗"), "error"); return; }
      setFb(accountTab.fb, "BAN を解除しました", "success");
      accountTab.disabledStatusSpan.textContent = "有効";
    } catch (err) {
      setFb(accountTab.fb, "通信エラーが発生しました", "error");
    }
  });

  // ----------------------------------------------------------------
  // 「表示」ボタン
  // ----------------------------------------------------------------
  loadBtn.addEventListener("click", function () {
    var val = parseInt(idInp.value, 10);
    if (!val || val <= 0) {
      setFb(mainFb, "有効な CharID を入力してください", "error");
      return;
    }
    doFetchChar(val);
  });

  // ----------------------------------------------------------------
  // ゲーム画面からの charId 受信（グローバルブリッジ）
  // ----------------------------------------------------------------
  window._charEditMount = function (charId) {
    var n = Number(charId);
    if (n > 0) {
      idInp.value = String(n);
      doFetchChar(n);
    }
  };

  // キャラ一覧から遷移してきた場合に pendingCharId を消費して詳細を表示する
  var pending = Number(window._charEditPendingCharId);
  window._charEditPendingCharId = null;
  if (pending > 0) {
    idInp.value = String(pending);
    doFetchChar(pending);
  }

  _destroyFn = function () {
    window._charEditMount = null;
    container.innerHTML = "";
  };
}

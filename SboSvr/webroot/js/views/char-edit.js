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
 *   PUT  /api/characters/{charId}/movement
 *   GET/POST/DELETE /api/characters/{charId}/items[/{slot}]
 *   GET/POST/DELETE /api/characters/{charId}/skills[/{slot}]
 *   GET  /api/characters/{charId}/account
 *   PUT  /api/characters/{charId}/account
 *   PUT  /api/characters/{charId}/admin
 *   PUT  /api/characters/{charId}/disabled
 */

import { fetchJson } from "../core/api.js";
import { createSpriteThumb } from "../components/sprite-thumb.js";
import { loadCatalog, calcSpriteCoord, loadSheetImage } from "../data/assets.js";

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

function createNumberValueField(labelText, onChange) {
  var nf = mkNumField(labelText);
  nf.inp.min = "0";
  nf.inp.addEventListener("change", function () {
    onChange?.();
  });
  return {
    el: nf.wrap,
    getValue: function () {
      var v = parseInt(nf.inp.value, 10);
      return Number.isFinite(v) && v >= 0 ? v : 0;
    },
    setValue: function (value) {
      nf.inp.value = String(value || 0);
    },
  };
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

  var form = mkEl("form", "edit-form char-graphics-form");
  form.id = "ce-graphics-form";
  panel.appendChild(form);

  var previewSection = mkEl("section", "char-graphic-preview-panel");
  var previewCanvas = document.createElement("canvas");
  previewCanvas.width = 128;
  previewCanvas.height = 128;
  previewSection.appendChild(previewCanvas);
  form.appendChild(previewSection);

  var grid = mkEl("div", "char-graphic-grid");
  form.appendChild(grid);

  var fields = {};
  var graphicsContext = { is2x2: false, familyId: 0, sex: 0, baseGrpIdSubDown: 2 };
  var catalogCache = null;
  var motionTypeIdRef = null; // openNpcMotionPicker が motionTypeId input を更新するための参照

  function raw(key) {
    var field = fields[key];
    return field ? field.getValue() : 0;
  }

  function is2x2() {
    return !!graphicsContext.is2x2;
  }

  async function getCategory(key) {
    if (!catalogCache) { catalogCache = await loadCatalog(); }
    return catalogCache.find(function (c) { return c.key === key; }) || null;
  }

  function clampValue(v) {
    v = Number(v) || 0;
    return v < 0 ? 0 : v;
  }

  function sheetIndexForRowValue(value) {
    return Math.floor(clampValue(value) / 32);
  }

  function rowForValue(value) {
    return clampValue(value) % 32;
  }

  function sexOffset(size) {
    return graphicsContext.sex === 2 ? size * 16 : 0;
  }

  function downX(size) {
    return sexOffset(size) + size * 4;
  }

  function frontEyeX(size) {
    return sexOffset(size) + size;
  }

  function twoByTwoDownCoord(category) {
    var sub = Math.max(0, (Number(graphicsContext.baseGrpIdSubDown) || 1) - 1);
    return calcSpriteCoord(category, sub);
  }

  function npc2x2FileIndex(value) {
    // クライアントの GetDib2x2NPC(wNPCID) は wNPCID をそのまま配列インデックスとして使い、
    // i = wNPCID で読み込んだ IDP_2X2_NPC_%03d (i+1) を返す。
    // つまり DB の wGrpIDNPC = N → サーバの sheetIndex = N → IDP_2X2_NPC_(N+1).png となる。
    // ここで割り算/スロット分割をしてはいけない (1ファイル=1NPC)。
    return clampValue(value);
  }

  async function drawFromSheet(ctx, categoryKey, sheetIndex, sx, sy, sw, sh, dx, dy, dw, dh) {
    var category = await getCategory(categoryKey);
    if (!category || !category.sheetUrl || sheetIndex < 0) { return; }
    var img = await loadSheetImage(category.sheetUrl + "/" + sheetIndex);
    ctx.imageSmoothingEnabled = false;
    ctx.drawImage(img, sx, sy, sw, sh, dx, dy, dw, dh);
  }

  async function drawNormalPart(ctx, kind, value, dx, dy, scale, refs) {
    refs = refs || {};
    var size = 16;
    var dst = size * scale;
    if (kind === "cloth") {
      await drawFromSheet(ctx, "cloth", sheetIndexForRowValue(value), downX(size), rowForValue(value) * size, size, size, dx, dy, dst, dst);
    } else if (kind === "sp") {
      var sp = clampValue(value);
      if (sp > 0) { await drawFromSheet(ctx, "spCloth", 0, size * 4, (sp - 1) * size, size, size, dx, dy, dst, dst); }
    } else if (kind === "npc") {
      var npc = clampValue(value);
      if (npc >= 50000) { npc -= 50000; }
      if (npc > 0) { await drawFromSheet(ctx, "npcRow", 0, size * 4, npc * size, size, size, dx, dy, dst, dst); }
    } else if (kind === "hairDown") {
      if (clampValue(value) <= 0) { return; }
      await drawFromSheet(ctx, "hairDown", Math.max(0, raw(refs.hairColorKey || "hairColor") - 1), downX(size), clampValue(value) * size, size, size, dx, dy, dst, dst);
    } else if (kind === "hairColor") {
      var hairType = raw(refs.hairTypeKey || "hairType");
      if (hairType <= 0) { return; }
      await drawFromSheet(ctx, "hairDown", Math.max(0, clampValue(value) - 1), downX(size), hairType * size, size, size, dx, dy, dst, dst);
    } else if (kind === "hairUp") {
      if (clampValue(value) <= 0) { return; }
      await drawFromSheet(ctx, "hairUp", Math.max(0, raw(refs.hairColorKey || "hairColor") - 1), downX(size), clampValue(value) * size, size, size, dx, dy, dst, dst);
    } else if (kind === "eye") {
      await drawFromSheet(ctx, "eye", Math.max(0, raw(refs.eyeColorKey || "eyeColor") - 1), frontEyeX(size), clampValue(value) * size, size, size, dx, dy, dst, dst);
    } else if (kind === "eyeColor") {
      await drawFromSheet(ctx, "eye", Math.max(0, clampValue(value) - 1), frontEyeX(size), raw(refs.eyeKey || "eye") * size, size, size, dx, dy, dst, dst);
    } else if (kind === "acce") {
      await drawFromSheet(ctx, "acce", sheetIndexForRowValue(value), downX(size), rowForValue(value) * size, size, size, dx, dy, dst, dst);
    }
  }

  async function draw2x2Part(ctx, kind, value, dx, dy, scale) {
    var size = 32;
    var dst = size * scale;
    var categoryKey = "";
    var sheetIndex = 0;
    if (kind === "body") {
      categoryKey = "char2x2";
      sheetIndex = 0;
    } else if (kind === "cloth") {
      categoryKey = "cloth2x2";
      sheetIndex = clampValue(value);
    } else if (kind === "sp") {
      if (clampValue(value) <= 0) { return; }
      categoryKey = "spCloth2x2";
      sheetIndex = Math.max(0, clampValue(value) - 1);
    } else if (kind === "npc") {
      // value=0 (= NPC未設定) は NPC欄サムネを空欄にするためスキップ。
      // 新しい NPC ピッカーは自前で drawImage を呼ぶのでここを通らない。
      if (clampValue(value) <= 0) { return; }
      categoryKey = "npc2x2";
      sheetIndex = npc2x2FileIndex(value >= 50000 ? value - 50000 : value);
    } else if (kind === "hair") {
      if (clampValue(value) <= 0) { return; }
      if (value >= 10001) {
        categoryKey = "spHair2x2";
        sheetIndex = Math.max(0, clampValue(value) - 10001);
      } else {
        categoryKey = "hair2x2";
        sheetIndex = Math.max(0, clampValue(value) - 1);
      }
    } else if (kind === "eye") {
      categoryKey = "eye2x2";
      sheetIndex = 0;
    }
    var category = await getCategory(categoryKey);
    if (!category || !category.sheetUrl) { return; }
    var coord = kind === "eye" ? calcSpriteCoord(category, 0) : twoByTwoDownCoord(category);
    var img = await loadSheetImage(category.sheetUrl + "/" + sheetIndex);
    var sy = coord.y + ((kind !== "eye" && graphicsContext.sex === 2) ? size * 4 : 0);
    var sx = coord.x;
    if (sx + size > img.naturalWidth || sy + size > img.naturalHeight) { return; }
    ctx.imageSmoothingEnabled = false;
    ctx.drawImage(img, sx, sy, size, size, dx, dy, dst, dst);
  }

  function sampleKindFor2x2(kind) {
    if (kind === "hairDown" || kind === "hairColor") { return "hair"; }
    if (kind === "eyeColor") { return "eye"; }
    return kind;
  }

  async function drawPartSample(ctx, field, value, canvasSize) {
    ctx.clearRect(0, 0, canvasSize, canvasSize);
    var size = is2x2() ? 32 : 16;
    var scale = Math.max(1, Math.floor(canvasSize / size));
    var dst = size * scale;
    var dx = Math.floor((canvasSize - dst) / 2);
    var dy = Math.floor((canvasSize - dst) / 2);
    if (is2x2()) {
      await draw2x2Part(ctx, sampleKindFor2x2(field.kind), value, dx, dy, scale);
    } else {
      await drawNormalPart(ctx, field.kind, value, dx, dy, scale, field.sampleRefs);
    }
  }

  async function renderPreview() {
    var ctx = previewCanvas.getContext("2d");
    ctx.clearRect(0, 0, previewCanvas.width, previewCanvas.height);
    ctx.imageSmoothingEnabled = false;
    var size = is2x2() ? 32 : 16;
    var scale = is2x2() ? 3 : 5;
    var dst = size * scale;
    var dx = Math.floor((previewCanvas.width - dst) / 2);
    var dy = Math.floor((previewCanvas.height - dst) / 2);
    if (is2x2()) {
      if (raw("npc") > 0) {
        await draw2x2Part(ctx, "npc", raw("npc"), dx, dy, scale);
        return;
      }
      await draw2x2Part(ctx, "body", 0, dx, dy, scale);
      await draw2x2Part(ctx, raw("sp") > 0 ? "sp" : "cloth", raw("sp") > 0 ? raw("sp") : raw("cloth"), dx, dy, scale);
      await draw2x2Part(ctx, "hair", raw("hairType"), dx, dy, scale);
      await draw2x2Part(ctx, "eye", raw("eye"), dx, dy, scale);
    } else {
      if (raw("npc") > 0) {
        await drawNormalPart(ctx, "npc", raw("npc"), dx, dy, scale);
        return;
      }
      await drawNormalPart(ctx, raw("sp") > 0 ? "sp" : "cloth", raw("sp") > 0 ? raw("sp") : raw("cloth"), dx, dy, scale);
      await drawNormalPart(ctx, "hairDown", raw("hairType"), dx, dy, scale);
      await drawNormalPart(ctx, "hairUp", raw("hairType"), dx, dy, scale);
      await drawNormalPart(ctx, "eye", raw("eye"), dx, dy, scale);
      if (raw("acce") > 0) { await drawNormalPart(ctx, "acce", raw("acce"), dx, dy, scale); }
    }
  }

  function redrawGraphics() {
    Object.keys(fields).forEach(function (key) {
      if (fields[key].redraw) { fields[key].redraw(); }
    });
    renderPreview();
  }

  function candidateValues(field) {
    var values = [];
    var max = field.max || 31;
    var min = field.min || 0;
    if (typeof field.getMax === "function") { max = field.getMax(); }
    for (var i = min; i <= max; i++) { values.push(i); }
    return values;
  }

  async function loadNpc2x2CandidateValues() {
    var category = await getCategory("npc2x2");
    if (!category || !category.sheetUrl) { return [0]; }
    var coord = twoByTwoDownCoord(category);
    var sy = coord.y + (graphicsContext.sex === 2 ? category.cellSize * 4 : 0);
    var values = [0];
    var misses = 0;
    // 1ファイル=1NPC。sheetIndex N → IDP_2X2_NPC_(N+1).png に対応するため、
    // 値 N を sheetIndex として直接プローブする。
    for (var fileIndex = 0; fileIndex <= 255; fileIndex++) {
      try {
        var img = await loadSheetImage(category.sheetUrl + "/" + fileIndex);
        var sx = coord.x;
        if (sx + category.cellSize <= img.naturalWidth && sy + category.cellSize <= img.naturalHeight) {
          if (fileIndex === 0) {
            // 既に [0] で初期化済みなので push しない (重複防止)。
            // fileIndex=0 (= IDP_2X2_NPC_001) のサムネイルは draw2x2Part で描画される。
          } else {
            values.push(fileIndex);
          }
        }
        misses = 0;
      } catch (_err) {
        misses++;
        if (misses >= 4) { break; }
      }
    }
    return values;
  }

  async function candidateValuesForField(field, mode) {
    if (field.kind === "npc" && is2x2()) { return loadNpc2x2CandidateValues(); }
    if (!(field.hairModeSwitch && is2x2())) { return candidateValues(field); }
    var values = [0];
    var max = field.getMax();
    for (var i = 1; i <= max; i++) {
      values.push(mode === "special" ? 10000 + i : i);
    }
    return values;
  }

  function openPartPicker(field) {
    var backdrop = mkEl("div", "cg-backdrop");
    var dialog = mkEl("div", "cg-dialog");
    var header = mkEl("div", "cg-header");
    var title = mkEl("h3", "", field.label);
    var idWrap = mkEl("label", "cg-id-input", "ID");
    var idInput = mkInput("number", "form-input");
    idInput.value = String(raw(field.key));
    idInput.min = String(field.min || 0);
    idWrap.appendChild(idInput);
    header.append(title, idWrap);
    var mode = field.hairModeSwitch && raw(field.key) >= 10001 ? "special" : "normal";
    var switcher = null;
    if (field.hairModeSwitch && is2x2()) {
      switcher = mkEl("div", "cg-mode-switch");
      var normalBtn = mkEl("button", "cg-mode-btn selected", "通常");
      var specialBtn = mkEl("button", "cg-mode-btn", "特殊");
      normalBtn.type = "button";
      specialBtn.type = "button";
      switcher.append(normalBtn, specialBtn);
      normalBtn.addEventListener("click", function () { mode = "normal"; renderCandidates(); });
      specialBtn.addEventListener("click", function () { mode = "special"; renderCandidates(); });
    }
    var list = mkEl("div", "cg-candidate-grid");
    var footer = mkEl("div", "cg-footer");
    var closeBtn = mkEl("button", "button", "閉じる");
    closeBtn.type = "button";
    footer.appendChild(closeBtn);
    dialog.append(header);
    if (switcher) { dialog.appendChild(switcher); }
    dialog.append(list, footer);
    backdrop.appendChild(dialog);
    document.body.appendChild(backdrop);

    function selectValue(value) {
      fields[field.key].setValue(value);
      idInput.value = String(value);
      redrawGraphics();
      Array.from(list.querySelectorAll(".cg-candidate")).forEach(function (btn) {
        btn.classList.toggle("selected", Number(btn.dataset.value) === Number(value));
      });
    }

    async function renderCandidates() {
      list.innerHTML = "";
      if (switcher) {
        switcher.querySelectorAll(".cg-mode-btn").forEach(function (btn) {
          btn.classList.toggle("selected", btn.textContent === (mode === "special" ? "特殊" : "通常"));
        });
      }
      var values = await candidateValuesForField(field, mode);
      values.forEach(function (value) {
        var btn = mkEl("button", "cg-candidate");
        btn.type = "button";
        btn.dataset.value = String(value);
        if (value === raw(field.key)) { btn.classList.add("selected"); }
        var canvas = document.createElement("canvas");
        var canvasSize = is2x2() ? 128 : 64;
        canvas.width = canvasSize;
        canvas.height = canvasSize;
        var id = mkEl("span", "", String(value));
        btn.append(canvas, id);
        list.appendChild(btn);
        drawPartSample(canvas.getContext("2d"), field, value, canvasSize);
        btn.addEventListener("click", function () { selectValue(value); });
      });
    }

    renderCandidates();

    idInput.addEventListener("change", function () {
      var v = parseInt(idInput.value, 10);
      if (Number.isFinite(v)) { selectValue(Math.max(0, v)); }
    });
    closeBtn.addEventListener("click", function () { backdrop.remove(); });
    backdrop.addEventListener("click", function (ev) {
      if (ev.target === backdrop) { backdrop.remove(); }
    });
  }

  // NPC 専用ピッカー: GrpIDNPC と MotionTypeID を同時に設定する
  // is2x2() が true で key="npc" の場合のみ呼ばれる
  async function openNpcMotionPicker(field) {
    var backdrop = mkEl("div", "cg-backdrop");
    var dialog = mkEl("div", "cg-dialog");
    var header = mkEl("div", "cg-header");
    var title = mkEl("h3", "", field.label + " (NPC モーション選択)");
    header.appendChild(title);
    var list = mkEl("div", "cg-candidate-grid");
    var footer = mkEl("div", "cg-footer");
    var closeBtn = mkEl("button", "button", "閉じる");
    closeBtn.type = "button";
    footer.appendChild(closeBtn);
    dialog.append(header, list, footer);
    backdrop.appendChild(dialog);
    document.body.appendChild(backdrop);

    // ローディング表示
    list.textContent = "読み込み中...";
    var motionTypes, fileCount;
    try {
      var resp = await fetch("/api/npc-motion-pairs");
      if (!resp.ok) { throw new Error("HTTP " + resp.status); }
      var data = await resp.json();
      motionTypes = data.motionTypes || [];
      fileCount = Number(data.fileCount) || 0;
    } catch (err) {
      list.textContent = "取得失敗: " + err.message;
      closeBtn.addEventListener("click", function () { backdrop.remove(); });
      backdrop.addEventListener("click", function (ev) { if (ev.target === backdrop) { backdrop.remove(); } });
      return;
    }

    var category = await getCategory("npc2x2");
    if (!category || !category.sheetUrl) {
      list.textContent = "スプライトカテゴリが見つかりません";
      closeBtn.addEventListener("click", function () { backdrop.remove(); });
      backdrop.addEventListener("click", function (ev) { if (ev.target === backdrop) { backdrop.remove(); } });
      return;
    }
    var cellSize = category.cellSize || 32;
    var sexOffsetY = graphicsContext.sex === 2 ? cellSize * 4 : 0;

    // ファイル F に表示する motion は sys_motion_type.GrpIDSub == F のものだけ。
    // クライアントが用意した「このモーション種別はこの NPC ファイル用」というメタ情報を
    // 使うことで、(file × motion) のクロスから「実際に絵が入っている組合せ」だけを残せる。
    // セル中身チェック (cellHasContent) は念のためのサニティ用。
    var currentGrpIdNpc = raw("npc");
    list.textContent = "";

    function cellHasContent(img, sx, sy) {
      // セルの代表点 (4×4 サンプル) のうち 1 点でも不透明があれば「中身あり」と判定。
      // 完全に透明なセルだけ落とせれば十分なので軽量サンプリング。
      var tmp = document.createElement("canvas");
      tmp.width = cellSize; tmp.height = cellSize;
      var c = tmp.getContext("2d");
      c.imageSmoothingEnabled = false;
      try {
        c.drawImage(img, sx, sy, cellSize, cellSize, 0, 0, cellSize, cellSize);
        var pixels = c.getImageData(0, 0, cellSize, cellSize).data;
        for (var py = 2; py < cellSize; py += 4) {
          for (var px = 2; px < cellSize; px += 4) {
            var a = pixels[(py * cellSize + px) * 4 + 3];
            if (a > 0) { return true; }
          }
        }
      } catch (_e) {
        // CORS 等で getImageData 失敗 → 中身ありと仮定 (落とさない)
        return true;
      }
      return false;
    }

    var canvasSize = 128;
    var renderedCount = 0;

    // sub-base (1始まり) → セル左上座標 (sex 補正含む)
    function subToXY(sub) {
      if (!sub) { return null; }
      var coord = calcSpriteCoord(category, Math.max(0, sub - 1));
      return { x: coord.x, y: coord.y + sexOffsetY };
    }

    // motion の定義済み方向 (sub != 0) のセル全てに描画があるかチェック。
    // クライアントの描画パスは StandXxx の sub-base ごとに該当セルを引くため、
    // どこか1つでも空セルがあるとその NPC として成立しない。
    // = ファイル内に「実際に居る NPC」のみが残るフィルタ。
    function motionFitsFile(img, mt) {
      var dirs = [mt.subUp, mt.subDown, mt.subLeft, mt.subRight];
      var checked = 0;
      for (var i = 0; i < dirs.length; i++) {
        if (!dirs[i]) { continue; }  // この方向は未定義 → スキップ (チェック対象外)
        var p = subToXY(dirs[i]);
        if (!p) { continue; }
        if (p.x + cellSize > img.naturalWidth || p.y + cellSize > img.naturalHeight) { return false; }
        if (!cellHasContent(img, p.x, p.y)) { return false; }
        checked++;
      }
      return checked > 0;  // 1 方向以上有効なら OK
    }

    // ファイル順 → モーション順で並べる
    for (var fIdx = 0; fIdx < fileCount; fIdx++) {
      var img;
      try {
        img = await loadSheetImage(category.sheetUrl + "/" + fIdx);
      } catch (_e) {
        continue; // 読めないファイルはスキップ
      }
      for (var mIdx = 0; mIdx < motionTypes.length; mIdx++) {
        var mt = motionTypes[mIdx];
        // 対象 NPC ファイルが一致するモーション種別だけを表示。
        // (例: motion 6〜30 は GrpIDSub=5 → npc006 用)
        if (Number(mt.targetGrpIdNpc) !== fIdx) { continue; }
        if (!motionFitsFile(img, mt)) { continue; }

        // サムネは STAND_DOWN が有れば優先、無ければ定義済み最初の方向を使う
        var thumbSub = mt.subDown || mt.subUp || mt.subLeft || mt.subRight;
        var thumbXY = subToXY(thumbSub);
        if (!thumbXY) { continue; }

        renderedCount++;

        var entry = { grpIdNpc: fIdx, motionTypeId: mt.motionTypeId, baseGrpIdSubDown: thumbSub };
        var btn = mkEl("button", "cg-candidate");
        btn.type = "button";
        btn.dataset.grpIdNpc = String(entry.grpIdNpc);
        btn.dataset.motionTypeId = String(entry.motionTypeId);

        var canvas = document.createElement("canvas");
        canvas.width = canvasSize;
        canvas.height = canvasSize;
        var scale = Math.max(1, Math.floor(canvasSize / cellSize));
        var dst = cellSize * scale;
        var dx = Math.floor((canvasSize - dst) / 2);
        var dy = Math.floor((canvasSize - dst) / 2);
        var ctx2 = canvas.getContext("2d");
        ctx2.imageSmoothingEnabled = false;
        ctx2.drawImage(img, thumbXY.x, thumbXY.y, cellSize, cellSize, dx, dy, dst, dst);

        var lbl = mkEl("span", "", "npc" + entry.grpIdNpc + "/mtn" + entry.motionTypeId);
        btn.append(canvas, lbl);
        list.appendChild(btn);

        if (entry.grpIdNpc === currentGrpIdNpc) { btn.classList.add("selected"); }

        (function (e, b) {
          b.addEventListener("click", function () {
            // GrpIDNPC・MotionTypeID を同時更新し、プレビュー用 sub-base も差し替え。
            // motionTypeIdRef.dispatchEvent(change) で他リスナにも通知。
            fields[field.key].setValue(e.grpIdNpc);
            if (motionTypeIdRef) {
              motionTypeIdRef.value = String(e.motionTypeId);
              try { motionTypeIdRef.dispatchEvent(new Event("change", { bubbles: true })); } catch (_x) {}
            }
            graphicsContext.baseGrpIdSubDown = e.baseGrpIdSubDown;
            redrawGraphics();
            Array.from(list.querySelectorAll(".cg-candidate")).forEach(function (other) {
              other.classList.toggle("selected", other === b);
            });
          });
        })(entry, btn);
      }
    }

    if (renderedCount === 0) {
      list.textContent = "選択可能な NPC がありません";
    }

    closeBtn.addEventListener("click", function () { backdrop.remove(); });
    backdrop.addEventListener("click", function (ev) {
      if (ev.target === backdrop) { backdrop.remove(); }
    });
  }

  function createGraphicField(field) {
    var wrap = mkEl("div", "char-graphic-field");
    var label = mkEl("span", "char-graphic-label", field.label);
    var canvas = document.createElement("canvas");
    canvas.width = 48;
    canvas.height = 48;
    canvas.tabIndex = 0;
    canvas.setAttribute("role", "button");
    canvas.setAttribute("aria-label", field.label + "を選択");
    var input = mkInput("number", "form-input");
    input.min = String(field.min || 0);
    wrap.append(label, canvas, input);
    input.addEventListener("change", redrawGraphics);

    function openPicker() {
      if (field.key === "npc" && is2x2()) {
        openNpcMotionPicker(field);
      } else {
        openPartPicker(field);
      }
    }
    canvas.addEventListener("click", openPicker);
    canvas.addEventListener("keydown", function (ev) {
      if (ev.key === "Enter" || ev.key === " ") {
        ev.preventDefault();
        openPicker();
      }
    });
    return {
      el: wrap,
      getValue: function () { return clampValue(parseInt(input.value, 10)); },
      setValue: function (value) { input.value = String(value || 0); },
      redraw: function () { drawPartSample(canvas.getContext("2d"), field, this.getValue(), 48); },
    };
  }

  var graphicFields = [
    { key: "npc", label: "NPC", kind: "npc", min: 0, getMax: function () { return is2x2() ? 5 : 64; } },
    { key: "cloth", label: "服", kind: "cloth", min: 0, getMax: function () { return is2x2() ? 4 : 63; } },
    { key: "eye", label: "目", kind: "eye", min: 0, getMax: function () { return is2x2() ? 3 : 31; } },
    { key: "eyeColor", label: "目色", kind: "eyeColor", min: 1, getMax: function () { return is2x2() ? 1 : 6; } },
    { key: "hairType", label: "髪", kind: "hairDown", min: 0, hairModeSwitch: true, getMax: function () { return is2x2() ? 4 : 31; } },
    { key: "hairColor", label: "髪色", kind: "hairColor", min: 1, getMax: function () { return is2x2() ? 1 : 13; } },
    { key: "sp", label: "特殊服", kind: "sp", min: 0, getMax: function () { return is2x2() ? 2 : 3; } },
    { key: "tmpMain", label: "一時服(メイン)", number: true },
    { key: "tmpSub", label: "一時服(サブ)", number: true },
    { key: "acce", label: "アクセサリ", kind: "acce", min: 0, getMax: function () { return 63; } },
    { key: "armsMain", label: "持ち物(メイン)", number: true },
    { key: "armsSub", label: "持ち物(サブ)", number: true },
    { key: "armsLeftMain", label: "盾(メイン)", number: true },
    { key: "armsLeftSub", label: "盾(サブ)", number: true },
    { key: "initNpc", label: "初期NPC", kind: "npc", min: 0, getMax: function () { return is2x2() ? 5 : 64; } },
    { key: "initCloth", label: "初期服", kind: "cloth", min: 0, getMax: function () { return is2x2() ? 4 : 63; } },
    { key: "initEye", label: "初期目", kind: "eye", min: 0, sampleRefs: { eyeColorKey: "initEyeColor" }, getMax: function () { return is2x2() ? 3 : 31; } },
    { key: "initEyeColor", label: "初期目色", kind: "eyeColor", min: 1, sampleRefs: { eyeKey: "initEye" }, getMax: function () { return is2x2() ? 1 : 6; } },
    { key: "initHairType", label: "初期髪", kind: "hairDown", min: 0, hairModeSwitch: true, sampleRefs: { hairColorKey: "initHairColor" }, getMax: function () { return is2x2() ? 4 : 31; } },
    { key: "initHairColor", label: "初期髪色", kind: "hairColor", min: 1, sampleRefs: { hairTypeKey: "initHairType" }, getMax: function () { return is2x2() ? 1 : 13; } },
    { key: "initSp", label: "初期特殊服", kind: "sp", min: 0, getMax: function () { return is2x2() ? 2 : 3; } },
  ];

  graphicFields.forEach(function (f) {
    if (f.number) {
      var nf = createNumberValueField(f.label, redrawGraphics);
      grid.appendChild(nf.el);
      fields[f.key] = nf;
      return;
    }
    var sf = createGraphicField(f);
    grid.appendChild(sf.el);
    fields[f.key] = sf;
  });

  var actions = mkEl("div", "form-actions");
  var saveBtn = mkEl("button", "button primary", "保存");
  saveBtn.type = "submit";
  actions.appendChild(saveBtn);
  form.appendChild(actions);

  function setGraphicsContext(next, rootData) {
    graphicsContext.is2x2 = !!(next && next.is2x2);
    graphicsContext.familyId = rootData ? (rootData.familyId || 0) : graphicsContext.familyId;
    graphicsContext.sex = rootData ? (rootData.sex || 0) : graphicsContext.sex;
    graphicsContext.baseGrpIdSubDown = next ? (next.baseGrpIdSubDown || next.baseGrpIdSub || 2) : graphicsContext.baseGrpIdSubDown;
    redrawGraphics();
  }

  function setMotionTypeIdRef(inp) {
    motionTypeIdRef = inp;
  }

  return { panel, fb, form, spriteFields: fields, syncGraphicsThumbs: redrawGraphics, setGraphicsContext, setMotionTypeIdRef };
}

// ----------------------------------------------------------------
// タブパネル: 移動
// ----------------------------------------------------------------

function buildMovementTab() {
  var panel = mkEl("div", "tab-panel");
  panel.id = "ce-tab-movement";
  panel.style.display = "none";

  var fb = mkEl("p", "form-feedback");
  fb.setAttribute("aria-live", "polite");
  panel.appendChild(fb);

  var form = mkEl("form", "edit-form");
  form.id = "ce-movement-form";
  panel.appendChild(form);

  var grid = mkEl("div", "form-grid");
  form.appendChild(grid);

  var inputs = {};
  var mvFields = [
    ["maxItemCount","最大アイテム所持数"],["dropItemAverage","ドロップ率"],
    ["moveAverage","移動確率"],["moveAverageBattle","戦闘時移動確率"],
    ["moveWait","移動待ち(ms)"],["moveWaitBattle","戦闘時移動待ち(ms)"],
    ["searchDistanceCX","索敵範囲CX"],["searchDistanceCY","索敵範囲CY"],
    ["motionTypeId","モーション種別ID"],
  ];
  mvFields.forEach(function (pair) {
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
  // NPC ピッカーで motionTypeId input を更新できるよう参照を渡す
  graphicsTab.setMotionTypeIdRef(basicTab.motionTypeIdInp);
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
      graphicsTab.setGraphicsContext(g, d);
    }

    // 移動
    if (d.movement) {
      var mv = d.movement;
      Object.keys(movementTab.inputs).forEach(function (k) { setNumInp(movementTab.inputs[k], mv[k]); });
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
  // 移動 保存
  // ----------------------------------------------------------------
  movementTab.form.addEventListener("submit", async function (ev) {
    ev.preventDefault();
    if (!currentCharId) { setFb(movementTab.fb, "先にキャラクターを表示してください", "error"); return; }
    setFb(movementTab.fb, "保存中...", "");

    var body = {};
    Object.keys(movementTab.inputs).forEach(function (k) {
      var v = numVal(movementTab.inputs[k]);
      if (v !== null) { body[k] = v; }
    });

    try {
      var { response, data } = await putJson("/api/characters/" + currentCharId + "/movement", body);
      if (!response.ok) {
        var msg = (data && data.error) ? data.error : "保存に失敗しました";
        setFb(movementTab.fb, "エラー: " + msg, "error");
        return;
      }
      setFb(movementTab.fb, "保存しました", "success");
      await doFetchChar(currentCharId);
    } catch (err) {
      console.error("char-edit movement save error", err);
      setFb(movementTab.fb, "通信エラーが発生しました", "error");
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

      // アイテム列: アイコン + [ID] 名前 を横並び (td 自体は table-cell のまま、
      // 内側 div を flex にして他列の vertical-align: middle と整合させる)
      var tdItem = mkEl("td");
      var itemWrap = mkEl("div");
      itemWrap.style.display = "flex";
      itemWrap.style.alignItems = "center";
      itemWrap.style.gap = "6px";
      if (info && info.iconGrpId) {
        var thumb = createSpriteThumb({ categoryKey: "item", sub: info.iconGrpId, size: 20 });
        itemWrap.appendChild(thumb.el);
      }
      var itemName = (info && info.name) ? info.name : "(名前なし)";
      var spanLabel = mkEl("span", "", "[" + String(item.itemId) + "] " + itemName);
      itemWrap.appendChild(spanLabel);
      tdItem.appendChild(itemWrap);

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

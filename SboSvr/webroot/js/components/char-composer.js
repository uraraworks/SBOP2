/**
 * components/char-composer.js
 * 2x2 キャラの参照レイヤー合成プレビュー (S3b)
 *
 * 画像エディタで服・髪・目などの単一レイヤーを編集していると、実際に
 * キャラとして重なった時の見た目が分からない。このコンポーネントは
 * SboCli の CMgrDraw::DrawChar (MgrDraw.cpp:533-575) と同じ手順で
 * 体 → 服 → 髪 → 目 を canvas に合成し、向き・コマ・性別を切り替えて
 * 確認できるようにする。
 *
 * ゲーム側の合成規則（実データ検証済み）:
 *   - シートは 512x256 = 16列 x 8行 の 32px セル
 *   - フレーム番号 0-3=上 / 4-7=下 / 8-11=左 / 12-15=右
 *   - セル座標は (frame % 16) * 32, floor(frame / 16) * 32
 *   - 女性は体・服・髪・NPC のみ y += 32 * 4
 *   - 目だけ特殊: x -= 32 * 4 で、かつ女性オフセットを足さない
 *   - 目は向きが「上」(drawDirection == 0) の時は描かない
 */

const CELL = 32;

// CInfoCharBase::GetDrawDirection の戻り値。0=上 1=下 2=左 3=右
const DIRECTIONS = [
  { value: 1, label: "下" },
  { value: 0, label: "上" },
  { value: 2, label: "左" },
  { value: 3, label: "右" },
];

// nEyeID の意味（MgrDraw.cpp:445-454）
const EYE_LABELS = ["通常", "ダメージ", "気絶", "睡眠"];

// 服スロットを置き換えるカテゴリ。
// wGrpIDSP / wGrpIDTmpMain が立っている時は服の代わりにこれらが重なる。
const CLOTH_SLOT_KEYS = new Set(["cloth2x2", "spCloth2x2", "spHair2x2"]);

// このコンポーネントを表示するカテゴリ
export const COMPOSABLE_KEYS = new Set([
  "char2x2",
  "cloth2x2",
  "spCloth2x2",
  "hair2x2",
  "spHair2x2",
  "eye2x2",
  "npc2x2",
]);

// ----------------------------------------------------------------
// シート画像のロード（カテゴリ+シート番号+世代 でキャッシュ）
// ----------------------------------------------------------------

const _sheetCache = new Map();

function sheetCacheKey(key, index, generation) {
  return `${key}/${index}/${generation}`;
}

/**
 * スプライトシートを HTMLImageElement として取得する。
 * 取得に失敗したカテゴリ（未登録・シート無し）は null を解決する。
 */
function loadSheet(key, index, generation) {
  const cacheKey = sheetCacheKey(key, index, generation);
  const hit = _sheetCache.get(cacheKey);
  if (hit) return hit;

  const promise = new Promise((resolve) => {
    const img = new Image();
    img.onload = () => resolve(img);
    img.onerror = () => resolve(null);
    // 差し替え直後に古い画像を掴まないよう世代番号でキャッシュを外す
    img.src = `/api/assets/sprites/${encodeURIComponent(key)}/${index}?v=${generation}`;
  });
  _sheetCache.set(cacheKey, promise);
  return promise;
}

// ----------------------------------------------------------------
// 部品
// ----------------------------------------------------------------

function makeSelect(className) {
  const sel = document.createElement("select");
  if (className) sel.className = className;
  return sel;
}

function makeLabeled(labelText, control) {
  const label = document.createElement("label");
  label.className = "cc-field";
  const span = document.createElement("span");
  span.textContent = labelText;
  label.append(span, control);
  return label;
}

function fillSheetOptions(select, count, labeller) {
  const prev = select.value;
  select.innerHTML = "";
  for (let i = 0; i < count; i++) {
    const opt = document.createElement("option");
    opt.value = String(i);
    opt.textContent = labeller ? labeller(i) : `#${i}`;
    select.appendChild(opt);
  }
  if (prev !== "" && Number(prev) < count) {
    select.value = prev;
  }
}

// ----------------------------------------------------------------
// 本体
// ----------------------------------------------------------------

/**
 * @param {object} opts
 * @param {Array}  opts.categories /api/image-categories の categories 配列
 */
export function createCharComposer({ categories }) {
  const catByKey = new Map((categories ?? []).map((c) => [c.key, c]));
  const sheetCountOf = (key) => Number(catByKey.get(key)?.sheetCount ?? 0);

  let _target = null;      // { key, index }
  let _generation = 1;     // 画像キャッシュの世代
  let _sex = 0;            // 0=男 1=女
  let _dir = 1;            // drawDirection (既定は下)
  let _step = 0;           // アニメのコマ 0-3
  let _scale = 4;
  let _timer = null;
  let _drawSeq = 0;        // 非同期描画の競合防止

  const visible = { body: true, cloth: true, hair: true, eye: true };

  const section = document.createElement("section");
  section.className = "detail-section cc-section";

  const h3 = document.createElement("h3");
  h3.textContent = "合成プレビュー";
  section.appendChild(h3);

  const hint = document.createElement("p");
  hint.className = "card-description";
  hint.textContent =
    "ゲームと同じ手順（体 → 服 → 髪 → 目）で重ねた結果です。編集中のレイヤーは固定され、他は下のセレクトで差し替えられます。";
  section.appendChild(hint);

  // --- 操作列 1: 性別 / 向き / コマ / 拡大率 ---
  const toolbar1 = document.createElement("div");
  toolbar1.className = "cc-toolbar";

  const sexSelect = makeSelect();
  [["男", 0], ["女", 1]].forEach(([label, value]) => {
    const opt = document.createElement("option");
    opt.value = String(value);
    opt.textContent = label;
    sexSelect.appendChild(opt);
  });
  sexSelect.addEventListener("change", () => {
    _sex = Number(sexSelect.value) || 0;
    draw();
  });

  const dirSelect = makeSelect();
  DIRECTIONS.forEach((d) => {
    const opt = document.createElement("option");
    opt.value = String(d.value);
    opt.textContent = d.label;
    if (d.value === _dir) opt.selected = true;
    dirSelect.appendChild(opt);
  });
  dirSelect.addEventListener("change", () => {
    _dir = Number(dirSelect.value) || 0;
    draw();
  });

  const stepSelect = makeSelect();
  for (let i = 0; i < 4; i++) {
    const opt = document.createElement("option");
    opt.value = String(i);
    opt.textContent = String(i);
    stepSelect.appendChild(opt);
  }
  stepSelect.addEventListener("change", () => {
    _step = Number(stepSelect.value) || 0;
    draw();
  });

  const playLabel = document.createElement("label");
  playLabel.className = "cc-check";
  const playCb = document.createElement("input");
  playCb.type = "checkbox";
  playCb.addEventListener("change", () => {
    if (playCb.checked) startAnim(); else stopAnim();
  });
  playLabel.append(playCb, " アニメ再生");

  const scaleSelect = makeSelect();
  [2, 4, 8, 12].forEach((s) => {
    const opt = document.createElement("option");
    opt.value = String(s);
    opt.textContent = `${s}x`;
    if (s === _scale) opt.selected = true;
    scaleSelect.appendChild(opt);
  });
  scaleSelect.addEventListener("change", () => {
    _scale = Number(scaleSelect.value) || 4;
    applyCanvasSize();
    draw();
  });

  toolbar1.append(
    makeLabeled("性別", sexSelect),
    makeLabeled("向き", dirSelect),
    makeLabeled("コマ", stepSelect),
    playLabel,
    makeLabeled("拡大率", scaleSelect)
  );
  section.appendChild(toolbar1);

  // --- 操作列 2: 参照レイヤーの差し替え ---
  const toolbar2 = document.createElement("div");
  toolbar2.className = "cc-toolbar";

  const clothSelect = makeSelect();
  clothSelect.addEventListener("change", draw);
  const clothField = makeLabeled("服", clothSelect);

  const hairSelect = makeSelect();
  hairSelect.addEventListener("change", draw);
  const hairField = makeLabeled("髪", hairSelect);

  const eyeSelect = makeSelect();
  eyeSelect.addEventListener("change", draw);
  const eyeField = makeLabeled("目", eyeSelect);

  toolbar2.append(clothField, hairField, eyeField);
  section.appendChild(toolbar2);

  // --- 操作列 3: レイヤー表示切替 ---
  const toolbar3 = document.createElement("div");
  toolbar3.className = "cc-toolbar";
  const layerChecks = {};
  [["body", "体"], ["cloth", "服"], ["hair", "髪"], ["eye", "目"]].forEach(([slot, label]) => {
    const wrap = document.createElement("label");
    wrap.className = "cc-check";
    const cb = document.createElement("input");
    cb.type = "checkbox";
    cb.checked = true;
    cb.addEventListener("change", () => {
      visible[slot] = cb.checked;
      draw();
    });
    wrap.append(cb, " " + label);
    layerChecks[slot] = wrap;
    toolbar3.appendChild(wrap);
  });
  section.appendChild(toolbar3);

  // --- キャンバス ---
  const stage = document.createElement("div");
  stage.className = "ie-preview-stage cc-stage";
  const canvas = document.createElement("canvas");
  canvas.className = "cc-canvas";
  stage.appendChild(canvas);
  section.appendChild(stage);

  const note = document.createElement("p");
  note.className = "ie-dim-info";
  section.appendChild(note);

  function applyCanvasSize() {
    canvas.width = CELL;
    canvas.height = CELL;
    canvas.style.width = `${CELL * _scale}px`;
    canvas.style.height = `${CELL * _scale}px`;
  }
  applyCanvasSize();

  function startAnim() {
    stopAnim();
    _timer = setInterval(() => {
      // ルーターに unmount フックが無いため、DOM から外れたら自分で止まる
      if (!section.isConnected) { stopAnim(); return; }
      _step = (_step + 1) % 4;
      stepSelect.value = String(_step);
      draw();
    }, 200);
  }

  function stopAnim() {
    if (_timer) { clearInterval(_timer); _timer = null; }
  }

  // 編集中カテゴリに応じて各スロットが参照するシートを決める
  function resolveSlots() {
    if (!_target) return null;

    if (_target.key === "npc2x2") {
      return {
        mode: "npc",
        npc: { key: "npc2x2", index: _target.index },
      };
    }

    return {
      mode: "char",
      body: { key: "char2x2", index: 0 },
      cloth: CLOTH_SLOT_KEYS.has(_target.key)
        ? { key: _target.key, index: _target.index }
        : { key: "cloth2x2", index: Number(clothSelect.value) || 0 },
      hair: _target.key === "hair2x2"
        ? { key: "hair2x2", index: _target.index }
        : { key: "hair2x2", index: Number(hairSelect.value) || 0 },
      eye: _target.key === "eye2x2"
        ? { key: "eye2x2", index: _target.index }
        : { key: "eye2x2", index: Number(eyeSelect.value) || 0 },
    };
  }

  // シート上のセル矩形を求める。範囲外なら null。
  function cellRect(img, frame, opts) {
    const femaleOffset = opts?.applyFemaleOffset === false ? 0 : (_sex === 1 ? CELL * 4 : 0);
    const sx = (frame % 16) * CELL + (opts?.offsetX ?? 0);
    const sy = Math.floor(frame / 16) * CELL + femaleOffset;
    if (sx < 0 || sy < 0) return null;
    if (sx + CELL > img.naturalWidth || sy + CELL > img.naturalHeight) return null;
    return { sx, sy };
  }

  async function draw() {
    const seq = ++_drawSeq;
    const ctx = canvas.getContext("2d");
    ctx.imageSmoothingEnabled = false;
    ctx.clearRect(0, 0, CELL, CELL);

    const slots = resolveSlots();
    if (!slots) { note.textContent = ""; return; }

    const frame = _dir * 4 + _step;
    const missing = [];

    // 描画順は MgrDraw.cpp と同じ。目だけ最後・特殊オフセット。
    const plan = slots.mode === "npc"
      ? [{ slot: "body", ref: slots.npc, label: "NPC" }]
      : [
          { slot: "body",  ref: slots.body,  label: "体" },
          { slot: "cloth", ref: slots.cloth, label: "服" },
          { slot: "hair",  ref: slots.hair,  label: "髪" },
          { slot: "eye",   ref: slots.eye,   label: "目", offsetX: -CELL * 4, applyFemaleOffset: false },
        ];

    for (const step of plan) {
      if (!visible[step.slot]) continue;
      // 目は後ろ向き（上）では描かれない
      if (step.slot === "eye" && _dir === 0) continue;
      if (!step.ref || sheetCountOf(step.ref.key) <= 0) { missing.push(step.label); continue; }

      const img = await loadSheet(step.ref.key, step.ref.index, _generation);
      if (seq !== _drawSeq) return;  // 途中で操作されたら破棄
      if (!img) { missing.push(step.label); continue; }

      const rect = cellRect(img, frame, step);
      if (!rect) { missing.push(step.label); continue; }
      ctx.drawImage(img, rect.sx, rect.sy, CELL, CELL, 0, 0, CELL, CELL);
    }

    if (seq !== _drawSeq) return;
    const dirLabel = DIRECTIONS.find((d) => d.value === _dir)?.label ?? "?";
    let text = `フレーム ${frame} (${dirLabel} / コマ ${_step})`;
    if (slots.mode === "char" && _dir === 0) {
      text += " ・ 上向きは目を描画しません";
    }
    if (missing.length) {
      text += ` ・ 取得できなかったレイヤー: ${missing.join(", ")}`;
    }
    note.textContent = text;
  }

  /**
   * 編集対象を切り替える。表示対象外のカテゴリなら false を返す。
   */
  function setTarget(cat, index) {
    _target = null;
    stopAnim();
    playCb.checked = false;

    if (!cat || !COMPOSABLE_KEYS.has(cat.key)) {
      section.style.display = "none";
      return false;
    }
    section.style.display = "";
    _target = { key: cat.key, index };

    const isNpc = cat.key === "npc2x2";

    // 目は上向きでは描かれない。目を編集し始めた時に向きが「上」のままだと
    // 「編集中のレイヤーだけ見えない」状態になるので、下向きへ戻す。
    if (cat.key === "eye2x2" && _dir === 0) {
      _dir = 1;
      dirSelect.value = String(_dir);
    }

    // 参照レイヤーのセレクトは「編集中でないレイヤー」だけ有効にする
    fillSheetOptions(clothSelect, sheetCountOf("cloth2x2"));
    fillSheetOptions(hairSelect, sheetCountOf("hair2x2"));
    fillSheetOptions(eyeSelect, sheetCountOf("eye2x2"), (i) => (EYE_LABELS[i] ? `#${i} ${EYE_LABELS[i]}` : `#${i}`));

    clothField.style.display = isNpc || CLOTH_SLOT_KEYS.has(cat.key) ? "none" : "";
    hairField.style.display = isNpc || cat.key === "hair2x2" ? "none" : "";
    eyeField.style.display = isNpc || cat.key === "eye2x2" ? "none" : "";

    layerChecks.cloth.style.display = isNpc ? "none" : "";
    layerChecks.hair.style.display = isNpc ? "none" : "";
    layerChecks.eye.style.display = isNpc ? "none" : "";

    hint.textContent = isNpc
      ? "NPC は 1 枚のシートで完結するため、向き・コマ・性別だけを切り替えて確認できます。"
      : "ゲームと同じ手順（体 → 服 → 髪 → 目）で重ねた結果です。編集中のレイヤーは固定され、他は下のセレクトで差し替えられます。";

    draw();
    return true;
  }

  // 差し替え後に呼ぶ。画像キャッシュの世代を上げて読み直す。
  // 直後に setTarget を呼ぶ場合は redraw:false で二重描画を避ける。
  function invalidate({ redraw = true } = {}) {
    _generation++;
    _sheetCache.clear();
    if (redraw) draw();
  }

  function destroy() {
    stopAnim();
  }

  return { el: section, setTarget, invalidate, destroy };
}

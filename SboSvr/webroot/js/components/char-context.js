/**
 * components/char-context.js
 * 「性別 / 服 / 髪 / 目」の合成の見え方を、合成プレビュー(char-composer.js)と
 * ペイントの下敷き(sprite-paint.js)の間で共有するための小さなストア。
 *
 * 元々は両コンポーネントがそれぞれ独自にセレクトを持っていて、片方を変えても
 * もう片方に反映されなかった（同じ「どのキャラとして見るか」の指定なのに
 * 別々に持っていたため）。このストアを 1 箇所だけ作って両方へ渡すことで、
 * どちらの UI から変更しても同じ値を参照するようにする。
 */

import { EYE_LABELS } from "./char-composer.js";

function makeSelect() {
  return document.createElement("select");
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

/**
 * @param {object} opts
 * @param {Array}  opts.categories /api/image-categories の categories 配列
 */
export function createCharContext({ categories }) {
  const catByKey = new Map((categories ?? []).map((c) => [c.key, c]));
  const sheetCountOf = (key) => Number(catByKey.get(key)?.sheetCount ?? 0);

  let sex = 0;    // 0=男 1=女
  let cloth = 0;  // cloth2x2 のシート番号
  let hair = 0;   // hair2x2 のシート番号
  let eye = 0;    // eye2x2 のシート番号

  const listeners = new Set();
  function notify() { listeners.forEach((fn) => fn()); }

  const el = document.createElement("div");
  el.className = "ie-charctx";
  el.hidden = true;

  const title = document.createElement("p");
  title.className = "ie-charctx-title";
  title.textContent = "合成の見え方";
  el.appendChild(title);

  const toolbar = document.createElement("div");
  toolbar.className = "cc-toolbar";
  el.appendChild(toolbar);

  const sexSelect = makeSelect();
  [["男", 0], ["女", 1]].forEach(([label, value]) => {
    const opt = document.createElement("option");
    opt.value = String(value);
    opt.textContent = label;
    sexSelect.appendChild(opt);
  });
  sexSelect.addEventListener("change", () => {
    sex = Number(sexSelect.value) || 0;
    notify();
  });

  const clothSelect = makeSelect();
  clothSelect.addEventListener("change", () => {
    cloth = Number(clothSelect.value) || 0;
    notify();
  });

  const hairSelect = makeSelect();
  hairSelect.addEventListener("change", () => {
    hair = Number(hairSelect.value) || 0;
    notify();
  });

  const eyeSelect = makeSelect();
  eyeSelect.addEventListener("change", () => {
    eye = Number(eyeSelect.value) || 0;
    notify();
  });

  fillSheetOptions(clothSelect, sheetCountOf("cloth2x2"));
  fillSheetOptions(hairSelect, sheetCountOf("hair2x2"));
  fillSheetOptions(eyeSelect, sheetCountOf("eye2x2"),
    (i) => (EYE_LABELS[i] ? `#${i} ${EYE_LABELS[i]}` : `#${i}`));

  toolbar.append(
    makeLabeled("性別", sexSelect),
    makeLabeled("服", clothSelect),
    makeLabeled("髪", hairSelect),
    makeLabeled("目", eyeSelect)
  );

  function get() {
    return { sex, cloth, hair, eye };
  }

  function subscribe(fn) {
    listeners.add(fn);
    return () => listeners.delete(fn);
  }

  function setVisible(visible) {
    el.hidden = !visible;
  }

  return { el, get, subscribe, setVisible };
}

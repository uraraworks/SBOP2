/**
 * components/tooltip.js
 * デザインツール風の「1秒ホバーで出る」ツールチップ。
 *
 * CSS の ::after では実装しない: 画像エディタの中央カラム・タブパネルは
 * overflow: auto で囲われているため、擬似要素だと端で切れてしまう。
 * 代わりにツールチップ本体を position: fixed の 1 個の要素にして body 直下へ
 * 置き、表示のたびに対象要素の座標から位置を計算する。
 *
 * document ではなく el.ownerDocument を使う点が重要: この画面は「編集を
 * 別ウィンドウで開く」機能で DOM ごと別ウィンドウへ adoptNode されるため、
 * 親ウィンドウの document.body にツールチップを付けると別ウィンドウ側では
 * 見えなくなる。ownerDocument ごとにシングルトンを持たせて対応する。
 */

const SHOW_DELAY_MS = 1000;

// ownerDocument -> ツールチップ要素 のシングルトン管理
const tipByDoc = new WeakMap();

function getTooltipEl(doc) {
  let tip = tipByDoc.get(doc);
  if (!tip) {
    tip = doc.createElement("div");
    tip.className = "ui-tip";
    doc.body.appendChild(tip);
    tipByDoc.set(doc, tip);
  }
  return tip;
}

function showTooltip(el, text) {
  const doc = el.ownerDocument;
  const tip = getTooltipEl(doc);
  const win = doc.defaultView || window;

  tip.textContent = text;
  tip.classList.add("is-visible");

  const rect = el.getBoundingClientRect();
  const tipRect = tip.getBoundingClientRect();

  // 既定は要素の下。下に収まらなければ上へ回す。
  let top = rect.bottom + 6;
  if (top + tipRect.height > win.innerHeight) {
    top = rect.top - tipRect.height - 6;
  }

  // 左右は画面内に収まるようクランプする
  let left = rect.left + (rect.width - tipRect.width) / 2;
  left = Math.max(4, Math.min(left, win.innerWidth - tipRect.width - 4));

  tip.style.left = `${left}px`;
  tip.style.top = `${top}px`;
}

function hideTooltip(doc) {
  const tip = tipByDoc.get(doc);
  if (tip) tip.classList.remove("is-visible");
}

/**
 * 要素にホバー/フォーカスで出るツールチップを付ける。
 * aria-label も同時に設定するので、アイコンのみのボタンでも読み上げできる。
 * title 属性は付けない（ブラウザ標準のツールチップと二重に出るため）。
 * @param {Element} el
 * @param {string} text
 */
export function attachTip(el, text) {
  el.setAttribute("aria-label", text);

  let timer = null;

  const cancelTimer = () => {
    if (timer) { clearTimeout(timer); timer = null; }
  };

  const showNow = () => { cancelTimer(); showTooltip(el, text); };
  const hideNow = () => { cancelTimer(); hideTooltip(el.ownerDocument); };

  el.addEventListener("mouseenter", () => {
    cancelTimer();
    timer = setTimeout(() => showTooltip(el, text), SHOW_DELAY_MS);
  });
  el.addEventListener("mouseleave", hideNow);
  el.addEventListener("mousedown", hideNow);
  el.addEventListener("click", hideNow);
  el.addEventListener("blur", hideNow);

  // キーボード操作向け: フォーカスでは遅延なしに出す
  el.addEventListener("focusin", showNow);
  el.addEventListener("focusout", hideNow);
}

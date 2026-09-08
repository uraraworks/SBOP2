/**
 * components/icons.js
 * 画像エディタ等で使う共通 SVG アイコン。
 *
 * stroke="currentColor" にしてあるので、ボタンの文字色にそのまま追従する。
 * icon() は呼び出すたびに新しい <svg> 要素を生成して返す。同じ要素を使い回すと
 * DOM 上には 1 箇所にしか出せない（同じノードは複数の場所に同時に存在できない）ため。
 */

const SVG_NS = "http://www.w3.org/2000/svg";

// name -> <path d="..."> の d 属性の配列（複数あれば順に子として追加する）
const ICON_PATHS = {
  pen: ["M4 20h4L20 8l-4-4L4 16z", "M14 6l4 4"],
  eraser: ["M16 4l4 4-9 9H7l-3-3z", "M4 20h16"],
  picker: ["M17 3l4 4-2 2-4-4z", "M15 7L5 17v2h2L17 9z"],
  fill: [
    "M6 10l6-6 7 7-6 6a2 2 0 0 1-3 0l-4-4a2 2 0 0 1 0-3z",
    "M20 15c0 1.1-.9 2-2 2s-2-.9-2-2 2-3 2-3 2 1.9 2 3z",
  ],
  undo: ["M9 14L4 9l5-5", "M4 9h10a6 6 0 0 1 0 12h-3"],
  redo: ["M15 14l5-5-5-5", "M20 9H10a6 6 0 0 0 0 12h3"],
  save: ["M5 4h11l3 3v13H5z", "M8 4v5h7V4", "M8 13h8v7H8z"],
  revert: ["M3 12a9 9 0 1 0 2.6-6.4L3 8", "M3 3v5h5"],
  grid: ["M3 3h18v18H3z", "M3 9h18M3 15h18", "M9 3v18M15 3v18"],
  layers: ["M12 3l9 5-9 5-9-5z", "M3 13l9 5 9-5"],
  upload: ["M12 16V4", "M7 9l5-5 5 5", "M4 20h16"],
  history: ["M3 12a9 9 0 1 0 2.6-6.4L3 8", "M3 3v5h5", "M12 7v5l4 2"],
  trash: ["M4 7h16", "M9 7V4h6v3", "M6 7l1 13h10l1-13"],
  plus: ["M12 5v14M5 12h14"],
  folderOpen: ["M4 6h6l2 2h8v10H4z"],
  eye: ["M2 12s3.5-7 10-7 10 7 10 7-3.5 7-10 7-10-7-10-7z", "M12 9a3 3 0 1 0 0 6 3 3 0 0 0 0-6z"],
  sliders: ["M4 6h16M4 12h16M4 18h16", "M9 4v4M15 10v4M7 16v4"],
  zoom: ["M11 4a7 7 0 1 0 0 14 7 7 0 0 0 0-14z", "M20 20l-4-4", "M8 11h6M11 8v6"],
  arrowLeft: ["M15 5l-7 7 7 7"],
  arrowRight: ["M9 5l7 7-7 7"],
  arrowUp: ["M5 15l7-7 7 7"],
  arrowDown: ["M5 9l7 7 7-7"],
  help: ["M12 3a9 9 0 1 0 0 18 9 9 0 0 0 0-18z", "M9.5 9a2.5 2.5 0 1 1 3.2 2.4c-.8.3-1.2 1-1.2 1.8v.3", "M12 17h.01"],
  chevronDown: ["M5 9l7 7 7-7"],
  chevronRight: ["M9 5l7 7-7 7"],
};

/**
 * 名前を指定してアイコンの <svg> 要素を新規生成して返す。
 * @param {string} name ICON_PATHS のキー
 * @returns {SVGSVGElement}
 */
export function icon(name) {
  const svg = document.createElementNS(SVG_NS, "svg");
  svg.setAttribute("viewBox", "0 0 24 24");
  svg.setAttribute("width", "16");
  svg.setAttribute("height", "16");
  svg.setAttribute("fill", "none");
  svg.setAttribute("stroke", "currentColor");
  svg.setAttribute("stroke-width", "1.7");
  svg.setAttribute("stroke-linecap", "round");
  svg.setAttribute("stroke-linejoin", "round");
  svg.setAttribute("class", "ui-icon");
  svg.setAttribute("aria-hidden", "true");
  svg.setAttribute("focusable", "false");

  const ds = ICON_PATHS[name] || [];
  ds.forEach((d) => {
    const path = document.createElementNS(SVG_NS, "path");
    path.setAttribute("d", d);
    svg.appendChild(path);
  });
  return svg;
}

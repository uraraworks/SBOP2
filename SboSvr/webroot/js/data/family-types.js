/**
 * data/family-types.js
 * キャラの種族（familyId）選択肢。
 * 値は Common/GlobalDefine.h の FAMILYTYPE_* と対応。
 *   0: FAMILYTYPE_NONE
 *   1: FAMILYTYPE_HUMAN（ニンゲン）
 *   2: FAMILYTYPE_ELF（エルフ）
 *   3: FAMILYTYPE_BST（ジュウジン）
 *   4: FAMILYTYPE_DAEMON（マゾク）
 * char-edit.js / npc-add.js 等、複数画面から流用する想定。
 */

export const FAMILY_TYPE_OPTIONS = [
  { value: 0, label: "0: NONE（なし）" },
  { value: 1, label: "1: HUMAN（ニンゲン）" },
  { value: 2, label: "2: ELF（エルフ）" },
  { value: 3, label: "3: BST（ジュウジン）" },
  { value: 4, label: "4: DAEMON（マゾク）" },
];

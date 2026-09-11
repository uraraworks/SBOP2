/**
 * data/move-types.js
 * NPC の移動種別（moveType）選択肢。
 * npc-add.js / char-edit.js / weapon-list.js 等、複数画面から流用する想定。
 * 値は Common/Info/InfoCharBase.h の CHARMOVETYPE_* と対応。
 */

export const MOVE_TYPE_OPTIONS = [
  { value: 1,  label: "1: STAND（移動しない）" },
  { value: 0,  label: "0: PC" },
  { value: 5,  label: "5: ATACKANIME（攻撃アニメ）" },
  { value: 6,  label: "6: MOVEATACK（移動して攻撃）" },
  { value: 8,  label: "8: PUTNPC（NPC発生）" },
  { value: 9,  label: "9: BATTLE1" },
  { value: 10, label: "10: BATTLE2" },
  { value: 2,  label: "2: BALL（ボール）" },
  { value: 3,  label: "3: SCORE（得点）" },
  { value: 4,  label: "4: STYLECOPY_PUT（容姿コピー 取り込み）" },
  { value: 7,  label: "7: STYLECOPY_GET（容姿コピー 反映）" },
];

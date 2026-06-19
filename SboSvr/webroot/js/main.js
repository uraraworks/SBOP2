/**
 * main.js  — ES Modules エントリポイント
 *
 * 役割:
 *   1. 移行済みビューのレジストリを管理する(現時点は空)
 *   2. hashchange / data-route クリックをフックし、
 *      「移行済みビューならモジュール側で描画、未移行ならレガシー app.js に委ねる(何もしない)」
 *
 * レガシー app.js の hashchange リスナーと競合しない方針:
 *   - window.location.hash は書き換えない
 *   - isMigrated() が false の場合は何も処理しないのでレガシーがそのまま動く
 *   - isMigrated() が true の場合のみ handleRoute() を呼ぶ
 *     (将来: 必要なら app.js 側の処理をスキップさせるフラグを追加)
 */

import { registerRoute, isMigrated, handleRoute, currentRoute } from "./core/router.js";

// ----------------------------------------------------------------
// 移行済みビューの登録(F2 フェーズ以降にここへ追加していく)
// ----------------------------------------------------------------
import { mount as mountMapPartsEdit }    from "./views/map-parts-edit.js";
import { mount as mountMapShadowEdit }   from "./views/map-shadow-edit.js";
import { mount as mountMapPartsPlace }   from "./views/map-parts-place.js";
import { mount as mountMapObjectPlace }  from "./views/map-object-place.js";
import { mount as mountMotionTypes }     from "./views/motion-types.js";
import { mount as mountMotionEdit }      from "./views/motion-edit.js";
import { mount as mountEffectEdit }      from "./views/effect-edit.js";
import { mount as mountBalloonEdit }     from "./views/balloon-edit.js";
import { mount as mountItemTypes }       from "./views/item-types.js";
import { mount as mountItemList }        from "./views/item-list.js";
import { mount as mountWeaponList }      from "./views/weapon-list.js";
import { mount as mountNpcAdd }          from "./views/npc-add.js";
import { mount as mountCharEdit }        from "./views/char-edit.js";
import { mount as mountSkillEdit }       from "./views/skill-edit.js";
import { mount as mountInitStatus }      from "./views/init-status.js";
import { mount as mountServerDashboard } from "./views/server-dashboard.js";
import { mount as mountAccountCreate }   from "./views/account-create.js";
import { mount as mountRoleManagement }  from "./views/role-management.js";
import { mount as mountCharacterList }   from "./views/character-list.js";
import { mount as mountCharacterAccount }from "./views/character-account.js";
import { mount as mountMapInfo }         from "./views/map-info.js";
import { mount as mountMapEvents }       from "./views/map-events.js";
import { mount as mountTalkEvents }      from "./views/talk-events.js";
import { mount as mountMapWindow }       from "./views/map-window.js";
import { mount as mountAuditReports }    from "./views/audit-reports.js";
import { mount as mountOperationHistory }from "./views/operation-history.js";
import { mount as mountFailbackGuide }   from "./views/failback-guide.js";
import { mount as mountMapGenPatterns }  from "./views/map-gen-patterns.js";

registerRoute("map-parts",         mountMapPartsEdit);
registerRoute("map-shadows",       mountMapShadowEdit);
registerRoute("map-parts-place",   mountMapPartsPlace);
registerRoute("map-objects",       mountMapObjectPlace);
registerRoute("motion-management", mountMotionTypes);
registerRoute("motion-edit",       mountMotionEdit);
registerRoute("effect-library",    mountEffectEdit);
registerRoute("effect-balloon",    mountBalloonEdit);
registerRoute("item-types",        mountItemTypes);
registerRoute("item-list",         mountItemList);
registerRoute("weapon-list",       mountWeaponList);
registerRoute("npc-management",    mountNpcAdd);
registerRoute("character-overview",mountCharEdit);
registerRoute("skill-management",  mountSkillEdit);
registerRoute("initial-status",    mountInitStatus);
registerRoute("server-dashboard",  mountServerDashboard);
registerRoute("account-create",    mountAccountCreate);
registerRoute("role-management",   mountRoleManagement);
registerRoute("character-list",    mountCharacterList);
registerRoute("character-account", mountCharacterAccount);
registerRoute("map-info",          mountMapInfo);
registerRoute("map-events",        mountMapEvents);
registerRoute("talk-events",       mountTalkEvents);
registerRoute("map-window",        mountMapWindow);
registerRoute("audit-reports",     mountAuditReports);
registerRoute("operation-history", mountOperationHistory);
registerRoute("failback-guide",    mountFailbackGuide);
registerRoute("map-gen-patterns",  mountMapGenPatterns);

// ----------------------------------------------------------------
// hash ルーティングフック
// ----------------------------------------------------------------

function onHashChange() {
  const route = currentRoute();
  if (isMigrated(route)) {
    handleRoute(route);
  }
  // 未移行ルートは何もしない → レガシー app.js の hashchange リスナーが処理する
}

window.addEventListener("hashchange", onHashChange);

// 初回ロード時: すでに hash がある場合に対応
// app.js の DOMContentLoaded 以降に実行されるため、レガシーの初期化と競合しない
(function initCurrentRoute() {
  const route = currentRoute();
  if (route && isMigrated(route)) {
    handleRoute(route);
  }
})();

// ----------------------------------------------------------------
// data-route クリックフック(移行済みビューのみ介入)
// ----------------------------------------------------------------

document.addEventListener("click", (event) => {
  const link = event.target.closest("[data-route]");
  if (!link) {
    return;
  }
  const route = link.dataset.route;
  if (!isMigrated(route)) {
    // 未移行: レガシーのクリックリスナーに委ねる
    return;
  }
  // 移行済み: hash 書き換えは行わず handleRoute を直接呼ぶ
  // (hash 書き換えは app.js 側が行うのでそちらに任せる)
  // ここでは何も追加処理しない — hashchange イベントが発火して onHashChange が処理する
}, { capture: false });

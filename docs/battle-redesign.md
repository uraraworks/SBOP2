# 戦闘の作り直し計画書

> 作成日: 2026-09-15
> 対象: 通常攻撃・戦闘モード・攻撃ゲージ・釣り・X/Zキーの操作系全般。

---

## 1. 背景・目的

- 現状は「Tab で戦闘モードに切替 → 立ち止まるとクライアント側の攻撃ゲージが溜まる → 10以上で攻撃し10消費」という流れで、「敵→モード切替→待つ→攻撃→待つ」とテンポが悪い。旧SBO踏襲の設計で、当時は通信環境への配慮もあった。
- 目的: 通常攻撃は即発動・ボタン押しっぱなしで連続攻撃にする。戦闘モード切替は廃止し、戦闘可能マップかどうかで判定する。準備時間の概念はスキル・魔法側（詠唱時間）に移す。ロードマップ一層目「ハクスラのループ」のテンポ改善が狙い。

---

## 2. 現状調査（要点と file:line）

- 戦闘モードは独立フラグでなく `m_nMoveState` の CHARMOVESTATE_BATTLE/BATTLEMOVE/BATTLEATACK/BATTLEATACK_WAIT/BATTLE_DEFENSE（`Common/Info/InfoCharBase.h:58-66`）。判定は `IsStateBattle()`（`Common/Info/InfoCharBase.cpp:1114`、BATTLEATACK_WAIT は含まない）。切替は `CStateProcMAP::OnTab`（`SboCli/src/StateProc/StateProcMAP.cpp:2659`、モード遷移の中心部分は2699-2726）、サーバー受信は `RecvProcCHAR_STATE`（`SboSvr/src/MainFrame/MainFrameRecvProcCHAR.cpp:1010`）。移動・同期・NPC AI・`SboSvrTest/TestMoveStateDecision.cpp` まで広く絡む → 内部状態は残し、切替を自動化する方針。
- 攻撃ゲージ `m_wAtackGauge`（`Common/Info/InfoCharBase.h:157`、`MAX_ATACKGAUGE=100`）はクライアントのみ。`TimerProcGauge`（`SboCli/src/StateProc/StateProcMAP.cpp:1791`）で最終行動から750ms後に25msごと+1。消費は `OnX`（`StateProcMAP.cpp:2165`、`Todo:暫定` コメントあり）。同期なし。
- 攻撃の流れ: `OnX` → `CHAR_STATE(BATTLEATACK)` 送信・`ChgWait` → サーバー応答でモーション → 攻撃コマで `BATTLE_REQ_ATACK`（`SboCli/src/Info/InfoCharCli.cpp:2369`）→ サーバー `RecvProcBATTLE_REQ_ATACK`（`SboSvr/src/MainFrame/MainFrameRecvProcBATTLE.cpp:22`）→ `CLibInfoCharSvr::Atack`/`AtackImple`（`SboSvr/src/LibInfo/LibInfoCharSvr.cpp:642`/`722`）。攻撃1回に往復2回。
- サーバー検証の欠如: 間隔・ゲージ・状態・戦闘可能マップのいずれも未検証（HP>0・非気絶のみ）。改造クライアントで無制限連打・非戦闘モードから攻撃可能。`CHARMOVESTATE_BATTLEATACK_WAIT`（1000ms、`SboSvr/src/Info/InfoCharSvr.cpp:172`）は定義済みだがどこからもセットされず未使用。
- 攻撃対象判定: `AtackImple` の `m_nAtackTarget` 分岐（`LibInfoCharSvr.cpp:737-755`）で、未設定なら「NPC同士」以外は全部当たる。~~店員NPCや非戦闘PCも殴れる~~ → 訂正: `CInfoCharSvr::IsAtackTarget`（`InfoCharSvr.cpp:572`付近）が STAND/BALL/SCORE/PUTNPC を既に除外しており、店員NPC等（`CHARMOVETYPE_STAND`）は対象外だった。一方、容姿コピー(`STYLECOPY_PUT`/`GET`)・矢等(`MOVEATACK`)・PCは対象に残っていた（容姿コピーは会話専用の仕掛けでダメージ想定外、矢は攻撃側であり的にする想定外、PCはPvP可否フラグが無いのに殴れてしまう）。PvP可否フラグなし。
- 溜め攻撃: X 2秒押しで成立（`StateProcMAP.cpp:1766` 付近、`dwTime < 2000` 判定）、ダメージ2倍。
- 釣り: 戦闘モード＋釣り竿装備で X → 攻撃モーションが釣りに差し替わり（`SboCli/src/LibInfo/LibInfoCharCli.cpp:273`）、`SBOCOMMANDID_SUB_CHAR_PROC_FISHING` 送信（`InfoCharCli.cpp:2376`）。サーバーで水タイル確認し待ち登録（`MainFrameRecvProcCHAR.cpp:1929` 付近）、アタリ・釣り上げは完全自動（`SboSvr/src/Info/InfoCharSvr.cpp:649,672` 付近）。ゲージも10消費。
- キー: X は戦闘モード中=攻撃、立ち=会話(`OnXChar`)→拾う→追従解除（`StateProcMAP.cpp:2131-2252`）。Z は戦闘中=防御、それ以外=追従要求（`StateProcMAP.cpp:2256`〜）。押す/蹴るは移動キーで当たる方式なので1ボタン化と衝突しない。
- 会話可否は `m_strTalk`（`Common/Info/InfoCharBase.h:253`）が空でないか。敵専用フラグなし。
- 戦闘モード付随効果: 移動周期1.4倍（`GetMoveWait`、`InfoCharBase.cpp:1488`、`IsStateBattle()` 分岐は1493）、HP/SP/ゲージは戦闘モード中のみ描画（`SboCli/src/Layer/LayerMap.cpp:1659`）、相手が戦闘モードなら追従拒否（`LibInfoCharSvr.cpp:563`）、BGM切替（`SboCli/src/MainFrame/MainFrame.cpp:661-669`）、スマホパッド通知（`StateProcMAP.cpp:785-801`）。
- スマホパッド（`tools/emscripten/sbocli-title.shell.html:2118-2198` 付近）: A=Z, B=X, 戦闘=Tab, メニュー=Space, 座る=R, 向き=Shift。
- スキル: ゲージ不使用、SP消費のみ、クールダウンなし。`SKILLUSE_BATTLE`（`Common/Info/InfoSkill/InfoSkillBase.h:45`）は定義のみ未チェック。
- 戦闘可能マップ: `CInfoMapBase::IsEnableBattle()`（`m_bEnableBattle`、`Common/Info/InfoMapBase.cpp:730`）。

---

## 3. 決定した仕様

1. 通常攻撃: 即発動・押しっぱなしで連続。武器ごとの攻撃間隔。攻撃モーション中は足が止まる。
2. 攻撃ゲージ廃止。溜め攻撃はいったん廃止。
3. 戦闘モード切替（Tab・スマホ「戦闘」ボタン）廃止。戦闘可能マップで攻撃すると内部的に戦闘状態へ自動遷移し、一定時間攻撃しなければ戻る。
4. 攻撃対象の判定（共通コードに置き、クライアントのボタン動作決定とサーバーの受付判定の両方で使う）: 移動種別 BATTLE1/BATTLE2/ATACKANIME かつ HP≥1。PvP可マップではPCも対象（PvP可否のマップ設定は新設が必要）。スポーン元有無・HPのみでの判定は不採用（理由: スポーン元はDB直置き敵を見逃す・敵以外も出す、HPは村人等にも入っている）。
5. X ボタン（状況依存1ボタン）: 押した瞬間に正面を判定する。敵→攻撃、会話可NPC→会話、釣り竿装備で正面が水→釣り、何もなし→空振り。敵と会話NPCが同時なら敵優先。押しっぱなしで攻撃が始まったら離すまで攻撃（空振り含む）、会話・釣りは押し直した時だけ（連続攻撃中に奥の村人と会話が始まる事故防止）。
6. Z ボタン: 正面がPC→付いて行く、それ以外は戦闘可能マップで防御。PvPマップで相手がPCなら防御優先。
7. 移動速度: 戦闘モード中の1.4倍減速はなくなる。代わりに被弾で一定時間減速。サーバーの速度チェックも同じ値に揃える（過去に速度超過で移動が永久に弾かれる事故あり）。弓は引き撃ち対策として攻撃後の停止を長めにする調整を後で検討。
8. HP バー: 戦闘可能マップで常時表示、立ち止まってしばらくでフェードアウト、攻撃・被弾で再表示、メニューを開いたら隠す。
9. 付いて行く: 攻撃したら解除。「相手が戦闘モードなら拒否」チェックは不要になる。
10. BGM: 切替なし。戦闘BGMはいったん未使用。
11. 釣り: 攻撃から切り離し、竿装備＋正面が水＋X で投げる。アタリ・合わせを操作にするかは将来検討（当面は現行の自動判定を流用でよい）。
12. スキル・魔法: SP消費は現行のまま。詠唱時間・クールダウンは後段。

---

## 4. 段階計画

| 段階 | 目的 | 変更対象 | 完了条件 | 確認方法 |
|---|---|---|---|---|
| S1 | 改造クライアント対策としてサーバー側の攻撃検証を先に固める（仕様変更と独立に有効） | サーバーの攻撃間隔・戦闘可能マップチェック・PvP判定・攻撃対象判定のホワイトリスト化（`AttackDecision.h/.cpp` に純粋関数として切り出し、`RecvProcBATTLE_REQ_ATACK`/`AtackImple` から利用） | 未検証だった間隔・マップ・対象がサーバーで拒否できる | `SboSvrTest` にテスト追加（新テストは必ず一度落ちることを確認してから直す） |
| S2 | ゲージ・溜め攻撃を廃止し即攻撃＋押しっぱなし連続にする | `m_wAtackGauge`/`TimerProcGauge`/溜め攻撃周りの削除、`OnTab`・スマホ「戦闘」ボタン削除、戦闘状態の自動遷移、攻撃の往復2回をクライアント即モーション＋送信1回に（クライアント予測） | Tab無しで敵に攻撃が当たり連打できる、戦闘モードへの明示切替が不要 | 手元サーバー＋ブラウザ版で連続攻撃・自動遷移・復帰を確認 |
| S3 | X・Z を状況依存の1ボタンにする | `OnX`/`OnXChar`/`OnZ` の判定順（敵優先）の実装 | 正面の敵/会話NPC/水/何もなしで意図した動作になる | 敵と会話NPCが同時にいる配置で優先順を確認 |
| S4 | 釣りを攻撃から切り離す | 釣り竿装備＋正面水＋X の専用経路に分離 | 戦闘可能マップでも竿装備時は釣りが優先して起きる | 水際・戦闘可能マップの両方で確認 |
| S5 | 被弾減速とHPバーのフェード表示、攻撃での追従解除 | 移動速度チェックの整合、`LayerMap` のHP表示、追従解除処理 | 被弾後に減速し時間で戻る、HPバーがフェードイン/アウトする、攻撃で追従が切れる | 被弾・放置・攻撃の各シナリオで確認 |
| 後段 | スキル詠唱時間・クールダウン、弓バランス、PvPマップ設定、戦闘BGMの扱い | 未着手 | — | — |

### S1 実装メモ

- 間隔は `CHARMOVESTATE_BATTLEATACK_WAIT` 状態(未使用のまま残す)ではなく、`CInfoCharSvr::m_dwLastAtackAcceptedTime`/`m_dwPrevAtackAcceptedTime`(受理時刻2つ)で判定する。移動同期等、状態同期に割り込むモード変更を増やさないため。
- S1ではPC同士の攻撃を全面禁止する（`AttackDecision::IsPvpAttackBlocked()` に呼び出し側から常に `bMapAllowsPvp=false` を渡す）。PvPマップ設定を追加したらそこから引いた値を渡して解禁する。
- 容姿コピー(`STYLECOPY_PUT`/`GET`)・矢等(`MOVEATACK`)は `CInfoCharSvr::IsAtackTarget` のホワイトリスト化により攻撃対象外になった(PC・`BATTLE1`・`BATTLE2`・`ATACKANIME` のみ対象)。

---

## 5. リスク・注意点

- `m_nMoveState` の BATTLE 系は移動同期に絡む。`MoveStateDecision` 系テストを壊さないこと。
- 並行ビルドで古い obj が最新扱いになる罠がある（コミット前に変更した `.cpp` を touch してリンクまで通す、[[feedback_parallel_build_stale_obj]]）。
- ブラウザ版はソース一覧が直書き（[[project_browser_build_pipeline_gotchas]]）。新規 `.cpp` を追加する段では `tools/build-sbocli-browser-title.ps1` の一覧更新を忘れないこと。加えて `tools/emscripten/sbocli-title.shell.html` のパッド定義（戦闘ボタン等）も要修正。
- 敵が `m_strTalk` を持っていても敵優先（会話は起こさない）。

---

## 6. 未決事項

- PvP可否のマップ設定フラグの名称・保存先（管理画面側の対応も別途必要）。
- 弓の引き撃ち対策（攻撃後停止時間の具体的な値）。
- スキル詠唱時間・クールダウンの具体的な数値と実装場所。
- 釣りのアタリ・合わせを操作化するかどうか。

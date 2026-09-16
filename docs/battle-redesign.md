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

1. 通常攻撃: 即発動・押しっぱなしで連続。武器ごとの攻撃間隔＝装備武器の攻撃モーション全体時間（起こりうる向き・振り/突きの候補すべてのうち最短）。サーバーはその80%を下限とし、モーションが求められない場合は200ms、どちらの経路でも下限100msでクランプする（`AttackDecision::GetMotionDurationMs`/`ComputeAttackIntervalMs`、S2実装）。攻撃モーション中は足が止まる。
2. 攻撃ゲージ廃止。溜め攻撃はいったん廃止。
3. 戦闘モード切替（Tab・スマホ「戦闘」ボタン）廃止。戦闘可能マップで攻撃すると内部的に戦闘状態へ自動遷移し、一定時間（攻撃しないで5秒）攻撃しなければ戻る。サーバーは戦闘不可マップでの戦闘状態遷移（BATTLE/BATTLEATACK/BATTLE_DEFENSE）を拒否する（`RecvProcCHAR_STATE`、S2実装）。
4. 攻撃対象の判定（共通コードに置き、クライアントのボタン動作決定とサーバーの受付判定の両方で使う）: 移動種別 BATTLE1/BATTLE2/ATACKANIME かつ HP≥1。PvP可マップではPCも対象（PvP可否のマップ設定は新設が必要）。スポーン元有無・HPのみでの判定は不採用（理由: スポーン元はDB直置き敵を見逃す・敵以外も出す、HPは村人等にも入っている）。
5. X ボタン（状況依存1ボタン）: 押した瞬間に正面を判定する。敵→攻撃、会話可NPC→会話、釣り竿装備で正面が水→釣り、何もなし→空振り。敵と会話NPCが同時なら敵優先。押しっぱなしで攻撃が始まったら離すまで攻撃（空振り含む）、会話・釣りは押し直した時だけ（連続攻撃中に奥の村人と会話が始まる事故防止）。
6. Z ボタン: 正面がPC→付いて行く、それ以外は戦闘可能マップで防御。PvPマップで相手がPCなら防御優先。
7. 移動速度: 戦闘モード中の1.4倍減速はなくなる。PCの戦闘中減速はS2で廃止（`GetMoveWait`、S2実装。NPCは従来どおり1.4倍/`m_dwMoveWaitBattle`を維持）。代わりに被弾で一定時間減速（S5で実装）。サーバーの速度チェック（`GetMoveWaitBase`/`GetCharMovePixelsPerSec`）は同じ`GetMoveWait`を経由するため自動的に揃う（過去に速度超過で移動が永久に弾かれる事故あり）。弓は引き撃ち対策として攻撃後の停止を長めにする調整を後で検討。
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
| S3 | X・Z を状況依存の1ボタンにする | `OnX`/`OnXChar`/`OnZ` の判定順（敵優先）の実装 | 正面の敵/会話NPC/水/何もなしで意図した動作になる | 実機確認済み（2026-09-16）。会話は別要因（NPCのセリフがDBに無い）、付いて行きは既存回帰のため別作業 |
| S4 | 釣りを攻撃から切り離す | 釣り竿装備＋正面水＋X の専用経路に分離 | 戦闘可能マップでも竿装備時は釣りが優先して起きる | 水際・戦闘可能マップの両方で確認 |
| S5 | 被弾減速とHPバーのフェード表示、攻撃での追従解除 | 移動速度チェックの整合、`LayerMap` のHP表示、追従解除処理 | 被弾後に減速し時間で戻る、HPバーがフェードイン/アウトする、攻撃で追従が切れる | 被弾・放置・攻撃の各シナリオで確認 |
| 後段 | スキル詠唱時間・クールダウン、弓バランス、PvPマップ設定、戦闘BGMの扱い | 未着手 | — | — |

### S1 実装メモ

- 間隔は `CHARMOVESTATE_BATTLEATACK_WAIT` 状態(未使用のまま残す)ではなく、`CInfoCharSvr::m_dwLastAtackAcceptedTime`/`m_dwPrevAtackAcceptedTime`(受理時刻2つ)で判定する。移動同期等、状態同期に割り込むモード変更を増やさないため。
- S1ではPC同士の攻撃を全面禁止する（`AttackDecision::IsPvpAttackBlocked()` に呼び出し側から常に `bMapAllowsPvp=false` を渡す）。PvPマップ設定を追加したらそこから引いた値を渡して解禁する。
- 容姿コピー(`STYLECOPY_PUT`/`GET`)・矢等(`MOVEATACK`)は `CInfoCharSvr::IsAtackTarget` のホワイトリスト化により攻撃対象外になった(PC・`BATTLE1`・`BATTLE2`・`ATACKANIME` のみ対象)。

### S2 実装メモ（サーバー・共通コード分。クライアントは別作業）

- 攻撃間隔: `AttackDecision::GetMotionDurationMs()`(Wait配列→全体時間)と`ComputeAttackIntervalMs()`(候補の最短×80%・フォールバック・下限クランプ)を純粋関数として追加。武器→候補モーション列挙(振り/突き両方、弓・打撃・釣りの上書き優先順、上下左右4方向)は `MainFrameRecvProcBATTLE.cpp` の `CollectAtackMotionListBases()`/`GetAtackIntervalMs()` に置き、クライアントの選択ロジック(`SboCli/src/LibInfo/LibInfoCharCli.cpp` の `SetMotionInfo()`)と同じ優先順位で候補を集める。`RecvProcBATTLE_REQ_ATACK` の固定300ms(`ATACK_MIN_INTERVAL_MS`)をこれに置き換えた。
- `RecvProcCHAR_STATE`(`MainFrameRecvProcCHAR.cpp`)で、BATTLE/BATTLEATACK/BATTLE_DEFENSE への遷移要求は `IsEnableBattle()` に加え、`pInfoChar->m_dwMapID` から引いたマップの `IsEnableBattle()` も必須にした（Tabを介さず攻撃と同時に自動遷移するため、戦闘不可マップでの遷移をサーバーで拒否）。
- `CInfoCharBase::GetMoveWait()`(`Common/Info/InfoCharBase.cpp`)で `m_nMoveType==CHARMOVETYPE_PC` の場合は戦闘状態でも1.4倍/`m_dwMoveWaitBattle`を適用しないようにした。NPCは従来どおり。サーバーの移動速度検証(`GetMoveWaitBase`/`GetCharMovePixelsPerSec`、押し要求の速度検証も同じ経路)はこの関数を経由するため、PCの許容速度も自動的に緩和側へ揃い、矛盾は生じない。
- 「戦闘状態の自動遷移」「攻撃しないで5秒で戦闘状態を解除」自体（クライアント側のTab/ゲージ削除・自動遷移・タイムアウト）はクライアント側作業として別途行う。

### S2 実装メモ（クライアント側。クライアント側実装済み・実機確認済み（2026-09-16））

- `CStateProcMAP::OnX`(`StateProcMAP.cpp`): bDown==TRUEの時だけ処理し、bDown==FALSE(離した時)は何もしない（攻撃・会話・拾いを押した瞬間に1回だけ決定する）。STANDでは従来通り会話(`OnXChar`)→足元アイテム拾いを優先し、どちらも成立しなければ`m_pMap->IsEnableBattle()`が真の場合に`StartLocalAtack()`で即座にBATTLEATACKへ入る。BATTLE(静止)でも同様に即攻撃。付いて行き中(`m_dwFrontCharID`)はOnTab旧メッセージ相当（「付いて行っている時は攻撃できません」）を出し、`IsEnableMove()==FALSE`（攻撃モーション中・気絶・防御中含む）は何もしない。
- `CStateProcMAP::StartLocalAtack()`(新設): サーバーの返事を待たずCHAR_STATE(BATTLEATACK)を1回送信した直後に、ローカルでも`RenewMotionInfo`+`ChgMoveState(BATTLEATACK)`を呼んで即座に攻撃モーションを開始する（往復2回→送信1回+ローカル即時反映）。MOVE_STOPも同時送信して他プレイヤーの見た目を止める。
- 自キャラの二重再生/引き延ばし対策: `MainFrameRecvProcCHAR.cpp`の`RecvProcCHAR_STATE`で、`nState==CHARMOVESTATE_BATTLEATACK`かつ自キャラが既にBATTLEATACK中（＝ローカルで開始済み）の場合は`RenewMotionInfo`を呼び直さないようにした。`RenewMotionInfo`→`InitMotionInfo(-1)`が`m_dwLastTimeAnime`をリセットするため、素朴にサーバーエコーへ反応すると再生中の攻撃モーションの現在コマ表示時間が引き延ばされて見えることが分かったため。`ChgMoveState`自体は同一状態なら`break`するだけで実害はない。
- 自キャラの攻撃モーション中はサーバーからの BATTLE エコーを無視（モーション切れ・連続攻撃の早撃ち防止）。
- 押しっぱなし連続攻撃: `TimerProcAtackRepeat()`(旧`TimerProcChargeAtack`を作り替え)が毎フレーム、`m_bAtackKeyAutoRepeat`(攻撃で始まった押下のみTRUE。会話・拾いで始まった押下はFALSEのまま)かつ`m_nMoveState==CHARMOVESTATE_BATTLE`（＝攻撃モーションが自動的にBATTLEへ戻った後）かつXキー押下継続中なら`StartLocalAtack()`を再度呼ぶ。ウィンドウ表示中・Xキー解放・戦闘不可マップになった場合は`m_bAtackKeyAutoRepeat`をFALSEにして止める。防御開始(`OnZ`)でも明示的にFALSEにする。BATTLEATACK再生中(攻撃モーション終了は既存の`CInfoCharCli::TimerProcAtack`が自動でBATTLEへ戻す)はこの関数では何もしない。
- 自動解除: `TimerProcAtackAutoOff()`(旧`TimerProcGauge`を作り替え)が、`m_nMoveState==CHARMOVESTATE_BATTLE`のまま`m_dwLastAtackTime`(最後に`StartLocalAtack()`した時刻)から5秒経過したらSTANDへ戻しCHAR_STATE(STAND)を送る。BATTLEMOVE中は判定しない（止まってから判定でよい、5章参照）。
- 廃止した箇所: `TimerProcGauge`・`OnX`のゲージ判定・被弾時ゲージ減少(`OnWindowMsg`の`MAINFRAMEMSG_DAMAGE`)・`LayerMap::DrawGauge`のアタック/ガードゲージ描画・溜め攻撃(`TimerProcChargeAtack`・`m_dwStartChargeTime`・`OnX`/`OnZ`/`OnTab`の`m_bChargeAtack`分岐・`CInfoCharCli::ChgMoveState`/`MotionProc`/`TimerProcMove`/`GetMotionInfo`の`m_bChargeAtack`分岐・`CMainFrame::ChgMoveState`の溜め時2倍待ち)・`OnTab`本体（no-opのスタブ化）・スマホパッドの`padTab`ボタン（`tools/emscripten/sbocli-title.shell.html`、空きマスとして`.pad-slot-empty`に置換）。`SBOCOMMANDID_SUB_CHAR_STATE_CHARGE`の送信元が無くなったため、クライアントの受信ハンドラ(`RecvProcCHAR_STATE_CHARGE`)とサーバー側・`Common`の`m_bChargeAtack`/`m_wAtackGauge`/`m_wDefenseGauge`フィールド自体は実害がないため残した（`SboSvr`/`Common`は本タスクの対象外）。
- BGM: `OnTab`が唯一`CMainFrame::ChgMoveState(TRUE)`(BGM切替あり)を呼んでいた呼び出し元だったため、OnTab削除に伴い戦闘BGMへの切替は自動的に発生しなくなった（想定通り。個別の分岐削除はしていない）。
- 未確定/判断した点: 「付いて行き中は従来メッセージ」は元のOnTabの文言をそのまま使わず「付いて行っている時は攻撃できません」に変更した（Tab→攻撃に主語を合わせた）。また、付いて行い中でも従来通りREQ_TAIL(0,FALSE)で追従解除は行う（メッセージ表示と両立、追従解除機能自体は壊さない判断）。連続攻撃の一時停止条件はBATTLE_DEFENSE中を含め「BATTLE状態でなければ何もしない」という単純な条件に統一し、防御解除後に再開したくない場合を考慮して`OnZ`の防御開始時に明示的に`m_bAtackKeyAutoRepeat=FALSE`にした。

### S3 実装メモ（完了、2026-09-16）

- 攻撃対象の判定を共通化: `CInfoCharBase::IsAtackTarget()`(`Common/Info/InfoCharBase.cpp`)へホワイトリスト判定(PC/BATTLE1/BATTLE2/ATACKANIME)を移動した。サーバーの`CInfoCharSvr::IsAtackTarget()`はこれを呼んだ後に無敵チェックを足すだけに簡略化(挙動は変えていない)。クライアントは`CInfoCharCli`がオーバーライドしないため基底のこの判定をそのまま使う。PCも判定には残るため、クライアント側は別途`IsNPC()`でPCを除外する(`CStateProcMAP::IsEnemyChar`)。
- 攻撃の届く範囲での正面キャラ検出を共通化: `CLibInfoCharBase::GetFrontCharIDTarget()`(`Common/LibInfo/LibInfoCharBase.h/.cpp`)を新設し、サーバーの`CLibInfoCharSvr::GetFrontCharIDTarget`と同じジオメトリ(1歩前の当たり矩形+24pxリーチ、斜めは`GetDrawDirection`で上下左右に分解)にした。サーバー版は`m_paInfoLogin`(ログイン中の全キャラ、PC+NPC)を、新設した基底実装は`m_paInfo`(クライアントが知っているキャラ)を走査する点だけが違う。サーバーは既存の独自実装で上書きするため挙動は変わらない。
- クライアントの敵判定: `CStateProcMAP::IsEnemyChar(pInfoChar)`(NPCかつ`IsAtackTarget()`)と`GetFrontEnemyCharID(dwCharID, nDirection)`(`GetFrontCharIDTarget`+`IsEnemyChar`)を新設(`StateProcMAP.cpp`)。
- `OnX`: STAND/BATTLE(静止)を1つのcaseにまとめ、押した瞬間の判定順を a.戦闘可能マップで正面に敵がいれば攻撃 → b.正面に会話できるNPCがいれば会話(`GetTalkCharID`→`OnXChar`) → c.足元にアイテムがあれば拾う(戦闘中でも拾えるようにした) → d.戦闘可能マップなら空振り攻撃 → e.それ以外は従来どおり付いて行い解除、の順にした。押しっぱなし連続攻撃(`m_bAtackKeyAutoRepeat`)はa/dで始まった押下のみで従来どおり。
- `GetTalkCharID`: 見つけた相手が`IsEnemyChar()`なら0(会話対象なし)を返すようにし、敵は`m_strTalk`を持っていても会話しないようにした(実際にはOnX側の判定順で敵が先に処理されるため通常は到達しないが、`GetFrontCharIDTarget`(リーチ矩形)と`GetFrontCharID`/`GetHitCharID`(1マス)は判定基準が異なるため保険として両方の戻り値に適用)。
- カウンター越し会話の座標不具合を修正: `GetTalkCharID`のカウンター越しループで`ptFrontMapPos.x = nPosX[nDirection]`のように代入になっていて現在位置を失っていた箇所を、隣の`ptFrontPos`と同様に`+=`(加算)に修正した。カウンターが連続する配置でだけ症状が出るため実機で気付きにくいバグだった。
- `OnZ`: 離した時の挙動を「押した瞬間の判定」で決めるよう`m_bZKeyDefenseActive`フラグを新設(押した瞬間に防御を開始したかを記録)。押した瞬間は 付いて行き中/座り中/気絶中(`m_dwFrontCharID`及びSTAND/BATTLE以外の`m_nMoveState`)なら不可、正面がPC(`IsNPC()==FALSE`)なら何もしない(離した時にPvPマップ未実装のため従来どおり付いて行い要求を送る)、それ以外は戦闘可能マップなら STAND/BATTLE から自動でBATTLE_DEFENSEへ遷移。離した時は`m_bZKeyDefenseActive`を見て`DefenseOff()`か付いて行い要求かを選ぶ(現在の`m_nMoveState`でなく押した瞬間の意図で決めることで、保持中に状態が変わっても意図がぶれないようにした)。防御開始時も`m_dwLastAtackTime`を更新し、5秒無操作の自動解除タイマーに「最後の戦闘行動」として含めた。`TimerProc`内の別経路(Z押しっぱなし検出でOnZ(TRUE)を毎フレーム呼ぶ処理、防御中のリリース検出でDefenseOff()を直接呼ぶ処理)は変更していない(既存のガード条件だけで両立する)。
- スマホパッドのA/B表記: `SBOP2_NotifyBattleModeChange`(EM_JS)の判定基準を自キャラの`m_nMoveState`(戦闘系状態)から`m_pMap->IsEnableBattle()`(戦闘可能マップかどうか)に変更した(`StateProcMAP.cpp`のTimerProc、`sbocli-title.shell.html`のコメントも追従)。戦闘モード切替(Tab)がS2で廃止済みのため「状態に関わらずA=防御/B=攻撃」という仕様に合わせた。関数名・EM_JS名・JS側の変数名(`padIsBattleMode`)はそのまま残し、コメントだけ更新した。
- 未確定/判断した点: 会話イベント(@)専用の窓(`SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT`)はブラウザ版で未移植の可能性がある(S2時点で確認できなかった)。本タスクでは`OnXChar`の会話イベント分岐自体には手を入れておらず、別作業として切り出す。

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

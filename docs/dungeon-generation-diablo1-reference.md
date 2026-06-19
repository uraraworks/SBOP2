# Diablo 1 ダンジョン生成アルゴリズム 調査メモ（SBOP2 移植視点）

> 調査日: 2026-06-19（初版）／2026-06-19（コード裏取り更新）
> 目的: ランダムダンジョン生成の「考え方」の目標として Diablo 1 (devilutionX/devilution) の生成方式を参考にする。
> SBOP2 はクォータービューではないが、Diablo の内部マップも **2D タイル配列**なので、考え方はトップダウンのタイルマップにほぼ直で移植できる。

## 出典と裏取り状況

- 一次情報: `devilutionX` の `Source/levels/drlg_l1.cpp` / `drlg_l2.cpp` / `drlg_l3.cpp` / `drlg_l4.cpp`、`Source/levels/gendung.cpp`、`Source/levels/gendung.h`、`Source/engine/random.hpp/cpp`。
- 二次情報（本メモの初版ベース）: BorisTheBrave の解析記事「Dungeon Generation in Diablo 1」 https://www.boristhebrave.com/2019/07/14/dungeon-generation-in-diablo-1/
- 2026-06-19 にローカル clone したソースで全重点箇所を裏取り済み。確認箇所には「（コード確認済: 関数名/ファイル:行）」を付した。

---

## 全体像（4ステージ共通）

- **マップは 40×40 の論理グリッド**で生成する（`#define DMAXX 40` / `#define DMAXY 40`）（コード確認済: `gendung_defs.hpp:5-6`）。最終的な描画タイルはこれを展開したもの。
- **2段階生成**:
  1. **事前生成（pre-generation）**: タイルの絵柄は決めず、「通行可/壁/ドア」等の高レベルな意味だけを持つ抽象配列を作る。
  2. **タイル変換（+ fixup）**: 抽象配列を実タイルIDの配列に変換し、接続の破綻を直す後処理を入れる。
- **ステージ（テーマ）ごとに別の生成ルーチン**を持つ。共通の万能アルゴリズムではなく、テーマ別に最適な手法を使い分けている。

| ステージ | レベル | 生成方式の核 |
|---|---|---|
| Cathedral（聖堂） | 1-4 | チェンバー（最大3個の10×10部屋）＋廊下 → 再帰的 `GenerateRoom` で部屋増殖 |
| Catacombs（地下墓地） | 5-8 | 部屋の周囲をぐるりと回る廊下構造（狭く入り組む） |
| Caves（洞窟） | 9-12 | 中央から再帰的にブロックを生やす＋縁を50%確率で床に（有機的） |
| Hell（地獄） | 13-16 | 第1象限20×20を `GenerateRoom` で作り、縦横通路を足して**4象限ミラー**で全体化 |

---

## ステージ別アルゴリズム

### L1 Cathedral（チェンバー＋再帰出芽）

（コード確認済: `drlg_l1.cpp` `FirstRoom()`:504, `GenerateRoom()`:456, `FillChambers()`:987, `GenerateLevel()`:1163）

1. **`FirstRoom()` で背骨を引く**: `HasChamber1`/`HasChamber2`/`HasChamber3` の3フラグ（各50%）で最大3個の **10×10 チェンバー**（位置: 1,15 / 15,15 / 29,15）を配置し、幅6の横（または縦）廊下で繋ぐ。少なくとも中央チェンバーは常に有効（コード確認済: `drlg_l1.cpp:516-518`）。`VerticalLayout` フラグが立つと座標を x/y 反転。
2. **`GenerateRoom(rectangle, verticalLayout)` の再帰停止条件**: 毎回 `FlipCoin(4)` = 25% で縦横向きを反転する。部屋サイズを `(GenerateRnd(5)+2) & ~1`（2〜6の偶数）で20回試行し、`CheckRoom()` が通れば配置、失敗したままでも次のチェック側へ進む。再帰に深さ上限はなく、**`CheckRoom()` が両方失敗した場合は自然終了**（コード確認済: `drlg_l1.cpp:458-498`）。
3. **最小面積によるリトライ**: `GenerateLevel()` は `FindArea() >= minarea` になるまで `FirstRoom()` を繰り返す（lv1: 533, lv2: 693, それ以外: 761）（コード確認済: `drlg_l1.cpp:1168-1186`）。
4. **`FillChambers()` でチェンバー内部を確定**: `GenerateChamber()` でチェンバー内部 9×9 を Floor タイルで塗り、隣接チェンバーとのアーチ接続タイル（`HArch`/`VArch`/`DArch` 等）を配置。`GenerateHall()` で間の廊下を確定（コード確認済: `drlg_l1.cpp:987-1025`）。
5. **壁の後処理 `AddWall()`**: `Corner` / `HWallEnd` / `VWallEnd` / `HWall` / `VWall` の各タイルを起点に `HorizontalWallOk()` / `VerticalWallOk()` で延伸可能範囲を測り、`HorizontalWall()` / `VerticalWall()` で壁タイルを引く（コード確認済: `drlg_l1.cpp:702-753`）。`Protected` / `Chamber` タイルはスキップ。
6. **L1固有 miniset**: `LAMPS`（5〜9個ランダム設置）/ `STAIRSUP` / `STAIRSDOWN` / `PWATERIN`（コード確認済: `drlg_l1.cpp:1224-1226`）。

**SBOP2 移植メモ**: L1 の出芽再帰は「部屋を置けたら同じ区域を再帰」という単純構造。チェンバー3個 + 廊下の骨格が最初にあるため、必ず広い空間が1箇所以上生まれることが保証されている点が移植しやすい。

### L2 Catacombs（再帰分割部屋 ＋ steering 廊下）

（コード確認済: `drlg_l2.cpp` `CreateRoom()`:1766, `ConnectHall()`:1864, `CreateDungeon()`:2440, `GenerateLevel()`:2674）

#### 事前生成フェーズ（predungeon 文字配列）

L2 は `dungeon[][]` を直接操作せず、**ASCII 文字による事前生成配列 `char predungeon[40][40]`** を中間表現として使う。

| 文字 | 意味 |
|---|---|
| `' '`（空白） | 未確定（生成前） |
| `'.'` | 床（open floor） |
| `'#'` | 壁 |
| `'D'` | ドア候補 |
| `','` | 廊下 |
| `'A'`〜`'E'` | 部屋コーナーマーカー（後で `'#'` に変換） |

#### 部屋生成: `CreateRoom()` ＋ 再帰4分割

1. **エントリポイント**: `CreateDungeon()` が `CreateRoom({2,2}, {DMAXX-1, DMAXY-1}, 0, None, size)` で全体を再帰の起点にする（コード確認済: `drlg_l2.cpp:2461`）。
2. **終了条件**: `nRoomCnt >= 80`（部屋数上限）または残り領域が `AreaMin=2` 以下（コード確認済: `drlg_l2.cpp:1769`）。
3. **部屋サイズ**: `RoomMin=4` 〜 `RoomMax=10` の範囲でランダム生成（コード確認済: `drlg_l2.cpp:1775-1781`）。
   - 領域幅/高さが `RoomMin(4)` を超えるときのみランダム。4以下なら領域サイズをそのまま使用。
   - 最大部屋数: **80**（`RoomList[81]`、コード確認済: `drlg_l2.cpp:48`）。
4. **位置決め**: 部屋は領域内のランダム座標に置き、座標は `[1, 38]` に clamp される（コード確認済: `drlg_l2.cpp:1800-1803`）。
5. **`DefineRoom()`** で部屋を predungeon に書き込み、`RoomList` に登録、4コーナーにマーカー文字を付与。
6. **再帰4分割**: 部屋を置いた後、**残り4つの部分空間**（部屋の高さ > 幅 か否かで分割方向が変わる）それぞれに対して `CreateRoom()` を再帰呼び出し（コード確認済: `drlg_l2.cpp:1851-1861`）。各再帰呼び出しには親部屋 id `nRid` と接続方向 `HallDirection` を渡す。

**クエスト固定室**: レベル5（Q_BLOOD: 14×20）、6（Q_SCHAMB: 10×10）、7（Q_BLIND: 15×15）で `size` パラメータを固定値に設定（コード確認済: `drlg_l2.cpp:2444-2459`）。

#### 廊下生成: `HallList` への追加 → `ConnectHall()` で実引き

1. `CreateRoom()` が親部屋 `nRDest` を持つとき、**2端点（beginning, end）と方向を持つ `HallNode`** を `HallList` に push する（コード確認済: `drlg_l2.cpp:1846`）。
2. `CreateDungeon()` がすべての再帰を終えた後、`HallList` を順に消費して `ConnectHall()` を呼ぶ（コード確認済: `drlg_l2.cpp:2463-2466`）。
3. **`ConnectHall()` の経路引き**: 目的地に向かって1ステップずつ進みながら、距離に応じた確率で方向転換する **steering アルゴリズム**（コード確認済: `drlg_l2.cpp:1878-1976`）。
   - `nDx > nDy` のとき `min(2*nDx, 30)%` で水平方向へ、`nDy >= nDx` のとき `min(5*nDy, 80)%` で垂直方向へ転換。
   - 廊下は直線でなく、少し曲がりながら目的地へ到達する。
   - 廊下幅の両側拡張: `fMinusFlag` / `fPlusFlag`（各50%確率）が立つと片側に `PlaceHallExt()` で追加床を置く（コード確認済: `drlg_l2.cpp:1869-1870, 1901-1912`）。

#### ドア配置: `CreateDoorType()`

- `ConnectHall()` は経路開始点・終了点と、廊下が壁 `'#'` に当たる場所に `CreateDoorType()` を呼ぶ（コード確認済: `drlg_l2.cpp:1736-1750`）。
- **既に隣接してドア `'D'` があればスキップ**し、コーナーマーカー（`'A'`〜`'E'`）位置にもドアを置かない。
- 後処理 `FixDoors()` でタイルID 4→7、5→9 などに修正（コード確認済: `drlg_l2.cpp:2630-2642`）。
- `FixLockout()` で壁で挟まれた通路に必ずドアが1つ入るよう保証（コード確認済: `drlg_l2.cpp:2566-2628`）。

#### 空隙埋め: `FillVoids()`

- すべての廊下・部屋生成後、**空白タイル数（未確定 `' '`）が 700 以下になるまで**、最大100回の試行で空白を壁で埋める（コード確認済: `drlg_l2.cpp:2381-2438`）。
- 700 以下にならなければ `CreateDungeon()` は `false` を返し、`GenerateLevel()` のループで**再生成**（コード確認済: `drlg_l2.cpp:2492-2494`）。

#### 再生成ループと成功条件

`GenerateLevel()` は `while(true)` で回り、以下が揃うまでリトライする（コード確認済: `drlg_l2.cpp:2679-2693`）:
- `CreateDungeon()` が `true`（FillVoids 成功）
- `PlaceStairs()` が `true`（階段設置成功）

#### miniset fixup（VARCH / HARCH / CTRDOOR / BIG など）

`GenerateLevel()` 内で以下の miniset を `PlaceMiniSetRandom()` で適用（コード確認済: `drlg_l2.cpp:2700-2813`）:

| 種別 | 定数名 | 適用確率 | 目的 |
|---|---|---|---|
| ドア飾り | `CTRDOOR1`〜`CTRDOOR8` | 100% | ドア位置のタイル変換 |
| 縦アーチ | `VARCH1`〜`VARCH40` | 100% | 縦壁のアーチ装飾（40バリエーション） |
| 横アーチ | `HARCH1`〜`HARCH40` | 100% | 横壁のアーチ装飾（40バリエーション） |
| 柱 | `CRUSHCOL` | 99% | 崩れ柱の設置 |
| 大物装飾 | `BIG1`〜`BIG10` | 3〜20% | 大型オブジェクト（油溜まりなど） |
| 珍しい装飾 | `PANCREAS1`, `PANCREAS2` | 1% | レア装飾 |

- アーチ miniset は最も数が多く（縦横各40種）、既存ドキュメントで参照されている "探索置換" の実例。
- `DoPatternCheck()` でも `Patterns[100][10]` テーブルによる後処理を全タイルに実行（コード確認済: `drlg_l2.cpp:1979`）。

#### テーマ部屋

`DRLG_PlaceThemeRooms(6, 10, 3, 0, false)` でテーマ部屋を配置。引数: 最小サイズ=6, 最大サイズ=10, 深さ=3（コード確認済: `drlg_l2.cpp:2699`）。

**SBOP2 移植メモ**: L2 の核心は「再帰4分割でランダム矩形を置き、HallList にキューされた廊下をステアリングで繋ぐ」方式。predungeon 中間配列を用いた2パス設計は、抽象生成→タイル変換の分離を実装する際の良い参考になる。

### L3 Caves（再帰ブロック＋ラフな縁）— 有機的洞窟

（コード確認済: `drlg_l3.cpp` `CreateBlock()`:755, `FillRoom()`:713, `GenerateLevel()`:1987）

- **`GenerateLevel()` の初期化**: `GenerateRnd(20)+10` でランダム位置を決め、`FillRoom(x1, y1, x1+2, y1+2)` で **3×3 の起点ブロック**（端を含めると4×4だが有効内部は3×3）を1つ置く。その4辺に対し `CreateBlock` を呼ぶ。
- **`CreateBlock(point, obs, dir)` の挙動**:
  1. 新ブロックの各辺サイズを `RandomIntBetween(3, 4)` で **3〜4** で選ぶ（コード確認済: `drlg_l3.cpp:762-763`）。
  2. 入ってきた辺（`obs`）に対してランダムに位置合わせ。
  3. `FillRoom` が範囲外 or 重複なら終了（スペースなし）。
  4. **内部は常に `dungeon[i][j] = 1`（床）**、**縁の各タイルは `!FlipCoin()` = 1/2確率（50%）で床**（コード確認済: `drlg_l3.cpp:736-749`、`random.cpp:105-109`）。
  5. **`FlipCoin(4)` = 1/4確率でここで打ち切り**（コード確認済: `drlg_l3.cpp:823`）、成功したら来た辺以外の3辺に対して再帰。
- ループは **`while(true)` で無限リトライ**（終了条件: `GetFloorArea() >= 600` かつ `Lockout()` 成功、コード確認済: `drlg_l3.cpp:2016`）。
- 小さめブロック＋荒い境界 → 有機的な洞窟形状になる。

### L4 Hell（ミラーで対称化）

（コード確認済: `drlg_l4.cpp` `FirstRoom()`:261, `MirrorDungeonLayout()`:300, `PrepareInnerBorders()`:869, `GenerateLevel()`:1141）

- 初期部屋サイズは `GenerateRnd(5) + 2` で **2〜6** のランダム値（クエスト有りは11×11固定）。※ 初版の「5〜6」は誤り（コード確認済: `drlg_l4.cpp:270-272`）。
- 生成範囲を **第1象限 20×20（DMAXX/2 × DMAXY/2）に制限**して `FirstRoom()` → `GenerateRoom()` で budding。最小面積 `Minarea=692`（第1象限分×4換算）以上になるまで `do-while` リトライ（コード確認済: `drlg_l4.cpp:1149-1155`）。
- **`CloseOuterBorders()`** で外縁1マスを壁で閉じてから面積チェック（コード確認済: `drlg_l4.cpp:1154`）。
- **`PrepareInnerBorders()`**: 第1象限の内側（中央寄り）に**横通路1本 + 縦通路1本**を通す。各通路はランダムな y/x 行を選び、その行の右端まで未確定セルをマスクに追加して通路を確保。これによりミラー後に中心十字の通路が生まれる（コード確認済: `drlg_l4.cpp:869-935`）。
- **`MirrorDungeonLayout()`**: `DungeonMask` の第1象限 `(x, y)` を残り3象限 `(x, DMAXY-1-y)` / `(DMAXX-1-x, y)` / `(DMAXX-1-x, DMAXY-1-y)` にコピー → 地獄特有の**4象限対称**レイアウト（コード確認済: `drlg_l4.cpp:300-311`）。
- **ボス部屋（Diablo）**: lv16 では `FirstRoom()` 内で `L4Hold` に部屋位置を記録。`ProtectQuads()` で4象限の対応領域を全て `Protected` にし、`LoadDiabQuads()` でハードコードされた `.dun` ファイル（`diab1.dun`〜`diab4.dun`）を4箇所に配置（コード確認済: `drlg_l4.cpp:948-977`）。
- **ペンタグラム**: lv15 では `Q_DIABLO` クエストが閉鎖状態の場合 `L4PENTA` miniset をクエスト位置に配置（コード確認済: `drlg_l4.cpp:1198-1200`）。
- **後処理**: `GeneralFix()` → `DRLG_PlaceThemeRooms(7, 10, 6, 8, true)`（lv16 以外）→ `ApplyShadowsPatterns()` → `FixCornerTiles()` → `Substitution()`（コード確認済: `drlg_l4.cpp:1183-1191`）。

**SBOP2 移植メモ**: ミラー方式は実装コストが低く（第1象限だけ生成してコピー）、対称の美しいマップが生まれる。SBOP2 の管理側フロアや特殊エリアに応用できる。`PrepareInnerBorders()` で中心通路を明示的に作ることで孤立を防ぐ点も参考になる。

---

## 共通テクニック（ここが移植の肝）

### セットピース（pre-authored .DUN 部屋）
- **手作りの固定部屋データ**を、生成したダンジョンの所定位置に「スタンプ」する仕組み。クエスト部屋などに使う。
- 置いた部屋は `Protected` ビットセットで保護領域となり、後段の壁追加・miniset から守られる（コード確認済: `gendung.h` `Miniset::matches()`:269）。

### miniset（探索置換 = find & replace）

（コード確認済: `gendung.h` `struct Miniset`:259, `PlaceMiniSet()`:316）

- `struct Miniset` は `WorldTileSize size`（最大6×6）、`uint8_t search[6][6]`、`uint8_t replace[6][6]` の3フィールド。
- `matches()` でパターン一致確認、`place()` で置換を適用する。
- `PlaceMiniSet(miniset, tries=199)` はデフォルト199回試行してランダム位置に置く。
- 用途:
  - 階段など大きなオブジェクトの設置
  - 繋がりの悪いタイル組み合わせの修正（fixup）
  - タイルにランダムなバリエーションを足す
- 制約付き: `Protected` タイルと重ならない／常に適用するものと確率的に適用するものがある。

### theme room（テーマ部屋）
- 壁とドアで囲まれた小空間に、**決まった種類のオブジェクトをランダム配置**する。図書室・拷問部屋などの「味」を後付けする層。

### connectivity「Lockout」チェック → 再生成

（コード確認済: `drlg_l3.cpp` `Lockout()`:1900, `LockRectangle()`:1886, `GenerateLevel()`:1992-2024）

- `Lockout()` / `LockRectangle(x, y)` の2関数で実装。`LockRectangle` は再帰フラッドフィルで到達可能セルを `DungeonMask` から消しながら `lockoutcnt` をカウント。全床セル数 `t` と `lockoutcnt` が一致すれば全連結（`return t == lockoutcnt`）。
- 失敗時は **`while(true)` ループで無制限にリトライ**。リトライ上限はなく、成功するまで `GenerateLevel()` 全体を繰り返す。
- L3 では床面積 `GetFloorArea() < 600` でも再生成（面積不足）（コード確認済: `drlg_l3.cpp:2016`）。

### tile variants（タイル差し替え）
- 一部タイルを同義のバリエーションに置換。よく出るタイルほどバリ数を多くして単調さを崩す。

---

## SBOP2 への移植マッピング

| Diablo の概念 | SBOP2 での対応 | 備考 |
|---|---|---|
| 40×40 論理グリッド | `CInfoMapBase` のタイル配列（`m_pwMap` 等） | サイズは自由に決める |
| 事前生成（抽象: 床/壁/ドア） | 生成時に「床/壁」の中間配列を作る | 実タイルIDと分離するのが重要 |
| タイル変換（抽象→実タイル） | 中間配列 → マップパーツID へ変換 | 既存のマップパーツ（auto-tile的な配置）に割当 |
| **セットピース（.DUN）** | **管理画面で手作りしたマップ片を「部屋」として登録し、生成時に埋め込む** | ★最重要。後述 |
| miniset（探索置換） | 階段/ポータル/装飾の設置・繋ぎ目修正 | ポータルは既存 MAPMOVE イベントを置けばよい |
| theme room | ゾーンごとの装飾・敵セット | スポーナー配置と相性◎ |
| connectivity Lockout | 生成後フラッドフィル→不通なら再生成 | まずこれを入れるのが堅牢で安い |
| tile variants | マップパーツのランダム差し替え | 単調さ回避 |

### ★ 最大の収穫: 「セットピース」が"魂"と自動生成の橋渡しになる
Diablo は **完全自動ではなく、手作りの部屋（セットピース）を procedural な骨格に埋め込む**ハイブリッド。
これは SBOP2 の二層構想（A:自動ループ / C:みんなで手作りする永続世界）を**1つのダンジョン内で両立**させる答えになる:

- **骨格（通路・部屋の繋がり）= 自動生成**（あなたの作業ゼロで無限に変わる）
- **作り込み部屋 = 管理画面で手描きした map 片をセットピースとして登録**（"魂"の部分を温存・流用）

つまり「自動生成だけど、ところどころに自分（やいつかの仲間）が作ったこだわり部屋が混ざる」ダンジョンにできる。完全自動生成にはしない方針とも噛み合う。

---

## SBOP2 向け 推奨アプローチ（段階的・小さく）

1. **まずは Caves 方式（L3）から**: `CreateBlock` 相当の「中央から再帰ブロック＋縁50%床」が最も実装が短く、洞窟っぽい見栄えが出る。fixup を最小にできる。
2. **connectivity チェック → 再生成**を必ず入れる（フラッドフィルで全床到達を確認、ダメなら振り直し）。これだけで「行き止まりで詰む」事故が消える。
3. **入口/出口を miniset 的に設置**: 既存の MAPMOVE ポータルを、生成後に「床として開いている場所」へ置く。
4. **スポーナー・ドロップは既存の仕組みに乗せる**: 生成した床にスポーナー（`CHARMOVETYPE_PUTNPC`）を撒くだけ。→ Phase A のループがそのまま回る。
5. **後で セットピース対応**: 管理画面で作ったマップ片を「部屋テンプレ」として読み込み、生成骨格に埋め込む。ここで"魂"が乗る。
6. **さらに後で 部屋＋出芽（L1）/ ミラー（L4）** など別テーマを足してバリエーション化。

> 注意: Diablo の生成はサーバ/クライアントどちらで回すかを SBOP2 の同期方式に合わせる必要あり。SBOP2 はサーバ主導でマップ配信しているので、**生成はサーバ側で行い、結果を既存のマップ配信経路で送る**のが筋が良い（要設計）。

---

## コード裏取り完了済み一覧

| 調査項目 | 結果 | ソース |
|---|---|---|
| グリッドサイズ | DMAXX=40, DMAXY=40（マクロ定義） | `gendung_defs.hpp:5-6` |
| L3 再帰ブロック関数名 | `CreateBlock(Point, int obs, int dir)` | `drlg_l3.cpp:755` |
| L3 ブロックサイズ範囲 | `RandomIntBetween(3, 4)` → **3〜4** | `drlg_l3.cpp:762-763` |
| L3 縁が床になる確率 | `!FlipCoin()` = **50%（1/2）** | `drlg_l3.cpp:736`, `random.cpp:105` |
| L3 打ち切り確率 | `FlipCoin(4)` = **25%（1/4）** | `drlg_l3.cpp:823` |
| L3 起点ブロック | 3×3有効内部（`x1+2, y1+2`で端含め4点） | `drlg_l3.cpp:1995-1999` |
| connectivity 関数名 | `Lockout()` + `LockRectangle()` （再帰フラッドフィル） | `drlg_l3.cpp:1886,1900` |
| connectivity リトライ上限 | **上限なし**（`while(true)` で無制限） | `drlg_l3.cpp:1992` |
| L3 面積足切り | `GetFloorArea() < 600` で再試行 | `drlg_l3.cpp:2016` |
| L1 背骨関数名 | `FirstRoom()` | `drlg_l1.cpp:504` |
| L1 背骨チェンバー | 10×10 × 最大3個（各50%確率） | `drlg_l1.cpp:516-518` |
| L1 budding 関数名 | `GenerateRoom(rectangle, verticalLayout)` ※ `L5roomGen` は誤り | `drlg_l1.cpp:456` |
| L4 初期部屋サイズ | `GenerateRnd(5)+2` → **2〜6**（クエスト有: 11×11固定） ※「5〜6」は誤り | `drlg_l4.cpp:270-272` |
| L4 ミラー処理 | あり。`MirrorDungeonLayout()` で第1象限→4象限 | `drlg_l4.cpp:300` |
| miniset 構造体 | `struct Miniset { size, search[6][6], replace[6][6] }` | `gendung.h:259` |
| L2 事前生成配列 | `char predungeon[40][40]`（ASCII 中間表現） | `drlg_l2.cpp:51` |
| L2 部屋生成関数名 | `CreateRoom(topLeft, bottomRight, nRDest, nHDir, size)` | `drlg_l2.cpp:1766` |
| L2 部屋数上限 | `nRoomCnt >= 80`（`RoomList[81]`） | `drlg_l2.cpp:48, 1769` |
| L2 部屋サイズ範囲 | `RoomMin=4` 〜 `RoomMax=10`（clamp [1,38]） | `drlg_l2.cpp:1775-1803` |
| L2 廊下引き関数名 | `ConnectHall(HallNode)` ＋ `HallList` キュー | `drlg_l2.cpp:1864` |
| L2 廊下 steering 確率 | 水平 `min(2*nDx,30)%`、垂直 `min(5*nDy,80)%` | `drlg_l2.cpp:1923-1935` |
| L2 廊下幅拡張 | `fMinusFlag`/`fPlusFlag` 各50%で片側拡張 | `drlg_l2.cpp:1869-1912` |
| L2 ドア配置関数名 | `CreateDoorType()` → `FixDoors()` → `FixLockout()` | `drlg_l2.cpp:1736, 2630, 2566` |
| L2 空隙足切り | 空白 `' '` タイル数 > 700 で `FillVoids()` 埋め（失敗で再生成） | `drlg_l2.cpp:2381-2437` |
| L2 再生成ループ | `CreateDungeon()` + `PlaceStairs()` 両方成功まで `while(true)` | `drlg_l2.cpp:2679-2693` |
| L2 クエスト固定室 | lv5: 14×20, lv6: 10×10, lv7: 15×15 | `drlg_l2.cpp:2444-2459` |
| L2 アーチ miniset 数 | VARCH×40 + HARCH×40（合計80種、全100%適用） | `drlg_l2.cpp:67-1167` |
| L2 テーマ部屋 | `DRLG_PlaceThemeRooms(6, 10, 3, 0, false)` | `drlg_l2.cpp:2699` |
| L1 GenerateRoom 停止条件 | `CheckRoom()` が両方失敗で自然終了（深さ上限なし） | `drlg_l1.cpp:456-498` |
| L1 部屋サイズ | `(GenerateRnd(5)+2) & ~1`（2〜6の偶数） | `drlg_l1.cpp:465-466` |
| L1 最小面積閾値 | lv1: 533, lv2: 693, lv3-4: 761 | `drlg_l1.cpp:1168-1178` |
| L1 チェンバー接続 | `FillChambers()` → `GenerateChamber()` + `GenerateHall()` | `drlg_l1.cpp:987, 755, 811` |
| L1 壁後処理 | `AddWall()` で Corner/HWallEnd 起点に `HorizontalWall()`/`VerticalWall()` | `drlg_l1.cpp:702-753` |
| L4 最小面積閾値 | `Minarea=692`（第1象限×4換算） | `drlg_l4.cpp:1149` |
| L4 外縁閉じ | `CloseOuterBorders()` を面積チェック前に実行 | `drlg_l4.cpp:1154` |
| L4 中央通路 | `PrepareInnerBorders()` で横1本＋縦1本を第1象限に確保 | `drlg_l4.cpp:869-935` |
| L4 ミラー処理詳細 | `DungeonMask` コピーで4象限対称化 | `drlg_l4.cpp:300-311` |
| L4 ボス部屋 | lv16: `LoadDiabQuads()` で `diab1〜4.dun` を4箇所配置 | `drlg_l4.cpp:960-982` |
| L4 Protected | `ProtectQuads()` で lv16 ボス部屋を4象限全て保護 | `drlg_l4.cpp:948-958` |

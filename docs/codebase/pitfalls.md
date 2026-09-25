# 実装・ビルドの落とし穴集

このファイルは過去に実際に踏んだ落とし穴の集約。新しい落とし穴を見つけたら該当節に追記する。
関連箇所を触る前に一度確認すると、同じ調査を繰り返さずに済む。

## 文字列・エンコード

- **`Format(_T("%s"), wideArg)` はネイティブとブラウザ版で解釈が違う。** MSVC のワイド書式 `%s` は `wchar_t*` を期待するが、Emscripten(POSIX/musl) 側のワイド書式 `%s` は `char*` を期待し、ワイドは `%ls` になる。`CmyString::Format(_T("%s..."), (LPCTSTR)wideArg)` はネイティブで動いてブラウザ版だけ空文字列になる（実例: `ImGuiMsgLog::Add` がチャットログで `len=0` になり早期 return）。対処は文字列連結で組むか `%ls` を使うこと。ブラウザ版で「文字列がなぜか空/化ける」時はまずこのパターンを疑う。

- **`WideCharToMultiByte` のブラウザ版スタブが codepage を無視して常に UTF-8 を返す実装だった時期がある（UTF-8 移行前の話。現在は通信・DB・ログとも UTF-8 に統一済みで、この経路自体は解消済み）。** `SboCli/src/Platform/TCharCompat.h` のスタブ経由で `(LPCSTR)m_strCharName` が UTF-8 バイト列を返し、サーバの `LegacyAnsiToTString` が SJIS と誤読してキャラ名が文字化けした。修正は codepage=932/0 のとき `WstringToSjis`（`SboCli/src/Platform/SjisConvert.cpp`）を呼ぶ分岐。SJIS→UTF-8 の逆変換は `utf8_to_sjis_js`（EM_JS）が `TextDecoder('shift-jis')` で全 SJIS バイト列を舐めて Unicode→SJIS の逆引きマップ（`Module._sjisReverseMap`）を初回構築する（マップに無い文字は `?` で代替）。

- **`IsDBCSLeadByte`（`SboCli/src/Platform/Win32ApiStubs.h`）のブラウザ版スタブは常に FALSE を返す。** Emscripten には ACP の概念が無いため。`NameCheck`（`Common/LibInfo/LibInfoCharBase.cpp`）のような「DBCS 先頭バイトを許す」ロジックがブラウザ版で全て SJIS 多バイト文字を弾く原因になっていた。この API 単体に頼らず、SJIS DBCS 範囲 (0x81-0x9F, 0xE0-0xFC) の明示的な範囲チェックを併用する（`NameCheck` で修正済みのパターンを参照）。

- **`SjisToUtf8` の同名関数二重定義に注意。** `Common/myLib/myString.h` の inline 定義は `!_WIN32`（ブラウザ版）では `result=pszSrc` の無変換パススルーで、文字化けの原因になる。正しい変換が必要な箇所は別名の `SjisBytesToUtf8`（`SboCli/src/Platform/SjisConvert.cpp`、include は `Platform/SjisConvert.h`）を使う。`SboCli/` 配下だけ grep すると見落とすので、`Common/` 側の定義も確認する。

- **ColorKey に RGB(0,0,0) を使っているウィンドウ（`CWindowCHARNAME` 等）で文字色を黒(0,0,0)にすると、文字本体が透過して縁取りだけ残る。** `m_pDib->SetColorKey(0)` が RGB(0,0,0) を透過対象にするため。SetColorKey(0) のウィンドウで「黒文字」を出す時は `RGB(1,1,1)` を使う（`WindowLOGINMENU` 等の既存慣例）。ウィンドウの雛形を `CWindowSEX` 等からコピーする時は文字色もこの値に直すこと。

- **黒背景＋カラーキー0の DIB に SDL_ttf で文字を描くと、縁取り・アンチエイリアスの暗いピクセルが透過して欠ける（`WindowTEXTMSG` の Web 版対応で実例）。** 下地は枠の背景色（`CWindowBase::GetFrameBackColor`）で塗ってから文字を描くこと。不透明にした DIB は枠線を上書きするので、Blt 範囲を枠の内側に収める必要がある。

- **`CmyString::GetLength()` はワイド文字数を返す。** UTF-8 のバイト位置計算をする時は `strlen((LPCSTR)str)` を使うこと（`WindowTEXTMSG` の会話文字列処理で実際に踏んだ）。

- **ソース文字コードと SJIS 誤読 (C1020 / C4819)。** `Common/UraraSockTCP.h` のような UTF-8 BOM 無しファイルの日本語コメントを、`/utf-8` オプション無しでビルドするプロジェクト（SboSockLib）が SJIS(CP932) として誤読し、日本語の途中バイト(0x5C)を行継続文字と解釈 → 直後の `#endif` が飲み込まれ C1020 が出た（修正: `7864168`、ファイル先頭に UTF-8 BOM を付加）。`warning C4819` がヒント。SboCli は `/utf-8` 付きビルドのため問題にならないが、同じヘッダを別プロジェクト（Common 配下等）から include すると再発しうる。

- **UTF-8 完全移行後の前提（2026-06-23, `ad93086`/`2898e5a`/`56d6b41`/`a2de14c`）。** DB 保存・通信パケット・ログ出力の3経路はすべて UTF-8 に統一済み。`CmyString` の `operator LPCSTR()` は `GetUtf8Pointer()` を返すので `(LPCSTR)cmystr` は安全。クライアント/サーバのバイナリは同時更新が必須（古い CP932 クライアントと混在させない）。撤去済みシンボル（`GetLegacyAnsiPointer`/`GetLegacyStoreLength`/`TStringToLegacyAnsi`/`LegacyAnsiToTString`）は復活させないこと。クライアント側の描画/表示（`LayerSystemMsg`/`SdlFont`/`ImGuiMsgLog`/`GdiStubs`）にも「LPCSTR=SJIS」前提が残っていたため、`SjisToWstring`/`SjisBytesToUtf8`/CP_ACP 経路を UTF-8 直接処理に置換した。UTF-8化の影響範囲を見積もる時は `SboCli/src/Platform/` も必ず当たること。

## ブラウザ版クライアント

- **ブラウザ版の実質メイン画像ローダーは `CMgrGrpData::Load()`（`SboCli/src/MgrGrpData.cpp`）ではなく `LoadLocalTitleAssets()`（同ファイル、`MainFrame.cpp` からフォールバックで呼ばれる）。** ブラウザ版は `SDL_LoadObject("SboGrpData.dll")` に失敗して `Load()` が即 return するため、`LoadLocalTitleAssets()` が全画像をファイルから読む。**`Load()` に画像種別を追加しても `LoadLocalTitleAssets()` に同じロードループを足さないと、ネイティブでは出るのにブラウザ版だけ画像が欠落する**（`GetDibXxx` が NULL を返し描画が静かにスキップされるだけでクラッシュしない）。実例: 撃破時の煙エフェクト(`IDP_EFC_32_*`/`IDP_EFC_64_*`)、16px NPC シート(`IDP_NPC_01`/`IDP_NPC_02` → `m_paImgNPC`、得点NPC `CHARMOVETYPE_SCORE` 表示に必須)。未確認の欠落候補として `IDP_WEAPON_*` や吹き出し(balloon)も疑ってよい。診断は `SboDbgLog` で `GetEfcImg grpMain=4 pRetNull=1` のようなログを仕込むと特定できる。

- **DOM オーバーレイの座標系は報告座標と一致しない。** Window 系の描画は `pDst->Blt(m_ptViewPos+32, ...)` するが、canvas 上の実描画位置は `m_ptViewPos` そのまま（報告座標-32）。DOM オーバーレイを canvas に重ねる時は `(state.windowX - 32) * scaleX` で配置すること。IME 候補ウィンドウは `#sbop2ImeInput`（1x1透明・(0,0)固定）を `placeImeInputForCandidates()` でアクティブ入力欄直下へ動かす。`.login-input` は content-box なので box-sizing:border-box が必要。検証は canvas を `drawImage` で同期コピーし、特定色の bbox を検出すると座標を実測できる。

- **X キー2回押し問題（`MainFrame.cpp` のキー振り分け）。** ウィンドウ経路で DOWN を処理した keycode を `m_byWindowHandledDownCode` に記録して StateProc への UP 漏れを防ぐ保護ロジックがあるが、ウィンドウ経路の UP 受信時に記録をクリアしていなかった。メニュー X UP で別状態へ遷移するケースで次の X UP が StateProc に届かず2回押しが必要になっていた。修正は `else if (byCode == m_byWindowHandledDownCode) { m_byWindowHandledDownCode = 0; }` をウィンドウ側 UP 処理に追加。

- **SDL2 (Emscripten) は既定で window レベルにキーボードリスナを張り `preventDefault()` で全キーイベントを奪う。** そのため canvas 外の DOM `<input>` にフォーカスがあってもキー入力が届かない。`SDLApp.cpp` に `SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas")` を `#ifdef __EMSCRIPTEN__` で追加して解消（コミット `0705918`）。

- **リポジトリルートに置いた Windows 用スタブ `emscripten/emscripten.h` が、`-I .` により em++ のインクルードパスでも本物の Emscripten ヘッダより先に読まれ、`emscripten_set_main_loop_arg` が空の `static inline` 実装にインライン化されてメインループが潰れた。** shell.html の `setInterval` フォールバックだけで動く状態（`ML/s:0`）になっていた。修正（コミット `26025e2`）は `#ifdef __EMSCRIPTEN__` で分岐し、Emscripten ビルド時は `extern` 宣言のみにして本物のヘッダへ委譲する。同名ディレクトリにスタブを置く場合は必ずこの分岐を入れること。

- **ウィンドウ内にインラインのテキスト入力欄を埋め込む制約（新規キャラ作成画面 `CWindowCHARNAME`/`CWindowSTYLESELECT` で実例）。** SDL のキー/テキストイベントは `CMgrWindow::GetActiveWindow()` の1枚にしか配られず、`CMgrWindow::SetActive()` は `IsInput()==TRUE` の最前面1枚だけをアクティブにするため、子ウィンドウを出すと親がキー入力を失う。解法: 子は `m_bInput=FALSE`/`m_bActive=FALSE` 固定にして SetActive 対象から外し、専用フラグ `m_bFocused`（`IsFocused()`）でフォーカス管理、親が `HandleSDLKeyDown/TextInput/TextEditing` をオーバーライドしてカーソル位置に応じて子へ転送する。子の `m_strName` は Enter 確定時にしか親へ流れないため、決定処理の頭で毎回子から引き取ること。描画順の罠: `MakeWindowXXX()` は `Create()`→`Add()` の順なので、親の `Create()` 内で作った子は親より先に `m_paWindow` へ Add され、親が後から描いて子を覆い隠す。埋め込み入力欄の canvas 描画は親が担当すること。

## ビルド・配信

- **`tools/build-sbocli-browser-title.ps1` の `-SkipPreflight` はコンパイル自体を飛ばす。** `test-sbocli-browser-preflight.ps1` が各 `.cpp` を em++ でコンパイルする本体で、em++ リンク段は既存 `.o` をリンクするだけ。`-SkipPreflight` を付けるとソース編集が反映されず古い `.o` がリンクされる。確実に反映するには emsdk の `emsdk_env.ps1` を読み込んだ状態で `-SkipPreflight` を付けずに実行する（必要なら `-Force`）。emsdk env 未読み込みだとプリフライトが cl.exe フォールバックに落ち、リポジトリに無い `SBOP2\SDL2\include` を探して `SboCli_priv.h(385): fatal error C1083: 'SDL.h'` が出る（ソースの問題ではない）。ビルドを他の担当や自動化タスクに依頼する時は、emsdk env 読み込みを含めたコマンドを丸ごと渡すこと。同じビルドを複数箇所で同時に走らせると `.o` を取り合うので、ビルド担当は1つに絞る。

- **ビルドスクリプトは完了時に `SboSvr\Debug\webroot\game` / `Release\webroot\game` へ自動コピーする（`[browser-deploy]`）。** SboSvr を再ビルドしなくても、ローカルサーバーは新しい wasm を配信する。反映確認は配信物のファイルサイズ等を突き合わせるとよい。

- **`printf` はブラウザの devtools コンソールに出ない。** Emscripten では printf 出力がコンソールに流れないため、診断ログは `SboDbgLog`(=`SDL_Log` マクロ, `SboCli_priv.h:386`) を使う。コンソールは shell.html の `<textarea id="output">` にもミラーされる。

- **ブラウザ版のソース一覧は `.vcxproj` とは別管理（`tools/browser-sources.txt`）。** 共有ファイル（`Common/` や `SboCli/src/Platform/`）を移動・改名すると、ネイティブビルドとテストが全部通ってもブラウザ版だけ壊れる（実例: `SboCli/src/Platform/SjisConvert.cpp` → `Common/Platform/` 移動時に `SboCli.vcxproj` は直したがブラウザ版の一覧（当時はスクリプトに直書き）を直し忘れ、`browser preflight failed: SboCli/src/Platform/SjisConvert.cpp` で落ちた）。共通コードやプラットフォーム層のファイルを動かしたら、必ずブラウザ版ビルドも回す。

- **cl.exe フォールバック用の Emscripten スタブは `tools/msvc-stubs/emscripten/` に置く（リポジトリルートの `emscripten/` に置くと `Get-IncludeArgs` 経由で em++ のインクルードパスにも入り本物のヘッダを shadow する）。** cl.exe は `_WIN32` を必ず定義するため `#if defined(_WIN32)...#else #include <emscripten/em_js.h>` を通れず `/FI` で em_js.h スタブを強制インクルードする。SDL_ttf.h は em++ では ports 供給だが cl.exe 分岐だけ `SDL2_ttf/include` を追加する必要がある。cl.exe の COFF `.obj` を `out/browser-preflight/` 直下に出すと em++ がリンク対象に拾ってしまうため `msvc-syntax-check/` サブディレクトリへ `.obj` 拡張子で退避する。

- **ブロックコメント `/* */` → `//` の機械置換ミス。** `SboSvr/src/MainFrame/MainFrame.cpp` と `SboSvr/src/UpdateServerInfo.cpp` で `/*=TRUE*/,` が `// =TRUE*/,` に化け、`//` が行末までコメント化して `,`/`)`/次行の引数を飲み込み `C2146`/`C3646`/`C2511` が発生した（修正: `ee2b6c3`）。`// .*\*/` で grep すると芋づる式に発見できる。SboSvr 以外にも残っている可能性がある。

- **データファイル不在を許容する設計。** `CMgrData::ReadHashList` が `Update/SBOHashList.txt` を `CTextInput::Create` で開こうとして失敗した際、戻り値を確認せず `GetLineCount`→内部で `SetLine(-1)`→`fseek(NULL,...)` でクラッシュした（修正: `d64fe31`、`CTextInput::SetLine`/`ReadProc` 冒頭に `m_pFile==NULL` ガード追加）。同様に SDL_ttf 移行後、`SboCli/Debug/font/NotoSansCJKjp-{Bold,Regular}.otf` を手動コピーしないと `TTF_OpenFont` が失敗して全テキスト描画が無効化される（`INFO: SdlFontCreate: TTF_OpenFont failed` がヒント）。恒久対応は `SboCli.vcxproj` の `PostBuildEvent` に `xcopy /y /d /i "font" "$(OutDir)font"` を追加（コミット `015c49c`）。

- **`Common/myLib/myZlib/zlibD.lib`（2008年ビルドの zlib）が `/manifestdependency:Microsoft.VC90.DebugCRT 9.0.21022.8` を embedded manifest にマージし、VC90 ランタイムが無い環境で SxS エラーで起動しない。** SboSvr は `SboSvr/SboSvr.manifest` を PostBuildEvent の `mt.exe -nologo -manifest ... -outputresource:"$(TargetPath)";#1` で上書き。SboCli も同様に `SboCli/SboCli.manifest`（BOM付きUTF-8、VC90依存なし）で対処した。「CRT 静的リンクだから SxS を回避できている」という推測は誤りだった実例あり。本来の対処は zlibD.lib/zlib.lib を VS2022 でリビルドすること。

- **`tools/publish.ps1` は staging 段階で `sbocli-title.html` に wss 書換シム（マーカー `/*__SBOP2_WSS_SHIM__*/`）を注入するが、事前圧縮版 `.br`/`.gz` はその前のブラウザビルドで作られている。** SboSvr は圧縮版を優先配信するため、放置すると本番ブラウザにシム無し HTML が届き HTTPS で wss 接続できない（2026-09-08 実発生、現地で `.br`/`.gz` を作り直して復旧）。対処: シム注入直後に stale な `.br`/`.gz` を削除→`tools/emscripten/precompress.mjs` で再生成→展開してマーカー有無を検証（`Test-CompressedContains`）。検証は `-SkipBrowserBuild -SkipServerBuild -NoZip -OutputDir <一時先>` で staging だけ回せる。

- **`SboSvr.vcxproj` は Release|Win32 が全体で `PrecompiledHeader=Use`、Debug は PCH 未使用。** StdAfx.h を include しない純粋関数ファイル（`MoveStateDecision`/`ProcessMetrics`/`SessionsJsonBuilder`）を追加すると Debug は通るが `publish.bat`（Release Rebuild）だけ `error C1010` で失敗し zip が作られない。対処: 該当 `<ClCompile>` に Debug/Release 両方の `PrecompiledHeader=NotUsing` を付ける（`#include "StdAfx.h"` を足すのは設計違反）。publish 失敗の調査はリポジトリ直下の `publish_build.log` を `error` で grep する。

- **`SboSvr.vcxproj` の PostBuild（`webroot` を rmdir→再作成）と `publish.ps1` が `out/browser-title/sbocli-title.*` しかコピーせず `BGM/` を落としていた。** 本番でBGMが鳴らない原因（2026-09-16）。両方に BGM コピーを追加して修正。「DL進捗が100%を超える」表示は file_packager の total が `.data.br` の圧縮後 Content-Length で loaded が展開後バイトのため（表示だけの問題）。`webroot/game` 配下に新ディレクトリを足したら PostBuild と `publish.ps1` の両方のコピー対象を確認すること。

- **複数エージェントが同じ vcxproj（例: SboSvr）を並行編集・ビルドすると、コンパイルがソース保存前に読み込み→中間 `.obj` 書き出しは保存後、というすれ違いで `.obj` の日時がソースより新しくなり msbuild が再コンパイルを飛ばす。** `/t:ClCompile` の個別確認では「通った」ように見えても最終リンクで `LNK2019`（追加したメソッドが obj に無い）のような形で破綻する。対処: コミット前に変更した `.cpp` の LastWriteTime を現在時刻に更新してから、リンクまで含む増分ビルドを1回だけ通す。可能なら同一プロジェクトの編集エージェントは直列にする。

- **SQLite DB（`SBODATA/SboData.db`）を稼働中にコピーすると、ファイルサイズが一致していても中身が全ゼロの破損ファイルになることがある。** `PRAGMA journal_mode=WAL; synchronous=NORMAL;`（`MgrData.cpp:193`）の DB をチェックポイント前に強制終了すると本体ファイルがゼロ化する。`OpenSboDb` が `sqlite3_exec` の戻り値を見ず、壊れた db でも true を返す（整合性チェック不在）ため、旧 CP932 の `.dat` seed へ静かにフォールバックし、半角は正常なのに日本語だけ文字化けする分かりにくい症状になる。切り分けは `head -c16` が `SQLite format 3` か・全ゼロ率を確認。コピー後はサイズ一致で安心せず中身（ヘッダ・非ゼロ）を検証すること。UTF-8 変換済み db に再度 SJIS→UTF8 変換をかけると二重エンコードで全壊するので、変換前に現エンコードを必ず確認する。

## Linux（CMake）ビルド

- **非Windows の `PlatformDefs.h` は `min`/`max` を関数形式マクロで定義する。** これより後に libstdc++ の `<algorithm>`・`<vector>`・`<deque>`・`<random>` などを読むと、`std::max(...)` や `__g.max()` がマクロ展開されて大量のエラーになる（em++/libc++ では表面化しなかった）。対処は「標準ヘッダを先に読む」こと。`SboSvr/src/Platform/SvrCompat.h` で主要な標準ヘッダを `PlatformDefs.h` より前に読み、`TCharCompat.h` は `CStringCompat.h`（標準ヘッダを読む）を先に include する順にしてある。新しい標準ヘッダを使って同じ系統のエラーが出たら、`SvrCompat.h` の一覧に足す。
- **`.vcxproj` の `ClCompile` は大文字小文字が実ファイルと違うことがある（例: `..\common\crc.cpp`）。** Windows では通るが Linux では見つからない。`cmake/VcxprojSources.cmake` は1階層ずつ大文字小文字を無視して実名に直すので `.vcxproj` 側は直さなくてよい。一方ソース中の `#include` の綴り違い（例: `LibInfoCharSVr.h`）は自動では直らないので、見つけたらソースを直す。
- **Winsock の書き方は POSIX でそのまま動かないものがある（コンパイルは通る）。** `select(0, ...)` は Windows では第1引数が無視されるが、POSIX では「最大 fd + 1」が必要で、0 のままだと永久にタイムアウトし HTTP が接続だけ受けて無応答になる。`SO_RCVTIMEO`/`SO_SNDTIMEO` に `DWORD`（ミリ秒）を渡すと POSIX では `timeval` との長さ不一致で失敗してタイムアウトが効かない（`SboPlatform::SetSocketTimeoutMs()` を使う）。`accept`/`getpeername` の長さは `socklen_t*`。
- **Linux のゲーム用 TCP ポート（既定 2006）は select 版（`SboSockLib/UraraSockTCPSelect.cpp`）で待ち受ける。** WSAAsyncSelect 版（`UraraSockTCP.cpp`）は Windows 専用のままで、`CUraraSockTCPSBO` が非Windows では常に select 版を選ぶ。`Common/UraraSockTCP.h` の `CUraraSockTCPStub` は何もしないスタブなので、非Windows でこれが返っていたら待ち受けていない。
- **SboSockLib は `StdAfx.h` に `using namespace std;` があるので、`<functional>` 系（`<thread>`・`<future>` などから間接的にも読まれる）を足すとソケットの `bind(...)` が `std::bind` に化ける。** 第3引数の `sizeof(addr)` は `size_t` なので、Winsock の `bind(SOCKET, const sockaddr*, int)` よりテンプレートの `std::bind` の方が一致度が高く選ばれ、`== SOCKET_ERROR` の比較でコンパイルエラーになる（実例: `UraraSockTCPSelect.cpp` に `<thread>`/`<future>` を足した時、Windows だけで発生。Linux は `StdAfx.h` の別分岐を通るので CI では出なかった）。ソケット API の `bind` は `::bind(...)` と書く。
- **POSIX では切断済みの相手へ `send` すると SIGPIPE でプロセスごと落ちる（Windows には無い挙動）。** `SboSvr`/`SboSvrTest` の `main()` で `SIGPIPE` を無視し、select 版とテスト用クライアントは `MSG_NOSIGNAL` も付けている。新しくソケットを扱う exe を足す時は同じ対処が要る。
- **非Windows のワイド書式 `%s`/`%c` は char 側を指す（MSVC は wchar_t 側）。** サーバーのコードは MSVC の意味で `Format(_T("%s"), (LPCTSTR)str)` と書いてあるため、Linux 向けには `CStringCompat.h` の `ConvertMsvcWideFormat()` が `%ls`/`%lc` に直してから `vswprintf` に渡す（Emscripten 版は対象外で、従来どおり `%ls` か連結で書く）。glibc の `vswprintf` も測定モードが無いので、同じ箇所で収まるまでバッファを広げて測っている。`CStringCompat` を経由しない `_stprintf` 系を直接使う箇所には効かないので注意。
- **Linux の CP932 変換は glibc の iconv（`SjisConvert.cpp`）で行う。** 空 DB で起動した時に読む旧 `.dat`（CP932）の日本語名もこれで正しく入る。変換できないバイト/文字は `?` に置き換える。
- **`CStaticFileHandler::ToUtf8()` は ASCII 以外を `?` にする表示用の関数。** ファイルパスの変換に使うと日本語ファイル名が開けない（実例: Linux 版 `StatFile` で使って「テスト.png」が 404）。パスには `WstringToUtf8()` を使う。
- **ブラウザ版の Linux ビルド（`tools/build-sbocli-browser-title.sh`）でも `#include` の大文字小文字が効く。** 実例: `LayerCloud.cpp` の `InfoCharCLI.h`（実名 `InfoCharCli.h`）。ps1 版のインクルードディレクトリ `Common/myLib/myZLib` も実名は `myZlib` なので、bash 版は実名で書いてある。
- **クラウドセッションでは Emscripten ports（SDL2・SDL2_ttf・freetype・harfbuzz・zlib）のアーカイブ取得が 403 になる**（`github.com/.../archive/...`・`releases/download` は許可されず、`git clone` は通る）。初回ビルド前に `tools/emscripten/prefetch-ports-via-git.sh` で git から取り込んでおく。GitHub Actions や手元では不要。
- **Linux の CMake ビルドは既定で `_DEBUG` なし（`/api/debug/fixture` が入らない）。** 自動確認（`tools/test-browser-e2e-linux.sh`）には `-DSBO_DEBUG_API=ON` で別ディレクトリにビルドしたサーバーが要る。このオプションはステージング・本番に絶対に使わない。
- **ステージング（`deploy/staging`）の Docker Compose では、`env_file` の値も `$` が変数展開される。** bcrypt ハッシュ（`$2a$14$...`）が壊れるので、`staging.env` は `format: raw` で読んでいる（`.env` という名前にすると compose.yaml の展開用にも読まれて警告が出るので避けた）。
- **ステージングの SboSvr は Caddy のネットワーク名前空間に入っている（`network_mode: service:caddy`）。** SboSvr を作り直すのは問題ないが、Caddy だけを作り直すと SboSvr のネットワークが切れる。その時は両方作り直す。
- **Google Cloud の VM では `~/.ssh/authorized_keys` に手で足した鍵が消える。** ブラウザの SSH で入ると GCP のゲストエージェントがこのファイルを書き直し、自分で足したデプロイ鍵が消えて Actions の SSH が `Permission denied (publickey)` になった(2026-09-25)。デプロイ鍵は VM の「編集 → SSH 認証鍵」(メタデータ)に登録する。鍵の末尾のコメントがログインユーザー名になる。
- **ブラウザの SSH 画面から秘密鍵を `cat` してコピーすると改行が崩れ、Actions で `Load key ...: error in libcrypto` になる。** `staging-deploy.yml` は `base64 -w0` の1行も受け付けるので、Secret にはその形で登録する。

## サーバー

- **SboSvr は起動時に DB をメモリへ読み込み、停止時にメモリ内容を DB へ書き戻す。** 稼働中に SQLite ファイルを直接 UPDATE しても、サーバー停止時に旧値で上書きされて巻き戻る（実例: uraran 等の MoveWait を稼働中に修正しても停止時に旧値で上書きされた）。DB を直接編集する時は必ず先にサーバーを停止すること。書き戻し経路はウィンドウ版が `OnClose→TermServer→OnDestroy→m_pMgrData->Save()`（`MainFrameWindow.cpp`）、ヘッドレス版が `--stop`/Ctrl+C→ループ脱出→`TermServer()`→`m_pMgrData->Save()`（`MainFrame.cpp`）で、他に30分毎の定期保存 `TimerProcSave`（`MainFrame.cpp`）がある。ウィンドウが画面上に見えない場所で起動していると `WM_CLOSE` が `OnClose` に届かず保存されないことがある（`SboSvr.ini` の `[Pos]` 更新日時が止まっているのが証拠）。自動化から起動・停止する時はヘッドレスモード（`--headless` 起動、`SboSvr.exe --stop` で停止）を使うこと。DB の実体は `SboSvr\Debug\SBODATA\SboData.db`。

- **`CLibInfoCharSvr::ProcLocalFlgCheck()`（`SboSvr/src/LibInfo/LibInfoCharSvr.cpp`）は状態変化フラグを巨大な `else-if` チェーンで1tickに1つだけ処理する。** 先頭に近い分岐ほど優先され、手前のフラグが立ちっぱなしだと後段の分岐（末尾の撃破消滅 DELETE 予約 `m_nReserveChgMoveState` 等）に永久に到達できない「飢餓」を起こす。実例（コミット `22e72d4`）: `TargetChar()` が索敵対象なし(`GetNearCharID==0`)で早期 return して `m_bChgTargetChar` をクリアし忘れ、ターゲット変更分岐が毎tick優先されて弓矢で倒した敵が消えなかった。「処理予約フラグ」は早期 return する経路でも必ずクリアすること。調査は `m_pMgrData->GetLog()->Write(...)` で該当フラグを定期ダンプすると絞り込みやすい。撃破消滅は SWOON→ProcSWOON の2段階（effectID=5 送出→200ms後 DELETE）。

- **サーバー側 `CInfoCharSvr::m_pInfoMap` は `SetMap()` でしか設定されず、呼ばれるのはスキル起動キャラ・アイテムで作ったキャラ・NPC発生の2箇所だけ。** DB から読み込まれたマップ配置の NPC（ボール CharID 989 等）は `m_pInfoMap` が NULL のまま。押し判定が `pInfoObj->m_pInfoMap` を使うと NULL→「塞がっている」扱いで最初の1pxから却下される（ログ `[理由:0:NONE][要求距離:1]`）。同様に `m_pLibInfoCharSvr` も NULL になりうる。サーバーで NPC のマップ判定をする時は `m_pLibInfoMap->GetPtr(pChar->m_dwMapID)` で `m_dwMapID` から引くこと。クライアントとサーバーで同じ判定関数を共用していても、渡すマップ取得方法が違うと食い違う。

- **`Common/Lib/TextOutput.cpp` の `CTextOutput::Write` は `char szTmp[256]` に `_vsnprintf` で書き、戻り値<=0 なら何も書かずに抜ける。** MSVC の `_vsnprintf` は入りきらないと -1 を返すので、255バイトを超える行は丸ごと黙って消える（クラッシュしない）。実例: 押し処理の診断ログで項目を増やした「押し要求を却下」ログだけ消え、「受信」「判定直前」（短い行）は出たため「処理がどこかで抜けている」と誤読して調査が一往復遠回りした（実際は毎回却下されていた）。長い診断ログを `m_pLog->Write` に足す時は1行255バイト以内に分割するか、TextOutput 側の入れ物拡大/切り詰め出力の修正を確認する。「出るはずのログが出ない」時はまず行の長さを疑う。

- **`RecvProcCHAR_MOVEPOS`（SboSvr）が受信パケットを `Set()` しただけの入れ物を `SendTo` していた。** `CPacketBase::Set` は読み取り専用で送信バッファ(`m_pPacket`)を作らないため、中身が空のまま送信される（エラーにならない）。他PCのカクつき・後退の真因はこれで、見る側は150ms間隔の定期POS_SYNC（`LibInfoCharSvr::Proc`）だけで先読みしていた。パケットを転送する時は必ず `Make()` し直すこと。

- **マップ自動生成で「全床が1連結であること」(`Lockout`)を毎回要求すると、50%確率の縁で孤立ポケットができ `generation_failed`(max_retries_exceeded) で高確率に失敗する。** 対処は `KeepLargestComponent`（最大連結成分だけ残し他を壁に戻す明示スタックBFS）を導入し、残った床は定義上100%連結にする（コミット `386a385`）。4分割ミラーコピー方式（L4）では、中央通路が継ぎ目(x=HW-1, y=HH-1)まで届かないと各象限が非連結になるため、`PrepareInnerBorders` の通路ループ上限を `<HW`/`<HH` に延長して対称軸で接続する。しきい値 `floorAreaMin`（既定600）は40×40でL2/L3しか満たせない過大値で、連結後の上限がL1≈332/L4≈212だったため既定を150へ下げた（C++ Params 2箇所 + フロント `map-gen-patterns.js` の DEFAULT_PARAMS）。大マップ(200×200)では `BuildSpine`（L1聖堂）が40×40ハードコード想定で床1.3%しか出ない事故もあり、グリッド全体に格子状チェンバーを敷き詰める設計へ全面置換した。検証は各アルゴリズムを StdAfx.h 非依存で cl.exe 単体コンパイルし、seed 1..200 で成功率・連結床面積を実測する方法が有効。

## 移動・当たり判定

- **`CInfoCharBase::GetCollisionRect`（`Common/Info/InfoCharBase.cpp`）はピクセル単位移行済みだが、`CInfoCharBase::GetPosRect`（同ファイル）は旧スケールのまま（`top = m_nMapY - 2*(cy-1)` の縦長矩形）だった。** 敵ブロック判定 `IsBlockChar` は `GetCollisionRect` を使い正常なのに、押し相手検出 `CLibInfoCharBase::GetFrontCharIDPush`（`Common/LibInfo/LibInfoCharBase.cpp`）は `GetPosRect` を使っていて検出失敗し、押せるボールNPCがすり抜けた。修正は `GetFrontCharIDPush` を `GetCollisionRectOnce`/`GetCollisionRect` ベースに変更。座標系（旧16px/unit→ピクセル単位）を移行する時は、同じ「矩形取得」役割の関数を全て洗い出すこと。移植コメント「`GetPosRect()+/2` は px単位では不正確」のような警告が残る箇所は要注意サイン。

- **`CLibInfoCharCli::IsMove` 内の `TrySlideMove`（壁際±8pxスライド）と斜めコーナー補正は `m_nMapX/Y` を直接書き換えて TRUE を返すが、`CStateProcMAP::MoveProc` は移動適用を入口で捕まえた古いローカル `x,y` 基準の `SetPos(x+xx, y+yy)` で行う。** 「補正後座標で判定通過→未検証の元座標で移動」というズレで、全方向ブロック（PartsType&1）のタイルへ横からめり込めた（マップ7 タイル(59,36) のランプ柱）。修正は `IsMove` が TRUE を返した直後に `x = m_nMapX; y = m_nMapY;` で基準座標を最新化する（`StateProcMAP.cpp MoveProc`）。斜め分岐(case4-7)の `GetFrontPos` が16px先に仮置きして横方向を検証する旧ハーフタイル移動の遺物も、実際の1px斜めステップの重なりを見逃す原因だったため `CanMoveDirection` による1px直接検証に置換した。全方向ブロックタイルからの脱出救済は「4方向どこからも進入できないタイルに重なっている場合」に限定すること（緩い条件だと `PartsType`=1487=UP|DOWN|LEFT のようなイス背もたれに正規に重なった状態で DOWN 方向の出口ブロックが誤って免除され、上から下へすり抜ける回帰を起こす）。

- **マップパーツの方向ブロック（`sys_map_parts.BlockDirection`、UP=1/DOWN=2/LEFT=4/RIGHT=8）は「そのタイルのどの辺が固いか」を表すエッジ属性だが、`CanMoveDirection`（`SboCli/src/LibInfoCharCli.cpp`）は `IsMoveOut`/`IsMove` の両方を移動先リーディングエッジのタイルに対して評価するため非対称な当たり判定になる。** `IsMoveOut(destタイル, dir)` がリーディングエッジがタイル内にある間ずっと発火し、1タイル手前で止まる。縦移動は同じパーツに UP/DOWN ビットが無ければ素通りするため「上からは通れるのに左からはぶつかる」非対称になる（マップ21のイス: 左列パーツ1482=RIGHTのみ、右列1487=UP|DOWN|LEFT）。修正は (1) リーディングエッジのタイルが変わる（境界を跨ぐ）ときだけ判定、(2) `IsMoveOut` は現在側（出るタイル）の辺、`IsMove` は移動先タイルの辺で評価、の2点セット。サーバ側 `LibInfoCharSvr::IsMove` も同構造だが PC移動はサーバ非検証（dead reckoning）なので当面クライアントのみで可。検証は python で DB を読む当たり判定シミュレータが有効。

- **他PCの Dead Reckoning（`UpdatePredictedPos`, `SboCli/src/Info/InfoCharCli.cpp`）は `m_bPredictedMove==TRUE` の間、最後の方向 `m_nPredictDirection` へ毎フレーム先読みし続けるが、「一定時間動きが無ければ止める」ウォッチドッグが無かった。** `POS_SYNC` 受信（`MainFrameRecvProcCHAR.cpp`）が `m_bUpdate==TRUE` なら無条件に `StartPredictedMove` を呼ぶため、静止キャラに update=TRUE の定期同期が届くと「先読みで前進→次の同期で実座標へ引き戻し」を繰り返し前後に振動した（コミット `366a8fe`）。修正は二段構え: fix1 ゼロ変位ガード（前回同期からの実変位が `PREDICT_MIN_MOVE_PIXELS`(4px) 未満なら静止扱いで予測しない）、fix2 失効ウォッチドッグ（`UpdatePredictedPos` で最後の受信から `PREDICT_STALE_STOP_MS`(500ms) 以上経過したら予測を打ち切り `m_nPredictSyncX/Y` へスナップ）。予測/補間を触る時は「動きが無ければ予測を回さない」を守ること。

- **NPC移動同期はウェイポイント追従方式（サーバーが100ms毎に `GetInterpolatedNPCPos` で送信、クライアントの `CInfoCharCli::UpdateWaypointMove` が `m_apMovePosQue` を順になぞる）、PCは Dead Reckoning。分岐は `RecvProcCHAR_MOVE_CORE` の `IsNPC()`(= m_nMoveType != CHARMOVETYPE_PC)。** `ChgMoveState` は移動中に立ち系へ変更するとマーカーを `AddMovePosQue` して return するため、キュー消化側から直接呼ぶとマーカーが自己増殖して無限ループ(フリーズ)になる。先に `m_nMoveState = 0` にしてから呼ぶこと。1px/frame未満の低速NPCは端数時間を繰り越さないとキューが無限成長する。`RecvProcCHAR_STATE` の停止処理は `m_dwPredictRecvTime`/`m_nPredictSyncX/Y` を参照するので、ウェイポイント受信時もこれらを更新しないと古い座標へ誤ワープする。`DeleteMovePosQue` は要素を SAFE_DELETE するので pop 後に参照しないこと。座ったまま向きを変えると立ち上がる回帰（PC の Dead Reckoning 移行 `a5a1e58` 由来）は、サーバー `RecvProcCHAR_MOVEPOS` が位置不変でも START/DIR で MOVE、STOP で STAND に書き換えていたため。判定を純粋関数 `MoveStateDecision`（`SboSvr/src/MainFrame/`）に切り出し「位置不変なら START/DIR で状態を変えない」「実際に移動中でなければ STOP で状態を変えない」にした（コミット `9751cd9`）。サーバーのキャラ状態を書き換える時は座り・防御・休憩等の「移動していない状態」を壊さないか必ず確認すること。

- **`CLibInfoItem::MakeItem(dwMapID, pptPos, dwItemTypeID)`（`Common/LibInfo/LibInfoItem.cpp`）は `dwMapID > 0` の時だけ `m_dwMapID`/`m_ptPos` を設定する。** `MakeItem(0, ...)` を呼ぶと座標・マップが入らず地面アイテムとして可視化されない（`ITEM_RES_ITEMINFO` は送られるのでドロップSEだけ鳴る）。「釣りヒットで魚が足元に落ちない（SEだけ鳴る）」バグの原因だった（コミット `249642d`）。正しくは `DropItem` と同じく `MakeItem(pInfoChar->m_dwMapID, &ptPos, ...)` に実マップと足元座標(`m_nMapX/m_nMapY`)を渡す。地面アイテムの `m_ptPos` はピクセル単位で、クライアント描画(`LayerMap.cpp DrawItem`)もピクセル前提。

## 管理画面 Web UI

- **稼働中の SboSvr は `SboSvr/webroot` ではなく `SboSvr/Debug/webroot` のコピーを配信する。** コピーは `SboSvr.vcxproj` の PostBuildEvent（`rmdir`→`xcopy /E /I /Y /H webroot`→`out\browser-title\sbocli-title.*` を `webroot\game\` へ）でビルド時にだけ行われる。実例: 新設した `entity-picker.js`/`move-types.js` が404、`index.html` が古い版のまま配信された。JS/HTML の直接修正が「効かない」ように見えたら、ブラウザキャッシュより配信元の同期漏れを疑う。手動同期は `xcopy /E /I /Y /H "SboSvr\webroot\*.*" "SboSvr\Debug\webroot\"`（サーバー稼働中でも可、DBに影響なし）。同じ `rmdir` は `webroot\game`（ゲーム本体）まで消すので手動では使わないこと。確認は `fetch(url, {cache:'no-store'})` で配信内容を直接見ると早い。

- **管理画面の「編集を別ウィンドウで開く」（`webroot/js/core/workspace-layout.js`）は `#view-container` の DOM を `adoptNode` で別ウィンドウへ移す方式。** スクリプトの realm は元ウィンドウのままなので、`confirm()`/`prompt()`/`alert()` は元ウィンドウ側に出て別ウィンドウの背後に隠れ「押しても何も起きない」ように見える。環境によっては `prompt()` が例外(`prompt() is not supported.`)になったり `confirm()` が自動的に false を返したりもする（途中セーブの `prompt()`、「出荷時の画像に戻す」の `confirm()` で2回実際に踏んだ）。対処パターン（画像エディタで採用）: 文字入力はモーダルをやめてインラインの `<input>` にする、破壊的操作は2段階クリック（`image-editor.js` の `armConfirmButton()`、1回目で「本当に◯◯?」+`is-armed`(赤)、4〜5秒で自動解除、2回目で実行）、保険として `installDialogFocusGuard`（`workspace-layout.js`）でポップアップ中だけ `confirm/alert/prompt` を `window.focus()` でラップする。

- **管理画面 (`SboSvr/webroot`) の編集ペインは左ナビ280pxと常設ゲーム画面パネルに挟まれ、実幅がビューポート幅よりはるかに狭い（実測: ビューポート1400pxで「並べて表示」時458px、「編集」時1062px）。** `@media (max-width: N)` でレイアウトの折り返しを判定すると永久に発火しない（画像エディタの3カラム化で固定幅240px+360pxが458pxのペインに収まらず中央カラムが0pxに潰れた実例）。編集ペイン内で多カラムを組む時は `.view[data-view="..."]` に `container-type: inline-size` を付け、`@container (max-width: ...)` で判定すること。`.view` は `height:auto` なのでシェルの `height:100%` は解決されないため、`main` の `calc(100vh - ...)` から `.view[data-view="..."] { height:100%; min-height:0 }` を画面ごとにスコープして高さを通す。

- **管理画面 (`SboSvr/webroot`) の CSS でセレクタに `display: flex` 等を指定すると、ブラウザ既定の `[hidden] { display: none }` が詳細度で負け、JS で `el.hidden = true` にしても表示されたままになる。** 画像エディタのカテゴリグループ折りたたみで実際に発生（`items.hidden === true` かつ `getComputedStyle(items).display === "flex"`）。`hidden` 属性で出し入れする要素に `display` を指定したら、必ず直後に `セレクタ[hidden] { display: none; }` を書く。非表示切り替えを `style.display` で行っている既存箇所と混ぜないこと。

- **キャラのバッグ(`m_adwItemID`)とアイテムの所有者ID(`CInfoItem::m_dwCharID`)は必ずそろえる。** クライアントは ITEM_RES_ITEMINFO を受けたとき、所有者キャラが見つからずマップにも無いアイテムを削除する（`CStateProcMAP::OnMainFrameRENEWITEMINFO`）。管理画面の「バッグに追加」が ID を足すだけで所有者を設定していなかったため、そのアイテムを装備するとサーバーが送り直したアイテム情報でクライアントから消え、見た目は変わるのに装備欄が空・バッグからも消える状態になった（ステージングで実際に発生）。バッグへの出し入れは `CLibInfoItem::AddItem` / `DeleteItem` を通し、装備時は `CLibInfoCharSvr::SetItemOwner` で所有者を補正している。

- **画像を配信するハンドラで `SboPlatform::LoadEmbeddedPng` を直接使わない。** これは SboGrpData.dll のリソースを読むだけで、Linux では常に失敗する。マップパーツ配置画面のシート(`/api/assets/map-parts/sheets/N`)がこれを直接使っていたため、ステージングではパーツ画像が出なかった(編集画面は `/api/assets/sprites` 経由なので出ていた)。画像ストア(DB)→ `SboGrpData/res/` → DLL の順に探す `CGrpResourceProvider` を通すこと。

## 画像・データ

- **画像の実体は `SboGrpData/res/` の PNG（`SboGrpData.dll` は .rc 埋め込みのフォールバックにすぎない）。** 読み込み経路は3系統: ネイティブ版 `MgrGrpData.cpp` の `Read()`/`Read256()`（ファイル優先→DLLフォールバック、`GetFileNameForResource()` の静的テーブルでリソース名→ファイル名解決）、ブラウザ版（`build-sbocli-browser-title.ps1` の `--preload-file res@/SboGrpData/res` で `.data` に同梱、コードパスはネイティブと同じ）、管理画面サーバ `SpriteSheetHandler.cpp` の `CGrpResourceProvider`（`LoadLibraryW`+`FindResourceW` で DLL から読む唯一の経路）。新しい画像種別を追加する時は3系統すべての対応表を揃える。

- **キャラ合成順（`InfoCharCli.cpp:818` 付近）は 体→髪下→服→耳→髪上→目→アクセ。SP服・NPCの時は体〜髪上を飛ばし服1枚で置換。** 2x2(32px・現行メイン、`MgrDraw.cpp:533-575`)は512×256=16列×8行、男=行0-3/女=行4-7（女はy+=32*4）、合成順は体→服→髪→目（16px版と違い耳・髪下/髪上が無い）、目だけ `x-32*4` かつ女性オフセットを足さない。16px（旧、`InfoCharCli.cpp:818`付近）は男女がX方向`x+=16*16=256`（2x2と逆！）、1装備=シートの1行、目は`eye01.png` 96×320=6列×20行(下=0,1/左=2,3/右=4,5)。`Common/GrpLayout.h` の `nCountX/nCountY` は物理サイズと一致しない場合がある（29カテゴリ中13で食い違う、例: `eye` は式16x512に対し実寸96x320）。カタログ値だけで座標計算しているコード（`webroot/js/data/assets.js` の `calcSpriteCoord()`）はこの13カテゴリでズレている可能性がある。

- **素材は全て8bitパレットPNG（インデックス0が透過）が前提。** クライアント `MgrGrpData.cpp` の `Read256()` は `colortype != LCT_PALETTE` なら読み込み拒否するが、サーバーの `MakeTransparentPng()` は非パレットを素通しする。RGBA PNG を入れると「管理画面では正常に見えるのにゲームだけ読めない」症状になる（アップロード入口に検証を追加済み、コミット `d8ae349`）。ブラウザの `canvas.toBlob()` は必ず RGBA PNG を生成するため、`webroot/js/lib/indexed-png.js` のように自前で8bitパレットPNGのデコーダ/エンコーダを持ち、インデックス値のまま編集・保存する必要がある。`MakeTransparentPng` を通した後もカラータイプは3のまま(`palette[0].a`だけ0)なので、ブラウザ側でインデックスを直接扱える。

- **画像アセットを `SboData.db` に入れてはいけない。** 起動時にメモリへ丸ごと読み込み停止時に書き戻す設計のため、管理画面からの差し替えが停止時に消える。専用の `SboGrpData.db`（journal_mode=DELETE で -wal/-shm を作らない）に res_name キーで PNG を保存し、`SpriteSheetHandler` が DB→res/→DLL の3段フォールバックで配信する構成にした。公開口 `GET /assets/manifest`/`GET /assets/sprite/{resName}`（認証不要、`/api/` の外）から取得し、shell の `preRun` で MEMFS の `/grp_override/<resName>.png` へ書き込む。C++ `TryReadGrpOverride()` が `Read`/`Read256` の先頭で見る。`fetch` は `cache:'no-cache'`（`no-store`だとIf-None-Matchが送られず毎回フル取得になる）。

- **エフェクト定義は SQLite の `sys_effect`（EffectID, Name, AnimeCount, Loop, GrpIDMain）と `sys_effect_anime`（EffectID, Slot, Wait×10ms/コマ, Level, GrpIDBase, GrpIDPile）。** コマ送りは `dwTime - m_dwLastAnime >= Wait*10`（`InfoEffect.cpp:375`）で進む。既知の主要ID: 1=打撃ヒット、2=気絶中(loop)、**5=消滅**（撃破時の消滅演出はこれ）、20=回復。演出の尺を調整する時はコード側の固定値ではなく DB の `Wait` 列を変更する。Windows から sqlite3 CLI 無しで照会する場合は OS 同梱の `winsqlite3.dll` を PowerShell `Add-Type` で P/Invoke（`sqlite3_open`/`prepare_v2`/`step`/`column_text`）するとリビルド不要で確認できる。

## テスト・自動操作

- **`SboSockLib/UraraSockTCPSelect.cpp` の select 実装はウィンドウ非依存なため、WSAAsyncSelect 実装では書けなかった通信層の単体テストが書けるようになった。** `SboSvrTest/` は外部依存の無い `TEST`/`CHECK`/`CHECK_EQ` の小さな基盤で `SboSvrTest.exe [名前の部分一致]` で実行。`SboSockLib/SboSockTestClient` はSBOプロトコルを喋る同期クライアント（フレーミング/CRC自前）で、仮想プレイヤー（NPCボット）の土台にそのまま使える。実装切り替えは環境変数 `SBO_SOCK_IMPL=select`（既定は従来実装）。プロトコル回帰は `tools/test-sbosvr-protocol.ps1`。

- **新しいテストは必ず「わざと壊すと落ちる」ことを確認すること。** テストクライアントの `Close()` が `SD_BOTH` で RST を送るため `recv()==0` の経路を踏まず、故障注入で13/13が通ってしまった実例がある。green であることと意図した経路を検証できていることは別問題。ビルド時は `zlibD.lib` が `/MD` なので `IgnoreSpecificDefaultLibraries=MSVCRTD` が必要、かつ VC90 マニフェスト依存を `mt.exe` で差し替える PostBuildEvent が要る。

- **`SboSvrTest` は `StaticFileHandler.cpp` 等を自分の exe に直接取り込むので、`SboSvr.exe` 稼働中（ロック中）でもビルド・実行できる。** SboSvr 側は `/t:ClCompile` でコンパイル確認だけ済ませ、リンクはサーバー停止後に行う運用が取れる（パス解釈のパーセントデコード・`%2e%2e` 拒否・UTF-8ファイル名など含め全108件のテストがある）。

- **ブラウザ枠の `computer` によるキー入力/クリックや `window` への KeyboardEvent dispatch はゲームに届かない（SDL のキー受け口が `#sbop2ImeInput` に固定されているため、`SDLApp.cpp` の `SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT`）。** 代わりに仮想パッドの注入口を使う: `Module.ccall('SBOP2_BrowserSetPadKey', null, ['number','number'], [vk,1/0])`（Windows仮想キーコード、↓=40/↑=38/←=37/→=39/X=88/Z=90/ESC=27）。名前入力は `SBOP2_BrowserCharNameSetText`/`SBOP2_BrowserCharNameSubmit`。ページ遷移すると javascript_tool の戻り値が失われるので確認値は sessionStorage に退避する。**専用の仕組み（コミット `55aa2c6`、手順は `docs/debug-automation.md`）を優先して使う**: `POST /api/debug/fixture` でアカウント＋キャラ＋トークンを作成（Debugビルドかつloopback直のみ）→トークンをlocalStorageへ→`/debug` を開き `window.sbop2Debug`（`clickStart`→`waitFor LOGIN`→`startWithToken`→`x,x`→`waitFor MAP`、約13秒でマップ）。`state()` でゲーム状態・ウィンドウ・カーソル位置・キャラ座標・messagesをJSONで取得できる。

- **`sbop2Debug` の hold/press 後にキーが押しっぱなしになり「キャラが固まった」ように見えることがある。** `releaseAll()` で解消する（ゲームの不具合と誤認しないこと）。テストNPCはマップ1のCharID 824「リナ」(1216,1312付近)。

- **管理画面からの値変更はメモリだけ変わりクライアントに届かない。** `LibInfoCharSvr::NotifyAdminEditCharInfo` で `RES_CHARINFO` を再送する必要がある。（`CharacterUpdateHandler` の基本情報とグラフィックは対応済み。ステータスなど他のタブも同じ落とし穴がある）

- **Linux 版 SboSvr は管理画面の画像を実行ファイルの隣の `SboGrpData/res/` から配信する。** Windows 版は `SboGrpData.dll` の埋め込みリソースなので気付きにくい。配置先（ステージングの Docker イメージなど）に含め忘れると `/api/assets/sprites` が全カテゴリ 404 になり、グラフィック画面やサムネが空になる。

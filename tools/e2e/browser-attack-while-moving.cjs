// ブラウザ版で「移動キーを押したまま攻撃キーを押すと、その場で立ち止まって攻撃する」ことを確かめる。
// 以前は移動中(MOVE/BATTLEMOVE)の攻撃キーを受け付けず、移動キーを離して立ち止まらないと攻撃できなかった。
// あわせて、攻撃キーを押しっぱなしの間は歩き出さないことも見る。
// (離したあとまた歩き出すかはログに出すだけ。テスト環境ではタイルを跨ぐとマップイベント判定の返事待ちで
//  歩きが止まったままになることがあり(master でも同じ)、確かめられないため)
// tools/test-browser-e2e-linux.sh から呼ばれる。
//
//   node tools/e2e/browser-attack-while-moving.cjs <http の URL> <スクショ出力ディレクトリ>
//
// サーバーは -DSBO_DEBUG_API=ON でビルドしたもの(テスト準備 API あり)を loopback で使うこと。
const path = require('path');
const fs = require('fs');
const { chromium } = require('playwright');

const baseUrl = process.argv[2] || 'http://127.0.0.1:18080';
const outDir = process.argv[3] || 'out/e2e';
fs.mkdirSync(outDir, { recursive: true });

function log(msg) { console.log(`[e2e-attack-move] ${msg}`); }

// Common/Info/InfoCharBase.h の CHARMOVESTATE_*
const MOVESTATE_MOVE = 2;
const MOVESTATE_BATTLEMOVE = 6;
const MOVESTATE_BATTLEATACK = 7;

(async () => {
  const browser = await chromium.launch({
    headless: true,
    // ヘッドレスでも WebGL(SDL2 の描画)が動くようにソフトウェア GL を使う
    args: ['--use-angle=swiftshader', '--enable-unsafe-swiftshader', '--ignore-gpu-blocklist', '--autoplay-policy=no-user-gesture-required'],
  });
  const context = await browser.newContext({ viewport: { width: 1280, height: 800 } });
  const page = await context.newPage();
  const consoleLines = [];
  page.on('console', (m) => consoleLines.push(`[${m.type()}] ${m.text()}`));
  page.on('pageerror', (e) => consoleLines.push(`[pageerror] ${e.message}`));

  let ok = false;
  try {
    // 1) テスト用アカウント・キャラを作る(毎回新しいアカウント)
    await page.goto(`${baseUrl}/health`);
    const fixture = await page.evaluate(async () => {
      const r = await fetch('/api/debug/fixture', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: '{}' });
      if (!r.ok) throw new Error(`fixture HTTP ${r.status}`);
      const j = await r.json();
      localStorage.setItem('sbop2_device_token', j.deviceToken);
      localStorage.setItem('sbop2_device_account', j.account);
      return j;
    });
    log(`fixture: account=${fixture.account} charId=${fixture.charId}`);

    // 2) マップ画面まで入る
    await page.goto(`${baseUrl}/debug`);
    await page.waitForFunction(() => window.sbop2Debug && window.sbop2Debug.state && window.sbop2Debug.state(), null, { timeout: 60000, polling: 250 });
    const player = await page.evaluate(async () => {
      const d = window.sbop2Debug;
      const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
      await d.clickStart();
      await d.waitFor({ gameState: 'LOGIN' }, 30000);
      await d.startWithToken(30000);
      await d.waitFor({ gameState: 'LOGINMENU' }, 30000);
      await d.press('x'); await sleep(600); await d.press('x'); // キャラ選択 → 1人目
      const s = await d.waitFor({ gameState: 'MAP' }, 30000);
      return s.player;
    });
    log(`MAP に入った player=${JSON.stringify(player)}`);

    // 3) 右へ歩きながら攻撃キーを押す。下は戦闘の確認で敵を置くので右を使う(近づく確認はこのあとに流す)。
    //    状態は 30ms ごとに記録して、あとでまとめて調べる。
    const r = await page.evaluate(async () => {
      const d = window.sbop2Debug;
      const sleep = (ms) => new Promise((res) => setTimeout(res, ms));
      const samples = [];
      const sample = async (phase, ms) => {
        const end = performance.now() + ms;
        do {
          const p = d.state().player;
          samples.push({ phase, t: Math.round(performance.now()), x: p.x, y: p.y, moveState: p.moveState });
          await sleep(30);
        } while (performance.now() < end);
      };
      d.keyDown('right');
      // 歩き出したらすぐ攻撃キーを押す
      await sample('walk', 0);
      for (let i = 0; (i < 50) && (samples[samples.length - 1].x === samples[0].x); i ++) {
        await sample('walk', 1);
      }
      d.keyDown('x');
      await sample('attack', 2500);   // 右を押したまま攻撃キーも押しっぱなし
      d.keyUp('x');
      await sample('resume', 900);    // 攻撃キーを離す(右は押したまま)
      d.releaseAll();
      await sleep(300);
      return samples;
    });
    // 調べやすいよう記録をそのまま残す
    fs.writeFileSync(path.join(outDir, 'attack-while-moving-samples.json'), JSON.stringify(r));
    const phase = (name) => r.filter((s) => s.phase === name);
    const walk = phase('walk');
    const attack = phase('attack');
    const resume = phase('resume');
    const walked = walk[walk.length - 1].x > walk[0].x;
    const moving = walk.some((s) => (s.moveState === MOVESTATE_MOVE) || (s.moveState === MOVESTATE_BATTLEMOVE));
    // 攻撃キーを押してすぐ攻撃が始まること(ヘッドレスは描画が遅く 1 フレームが 150ms ほどかかるので 600ms まで待つ)
    const firstAttack = attack.findIndex((s) => s.moveState === MOVESTATE_BATTLEATACK);
    const attackMs = (firstAttack >= 0) ? (attack[firstAttack].t - attack[0].t) : -1;
    const attackedSoon = (firstAttack >= 0) && (attackMs <= 600);
    // 攻撃が始まってから攻撃キーを離すまで、右を押していても一歩も進まないこと
    const afterAttack = (firstAttack >= 0) ? attack.slice(firstAttack) : [];
    const stayed = (afterAttack.length > 0) && afterAttack.every((s) => s.x === afterAttack[0].x);
    // 押しっぱなしの連続攻撃の回数(ヘッドレスは描画が遅く攻撃モーションも長くなるのでログに出すだけ)
    let attackStarts = 0;
    for (let i = 0; i < attack.length; i ++) {
      if ((attack[i].moveState === MOVESTATE_BATTLEATACK) && ((i === 0) || (attack[i - 1].moveState !== MOVESTATE_BATTLEATACK))) {
        attackStarts ++;
      }
    }
    // 攻撃キーを離したら、また歩き出したか(ログに出すだけ)
    const resumed = (afterAttack.length > 0) &&
      (resume.some((s) => (s.moveState === MOVESTATE_MOVE) || (s.moveState === MOVESTATE_BATTLEMOVE)) ||
       (resume[resume.length - 1].x > afterAttack[afterAttack.length - 1].x));
    log(`歩き: ${walk[0].x}→${walk[walk.length - 1].x} 移動状態あり=${moving}`);
    log(`攻撃: 押してから ${attackMs}ms で攻撃開始 / 攻撃回数=${attackStarts} / 押している間の x=${[...new Set(afterAttack.map((s) => s.x))].join(',')}`);
    log(`再開(参考): 歩き出した=${resumed} ${afterAttack.length ? afterAttack[afterAttack.length - 1].x : '-'}→${resume[resume.length - 1].x}`);
    await page.screenshot({ path: path.join(outDir, 'attack-while-moving.png') });
    ok = walked && moving && attackedSoon && stayed && (attackStarts >= 1);
  } catch (e) {
    log(`失敗: ${e.message}`);
    try {
      const s = await page.evaluate(() => window.sbop2Debug && window.sbop2Debug.state());
      log(`最後の状態: ${JSON.stringify(s)}`);
    } catch (_) { /* 状態が取れなくても続行 */ }
    await page.screenshot({ path: path.join(outDir, 'attack-while-moving-failure.png') }).catch(() => {});
  } finally {
    fs.writeFileSync(path.join(outDir, 'attack-while-moving-console.log'), consoleLines.join('\n') + '\n');
    await browser.close();
  }
  log(ok ? 'OK' : 'NG');
  process.exit(ok ? 0 : 1);
})();

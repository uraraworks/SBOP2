// ブラウザ版で「敵を攻撃 → 反撃されて気絶」までを確かめる。
// tools/test-browser-e2e-linux.sh から呼ばれる。湧く敵は場所と時間がランダムなので、
// テスト準備 API(/api/debug/npc・/api/debug/char-status)で敵を置き、自キャラの HP を 1 にする。
//
//   node tools/e2e/browser-battle.cjs <http の URL> <スクショ出力ディレクトリ>
//
// サーバーは -DSBO_DEBUG_API=ON でビルドしたもの(テスト準備 API あり)を loopback で使うこと。
const path = require('path');
const fs = require('fs');
const { chromium } = require('playwright');

const baseUrl = process.argv[2] || 'http://127.0.0.1:18080';
const outDir = process.argv[3] || 'out/e2e';
fs.mkdirSync(outDir, { recursive: true });

function log(msg) { console.log(`[e2e-battle] ${msg}`); }

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

    // 3) HP を 1 にし、正面(1マス下)に敵を置く。敵は攻撃されると反撃してくる(戦闘1)
    const setup = await page.evaluate(async (p) => {
      const post = async (url, body) => {
        const r = await fetch(url, { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(body) });
        return { status: r.status, json: await r.json() };
      };
      const status = await post('/api/debug/char-status', { charId: p.charID, hp: 1 });
      const npc = await post('/api/debug/npc', { mapId: p.mapID, x: p.x, y: p.y + 32, charName: 'E2E敵', hp: 500, atack: 5 });
      return { status, npc };
    }, player);
    log(`準備: ${JSON.stringify(setup)}`);
    if ((setup.status.status !== 200) || (setup.status.json.hp !== 1) || (setup.npc.status !== 201)) {
      throw new Error('テスト準備 API が失敗した');
    }
    await page.waitForTimeout(1500);

    // 4) 下を向いて攻撃し続け、反撃で気絶メニューが出ることを確かめる
    const swoon = await page.evaluate(async () => {
      const d = window.sbop2Debug;
      await d.press('down');
      d.hold('x', 8000);
      try {
        const s = await d.waitFor((st) => st.activeWindow && st.activeWindow.name === 'SWOON', 15000);
        return s.activeWindow.name;
      } finally {
        d.releaseAll();
      }
    });
    log(`気絶後のアクティブウィンドウ: ${swoon}`);
    await page.waitForTimeout(500);
    await page.screenshot({ path: path.join(outDir, 'battle-swoon.png') });
    ok = swoon === 'SWOON';
  } catch (e) {
    log(`失敗: ${e.message}`);
    try {
      const s = await page.evaluate(() => window.sbop2Debug && window.sbop2Debug.state());
      log(`最後の状態: ${JSON.stringify(s)}`);
    } catch (_) { /* 状態が取れなくても続行 */ }
    await page.screenshot({ path: path.join(outDir, 'battle-failure.png') }).catch(() => {});
  } finally {
    fs.writeFileSync(path.join(outDir, 'battle-console.log'), consoleLines.join('\n') + '\n');
    await browser.close();
  }
  log(ok ? 'OK' : 'NG');
  process.exit(ok ? 0 : 1);
})();

// ブラウザ版クライアントを Playwright(Chromium)で開き、デバッグ API で MAP まで入ってスクショを撮る。
// tools/test-browser-e2e-linux.sh から呼ばれる。手順は docs/debug-automation.md の「実証済みの通し手順」。
//
//   node tools/e2e/browser-enter-map.cjs <http の URL> <スクショ出力ディレクトリ>
//
// サーバーは _DEBUG 付き(/api/debug/fixture あり)で、loopback から叩くこと。
//
// ステージング(fixture 無し・https・Basic 認証)を確かめる時は環境変数で指定する:
//   SBOP2_E2E_ACCOUNT / SBOP2_E2E_TOKEN  既存アカウントの端末トークン(指定時は fixture を呼ばない)
//   SBOP2_E2E_BASIC                      Basic 認証の "ユーザー:パスワード"
//   SBOP2_E2E_INSECURE=1                 自己署名証明書を許す(手元の Caddy で試す時)
const path = require('path');
const fs = require('fs');
const { chromium } = require('playwright');

const baseUrl = process.argv[2] || 'http://127.0.0.1:18080';
const outDir = process.argv[3] || 'out/e2e';
fs.mkdirSync(outDir, { recursive: true });

function log(msg) { console.log(`[e2e] ${msg}`); }

(async () => {
  const browser = await chromium.launch({
    headless: true,
    // ヘッドレスでも WebGL(SDL2 の描画)が動くようにソフトウェア GL を使う
    args: ['--use-angle=swiftshader', '--enable-unsafe-swiftshader', '--ignore-gpu-blocklist', '--autoplay-policy=no-user-gesture-required'],
  });
  const contextOptions = { viewport: { width: 1280, height: 800 } };
  if (process.env.SBOP2_E2E_BASIC) {
    const i = process.env.SBOP2_E2E_BASIC.indexOf(':');
    contextOptions.httpCredentials = {
      username: process.env.SBOP2_E2E_BASIC.slice(0, i),
      password: process.env.SBOP2_E2E_BASIC.slice(i + 1),
    };
  }
  if (process.env.SBOP2_E2E_INSECURE === '1') { contextOptions.ignoreHTTPSErrors = true; }
  const context = await browser.newContext(contextOptions);
  const page = await context.newPage();
  const consoleLines = [];
  page.on('console', (m) => consoleLines.push(`[${m.type()}] ${m.text()}`));
  page.on('pageerror', (e) => consoleLines.push(`[pageerror] ${e.message}`));
  // どの URL が 404 なのかはコンソールに出ないので別に記録する
  page.on('response', (r) => { if (r.status() >= 400) consoleLines.push(`[http ${r.status()}] ${r.url()}`); });

  let ok = false;
  try {
    // 1) テスト用アカウント・キャラ・端末トークンを作り、localStorage に置く
    await page.goto(`${baseUrl}/health`);
    if (process.env.SBOP2_E2E_TOKEN) {
      await page.evaluate(([token, account]) => {
        localStorage.setItem('sbop2_device_token', token);
        localStorage.setItem('sbop2_device_account', account);
      }, [process.env.SBOP2_E2E_TOKEN, process.env.SBOP2_E2E_ACCOUNT || '']);
      log(`既存アカウントを使用: ${process.env.SBOP2_E2E_ACCOUNT || '(不明)'}`);
    } else {
      const fixture = await page.evaluate(async () => {
        const r = await fetch('/api/debug/fixture', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: '{}' });
        if (!r.ok) throw new Error(`fixture HTTP ${r.status}`);
        const j = await r.json();
        localStorage.setItem('sbop2_device_token', j.deviceToken);
        localStorage.setItem('sbop2_device_account', j.account);
        return j;
      });
      log(`fixture: account=${fixture.account} char=${fixture.charName}`);
    }

    // 2) /debug から起動し、sbop2Debug で MAP まで進める
    const t0 = Date.now();
    await page.goto(`${baseUrl}/debug`);
    await page.waitForFunction(() => window.sbop2Debug && window.sbop2Debug.state && window.sbop2Debug.state(), null, { timeout: 60000, polling: 250 });
    log(`起動 ${(Date.now() - t0) / 1000}s`);
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
    log(`MAP に入った ${(Date.now() - t0) / 1000}s player=${JSON.stringify(player)}`);
    // 描画が落ち着くのを少し待ってから撮る
    await page.waitForTimeout(1500);
    await page.screenshot({ path: path.join(outDir, 'map.png') });

    // 3) ESC でシステムメニューが開くこと
    const active = await page.evaluate(async () => {
      const d = window.sbop2Debug;
      await d.press('esc');
      const s = d.state();
      d.releaseAll();
      return s.activeWindow && s.activeWindow.name;
    });
    log(`ESC 後のアクティブウィンドウ: ${active}`);
    await page.screenshot({ path: path.join(outDir, 'map-menu.png') });
    ok = player && active === 'SYSTEMMENU';
  } catch (e) {
    log(`失敗: ${e.message}`);
    try {
      const s = await page.evaluate(() => window.sbop2Debug && window.sbop2Debug.state());
      log(`最後の状態: ${JSON.stringify(s)}`);
    } catch (_) { /* 状態が取れなくても続行 */ }
    await page.screenshot({ path: path.join(outDir, 'failure.png') }).catch(() => {});
  } finally {
    fs.writeFileSync(path.join(outDir, 'console.log'), consoleLines.join('\n') + '\n');
    await browser.close();
  }
  log(ok ? 'OK' : 'NG');
  process.exit(ok ? 0 : 1);
})();

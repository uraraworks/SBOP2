// ブラウザ版で「敵に横から・縦から歩いて近づくと、どちらもぴったり接するところまで行ける」ことを確かめる。
// 以前は自キャラのキャラ当たり判定が半タイル(16px)先を見ていたため、横から近づくと敵の 16px 手前で止まっていた。
// tools/test-browser-e2e-linux.sh から呼ばれる。テスト準備 API(/api/debug/npc)で敵を置く。
//
//   node tools/e2e/browser-approach-enemy.cjs <http の URL> <スクショ出力ディレクトリ>
//
// サーバーは -DSBO_DEBUG_API=ON でビルドしたもの(テスト準備 API あり)を loopback で使うこと。
const path = require('path');
const fs = require('fs');
const { chromium } = require('playwright');

const baseUrl = process.argv[2] || 'http://127.0.0.1:18080';
const outDir = process.argv[3] || 'out/e2e';
fs.mkdirSync(outDir, { recursive: true });

function log(msg) { console.log(`[e2e-approach] ${msg}`); }

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

    // 3) 敵を置いて歩くための道具
    const post = (url, body) => page.evaluate(async ([u, b]) => {
      const r = await fetch(u, { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(b) });
      return { status: r.status, json: await r.json() };
    }, [url, body]);
    const walk = (key, ms) => page.evaluate(async ([k, t]) => {
      const d = window.sbop2Debug;
      const sleep = (w) => new Promise((r) => setTimeout(r, w));
      d.hold(k, t);
      await sleep(t + 500);
      d.releaseAll();
      await sleep(300);
      return d.state().player;
    }, [key, ms]);

    // 敵を置き、クライアントに届くまで待つ(届く前に歩くと当たり判定が効かない)。
    // 新しく置いた NPC は自キャラが動いたときに届くので、back の向きに少しずつ動いて待つ
    // (back に配列を渡すと順番に使う。左右交互にすればその場からあまり動かない)
    const placeEnemy = async (mapId, x, y, charName, back) => {
      const npc = await post('/api/debug/npc', { mapId, x, y, charName, hp: 500, atack: 0 });
      if (npc.status !== 201) {
        throw new Error(`敵を置けなかった: ${JSON.stringify(npc)}`);
      }
      let seen = false;
      for (let i = 0; (i < 30) && !seen; i ++) {
        seen = await page.evaluate(async ([id, key]) => {
          const d = window.sbop2Debug;
          const o = d.others();
          if (((o && o.chars) || []).some((c) => c.id === id)) {
            return true;
          }
          d.hold(key, 60);
          await new Promise((r) => setTimeout(r, 400));
          return false;
        }, [npc.json.charId, Array.isArray(back) ? back[i % back.length] : back]);
      }
      if (!seen) {
        throw new Error(`置いた敵がクライアントに届かない: ${JSON.stringify(npc.json)}`);
      }
      return npc;
    };
    const enemyPos = (id) => page.evaluate((i) => {
      const o = window.sbop2Debug.others();
      return ((o && o.chars) || []).find((c) => c.id === i);
    }, id);

    const results = [];
    let cur = player;
    // 4) 右と下に敵を置き、それぞれへ歩いて近づいて止まった位置を調べる。
    //    キャラ同士は見た目どおり 32x32 で当たるので、ぴったり接すると縦横とも座標の差が 32 になる。
    const tries = [
      // 横は「以前の判定なら一歩も進めない距離(16px 空き)」に置く。縦は以前と同じ位置で止まること
      { name: '右から', key: 'right', back: 'left', dx: 48, dy: 0, axis: 'x', expect: 32 },
      { name: '下から', key: 'down', back: 'up', dx: 0, dy: 64, axis: 'y', expect: 32 },
    ];
    for (const t of tries) {
      const ex = cur.x + t.dx;
      const ey = cur.y + t.dy;
      const npc = await placeEnemy(cur.mapID, ex, ey, `E2E敵${t.key}`, t.back);
      await page.waitForTimeout(500);
      cur = await walk(t.key, 2000);
      // 敵の実際の位置はクライアントが知っている座標で見る
      const enemy = await enemyPos(npc.json.charId);
      if (!enemy) {
        throw new Error(`置いた敵が見えない: ${JSON.stringify(npc.json)}`);
      }
      const gap = (t.axis === 'x') ? (enemy.x - cur.x) : (enemy.y - cur.y);
      log(`${t.name}: 敵=(${enemy.x},${enemy.y}) 止まった位置=(${cur.x},${cur.y}) 差=${gap} (期待 ${t.expect})`);
      await page.screenshot({ path: path.join(outDir, `approach-${t.key}.png`) });
      results.push(gap === t.expect);
    }

    // 5) 敵が一歩で重なってきた状態(左に半分重なる)を作り、めり込む向き(左)には進めず、
    //    離れる向き(上)には抜け出せること。以前は重なった相手を判定から外していたため、
    //    そのまま左へすり抜けられた。
    {
      const npc = await placeEnemy(cur.mapID, cur.x - 16, cur.y, 'E2E敵overlap', ['up', 'down']);
      await page.waitForTimeout(500);
      const before = (await page.evaluate(() => window.sbop2Debug.state().player));
      const enemy = await enemyPos(npc.json.charId);
      const left = await walk('left', 1500);
      const up = await walk('up', 300);
      const overlapped = Math.abs(enemy.x - before.x) < 32 && Math.abs(enemy.y - before.y) < 32;
      log(`重なった敵: 敵=(${enemy.x},${enemy.y}) 前=(${before.x},${before.y}) 左へ=(${left.x},${left.y}) 上へ=(${up.x},${up.y})`);
      await page.screenshot({ path: path.join(outDir, 'approach-overlap.png') });
      results.push(overlapped && (left.x === before.x) && (up.y < left.y));
    }
    ok = results.every((r) => r);
  } catch (e) {
    log(`失敗: ${e.message}`);
    try {
      const s = await page.evaluate(() => window.sbop2Debug && window.sbop2Debug.state());
      log(`最後の状態: ${JSON.stringify(s)}`);
    } catch (_) { /* 状態が取れなくても続行 */ }
    await page.screenshot({ path: path.join(outDir, 'approach-failure.png') }).catch(() => {});
  } finally {
    fs.writeFileSync(path.join(outDir, 'approach-console.log'), consoleLines.join('\n') + '\n');
    await browser.close();
  }
  log(ok ? 'OK' : 'NG');
  process.exit(ok ? 0 : 1);
})();

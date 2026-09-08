// ブラウザ版成果物の事前圧縮スクリプト
// 指定ディレクトリのファイルに対して .br / .gz を作る。
// サーバーは実行時に圧縮せず、置いてあるものを Accept-Encoding に応じて選ぶだけにする。
// 出力が入力より新しければスキップするので、差分ビルドでは走らない。
import { readFileSync, writeFileSync, statSync, existsSync, unlinkSync } from 'node:fs';
import { join } from 'node:path';
import { brotliCompressSync, gzipSync, constants } from 'node:zlib';

const [, , dir, ...names] = process.argv;
if (!dir || names.length === 0) {
  console.error('usage: node precompress.mjs <dir> <file>...');
  process.exit(2);
}

// 大きいファイルは圧縮率を落とさないとビルド時間が跳ねる
const LARGE_BYTES = 8 * 1024 * 1024;

for (const name of names) {
  const src = join(dir, name);
  if (!existsSync(src)) {
    continue;
  }
  const srcStat = statSync(src);
  const isLarge = srcStat.size >= LARGE_BYTES;
  let data = null;

  const variants = [
    {
      ext: '.br',
      compress: (buf) => brotliCompressSync(buf, {
        params: {
          [constants.BROTLI_PARAM_QUALITY]: isLarge ? 5 : 11,
          [constants.BROTLI_PARAM_SIZE_HINT]: buf.length,
        },
      }),
    },
    {
      ext: '.gz',
      compress: (buf) => gzipSync(buf, { level: isLarge ? 6 : 9 }),
    },
  ];

  for (const variant of variants) {
    const dst = src + variant.ext;
    if (existsSync(dst) && statSync(dst).mtimeMs >= srcStat.mtimeMs) {
      continue; // 既に最新
    }
    if (data === null) {
      data = readFileSync(src);
    }
    const out = variant.compress(data);
    if (out.length >= data.length) {
      // 縮まないものは置かない。古い成果物が残っていると配信されるので消す。
      if (existsSync(dst)) {
        unlinkSync(dst);
      }
      console.log(`[precompress] ${name}${variant.ext}: skipped (not smaller)`);
      continue;
    }
    writeFileSync(dst, out);
    const pct = ((out.length / data.length) * 100).toFixed(1);
    console.log(`[precompress] ${name}${variant.ext}: ${data.length} -> ${out.length} bytes (${pct}%)`);
  }
}

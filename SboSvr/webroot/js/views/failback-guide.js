/**
 * views/failback-guide.js
 * フェイルバック手順画面 (route: failback-guide)
 *
 * 現在はプレースホルダー画面。Wave 3 で切り戻し手順ナビゲーション等を実装予定。
 */

export function mount(container) {
  container.innerHTML = `
      <section class="card placeholder-card">
        <h2>フェイルバック手順（プレースホルダー）</h2>
        <p>Web UI 障害時に MFC へ切り戻す手順をナビゲートする画面です。</p>
        <ul>
          <li>現在のシステム状態チェック</li>
          <li>切り戻し手順ステップ表示</li>
          <li>復旧チェックリスト</li>
        </ul>
      </section>
  `;
}

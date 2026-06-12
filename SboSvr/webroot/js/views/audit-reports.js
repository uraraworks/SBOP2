/**
 * views/audit-reports.js
 * 監査レポートダウンロード画面 (route: audit-reports)
 *
 * 現在はプレースホルダー画面。Wave 3 で CSV/JSON エクスポート等を実装予定。
 */

export function mount(container) {
  container.innerHTML = `
      <section class="card placeholder-card">
        <h2>監査レポートダウンロード（プレースホルダー）</h2>
        <p>Wave 3 で予定している監査ログ出力画面です。</p>
        <ul>
          <li>期間指定と CSV/JSON エクスポート</li>
          <li>フィルター（操作種別、担当者）</li>
          <li>自動配信スケジュール設定</li>
        </ul>
      </section>
  `;
}

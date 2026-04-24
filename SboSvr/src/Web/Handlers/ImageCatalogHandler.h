#pragma once

#include "Web/ApiHandler.h"

// ---------------------------------------------------------------------------
// /api/image-categories — 画像カテゴリカタログ（読み取り専用）
//
// GET /api/image-categories   画像カテゴリ（GRPIDMAIN_*）一覧を返す
//
// 旧 MFC の DlgAdminSelectImage / DlgAdminMapEventGRPIDTMP 等で使われていた
// カテゴリ選択（キャラ / 武器 / エフェクト / 2x2 系 など）を Web 管理画面から
// picker で扱うための、軽量なカタログ提供 API。
//
// サーバープロセス側は画像ピクセルデータを保持しないため、このハンドラは
// 「カテゴリ ID ＋ 日本語ラベル ＋ 参考用の推奨最大枚数」のみを返す。
// ピクセルプレビューが欲しくなったら、将来的にクライアント側のリソースを
// 静的配信する枠を別途用意する想定。
// ---------------------------------------------------------------------------

class CImageCatalogHandler : public IApiHandler
{
public:
        virtual void Handle(const HttpRequest &request, HttpResponse &response);
};

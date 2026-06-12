#pragma once

// SoundCatalogHandler.h
// サウンドカタログ API ハンドラ
//
// GET /api/sounds
//   SboSoundData/resource.h の IDR_WAVE_* を静的テーブルとして返す。
//   resource.h を変更した場合は SoundCatalogHandler.cpp の kSoundEntries[] も
//   必ず同期して更新すること（「resource.h と同期必須」コメント参照）。
//
// レスポンス例:
//   [
//     {"id":101,"key":"IDR_WAVE_PI73",    "label":"PI73"},
//     {"id":102,"key":"IDR_WAVE_ASI_2",   "label":"ASI_2"},
//     ...
//   ]
//
// label は IDR_WAVE_ プレフィクスを除いた名前を機械生成しているが、
// kSoundEntries の pszLabel フィールドに直接和名を記述することで
// 後から人間可読なラベルへ差し替え可能。

#include "Web/ApiHandler.h"

// ---------------------------------------------------------------------------
// CSoundCatalogHandler: GET /api/sounds
// ---------------------------------------------------------------------------
class CSoundCatalogHandler : public IApiHandler
{
public:
    void Handle(const HttpRequest &request, HttpResponse &response) override;
};

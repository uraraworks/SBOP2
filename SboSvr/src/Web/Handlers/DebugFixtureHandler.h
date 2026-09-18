/// @file DebugFixtureHandler.h
/// @brief /api/debug/fixture (テスト準備API) 定義ファイル
/// @details _DEBUG ビルドにのみ存在する。1回の呼び出しでテスト用アカウント・
///          キャラクター・端末トークンを作り、ブラウザ版クライアントをその
///          トークンで開けばすぐマップ画面に入れる状態にする。
///          安全策として、実行時にも loopback からの直接リクエスト以外は
///          404 として扱う(DebugFixtureGuard::IsAllowedRequest 参照)。
///          このファイルは #ifdef _DEBUG で全体を囲み、Release ビルドの
///          .vcxproj からも除外することで、Release バイナリに一切含まれない
///          ようにしてある。

#pragma once

#ifdef _DEBUG

#include "Web/ApiHandler.h"

class CMgrData;

/// POST /api/debug/fixture : account?, password?, charName? -> account, password,
///                            deviceToken, accountId, charId, charName
class CDebugFixtureHandler : public IApiHandler
{
public:
	explicit CDebugFixtureHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	CMgrData *m_pMgrData;
};

#endif // _DEBUG

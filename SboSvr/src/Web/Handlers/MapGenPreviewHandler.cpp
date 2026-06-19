#include "StdAfx.h"
#include "MapGenPreviewHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "SaveLoad/SaveLoadInfoMapGenPattern.h"
#include "MapGen/MapGenCaveL3.h"

// ---------------------------------------------------------------------------
// CMapGenPreviewHandler — POST /api/map-gen-patterns/preview
// ---------------------------------------------------------------------------

CMapGenPreviewHandler::CMapGenPreviewHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CMapGenPreviewHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // 認証チェック
    AuthProvider::AuthContext authContext;
    AuthProvider::AuthStatus authStatus =
        AuthProvider::Authenticate(request, m_pMgrData, authContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return;
    }
    if (m_pMgrData == NULL) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return;
    }

    const std::string &body = request.body;

    // patternId が指定されているか確認し、あれば DB から既定値を読む
    MapGenCaveL3::Params genParams; // デフォルト値で初期化済み
    int floorGrpId = 1;            // roleMap.floor のデフォルト
    int wallGrpId  = 0;            // roleMap.wall のデフォルト

    int patternId = 0;
    bool hasPatternId = JsonUtils::TryGetInt(body, "patternId", patternId);

    if (hasPatternId && patternId > 0)
    {
        // DB から該当レコードを取得
        CSaveLoadInfoMapGenPattern db;
        std::vector<MapGenPatternRecord> records;
        int nCount = db.LoadAll(records);
        if (nCount < 0)
        {
            response.statusLine = "HTTP/1.1 500 Internal Server Error";
            response.SetJsonBody("{\"error\":\"db_error\"}");
            return;
        }

        // patternId で絞り込む
        const MapGenPatternRecord *pRec = NULL;
        for (size_t i = 0; i < records.size(); ++i)
        {
            if (records[i].nPatternID == patternId)
            {
                pRec = &records[i];
                break;
            }
        }
        if (pRec == NULL)
        {
            response.statusLine = "HTTP/1.1 400 Bad Request";
            response.SetJsonBody("{\"error\":\"pattern_not_found\"}");
            return;
        }

        // paramsJson から各フィールドを読み込んで genParams に適用
        // （S1 で文字列のまま保存されているので、ここで解釈する）
        const std::string &pj = pRec->strParamsJson;
        int v = 0;
        if (JsonUtils::TryGetInt(pj, "width",        v)) genParams.width        = v;
        if (JsonUtils::TryGetInt(pj, "height",       v)) genParams.height       = v;
        if (JsonUtils::TryGetInt(pj, "floorAreaMin", v)) genParams.floorAreaMin = v;
        if (JsonUtils::TryGetInt(pj, "blockMin",     v)) genParams.blockMin     = v;
        if (JsonUtils::TryGetInt(pj, "blockMax",     v)) genParams.blockMax     = v;
        if (JsonUtils::TryGetInt(pj, "cutoffPercent",v)) genParams.cutoffPercent= v;
        if (JsonUtils::TryGetInt(pj, "maxRetries",   v)) genParams.maxRetries   = v;

        // roleMapJson から floor/wall を読む
        const std::string &rm = pRec->strRoleMapJson;
        if (JsonUtils::TryGetInt(rm, "floor", v)) floorGrpId = v;
        if (JsonUtils::TryGetInt(rm, "wall",  v)) wallGrpId  = v;
    }
    else if (!hasPatternId)
    {
        // patternId なし → リクエスト body の params がなければ 400
        std::string paramsStr;
        if (!JsonUtils::TryGetString(body, "params", paramsStr) || paramsStr.empty())
        {
            // params キーが文字列ではなくオブジェクトかもしれないので
            // FindKey で存在だけ確認する（JsonUtils にオブジェクト取得なし）
            if (JsonUtils::FindKey(body, "params") == std::string::npos)
            {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"patternId_or_params_required\"}");
                return;
            }
            // params キーはあるが文字列でない → body 全体から直接読む（後工程で処理）
        }
    }

    // リクエスト body の params オブジェクトで genParams を上書き
    // (JSON内 "params" キーの値がオブジェクトのため、body 全体に TryGetInt を試みる
    //  前に "params" サブオブジェクトを文字列として取り出せないケースもある。
    //  JsonUtils はネストオブジェクトの取り出しをサポートしないため、
    //  リクエスト body のトップレベルに params のフィールドが直置きされていると
    //  想定した場合の簡易フォールバックとして、body 全体からも読む。
    //  実際には body 文字列で "params":{...} の中身を簡易抽出する。)
    {
        // "params" サブオブジェクトの文字列を抽出する簡易実装
        // JsonUtils::FindKey で "params" のキー位置を探し、続く { ... } を切り出す
        size_t keyPos = JsonUtils::FindKey(body, "params");
        if (keyPos != std::string::npos)
        {
            // "params" の後の最初の '{' を探す
            size_t braceOpen = body.find('{', keyPos);
            if (braceOpen != std::string::npos)
            {
                // 対応する '}' を探す（ネスト深さ1を想定）
                int depth = 0;
                size_t braceClose = std::string::npos;
                for (size_t i = braceOpen; i < body.size(); ++i)
                {
                    if      (body[i] == '{') ++depth;
                    else if (body[i] == '}') { --depth; if (depth == 0) { braceClose = i; break; } }
                }
                if (braceClose != std::string::npos)
                {
                    std::string paramsJson = body.substr(braceOpen, braceClose - braceOpen + 1);
                    int v = 0;
                    if (JsonUtils::TryGetInt(paramsJson, "width",         v)) genParams.width         = v;
                    if (JsonUtils::TryGetInt(paramsJson, "height",        v)) genParams.height        = v;
                    if (JsonUtils::TryGetInt(paramsJson, "floorAreaMin",  v)) genParams.floorAreaMin  = v;
                    if (JsonUtils::TryGetInt(paramsJson, "blockMin",      v)) genParams.blockMin      = v;
                    if (JsonUtils::TryGetInt(paramsJson, "blockMax",      v)) genParams.blockMax      = v;
                    if (JsonUtils::TryGetInt(paramsJson, "cutoffPercent", v)) genParams.cutoffPercent = v;
                    if (JsonUtils::TryGetInt(paramsJson, "maxRetries",    v)) genParams.maxRetries    = v;
                }
            }
        }
    }

    // roleMap の上書き（リクエスト body から）
    {
        size_t keyPos = JsonUtils::FindKey(body, "roleMap");
        if (keyPos != std::string::npos)
        {
            size_t braceOpen = body.find('{', keyPos);
            if (braceOpen != std::string::npos)
            {
                int depth = 0;
                size_t braceClose = std::string::npos;
                for (size_t i = braceOpen; i < body.size(); ++i)
                {
                    if      (body[i] == '{') ++depth;
                    else if (body[i] == '}') { --depth; if (depth == 0) { braceClose = i; break; } }
                }
                if (braceClose != std::string::npos)
                {
                    std::string roleMapJson = body.substr(braceOpen, braceClose - braceOpen + 1);
                    int v = 0;
                    if (JsonUtils::TryGetInt(roleMapJson, "floor", v)) floorGrpId = v;
                    if (JsonUtils::TryGetInt(roleMapJson, "wall",  v)) wallGrpId  = v;
                }
            }
        }
    }

    // seed の上書き（リクエスト body から）
    {
        int seedVal = 0;
        if (JsonUtils::TryGetInt(body, "seed", seedVal))
            genParams.seed = static_cast<uint32_t>(seedVal);
    }

    // パラメータ簡易バリデーション
    if (genParams.width < 10 || genParams.height < 10
        || genParams.blockMin < 2 || genParams.blockMax < genParams.blockMin
        || genParams.floorAreaMin <= 0 || genParams.maxRetries <= 0)
    {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"invalid_params\"}");
        return;
    }

    // 面積上限チェック（maxRetries を大きくしてもグリッド全面積を超えられない）
    if (genParams.floorAreaMin > genParams.width * genParams.height)
    {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"floorAreaMin_exceeds_grid\"}");
        return;
    }

    // マップ生成実行
    std::vector<int> grid;
    int outW = 0, outH = 0, outSeed = 0;
    bool ok = MapGenCaveL3::GenerateCaveL3(genParams, grid, outW, outH, outSeed);

    if (!ok)
    {
        // リトライ上限超過
        response.statusLine = "HTTP/1.1 422 Unprocessable Entity";
        response.SetJsonBody("{\"error\":\"generation_failed\",\"reason\":\"max_retries_exceeded\"}");
        return;
    }

    // レスポンス JSON を組み立てる
    // tilesBase: 床→floorGrpId、壁→wallGrpId
    std::ostringstream oss;
    oss << '{';
    oss << "\"width\":"  << outW  << ',';
    oss << "\"height\":" << outH  << ',';
    oss << "\"seed\":"   << outSeed << ',';
    oss << "\"tilesBase\":[";

    int total = outW * outH;
    for (int i = 0; i < total; ++i)
    {
        oss << (grid[i] == 1 ? floorGrpId : wallGrpId);
        if (i + 1 < total)
            oss << ',';
    }

    oss << "]}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

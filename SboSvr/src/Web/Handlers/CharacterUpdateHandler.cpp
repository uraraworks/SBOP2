#include "stdafx.h"
#include "CharacterUpdateHandler.h"

#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoCharBase.h"
#include "myLib/myString.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 BOM を除去する
std::string RemoveUtf8Bom(std::string text)
{
        if (text.size() >= 3 &&
            static_cast<unsigned char>(text[0]) == 0xEF &&
            static_cast<unsigned char>(text[1]) == 0xBB &&
            static_cast<unsigned char>(text[2]) == 0xBF) {
                text.erase(0, 3);
        }
        return text;
}

// 制御文字（改行・タブ除く）を除去する
std::string RemoveControlCharacters(std::string text)
{
        text.erase(
                std::remove_if(
                        text.begin(),
                        text.end(),
                        [](unsigned char ch) {
                                if (ch == '\n' || ch == '\r' || ch == '\t') {
                                        return false;
                                }
                                return ch < 0x20;
                        }),
                text.end());
        return text;
}

#ifndef _UNICODE
// MBCS 環境用: 任意コードページ → UTF-8 変換を試みる
bool TryConvertToUtf8(const char *pszSource, int srcLength, UINT codePage, std::string &out)
{
        if ((pszSource == NULL) || (srcLength <= 0) || (codePage == 0)) {
                return false;
        }

        DWORD flags = (codePage == CP_UTF8) ? MB_ERR_INVALID_CHARS : 0;
        int wideLength = MultiByteToWideChar(codePage, flags, pszSource, srcLength, NULL, 0);
        if (wideLength <= 0) {
                return false;
        }

        std::vector<wchar_t> wide(static_cast<size_t>(wideLength));
        int converted = MultiByteToWideChar(codePage, flags, pszSource, srcLength, &wide[0], wideLength);
        if (converted <= 0) {
                return false;
        }

        if (!wide.empty() && wide.back() == L'\0') {
                wide.pop_back();
                converted = static_cast<int>(wide.size());
        }

        int utf8Length = WideCharToMultiByte(CP_UTF8, 0, &wide[0], converted, NULL, 0, NULL, NULL);
        if (utf8Length <= 0) {
                return false;
        }

        out.resize(static_cast<size_t>(utf8Length));
        WideCharToMultiByte(CP_UTF8, 0, &wide[0], converted, &out[0], utf8Length, NULL, NULL);
        return true;
}
#endif

// CmyString → UTF-8 std::string 変換
std::string ToUtf8String(const CmyString &value)
{
#ifdef _UNICODE
        CStringA utf8 = TStringToUtf8(static_cast<LPCTSTR>(value));
        if (utf8.IsEmpty()) {
                return std::string();
        }
        std::string converted(utf8.GetString(), static_cast<size_t>(utf8.GetLength()));
        return RemoveControlCharacters(RemoveUtf8Bom(converted));
#else
        LPCSTR pszSource = static_cast<LPCSTR>(value);
        if ((pszSource == NULL) || (pszSource[0] == '\0')) {
                return std::string();
        }

        int srcLength = static_cast<int>(std::strlen(pszSource));
        if (srcLength <= 0) {
                return std::string();
        }

        const UINT candidates[] = { CP_UTF8, 932u, CP_ACP };
        std::string utf8;
        for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); ++i) {
                if (TryConvertToUtf8(pszSource, srcLength, candidates[i], utf8)) {
                        return RemoveControlCharacters(RemoveUtf8Bom(utf8));
                }
        }

        return RemoveControlCharacters(RemoveUtf8Bom(std::string(pszSource, static_cast<size_t>(srcLength))));
#endif
}

// UTF-8 文字列を内部文字コードに変換して CmyString へ代入
bool SetStringFromUtf8(CmyString &out, const std::string &utf8)
{
        if (utf8.empty()) {
                out = _T("");
                return true;
        }
#ifdef _UNICODE
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) {
                return false;
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
        out = &wide[0];
#else
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) {
                return false;
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);

        int mbLen = WideCharToMultiByte(932, 0, &wide[0], wideLen, NULL, 0, NULL, NULL);
        if (mbLen <= 0) {
                return false;
        }
        std::vector<char> mb(static_cast<size_t>(mbLen) + 1, '\0');
        WideCharToMultiByte(932, 0, &wide[0], wideLen, &mb[0], mbLen, NULL, NULL);
        out = &mb[0];
#endif
        return true;
}

// URL パスから charId とサブリソース名を抽出する
// /api/characters/123         → charId=123, subResource=""
// /api/characters/123/status  → charId=123, subResource="status"
// 取得できなければ false を返す
bool ExtractCharIdAndSub(const std::string &path, int &outCharId, std::string &outSub)
{
        const std::string prefix = "/api/characters/";
        if (path.size() <= prefix.size()) {
                return false;
        }

        // クエリ文字列除去
        std::string rest = path.substr(prefix.size());
        size_t nQuery = rest.find('?');
        if (nQuery != std::string::npos) {
                rest = rest.substr(0, nQuery);
        }

        // {charId}[/{subResource}] を分割
        size_t nSlash = rest.find('/');
        std::string idStr;
        if (nSlash != std::string::npos) {
                idStr   = rest.substr(0, nSlash);
                outSub  = rest.substr(nSlash + 1);
        } else {
                idStr  = rest;
                outSub = "";
        }

        if (idStr.empty()) {
                return false;
        }

        char *pEnd = NULL;
        long val = std::strtol(idStr.c_str(), &pEnd, 10);
        if (pEnd == NULL || *pEnd != '\0' || val <= 0) {
                return false;
        }

        outCharId = static_cast<int>(val);
        return true;
}

// charId で CInfoCharBase* を検索して返す（Lock 済みの状態で呼ぶこと）
// 見つからなければ NULL を返す
CInfoCharBase *FindChar(CLibInfoCharSvr *pCharLib, int nCharId)
{
        int nTotal = pCharLib->GetCount();
        for (int i = 0; i < nTotal; ++i) {
                CInfoCharBase *pTmp = static_cast<CInfoCharBase *>(pCharLib->GetPtr(i));
                if (pTmp != NULL && static_cast<int>(pTmp->m_dwCharID) == nCharId) {
                        return pTmp;
                }
        }
        return NULL;
}

// 基本情報 JSON を構築する（PUT /api/characters/{charId} の成功レスポンス用）
std::string BuildBasicJson(const CInfoCharBase *pChar)
{
        std::ostringstream oss;
        oss << '{';
        oss << "\"charId\":"    << pChar->m_dwCharID  << ',';
        oss << "\"charName\":\"" << JsonUtils::Escape(ToUtf8String(pChar->m_strCharName)) << "\",";
        oss << "\"mapId\":"     << pChar->m_dwMapID   << ',';
        oss << "\"x\":"         << pChar->m_nMapX     << ',';
        oss << "\"y\":"         << pChar->m_nMapY     << ',';
        oss << "\"direction\":" << pChar->m_nDirection << ',';
        oss << "\"moveType\":"  << pChar->m_nMoveType  << ',';
        oss << "\"familyId\":"  << pChar->m_wFamilyID  << ',';
        oss << "\"grpSize\":"   << pChar->m_nGrpSize   << ',';
        oss << "\"sex\":"       << pChar->m_nSex       << ',';
        oss << "\"block\":"     << (pChar->m_bBlock ? "true" : "false") << ',';
        oss << "\"push\":"      << (pChar->m_bPush  ? "true" : "false") << ',';
        oss << "\"motionTypeId\":" << pChar->m_dwMotionTypeID;
        oss << '}';
        return oss.str();
}

// ステータス JSON を構築する
std::string BuildStatusJson(const CInfoCharBase *pChar)
{
        std::ostringstream oss;
        oss << '{';
        oss << "\"hp\":"                << pChar->m_dwHP              << ',';
        oss << "\"maxHp\":"             << pChar->m_dwMaxHP           << ',';
        oss << "\"sp\":"                << pChar->m_dwSP              << ',';
        oss << "\"maxSp\":"             << pChar->m_dwMaxSP           << ',';
        oss << "\"exp\":"               << pChar->m_dwExp             << ',';
        oss << "\"level\":"             << pChar->m_wLevel            << ',';
        oss << "\"stamina\":"           << pChar->m_wStamina          << ',';
        oss << "\"power\":"             << pChar->m_wPower            << ',';
        oss << "\"strength\":"          << pChar->m_wStrength         << ',';
        oss << "\"magic\":"             << pChar->m_wMagic            << ',';
        oss << "\"skillful\":"          << pChar->m_wSkillful         << ',';
        oss << "\"abilityAT\":"         << pChar->m_wAbillityAT       << ',';
        oss << "\"abilityDF\":"         << pChar->m_wAbillityDF       << ',';
        oss << "\"pAtack\":"            << pChar->m_wPAtack           << ',';
        oss << "\"pDefense\":"          << pChar->m_wPDefense         << ',';
        oss << "\"pMagic\":"            << pChar->m_wPMagic           << ',';
        oss << "\"pMagicDefense\":"     << pChar->m_wPMagicDefense    << ',';
        oss << "\"pHitAverage\":"       << pChar->m_wPHitAverage      << ',';
        oss << "\"pAvoidAverage\":"     << pChar->m_wPAvoidAverage    << ',';
        oss << "\"pCriticalAverage\":"  << pChar->m_wPCriticalAverage << ',';
        oss << "\"attrFire\":"          << pChar->m_wAttrFire         << ',';
        oss << "\"attrWind\":"          << pChar->m_wAttrWind         << ',';
        oss << "\"attrWater\":"         << pChar->m_wAttrWater        << ',';
        oss << "\"attrEarth\":"         << pChar->m_wAttrEarth        << ',';
        oss << "\"attrLight\":"         << pChar->m_wAttrLight        << ',';
        oss << "\"attrDark\":"          << pChar->m_wAttrDark;
        oss << '}';
        return oss.str();
}

// 装備 JSON を構築する
std::string BuildEquipmentJson(const CInfoCharBase *pChar)
{
        std::ostringstream oss;
        oss << '{';
        oss << "\"cloth\":"       << pChar->m_dwEquipItemIDCloth     << ',';
        oss << "\"accesory1\":"   << pChar->m_dwEquipItemIDAcce1     << ',';
        oss << "\"accesory2\":"   << pChar->m_dwEquipItemIDAcce2     << ',';
        oss << "\"armsRight\":"   << pChar->m_dwEquipItemIDArmsRight << ',';
        oss << "\"armsLeft\":"    << pChar->m_dwEquipItemIDArmsLeft  << ',';
        oss << "\"head\":"        << pChar->m_dwEquipItemIDHead;
        oss << '}';
        return oss.str();
}

// グラフィック JSON を構築する
std::string BuildGraphicsJson(const CInfoCharBase *pChar)
{
        std::ostringstream oss;
        oss << '{';
        oss << "\"npc\":"            << pChar->m_wGrpIDNPC          << ',';
        oss << "\"cloth\":"          << pChar->m_wGrpIDCloth        << ',';
        oss << "\"eye\":"            << pChar->m_wGrpIDEye          << ',';
        oss << "\"eyeColor\":"       << pChar->m_wGrpIDEyeColor     << ',';
        oss << "\"hairType\":"       << pChar->m_wGrpIDHairType     << ',';
        oss << "\"hairColor\":"      << pChar->m_wGrpIDHairColor    << ',';
        oss << "\"sp\":"             << pChar->m_wGrpIDSP           << ',';
        oss << "\"tmpMain\":"        << pChar->m_wGrpIDTmpMain      << ',';
        oss << "\"tmpSub\":"         << pChar->m_wGrpIDTmpSub       << ',';
        oss << "\"acce\":"           << pChar->m_wGrpIDAcce         << ',';
        oss << "\"armsMain\":"       << pChar->m_wGrpIDArmsMain     << ',';
        oss << "\"armsSub\":"        << pChar->m_wGrpIDArmsSub      << ',';
        oss << "\"armsLeftMain\":"   << pChar->m_wGrpIDArmsLeftMain << ',';
        oss << "\"armsLeftSub\":"    << pChar->m_wGrpIDArmsLeftSub  << ',';
        oss << "\"initNpc\":"        << pChar->m_wGrpIDInitNPC      << ',';
        oss << "\"initCloth\":"      << pChar->m_wGrpIDInitCloth    << ',';
        oss << "\"initEye\":"        << pChar->m_wGrpIDInitEye      << ',';
        oss << "\"initEyeColor\":"   << pChar->m_wGrpIDInitEyeColor << ',';
        oss << "\"initHairType\":"   << pChar->m_wGrpIDInitHairType << ',';
        oss << "\"initHairColor\":"  << pChar->m_wGrpIDInitHairColor << ',';
        oss << "\"initSp\":"         << pChar->m_wGrpIDInitSP;
        oss << '}';
        return oss.str();
}

} // namespace

// ---------------------------------------------------------------------------
// CCharacterUpdateHandler
// PUT /api/characters/{charId}[/status|/equipment|/graphics]
// ---------------------------------------------------------------------------

CCharacterUpdateHandler::CCharacterUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CCharacterUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証チェック
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
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

        // URL から charId とサブリソース名を取り出す
        int nCharId = 0;
        std::string subResource;
        if (!ExtractCharIdAndSub(request.path, nCharId, subResource)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_char_id\"}");
                return;
        }

        // サブリソースに応じて処理を振り分ける
        if (subResource == "status") {
                HandleStatus(request, response, nCharId);
        } else if (subResource == "equipment") {
                HandleEquipment(request, response, nCharId);
        } else if (subResource == "graphics") {
                HandleGraphics(request, response, nCharId);
        } else if (subResource == "account") {
                HandleAccount(request, response, nCharId);
        } else if (subResource == "admin") {
                HandleAdmin(request, response, nCharId);
        } else if (subResource == "disabled") {
                HandleDisabled(request, response, nCharId);
        } else if (subResource.empty()) {
                HandleBasic(request, response, nCharId);
        } else {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
        }
}

// ---------------------------------------------------------------------------
// 基本情報更新  PUT /api/characters/{charId}
// ---------------------------------------------------------------------------

void CCharacterUpdateHandler::HandleBasic(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // charName
        std::string strVal;
        if (JsonUtils::TryGetString(request.body, "charName", strVal)) {
                CmyString newName;
                if (SetStringFromUtf8(newName, strVal)) {
                        pChar->m_strCharName = newName;
                }
        }

        // block
        bool bVal = false;
        if (JsonUtils::TryGetBool(request.body, "block", bVal)) {
                pChar->m_bBlock = bVal ? TRUE : FALSE;
        }

        // push
        if (JsonUtils::TryGetBool(request.body, "push", bVal)) {
                pChar->m_bPush = bVal ? TRUE : FALSE;
        }

        int nVal = 0;

        if (JsonUtils::TryGetInt(request.body, "moveType", nVal)) {
                pChar->m_nMoveType = nVal;
        }
        if (JsonUtils::TryGetInt(request.body, "motionTypeId", nVal)) {
                pChar->m_dwMotionTypeID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "mapId", nVal)) {
                pChar->m_dwMapID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "x", nVal)) {
                pChar->m_nMapX = nVal;
        }
        if (JsonUtils::TryGetInt(request.body, "y", nVal)) {
                pChar->m_nMapY = nVal;
        }
        if (JsonUtils::TryGetInt(request.body, "direction", nVal)) {
                pChar->m_nDirection = nVal;
        }
        if (JsonUtils::TryGetInt(request.body, "familyId", nVal)) {
                pChar->m_wFamilyID = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "grpSize", nVal)) {
                pChar->m_nGrpSize = nVal;
        }
        if (JsonUtils::TryGetInt(request.body, "sex", nVal)) {
                pChar->m_nSex = nVal;
        }

        std::string json = BuildBasicJson(pChar);
        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// ステータス更新  PUT /api/characters/{charId}/status
// ---------------------------------------------------------------------------

void CCharacterUpdateHandler::HandleStatus(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        int nVal = 0;

        if (JsonUtils::TryGetInt(request.body, "hp", nVal)) {
                pChar->m_dwHP = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "maxHp", nVal)) {
                pChar->m_dwMaxHP = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "sp", nVal)) {
                pChar->m_dwSP = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "maxSp", nVal)) {
                pChar->m_dwMaxSP = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "exp", nVal)) {
                pChar->m_dwExp = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "level", nVal)) {
                pChar->m_wLevel = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "stamina", nVal)) {
                pChar->m_wStamina = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "power", nVal)) {
                pChar->m_wPower = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "strength", nVal)) {
                pChar->m_wStrength = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "magic", nVal)) {
                pChar->m_wMagic = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "skillful", nVal)) {
                pChar->m_wSkillful = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "abilityAT", nVal)) {
                pChar->m_wAbillityAT = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "abilityDF", nVal)) {
                pChar->m_wAbillityDF = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pAtack", nVal)) {
                pChar->m_wPAtack = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pDefense", nVal)) {
                pChar->m_wPDefense = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pMagic", nVal)) {
                pChar->m_wPMagic = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pMagicDefense", nVal)) {
                pChar->m_wPMagicDefense = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pHitAverage", nVal)) {
                pChar->m_wPHitAverage = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pAvoidAverage", nVal)) {
                pChar->m_wPAvoidAverage = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "pCriticalAverage", nVal)) {
                pChar->m_wPCriticalAverage = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "attrFire", nVal)) {
                pChar->m_wAttrFire = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "attrWind", nVal)) {
                pChar->m_wAttrWind = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "attrWater", nVal)) {
                pChar->m_wAttrWater = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "attrEarth", nVal)) {
                pChar->m_wAttrEarth = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "attrLight", nVal)) {
                pChar->m_wAttrLight = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "attrDark", nVal)) {
                pChar->m_wAttrDark = static_cast<WORD>(nVal);
        }

        std::string json = BuildStatusJson(pChar);
        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// 装備更新  PUT /api/characters/{charId}/equipment
// ---------------------------------------------------------------------------

void CCharacterUpdateHandler::HandleEquipment(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        int nVal = 0;

        if (JsonUtils::TryGetInt(request.body, "cloth", nVal)) {
                pChar->m_dwEquipItemIDCloth = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "accesory1", nVal)) {
                pChar->m_dwEquipItemIDAcce1 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "accesory2", nVal)) {
                pChar->m_dwEquipItemIDAcce2 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "armsRight", nVal)) {
                pChar->m_dwEquipItemIDArmsRight = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "armsLeft", nVal)) {
                pChar->m_dwEquipItemIDArmsLeft = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "head", nVal)) {
                pChar->m_dwEquipItemIDHead = static_cast<DWORD>(nVal);
        }

        std::string json = BuildEquipmentJson(pChar);
        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// グラフィック更新  PUT /api/characters/{charId}/graphics
// ---------------------------------------------------------------------------
// NOTE: グラフィック変更時の全クライアントへのパケット通知は未実装（保留）。
//       設計書 docs/web-admin-char-plan.md §8 参照。

void CCharacterUpdateHandler::HandleGraphics(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        int nVal = 0;

        if (JsonUtils::TryGetInt(request.body, "npc", nVal)) {
                pChar->m_wGrpIDNPC = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "cloth", nVal)) {
                pChar->m_wGrpIDCloth = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "eye", nVal)) {
                pChar->m_wGrpIDEye = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "eyeColor", nVal)) {
                pChar->m_wGrpIDEyeColor = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "hairType", nVal)) {
                pChar->m_wGrpIDHairType = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "hairColor", nVal)) {
                pChar->m_wGrpIDHairColor = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "sp", nVal)) {
                pChar->m_wGrpIDSP = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "tmpMain", nVal)) {
                pChar->m_wGrpIDTmpMain = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "tmpSub", nVal)) {
                pChar->m_wGrpIDTmpSub = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "acce", nVal)) {
                pChar->m_wGrpIDAcce = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "armsMain", nVal)) {
                pChar->m_wGrpIDArmsMain = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "armsSub", nVal)) {
                pChar->m_wGrpIDArmsSub = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "armsLeftMain", nVal)) {
                pChar->m_wGrpIDArmsLeftMain = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "armsLeftSub", nVal)) {
                pChar->m_wGrpIDArmsLeftSub = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initNpc", nVal)) {
                pChar->m_wGrpIDInitNPC = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initCloth", nVal)) {
                pChar->m_wGrpIDInitCloth = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initEye", nVal)) {
                pChar->m_wGrpIDInitEye = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initEyeColor", nVal)) {
                pChar->m_wGrpIDInitEyeColor = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initHairType", nVal)) {
                pChar->m_wGrpIDInitHairType = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initHairColor", nVal)) {
                pChar->m_wGrpIDInitHairColor = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "initSp", nVal)) {
                pChar->m_wGrpIDInitSP = static_cast<WORD>(nVal);
        }

        std::string json = BuildGraphicsJson(pChar);
        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// アカウント紐付け変更  PUT /api/characters/{charId}/account
// body: { "accountId": <number> }
// accountId=0 は紐付け解除扱い（m_dwAccountID を 0 に設定）
// NPC キャラクターへの紐付けは 400 を返す
// ---------------------------------------------------------------------------

void CCharacterUpdateHandler::HandleAccount(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        int nAccountId = -1;
        if (!JsonUtils::TryGetInt(request.body, "accountId", nAccountId) || nAccountId < 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_accountId\"}");
                return;
        }

        // accountId > 0 の場合、アカウントライブラリに存在するか確認する
        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // accountId > 0 なら存在確認
        if (nAccountId > 0) {
                pAccountLib->Enter();
                PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(nAccountId));
                pAccountLib->Leave();
                if (pAcc == NULL) {
                        response.statusLine = "HTTP/1.1 400 Bad Request";
                        response.SetJsonBody("{\"error\":\"account_not_found\"}");
                        return;
                }
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // NPC には紐付け不可
        if (pChar->m_bNPC && nAccountId > 0) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"npc_cannot_have_account\"}");
                return;
        }

        pChar->m_dwAccountID = static_cast<DWORD>(nAccountId);

        std::ostringstream oss;
        oss << "{\"charId\":" << nCharId << ",\"accountId\":" << nAccountId << '}';
        std::string json = oss.str();

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// 管理者権限レベル設定  PUT /api/characters/{charId}/admin
// body: { "adminLevel": <number> }
// adminLevel 設定先はキャラクターに紐付くアカウントの m_nAdminLevel
// キャラクターが NPC または accountId=0 の場合は 400 を返す
// ---------------------------------------------------------------------------

void CCharacterUpdateHandler::HandleAdmin(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        int nAdminLevel = -1;
        if (!JsonUtils::TryGetInt(request.body, "adminLevel", nAdminLevel) || nAdminLevel < 0 || nAdminLevel > 255) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_adminLevel\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();
        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // NPC は管理者権限設定不可
        if (pChar->m_bNPC) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"npc_cannot_have_admin\"}");
                return;
        }

        DWORD dwAccountId = pChar->m_dwAccountID;
        pCharLib->Leave();

        if (dwAccountId == 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"no_account_linked\"}");
                return;
        }

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(dwAccountId);
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"account_not_found\"}");
                return;
        }

        pAcc->m_nAdminLevel = nAdminLevel;
        pAccountLib->Leave();

        std::ostringstream oss;
        oss << "{\"charId\":" << nCharId << ",\"accountId\":" << dwAccountId << ",\"adminLevel\":" << nAdminLevel << '}';

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// キャラ無効化/解除  PUT /api/characters/{charId}/disabled
// body: { "disabled": <bool>, "reason"?: <string> }
// 無効化先はキャラクターに紐付くアカウントの m_bDisable フラグ
// キャラクターが NPC または accountId=0 の場合は 400 を返す
// NOTE: reason フィールドは受け付けるが現行実装では保存場所が無いため無視する。
//       将来の拡張のためにパラメータとして定義しておく。
// ---------------------------------------------------------------------------

void CCharacterUpdateHandler::HandleDisabled(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        bool bDisabled = false;
        if (!JsonUtils::TryGetBool(request.body, "disabled", bDisabled)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_disabled\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();
        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // NPC は無効化不可
        if (pChar->m_bNPC) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"npc_cannot_be_disabled\"}");
                return;
        }

        DWORD dwAccountId = pChar->m_dwAccountID;
        pCharLib->Leave();

        if (dwAccountId == 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"no_account_linked\"}");
                return;
        }

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(dwAccountId);
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"account_not_found\"}");
                return;
        }

        pAcc->m_bDisable = bDisabled ? TRUE : FALSE;
        pAccountLib->Leave();

        std::ostringstream oss;
        oss << "{\"charId\":" << nCharId << ",\"accountId\":" << dwAccountId
            << ",\"disabled\":" << (bDisabled ? "true" : "false") << '}';

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

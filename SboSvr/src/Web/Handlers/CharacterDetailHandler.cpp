#include "StdAfx.h"
#include "CharacterDetailHandler.h"

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

// URL パスから charId を抽出する
// /api/characters/123 → 123
// 取得できなければ false を返す
bool ExtractCharIdFromPath(const std::string &path, int &outCharId)
{
        const std::string prefix = "/api/characters/";
        if (path.size() <= prefix.size()) {
                return false;
        }

        // クエリ文字列除去
        std::string idStr = path.substr(prefix.size());
        size_t nQuery = idStr.find('?');
        if (nQuery != std::string::npos) {
                idStr = idStr.substr(0, nQuery);
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

} // namespace

// ---------------------------------------------------------------------------
// CCharacterDetailHandler  GET /api/characters/{charId}
// ---------------------------------------------------------------------------

CCharacterDetailHandler::CCharacterDetailHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CCharacterDetailHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        // URL パスから charId を取り出す
        int nCharId = 0;
        if (!ExtractCharIdFromPath(request.path, nCharId)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_char_id\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // キャラライブラリをロックして指定 charId を検索する
        pCharLib->Enter();

        const CInfoCharBase *pChar = NULL;
        int nTotal = pCharLib->GetCount();
        for (int i = 0; i < nTotal; ++i) {
                const CInfoCharBase *pTmp = static_cast<const CInfoCharBase *>(pCharLib->GetPtr(i));
                if (pTmp != NULL && static_cast<int>(pTmp->m_dwCharID) == nCharId) {
                        pChar = pTmp;
                        break;
                }
        }

        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // JSON レスポンスを構築する
        std::string charName = ToUtf8String(pChar->m_strCharName);

        std::ostringstream oss;
        oss << '{';

        // --- 基本情報 ---
        oss << "\"charId\":"    << pChar->m_dwCharID  << ',';
        oss << "\"charName\":\"" << JsonUtils::Escape(charName) << "\",";
        oss << "\"mapId\":"     << pChar->m_dwMapID   << ',';
        oss << "\"x\":"         << pChar->m_nMapX     << ',';
        oss << "\"y\":"         << pChar->m_nMapY     << ',';
        oss << "\"direction\":" << pChar->m_nDirection << ',';
        oss << "\"moveState\":" << pChar->m_nMoveState << ',';
        oss << "\"moveType\":"  << pChar->m_nMoveType  << ',';
        oss << "\"familyId\":"  << pChar->m_wFamilyID  << ',';
        oss << "\"grpSize\":"   << pChar->m_nGrpSize   << ',';
        oss << "\"sex\":"       << pChar->m_nSex       << ',';
        oss << "\"isNpc\":"     << (pChar->m_bNPC  ? "true" : "false") << ',';
        oss << "\"block\":"     << (pChar->m_bBlock ? "true" : "false") << ',';
        oss << "\"push\":"      << (pChar->m_bPush  ? "true" : "false") << ',';
        oss << "\"accountId\":" << pChar->m_dwAccountID << ',';

        // --- ステータス ---
        oss << "\"status\":{";
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
        oss << "},";

        // --- 装備 ---
        oss << "\"equipment\":{";
        oss << "\"cloth\":"       << pChar->m_dwEquipItemIDCloth     << ',';
        oss << "\"accesory1\":"   << pChar->m_dwEquipItemIDAcce1     << ',';
        oss << "\"accesory2\":"   << pChar->m_dwEquipItemIDAcce2     << ',';
        oss << "\"armsRight\":"   << pChar->m_dwEquipItemIDArmsRight << ',';
        oss << "\"armsLeft\":"    << pChar->m_dwEquipItemIDArmsLeft  << ',';
        oss << "\"head\":"        << pChar->m_dwEquipItemIDHead;
        oss << "},";

        // --- グラフィック ---
        oss << "\"graphics\":{";
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
        oss << "},";

        // --- 移動 ---
        oss << "\"movement\":{";
        oss << "\"maxItemCount\":"       << pChar->m_nMaxItemCount      << ',';
        oss << "\"dropItemAverage\":"    << pChar->m_nDropItemAverage   << ',';
        oss << "\"moveAverage\":"        << pChar->m_nMoveAverage       << ',';
        oss << "\"moveAverageBattle\":"  << pChar->m_nMoveAverageBattle << ',';
        oss << "\"moveWait\":"           << pChar->m_dwMoveWait         << ',';
        oss << "\"moveWaitBattle\":"     << pChar->m_dwMoveWaitBattle   << ',';
        oss << "\"searchDistanceCX\":"   << pChar->m_sizeSearchDistance.cx << ',';
        oss << "\"searchDistanceCY\":"   << pChar->m_sizeSearchDistance.cy << ',';
        oss << "\"motionTypeId\":"       << pChar->m_dwMotionTypeID;
        oss << "},";

        // --- NPC 発生 ---
        oss << "\"npcSpawn\":{";
        oss << "\"putCycle\":"     << pChar->m_dwPutCycle   << ',';
        oss << "\"putMoveType\":"  << pChar->m_nPutMoveType << ',';
        oss << "\"maxPutCount\":"  << pChar->m_nMaxPutCount << ',';
        oss << "\"putAverage\":"   << pChar->m_nPutAverage  << ',';
        oss << "\"putAreaX\":"     << pChar->m_ptPutArea.x  << ',';
        oss << "\"putAreaY\":"     << pChar->m_ptPutArea.y;
        oss << "}";

        oss << '}';

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

#include "StdAfx.h"
#include "ImageCatalogHandler.h"

#include "GlobalDefine.h"
#include "Web/Handlers/SpriteSheetHandler.h"

#include <sstream>
#include <string>

namespace
{
// 画像カテゴリの 1 レコード
struct ImageCategoryEntry
{
        int         nIDMain;        // GRPIDMAIN_* 値
        const char *pszKey;         // 機械向け識別子（英字）
        const char *pszLabel;       // 画面表示用ラベル（UTF-8 日本語）
        int         nHintMaxSub;    // 参考用の最大サブID（-1 なら上限未設定）
};

// サーバー側は画像ピクセルデータを持たないので、最大件数は
// 「明らかに足りる上限目安」を静的に定義する。picker の数値入力の
// min / max やスピナー、グリッド行数の目安として使用する。
const ImageCategoryEntry kCategories[] = {
        { GRPIDMAIN_NONE,         "none",        "未設定",           0  },
        { GRPIDMAIN_CHAR,         "char",        "キャラ",           63 },
        { GRPIDMAIN_WEAPON,       "weapon",      "武器",             63 },
        { GRPIDMAIN_WEAPON_BOW,   "weaponBow",   "武器(弓)",         63 },
        { GRPIDMAIN_EFFECT32,     "effect32",    "エフェクト(32)",   63 },
        { GRPIDMAIN_WEAPON_GLOVE, "weaponGlove", "武器(打撃)",       63 },
        { GRPIDMAIN_WEAPON_ETC,   "weaponEtc",   "武器(その他)",     63 },
        { GRPIDMAIN_NPC,          "npc",         "NPC",              255 },
        { GRPIDMAIN_EFCBALLOON,   "efcBalloon",  "噴出し",           63 },
        { GRPIDMAIN_2X2_CHAR,     "char2x2",     "キャラ(2x2)",      255 },
        { GRPIDMAIN_2X2_CLOTH,    "cloth2x2",    "服(2x2)",          255 },
        { GRPIDMAIN_2X2_EYE,      "eye2x2",      "目(2x2)",          63 },
        { GRPIDMAIN_2X2_HAIR,     "hair2x2",     "髪(2x2)",          255 },
        { GRPIDMAIN_2X2_SPCLOTH,  "spCloth2x2",  "特殊服(2x2)",      255 },
        { GRPIDMAIN_2X2_SPHAIR,   "spHair2x2",   "特殊髪(2x2)",      255 },
        { GRPIDMAIN_2X2_ARMS,     "arms2x2",     "持ち物(2x2)",      255 },
        { GRPIDMAIN_2X2_SHIELD,   "shield2x2",   "盾(2x2)",          255 },
        { GRPIDMAIN_2X2_ARMSSP,   "armsSp2x2",   "特殊持ち物(2x2)",  255 },
        { GRPIDMAIN_2X2_BOW,      "bow2x2",      "弓(2x2)",          255 },
        { GRPIDMAIN_2X2_NPC,      "npc2x2",      "NPC(2x2)",         255 },
        { GRPIDMAIN_EFFECT64,     "effect64",    "エフェクト(64)",   63 },
        { GRPIDMAIN_ICON32,       "icon32",      "アイコン(32)",     63 },
        // マップ系: GRPIDMAIN_* に列挙値が無いため idMain = 0 を使用
        { 0,                      "mapParts",    "マップパーツ",      63 },
        { 0,                      "mapShadow",   "マップ影",          63 },
};

// 簡易 JSON エスケープ（このハンドラは静的文字列しか扱わないので最小実装）
std::string EscapeJson(const char *pszText)
{
        std::string out;
        if (pszText == NULL) {
                return out;
        }
        for (const char *p = pszText; *p != '\0'; ++p) {
                unsigned char ch = static_cast<unsigned char>(*p);
                switch (ch) {
                case '"':  out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\b': out += "\\b";  break;
                case '\f': out += "\\f";  break;
                case '\n': out += "\\n";  break;
                case '\r': out += "\\r";  break;
                case '\t': out += "\\t";  break;
                default:
                        if (ch < 0x20) {
                                char buf[8];
                                sprintf_s(buf, sizeof(buf), "\\u%04x", ch);
                                out += buf;
                        } else {
                                out += static_cast<char>(ch);
                        }
                        break;
                }
        }
        return out;
}

} // namespace

void CImageCatalogHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
        // SpriteSheetHandler のシングルトンから cellSize/countX/countY/sheetCount を突合する
        CGrpResourceProvider &provider = CGrpResourceProvider::GetInstance();

        std::ostringstream oss;
        oss << "{\"categories\":[";

        const size_t nCount = sizeof(kCategories) / sizeof(kCategories[0]);
        for (size_t i = 0; i < nCount; ++i) {
                if (i > 0) {
                        oss << ',';
                }
                const ImageCategoryEntry &e = kCategories[i];

                // SpriteSheetHandler のカテゴリテーブルから cellSize/countX/countY を取得
                int nCellSize = 0, nCountX = 0, nCountY = 0;
                bool bHasLayout = provider.GetCategoryLayout(
                    e.pszKey, nCellSize, nCountX, nCountY);

                int nSheetCount = provider.GetSheetCount(e.pszKey);

                oss << '{'
                    << "\"idMain\":"     << e.nIDMain << ','
                    << "\"key\":\""      << EscapeJson(e.pszKey)   << "\","
                    << "\"label\":\""    << EscapeJson(e.pszLabel) << "\","
                    << "\"hintMaxSub\":" << e.nHintMaxSub << ',';

                if (bHasLayout) {
                        oss << "\"cellSize\":" << nCellSize << ','
                            << "\"countX\":"   << nCountX   << ','
                            << "\"countY\":"   << nCountY   << ',';
                }

                oss << "\"sheetCount\":" << nSheetCount;

                if (nSheetCount > 0) {
                        oss << ','
                            << "\"sheetUrl\":\"/api/assets/sprites/"
                            << EscapeJson(e.pszKey) << "\"";
                }

                oss << '}';
        }
        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

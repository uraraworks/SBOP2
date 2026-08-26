#include "StdAfx.h"
#include "ImageCatalogHandler.h"

#include "GlobalDefine.h"
#include "GrpLayout.h"
#include "Web/Handlers/SpriteSheetHandler.h"

#include <sstream>
#include <string>

namespace
{

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

        size_t nCount = 0;
        const SGrpLayoutDef *pTable = GrpLayout_GetTable(&nCount);
        for (size_t i = 0; i < nCount; ++i) {
                if (i > 0) {
                        oss << ',';
                }
                const SGrpLayoutDef &e = pTable[i];

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

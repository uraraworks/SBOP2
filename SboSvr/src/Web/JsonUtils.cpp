#include "StdAfx.h"
#include "JsonUtils.h"

#include <cctype>
#include <cstdio>

namespace
{
size_t SkipWhitespace(const std::string &text, size_t pos)
{
        size_t nSize = text.size();
        while ((pos < nSize) && std::isspace(static_cast<unsigned char>(text[pos]))) {
                ++pos;
        }
        return pos;
}

// コードポイントを UTF-8 バイト列にエンコードして追加する
void AppendUtf8CodePoint(std::string &out, unsigned int codePoint)
{
        if (codePoint <= 0x7F) {
                out.push_back(static_cast<char>(codePoint));
        } else if (codePoint <= 0x7FF) {
                out.push_back(static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
        } else if (codePoint <= 0xFFFF) {
                out.push_back(static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
        } else {
                out.push_back(static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
        }
}

// 4桁の16進数を読んでコードポイントを返す（失敗時は false）
bool ParseHex4(const std::string &text, size_t pos, unsigned int &outValue)
{
        if (pos + 4 > text.size()) {
                return false;
        }
        unsigned int value = 0;
        for (size_t i = 0; i < 4; ++i) {
                char ch = text[pos + i];
                value <<= 4;
                if (ch >= '0' && ch <= '9') {
                        value |= static_cast<unsigned int>(ch - '0');
                } else if (ch >= 'a' && ch <= 'f') {
                        value |= static_cast<unsigned int>(ch - 'a' + 10);
                } else if (ch >= 'A' && ch <= 'F') {
                        value |= static_cast<unsigned int>(ch - 'A' + 10);
                } else {
                        return false;
                }
        }
        outValue = value;
        return true;
}

bool ExtractQuotedString(const std::string &text, size_t startPos, std::string &outValue, size_t *pNextPos)
{
        outValue.clear();
        if (startPos >= text.size() || text[startPos] != '"') {
                return false;
        }
        ++startPos;
        for (size_t i = startPos; i < text.size(); ++i) {
                char ch = text[i];
                if (ch == '\\') {
                        if (i + 1 >= text.size()) {
                                outValue.clear();
                                return false;
                        }
                        char nextCh = text[i + 1];
                        switch (nextCh) {
                        case '"':
                                outValue.push_back('"');
                                break;
                        case '\\':
                                outValue.push_back('\\');
                                break;
                        case '/':
                                outValue.push_back('/');
                                break;
                        case 'b':
                                outValue.push_back('\b');
                                break;
                        case 'f':
                                outValue.push_back('\f');
                                break;
                        case 'n':
                                outValue.push_back('\n');
                                break;
                        case 'r':
                                outValue.push_back('\r');
                                break;
                        case 't':
                                outValue.push_back('\t');
                                break;
                        case 'u':
                                {
                                        unsigned int codePoint = 0;
                                        if (!ParseHex4(text, i + 2, codePoint)) {
                                                outValue.clear();
                                                return false;
                                        }
                                        // サロゲートペアの処理
                                        if (codePoint >= 0xD800 && codePoint <= 0xDBFF &&
                                            i + 6 + 6 <= text.size() &&
                                            text[i + 6] == '\\' && text[i + 6 + 1] == 'u') {
                                                unsigned int lowSurrogate = 0;
                                                if (ParseHex4(text, i + 8, lowSurrogate) &&
                                                    lowSurrogate >= 0xDC00 && lowSurrogate <= 0xDFFF) {
                                                        unsigned int combined = 0x10000 +
                                                                ((codePoint - 0xD800) << 10) +
                                                                (lowSurrogate - 0xDC00);
                                                        AppendUtf8CodePoint(outValue, combined);
                                                        i += 11;
                                                        continue;
                                                }
                                        }
                                        AppendUtf8CodePoint(outValue, codePoint);
                                        i += 5;
                                        continue;
                                }
                        default:
                                outValue.clear();
                                return false;
                        }
                        ++i;
                        continue;
                }
                if (ch == '"') {
                        if (pNextPos != NULL) {
                                *pNextPos = i + 1;
                        }
                        return true;
                }
                outValue.push_back(ch);
        }
        outValue.clear();
        return false;
}
}

namespace JsonUtils
{
std::string Escape(const std::string &value)
{
        std::string escaped;
        escaped.reserve(value.size() + 8);
        for (size_t i = 0; i < value.size(); ++i) {
                unsigned char ch = static_cast<unsigned char>(value[i]);
                switch (ch) {
                case '\\':
                        escaped.append("\\\\");
                        break;
                case '\"':
                        escaped.append("\\\"");
                        break;
                case '\n':
                        escaped.append("\\n");
                        break;
                case '\r':
                        escaped.append("\\r");
                        break;
                case '\t':
                        escaped.append("\\t");
                        break;
                default:
                        if (ch < 0x20) {
                                char buffer[7];
                                std::snprintf(buffer, sizeof(buffer), "\\u%04X", ch);
                                escaped.append(buffer);
                        } else {
                                escaped.push_back(static_cast<char>(ch));
                        }
                        break;
                }
        }
        return escaped;
}

size_t FindKey(const std::string &json, const std::string &key)
{
        std::string pattern = "\"" + key + "\"";
        return json.find(pattern);
}

bool IsNull(const std::string &json, const std::string &key)
{
        size_t nKeyPos = FindKey(json, key);
        if (nKeyPos == std::string::npos) {
                return false;
        }

        size_t nColonPos = json.find(':', nKeyPos + key.size() + 2);
        if (nColonPos == std::string::npos) {
                return false;
        }
        nColonPos = SkipWhitespace(json, nColonPos + 1);
        if (nColonPos + 4 > json.size()) {
                return false;
        }
        return json.compare(nColonPos, 4, "null") == 0;
}

bool TryGetString(const std::string &json, const std::string &key, std::string &outValue)
{
        size_t nKeyPos = FindKey(json, key);
        if (nKeyPos == std::string::npos) {
                return false;
        }

        size_t nColonPos = json.find(':', nKeyPos + key.size() + 2);
        if (nColonPos == std::string::npos) {
                return false;
        }

        nColonPos = SkipWhitespace(json, nColonPos + 1);
        if (nColonPos >= json.size()) {
                return false;
        }

        if (json[nColonPos] != '"') {
                return false;
        }

        std::string value;
        if (!ExtractQuotedString(json, nColonPos, value, NULL)) {
                return false;
        }

        outValue.swap(value);
        return true;
}

bool TryGetInt(const std::string &json, const std::string &key, int &outValue)
{
        size_t nKeyPos = FindKey(json, key);
        if (nKeyPos == std::string::npos) {
                return false;
        }

        size_t nColonPos = json.find(':', nKeyPos + key.size() + 2);
        if (nColonPos == std::string::npos) {
                return false;
        }

        nColonPos = SkipWhitespace(json, nColonPos + 1);
        if (nColonPos >= json.size()) {
                return false;
        }

        char ch = json[nColonPos];
        if (ch != '-' && !std::isdigit(static_cast<unsigned char>(ch))) {
                return false;
        }

        char *pEnd = NULL;
        long value = std::strtol(json.c_str() + nColonPos, &pEnd, 10);
        if (pEnd == NULL || pEnd == json.c_str() + nColonPos) {
                return false;
        }

        outValue = static_cast<int>(value);
        return true;
}

bool TryGetBool(const std::string &json, const std::string &key, bool &outValue)
{
        size_t nKeyPos = FindKey(json, key);
        if (nKeyPos == std::string::npos) {
                return false;
        }

        size_t nColonPos = json.find(':', nKeyPos + key.size() + 2);
        if (nColonPos == std::string::npos) {
                return false;
        }

        nColonPos = SkipWhitespace(json, nColonPos + 1);
        if (nColonPos >= json.size()) {
                return false;
        }

        if (nColonPos + 4 <= json.size() && json.compare(nColonPos, 4, "true") == 0) {
                outValue = true;
                return true;
        }
        if (nColonPos + 5 <= json.size() && json.compare(nColonPos, 5, "false") == 0) {
                outValue = false;
                return true;
        }

        return false;
}

bool TryGetStringArray(const std::string &json, const std::string &key, std::vector<std::string> &outValues)
{
        outValues.clear();
        size_t nKeyPos = FindKey(json, key);
        if (nKeyPos == std::string::npos) {
                return false;
        }

        size_t nColonPos = json.find(':', nKeyPos + key.size() + 2);
        if (nColonPos == std::string::npos) {
                return false;
        }
        nColonPos = SkipWhitespace(json, nColonPos + 1);
        if (nColonPos >= json.size() || json[nColonPos] != '[') {
                return false;
        }
        ++nColonPos;

        std::vector<std::string> values;
        while (nColonPos < json.size()) {
                nColonPos = SkipWhitespace(json, nColonPos);
                if (nColonPos >= json.size()) {
                        return false;
                }
                if (json[nColonPos] == ']') {
                        ++nColonPos;
                        break;
                }
                if (json[nColonPos] != '"') {
                        return false;
                }

                std::string element;
                size_t nNextPos = 0;
                if (!ExtractQuotedString(json, nColonPos, element, &nNextPos)) {
                        return false;
                }
                values.push_back(element);

                nColonPos = nNextPos;
                nColonPos = SkipWhitespace(json, nColonPos);
                if (nColonPos >= json.size()) {
                        return false;
                }
                if (json[nColonPos] == ',') {
                        ++nColonPos;
                        continue;
                }
                if (json[nColonPos] == ']') {
                        ++nColonPos;
                        break;
                }
                return false;
        }

        outValues.swap(values);
        return true;
}
}

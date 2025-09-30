#include "stdafx.h"
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

bool ExtractQuotedString(const std::string &text, size_t startPos, std::string &outValue, size_t *pNextPos)
{
        outValue.clear();
        if (startPos >= text.size() || text[startPos] != '"') {
                return false;
        }
        ++startPos;
        bool bEscape = false;
        for (size_t i = startPos; i < text.size(); ++i) {
                char ch = text[i];
                if (bEscape) {
                        outValue.push_back(ch);
                        bEscape = false;
                        continue;
                }
                if (ch == '\\') {
                        bEscape = true;
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


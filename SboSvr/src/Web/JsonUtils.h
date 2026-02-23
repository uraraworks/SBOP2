#pragma once

#include <string>
#include <vector>

namespace JsonUtils
{
std::string Escape(const std::string &value);
bool TryGetString(const std::string &json, const std::string &key, std::string &outValue);
bool TryGetStringArray(const std::string &json, const std::string &key, std::vector<std::string> &outValues);
bool TryGetInt(const std::string &json, const std::string &key, int &outValue);
bool TryGetBool(const std::string &json, const std::string &key, bool &outValue);
size_t FindKey(const std::string &json, const std::string &key);
bool IsNull(const std::string &json, const std::string &key);
}


#pragma once

#include <string>
#include <vector>

namespace JsonUtils
{
std::string Escape(const std::string &value);
bool TryGetString(const std::string &json, const std::string &key, std::string &outValue);
bool TryGetStringArray(const std::string &json, const std::string &key, std::vector<std::string> &outValues);
size_t FindKey(const std::string &json, const std::string &key);
}


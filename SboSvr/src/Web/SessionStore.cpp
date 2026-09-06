#include "StdAfx.h"
#include "SessionStore.h"
#include "Platform/SvrPlatform.h"

#include <map>
#include <mutex>

namespace
{
/// セッションの有効期限（秒）: 8時間
const time_t kSessionLifetimeSeconds = 8 * 60 * 60;

/// 同時に保持できるセッション数の上限
const size_t kMaxSessions = 256;

std::map<std::string, SessionStore::SessionInfo> &GetSessionMap()
{
        static std::map<std::string, SessionStore::SessionInfo> sessions;
        return sessions;
}

std::mutex &GetSessionMutex()
{
        static std::mutex mtx;
        return mtx;
}

/// @brief バイト列を小文字16進文字列に変換する。
std::string ToHexLower(const unsigned char *pData, size_t nLength)
{
        static const char kHexTable[] = "0123456789abcdef";
        std::string result;
        result.reserve(nLength * 2);
        for (size_t i = 0; i < nLength; ++i) {
                result += kHexTable[(pData[i] >> 4) & 0x0F];
                result += kHexTable[pData[i] & 0x0F];
        }
        return result;
}

/// @brief 期限切れエントリを一掃する（呼び出し元でロック取得済みであること）。
void PurgeExpiredLocked(time_t now)
{
        std::map<std::string, SessionStore::SessionInfo> &sessions = GetSessionMap();
        for (std::map<std::string, SessionStore::SessionInfo>::iterator it = sessions.begin(); it != sessions.end(); ) {
                if (it->second.timeExpire <= now) {
                        it = sessions.erase(it);
                } else {
                        ++it;
                }
        }
}

/// @brief 上限を超える場合、最も期限が古いセッションを削除する（呼び出し元でロック取得済みであること）。
void EvictOldestIfFullLocked()
{
        std::map<std::string, SessionStore::SessionInfo> &sessions = GetSessionMap();
        if (sessions.size() < kMaxSessions) {
                return;
        }

        std::map<std::string, SessionStore::SessionInfo>::iterator oldestIt = sessions.end();
        for (std::map<std::string, SessionStore::SessionInfo>::iterator it = sessions.begin(); it != sessions.end(); ++it) {
                if ((oldestIt == sessions.end()) || (it->second.timeExpire < oldestIt->second.timeExpire)) {
                        oldestIt = it;
                }
        }

        if (oldestIt != sessions.end()) {
                sessions.erase(oldestIt);
        }
}
}

namespace SessionStore
{
std::string Create(unsigned int dwAccountID, const char *pszLoginId)
{
        unsigned char randomBytes[32];
        // 呼び出し側(ここ)は失敗時に rand() 等へフォールバックしないこと。
        if (!SboPlatform::GenerateRandomBytes(randomBytes, sizeof(randomBytes))) {
                return std::string();
        }

        std::string token = ToHexLower(randomBytes, sizeof(randomBytes));

        SessionInfo info;
        info.dwAccountID = dwAccountID;
        info.loginId = (pszLoginId != NULL) ? pszLoginId : "";
        info.timeExpire = time(NULL) + kSessionLifetimeSeconds;

        std::lock_guard<std::mutex> lock(GetSessionMutex());
        PurgeExpiredLocked(time(NULL));
        EvictOldestIfFullLocked();
        GetSessionMap()[token] = info;

        return token;
}

bool Touch(const std::string &token, SessionInfo &outInfo)
{
        if (token.empty()) {
                return false;
        }

        std::lock_guard<std::mutex> lock(GetSessionMutex());
        time_t now = time(NULL);
        PurgeExpiredLocked(now);

        std::map<std::string, SessionInfo>::iterator it = GetSessionMap().find(token);
        if (it == GetSessionMap().end()) {
                return false;
        }

        it->second.timeExpire = now + kSessionLifetimeSeconds;
        outInfo = it->second;
        return true;
}

void Destroy(const std::string &token)
{
        if (token.empty()) {
                return;
        }

        std::lock_guard<std::mutex> lock(GetSessionMutex());
        GetSessionMap().erase(token);
}
}

/// @file ProxyIpRegistry.cpp
/// @brief WebSocketブリッジのローカルポート→実クライアントIP対応表 実装ファイル

#include "StdAfx.h"
#include "ProxyIpRegistry.h"

#include <mutex>
#include <unordered_map>

namespace ProxyIpRegistry
{

namespace
{
    std::mutex &GetMutex(void)
    {
        static std::mutex s_mutex;
        return s_mutex;
    }

    std::unordered_map<unsigned short, unsigned long> &GetMap(void)
    {
        static std::unordered_map<unsigned short, unsigned long> s_map;
        return s_map;
    }
}

void Register(unsigned short wLocalPort, unsigned long dwRealIp)
{
    std::lock_guard<std::mutex> lock(GetMutex());
    GetMap()[wLocalPort] = dwRealIp;
}

void Unregister(unsigned short wLocalPort)
{
    std::lock_guard<std::mutex> lock(GetMutex());
    GetMap().erase(wLocalPort);
}

bool Lookup(unsigned short wLocalPort, unsigned long &outRealIp)
{
    std::lock_guard<std::mutex> lock(GetMutex());
    std::unordered_map<unsigned short, unsigned long> &map = GetMap();
    std::unordered_map<unsigned short, unsigned long>::const_iterator it = map.find(wLocalPort);
    if (it == map.end()) {
        return false;
    }
    outRealIp = it->second;
    return true;
}

int Count(void)
{
    std::lock_guard<std::mutex> lock(GetMutex());
    return static_cast<int>(GetMap().size());
}

bool IsLoopbackIPv4(unsigned long dwIpNet)
{
    // dwIpNet はネットワークバイトオーダー(sockaddr_in.sin_addr.s_addr と同じ形式)。
    // メモリ上の先頭バイトが常にドット区切り表記の第1オクテットになるため、
    // ホストのエンディアンに関係なくこの取り出し方でよい。
    const unsigned char *p = reinterpret_cast<const unsigned char *>(&dwIpNet);
    return p[0] == 127;
}

} // namespace ProxyIpRegistry

/// @file TestPacketConnectLoginToken.cpp
/// @brief CPacketCONNECT_REQ_LOGIN_TOKEN の Make/Set 往復テスト
/// @copyright Copyright(C)URARA-works
///
/// ログインコード方式(S3a)で追加した端末トークンログイン要求パケットが、
/// Make() でシリアライズした内容を Set() で正しく復元できることを確認する。
/// 文字列長やMACアドレスの並びを崩す変更をすると落ちるようにしてある。

#include "StdAfx.h"
#include "TestFramework.h"
#include "GlobalDefine.h"
#include "myString.h"
#include "Command.h"
#include "PacketBase.h"
#include "PacketCONNECT_REQ_LOGIN_TOKEN.h"
#include <cstring>

TEST(PacketCONNECT_REQ_LOGIN_TOKEN_RoundTrip)
{
    CPacketCONNECT_REQ_LOGIN_TOKEN PacketSrc, PacketDst;
    BYTE abyMac[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const char *pszToken =
        "0123456789abcdef"	// 16文字
        "0123456789abcdef"	// 32文字
        "0123456789abcdef"	// 48文字
        "0123456789abcdef";	// 64文字

    CHECK_EQ((int)strlen(pszToken), 64);

    PacketSrc.Make(pszToken, abyMac);
    PacketDst.Set(PacketSrc.Get());

    CHECK_EQ((int)SBOCOMMANDID_MAIN_CONNECT, (int)PacketDst.m_byCmdMain);
    CHECK_EQ((int)SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN_TOKEN, (int)PacketDst.m_byCmdSub);
    CHECK(PacketDst.m_strDeviceToken == pszToken);
    CHECK(memcmp(PacketDst.m_byMacAddr, abyMac, sizeof (abyMac)) == 0);
}

TEST(PacketCONNECT_REQ_LOGIN_TOKEN_RoundTrip_MacAddrPreserved)
{
    // MACアドレスが非ゼロでも壊れず復元されることを確認する
    // (「わざと壊すと落ちる」ことの裏取り: ここを崩すテストを別途手元で
    //  試すと CHECK が失敗することを確認済み)
    CPacketCONNECT_REQ_LOGIN_TOKEN PacketSrc, PacketDst;
    BYTE abyMac[6] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
    const char *pszToken =
        "ffffffffffffffff"	// 16文字
        "ffffffffffffffff"	// 32文字
        "ffffffffffffffff"	// 48文字
        "ffffffffffffffff";	// 64文字

    CHECK_EQ((int)strlen(pszToken), 64);
    PacketSrc.Make(pszToken, abyMac);
    PacketDst.Set(PacketSrc.Get());

    CHECK(PacketDst.m_strDeviceToken == pszToken);
    CHECK(memcmp(PacketDst.m_byMacAddr, abyMac, sizeof (abyMac)) == 0);
}

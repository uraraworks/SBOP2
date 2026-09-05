/// @file TestUraraSockTCPSelect.cpp
/// @brief select ベース通信実装のテスト
/// @copyright Copyright(C)URARA-works
///
/// 既存の WSAAsyncSelect 実装はウィンドウが無いと動かせないため、
/// この種のテストは書けなかった。select 実装はウィンドウに依存しないので
/// テストプロセスから直接起動して検証できる。

#include "StdAfx.h"
#include "TestFramework.h"
#include "UraraSockTCPSelect.h"
#include "SboSockTestClient.h"
#include "GlobalDefine.h"
#include <vector>

#define TESTPORT_BASE   (24100)

// 通知を受け取って記録する入れ物
//
// 通知はソケットスレッドから来るため、記録はクリティカルセクションで守る。

class CNotifyRecorder
{
public:
    struct EVENT {
        UINT   uMsgOffset;
        DWORD  dwSessionID;
        std::vector<BYTE> vecData;
    };

    CNotifyRecorder(void)  { InitializeCriticalSection(&m_Crit); }
    ~CNotifyRecorder(void) { DeleteCriticalSection(&m_Crit); }

    static void Thunk(void *pUserData, UINT uMsgOffset, WPARAM wParam, LPARAM lParam)
    {
        CNotifyRecorder *pThis = reinterpret_cast<CNotifyRecorder *>(pUserData);
        EVENT Event;

        Event.uMsgOffset  = uMsgOffset;
        Event.dwSessionID = static_cast<DWORD>(lParam);

        if (uMsgOffset == WM_URARASOCK_RECV) {
            PBYTE pData = reinterpret_cast<PBYTE>(wParam);
            if (pData) {
                // 長さは分からないので、テストでは先頭16バイトだけ見る
                Event.vecData.assign(pData, pData + 16);
                delete[] pData;
            }
        }

        EnterCriticalSection(&pThis->m_Crit);
        pThis->m_vecEvent.push_back(Event);
        LeaveCriticalSection(&pThis->m_Crit);
    }

    /// 指定の通知が来るまで待つ
    BOOL Wait(UINT uMsgOffset, int nTimeoutMs, EVENT *pOut = NULL)
    {
        DWORD dwEnd = GetTickCount() + nTimeoutMs;

        while (GetTickCount() < dwEnd) {
            EnterCriticalSection(&m_Crit);
            for (size_t i = 0; i < m_vecEvent.size(); i ++) {
                if (m_vecEvent[i].uMsgOffset == uMsgOffset) {
                    if (pOut) {
                        *pOut = m_vecEvent[i];
                    }
                    LeaveCriticalSection(&m_Crit);
                    return TRUE;
                }
            }
            LeaveCriticalSection(&m_Crit);
            Sleep(10);
        }
        return FALSE;
    }

    int Count(UINT uMsgOffset)
    {
        int nCount = 0;

        EnterCriticalSection(&m_Crit);
        for (size_t i = 0; i < m_vecEvent.size(); i ++) {
            if (m_vecEvent[i].uMsgOffset == uMsgOffset) {
                nCount ++;
            }
        }
        LeaveCriticalSection(&m_Crit);
        return nCount;
    }

    void Clear(void)
    {
        EnterCriticalSection(&m_Crit);
        m_vecEvent.clear();
        LeaveCriticalSection(&m_Crit);
    }

private:
    CRITICAL_SECTION   m_Crit;
    std::vector<EVENT> m_vecEvent;
};

// テストごとにポートをずらす(前のテストの TIME_WAIT を避ける)
static WORD NextPort(void)
{
    static WORD s_wPort = TESTPORT_BASE;
    return s_wPort ++;
}

//////////////////////////////////////////////////////////////////////

TEST(Host_待ち受けを開始できる)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    WORD wPort = NextPort();

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_HOST, 1000) != FALSE);

    pSock->Destroy();
    delete pSock;
}

TEST(Host_通知先が無ければ失敗する)
{
    // ウィンドウを使わない実装なので、通知先が無いと何も届けられない
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    WORD wPort = NextPort();

    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) == FALSE);
    delete pSock;
}

TEST(接続_プリチェックに成功するとADDCLIENTが来る)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    WORD wPort = NextPort();

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);

    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000) != FALSE);

    Client.Close();
    pSock->Destroy();
    delete pSock;
}

TEST(接続_プリチェックに失敗すると切断されADDCLIENTは来ない)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    WORD wPort = NextPort();

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);

    // わざと誤った応答を返す
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK, TRUE) != FALSE);

    CHECK(Client.WaitForDisconnect(3000) != FALSE);
    CHECK_EQ(0, Rec.Count(WM_URARASOCK_ADDCLIENT));

    pSock->Destroy();
    delete pSock;
}

TEST(受信_送ったパケットがRECVとして届く)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    CNotifyRecorder::EVENT Event;
    WORD wPort = NextPort();
    BYTE byData[16];

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000) != FALSE);

    for (int i = 0; i < 16; i ++) {
        byData[i] = static_cast<BYTE>(i + 1);
    }
    CHECK(Client.SendPacket(byData, sizeof(byData)) != FALSE);

    CHECK(Rec.Wait(WM_URARASOCK_RECV, 3000, &Event) != FALSE);
    CHECK_EQ(16, (int)Event.vecData.size());
    CHECK_EQ(1,  (int)Event.vecData[0]);
    CHECK_EQ(16, (int)Event.vecData[15]);

    Client.Close();
    pSock->Destroy();
    delete pSock;
}

TEST(受信_zlib圧縮されたパケットが展開されて届く)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    CNotifyRecorder::EVENT Event;
    WORD wPort = NextPort();
    std::vector<BYTE> vecData;

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000) != FALSE);

    // 128バイト以上なのでクライアント側が圧縮して送る
    vecData.resize(400);
    for (size_t i = 0; i < vecData.size(); i ++) {
        vecData[i] = static_cast<BYTE>((i * 7) % 251);
    }
    CHECK(Client.SendPacket(&vecData[0], static_cast<DWORD>(vecData.size())) != FALSE);

    CHECK(Rec.Wait(WM_URARASOCK_RECV, 3000, &Event) != FALSE);
    CHECK_EQ((int)vecData[0],  (int)Event.vecData[0]);
    CHECK_EQ((int)vecData[15], (int)Event.vecData[15]);

    Client.Close();
    pSock->Destroy();
    delete pSock;
}

TEST(送信_SendToしたデータがクライアントへ届く)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    CNotifyRecorder::EVENT Event;
    WORD wPort = NextPort();
    BYTE byData[32];
    std::vector<BYTE> vecRecv;

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000, &Event) != FALSE);

    for (int i = 0; i < 32; i ++) {
        byData[i] = static_cast<BYTE>(0xA0 + i);
    }
    pSock->SendTo(Event.dwSessionID, byData, sizeof(byData));

    CHECK(Client.RecvPacket(&vecRecv, 3000) != FALSE);
    CHECK_EQ(32, (int)vecRecv.size());
    CHECK_EQ(0xA0, (int)vecRecv[0]);
    CHECK_EQ(0xBF, (int)vecRecv[31]);

    Client.Close();
    pSock->Destroy();
    delete pSock;
}

TEST(送信_128バイト以上は圧縮されて届く)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    CNotifyRecorder::EVENT Event;
    WORD wPort = NextPort();
    std::vector<BYTE> vecSend;
    std::vector<BYTE> vecRecv;

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000, &Event) != FALSE);

    vecSend.resize(500);
    for (size_t i = 0; i < vecSend.size(); i ++) {
        vecSend[i] = static_cast<BYTE>((i * 3) % 199);
    }
    pSock->SendTo(Event.dwSessionID, &vecSend[0], static_cast<DWORD>(vecSend.size()));

    // クライアント側が展開して元に戻せることを確認する
    CHECK(Client.RecvPacket(&vecRecv, 3000) != FALSE);
    CHECK_EQ((int)vecSend.size(), (int)vecRecv.size());
    CHECK_EQ((int)vecSend[0],   (int)vecRecv[0]);
    CHECK_EQ((int)vecSend[250], (int)vecRecv[250]);
    CHECK_EQ((int)vecSend[499], (int)vecRecv[499]);

    Client.Close();
    pSock->Destroy();
    delete pSock;
}

TEST(切断_クライアントが切るとDECCLIENTが来る)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    WORD wPort = NextPort();

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000) != FALSE);

    Client.Close();

    CHECK(Rec.Wait(WM_URARASOCK_DECCLIENT, 3000) != FALSE);

    pSock->Destroy();
    delete pSock;
}

TEST(切断_FINだけ送られた場合もDECCLIENTが来る)
{
    // recv() が 0 を返す正常クローズを切断として扱えているかの検証。
    //
    // Close() は SD_BOTH のため相手に RST が届くことがあり、それだと
    // recv() はエラーを返すので別経路で検知されてしまう。ここでは
    // 送信方向だけ閉じて FIN を送り、recv()==0 の経路を確実に通す。
    //
    // ここを取りこぼすと select が「読める」と言い続けて空転する。
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    WORD wPort = NextPort();

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000) != FALSE);

    Client.ShutdownSend();

    CHECK(Rec.Wait(WM_URARASOCK_DECCLIENT, 3000) != FALSE);

    Client.Close();
    pSock->Destroy();
    delete pSock;
}

TEST(切断_DeleteClientでサーバー側から切れる)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client;
    CNotifyRecorder::EVENT Event;
    WORD wPort = NextPort();

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);
    CHECK(Client.Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000, &Event) != FALSE);

    pSock->DeleteClient(Event.dwSessionID);

    CHECK(Client.WaitForDisconnect(3000) != FALSE);
    CHECK(Rec.Wait(WM_URARASOCK_DECCLIENT, 3000) != FALSE);

    pSock->Destroy();
    delete pSock;
}

TEST(複数接続_それぞれに個別へ送れる)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client[3];
    WORD wPort = NextPort();
    DWORD adwSession[3];
    int i;

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);

    // 1本ずつ繋いでセッションIDを確定させる
    for (i = 0; i < 3; i ++) {
        CNotifyRecorder::EVENT Event;

        Rec.Clear();
        CHECK(Client[i].Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
        CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000, &Event) != FALSE);
        adwSession[i] = Event.dwSessionID;
    }

    // それぞれに別の値を送る
    for (i = 0; i < 3; i ++) {
        BYTE byData[4];

        byData[0] = static_cast<BYTE>(0x10 + i);
        byData[1] = byData[2] = byData[3] = 0;
        pSock->SendTo(adwSession[i], byData, sizeof(byData));
    }

    for (i = 0; i < 3; i ++) {
        std::vector<BYTE> vecRecv;

        CHECK(Client[i].RecvPacket(&vecRecv, 3000) != FALSE);
        CHECK_EQ(4, (int)vecRecv.size());
        CHECK_EQ(0x10 + i, (int)vecRecv[0]);
    }

    for (i = 0; i < 3; i ++) {
        Client[i].Close();
    }
    pSock->Destroy();
    delete pSock;
}

TEST(複数接続_全員宛の送信が全員に届く)
{
    CUraraSockTCP *pSock = GetUraraSockTCPSelect();
    CNotifyRecorder Rec;
    CSboSockTestClient Client[3];
    WORD wPort = NextPort();
    BYTE byData[4];
    int i;

    pSock->SetNotifySink(&CNotifyRecorder::Thunk, &Rec);
    CHECK(pSock->Host(NULL, 0, URARASOCK_PRECHECK, wPort, 10) != FALSE);

    for (i = 0; i < 3; i ++) {
        Rec.Clear();
        CHECK(Client[i].Connect("127.0.0.1", wPort, URARASOCK_PRECHECK) != FALSE);
        CHECK(Rec.Wait(WM_URARASOCK_ADDCLIENT, 3000) != FALSE);
    }

    byData[0] = 0x5A;
    byData[1] = byData[2] = byData[3] = 0;
    pSock->Send(byData, sizeof(byData));

    for (i = 0; i < 3; i ++) {
        std::vector<BYTE> vecRecv;

        CHECK(Client[i].RecvPacket(&vecRecv, 3000) != FALSE);
        CHECK_EQ(4, (int)vecRecv.size());
        CHECK_EQ(0x5A, (int)vecRecv[0]);
    }

    for (i = 0; i < 3; i ++) {
        Client[i].Close();
    }
    pSock->Destroy();
    delete pSock;
}

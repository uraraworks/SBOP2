#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <future>

#include "ApiRouter.h"

class CMgrData;
class CMainFrame;

class CHttpServer
{
public:
        CHttpServer();
        ~CHttpServer();

        bool    Start(unsigned short wPort);
        void    Stop();
        void    SetMgrData(CMgrData *pMgrData);
        void    SetMainFrame(CMainFrame *pMainFrame);

private:
        /// @brief メインスレッド本体
        /// @param startedPromise 起動完了(成否)を Start() 側へ伝える promise。スレッド内で必ず1回 set_value() する。
        void    Run(std::promise<bool> startedPromise);
        bool    CreateListener();
        void    CloseListener();
        void    ProcessLoop();
        void    HandleAccept();
        void    HandleClient(SOCKET hClient, bool &outTransferred);
        bool    ParseHttpRequest(const std::string &rawRequest, HttpRequest &outRequest);
        static std::string Trim(const std::string &text);
        void    SendResponse(SOCKET hClient, const HttpResponse &response);
        bool    SendAll(SOCKET hSocket, const char *pData, size_t nLength);
        void    RegisterDefaultHandlers();
        bool    ResolveWebRootPath(std::wstring &outPath) const;
        bool    ResolveBrowserGamePath(std::wstring &outPath) const;

        /// @brief /ws/admin WebSocket アップグレード要求を処理する
        /// ハンドシェイク成功後はソケットを CAdminWsHub に渡し、HttpServer は所有権を手放す。
        /// @return ソケットを Hub に移譲した場合 true（HandleAccept は closesocket しない）
        bool    HandleAdminWsUpgrade(SOCKET hClient, const std::string &rawHeaders);

        /// @brief 終了済みクライアントスレッドを m_clientThreads から除去する (mutex 保護済みの呼び出し元から使う)
        void    PruneClientThreadsLocked();

        static const int kMaxClientThreads = 32;

        /// @brief クライアント接続を処理するスレッド1本分の情報
        /// std::thread / std::future はコピー不可のため、この構造体自体もムーブ専用になる
        /// （m_clientThreads への push_back / 除去はいずれもムーブで行う）。
        /// std::thread には非ブロッキングで「終了したか」を問う手段が無いため、
        /// スレッド末尾で set_value() される doneFuture を wait_for(0秒) で代用する
        /// （PruneClientThreadsLocked が従来の WaitForSingleObject(h, 0) の代わりに使う）。
        struct ClientThreadEntry
        {
                std::thread       thread;      ///< クライアント処理スレッド
                std::future<void> doneFuture;  ///< スレッド終了通知
        };

        SOCKET          m_hListen;
        std::thread     m_thread;
        std::future<void> m_doneFuture;  ///< メインスレッド終了通知（Stop のタイムアウト付き待機に使用）
        std::atomic<bool> m_bStop;
        unsigned short  m_wPort;
        CApiRouter      m_router;
        bool            m_bHandlersRegistered;
        CMgrData       *m_pMgrData;
        CMainFrame     *m_pMainFrame;

        std::vector<ClientThreadEntry> m_clientThreads;
        std::mutex                     m_clientThreadsMutex;
};

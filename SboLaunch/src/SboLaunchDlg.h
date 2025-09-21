/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SboLaunchDlg.h												 */
/* 内容			:メインダイアログ 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/27													 */
/* ========================================================================= */

#pragma once

class CUraraSockTCPSBO;
class CInfoFileList;

/* ========================================================================= */
/* 構造体宣言																 */
/* ========================================================================= */

/* ファイルリストチェック構造体 */
typedef struct _CHECKFILELIST {
	int			nNo;			/* チェック中のファイルリスト番号 */
	ARRAYINT	anGetFlieNo;	/* 取得するファイルリスト番号 */
} CHECKFILELIST, *PCHECKFILELIST;

/* ファイル取得情報構造体 */
typedef struct _GETFILEINFO {
	int			nNo;			/* 取得中のファイルリスト番号 */
	DWORD		dwRecvSize;		/* 取得済みサイズ */
	PBYTE		pRecvDataTmp;	/* 取得中データ */
} GETFILEINFO, *PGETFILEINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CSboLaunchDlg : public CDialog
{
public:
			CSboLaunchDlg(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CSboLaunchDlg();							/* デストラクタ */


protected:
	void	AddMsg					(LPCSTR pszMsg);	/* メッセージ追加 */
	void	ReadIniData				(void);				/* iniファイルから設定を読み込み */
	void	ChgState				(int nState);		/* 状態遷移 */
	void	ChgStateNONE			(void);				/* 状態遷移(停止中) */
	void	ChgStateCONNECT			(void);				/* 状態遷移(サーバーへ接続) */
	void	ChgStateCONNECTMISS		(void);				/* 状態遷移(サーバーへ接続失敗) */
	void	ChgStateDISCONNECT		(void);				/* 状態遷移(サーバーと切断) */
	void	ChgStateGETFILELIST		(void);				/* 状態遷移(ファイルリスト取得) */
	void	ChgStateCHECKFILELIST	(void);				/* 状態遷移(ファイルリストチェック) */
	void	ChgStateGETFILE			(void);				/* 状態遷移(ファイル取得) */
	void	ChgStateMAKEFILELIST	(void);				/* 状態遷移(ファイルリスト出力) */
	void	ChgStateRUNUPDATE		(void);				/* 状態遷移(アップデータ起動) */
	void	ChgStateQUIT			(void);				/* 状態遷移(終了) */
	void	ProcCHECKFILELIST		(void);				/* 処理(ファイルリストチェック) */
	void	ProcGETFILE				(void);				/* 処理(ファイル取得) */
	void	OnRecv					(PBYTE pRecvData);	/* データ受信 */
	void	RecvProcVERSION					(BYTE byCmdSub, PBYTE pData);	/* 受信処理(バージョン系) */
	void	RecvProcVERSION_RES_FILELIST	(PBYTE pData);					/* 受信処理(ファイルリスト応答) */
	void	RecvProcVERSION_RES_FILE		(PBYTE pData);					/* 受信処理(ファイル応答) */
	void	DeleteAllFiles			(LPCSTR pszPath);						/* 指定パス以下のファイルを全て削除 */
	void	DeleteAllFilesTmp		(HANDLE hFind, LPCSTR pszPath1, LPCSTR pszPath2, CmyStringArray &astrFolderPath);	/* 指定パス以下のファイルを全て削除(処理) */


protected:
	BOOL				m_bConnect;				/* サーバーと接続した */
	int					m_nState;				/* 状態 */
	CUraraSockTCPSBO	*m_pSock;				/* 通信ライブラリ */
	CString				m_strServerAddr;		/* サーバーアドレス */
	WORD				m_wServerPort;			/* サーバー接続ポート */
	CInfoFileList		*m_pInfoFileList;		/* ファイルリスト情報 */
	CHECKFILELIST		*m_pstCheckFlieList;	/* ファイルリストチェック構造体 */
	GETFILEINFO			*m_pstGetFileInfo;		/* ファイル取得情報構造体 */



public:
	//{{AFX_DATA(CSboLaunchDlg)
	enum { IDD = IDD_SBOLAUNCH_DIALOG };
	CProgressCtrl	m_ctlProgressFile;
	CProgressCtrl	m_ctlProgressAll;
	CEdit	m_edLog;
	CString	m_strFile;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSboLaunchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CSboLaunchDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHistory();
	afx_msg void OnBnClickedHowto();
	afx_msg void OnBnClickedRetry();
};

//{{AFX_INSERT_LOCATION}}
/* Copyright(C)URARA-works 2008 */

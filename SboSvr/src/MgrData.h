/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrData.h													 */
/* 内容			:データマネージャクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#pragma once

class CMainFrame;
class CUraraSockTCPSBO;
class CTextOutput;
class CLibInfoAccount;
class CLibInfoCharSvr;
class CLibInfoDisable;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;
class CLibInfoTalkEvent;
class CInfoFileList;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrData
{
public:
			CMgrData();													/* コンストラクタ */
	virtual ~CMgrData();												/* デストラクタ */

	void	Create				(CMainFrame *pMainFrame, CUraraSockTCPSBO *pSock);	/* 作成 */
	void	Destroy				(void);												/* 破棄 */

	void	Save				(void);											/* データの保存 */
	void	Load				(void);											/* データの読み込み */
	void	ReadHashList		(void);											/* ファイルリストを読み込み */
	WORD	GetPort				(void)	{ return m_wPort;				}		/* 待ちうけポート番号を取得 */
	void	AddOnline			(void)	{ m_byOnline ++;				}		/* オンライン数を追加 */
	void	DecOnline			(void)	{ m_byOnline --;				}		/* オンライン数を減算 */
	BYTE	GetOnline			(void)	{ return m_byOnline;			}		/* オンライン数を取得 */
	LPCSTR	GetAdminAccount		(void)	{ return m_strAdminAccount;		}		/* 管理者権限アカウントを取得 */
	LPCSTR	GetClientVersion	(void)	{ return m_strClientVersion;	}		/* クライアントバージョンを取得 */
	void	SetClientVersion	(LPCSTR pszVersion);							/* クライアントバージョンを設定 */
	BYTE	GetLastSendClock	(void)	{ return m_byLastSendClock; 	}		/* 最後に送信した時刻を取得 */
	void	SetLastSendClock	(BYTE byLastSendClock)	{ m_byLastSendClock = byLastSendClock; };	/* 最後に送信した時刻を設定 */
	LPCSTR	GetFtpAccount		(void)	{ return m_strFtpAccount;		}		/* FTPアカウントを取得 */
	LPCSTR	GetFtpPassword		(void)	{ return m_strFtpPassword;		}		/* FTPパスワードを取得 */
	LPCSTR	GetFtpServerAddr	(void)	{ return m_strFtpServerAddr;	}		/* サーバーアドレスを取得 */
	LPCSTR	GetFtpUploadPath	(void)	{ return m_strFtpUploadPath;	}		/* アップロード先を取得 */

	CMainFrame			*GetMainFrame			(void)	{ return m_pMainFrame;		}	/* メインフレームを取得 */
	CUraraSockTCPSBO	*GetSock				(void)	{ return m_pSock;			}	/* 通信マネージャを取得 */

	CTextOutput			*GetLog					(void)	{ return m_pLog;				}	/* ログクラスを取得 */
	CLibInfoAccount		*GetLibInfoAccount		(void)	{ return m_pLibInfoAccount;		}	/* アカウント情報ライブラリを取得 */
	CLibInfoCharSvr		*GetLibInfoChar			(void)	{ return m_pLibInfoChar;		}	/* キャラ情報ライブラリを取得 */
	CLibInfoDisable		*GetLibInfoDisable		(void)	{ return m_pLibInfoDisable;		}	/* 拒否情報ライブラリを取得 */
	CLibInfoMapBase		*GetLibInfoMap			(void)	{ return m_pLibInfoMap;			}	/* マップ情報ライブラリを取得 */
	CLibInfoMapObject	*GetLibInfoMapObject	(void)	{ return m_pLibInfoMapObject;	}	/* マップオブジェクト情報ライブラリを取得 */
	CLibInfoMapParts	*GetLibInfoMapParts		(void)	{ return m_pLibInfoMapParts;	}	/* マップパーツ情報ライブラリを取得 */
	CLibInfoMapShadow	*GetLibInfoMapShadow	(void)	{ return m_pLibInfoMapShadow;	}	/* マップ影情報ライブラリを取得 */
	CLibInfoItemType	*GetLibInfoItemType		(void)	{ return m_pLibInfoItemType;	}	/* アイテム種別情報ライブラリを取得 */
	CLibInfoItem		*GetLibInfoItem			(void)	{ return m_pLibInfoItem;		}	/* アイテム情報ライブラリを取得 */
	CLibInfoItemWeapon	*GetLibInfoItemWeapon	(void)	{ return m_pLibInfoItemWeapon;	}	/* 武器情報ライブラリを取得 */
	CLibInfoEffect		*GetLibInfoEffect		(void)	{ return m_pLibInfoEffect;		}	/* エフェクト情報ライブラリを取得 */
	CLibInfoMotion		*GetLibInfoMotion		(void)	{ return m_pLibInfoMotion;		}	/* モーション情報ライブラリを取得 */
	CLibInfoMotionType	*GetLibInfoMotionType	(void)	{ return m_pLibInfoMotionType;	}	/* モーション種別情報ライブラリを取得 */
	CLibInfoEfcBalloon	*GetLibInfoEfcBalloon	(void)	{ return m_pLibInfoEfcBalloon;	}	/* 噴出し情報ライブラリを取得 */
	CLibInfoSystem		*GetLibInfoSystem		(void)	{ return m_pLibInfoSystem;		}	/* システム情報ライブラリを取得 */
	CLibInfoSkill		*GetLibInfoSkill		(void)	{ return m_pLibInfoSkill;		}	/* スキル情報ライブラリを取得 */
	CLibInfoTalkEvent	*GetLibInfoTalkEvent	(void)	{ return m_pLibInfoTalkEvent;	}	/* 会話イベント情報ライブラリを取得 */
	CInfoFileList		*GetInfoFileList		(void)	{ return m_pInfoFileList;		}	/* ファイルリスト情報を取得 */


private:
	void	ReadIniData		(void);				/* iniファイルから設定を読み込み */


private:
	WORD				m_wPort;				/* 待ちうけポート */
	BYTE				m_byOnline,				/* オンライン数 */
						m_byLastSendClock;		/* 最後に送信した時刻 */
	CmyString			m_strAdminAccount,		/* 管理者権限アカウント名 */
						m_strClientVersion,		/* クライアントバージョン */
						m_strFtpAccount,		/* FTPアカウント */
						m_strFtpPassword,		/* FTPパスワード */
						m_strFtpServerAddr,		/* サーバーアドレス */
						m_strFtpUploadPath;		/* アップロード先 */

	CMainFrame			*m_pMainFrame;			/* メインフレーム */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */
	CTextOutput			*m_pLog;				/* ログファイル */

	CLibInfoAccount		*m_pLibInfoAccount;		/* アカウント情報ライブラリ */
	CLibInfoCharSvr		*m_pLibInfoChar;		/* キャラ情報ライブラリ */
	CLibInfoDisable		*m_pLibInfoDisable;		/* 拒否情報ライブラリ */
	CLibInfoMapBase		*m_pLibInfoMap;			/* マップ情報ライブラリ */
	CLibInfoMapObject	*m_pLibInfoMapObject;	/* マップオブジェクト情報ライブラリ */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* マップパーツ情報ライブラリ */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* マップ影情報ライブラリ */
	CLibInfoItemType	*m_pLibInfoItemType;	/* アイテム種別情報ライブラリ */
	CLibInfoItem		*m_pLibInfoItem;		/* アイテム情報ライブラリ */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* 武器情報ライブラリ */
	CLibInfoEffect		*m_pLibInfoEffect;		/* エフェクト情報ライブラリ */
	CLibInfoMotion		*m_pLibInfoMotion;		/* モーション情報ライブラリ */
	CLibInfoMotionType	*m_pLibInfoMotionType;	/* モーション種別情報ライブラリ */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;	/* 噴出し情報ライブラリ */
	CLibInfoSystem		*m_pLibInfoSystem;		/* システム情報ライブラリ */
	CLibInfoSkill		*m_pLibInfoSkill;		/* スキル情報ライブラリ */
	CLibInfoTalkEvent	*m_pLibInfoTalkEvent;	/* 会話イベント情報ライブラリ */
	CInfoFileList		*m_pInfoFileList;		/* ファイルリスト情報 */
} CMgrData, *PCMgrData;

/* Copyright(C)URARA-works 2006 */

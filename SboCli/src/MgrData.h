/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrData.h													 */
/* 内容			:データマネージャクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#include "DataSlot.h"

class CMainFrame;
class CUraraSockTCPSBO;
class CMgrSound;
class CMgrGrpData;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;
class CInfoCharCli;
class CInfoMapBase;
class CInfoAccount;
class CInfoTalkEvent;
class CLibInfoCharCli;
class CLibInfoDisable;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItem;
class CLibInfoItemType;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrData : public CDataSlot
{
public:
			CMgrData();											/* コンストラクタ */
	virtual ~CMgrData();										/* デストラクタ */

	void			Create				(CMainFrame *m_pMainFrame, CMgrGrpData *pMgrGrpData);	/* 作成 */
	void			Destroy				(void);													/* 破棄 */
	void			SaveIniData			(void);													/* 設定情報を保存 */

	void			SetWindowInfo		(HINSTANCE hInstance, HWND hWndMain);		/* ウィンドウ情報を設定 */
	HINSTANCE		GetInstance			(void)	{ return m_hInstance;		}		/* インスタンスハンドルを取得 */
	HWND			GetMainWindow		(void)	{ return m_hWndMain;		}		/* メインウィンドウハンドルを取得 */

	void				SetUraraSockTCP		(CUraraSockTCPSBO *pSock)	{ m_pSock = pSock;	}	/* 通信マネージャを設定 */
	CUraraSockTCPSBO	*GetUraraSockTCP	(void)						{ return m_pSock;	}	/* 通信マネージャを取得 */

	void			SetAdminWindow		(HWND hWndAdmin)	{ m_hWndAdmin = hWndAdmin;		}	/* 管理者ウィンドウを設定 */
	HWND			GetAdminWindow		(void)				{ return m_hWndAdmin;			}	/* 管理者ウィンドウを取得 */
	void			SetDebugWindow		(HWND hWndDebug)	{ m_hWndDebug = hWndDebug;		}	/* デバッグウィンドウを設定 */
	HWND			GetDebugWindow		(void)				{ return m_hWndDebug;			}	/* デバッグウィンドウを取得 */
	void			SetAdminLevel		(int nAdminLevel)	{ m_nAdminLevel = nAdminLevel;	}	/* 管理者権限レベルを設定 */
	int				GetAdminLevel		(void)				{ return m_nAdminLevel;			}	/* 管理者権限レベルを取得 */
	void			SetDrawMode			(int nDrawMode)		{ m_nDrawMode = nDrawMode;		}	/* 描画モードを設定 */
	int				GetDrawMode			(void)				{ return m_nDrawMode;			}	/* 描画モードを取得 */
	void			SetSetItemID		(DWORD dwItemID)	{ m_dwSetItemID = dwItemID;		}	/* 配置するアイテムIDを設定 */
	DWORD			GetSetItemID		(void)				{ return m_dwSetItemID;			}	/* 配置するアイテムIDを取得 */
	void			SetChatModeBack		(int nChatMode)		{ m_nChatModeBack = nChatMode;	}	/* 前回のチャット種別を設定 */
	int				GetChatModeBack		(void)				{ return m_nChatModeBack;		}	/* 前回のチャット種別を取得 */
	void			SetDrawTime			(DWORD dwDrawTime)	{ m_dwDrawTime = dwDrawTime;	}	/* 描画時間を設定 */
	DWORD			GetDrawTime			(void)				{ return m_dwDrawTime;			}	/* 描画時間を取得 */

	void			SetAdminNotifyTypeL		(int nType)		{ m_nAdminNotifyTypeL = nType;	}	/* 管理者ウィンドウへの通知種別(左クリック)を設定 */
	int				GetAdminNotifyTypeL		(void)			{ return m_nAdminNotifyTypeL;	}	/* 管理者ウィンドウへの通知種別(左クリック)を取得 */
	void			SetAdminNotifyTypeR		(int nType)		{ m_nAdminNotifyTypeR = nType;	}	/* 管理者ウィンドウへの通知種別(右クリック)を設定 */
	int				GetAdminNotifyTypeR		(void)			{ return m_nAdminNotifyTypeR;	}	/* 管理者ウィンドウへの通知種別(右クリック)を取得 */
	void			SetAdminNotifyTypeRR	(int nType)		{ m_nAdminNotifyTypeRR = nType;	}	/* 管理者ウィンドウへの通知種別(右ダブルクリック)を設定 */
	int				GetAdminNotifyTypeRR	(void)			{ return m_nAdminNotifyTypeRR;	}	/* 管理者ウィンドウへの通知種別(右ダブルクリック)を取得 */

	void			SetLastAccount		(LPCSTR pszAccount)		{ m_strLastAccount = pszAccount;	}	/* 最終アカウント名を設定 */
	LPCSTR			GetLastAccount		(void)					{ return m_strLastAccount;			}	/* 最終アカウント名を取得 */
	void			SetLastPassword		(LPCSTR pszPassword)	{ m_strLastPassword = pszPassword;	}	/* 最終パスワードを設定 */
	LPCSTR			GetLastPassword		(void)					{ return m_strLastPassword;			}	/* 最終パスワードを取得 */

	void			SetEditMapPile			(BOOL bEditMapPile)		{ m_bEditMapPile = bEditMapPile;			}	/* 重ね合わせ用としてマップ編集を設定 */
	BOOL			GetEditMapPile			(void)					{ return m_bEditMapPile;					}	/* 重ね合わせ用としてマップ編集を取得 */
	void			SetMoveNoBlock			(BOOL bMoveNoBlock)		{ m_bMoveNoBlock = bMoveNoBlock;			}	/* 当たり判定無効を設定 */
	BOOL			GetMoveNoBlock			(void)					{ return m_bMoveNoBlock;					}	/* 当たり判定無効を取得 */
	void			SetSavePassword			(BOOL bSavePassword)	{ m_bSavePassword = bSavePassword;			}	/* パスワードを保存するを設定 */
	BOOL			GetSavePassword			(void)					{ return m_bSavePassword;					}	/* パスワードを保存するを取得 */
	void			SetDisableLogin			(BOOL bDisableLogin)	{ m_bDisableLogin = bDisableLogin;			}	/* ログイン拒否を設定 */
	BOOL			GetDisableLogin			(void)					{ return m_bDisableLogin;					}	/* ログイン拒否を取得 */
	void			SetSelectMapPartsID		(DWORD dwMapPartsID)	{ m_dwSelectMapPartsID = dwMapPartsID;		}	/* 選択中のマップパーツIDを設定 */
	DWORD			GetSelectMapPartsID		(void)					{ return m_dwSelectMapPartsID;				}	/* 選択中のマップパーツIDを取得 */
	void			SetSelectMapShadowID	(DWORD dwMapShadowID)	{ m_dwSelectMapShadowID = dwMapShadowID;	}	/* 選択中のマップ影IDを設定 */
	DWORD			GetSelectMapShadowID	(void)					{ return m_dwSelectMapShadowID;				}	/* 選択中のマップ影IDを取得 */
	void			SetViewGrid				(BYTE byViewGrid)		{ m_byViewGrid = byViewGrid;				}	/* グリッド表示を設定 */
	BYTE			GetViewGrid				(void)					{ return m_byViewGrid;						}	/* グリッド表示を取得 */
	void			SetOptionTaskbar		(BOOL bOn)				{ m_bOptionTaskbar = bOn;					}	/* 発言時にタスクバーチカチカを設定 */
	BOOL			GetOptionTaskbar		(void)					{ return m_bOptionTaskbar;					}	/* 発言時にタスクバーチカチカを取得 */
	void			SetOptionViewChat		(BOOL bOn)				{ m_bOptionViewChat = bOn;					}	/* 発言を表示するを設定 */
	BOOL			GetOptionViewChat		(void)					{ return m_bOptionViewChat;					}	/* 発言を表示するを取得 */
	void			SetOptionViewItem		(BOOL bOn)				{ m_bOptionViewItem = bOn;					}	/* アイテムを表示するを設定 */
	BOOL			GetOptionViewItem		(void)					{ return m_bOptionViewItem;					}	/* アイテムを表示するを取得 */
	void			SetOptionViewItemName	(BOOL bOn)				{ m_bOptionViewItemName = bOn;				}	/* アイテム名を表示するを設定 */
	BOOL			GetOptionViewItemName	(void)					{ return m_bOptionViewItemName;				}	/* アイテム名を表示するを取得 */
	void			SetOptionViewHelpIcon	(BOOL bOn)				{ m_bOptionViewHelpIcon = bOn;				}	/* ヘルプアイコンを表示しないを設定 */
	BOOL			GetOptionViewHelpIcon	(void)					{ return m_bOptionViewHelpIcon;				}	/* ヘルプアイコンを表示しないを取得 */
	void			SetOptionBattleMsgLog	(BOOL bOn)				{ m_bOptionBattleMsgLog = bOn;				}	/* 戦闘メッセージをログに残すを設定 */
	BOOL			GetOptionBattleMsgLog	(void)					{ return m_bOptionBattleMsgLog;				}	/* 戦闘メッセージをログに残すを取得 */
	void			SetOption60Frame		(BOOL bOn)				{ m_bOption60Frame = bOn;					}	/* 60フレームで表示するを設定 */
	BOOL			GetOption60Frame		(void)					{ return m_bOption60Frame;					}	/* 60フレームで表示するを取得 */

	int				GetWindowPosITEMMENUPos		(void)		{ return m_nWindowPosITEMMENUPos;		}	/* アイテムメニュー(位置)を取得 */
	void			SetWindowPosITEMMENUPos		(int nPos)	{ m_nWindowPosITEMMENUPos = nPos;		}	/* アイテムメニュー(位置)を設定 */
	int				GetWindowPosSKILLMENUPos	(void)		{ return m_nWindowPosSKILLMENUPos;		}	/* スキルメニュー(位置)を取得 */
	void			SetWindowPosSKILLMENUPos	(int nPos)	{ m_nWindowPosSKILLMENUPos = nPos;		}	/* スキルメニュー(位置)を設定 */
	int				GetWindowPosSKILLMENUType	(void)		{ return m_nWindowPosSKILLMENUType;		}	/* スキルメニュー(種別)を取得 */
	void			SetWindowPosSKILLMENUType	(int nType)	{ m_nWindowPosSKILLMENUType = nType;	}	/* スキルメニュー(種別)を設定 */

	LPCSTR			GetServerAddr		(void)	{ return m_strServerAddr;	}		/* サーバーアドレスを取得 */
	WORD			GetServerPort		(void)	{ return m_wServerPort;		}		/* サーバーポート番号を取得 */

	CInfoCharCli	*GetPlayerChar		(void)	{ return m_pPlayerChar;		}						/* 操作中のキャラ情報を取得 */
	void			SetPlayerChar		(CInfoCharCli *pInfoChar)	{ m_pPlayerChar = pInfoChar; }	/* 操作中のキャラ情報を設定 */
	CInfoMapBase	*GetMap				(void)	{ return m_pMap;			}		/* 表示中のマップ情報を取得 */
	CInfoAccount	*GetAccount			(void)	{ return m_pInfoAccount;	}		/* 使用中のアカウント情報を取得 */
	void			SetMap				(CInfoMapBase *pMap)	{ m_pMap = pMap; }	/* 表示中のマップ情報を設定 */
	void			SetAccount			(CInfoAccount *pInfoAccount);				/* 使用中のアカウント情報を設定 */
	DWORD			GetAccountID		(void);										/* 使用中のアカウントIDを取得 */
	void			SetCharID			(DWORD dwCharID);							/* 使用中のキャラIDを設定 */
	DWORD			GetCharID			(void);										/* 使用中のキャラIDを取得 */
	DWORD			GetMapID			(void);										/* 現在のマップIDを取得 */
	LPCSTR			GetFamilyTypeName	(WORD wFamilyTypeID);						/* 種族名を取得 */
	LPCSTR			GetHairTypeName		(WORD wHairTypeID);							/* 髪型名を取得 */
	LPCSTR			GetHairColorName	(WORD wHairColorID);						/* 髪色名を取得 */
	LPCSTR			GetEyeColorName		(WORD wEyeColorID);							/* 目色名を取得 */
	LPCSTR			GetMotionName		(DWORD dwMotionID);							/* モーション名を取得 */
	LPCSTR			GetMotionProcName	(DWORD dwProcID);							/* 行動名を取得 */
	LPCSTR			GetMapEventName		(int nMapEventType);						/* マップイベント種別名を取得 */

	int				GetSystemMsgCount	(void);											/* システムメッセージ数を取得 */
	PSTSYSTEMMSG	GetSystemMsg		(int nNo);										/* システムメッセージを追加 */
	void			AddSystemMsg		(BOOL bAddLog, LPCSTR pszMsg, COLORREF clMsg);	/* システムメッセージを取得 */
	void			DeleteAllSystemMsg	(void);											/* システムメッセージを全て削除 */

	void			SetOnlineCount		(int nCount)	{ m_nOnlineCount = nCount;	}	/* オンライン数を設定 */
	int				GetOnlineCount		(void)			{ return m_nOnlineCount;	}	/* オンライン数を取得 */
	void			SetCharCount		(int nCount)	{ m_nCharCount = nCount;	}	/* キャラ数を設定 */
	int				GetCharCount		(void)			{ return m_nCharCount;		}	/* キャラ数を取得 */
	void			SetPing				(int nPing)		{ m_nPing = nPing;			}	/* サーバーとのパケット往復時間を設定 */
	int				GetPing				(void)			{ return m_nPing;			}	/* サーバーとのパケット往復時間を取得 */
	int				GetSEVolume			(void)			{ return m_nSEVolume;		}	/* 効果音量を取得 */
	void			SetSEVolume			(int nSEVolume);								/* 効果音量を設定 */
	int				GetBGMVolume		(void)			{ return m_nBGMVolume;		}	/* BGM音量を取得 */
	void			SetBGMVolume		(int nBGMVolume);								/* BGM音量を設定 */
	GUID			GetInputGuid		(void)			{ return m_stInputGuid;		}	/* 入力パッドのGUIDを取得 */
	void			SetInputGuid		(GUID stGuid)	{ m_stInputGuid = stGuid;	}	/* 入力パッドのGUIDを設定 */
	int				GetSleepTimer		(void)			{ return m_nSleepTimer;		}	/* おひるねタイマーを取得 */
	void			SetSleepTimer		(int nSleepTimer) { m_nSleepTimer = nSleepTimer; }/* おひるねタイマーを設定 */
	SIZE			GetDlgMapPartsEdit	(void)			{ return m_sizeDlgMapPartsEdit;	}								/* マップパーツ編集ダイアログサイズを取得 */
	void			SetDlgMapPartsEdit	(SIZE sizeDlgMapPartsEdit)	{ m_sizeDlgMapPartsEdit = sizeDlgMapPartsEdit; }	/* マップパーツ編集ダイアログサイズを設定 */
	SIZE			GetWndMap			(void)						{ return m_sizeWndMap;	}							/* マップウィンドウサイズを取得 */
	void			SetWndMap			(SIZE sizeWndMap)			{ m_sizeWndMap = sizeWndMap; }						/* マップウィンドウサイズを設定 */
	int				GetLastSelectGrpNo	(void)						{ return m_nLastSelectGrpNo;	}					/* 最後に選んだ画像番号を取得 */
	void			SetLastSelectGrpNo	(int nLastSelectGrpNo)		{ m_nLastSelectGrpNo = nLastSelectGrpNo; }			/* 最後に選んだ画像番号を設定 */
	void			SetInfoTalkEvent	(CInfoTalkEvent *pSrc);						/* 会話イベント情報の設定 */
	CInfoTalkEvent	*GetInfoTalkEvent	(void);										/* 会話イベント情報の取得 */

	CMainFrame		*GetMainFrame		(void)	{ return m_pMainFrame;		}		/* メインフレームを取得 */
	CMgrSound		*GetMgrSound		(void)	{ return m_pMgrSound;		}		/* サウンド管理を取得 */
	CMgrGrpData 	*GetMgrGrpData		(void)	{ return m_pMgrGrpData;		}		/* グラフィックデータ管理を取得 */
	CMgrDraw		*GetMgrDraw			(void)	{ return m_pMgrDraw;		}		/* 描画管理を取得 */
	CMgrLayer		*GetMgrLayer		(void)	{ return m_pMgrLayer;		}		/* レイヤー管理を取得 */
	CMgrWindow		*GetMgrWindow		(void)	{ return m_pMgrWindow;		}		/* ウィンドウ管理を取得 */
	CMgrKeyInput	*GetMgrKeyInput		(void)	{ return m_pMgrKeyInput;	}		/* キー入力管理を取得 */

	CLibInfoCharCli		*GetLibInfoChar			(void)	{ return m_pLibInfoChar;		}	/* キャラ情報ライブラリを取得 */
	CLibInfoDisable		*GetLibInfoDisable		(void)	{ return m_pLibInfoDisable;		}	/* 拒否情報ライブラリを取得 */
	CLibInfoMapBase		*GetLibInfoMap			(void)	{ return m_pLibInfoMap;			}	/* マップ情報ライブラリを取得 */
	CLibInfoMapObject	*GetLibInfoMapObject	(void)	{ return m_pLibInfoMapObject;	}	/* マップオブジェクト情報ライブラリを取得 */
	CLibInfoMapParts	*GetLibInfoMapParts		(void)	{ return m_pLibInfoMapParts;	}	/* マップパーツ情報ライブラリを取得 */
	CLibInfoMapShadow	*GetLibInfoMapShadow	(void)	{ return m_pLibInfoMapShadow;	}	/* マップ影情報ライブラリを取得 */
	CLibInfoItem		*GetLibInfoItem			(void)	{ return m_pLibInfoItem;		}	/* アイテム情報ライブラリを取得 */
	CLibInfoItemType	*GetLibInfoItemType		(void)	{ return m_pLibInfoItemType;	}	/* アイテム種別情報ライブラリを取得 */
	CLibInfoItemWeapon	*GetLibInfoItemWeapon	(void)	{ return m_pLibInfoItemWeapon;	}	/* 武器情報ライブラリを取得 */
	CLibInfoEffect		*GetLibInfoEffect		(void)	{ return m_pLibInfoEffect;		}	/* エフェクト情報ライブラリを取得 */
	CLibInfoMotion		*GetLibInfoMotion		(void)	{ return m_pLibInfoMotion;		}	/* モーション情報ライブラリを取得 */
	CLibInfoMotionType	*GetLibInfoMotionType	(void)	{ return m_pLibInfoMotionType;	}	/* モーション種別情報ライブラリを取得 */
	CLibInfoEfcBalloon	*GetLibInfoEfcBalloon	(void)	{ return m_pLibInfoEfcBalloon;	}	/* 噴出し情報ライブラリを取得 */
	CLibInfoSystem		*GetLibInfoSystem		(void)	{ return m_pLibInfoSystem;		}	/* システム情報ライブラリを取得 */
	CLibInfoSkill		*GetLibInfoSkill		(void)	{ return m_pLibInfoSkill;		}	/* スキル情報ライブラリを取得 */

	CLibInfoCharCli		*GetLibInfoCharOnline	(void)	{ return m_pLibInfoCharOnline;	}	/* ログイン中キャラ情報ライブラリを取得 */


private:
	void	ReadIniData	(void);						/* iniファイルから設定を読み込み */


private:
	HINSTANCE			m_hInstance;				/* インスタンスハンドル */
	HWND				m_hWndMain,					/* メインウィンドウハンドル */
						m_hWndAdmin,				/* 管理者ウィンドウのウィンドウハンドル */
						m_hWndDebug;				/* デバッグウィンドウのウィンドウハンドル */
	int					m_nOnlineCount,				/* オンライン数 */
						m_nCharCount,				/* 処理キャラ数 */
						m_nPing,					/* サーバーとのパケット往復時間 */
						m_nAdminLevel,				/* 管理者権限レベル */
						m_nDrawMode,				/* 描画モード */
						m_nChatModeBack,			/* 前回のチャット種別 */
						m_nAdminNotifyTypeL,		/* 管理者ウィンドウへの通知種別(左クリック) */
						m_nAdminNotifyTypeR,		/* 管理者ウィンドウへの通知種別(右クリック) */
						m_nAdminNotifyTypeRR,		/* 管理者ウィンドウへの通知種別(右ダブルクリック) */
						m_nSEVolume,				/* 効果音量 */
						m_nBGMVolume,				/* BGM音量 */
						m_nSleepTimer;				/* おひるねタイマー */
	BOOL				m_bEditMapPile,				/* 重ね合わせ用としてマップ編集 */
						m_bMoveNoBlock,				/* 当たり判定無効 */
						m_bSavePassword,			/* パスワードを保存する？ */
						m_bDisableLogin,			/* ログイン拒否 */
						m_bOptionTaskbar,			/* 発言時にタスクバーをチカチカ */
						m_bOptionViewChat,			/* 発言を表示する */
						m_bOptionViewItem,			/* アイテムを表示する */
						m_bOptionViewItemName,		/* アイテム名を表示する */
						m_bOptionViewHelpIcon,		/* ヘルプアイコンを表示する */
						m_bOptionBattleMsgLog,		/* 戦闘メッセージをログに残す */
						m_bOption60Frame;			/* 60フレームで表示する */
	BYTE				m_byViewGrid;				/* グリッド表示 */
	GUID				m_stInputGuid;				/* 入力パッドのGUID */
	SIZE				m_sizeDlgMapPartsEdit,		/* マップパーツ編集ダイアログサイズ */
						m_sizeWndMap;				/* マップウィンドウサイズ */
	int					m_nLastSelectGrpNo;			/* 最後に選んだ画像番号 */

	int					m_nWindowPosITEMMENUPos,	/* アイテムメニュー(位置) */
						m_nWindowPosSKILLMENUPos,	/* スキルメニュー(位置) */
						m_nWindowPosSKILLMENUType;	/* スキルメニュー(種別) */


	CmyString			m_strServerAddr,			/* サーバーアドレス */
						m_strLastAccount,			/* 最後に使われたアカウント名 */
						m_strLastPassword;			/* 最後に使われたパスワード */
	WORD				m_wServerPort;				/* サーバーポート番号 */
	DWORD				m_dwSelectMapPartsID,		/* 選択中のマップパーツID(マップ編集用) */
						m_dwSelectMapShadowID,		/* 選択中のマップ影ID(マップ編集用) */
						m_dwSetItemID,				/* 配置するアイテムID */
						m_dwDrawTime;				/* 描画時間 */

	CInfoCharCli		*m_pPlayerChar;				/* 操作中のキャラ情報 */
	CInfoMapBase		*m_pMap;					/* 表示中のマップ情報 */
	CInfoAccount		*m_pInfoAccount;			/* 使用中のアカウント情報 */
	CInfoTalkEvent		*m_pInfoTalkEvent;			/* 会話イベント情報 */

	CMainFrame			*m_pMainFrame;				/* メインフレーム */
	CUraraSockTCPSBO	*m_pSock;					/* 通信マネージャ */
	CMgrSound			*m_pMgrSound;				/* サウンド管理 */
	CMgrGrpData			*m_pMgrGrpData;				/* グラフィックデータ管理 */
	CMgrDraw			*m_pMgrDraw;				/* 描画管理 */
	CMgrLayer			*m_pMgrLayer;				/* レイヤー管理 */
	CMgrWindow			*m_pMgrWindow;				/* ウィンドウ管理 */
	CMgrKeyInput		*m_pMgrKeyInput;			/* キー入力管理 */

	CLibInfoCharCli		*m_pLibInfoChar;			/* キャラ情報ライブラリ */
	CLibInfoDisable		*m_pLibInfoDisable;			/* 拒否情報ライブラリ */
	CLibInfoMapBase		*m_pLibInfoMap;				/* マップ情報ライブラリ */
	CLibInfoMapObject	*m_pLibInfoMapObject;		/* マップオブジェクト情報ライブラリ */
	CLibInfoMapParts	*m_pLibInfoMapParts;		/* マップパーツ情報ライブラリ */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;		/* マップ影情報ライブラリ */
	CLibInfoItem		*m_pLibInfoItem;			/* アイテム情報ライブラリ */
	CLibInfoItemType	*m_pLibInfoItemType;		/* アイテム種別情報ライブラリ */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;		/* 武器情報ライブラリ */
	CLibInfoEffect		*m_pLibInfoEffect;			/* エフェクト情報ライブラリ */
	CLibInfoMotion		*m_pLibInfoMotion;			/* モーション情報ライブラリ */
	CLibInfoMotionType	*m_pLibInfoMotionType;		/* モーション種別情報ライブラリ */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;		/* 噴出し情報ライブラリ */
	CLibInfoSystem		*m_pLibInfoSystem;			/* システム情報ライブラリ */
	CLibInfoSkill		*m_pLibInfoSkill;			/* スキル情報ライブラリ */
	CLibInfoCharCli		*m_pLibInfoCharOnline;		/* ログイン中キャラ情報ライブラリ */

	CmyArray<PSTSYSTEMMSG, PSTSYSTEMMSG>	m_aSystemMsg;	/* システムメッセージ */
} CMgrData, *PCMgrData;

/* Copyright(C)URARA-works 2006 */

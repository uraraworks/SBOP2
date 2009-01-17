/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharSvr.h											 */
/* 内容			:キャラ情報ライブラリクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/21													 */
/* ========================================================================= */

#pragma once

#include "LibInfoCharBase.h"

class CMainFrame;
class CMgrData;
class CInfoCharSvr;
class CInfoSkillBase;
class CLibInfoMapBase;
class CLibInfoSkill;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoCharSvr : public CLibInfoCharBase
{
public:
			CLibInfoCharSvr();							/* コンストラクタ */
	virtual ~CLibInfoCharSvr();						/* デストラクタ */

	void	Create			(CMgrData *pMgrData);				/* 作成 */
	void	Destroy			(void);								/* 破棄 */
	BOOL	Proc			(void);								/* 処理 */

	int		GetCountLogIn	(void);													/* ログイン中キャラ数を取得 */
	int		GetCountOnline	(DWORD dwMapID = 0);									/* オンライン中のPC数を取得 */
	void	LogIn			(DWORD dwCharID, DWORD dwSessionID, DWORD dwAccountID);	/* ログイン */
	void	LogOut			(DWORD dwCharID);										/* ログアウト */
	void	Revice			(void);													/* データの補正 */
	void	Move			(DWORD dwCharID, int nDirection, BOOL bTurn = FALSE);	/* 1歩進める */
	DWORD	Tail			(PCInfoCharBase pChar, PCInfoCharBase pCharTarget, BOOL bTail);	/* 付いて行く設定・解除 */
	BOOL	Atack			(CInfoCharSvr *pChar);									/* 攻撃 */
	BOOL	Equip			(CInfoCharSvr *pChar, DWORD dwItemID);					/* 装備 */
	BOOL	UnEquip			(CInfoCharSvr *pChar, DWORD dwItemID);					/* 装備解除 */
	BOOL	UseItem			(CInfoCharSvr *pChar, DWORD dwItemID);					/* アイテム使用 */
	void	DragItem		(CInfoCharSvr *pChar, DWORD dwItemID, POINT ptNewPos);	/* アイテム位置変更 */
	void	RenewGrpID		(DWORD dwCharID);										/* 装備画像IDを更新 */
	DWORD	GetPlaceName	(CmyString &strDst);									/* 最も人が集まっているマップ名を取得 */
	void	MoveMapIn		(CInfoCharSvr *pInfoChar);								/* マップ内移動 */
	void	MoveMapOut		(CInfoCharSvr *pInfoChar);								/* マップ外移動 */
	void	SetInitStatus	(CInfoCharSvr *pInfoChar, BOOL bInitPos=FALSE);			/* ステータス初期値設定 */
	void	GetDistance		(SIZE &sizeDst, CInfoCharSvr *pInfoCharSrc, CInfoCharSvr *pInfoCharDst);/* キャラ座標で距離を取得 */

	PCInfoCharBase	AddNPC			(CInfoCharBase *pInfoChar);						/* NPCの追加 */
	PCInfoBase		GetNew			(int nType);									/* 新規データを取得 */
	void			SetPtr			(DWORD dwCharID, PCInfoCharBase pChar);			/* キャラ情報を更新 */
	PCInfoCharBase	GetPtrSessionID	(DWORD dwSessionID);							/* キャラ情報を取得(セッションIDから) */
	PCInfoCharBase	GetPtrLogIn		(int nNo);										/* ログイン中キャラ情報を取得 */
	PCInfoCharBase	GetPtrLogIn		(DWORD dwCharID);								/* ログイン中キャラ情報を取得 */
	PCInfoCharBase	GetPtrParent	(CInfoCharSvr *pChar);							/* 親キャラ情報を取得 */

	int		GetNoLogIn				(DWORD dwCharID);								/* ログイン中キャラの配列番号を取得 */
	void	GetScreenCharID			(CInfoCharSvr *pInfoChar, ARRAYDWORD &aDst);	/* 指定キャラの画面内にいるキャラIDを取得 */
	void	GetScreenCharIDLineOut	(CInfoCharSvr *pInfoChar, ARRAYDWORD &aDst);	/* 指定キャラの画面端にいるキャラIDを取得 */
	void	GetTailCharID			(CInfoCharSvr *pInfoChar, CmyArray<CInfoCharSvr *, CInfoCharSvr *> &aDst);	/* ついてきているキャラIDを取得 */
	BOOL	IsMove					(PCInfoCharBase pInfoChar, int &nDirection, BOOL bEvent=FALSE);	/* 指定方向に進めるかチェック */
	BOOL	IsNPC					(CInfoCharSvr *pInfoChar);						/* NPCか判定 */
	DWORD	GetFrontCharID			(DWORD dwCharID, int nDirection = -1);			/* 一歩前のキャラIDを取得 */
	DWORD	GetFrontCharIDPush		(DWORD dwCharID, int nDirection = -1);			/* 一歩前の押せるキャラIDを取得 */
	DWORD	GetFrontCharIDTarget	(DWORD dwCharID, int nDirection = -1, int nXType=0);/* 一歩前の攻撃対象キャラIDを取得 */
	void	SetPos					(CInfoCharSvr *pInfoChar, DWORD dwMapID, int x, int y, BOOL bTail);	/* キャラの座標を設定 */
	void	RenewItemGrp			(DWORD dwTypeID);								/* 装備中のアイテム画像を更新 */
	void	ProcAtack				(CInfoCharSvr *pInfoChar);						/* 攻撃処理 */


protected:
	void SendSystemMsg				(DWORD dwSessionID, LPCSTR pszMsg, int nMsgType = 2, COLORREF clMsg = RGB (255, 255, 255), BOOL bSound = FALSE);	/* システムメッセージを送信 */
	void SendFormatMsg				(DWORD dwSessionID, DWORD dwMsgID, DWORD dwPara1 = 0, DWORD dwPara2 = 0, int nMsgType = 2, COLORREF clMsg = RGB (255, 255, 255), BOOL bSound = FALSE);	/* フォーマットメッセージを送信 */
	BOOL ProcLocal					(int nNo);							/* 処理 */
	BOOL ProcLocalFlgCheck			(CInfoCharSvr *pInfoChar);			/* フラグチェック処理 */
	BOOL ProcLocalState				(CInfoCharSvr *pInfoChar);			/* 状態に応じた処理 */
	void ProcChgPos					(CInfoCharSvr *pInfoChar);			/* 移動処理 */
	void ProcChgMap					(CInfoCharSvr *pInfoChar);			/* マップ移動処理 */
	void ProcChgPosRenew			(CInfoCharSvr *pInfoChar);			/* 座標変更処理 */
	BOOL ProcLocalStateBATTLEATACK	(CInfoCharSvr *pInfoChar);			/* 状態に応じた処理(戦闘攻撃中) */
	void CharProcMoveMarkPos		(CInfoCharSvr *pInfoChar);			/* 記録位置へ移動 */
	void CharProcSWOON				(CInfoCharSvr *pInfoChar);			/* キャラ処理(気絶) */
	void CharProcAtack				(CInfoCharSvr *pInfoChar);			/* 攻撃 */
	void PutNpc						(CInfoCharSvr *pInfoChar);			/* NPC発生 */
	void DropItem					(CInfoCharSvr *pInfoChar);			/* アイテムドロップ */

	void	GetTargetCharID			(CInfoCharSvr *pInfoChar, int nTarget, int nArea, ARRAYDWORD &adstCharID);	/* 対象キャラIDを取得 */
	BOOL	IsHitAtack				(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget);		/* 攻撃がヒットするか判定 */
	void	Damage					(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget, DWORD dwPoint, int nEffectID, BOOL bCritical);	/* ダメージ処理 */
	DWORD	GetAtackDamage			(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget);		/* 攻撃ダメージを取得 */
	DWORD	GetAtackEffectID		(CInfoCharSvr *pInfoChar, BOOL bCritical);					/* 装備に応じた攻撃時のエフェクトIDを取得 */

	/* アイテム使用処理 */
	int UseItemProcHP				(CInfoCharSvr *pInfoChar, DWORD dwItemID);	/* HP増減 */
	int UseItemProcLIGHT			(CInfoCharSvr *pInfoChar, DWORD dwItemID);	/* 灯り */

	/* マップイベント処理(LibInfoCharSvrMapEvent.cpp) */
	BOOL CheckMapEvent			(CInfoCharSvr *pInfoChar, BOOL bCheck=FALSE);						/* マップイベントチェック */
	BOOL MapEventProcMOVE		(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* マップ内移動 */
	BOOL MapEventProcMAPMOVE	(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* マップ間移動 */
	BOOL MapEventProcINITSTATUS	(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* ステータス初期化 */
	BOOL MapEventProcGRPIDTMP	(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* 一時画像設定 */
	BOOL MapEventProcLIGHT		(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* 灯り */

	/* スキル関連処理(LibInfoCharSvrSkill.cpp) */
public:
	BOOL UseSkill			(CInfoCharSvr *pInfoChar, DWORD dwSkillID);					/* スキル使用 */
protected:
	BOOL UseSkillBATTLE_MOVEATACK	(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);		/* スキル使用(戦闘:移動して攻撃) */
	BOOL UseSkillFISHING			(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);		/* スキル使用(釣り) */


public:
	CMainFrame			*m_pMainFrame;			/* メインフレーム */
	CMgrData			*m_pMgrData;			/* データマネージャ */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */
	CLibInfoMapBase		*m_pLibInfoMap;			/* マップ情報 */
	CLibInfoSkill		*m_pLibInfoSkill;		/* スキル情報 */
	CLibInfoItemType	*m_pLibInfoItemType;	/* アイテム種別情報 */
	CLibInfoItem		*m_pLibInfoItem;		/* アイテム情報 */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* 武器情報 */

	PARRAYINFOCHARBASE	m_paInfoLogin;			/* ログイン中のキャラ情報 */
} CLibInfoCharSvr, *PCLibInfoCharSvr;

/* Copyright(C)URARA-works 2007 */

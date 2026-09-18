/// @file LibInfoCharSvr.h
/// @brief キャラ情報ライブラリクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/21
/// @copyright Copyright(C)URARA-works 2007

#pragma once
#include <vector>
#include "myArray.h"

#include "LibInfoCharBase.h"

class CMainFrame;
class CMgrData;
class CInfoAccount;
class CInfoCharSvr;
class CInfoSkillBase;
class CLibInfoMapBase;
class CLibInfoSkill;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CUraraSockTCPSBO;
class CInfoMapEventBase;

using ARRAYINFOCHARSVR = CStdArray<CInfoCharSvr *>;
using PARRAYINFOCHARSVR = ARRAYINFOCHARSVR *;

// クラス宣言

typedef class CLibInfoCharSvr : public CLibInfoCharBase
{
public:
			CLibInfoCharSvr();	// コンストラクタ
	virtual ~CLibInfoCharSvr();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Destroy(void);	// 破棄
	BOOL	Proc(void);	// 処理

	int	GetCountLogIn(void);	// ログイン中キャラ数を取得
	int	GetCountOnline(DWORD dwMapID = 0);	// オンライン中のPC数を取得
	void	LogIn(DWORD dwCharID, DWORD dwSessionID, DWORD dwAccountID);	// ログイン
	void	LogOut(DWORD dwCharID);	// ログアウト
	void	Revice(void);	// データの補正
	void	Move(DWORD dwCharID, int nDirection, BOOL bTurn = FALSE);	// 1歩進める
	int	GetCharMovePixelsPerSec(CInfoCharSvr *pInfoChar);	// キャラの現在の実移動速度(px/秒)を取得（不正速度チェック用）
	DWORD	Tail(PCInfoCharBase pChar, PCInfoCharBase pCharTarget, BOOL bTail);	// 付いて行く設定・解除
	BOOL	Atack(CInfoCharSvr *pChar);	// 攻撃
	BOOL	AtackImple(CInfoCharSvr *pChar, DWORD dwCharID);	// 攻撃実処理
	BOOL	Equip(CInfoCharSvr *pChar, DWORD dwItemID);	// 装備
	BOOL	UnEquip(CInfoCharSvr *pChar, DWORD dwItemID);	// 装備解除
	BOOL	UseItem(CInfoCharSvr *pChar, DWORD dwItemID);	// アイテム使用
	void	DragItem(CInfoCharSvr *pChar, DWORD dwItemID, POINT ptNewPos);	// アイテム位置変更
	void	RenewGrpID(DWORD dwCharID);	// 装備画像IDを更新
	DWORD	GetPlaceName(CmyString &strDst);	// 最も人が集まっているマップ名を取得
	void	MoveMapIn(CInfoCharSvr *pInfoChar);	// マップ内移動
	void	MoveMapOut(CInfoCharSvr *pInfoChar);	// マップ外移動
	void	SetInitStatus(CInfoCharSvr *pInfoChar, BOOL bInitPos=FALSE);	// ステータス初期値設定

	/// @brief 名前検証込みでプレイヤーキャラクターを新規作成し、アカウントへ追加する。
	///
	/// MainFrameRecvProcACCOUNT.cpp の RecvProcACCOUNT_REQ_MAKECHAR(ゲームスレッド)と
	/// /api/debug/fixture(HTTPスレッド、_DEBUG限定)で共用する。挙動は元のMAKECHAR処理と
	/// 完全に同じにしてある(NameCheckには生の名前を、重複チェックには前後空白を除いた
	/// 名前を渡す既存の非対称な扱いも含む)。
	///
	/// 呼び出し元スレッドを問わず安全に呼べるよう、内部で Enter()/Leave() を取る
	/// (元のゲームスレッド側処理は無施錠だったが、HTTPスレッドから同じ CLibInfoCharSvr を
	/// 触るこの共用化に合わせて追加した)。
	///
	/// @param strRawCharName 検証前の名前(NameCheck・保存に使う。末尾空白を含みうる)
	/// @param strTrimmedCharName 前後空白を除いた名前(重複チェック IsUseName に使う)
	/// @param nMoveType CHARMOVETYPE_*(通常は CHARMOVETYPE_PC)
	/// @param nSex, wFamilyID, wGrpIDEye, wGrpIDEyeColor, wGrpIDHairType, wGrpIDHairColor, dwMotionTypeID: 容姿等の初期値
	/// @param pInfoAccount 追加先アカウント(m_adwCharID に登録する)。NULLなら登録しない
	/// @param outCharID 成功時、新しいキャラID(失敗時は0)
	/// @return MAKECHARRES_OK / MAKECHARRES_NG_USE / MAKECHARRES_NG_SPACE
	int	CreatePlayerCharacter(const CmyString &strRawCharName, const CmyString &strTrimmedCharName,
		int nMoveType, int nSex, WORD wFamilyID, WORD wGrpIDEye, WORD wGrpIDEyeColor,
		WORD wGrpIDHairType, WORD wGrpIDHairColor, DWORD dwMotionTypeID,
		CInfoAccount *pInfoAccount, DWORD &outCharID);

	PCInfoCharBase	AddNPC(CInfoCharBase *pInfoChar);	// NPCの追加
	PCInfoBase	GetNew(int nType);	// 新規データを取得
	void	SetPtr(DWORD dwCharID, PCInfoCharBase pChar);	// キャラ情報を更新
	PCInfoCharBase	GetPtrSessionID(DWORD dwSessionID);	// キャラ情報を取得(セッションIDから)
	PCInfoCharBase	GetPtrLogIn(int nNo);	// ログイン中キャラ情報を取得
	PCInfoCharBase	GetPtrLogIn(DWORD dwCharID);	// ログイン中キャラ情報を取得
	BOOL	IsTrashBoxInFront(CInfoCharSvr *pInfoChar);	// 前方リーチ矩形にゴミ箱があるか（投棄判定用）
	PCInfoCharBase	GetPtrParent(CInfoCharSvr *pChar);	// 親キャラ情報を取得

	int	GetNoLogIn(DWORD dwCharID);	// ログイン中キャラの配列番号を取得
	void	GetScreenCharID(CInfoCharSvr *pInfoChar, ARRAYDWORD &aDst);	// 指定キャラの画面内にいるキャラIDを取得
	void	GetScreenCharIDLineOut(CInfoCharSvr *pInfoChar, ARRAYDWORD &aDst);	// 指定キャラの画面端にいるキャラIDを取得
	void	GetAreaCharInfo(DWORD dwMapID, RECT *prcSrc, ARRAYINFOCHARSVR &aDst);	// 指定範囲にぶつかるキャラ情報を取得
	void	GetTailCharInfo(CInfoCharSvr *pInfoChar, ARRAYINFOCHARSVR &aDst);	// ついてきているキャラIDを取得
	BOOL	IsMove(PCInfoCharBase pInfoChar, int &nDirection, BOOL bEvent=FALSE);	// 指定方向に進めるかチェック
	BOOL	IsNPC(CInfoCharSvr *pInfoChar);	// NPCか判定
	DWORD	GetFrontCharID(DWORD dwCharID, int nDirection = -1);	// 一歩前のキャラIDを取得
	DWORD	GetFrontCharIDTarget(DWORD dwCharID, int nDirection = -1, int nXType=0, PARRAYDWORD padwCharID=NULL);	// 一歩前の攻撃対象キャラIDを取得
	DWORD	GetNearCharID(DWORD dwCharID, SIZE &sizedistance);	// 近くにいる敵キャラIDを取得
	void	SetPos(CInfoCharSvr *pInfoChar, DWORD dwMapID, int x, int y, BOOL bTail);	// キャラの座標を設定
	void	ApplyAdminEditWarp(CInfoCharSvr *pInfoChar, DWORD dwNewMapID, int nNewX, int nNewY, int nNewDir);	// 管理画面によるキャラ位置編集を反映する
	void	RenewItemGrp(DWORD dwTypeID);	// 装備中のアイテム画像を更新
	void	ProcAtack(CInfoCharSvr *pInfoChar);	// 攻撃処理

protected:
	void SendSystemMsg(DWORD dwSessionID, LPCSTR pszMsg, int nMsgType = 2, COLORREF clMsg = RGB(255, 255, 255), BOOL bSound = FALSE);	// システムメッセージを送信
	void SendFormatMsg(DWORD dwSessionID, DWORD dwMsgID, DWORD dwPara1 = 0, DWORD dwPara2 = 0, int nMsgType = 2, COLORREF clMsg = RGB(255, 255, 255), BOOL bSound = FALSE);	// フォーマットメッセージを送信
	BOOL ProcLocal(int nNo);	// 処理
	BOOL ProcLocalFlgCheck(CInfoCharSvr *pInfoChar);	// フラグチェック処理
	BOOL ProcLocalState(CInfoCharSvr *pInfoChar);	// 状態に応じた処理
	void ProcChgPos(CInfoCharSvr *pInfoChar);	// 移動処理
	void ProcChgMap(CInfoCharSvr *pInfoChar);	// マップ移動処理
	void ProcChgPosRenew(CInfoCharSvr *pInfoChar);	// 座標変更処理
	BOOL ProcLocalStateBATTLEATACK(CInfoCharSvr *pInfoChar);	// 状態に応じた処理(戦闘攻撃中)
	void CharProcMoveMarkPos(CInfoCharSvr *pInfoChar);	// 記録位置へ移動
	void CharProcSWOON(CInfoCharSvr *pInfoChar);	// キャラ処理(気絶)
	void CharProcAtack(CInfoCharSvr *pInfoChar);	// 攻撃
	void PutNpc(CInfoCharSvr *pInfoChar);	// NPC発生
	void TargetChar(CInfoCharSvr *pInfoChar);	// 近くのキャラにターゲット変更
	void DropItem(CInfoCharSvr *pInfoChar);	// アイテムドロップ

	void	GetTargetCharID(CInfoCharSvr *pInfoChar, int nTarget, int nArea, ARRAYDWORD &adstCharID);	// 対象キャラIDを取得
	BOOL	IsHitAtack(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget);	// 攻撃がヒットするか判定
	void	Damage(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget, DWORD dwPoint, int nEffectID, BOOL bCritical);	// ダメージ処理
	DWORD	GetAtackDamage(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget);	// 攻撃ダメージを取得
	DWORD	GetAtackEffectID(CInfoCharSvr *pInfoChar, BOOL bCritical);	// 装備に応じた攻撃時のエフェクトIDを取得

	// アイテム使用処理
	int UseItemProcHP(CInfoCharSvr *pInfoChar, DWORD dwItemID);	// HP増減
	int UseItemProcLIGHT(CInfoCharSvr *pInfoChar, DWORD dwItemID);	// 灯り

	// マップイベント処理(LibInfoCharSvrMapEvent.cpp)
	BOOL CheckMapEvent(CInfoCharSvr *pInfoChar, BOOL bCheck=FALSE, int *pnTileX=NULL, int *pnTileY=NULL, BOOL bIgnoreDirection=FALSE, int *pnType=NULL);	// マップイベントチェック
	BOOL MapEventProcMOVE(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	// マップ内移動
	BOOL MapEventProcMAPMOVE(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	// マップ間移動
	BOOL MapEventProcINITSTATUS(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	// ステータス初期化
	BOOL MapEventProcGRPIDTMP(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	// 一時画像設定
	BOOL MapEventProcLIGHT(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	// 灯り

	// スキル関連処理(LibInfoCharSvrSkill.cpp)
public:
	BOOL UseSkill(CInfoCharSvr *pInfoChar, DWORD dwSkillID);	// スキル使用
protected:
	BOOL UseSkillNONE_HEAL(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);	// スキル使用(能力:回復)
	BOOL UseSkillBATTLE_MOVEATACK(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);	// スキル使用(戦闘:移動して攻撃)
	BOOL UseSkillFISHING(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);	// スキル使用(釣り)

public:
	CMainFrame	*m_pMainFrame;	// メインフレーム
	CMgrData	*m_pMgrData;	// データマネージャ
	CUraraSockTCPSBO	*m_pSock;	// 通信マネージャ
	CLibInfoMapBase	*m_pLibInfoMap;	// マップ情報
	CLibInfoSkill	*m_pLibInfoSkill;	// スキル情報
	CLibInfoItemType	*m_pLibInfoItemType;	// アイテム種別情報
	CLibInfoItem	*m_pLibInfoItem;	// アイテム情報
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	// 武器情報
	DWORD	m_dwLastPosSyncTime;	// 最終座標同期通知時間

	PARRAYINFOCHARBASE	m_paInfoLogin;	// ログイン中のキャラ情報
} CLibInfoCharSvr, *PCLibInfoCharSvr;

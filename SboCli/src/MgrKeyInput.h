/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrKeyInput.h												 */
/* 内容			:キー入力管理クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#pragma once

class CDInputUtil;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrKeyInput
{
public:
			CMgrKeyInput();							/* コンストラクタ */
	virtual ~CMgrKeyInput();						/* デストラクタ */

	void	Create		(void);								/* 作成 */
	void	Destroy		(void);								/* 破棄 */

	void	Renew		(BYTE &byCode, BOOL &bDown);		/* 状態更新 */
	void	Reset		(void);								/* 初期化 */
	BOOL	IsInput		(BYTE byCode);						/* 押下判定 */

	/* ジョイパッド関連 */
	void	SetDevice		(int nNo, HWND hWnd);			/* 使用デバイスを設定 */
	void	SetDevice		(GUID &stSrc, HWND hWnd);		/* 使用デバイスを設定 */
	void	Enum			(void);							/* ジョイパッドを列挙 */
	int		GetDeviceCount	(void);							/* デバイス数を取得 */
	BOOL	GetDeviceName	(int nNo, CmyString &strDst);	/* デバイス名を取得 */
	BOOL	GetGUID			(int nNo, GUID &stDst);			/* 指定デバイスのGUIDを取得 */



private:
	BYTE		m_abyCode[256];					/* 入力チェックするコード */
	BYTE		m_abyKeyState[256];				/* キー入力情報 */
	BYTE		m_abyKeyStateBack[256];			/* 前回の入力状態 */

	CDInputUtil	*m_pDInputUtil;					/* DirectInputユーティリティ */
} CMgrKeyInput, *PCMgrKeyInput;

/* Copyright(C)URARA-works 2006 */

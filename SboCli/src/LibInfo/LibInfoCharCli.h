/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharCli.h											 */
/* 内容			:キャラ情報ライブラリクライアントクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "LibInfoCharBase.h"

class CMainFrame;
class CMgrData;
class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoCharCli : public CLibInfoCharBase
{
public:
			CLibInfoCharCli();													/* コンストラクタ */
	virtual ~CLibInfoCharCli();													/* デストラクタ */

	void Create				(CMgrData *pMgrData);										/* 作成 */
	BOOL Proc				(void);														/* 処理 */
	BOOL DeleteOutScreen	(CInfoCharCli *pChar, BOOL bDelete = FALSE);				/* 画面外のキャラを削除 */
	void RenewMotionInfo	(DWORD dwCharID);											/* モーション情報を更新 */
	void RenewMotionInfo	(CInfoCharCli *pChar);										/* モーション情報を更新 */
	void SetMotionInfo		(CInfoCharCli *pChar, DWORD dwMotionID, DWORD dwListID);	/* モーション情報を設定 */
	void RenewGrpID			(DWORD dwCharID);											/* 装備画像IDを更新 */
	BOOL IsMove				(PCInfoCharBase pInfoChar, int &nDirection);				/* 指定方向に進めるかチェック */

	CInfoBase	*GetNew		(int nType);												/* 新規データを取得 */
	CInfoBase	*GetPtr		(int nNo);													/* キャラ情報を取得 */
	CInfoBase	*GetPtr		(DWORD dwCharID);											/* キャラ情報を取得 */


public:
	int		m_nProcNo;						/* 処理中のキャラインデックス */

	CMainFrame	*m_pMainFrame;				/* メインフレーム */
	CMgrData	*m_pMgrData;				/* データマネージャ */
} CLibInfoCharCli, *PCLibInfoCharCli;

/* Copyright(C)URARA-works 2006 */

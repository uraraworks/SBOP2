/// @file InfoTalkEventMENU.h
/// @brief 会話イベント情報(項目選択)クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/28
/// @copyright Copyright(C)URARA-works 2008

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoTalkEventBase.h"

// 項目情報
typedef struct _STTALKEVENTMENUINFO {
	int	nPage;	// ジャンプ先ページ番号
	CmyString	strName;	// 項目名
} STTALKEVENTMENUINFO, *PSTTALKEVENTMENUINFO;
using ARRAYTALKEVENTMENUINFO = CStdArray<PSTTALKEVENTMENUINFO>;
using PARRAYTALKEVENTMENUINFO = ARRAYTALKEVENTMENUINFO *;

typedef class CInfoTalkEventMENU : public CInfoTalkEventBase
{
public:
			CInfoTalkEventMENU();	// コンストラクタ
	virtual ~CInfoTalkEventMENU();	// デストラクタ

	virtual int	GetElementCount(void);	// 要素数を取得
	virtual int	GetElementNo(LPCSTR pszName);	// 要素番号を取得
	virtual DWORD	GetDataSize(void);	// データサイズを取得
	virtual DWORD	GetDataSizeNo(int nNo);	// 指定要素のデータサイズを取得
	virtual LPCSTR	GetName(int nNo);	// 要素名を取得
	virtual PBYTE	GetWriteData(int nNo, PDWORD pdwSize);	// 指定要素の保存用データを取得
	virtual DWORD	ReadElementData(PBYTE pSrc, int nNo);	// 指定要素データを読み込み

	virtual DWORD	GetSendDataSize(void);	// 送信データサイズを取得
	virtual PBYTE	GetSendData(void);	// 送信データを取得
	virtual PBYTE	SetSendData(PBYTE pSrc);	// 送信データから取り込み

	virtual void	Copy(CInfoTalkEventBase *pSrc);	// コピー

	void DeleteMenuInfo(int nNo);	// 項目情報を削除
	void DeleteAllMenuInfo(void);	// 項目情報を全て削除
	void AddMenuInfo(int nPage, LPCSTR pszName);	// 項目情報を追加
	int  GetMenuInfoCount(void);	// 項目数を取得

	PSTTALKEVENTMENUINFO	GetPtr(int nNo);	// 項目情報を取得

public:
	ARRAYTALKEVENTMENUINFO	m_aMenuInfo;	// 項目情報
} CInfoTalkEventMENU, *PCInfoTalkEventMENU;


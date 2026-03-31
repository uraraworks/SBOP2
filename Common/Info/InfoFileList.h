/// @file InfoFileList.h
/// @brief ファイルリスト情報クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/02
/// @copyright Copyright(C)URARA-works 2008

#pragma once
#include <vector>

// ファイルリスト情報
typedef struct _FILELISTINFO {
	CmyString	strMD5,	// MD5ハッシュ
				strFileName;	// ファイル名
	DWORD	dwFileSize;	// ファイルサイズ
} FILELISTINFO, *PFILELISTINFO;

typedef class CInfoFileList
{
public:
			CInfoFileList();	// コンストラクタ
	virtual ~CInfoFileList();	// デストラクタ

	void	Add(LPCSTR pszHash, LPCSTR pszFileName, DWORD dwFileSize);	// 追加
	int	GetCount(void);	// データ数を取得
	PFILELISTINFO	GetPtr(int nNo);	// ファイルリスト情報を取得
	void	DeleteAll(void);	// 全て削除

	void	Copy(CInfoFileList *pSrc);	// コピー
	DWORD	GetSendDataSize(void);	// 送信データサイズを取得
	PBYTE	GetSendData(void);	// 送信データを取得
	PBYTE	SetSendData(PBYTE pSrc);	// 送信データから取り込み

protected:
	PFILELISTINFO	GetNew(void);	// 新規データ取得

protected:
	std::vector<PFILELISTINFO>	m_aFileListInfo;	// ファイルリスト情報
} CInfoFileList, *PCInfoFileList;


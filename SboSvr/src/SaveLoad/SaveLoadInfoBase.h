/// @file SaveLoadInfoBase.h
/// @brief データ保存・読み込み基底クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/26
/// @copyright Copyright(C)URARA-works 2005

#pragma once
#include <vector>
#include "myArray.h"

#include "LibInfoBase.h"

// SQLite3 前方宣言（sqlite3.h は CPP 側でのみ include）
struct sqlite3;

class CInfoBase;

// 構造体定義

// ヘッダ情報
typedef struct _HEADERINFO {
	char	szName[64];	// カラム名
	DWORD	dwOffset;	// オフセット
	DWORD	dwDataSize;	// データサイズ
} HEADERINFO, *PHEADERINFO;

using ARRAYHEADERINFO = CStdArray<PHEADERINFO>;

// クラス宣言

typedef class CSaveLoadInfoBase
{
public:
			CSaveLoadInfoBase();	// コンストラクタ
	virtual ~CSaveLoadInfoBase();	// デストラクタ

	virtual void	WriteData(void);	// データの書き込み（SQLite 優先、.dat フォールバック）
	virtual BOOL	ReadData(void);		// データの読み込み（SQLite 優先、.dat フォールバック）

	virtual void	Save(PCLibInfoBase pSrc);	// ファイルに書き込み
	virtual void	Load(PCLibInfoBase pDst);	// ファイルから読み込み

	// SQLite 共有接続をクラス全体で保持する静的メンバ
	// CMgrData::Save()/Load() の先頭でセット、末尾でクリア
	static sqlite3	*s_pDb;

protected:
	virtual void	SetHeaderInfo(PCInfoBase pInfo);	// ヘッダ情報を設定
	void	MakeWriteData(void);	// 保存データの作成
	void	ReadHeader(void);	// ヘッダ情報の読み込み
	void	SetFileName(LPCSTR pszName);	// ファイル名を設定（フォールバック .dat パスも設定）
	void	SetName(LPCSTR pszName);		// 論理名を設定（SQLite の PRIMARY KEY）
	void	AddHeaderInfo(LPCSTR pszName);	// ヘッダ情報を追加
	PHEADERINFO	GetHeaderInfo(LPCSTR pszName);	// 要素名からヘッダ情報を取得
	void	DelAllHeaderInfo(void);	// ヘッダ情報を削除

	void	CopyMemoryDataDst(PBYTE &pDst, PBYTE pSrc, DWORD dwSize);	// コピーしてコピー先ポインタを進める
	void	CopyMemoryDataSrc(PBYTE pDst, PBYTE &pSrc, DWORD dwSize);	// コピーしてコピー元ポインタを進める

protected:
	CmyString	m_strFileName;	// フォールバック用 .dat ファイルパス
	CmyString	m_strName;		// SQLite PRIMARY KEY 用論理名（例: "Account"）
	PBYTE	m_pData;	// 保存・読み込みデータ
	DWORD	m_dwDataSize;	// データサイズ
	DWORD	m_dwHeaderSize;	// ヘッダサイズ
	ARRAYHEADERINFO	*m_pHeaderInfo;	// ヘッダ情報

	PCLibInfoBase	m_pLibInfoBase;	// 情報クラス
} CSaveLoadInfoBase, *PCSaveLoadInfoBase;

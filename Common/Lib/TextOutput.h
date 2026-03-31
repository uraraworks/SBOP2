/// @file TextOutput.h
/// @brief テキスト出力クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/01
/// @copyright Copyright(C)URARA-works 2005

#ifndef _TEXTOUTPUT_H
#define _TEXTOUTPUT_H

class CTextOutput
{
public:
			CTextOutput();	// コンストラクタ
	virtual ~CTextOutput();	// デストラクタ

	BOOL Create(LPCSTR pszFileName, BOOL bHeader, BOOL m_bReturn);	// 初期化
	void Destroy(void);	// 破棄
	void Write(LPCSTR format, ...);	// 書き込み

protected:
	void WriteProc(LPCSTR pszText);	// 書き込み処理

protected:
	LPSTR	m_pszFileName;	// ファイル名
	BOOL	m_bHeader;	// ヘッダを書き込む
	BOOL	m_bReturn;	// 改行コードを書き込む
	CRITICAL_SECTION	m_csWrite;	// 書き込み排他用
};

#endif

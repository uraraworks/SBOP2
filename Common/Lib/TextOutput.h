/// @file TextOutput.h
/// @brief テキスト出力クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/01
/// @copyright Copyright(C)URARA-works 2005

#ifndef _TEXTOUTPUT_H
#define _TEXTOUTPUT_H

/// @brief CString (SJIS) をログ出力用 UTF-8 に変換するマクロ
/// 使用例: m_cLog.Write("キャラ名: %s", LOGSTR(pInfoChar->m_strCharName));
/// ※ このマクロは一時オブジェクトを返すため、呼び出し式の中でのみ有効
#define LOGSTR(cs) (SjisToUtf8((LPCSTR)(cs)).c_str())

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

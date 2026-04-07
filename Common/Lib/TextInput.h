/// @file TextInput.h
/// @brief テキスト入力クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/05/16
/// @copyright Copyright(C)URARA-works 2006

#pragma once

class CTextInput
{
public:
			CTextInput();	// コンストラクタ
	virtual ~CTextInput();	// デストラクタ

	BOOL	Create(LPCSTR pszFileName);	// 初期化
	void	Destroy(void);	// 破棄
	void	SetLine(DWORD dwLine);	// 読み込む行を設定
	CmyString	ReadLine(DWORD dwLine);	// 指定行から1行読み込み
	CmyString	ReadLine(void);	// 現在位置から1行読み込み
	DWORD	GetLineCount(void);	// 行数を取得

protected:
	BOOL	Open(LPCSTR pszFileName);	// ファイルを開く
	void	Close(void);	// ファイルを閉じる
	CmyString	ReadProc(void);	// 現在位置からの読み込み処理
	BOOL	IsCSVCheck(LPCSTR pszText);	// CSV形式文字列として正しいか判定

protected:
	DWORD	m_dwLine;	// 現在の読み込み位置
	LPSTR	m_pszFileName;	// ファイル名
	FILE*	m_pFile;	// ファイルポインタ
};


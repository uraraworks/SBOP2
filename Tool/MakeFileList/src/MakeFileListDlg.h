/// @file MakeFileListDlg.h
/// @brief メインダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/26
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include <vector>

// クラス宣言

class CMakeFileListDlg : public CDialog
{
public:
	CMakeFileListDlg(CWnd* pParent = NULL);		// コンストラクタ


protected:
	void	ReadIniFile(void);						// iniファイル読み込み
	void	MakeHashList(void);						// ファイルハッシュリスト作成


protected:
	std::vector<CString> m_astrFileList;		// ファイルリスト
	std::vector<CString> m_astrHashList;		// ファイルハッシュリスト



public:
	//{{AFX_DATA(CMakeFileListDlg)
	enum { IDD = IDD_MAKEFILELIST_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMakeFileListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CMakeFileListDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

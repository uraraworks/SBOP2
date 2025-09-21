/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MakeFileListDlg.h											 */
/* 内容			:メインダイアログクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/26													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CMakeFileListDlg : public CDialog
{
public:
	CMakeFileListDlg(CWnd* pParent = NULL);		/* コンストラクタ */


protected:
	void	ReadIniFile		(void);						/* iniファイル読み込み */
	void	MakeHashList	(void);						/* ファイルハッシュリスト作成 */


protected:
	CStringArray	m_astrFileList,				/* ファイルリスト */
					m_astrHashList;				/* ファイルハッシュリスト */



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

/* Copyright(C)URARA-works 2007 */

/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MakeFileListDlg.h											 */
/* ���e			:���C���_�C�A���O�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/26													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CMakeFileListDlg : public CDialog
{
public:
	CMakeFileListDlg(CWnd* pParent = NULL);		/* �R���X�g���N�^ */


protected:
	void	ReadIniFile		(void);						/* ini�t�@�C���ǂݍ��� */
	void	MakeHashList	(void);						/* �t�@�C���n�b�V�����X�g�쐬 */


protected:
	CStringArray	m_astrFileList,				/* �t�@�C�����X�g */
					m_astrHashList;				/* �t�@�C���n�b�V�����X�g */



public:
	//{{AFX_DATA(CMakeFileListDlg)
	enum { IDD = IDD_MAKEFILELIST_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMakeFileListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
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

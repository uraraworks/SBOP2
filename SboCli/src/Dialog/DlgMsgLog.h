/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	DlgMsgLog.h													 */
/* ���e�F		���b�Z�[�W���O�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/09/25													 */
/* ========================================================================= */

#pragma once

#include "resource.h"
#include "LayoutHelper.h"
#include "LogViewCtrl.h"

class CMgrData;
class CTextOutput;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgMsgLog : public CDialog, CLayoutHelper
{
public:
			CDlgMsgLog(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgMsgLog();								/* �f�X�g���N�^ */

	BOOL Create (HWND hWndParent, CMgrData *pMgrData);			/* �쐬 */
	void Add	(LPCSTR pszLog, COLORREF cl);					/* �ǉ� */

	void SetOnlineCount	(int nCount);							/* �I�����C�����X�V */
	void SetCharCount	(int nCount);							/* �L�������X�V */


protected:
	void	MakeLogFile	(void);									/* ���ݎ����Ń��O�t�@�C�����쐬 */
	static LRESULT CALLBACK ChatWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �`���b�g���͗��v���V�[�W�� */


protected:
	HWND			m_hWndChat;				/* �`���b�g���͗� */
	WNDPROC			m_OrgWndProcChat;		/* �`���b�g���͗��̌��E�B���h�E�v���V�[�W�� */
	BOOL			m_bPushEnter;			/* Enter�L�[������� */
	CTime			m_timeMakeLog;			/* ���O�쐬�N���� */
	CMgrData		*m_pMgrData;			/* �f�[�^�Ǘ� */
	CTextOutput		*m_pLog;				/* ���O�t�@�C�� */



public:
	void OnOK () {}
	void OnCancel () {}
	//{{AFX_DATA(CDlgMsgLog)
	enum { IDD = IDD_MSGLOG };
	CString	m_strOnline;
	CString	m_strCharCount;
	CLogViewCtrl m_wndLogViewCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgMsgLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgMsgLog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnLink(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTopMost();
	afx_msg void OnBnClickedHideMainframe();
	BOOL m_bTopMost;
	BOOL m_bHideMainFrame;
	CString m_strChat;
} CDlgMsgLog, *PCDlgMsgLog;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2005 */

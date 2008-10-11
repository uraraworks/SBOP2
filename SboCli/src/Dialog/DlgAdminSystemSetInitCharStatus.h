/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminSystemSetInitCharStatus.h							 */
/* ���e			:�L�����X�e�[�^�X�����l�̐ݒ�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/22													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminSystemSetInitCharStatus : public CDlgAdminBase
{
public:
			CDlgAdminSystemSetInitCharStatus(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminSystemSetInitCharStatus();					/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:



public:
	//{{AFX_DATA(CDlgAdminSystemSetInitCharStatus)
	enum { IDD = IDD_SYSTEM_SET_INITCHARSTATUS };
	DWORD	m_dwMapID;
	int	m_nPosX;
	int	m_nPosY;
	DWORD	m_dwMaxHP;
	DWORD	m_dwMaxSP;
	DWORD	m_dwStamina;
	DWORD	m_dwPower;
	DWORD	m_dwStrength;
	DWORD	m_dwMagic;
	DWORD	m_dwSkillful;
	DWORD	m_dwAbillityAT;
	DWORD	m_dwAbillityDF;
	DWORD	m_dwPAtack;
	DWORD	m_dwPDefense;
	DWORD	m_dwPMagic;
	DWORD	m_dwPMagicDefense;
	DWORD	m_dwPHitAverage;
	DWORD	m_dwPAvoidAverage;
	DWORD	m_dwPCriticalAverage;
	DWORD	m_dwAttrFire;
	DWORD	m_dwAttrWind;
	DWORD	m_dwAttrWater;
	DWORD	m_dwAttrEarth;
	DWORD	m_dwAttrLight;
	DWORD	m_dwAttrDark;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminSystemSetInitCharStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminSystemSetInitCharStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminSystemSetInitCharStatus, *PCDlgAdminSystemSetInitCharStatus;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */

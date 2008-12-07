/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C�����F	AdminWindow.h												 */
/* ���e�F		�Ǘ��҃E�B���h�E���C���t���[���N���X ��`�t�@�C��			 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2006/01/25													 */
/* ========================================================================= */

#pragma once

#include "myThread.h"
#include "LayoutHelper.h"

class CMgrData;
class CWndMap;
class CDlgAdminBase;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ���ID */
enum {
	SCRIDADMIN_TOP = 0,
	SCRIDADMIN_MAP_INFO,					/* �}�b�v���̕ҏW */
	SCRIDADMIN_MAP_EVENT,					/* �}�b�v�C�x���g���̕ҏW */
	SCRIDADMIN_MAP_PARTS,					/* �p�[�c�̕ҏW */
	SCRIDADMIN_MAP_SHADOW,					/* �e�̕ҏW */
	SCRIDADMIN_MAP_OBJECT,					/* �}�b�v�I�u�W�F�N�g�̕ҏW */
	SCRIDADMIN_MAP_OBJECTDATA,				/* �}�b�v�I�u�W�F�N�g�̔z�u */
	SCRIDADMIN_CHAR_MODIFY,					/* �I���L�������ҏW[��{���] */
	SCRIDADMIN_CHAR_MODIFY_STATUS,			/* �I���L�������ҏW[�X�e�[�^�X] */
	SCRIDADMIN_CHAR_MODIFY_GRP,				/* �I���L�������ҏW[�摜�̐ݒ�] */
	SCRIDADMIN_CHAR_MODIFY_ITEM,			/* �I���L�������ҏW[�����A�C�e���̐ݒ�] */
	SCRIDADMIN_CHAR_LIST,					/* �ꗗ */
	SCRIDADMIN_CHAR_ADMIN,					/* �Ǘ��Ҍ����̐ݒ� */
	SCRIDADMIN_CHAR_ADDNPC,					/* NPC�̒ǉ� */
	SCRIDADMIN_CHAR_MOTION,					/* ���[�V�����̕ҏW */
	SCRIDADMIN_CHAR_ACCOUNTINFO,			/* �A�J�E���g���̕ҏW */
	SCRIDADMIN_CHAR_SKILL,					/* �X�L���̕ҏW */
	SCRIDADMIN_ITEMTYPE_LIST,				/* �A�C�e����ʈꗗ */
	SCRIDADMIN_ITEM_LIST,					/* �A�C�e���ꗗ */
	SCRIDADMIN_ITEM_WEAPONLIST,				/* ������ꗗ */
	SCRIDADMIN_EFC_BALLOON_LIST,			/* ���o���ꗗ */
	SCRIDADMIN_EFC_EFFECT_LIST,				/* �G�t�F�N�g�ꗗ */
	SCRIDADMIN_SERVER_INFO,					/* �T�[�o�[��� */
	SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS,	/* �L�����X�e�[�^�X�����l�̐ݒ� */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CAdminWindow : public CWnd, public CLayoutHelper, public CmyThread
{
public:
			CAdminWindow();								/* �R���X�g���N�^ */
	virtual ~CAdminWindow();							/* �f�X�g���N�^ */


	BOOL	Create		(HWND hWndParent, CMgrData *pMgrData);	/* �쐬 */
	void	Destroy		(void);									/* �j�� */

	void	Show		(void);									/* �\�� */
	void	Hide		(void);									/* ��\�� */
	void	ChgScreen	(int nScrID);							/* ��ʐ؂�ւ� */


protected:
	void	ThreadMain	(void);							/* ���C������ */


protected:
	HANDLE		m_hInitEventWindow;				/* �E�B���h�E�쐬�����C�x���g */
	HWND		m_hWnd,							/* �E�B���h�E�n���h�� */
				m_hWndParent;					/* �e�E�B���h�E�n���h�� */
	int			m_nScrID;						/* ���ID */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CWndMap		*m_pWndMap;						/* �}�b�v�E�B���h�E */
	CMenu		m_Menu;							/* ���j���[ */

	CMgrData			*m_pMgrData;			/* �f�[�^�}�l�[�W�� */
	CLayoutHelper		*m_pLayoutHelper;		/* �z�u�⏕ */
	CDlgAdminBase		*m_pDlgBase;			/* �\�����̉�� */



public:
	DECLARE_DYNAMIC(CAdminWindow)
	//{{AFX_VIRTUAL(CAdminWindow)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CAdminWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMapInfo();
	afx_msg void OnMapEvent();
	afx_msg void OnMapParts();
	afx_msg void OnMapShadow();
	afx_msg void OnMapAdd();
	afx_msg void OnMapObject();
	afx_msg void OnMapObjectData();
	afx_msg void OnMapWindow();
	afx_msg void OnCharModify();
	afx_msg void OnCharModifyStatus();
	afx_msg void OnCharModifyGrp();
	afx_msg void OnCharModifyItem();
	afx_msg void OnCharList();
	afx_msg void OnCharAdmin();
	afx_msg void OnCharAddNPC();
	afx_msg void OnCharMotion();
	afx_msg void OnCharAccountInfo();
	afx_msg void OnCharSkill();
	afx_msg void OnItemTypeList();
	afx_msg void OnItemList();
	afx_msg void OnItemWeaponList();
	afx_msg void OnEfcBalloonList();
	afx_msg void OnEfcEffectList();
	afx_msg void OnServerInfo();
	afx_msg void OnDebugMoveNoBlock();
	afx_msg void OnDebugGridOff();
	afx_msg void OnDebugGrid();
	afx_msg void OnDebugGridHalf();
	afx_msg void OnSystemSetInitCharStatus();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainFrame(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnAdminJobInfo(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CAdminWindow, *PCAdminWindow;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */

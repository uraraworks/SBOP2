/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharSvr.h											 */
/* ���e			:�L������񃉃C�u�����N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/21													 */
/* ========================================================================= */

#pragma once

#include "LibInfoCharBase.h"

class CMainFrame;
class CMgrData;
class CInfoCharSvr;
class CInfoSkillBase;
class CLibInfoMapBase;
class CLibInfoSkill;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoCharSvr : public CLibInfoCharBase
{
public:
			CLibInfoCharSvr();							/* �R���X�g���N�^ */
	virtual ~CLibInfoCharSvr();						/* �f�X�g���N�^ */

	void	Create			(CMgrData *pMgrData);				/* �쐬 */
	void	Destroy			(void);								/* �j�� */
	BOOL	Proc			(void);								/* ���� */

	int		GetCountLogIn	(void);													/* ���O�C�����L���������擾 */
	int		GetCountOnline	(DWORD dwMapID = 0);									/* �I�����C������PC�����擾 */
	void	LogIn			(DWORD dwCharID, DWORD dwSessionID, DWORD dwAccountID);	/* ���O�C�� */
	void	LogOut			(DWORD dwCharID);										/* ���O�A�E�g */
	void	Revice			(void);													/* �f�[�^�̕␳ */
	void	Move			(DWORD dwCharID, int nDirection, BOOL bTurn = FALSE);	/* 1���i�߂� */
	DWORD	Tail			(PCInfoCharBase pChar, PCInfoCharBase pCharTarget, BOOL bTail);	/* �t���čs���ݒ�E���� */
	BOOL	Atack			(CInfoCharSvr *pChar);									/* �U�� */
	BOOL	Equip			(CInfoCharSvr *pChar, DWORD dwItemID);					/* ���� */
	BOOL	UnEquip			(CInfoCharSvr *pChar, DWORD dwItemID);					/* �������� */
	BOOL	UseItem			(CInfoCharSvr *pChar, DWORD dwItemID);					/* �A�C�e���g�p */
	void	DragItem		(CInfoCharSvr *pChar, DWORD dwItemID, POINT ptNewPos);	/* �A�C�e���ʒu�ύX */
	void	RenewGrpID		(DWORD dwCharID);										/* �����摜ID���X�V */
	DWORD	GetPlaceName	(CmyString &strDst);									/* �ł��l���W�܂��Ă���}�b�v�����擾 */
	void	MoveMapIn		(CInfoCharSvr *pInfoChar);								/* �}�b�v���ړ� */
	void	MoveMapOut		(CInfoCharSvr *pInfoChar);								/* �}�b�v�O�ړ� */
	void	SetInitStatus	(CInfoCharSvr *pInfoChar, BOOL bInitPos=FALSE);			/* �X�e�[�^�X�����l�ݒ� */
	void	GetDistance		(SIZE &sizeDst, CInfoCharSvr *pInfoCharSrc, CInfoCharSvr *pInfoCharDst);/* �L�������W�ŋ������擾 */

	PCInfoCharBase	AddNPC			(CInfoCharBase *pInfoChar);						/* NPC�̒ǉ� */
	PCInfoBase		GetNew			(int nType);									/* �V�K�f�[�^���擾 */
	void			SetPtr			(DWORD dwCharID, PCInfoCharBase pChar);			/* �L���������X�V */
	PCInfoCharBase	GetPtrSessionID	(DWORD dwSessionID);							/* �L���������擾(�Z�b�V����ID����) */
	PCInfoCharBase	GetPtrLogIn		(int nNo);										/* ���O�C�����L���������擾 */
	PCInfoCharBase	GetPtrLogIn		(DWORD dwCharID);								/* ���O�C�����L���������擾 */
	PCInfoCharBase	GetPtrParent	(CInfoCharSvr *pChar);							/* �e�L���������擾 */

	int		GetNoLogIn				(DWORD dwCharID);								/* ���O�C�����L�����̔z��ԍ����擾 */
	void	GetScreenCharID			(CInfoCharSvr *pInfoChar, ARRAYDWORD &aDst);	/* �w��L�����̉�ʓ��ɂ���L����ID���擾 */
	void	GetScreenCharIDLineOut	(CInfoCharSvr *pInfoChar, ARRAYDWORD &aDst);	/* �w��L�����̉�ʒ[�ɂ���L����ID���擾 */
	void	GetTailCharID			(CInfoCharSvr *pInfoChar, CmyArray<CInfoCharSvr *, CInfoCharSvr *> &aDst);	/* ���Ă��Ă���L����ID���擾 */
	BOOL	IsMove					(PCInfoCharBase pInfoChar, int &nDirection, BOOL bEvent=FALSE);	/* �w������ɐi�߂邩�`�F�b�N */
	BOOL	IsNPC					(CInfoCharSvr *pInfoChar);						/* NPC������ */
	DWORD	GetFrontCharID			(DWORD dwCharID, int nDirection = -1);			/* ����O�̃L����ID���擾 */
	DWORD	GetFrontCharIDPush		(DWORD dwCharID, int nDirection = -1);			/* ����O�̉�����L����ID���擾 */
	DWORD	GetFrontCharIDTarget	(DWORD dwCharID, int nDirection = -1, int nXType=0);/* ����O�̍U���ΏۃL����ID���擾 */
	void	SetPos					(CInfoCharSvr *pInfoChar, DWORD dwMapID, int x, int y, BOOL bTail);	/* �L�����̍��W��ݒ� */
	void	RenewItemGrp			(DWORD dwTypeID);								/* �������̃A�C�e���摜���X�V */
	void	ProcAtack				(CInfoCharSvr *pInfoChar);						/* �U������ */


protected:
	void SendSystemMsg				(DWORD dwSessionID, LPCSTR pszMsg, int nMsgType = 2, COLORREF clMsg = RGB (255, 255, 255), BOOL bSound = FALSE);	/* �V�X�e�����b�Z�[�W�𑗐M */
	void SendFormatMsg				(DWORD dwSessionID, DWORD dwMsgID, DWORD dwPara1 = 0, DWORD dwPara2 = 0, int nMsgType = 2, COLORREF clMsg = RGB (255, 255, 255), BOOL bSound = FALSE);	/* �t�H�[�}�b�g���b�Z�[�W�𑗐M */
	BOOL ProcLocal					(int nNo);							/* ���� */
	BOOL ProcLocalFlgCheck			(CInfoCharSvr *pInfoChar);			/* �t���O�`�F�b�N���� */
	BOOL ProcLocalState				(CInfoCharSvr *pInfoChar);			/* ��Ԃɉ��������� */
	void ProcChgPos					(CInfoCharSvr *pInfoChar);			/* �ړ����� */
	void ProcChgMap					(CInfoCharSvr *pInfoChar);			/* �}�b�v�ړ����� */
	void ProcChgPosRenew			(CInfoCharSvr *pInfoChar);			/* ���W�ύX���� */
	BOOL ProcLocalStateBATTLEATACK	(CInfoCharSvr *pInfoChar);			/* ��Ԃɉ���������(�퓬�U����) */
	void CharProcMoveMarkPos		(CInfoCharSvr *pInfoChar);			/* �L�^�ʒu�ֈړ� */
	void CharProcSWOON				(CInfoCharSvr *pInfoChar);			/* �L��������(�C��) */
	void CharProcAtack				(CInfoCharSvr *pInfoChar);			/* �U�� */
	void PutNpc						(CInfoCharSvr *pInfoChar);			/* NPC���� */
	void DropItem					(CInfoCharSvr *pInfoChar);			/* �A�C�e���h���b�v */

	void	GetTargetCharID			(CInfoCharSvr *pInfoChar, int nTarget, int nArea, ARRAYDWORD &adstCharID);	/* �ΏۃL����ID���擾 */
	BOOL	IsHitAtack				(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget);		/* �U�����q�b�g���邩���� */
	void	Damage					(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget, DWORD dwPoint, int nEffectID, BOOL bCritical);	/* �_���[�W���� */
	DWORD	GetAtackDamage			(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget);		/* �U���_���[�W���擾 */
	DWORD	GetAtackEffectID		(CInfoCharSvr *pInfoChar, BOOL bCritical);					/* �����ɉ������U�����̃G�t�F�N�gID���擾 */

	/* �A�C�e���g�p���� */
	int UseItemProcHP				(CInfoCharSvr *pInfoChar, DWORD dwItemID);	/* HP���� */
	int UseItemProcLIGHT			(CInfoCharSvr *pInfoChar, DWORD dwItemID);	/* ���� */

	/* �}�b�v�C�x���g����(LibInfoCharSvrMapEvent.cpp) */
	BOOL CheckMapEvent			(CInfoCharSvr *pInfoChar, BOOL bCheck=FALSE);						/* �}�b�v�C�x���g�`�F�b�N */
	BOOL MapEventProcMOVE		(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* �}�b�v���ړ� */
	BOOL MapEventProcMAPMOVE	(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* �}�b�v�Ԉړ� */
	BOOL MapEventProcINITSTATUS	(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* �X�e�[�^�X������ */
	BOOL MapEventProcGRPIDTMP	(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* �ꎞ�摜�ݒ� */
	BOOL MapEventProcLIGHT		(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase);	/* ���� */

	/* �X�L���֘A����(LibInfoCharSvrSkill.cpp) */
public:
	BOOL UseSkill			(CInfoCharSvr *pInfoChar, DWORD dwSkillID);					/* �X�L���g�p */
protected:
	BOOL UseSkillBATTLE_MOVEATACK	(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);		/* �X�L���g�p(�퓬:�ړ����čU��) */
	BOOL UseSkillFISHING			(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase);		/* �X�L���g�p(�ނ�) */


public:
	CMainFrame			*m_pMainFrame;			/* ���C���t���[�� */
	CMgrData			*m_pMgrData;			/* �f�[�^�}�l�[�W�� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */
	CLibInfoMapBase		*m_pLibInfoMap;			/* �}�b�v��� */
	CLibInfoSkill		*m_pLibInfoSkill;		/* �X�L����� */
	CLibInfoItemType	*m_pLibInfoItemType;	/* �A�C�e����ʏ�� */
	CLibInfoItem		*m_pLibInfoItem;		/* �A�C�e����� */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* ������ */

	PARRAYINFOCHARBASE	m_paInfoLogin;			/* ���O�C�����̃L������� */
} CLibInfoCharSvr, *PCLibInfoCharSvr;

/* Copyright(C)URARA-works 2007 */

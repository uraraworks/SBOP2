/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoItem.h												 */
/* ���e			:�A�C�e����񃉃C�u�������N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "InfoItem.h"
#include "LibInfoBase.h"

class CInfoCharBase;
class CLibInfoItemType;
class CLibInfoItemWeapon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoItem : public CLibInfoBase
{
public:
			CLibInfoItem();									/* �R���X�g���N�^ */
	virtual ~CLibInfoItem();								/* �f�X�g���N�^ */

	void Create			(void);										/* �쐬 */
	void Destroy		(void);										/* �j�� */
	void SetTypeInfo	(CLibInfoItemType *pLibInfoItemType);		/* �A�C�e����ʏ���ݒ� */
	void SetWeaponInfo	(CLibInfoItemWeapon *pLibInfoItemWeapon);	/* �������ݒ� */
	void RenewSize		(DWORD dwMapID, int nDirection, int nSize);	/* �}�b�v�T�C�Y�X�V */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwItemID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	Sort		(void);									/* �\���ʒu�ɍ��킹�ă\�[�g */
	LPCSTR	GetTypeName		(DWORD dwTypeID);					/* �A�C�e����ʖ����擾 */
	DWORD	GetItemType		(DWORD dwItemID);					/* �A�C�e����ʂ��擾 */
	DWORD	GetItemTypeID	(DWORD dwItemID);					/* �A�C�e�����ID���擾 */

	DWORD	GetMotionIDAtack		(DWORD dwItemID);			/* �g�p�\�ȍU�����[�V����ID���擾 */
	DWORD	GetMotionIDBattleStand	(DWORD dwItemID);			/* �퓬���[�h���̗������[�V����ID���擾 */
	DWORD	GetMotionIDBattleWalk	(DWORD dwItemID);			/* �퓬���[�h���̂��葫���[�V����ID���擾 */

	PCInfoBase	GetPtr (int nNo);								/* �A�C�e�������擾 */
	PCInfoBase	GetPtr (DWORD dwItemID);						/* �A�C�e�������擾 */
	PCInfoBase	GetPtr (DWORD dwMapID, POINT *pptPos, BOOL bPoint = TRUE);	/* �A�C�e�������擾 */
	PCInfoBase	GetItemTypePtr	(DWORD dwItemID);				/* �A�C�e����ʏ����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */

	void	AddItem		(DWORD dwCharID, DWORD dwItemID, ARRAYDWORD *padwItemID);			/* �����A�C�e����ǉ� */
	void	DeleteItem	(DWORD dwItemID, CInfoCharBase *pInfoChar, BOOL bNoPos = FALSE);	/* �����A�C�e�����폜 */
	void	Equip		(CInfoCharBase *pInfoChar, DWORD dwItemIDOld, DWORD dwItemIDNew);	/* �A�C�e���ʒu�𑕔��p�ɓ���ւ� */
	DWORD	MakeItem	(DWORD dwMapID, POINT *pptPos, DWORD dwItemTypeID);					/* �w��ꏊ�ɃA�C�e�����쐬 */
	BOOL	GetFreePos	(POINT &ptDst, ARRAYDWORD *padwItemID);	/* �A�C�e���̋󂫏ꏊ���擾 */
	BOOL	IsUseItem	(DWORD dwItemID);						/* �g�p�ł���A�C�e�������� */

	void		SetArea			(DWORD dwMapID, RECT *prcArea);	/* �͈͎w��ꗗ�쐬 */
	int			GetAreaCount	(void);							/* �͈͎w��ꗗ���擾 */
	PCInfoBase	GetPtrArea		(int nNo);						/* �͈͎w��ꗗ����A�C�e�������擾 */


protected:
	DWORD	GetNewID	(void);										/* �V�����A�C�e��ID���擾 */
	BOOL	IsItemPos	(POINT *ptItem, ARRAYDWORD *padwItemID);	/* �w��ꏊ���g�p�ς݂��`�F�b�N */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */

	ARRAYDWORD			m_adwAreaID;			/* �͈͎w��ꗗ */
	PARRAYITEMINFO		m_paInfo;				/* �A�C�e����� */
	CLibInfoItemType	*m_pLibInfoItemType;	/* �A�C�e����ʏ�� */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* ������ */
} CLibInfoItem, *PCLibInfoItem;

/* Copyright(C)URARA-works 2007 */

/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoSystem.h												 */
/* ���e			:�V�X�e�����N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/25													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */
/* �L�����X�e�[�^�X�����l */
typedef struct _STSYSTEM_INITCHARSTATUS {
	WORD		wStamina,				/* �X�^�~�i */
				wPower,					/* �r�� */
				wStrength,				/* �̗� */
				wMagic,					/* ���� */
				wSkillful,				/* ��p */
				wAbillityAT,			/* �U���Z�\ */
				wAbillityDF,			/* �h��Z�\ */
				wPAtack,				/* �U���� */
				wPDefense,				/* �h��� */
				wPMagic,				/* ���@�� */
				wPMagicDefense,			/* ���@�h��� */
				wPHitAverage,			/* ������ */
				wPAvoidAverage,			/* ��� */
				wPCriticalAverage,		/* �N���e�B�J���� */
				wAttrFire,				/* ����[��] */
				wAttrWind,				/* ����[��] */
				wAttrWater,				/* ����[��] */
				wAttrEarth,				/* ����[�y] */
				wAttrLight,				/* ����[��] */
				wAttrDark;				/* ����[��] */
	DWORD		dwMaxHP,				/* �ő�HP */
				dwMaxSP,				/* �ő�SP */
				dwInitPosMapID;			/* �����ʒu�}�b�vID */
	POINT		ptInitPos;				/* �����ʒu���W */
} STSYSTEM_INITCHARSTATUS, *PSTSYSTEM_INITCHARSTATUS;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoSystem : public CInfoBase
{
public:
			CInfoSystem();									/* �R���X�g���N�^ */
	virtual ~CInfoSystem();									/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoSystem *pSrc);			/* �R�s�[ */


public:
	PSTSYSTEM_INITCHARSTATUS	m_pInitCharStatus;		/* �L�����X�e�[�^�X�����l */
} CInfoSystem, *PCInfoSystem;
typedef CmyArray<PCInfoSystem, PCInfoSystem>	   ARRAYSYSTEMINFO;
typedef CmyArray<PCInfoSystem, PCInfoSystem>	 *PARRAYSYSTEMINFO;

/* Copyright(C)URARA-works 2008 */

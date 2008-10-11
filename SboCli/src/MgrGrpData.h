/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrGrpData.h												 */
/* ���e			:�O���t�B�b�N�f�[�^�}�l�[�W���N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CImg32;

/* ========================================================================= */
/* �萔�錾																	 */
/* ========================================================================= */

typedef CmyArray<CImg32 *, CImg32 *>	  ARRAYCIMG;
typedef CmyArray<CImg32 *, CImg32 *>	*PARRAYCIMG;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrGrpData
{
public:
			CMgrGrpData();									/* �R���X�g���N�^ */
	virtual ~CMgrGrpData();									/* �f�X�g���N�^ */

	BOOL	Load				(void);								/* �ǂݍ��� */
	BOOL	CheckVersion		(LPCSTR pszVersion);				/* �o�[�W�����`�F�b�N */
	void	Write				(LPCSTR pszFileName, CImg32 *pSrc);	/* PNG�f�[�^�̕ۑ� */
	CImg32	*GetDibSystem		(void);								/* �V�X�e���摜�C���[�W���擾 */
	CImg32	*GetDibShadow		(void);								/* �e�摜�C���[�W���擾 */
	CImg32	*GetDibMapParts		(int nNo);							/* �}�b�v�摜�C���[�W���擾 */
	CImg32	*GetDibMapShadow	(int nNo);							/* �}�b�v�e�摜�C���[�W���擾 */
	CImg32	*GetDibItem			(int nNo);							/* �A�C�e���摜�C���[�W���擾 */
	CImg32	*GetDibNumS			(void);								/* ����(S)�摜�C���[�W���擾 */
	CImg32	*GetDibNumM			(void);								/* ����(M)�摜�C���[�W���擾 */
	CImg32	*GetDibNumL			(void);								/* ����(L)�摜�C���[�W���擾 */
	CImg32	*GetDibNPC			(int nNo);							/* NPC�摜�C���[�W���擾 */
	CImg32	*GetDibBody			(WORD wFamilyID);					/* �̉摜�C���[�W���擾 */
	CImg32	*GetDibEar			(WORD wFamilyID);					/* ���摜�C���[�W���擾 */
	CImg32	*GetDibCloth		(int nNo);							/* ���摜�C���[�W���擾 */
	CImg32	*GetDibEye			(int nNo);							/* �ډ摜�C���[�W���擾 */
	CImg32	*GetDibHairD		(WORD wHairColorID);				/* ��(��)�摜�C���[�W���擾 */
	CImg32	*GetDibHairU		(WORD wHairColorID);				/* ��(��)�摜�C���[�W���擾 */
	CImg32	*GetDibSP			(int nNo);							/* ���ꕞ�摜�C���[�W���擾 */
	CImg32	*GetDibAcce			(int nNo);							/* �A�N�Z�T���摜�C���[�W���擾 */
	CImg32	*GetDibEfc32		(int nNo);							/* �G�t�F�N�g(32)�摜�C���[�W���擾 */
	CImg32	*GetDibEfc64		(int nNo);							/* �G�t�F�N�g(64)�摜�C���[�W���擾 */
	CImg32	*GetDibWeapon		(int nNo);							/* ����摜�C���[�W���擾 */
	CImg32	*GetDibWeaponBow	(int nNo);							/* ����(�|)�摜�C���[�W���擾 */
	CImg32	*GetDibWeaponArrow	(int nNo);							/* ����(��)�摜�C���[�W���擾 */
	CImg32	*GetDibWeaponGlove	(int nNo);							/* ����(�Ō�)�摜�C���[�W���擾 */
	CImg32	*GetDibWeaponEtc	(int nNo);							/* ����(���̑�)�摜�C���[�W���擾 */
	CImg32	*GetDibEfcBalloon	(int nNo);							/* ���o���摜�C���[�W���擾 */
	CImg32	*GetDib2x2Body		(WORD wFamilyID);					/* 2x2�̉摜�C���[�W���擾 */
	CImg32	*GetDib2x2Cloth		(WORD wClothID);					/* 2x2���摜�C���[�W���擾 */
	CImg32	*GetDib2x2Eye		(int nNo);							/* 2x2�ډ摜�C���[�W���擾 */
	CImg32	*GetDib2x2Hair		(WORD wHairColorID);				/* 2x2���摜�C���[�W���擾 */
	CImg32	*GetDib2x2SPCloth	(WORD wSPClothID);					/* 2x2���ꕞ�摜�C���[�W���擾 */
	CImg32	*GetDib2x2SPHair	(WORD wSPHairID);					/* 2x2���ꔯ�摜�C���[�W���擾 */
	CImg32	*GetDib2x2Arms		(WORD wArmsID);						/* 2x2�������摜�C���[�W���擾 */
	CImg32	*GetDib2x2Shield	(WORD wShieldID);					/* 2x2���摜�C���[�W���擾 */
	CImg32	*GetDib2x2ArmsSP	(WORD wArmsID);						/* 2x2���ꎝ�����摜�C���[�W���擾 */
	CImg32	*GetDib2x2Bow		(WORD wBowID);						/* 2x2�|�摜�C���[�W���擾 */
	CImg32	*GetDib2x2NPC		(WORD wNPCID);						/* 2x2NPC�摜�C���[�W���擾 */
	CImg32	*GetDib2x2CharShadow(int nNo);							/* 2x2�L�����e�摜�C���[�W���擾 */
	CImg32	*GetDib2x2NPCShadow	(int nNo);							/* 2x2NPC�e�摜�C���[�W���擾 */

	CImg32	*GetDib				(DWORD dwGrpIDMain, DWORD dwGrpIDSub, DWORD dwParam);	/* �摜�C���[�W���擾 */

	CImg32	*GetDibTmpLogo			(void);							/* URARA-works���S�摜�C���[�W���擾 */
	CImg32	*GetDibTmpTitle			(void);							/* �^�C�g���摜�C���[�W���擾 */
	CImg32	*GetDibTmpTitleBack		(void);							/* �^�C�g���w�i�摜�C���[�W���擾 */
	CImg32	*GetDibTmpTitleCloud	(void);							/* �^�C�g���_�摜�C���[�W���擾 */
	CImg32	*GetDibTmpLoginMenuBack	(void);							/* ���O�C�����j���[�摜�C���[�W���擾 */

	int		GetMapPartsCount	(void);								/* �}�b�v�p�[�c�����擾 */
	CImg32	*GetDibMapPartsTmp	(int nNo);							/* ���{�}�b�v�摜�C���[�W���擾 */
	void	ReadMapPartsTmp		(void);								/* ���{�}�b�v�p�[�c�摜��ǂݍ��� */
	void	DeleteMapPartsTmp	(void);								/* ���{�}�b�v�p�[�c�摜���폜 */
	int		GetMapShadowCount	(void);								/* �}�b�v�e�摜�����擾 */
	CImg32	*GetDibMapShadowTmp	(int nNo);							/* ���{�}�b�v�e�摜�C���[�W���擾 */
	void	ReadMapShadowTmp	(void);								/* ���{�}�b�v�e�摜��ǂݍ��� */
	void	DeleteMapShadowTmp	(void);								/* ���{�}�b�v�e�摜���폜 */
	int		GetItemCount		(void);								/* �A�C�e���摜�����擾 */
	CImg32	*GetDibItemTmp		(int nNo);							/* ���{�A�C�e���摜�C���[�W���擾 */
	void	ReadItemTmp			(void);								/* ���{�A�C�e���摜��ǂݍ��� */
	void	DeleteItemTmp		(void);								/* ���{�A�C�e���摜���폜 */

	int		GetGrpSize			(DWORD dwGrpIDMain);				/* �摜�P������̃T�C�Y���擾 */
	int		GetGrpCountX		(DWORD dwGrpIDMain);				/* �摜�P�t�@�C���̉������擾 */
	int		GetGrpCountY		(DWORD dwGrpIDMain);				/* �摜�P�t�@�C���̏c�����擾 */
	void	GetGrpPos			(DWORD dwGrpIDMain, DWORD dwGrpIDSub, POINT &ptDst, WORD wGrpID = 0);	/* �w��摜�̈ʒu���擾 */


private:
	BOOL	Read	(LPSTR pszName, CImg32 **pDib, int nSize);	/* PNG�f�[�^�̓ǂݍ��� */
	BOOL	Read256	(LPSTR pszName, CImg32 **pDib, int nSize);	/* PNG�f�[�^�̓ǂݍ��� */


private:
	HMODULE		m_hDll;					/* DLL�̃��W���[���n���h�� */

	int			m_nMapPartsCount,		/* �}�b�v�p�[�c�摜�� */
				m_nMapShadowCount;		/* �}�b�v�e�摜�� */
	CImg32		*m_pImgSystem,			/* �V�X�e�� */
				*m_pImgShadow,			/* �e�摜 */
				*m_pImgItem,			/* �A�C�e���摜 */
				*m_pImgNumS,			/* ����(S) */
				*m_pImgNumM,			/* ����(M) */
				*m_pImgNumL;			/* ����(L) */
	PARRAYCIMG	m_paImgMapParts,		/* �}�b�v�p�[�c */
				m_paImgMapShadow,		/* �}�b�v�e */
				m_paImgNPC,				/* NPC */
				m_paImgBody,			/* �� */
				m_paImgEar,				/* �� */
				m_paImgCloth,			/* �� */
				m_paImgEye,				/* �� */
				m_paImgHairD,			/* ��(��) */
				m_paImgHairU,			/* ��(��) */
				m_paImgSP,				/* ���ꕞ */
				m_paImgAcce,			/* �A�N�Z�T�� */
				m_paImgMapPartsTmp,		/* ���{�}�b�v�p�[�c */
				m_paImgMapShadowTmp,	/* ���{�}�b�v�e */
				m_paImgItemTmp,			/* ���{�A�C�e�� */
				m_paImgEfc32,			/* �G�t�F�N�g(32) */
				m_paImgEfc64,			/* �G�t�F�N�g(64) */
				m_paImgWeapon,			/* ���� */
				m_paImgWeaponBow,		/* ����(�|) */
				m_paImgWeaponArrow,		/* ����(��) */
				m_paImgWeaponGlove,		/* ����(�Ō�) */
				m_paImgWeaponEtc,		/* ����(���̑�) */
				m_paImgEfcBalloon,		/* ���o�� */
				m_paImg2x2Body,			/* 2x2�� */
				m_paImg2x2Cloth,		/* 2x2�� */
				m_paImg2x2Eye,			/* 2x2�� */
				m_paImg2x2Hair,			/* 2x2�� */
				m_paImg2x2SPCloth,		/* 2x2���ꕞ */
				m_paImg2x2SPHair,		/* 2x2���ꔯ */
				m_paImg2x2Arms,			/* 2x2������ */
				m_paImg2x2Shield,		/* 2x2�� */
				m_paImg2x2ArmsSP,		/* 2x2���ꎝ���� */
				m_paImg2x2Bow,			/* 2x2�| */
				m_paImg2x2NPC,			/* 2x2NPC */
				m_paImg2x2CharShadow,	/* 2x2�L�����e */
				m_paImg2x2NPCShadow;	/* 2x2NPC�e */
} CMgrGrpData, *PCMgrGrpData;

/* Copyright(C)URARA-works 2006 */

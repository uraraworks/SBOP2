/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapBase.h												 */
/* ���e			:�}�b�v�����N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/08													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

class CLibInfoMapParts;
class CLibInfoMapEvent;
class CInfoMapEventBase;
class CLibInfoMapObjectData;
class CLibInfoMapObject;
class CInfoMapObjectData;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */
/* �V�� */
enum {
	WEATHERTYPE_NONE = 0,
	WEATHERTYPE_CLOUD,			/* �_ */
	WEATHERTYPE_MISTY,			/* �� */
	WEATHERTYPE_SNOW,			/* �� */
	WEATHERTYPE_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapBase : public CInfoBase
{
public:
			CInfoMapBase();								/* �R���X�g���N�^ */
	virtual ~CInfoMapBase();							/* �f�X�g���N�^ */

	void Create			(CLibInfoMapParts *pLibInfoMapParts);	/* �쐬 */

	void Init			(int cx, int cy, WORD wParts, BOOL bDeleteMapEvent = TRUE);	/* ������ */
	void RenewSize		(int nDirection, int nSize);					/* �T�C�Y�X�V */
	void FillShadow		(int x, int y, int cx, int cy, WORD wShadowID);	/* ��`��h��Ԃ� */

	int		GetElementNo	(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize		(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo	(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName			(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData	(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData	(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual void TimerProc	(DWORD dwTime);						/* ���ԏ��� */

	BOOL	IsMapEvent		(int x, int y);						/* �w����W�Ƀ}�b�v�C�x���g�����邩���� */
	int		GetMapEventType	(int x, int y);						/* �w����W�̃}�b�v�C�x���g��ʂ��擾 */
	WORD	GetParts		(int x, int y);						/* �w����W�̃p�[�c�ԍ����擾 */
	void	SetParts		(int x, int y, DWORD dwPartsID);	/* �w����W�̃p�[�c�ԍ���ݒ� */
	WORD	GetPartsPile	(int x, int y);						/* �w����W�̏d�ˍ��킹�p�[�c�ԍ����擾 */
	void	SetPartsPile	(int x, int y, DWORD dwPartsID);	/* �w����W�̏d�ˍ��킹�p�[�c�ԍ���ݒ� */
	BOOL	IsMove			(int x, int y, int nDirection);		/* �i���\������ */
	BOOL	IsMoveOut		(int x, int y, int nDirection);		/* �E�o�\������ */
	void	DeleteParts		(DWORD dwPartsID);					/* �w��p�[�c���폜 */
	WORD	GetShadow		(int x, int y);						/* �w����W�̉e�ԍ����擾 */
	void	SetShadow		(int x, int y, DWORD dwShadowID);	/* �w����W�̉e�ԍ���ݒ� */
	void	DeleteShadow	(DWORD dwShadowID);					/* �w��e���폜 */
	DWORD	GetMapObject	(int x, int y);						/* �w����W�̃}�b�v�I�u�W�F�N�gID���擾 */
	BOOL	IsFlg			(int x, int y, DWORD dwFlg);		/* �w�葮�������� */
	BOOL	IsEnableBattle	(void);								/* �퓬�������� */

	DWORD	GetSendDataSize	(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData		(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData		(PBYTE pSrc);						/* ���M�f�[�^��ݒ� */

	void	Copy		(CInfoMapBase *pSrc);					/* �R�s�[ */
	void	RenewHitTmp	(void);									/* �}�b�v�p�[�c�ȊO�ł̓����蔻����X�V */

	/* �}�b�v�C�x���g�֘A */
	void				RenewMapEvent	(void);						/* �}�b�v�C�x���g�X�V */
	int					GetEventCount	(void);						/* �}�b�v�C�x���g�����擾 */
	CInfoMapEventBase	*GetEvent		(int nNo);					/* �}�b�v�C�x���g���擾 */
	CInfoMapEventBase	*GetEvent		(int x, int y);				/* �}�b�v�C�x���g���擾 */
	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�֘A */
	int					GetMapObjectDataCount	(void);				/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�����擾 */
	CInfoMapObjectData	*GetObjectData			(int nNo);			/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^���擾 */
	CInfoMapObjectData	*GetObjectData			(int x, int y);		/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^���擾 */


public:
	SIZE		m_sizeMap;				/* �}�b�v�T�C�Y */
	DWORD		m_dwMapID,				/* �}�b�vID */
				m_dwBGMID,				/* BGMID */
				m_dwWeatherType;		/* �V�C��� */
	BOOL		m_bEnableBattle,		/* �퓬���� */
				m_bRecovery;			/* �C���񕜂��� */
	PBYTE		m_pbyMapEvent,			/* �e���W�̃}�b�v�C�x���g */
				m_pbyHitTmp;			/* �}�b�v�p�[�c�ȊO�ł̓����蔻�� */
	PWORD		m_pwMap,				/* �}�b�v */
				m_pwMapPile,			/* �}�b�v�d�ˍ��킹 */
				m_pwMapShadow;			/* �}�b�v�e */
	BYTE		m_byLevel;				/* �Â����x�� */
	CmyString	m_strMapName;			/* �}�b�v�� */

	CLibInfoMapParts		*m_pLibInfoMapParts;		/* �}�b�v�p�[�c��� */
	CLibInfoMapEvent		*m_pLibInfoMapEvent;		/* �}�b�v�C�x���g��� */
	CLibInfoMapObjectData	*m_pLibInfoMapObjectData;	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */
	CLibInfoMapObject		*m_pLibInfoMapObject;		/* �}�b�v�I�u�W�F�N�g��� */
} CInfoMapBase, *PCInfoMapBase;
typedef CmyArray<PCInfoMapBase, PCInfoMapBase>	  ARRAYINFOMAPBASE;
typedef CmyArray<PCInfoMapBase, PCInfoMapBase>	*PARRAYINFOMAPBASE;

/* Copyright(C)URARA-works 2006 */

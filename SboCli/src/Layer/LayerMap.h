/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LayerMap.h													 */
/* ���e			:���C���[�`��N���X(�}�b�v) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CInfoCharCli;
class CLibInfoItem;
class CLibInfoMapParts;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerMap : public CLayerCloud
{
public:
			CLayerMap();											/* �R���X�g���N�^ */
	virtual ~CLayerMap();											/* �f�X�g���N�^ */

	void	Create				(CMgrData *pMgrData);						/* �쐬 */
	void	Draw				(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc			(void);										/* ���ԏ��� */
	int		IsScrollPos			(int x, int y, int nDirection);				/* �X�N���[������ꏊ������ */
	BOOL	IsScrollArea		(int x, int y, int &nDirection);			/* �X�N���[���ł��邩���� */
	BOOL	IsScroll			(void);										/* �X�N���[���������� */
	BOOL	IsInScreen			(int x, int y);								/* �w����W����ʓ��ɂ��邩�`�F�b�N */
	BYTE	Scroll				(BYTE byDirection, BOOL bNoCheck = FALSE);	/* �X�N���[�� */
	void	SetCenterPos		(int x, int y);								/* �w����W����ʒ����ɂȂ�悤�ɐݒ� */
	void	SetScrollWait		(int nMoveWait);							/* �X�N���[���ړ��҂����Ԃ�ݒ� */
	void	SetScrollMode		(BOOL bScroll, int nViewIcon = -1);			/* �X�N���[�����邩�ݒ� */
	void	SetSystemIconMode	(int nMode);								/* �V�X�e���A�C�R�����[�h��ݒ� */
	void	GetViewMapPos		(int &nDstX, int &nDstY);					/* ���_���}�b�v���W�Ŏ擾 */
	void	RenewLevel			(void);										/* ���x���x���摜���X�V */


private:
	BOOL	TimerProcScroll		(void);											/* ���ԏ���(�X�N���[��) */
	BOOL	TimerProcSystemIcon	(void);											/* ���ԏ���(�V�X�e���A�C�R��) */
	void	DrawPartsBase		(CImg32 *pDst, int nDrawY = -99);				/* �`��(�y��) */
	void	GetDrawMapPos		(POINT *ptPos, int &nDstX, int &nDstY);			/* �}�b�v���W�̕`��ʒu������ */
	void	DrawPartsPile		(CImg32 *pDst, int nDrawY = -99);				/* �`��(�d�ˍ��킹) */
	void	DrawShadow			(CImg32 *pDst, int nDrawY = -99);				/* �`��(�}�b�v�e) */
	void	DrawItem			(CImg32 *pDst, int nDrawY = -99);				/* �`��(�A�C�e��) */
	void	GetDrawPos			(CInfoCharCli *pChar, int &nDstX, int &nDstY);	/* �`��ʒu���擾 */
	void	DrawChar			(CImg32 *pDst, int nDrawY = -99);				/* �`��(�L����) */
	void	DrawCharText		(CImg32 *pDst, int nDrawY = -99);				/* �`��(�L�����֘A�̃e�L�X�g) */
	void	DrawSystemIcon		(CImg32 *pDst);									/* �`��(�V�X�e���A�C�R��) */
	void	DrawGauge			(CImg32 *pDst);									/* �`��(�Q�[�W��) */


public:
	BOOL		m_bScroll;					/* �X�N���[�����锻�� */
	BYTE		m_byDirection;				/* �X�N���[������ */
	int			m_nViewIcon,				/* ���_�A�C�R�� */
				m_nMoveWait,				/* �X�N���[���ړ��҂����� */
				m_nViewX,					/* ���_(��) */
				m_nViewY,					/* ���_(�c) */
				m_nMoveX,					/* �X�N���[����(��) */
				m_nMoveY,					/* �X�N���[����(�c) */
				m_nSystemIconMode,			/* �V�X�e���A�C�R���\�����[�h */
				m_nSyatemIconOffset;		/* �V�X�e���A�C�R���\���I�t�Z�b�g */
	DWORD		m_dwLastTimeScroll,			/* �O��̃X�N���[���������� */
				m_dwLastTimeSystemIconMode,	/* �O��̃V�X�e���A�C�R���\�����[�h�������� */
				m_dwMoveWaitOnce;			/* ��x�����̈ړ��҂����� */

	CImg32		*m_pDibLevel,					/* ���x���x���摜 */
				*m_pDibLevelTmp;
	CLibInfoItem		*m_pLibInfoItem;		/* �A�C�e����񃉃C�u���� */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* �}�b�v�p�[�c��� */
} CLayerMap, *PCLayerMap;

/* Copyright(C)URARA-works 2006 */

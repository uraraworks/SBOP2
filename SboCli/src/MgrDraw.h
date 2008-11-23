/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrDraw.h													 */
/* ���e			:�`��}�l�[�W���N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrLayer;
class CMgrWindow;
class CInfoCharCli;
class CInfoItem;
class CInfoMapParts;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItem;
class CInfoTextEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrDraw
{
public:
			CMgrDraw();									/* �R���X�g���N�^ */
	virtual ~CMgrDraw();								/* �f�X�g���N�^ */

	void	Create			(CMgrData *pMgrData);				/* �쐬 */
	void	Destroy			(void);								/* �j�� */

	void	Draw			(HDC hDC);							/* �`�� */
	void	DrawChar		(CImg32 *pDst, int x, int y, CInfoCharCli *pInfoChar, BOOL bLock = TRUE);	/* �L������`�� */
	void	DrawChar		(CImg32 *pDst, int x, int y, BYTE byDirection, BYTE byAnimeNo, BYTE byLevel, CInfoCharCli *pInfoChar);	/* �L������`�� */

	void	DrawMapParts	(CImg32 *pDst, int x, int y, DWORD dwPartsID, int nMode, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BYTE byLevel = 0);			/* �}�b�v�p�[�c��`�� */
	void	DrawMapParts	(CImg32 *pDst, int x, int y, CInfoMapParts *pInfoMapParts, int nMode, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE, BYTE byLevel = 0);		/* �}�b�v�p�[�c��`�� */
	void	DrawMapShadow	(CImg32 *pDst, int x, int y, DWORD dwShadowID, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE);	/* �}�b�v�e��`�� */
	void	DrawMapObject	(CImg32 *pDst, int x, int y, DWORD dwObjectID, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE);			/* �}�b�v�I�u�W�F�N�g��`�� */
	void	DrawItem		(CImg32 *pDst, int x, int y, DWORD dwItemID, BYTE byLevel = 0);			/* �A�C�e����`�� */
	void	DrawItem		(CImg32 *pDst, int x, int y, CInfoItem *pInfoItem, BYTE byLevel = 0, BOOL bLock = TRUE);	/* �A�C�e����`�� */
	void	DrawBalloon		(CImg32 *pDst, int x, int y, DWORD dwBalloonID);						/* ���o����`�� */
	void	DrawTextEffect	(CImg32 *pDst, int x, int y, CInfoTextEffect *pInfoTextEffect);			/* �����G�t�F�N�g��`�� */
	void	DrawMapHitMark	(CImg32 *pDst, int x, int y, int nNo);									/* �}�b�v�����蔻��}�[�N��`�� */
	void	DrawCursor		(CImg32 *pDst, int x, int y, int nType);								/* �w�J�[�\����`�� */

	BOOL	TimerProc		(void);								/* ���ԏ��� */
	BOOL	IsFade			(void);								/* �t�F�[�h�C��/�A�E�g�������� */
	void	SetLevel		(BYTE byLevel);						/* ���x�ݒ� */
	void	SetFadeState	(BYTE byFadeState);					/* �t�F�[�h��Ԑݒ� */
	void	Clear			(void);								/* �w�i�N���A */
	void	SaveScreenShot	(void);								/* �X�N���[���V���b�g�̕ۑ� */
	void	LockDibTmp		(void);								/* �`��e���|���������b�N */
	void	UnLockDibTmp	(void);								/* �`��e���|�����̃��b�N������ */


private:
	BOOL	Fade			(void);						/* �t�F�[�h���� */
	CImg32	*GetDibTmp		(void);						/* �`��e���|�����擾 */
	void	ReleaseDibTmp	(void);						/* �`��e���|������� */


private:
	BYTE				m_byLevel;						/* ���x */
	BYTE				m_byFadeState;					/* �t�F�[�h��� */
	DWORD				m_dwLastFade;					/* �O��̃t�F�[�h���� */

	CmySection			m_CritDataLock;					/* �f�[�^���b�N�p�̃N���e�B�J���Z�N�V���� */
	CImg32				*m_pDibBack,					/* �o�b�N�o�b�t�@ */
						*m_pDibTmp;						/* �`��p�e���|���� */
	CMgrData			*m_pMgrData;					/* �f�[�^�}�l�[�W�� */
	CMgrGrpData			*m_pMgrGrpData;					/* �O���t�B�b�N�f�[�^�}�l�[�W�� */
	CMgrLayer			*m_pMgrLayer;					/* ���C���[�}�l�[�W�� */
	CMgrWindow			*m_pMgrWindow;					/* �E�B���h�E�}�l�[�W�� */
	CLibInfoMapParts	*m_pLibInfoMapParts;			/* �}�b�v�p�[�c��� */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;			/* �}�b�v�e��� */
	CLibInfoItem		*m_pLibInfoItem;				/* �A�C�e����� */
	CLibInfoMotionType	*m_pLibInfoMotionType;			/* ���[�V������ʏ�񃉃C�u���� */
} CMgrDraw, *PCMgrDraw;

/* Copyright(C)URARA-works 2006 */

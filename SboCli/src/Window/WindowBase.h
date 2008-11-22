/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowBase.h												 */
/* ���e			:�E�B���h�E���N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#pragma once

class CImg32;
class CWindowBase;
class CMgrData;
class CMgrDraw;
class CMgrSound;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowBase
{
public:
			CWindowBase();										/* �R���X�g���N�^ */
	virtual ~CWindowBase();										/* �f�X�g���N�^ */

	virtual void	Create		(CMgrData *pMgrData);					/* �쐬 */
	virtual void	OnWindowMsg	(int nType, DWORD dwPara);				/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	virtual void	Update		(void);									/* �X�V */
	virtual void	Draw		(CImg32 *pDst);							/* �`�� */
	virtual BOOL	TimerProc	(void);									/* ���ԏ��� */
	virtual void	KeyProc		(BYTE byCode, BOOL bDown);				/* �L�[���� */
			BOOL	IsDelete	(void) { return m_bDelete;	}			/* �폜���� */
			BOOL	IsInput		(void) { return m_bInput;	}			/* �L�[���͂��s�������� */
			BOOL	IsActive	(void) { return m_bActive;	}			/* �A�N�e�B�u������ */
			BOOL	IsShow		(void) { return m_bShow;	}			/* �\�����邩���� */
			int		GetID		(void) { return m_nID;		}			/* �E�B���h�EID���擾 */
	virtual void	SetShow		(BOOL bShow);							/* �\�����邩�ݒ� */
	virtual void	SetActive	(BOOL bActive) { m_bActive = bActive; }	/* �A�N�e�B�u���ݒ� */
	virtual void	SetInput	(BOOL bInput) { m_bInput = bInput; }	/* �L�[���͂��s�����ݒ� */
			void	Redraw		(void);									/* �ĕ`�� */
			void	SetPos		(int nPos);								/* �ʒu�̐ݒ� */
			void	AddChild	(CWindowBase *pChild);					/* �q�E�B���h�E��ǉ� */
			void	DeleteChild	(CWindowBase *pChild);					/* �q�E�B���h�E���폜 */


protected:
	virtual BOOL OnUp		(void);										/* �L�[�n���h��(��) */
	virtual BOOL OnDown		(void);										/* �L�[�n���h��(��) */
	virtual BOOL OnLeft		(void);										/* �L�[�n���h��(��) */
	virtual BOOL OnRight	(void);										/* �L�[�n���h��(��) */
	virtual BOOL OnB		(BOOL bDown);								/* �L�[�n���h��(B) */
	virtual BOOL OnX		(BOOL bDown);								/* �L�[�n���h��(X) */
	virtual BOOL OnZ		(BOOL bDown);								/* �L�[�n���h��(Z) */
	virtual BOOL OnI		(BOOL bDown);								/* �L�[�n���h��(I) */
	virtual BOOL OnEscape	(BOOL bDown);								/* �L�[�n���h��(Escape) */
	virtual BOOL OnSpace	(BOOL bDown);								/* �L�[�n���h��(Space) */
	void TextOut2			(HDC hDC, int x, int y, LPCSTR pStr, COLORREF Color, BOOL bDraw = FALSE);	/* �������ŕ����`�� */
	void TextOut3			(HDC hDC, int x, int y, int cx, int cy, LPCSTR pStr, COLORREF Color);		/* �������ŕ����`��(�E�l) */
	void TextOut4			(HDC hDC, int x, int y, LPCSTR pStr, COLORREF Color);						/* �������ŕ����`�� */
	void DrawFrame			(int nType = 0);							/* �t���[����`�� */
	void DrawFrame			(int x, int y, int cx, int cy, int nType);	/* �t���[����`�� */
	void DrawFrame2			(int x, int y, int cx, int cy, int nType, CImg32 *pDst=NULL, int nBottom=0);	/* �t���[����`�� */
	void DrawFrame3			(int x, int y, int cx, int cy, int nType);	/* �t���[����`�� */
	void DrawCursor			(int x, int y);								/* �J�[�\����`�� */
	void DrawInputFrame1	(int x, int y, int cx, int cy, int nType);	/* ���͗��p�t���[����`��1 */


public:
	HWND			m_hWndMain;						/* ���C���E�B���h�E�n���h�� */
	DWORD			m_dwLastTimerProc,				/* �O��̎��ԏ������� */
					m_dwLastTimeCursor,				/* �O��̃J�[�\���������� */
					m_dwLastTimeKey;				/* �O��̃L�[�������� */
	BOOL			m_bShow,						/* �\������ */
					m_bDelete,						/* �폜�t���O */
					m_bInput,						/* �L�[���͂��s�� */
					m_bActive;						/* �A�N�e�B�u�ȃE�B���h�E */
	int				m_nCursorAnime,					/* �J�[�\���A�j���[�V�����R�}�ԍ� */
					m_nCountKeyRepeate;				/* �L�[���s�[�g�� */

	int				m_nID;							/* �E�B���h�EID */
	POINT			m_ptViewPos;					/* �\�����W */
	SIZE			m_sizeWindow;					/* �E�B���h�E�T�C�Y */
	int				m_nPos,							/* �ʒu */
					m_nPosMax;						/* �ʒu�̍ő�l */
	DWORD			m_dwTimeDrawStart;				/* �`�悵������ */
	HFONT			m_hFont,						/* �`��Ɏg���t�H���g */
					m_hFont12,						/* �`��Ɏg���t�H���g(12�h�b�g) */
					m_hFont14;						/* �`��Ɏg���t�H���g(14�h�b�g) */

	CWindowBase		*m_pParent;						/* �e�E�B���h�E */
	CImg32			*m_pDib,						/* �r�b�g�}�b�v */
					*m_pDibSystem;					/* �V�X�e���摜 */
	CMgrData		*m_pMgrData;					/* �f�[�^�Ǘ� */
	CMgrDraw		*m_pMgrDraw;					/* �`��Ǘ� */
	CMgrSound		*m_pMgrSound;					/* �T�E���h�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;					/* �O���t�B�b�N�f�[�^�Ǘ� */
	CmyArray<CWindowBase *, CWindowBase *>	m_apChild;	/* �q�E�B���h�E */
} CWindowBase, *PCWindowBase;
typedef CmyArray<PCWindowBase, PCWindowBase>	  ARRAYWINDOWBASE;
typedef CmyArray<PCWindowBase, PCWindowBase>	*PARRAYWINDOWBASE;

/* Copyright(C)URARA-works 2006 */

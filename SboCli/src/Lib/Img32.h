/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	Img32.h														 */
/* ���e�F		32bit�C���[�W�N���X ��`�t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/01/24													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CImg32
{
public:
			CImg32(void);										/* �R���X�g���N�^ */
	virtual ~CImg32();											/* �f�X�g���N�^ */

	BOOL	Create				(int cx, int cy);										/* �r�b�g�}�b�v���쐬 */
	BOOL	CreateWithoutGdi	(int cx, int cy, LPBYTE pBits = NULL);					/* GDI��݊��ō쐬 */
	BOOL	CreateWithoutGdi256	(int cx, int cy);										/* GDI��݊��ō쐬(256�F) */
	void	Destroy				(void);													/* �j�� */

	void	Clear				(void);													/* �N���A */
	void	FillRect			(int x, int y, int cx, int cy, COLORREF clFill);		/* ��`�h��Ԃ� */
	void	XorRect				(int x, int y, int cx, int cy);							/* ��`XOR */
	void	Rectangle			(int x, int y, int cx, int cy, COLORREF clDraw);		/* ��` */
	void	Circle				(int x, int y, int r, COLORREF clDraw);					/* �~ */
	void	CircleGradation		(int x, int y, int r, int rr, COLORREF clDraw);			/* �~(�O���f�[�V����) */
	void	ChgLevel			(int x, int y, int cx, int cy, BYTE byLevel);			/* ���x��ύX */
	void	ChgColor			(int x, int y, int cx, int cy, COLORREF clChg);			/* �F�f��ύX */
	void	SetLevel			(CImg32 *pSrc);											/* �w��摜��R�l�𖾓x�Ƃ��Ĕ��f */

	void	Blt					(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);						/* ��`�]�� */
	void	BltLevel			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevel, BOOL bColorKey = FALSE);		/* ���x��ς��Ȃ����`�]�� */
	void	BltAlpha			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevel, BOOL bColorKey = FALSE);		/* �������킹��`�]�� */
	void	BltAlpha2			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevelAlpha, BOOL bColorKey = FALSE);	/* �J���[�L�[�̂ݍ������킹��`�]�� */
	void	BltPlus				(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, int nMax, BOOL bColorKey = FALSE);			/* ��`���Z�]�� */
	void	BltTurnRightAndLeft	(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);						/* ���E���]���ċ�`�]�� */
	void	BltTurnUpAndDown(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);							/* �㉺���]���ċ�`�]�� */	

	HDC		Lock				(void);									/* �摜�����b�N */
	void	Unlock				(void);									/* ���b�N���� */

	HBITMAP	GetSafeHandle		(void);									/* �r�b�g�}�b�v�n���h�����擾 */
	int		Width				(void);									/* �����擾 */
	int		Height				(void);									/* �������擾 */
	int		GetImageSize		(void);									/* �f�[�^�T�C�Y���擾 */
	LPBYTE	GetBits				(void);									/* �r�b�g�f�[�^�̃A�h���X���擾 */
	void	SetColorKey			(COLORREF clKey);						/* �J���[�L�[��ݒ� */
	COLORREF GetColorKey		(void);									/* �J���[�L�[���擾 */

	/* 256�F�摜�p */
	BOOL	IsMode256			(void);											/* 256�F�摜������ */
	void	SetColorKeyNo		(BYTE byColorKeyNo);							/* �J���[�L�[��ݒ� */
	BYTE	GetColorKeyNo		(void);											/* �J���[�L�[���擾 */
	void	SetPallet			(BYTE byNo, BYTE r, BYTE g, BYTE b);			/* �p���b�g��ݒ� */
	PBYTE	GetPallet			(BYTE byNo);									/* �p���b�g���擾 */
	void	FillRect256			(int x, int y, int cx, int cy, BYTE byPallet);	/* ��`�h��Ԃ� */
	void	BltFrom256			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);
	void	BltAlphaFrom256		(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevel, BOOL bColorKey = FALSE);		/* �������킹��`�]�� */


protected:
	void	InitData			(void);									/* �f�[�^�������� */
	DWORD	ColorrefToDword		(COLORREF cl);							/* �F�f��DWORD�l�ɕϊ� */
	BYTE	PercentToHex		(BYTE byPercent);						/* �p�[�Z���g��255MAX��16�i���ɕϊ� */
	void	ClipPos				(int &dx, int &dy, int &cx, int &cy, CImg32 *pSrc, int &sx, int &sy);	/* ���W���N���b�s���O */


protected:
	HBITMAP			m_hBmp;								/* �r�b�g�}�b�v�n���h�� */
	HBITMAP			m_hBmpBack;							/* �r�b�g�}�b�v�n���h���ۑ��p */
	BITMAPINFO		m_bmpInfo;							/* �r�b�g�}�b�v��� */
	LPBYTE			m_pBits;							/* �r�b�g�f�[�^ */
	HDC				m_hDC;								/* �f�o�C�X�R���e�L�X�g�n���h�� */
	DWORD			m_dwColorKey;						/* �J���[�L�[ */

	BYTE			m_byColorKey;						/* �J���[�L�[�ԍ� */
	PBYTE			m_pPallet;							/* �p���b�g */
} CImg32, *PCImg32;

/* Copyright(C)URARA-works 2005 */

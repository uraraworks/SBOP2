/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrKeyInput.h												 */
/* ���e			:�L�[���͊Ǘ��N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

class CDInputUtil;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrKeyInput
{
public:
			CMgrKeyInput();							/* �R���X�g���N�^ */
	virtual ~CMgrKeyInput();						/* �f�X�g���N�^ */

	void	Create		(void);								/* �쐬 */
	void	Destroy		(void);								/* �j�� */

	void	Renew		(BYTE &byCode, BOOL &bDown);		/* ��ԍX�V */
	void	Reset		(void);								/* ������ */
	BOOL	IsInput		(BYTE byCode);						/* �������� */

	/* �W���C�p�b�h�֘A */
	void	SetDevice		(int nNo, HWND hWnd);			/* �g�p�f�o�C�X��ݒ� */
	void	SetDevice		(GUID &stSrc, HWND hWnd);		/* �g�p�f�o�C�X��ݒ� */
	void	Enum			(void);							/* �W���C�p�b�h��� */
	int		GetDeviceCount	(void);							/* �f�o�C�X�����擾 */
	BOOL	GetDeviceName	(int nNo, CmyString &strDst);	/* �f�o�C�X�����擾 */
	BOOL	GetGUID			(int nNo, GUID &stDst);			/* �w��f�o�C�X��GUID���擾 */



private:
	BYTE		m_abyCode[256];					/* ���̓`�F�b�N����R�[�h */
	BYTE		m_abyKeyState[256];				/* �L�[���͏�� */
	BYTE		m_abyKeyStateBack[256];			/* �O��̓��͏�� */

	CDInputUtil	*m_pDInputUtil;					/* DirectInput���[�e�B���e�B */
} CMgrKeyInput, *PCMgrKeyInput;

/* Copyright(C)URARA-works 2006 */

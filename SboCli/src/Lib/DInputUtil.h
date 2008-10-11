/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DInputUtil.h												 */
/* ���e			:DirectInput���[�e�B���e�B�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/30													 */
/* ========================================================================= */

#pragma once

#include <dinput.h>

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

#define BUTTON_NULL		0x00000000
#define BUTTON_UP		0x00000001
#define BUTTON_DOWN		0x00000002
#define BUTTON_LEFT		0x00000004
#define BUTTON_RIGHT	0x00000008
#define BUTTON_BUTTON1	0x00000010
#define BUTTON_BUTTON2	0x00000020


/* ========================================================================= */
/* �\���̂̒�`																 */
/* ========================================================================= */

/* �f�o�C�X��� */
typedef struct _DINPUTDEVICEINFO {
	GUID		guidInstance;
	CmyString	strName;
} DINPUTDEVICEINFO, *PDINPUTDEVICEINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDInputUtil
{
public:
			CDInputUtil();									/* �R���X�g���N�^ */
	virtual ~CDInputUtil();								/* �f�X�g���N�^ */

	BOOL Create		(void);											/* �쐬 */
	void Destroy	(void);											/* �j�� */

	BOOL	IsUseDevice		(void);									/* �f�o�C�X���g�p���Ă��邩���� */
	DWORD	GetKeyState		(void);									/* ���͏�Ԃ��擾 */
	void	SetDevice		(int nNo, HWND hWnd);					/* �g�p�f�o�C�X��ݒ� */
	void	SetDevice		(GUID &stSrc, HWND hWnd);				/* �g�p�f�o�C�X��ݒ� */
	BOOL	Enum			(void);									/* �f�o�C�X��� */
	int		GetDeviceCount	(void);									/* �f�o�C�X�����擾 */
	BOOL	GetDeviceName	(int nNo, CmyString &strDst);			/* �f�o�C�X�����擾 */
	BOOL	GetGUID			(int nNo, GUID &stDst);					/* �w��f�o�C�X��GUID���擾 */


protected:
	static BOOL CALLBACK EnumProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);	/* �񋓃R�[���o�b�N�֐� */

	void	DeleteDeviceInfo	(int nNo);							/* �f�o�C�X�����폜 */
	void	DeleteAllDeviceInfo	(void);								/* �f�o�C�X����S�č폜 */
	void	AddDeviceInfo		(PDINPUTDEVICEINFO pDeviceInfo);	/* �f�o�C�X����ǉ� */


protected:
	LPDIRECTINPUT8			m_pDInput;			/* DirectInput�C���^�[�t�F�C�X */
	LPDIRECTINPUTDEVICE8	m_pDevice;			/* �g�p�f�o�C�X */
	CmyArray <PDINPUTDEVICEINFO, PDINPUTDEVICEINFO>	m_aDeviceInfo;	/* �f�o�C�X��� */
} CDInputUtil, *PCDInputUtil;

/* Copyright(C)URARA-works 2007 */

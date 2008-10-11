/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DInputUtil.h												 */
/* ���e			:DirectInput���[�e�B���e�B�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/30													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "DInputUtil.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	{ if(p)	{ (p)->Release();(p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ if(p)	{ delete p;p=NULL; } }
#endif

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

/* ========================================================================= */
/* �֐���	:CDInputUtil::CDInputUtil										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

CDInputUtil::CDInputUtil()
{
	m_pDInput	= NULL;
	m_pDevice	= NULL;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::~CDInputUtil										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

CDInputUtil::~CDInputUtil()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

BOOL CDInputUtil::Create(void)
{
	BOOL bRet;
	HRESULT hResult;

	bRet = FALSE;

	hResult = DirectInput8Create (
					GetModuleHandle (NULL),
					DIRECTINPUT_VERSION,
					IID_IDirectInput8,
					(PVOID *)&m_pDInput,
					NULL);
	if (FAILED (hResult)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CDInputUtil::Destroy(void)
{
	DeleteAllDeviceInfo ();
	SAFE_RELEASE (m_pDInput);
	SAFE_RELEASE (m_pDevice);
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::IsUseDevice										 */
/* ���e		:�f�o�C�X���g�p���Ă��邩����									 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CDInputUtil::IsUseDevice(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_pDevice == NULL) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::GetKeyState										 */
/* ���e		:���͏�Ԃ��擾													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

DWORD CDInputUtil::GetKeyState(void)
{
	DWORD dwRet;
	HRESULT hr;
	DIJOYSTATE dijs;

	dwRet = 0;
	if (m_pDevice == NULL) {
		goto Exit;
	}

	/* �W���C�X�e�B�b�N�Ƀ|�[���������� */
	m_pDevice->Poll ();

	// �f�o�C�X��Ԃ�ǂݎ��
	hr = m_pDevice->GetDeviceState (sizeof (DIJOYSTATE), &dijs);
	if (hr != DI_OK){
		m_pDevice->Acquire ();
		goto Exit;
	}

	dwRet = 0;
	if (dijs.lY < 0) {
		dwRet |= BUTTON_UP;
	}
	if (dijs.lY > 0) {
		dwRet |= BUTTON_DOWN;
	}
	if (dijs.lX < 0) {
		dwRet |= BUTTON_LEFT;
	}
	if (dijs.lX > 0) {
		dwRet |= BUTTON_RIGHT;
	}
	if (dijs.rgbButtons[0] & 0x80) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (dijs.rgbButtons[1] & 0x80) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (dijs.rgbButtons[2] & 0x80) {
		dwRet |= BUTTON_BUTTON2;
	}
	if (dijs.rgbButtons[3] & 0x80) {
		dwRet |= BUTTON_BUTTON2;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::SetDevice											 */
/* ���e		:�W���C�p�b�h���												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDInputUtil::SetDevice(int nNo, HWND hWnd)
{
	PDINPUTDEVICEINFO pDeviceInfo;

	if (nNo < 0) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	pDeviceInfo = m_aDeviceInfo.GetAt (nNo);

	SetDevice (pDeviceInfo->guidInstance, hWnd);
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::SetDevice											 */
/* ���e		:�W���C�p�b�h���												 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

void CDInputUtil::SetDevice(GUID &stSrc, HWND hWnd)
{
	HRESULT hr;

	SAFE_RELEASE (m_pDevice);
	hr = m_pDInput->CreateDevice (
			stSrc,
			&m_pDevice, NULL);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	/* �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ� */
	hr = m_pDevice->SetDataFormat (&c_dfDIJoystick);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	/* ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j */
	hr = m_pDevice->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	/* ���̒l�͈̔͂�ݒ� */
	DIPROPRANGE diprg;
	ZeroMemory (&diprg, sizeof(diprg));
	diprg.diph.dwSize		= sizeof(diprg); 
	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
	diprg.diph.dwHow		= DIPH_BYOFFSET; 
	diprg.lMin				= -1000;
	diprg.lMax				= 1000;

	/* X���͈̔͂�ݒ� */
	diprg.diph.dwObj	= DIJOFS_X; 
	hr = m_pDevice->SetProperty (DIPROP_RANGE, &diprg.diph);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	/* Y���͈̔͂�ݒ� */
	diprg.diph.dwObj	= DIJOFS_Y;
	hr = m_pDevice->SetProperty (DIPROP_RANGE, &diprg.diph);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	/* �e�����ƂɁA�����̃]�[���l��ݒ肷��B */
	DIPROPDWORD				dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
	dipdw.diph.dwHow		= DIPH_BYOFFSET;
	dipdw.dwData			= 2500;

	/* X���̖����]�[����ݒ� */
	dipdw.diph.dwObj = DIJOFS_X;
	hr = m_pDevice->SetProperty (DIPROP_DEADZONE, &dipdw.diph);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	/* Y���̖����]�[����ݒ� */
	dipdw.diph.dwObj = DIJOFS_Y;
	hr = m_pDevice->SetProperty (DIPROP_DEADZONE, &dipdw.diph);
	if (FAILED (hr)) {
		SAFE_RELEASE (m_pDevice);
		return;
	}

	m_pDevice->Acquire ();
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::Enum												 */
/* ���e		:�f�o�C�X���													 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

BOOL CDInputUtil::Enum(void)
{
	BOOL bRet;
	HRESULT hResult;

	bRet = FALSE;
	if (m_pDInput == NULL) {
		goto Exit;
	}

	DeleteAllDeviceInfo ();
	hResult = m_pDInput->EnumDevices (
					DI8DEVCLASS_GAMECTRL,
					EnumProc,
					this,
					DIEDFL_ATTACHEDONLY);
	if (FAILED (hResult)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::GetDeviceCount									 */
/* ���e		:�f�o�C�X�����擾												 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

int CDInputUtil::GetDeviceCount(void)
{
	return m_aDeviceInfo.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::GetDeviceName										 */
/* ���e		:�f�o�C�X�����擾												 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

BOOL CDInputUtil::GetDeviceName(
	int nNo,				/* [in] �z��ԍ� */
	CmyString &strDst)		/* [ou] �f�o�C�X�� */
{
	BOOL bRet;
	PDINPUTDEVICEINFO pInfo;

	bRet = FALSE;
	if (nNo >= m_aDeviceInfo.GetSize ()) {
		goto Exit;
	}
	if (nNo < 0) {
		goto Exit;
	}

	pInfo	= m_aDeviceInfo[nNo];
	strDst	= pInfo->strName;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::GetGUID											 */
/* ���e		:�w��f�o�C�X��GUID���擾										 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

BOOL CDInputUtil::GetGUID(
	int nNo,			/* [in] �z��ԍ� */
	GUID &stDst)		/* [out] �擾�� */
{
	BOOL bRet;
	PDINPUTDEVICEINFO pInfo;

	bRet = FALSE;
	if (nNo >= m_aDeviceInfo.GetSize ()) {
		goto Exit;
	}
	if (nNo < 0) {
		goto Exit;
	}

	pInfo = m_aDeviceInfo[nNo];
	stDst = pInfo->guidInstance;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::EnumProc											 */
/* ���e		:�񋓃R�[���o�b�N�֐�											 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

BOOL CALLBACK CDInputUtil::EnumProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	PDINPUTDEVICEINFO pInfo;
	PCDInputUtil pThis;

	pThis = (PCDInputUtil)pvRef;

	/* �f�o�C�X����ǉ� */
	pInfo = new DINPUTDEVICEINFO;
	pInfo->guidInstance	= lpddi->guidInstance;
	pInfo->strName		= lpddi->tszInstanceName;
	pThis->AddDeviceInfo (pInfo);

	return DIENUM_CONTINUE;
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::DeleteDeviceInfo									 */
/* ���e		:�f�o�C�X�����폜												 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CDInputUtil::DeleteDeviceInfo(int nNo)
{
	PDINPUTDEVICEINFO pInfo;

	pInfo = m_aDeviceInfo[nNo];
	SAFE_DELETE (pInfo);
	m_aDeviceInfo.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::DeleteAllDeviceInfo								 */
/* ���e		:�f�o�C�X����S�č폜											 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CDInputUtil::DeleteAllDeviceInfo(void)
{
	int i, nCount;

	nCount = m_aDeviceInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteDeviceInfo (i);
	}
}


/* ========================================================================= */
/* �֐���	:CDInputUtil::AddDeviceInfo										 */
/* ���e		:�f�o�C�X����ǉ�												 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CDInputUtil::AddDeviceInfo(PDINPUTDEVICEINFO pDeviceInfo)
{
	m_aDeviceInfo.Add (pDeviceInfo);
}

/* Copyright(C)URARA-works 2007 */

/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrKeyInput.cpp											 */
/* ���e			:�L�[���͊Ǘ��N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "DInputUtil.h"
#include "MgrKeyInput.h"


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::CMgrKeyInput										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CMgrKeyInput::CMgrKeyInput()
{
	ZeroMemory (m_abyCode,			sizeof (m_abyCode));
	ZeroMemory (m_abyKeyState,		sizeof (m_abyKeyState));
	ZeroMemory (m_abyKeyStateBack,	sizeof (m_abyKeyStateBack));

	m_pDInputUtil = new CDInputUtil;
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::~CMgrKeyInput									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CMgrKeyInput::~CMgrKeyInput()
{
	Destroy ();
	SAFE_DELETE (m_pDInputUtil);
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Create(void)
{
	int i, aKeyTbl[] = {
		'Z',			/* Z */
		'X',			/* X */
		'S',			/* S */
		'A',			/* A */
		'R',			/* R */
		'I',			/* I */
		'B',			/* B */
		'F',			/* F */
		'G',			/* G */
		'H',			/* H */
		'L',			/* L */
		'C',			/* C */
		'P',			/* P */
		'M',			/* M */
		'N',			/* N */
		VK_OEM_3,		/* @ */
		VK_RETURN,		/* Enter */
		VK_ESCAPE,		/* Escape */
		VK_TAB,			/* Tab */
		VK_UP,			/* �� */
		VK_DOWN,		/* �� */
		VK_LEFT,		/* �� */
		VK_RIGHT,		/* �� */
		VK_SHIFT,		/* Shift */
		VK_CONTROL,		/* Ctrl */
		VK_SPACE,		/* Space */
		-1
	};
	ZeroMemory (m_abyCode, sizeof (m_abyCode));

	for (i = 0; ; i ++) {
		if (aKeyTbl[i] == -1) {
			break;
		}
		m_abyCode[i] = aKeyTbl[i];
	}

	m_pDInputUtil->Create ();
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Destroy(void)
{
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::Renew											 */
/* ���e		:��ԍX�V														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Renew(BYTE &byCode, BOOL &bDown)
{
	int i;
	BOOL bBreak;
	BYTE byCodeTmp;
	DWORD dwKeyInput;

//	GetKeyboardState (m_abyKeyState);

	byCode	= 0;
	bDown	= FALSE;

	/* ���͏�Ԃ̌��o */
	dwKeyInput = m_pDInputUtil->GetKeyState ();
	for (i = 0; ; i ++) {
		byCodeTmp = m_abyCode[i];
		if (byCodeTmp == 0) {
			break;
		}
		bBreak = TRUE;
		m_abyKeyState[byCodeTmp] = (BYTE)GetKeyState (byCodeTmp);
		switch (byCodeTmp) {
		case VK_UP:			/* �� */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_UP) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_DOWN:		/* �� */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_DOWN) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_LEFT:		/* �� */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_LEFT) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_RIGHT:		/* �� */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_RIGHT) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case 'X':			/* X */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_BUTTON1) ? 0x80 : 0;
			break;
		case 'Z':			/* Z */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_BUTTON2) ? 0x80 : 0;
			break;
		}
		if ((m_abyKeyState[byCodeTmp] & 0x80) && (m_abyKeyStateBack[byCodeTmp] == 0)) {
			m_abyKeyStateBack[byCodeTmp] = 1;
			byCode	= byCodeTmp;
			bDown	= TRUE;
			if (bBreak) {
				break;
			}

		} else if (((m_abyKeyState[byCodeTmp] & 0x80) == 0) && (m_abyKeyStateBack[byCodeTmp] == 1)) {
			m_abyKeyStateBack[byCodeTmp] = 0;
			byCode	= byCodeTmp;
			bDown	= FALSE;
			if (bBreak) {
				break;
			}
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::Reset											 */
/* ���e		:������															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Reset(void)
{
	ZeroMemory (m_abyKeyState,		sizeof (m_abyKeyState));
	ZeroMemory (m_abyKeyStateBack,	sizeof (m_abyKeyStateBack));
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::IsInput											 */
/* ���e		:��������														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CMgrKeyInput::IsInput(BYTE byCode)
{
	BOOL bRet;

	bRet = FALSE;

	if ((m_abyKeyState[byCode] & 0x80) == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::SetDevice										 */
/* ���e		:�g�p�f�o�C�X��ݒ�												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CMgrKeyInput::SetDevice(int nNo, HWND hWnd)
{
	m_pDInputUtil->SetDevice (nNo, hWnd);
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::SetDevice										 */
/* ���e		:�g�p�f�o�C�X��ݒ�												 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

void CMgrKeyInput::SetDevice(GUID &stSrc, HWND hWnd)
{
	m_pDInputUtil->SetDevice (stSrc, hWnd);
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::Enum												 */
/* ���e		:�W���C�p�b�h���												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CMgrKeyInput::Enum(void)
{
	m_pDInputUtil->Enum ();
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::GetDeviceCount									 */
/* ���e		:�f�o�C�X�����擾												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

int CMgrKeyInput::GetDeviceCount(void)
{
	return m_pDInputUtil->GetDeviceCount ();
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::GetDeviceName									 */
/* ���e		:�f�o�C�X�����擾												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CMgrKeyInput::GetDeviceName(int nNo, CmyString &strDst)
{
	return m_pDInputUtil->GetDeviceName (nNo, strDst);
}


/* ========================================================================= */
/* �֐���	:CMgrKeyInput::GetGUID											 */
/* ���e		:�w��f�o�C�X��GUID���擾										 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

BOOL CMgrKeyInput::GetGUID(
	int nNo,			/* [in] �z��ԍ� */
	GUID &stDst)		/* [out] �擾�� */
{
	return m_pDInputUtil->GetGUID (nNo, stDst);
}

/* Copyright(C)URARA-works 2006 */

/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:stdafx.h													 */
/* ���e			:�v���R���p�C���w�b�_ ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B
#define WINVER 0x0500
#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

//#pragma warning(disable:4192)
//#pragma warning(disable:4146)
//#pragma warning(disable:4049)

#define DIRECTINPUT_VERSION	DIRECTINPUT_HEADER_VERSION

#include <winsock2.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <imm.h>
#include <time.h>
#include <tchar.h>
#include <shlwapi.h>
#include <map>
using namespace std;

#include "GlobalDefine.h"
#include "myArray.h"
#include "myString.h"
#include "mySection.h"
#include "SBOGlobal.h"
#include "SboCli_priv.h"

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */

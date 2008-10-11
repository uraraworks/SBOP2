/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibSboSoundLoader.cpp										 */
/* ���e			:SBO�T�E���h�f�[�^���C�u�����ǂݍ��݃N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibSboSoundLoader.h"

/* ========================================================================= */
/* �֐���	:CLibSboSoundLoader::CLibSboSoundLoader							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

CLibSboSoundLoader::CLibSboSoundLoader()
{
	m_hLib = NULL;
	m_pGetSoundCount		= NULL;
	m_pGetSoundResourceID	= NULL;
	m_pGetSoundID			= NULL;
	m_pGetSoundNo			= NULL;
	m_pGetSoundName			= NULL;
	Load ();
}


/* ========================================================================= */
/* �֐����F	CLibSboSoundLoader::~CLibSboSoundLoader							 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

CLibSboSoundLoader::~CLibSboSoundLoader()
{
	Free ();
}


/* ========================================================================= */
/* �֐����F	CLibSboSoundLoader::Load										 */
/* ���e�F	�ǂݍ���														 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

void CLibSboSoundLoader::Load(void)
{
	if (m_hLib) {
		return;
	}
	m_hLib = LoadLibrary ("SboSoundData.dll");
	if (m_hLib == NULL) {
		return;
	}

	/* �e�֐��̃A�h���X���擾 */
	m_pGetSoundCount		= (LIBSBOSOUNDGetSoundCount)		GetProcAddress (m_hLib, "GetSoundCount");
	m_pGetSoundResourceID	= (LIBSBOSOUNDGetSoundResourceID)	GetProcAddress (m_hLib, "GetSoundResourceID");
	m_pGetSoundID			= (LIBSBOSOUNDGetSoundID)			GetProcAddress (m_hLib, "GetSoundID");
	m_pGetSoundNo			= (LIBSBOSOUNDGetSoundNo)			GetProcAddress (m_hLib, "GetSoundNo");
	m_pGetSoundName			= (LIBSBOSOUNDGetSoundName)			GetProcAddress (m_hLib, "GetSoundName");

	if ((m_pGetSoundCount		== NULL) ||
		(m_pGetSoundResourceID	== NULL) ||
		(m_pGetSoundID			== NULL) ||
		(m_pGetSoundNo			== NULL) ||
		(m_pGetSoundName		== NULL)) {
		Free ();
	}
}


/* ========================================================================= */
/* �֐����F	CLibSboSoundLoader::Free										 */
/* ���e�F	�J��															 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

void CLibSboSoundLoader::Free(void)
{
	if (m_hLib) {
		FreeLibrary (m_hLib);
		m_pGetSoundCount		= NULL;
		m_pGetSoundResourceID	= NULL;
		m_pGetSoundID			= NULL;
		m_pGetSoundNo			= NULL;
		m_pGetSoundName			= NULL;
	}
}


/* ========================================================================= */
/* �֐���	:CLibSboSoundLoader::GetSoundCount								 */
/* ���e		:���ʉ������擾													 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

int CLibSboSoundLoader::GetSoundCount(void)
{
	if (m_pGetSoundCount == NULL) {
		return 0;
	}
	return m_pGetSoundCount ();
}


/* ========================================================================= */
/* �֐���	:CLibSboSoundLoader::GetSoundResourceID							 */
/* ���e		:���ʉ��̃��\�[�XID���擾										 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

int CLibSboSoundLoader::GetSoundResourceID(int nNo)
{
	if (m_pGetSoundResourceID == NULL) {
		return 0;
	}
	return m_pGetSoundResourceID (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibSboSoundLoader::GetSoundID									 */
/* ���e		:���ʉ�ID���擾													 */
/* ���t		:2007/09/19														 */
/* ========================================================================= */

DWORD CLibSboSoundLoader::GetSoundID(int nNo)
{
	if (m_pGetSoundID == NULL) {
		return 0;
	}
	return m_pGetSoundID (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibSboSoundLoader::GetSoundNo									 */
/* ���e		:���ʉ��̃C���f�b�N�X���擾										 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

int CLibSboSoundLoader::GetSoundNo(DWORD dwSoundID)
{
	if (m_pGetSoundNo == NULL) {
		return -1;
	}
	return m_pGetSoundNo (dwSoundID);
}


/* ========================================================================= */
/* �֐���	:CLibSboSoundLoader::GetSoundName								 */
/* ���e		:���ʉ������擾													 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

LPCSTR CLibSboSoundLoader::GetSoundName(DWORD dwSoundID)
{
	if (m_pGetSoundName == NULL) {
		return "";
	}
	return m_pGetSoundName (dwSoundID);
}

/* Copyright(C)URARA-works 2007 */

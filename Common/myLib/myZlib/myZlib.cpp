/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	myZlib.cpp													 */
/* ���e�F		zlib�������ׂ̃N���X �����t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/02/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "myZlib.h"

#if 0	/* �ȉ���C�ӂ̃t�@�C���ɃR�s�[���Ďg�p����ƕ֗� */
/* lib�t�@�C���̃p�X */
#define LIBCMNPATH	""

/* �R���p�C���I�v�V�����ʂ̃����N�ݒ� */
#ifdef _DEBUG
	#pragma comment(lib, LIBCMNPATH "zlibD.lib")
#else
	#pragma comment(lib, LIBCMNPATH "zlib.lib")
#endif
#endif

/* ========================================================================= */
/* �֐����F	CmyZlib::CmyZlib												 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2005/02/02														 */
/* ========================================================================= */

CmyZlib::CmyZlib()
{
	m_pStream = NULL;
}


/* ========================================================================= */
/* �֐����F	CmyZlib::~CmyZlib												 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2005/02/02														 */
/* ========================================================================= */

CmyZlib::~CmyZlib()
{
	if (m_pStream) {
		delete m_pStream;
		m_pStream = NULL;
	}
}

/* ========================================================================= */
/* �֐����F	CmyZlib::DeflateInit											 */
/* ���e�F	���k:������														 */
/* ���t�F	2005/02/02														 */
/* ========================================================================= */

BOOL CmyZlib::DeflateInit(
	int nLevel)		/* [in] ���k���x�� */
{
	BOOL bRet;
	int nResult;

	bRet = FALSE;

	if (m_pStream) {
		goto Exit;
	}

	/* ���\���̂��m�ۂ��ď����� */
	m_pStream = new z_stream;
	m_pStream->zalloc	= Z_NULL;
	m_pStream->zfree	= Z_NULL;
	m_pStream->opaque	= Z_NULL;

	/* zlib�̏������֐����Ă� */
	nResult = deflateInit (m_pStream, nLevel);
	if (nResult != Z_OK) {
		delete m_pStream;
		m_pStream = NULL;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CmyZlib::Deflate												 */
/* ���e�F	���k:���k����													 */
/* ���t�F	2005/02/02														 */
/* �߂�l�F	���k��̃T�C�Y													 */
/* ========================================================================= */

DWORD CmyZlib::Deflate(
	LPBYTE pSrc,			/* [in] ���k�� */
	DWORD dwSrcSize,		/* [in] ���k���T�C�Y */
	LPBYTE pDst,			/* [in] ���k�� */
	DWORD dwDstSize)		/* [in] ���k��T�C�Y */
{
	DWORD dwRet;
	int nResult;

	dwRet = 0;
	m_pStream->next_in		= pSrc;
	m_pStream->avail_in		= dwSrcSize;
	m_pStream->next_out		= pDst;
	m_pStream->avail_out	= dwDstSize;
	deflateReset(m_pStream);

	/* ���k */
	nResult = deflate (m_pStream, Z_FINISH);
	if (nResult == Z_STREAM_END) {
		/* ���k��̃T�C�Y���擾 */
		dwRet = m_pStream->total_out;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐����F	CmyZlib::DeflateEnd												 */
/* ���e�F	���k:�㏈��														 */
/* ���t�F	2005/02/02														 */
/* ========================================================================= */

void CmyZlib::DeflateEnd(void)
{
	if (m_pStream == NULL) {
		return;
	}

	/* �㏈���֐����Ă� */
	deflateEnd (m_pStream);

	delete m_pStream;
	m_pStream = NULL;
}


/* ========================================================================= */
/* �֐����F	CmyZlib::InflateInit											 */
/* ���e�F	��:������														 */
/* ���t�F	2005/02/02														 */
/* ========================================================================= */

BOOL CmyZlib::InflateInit(void)
{
	BOOL bRet;
	int nResult;

	bRet = FALSE;

	if (m_pStream) {
		goto Exit;
	}

	/* ���\���̂��m�ۂ��ď����� */
	m_pStream = new z_stream;
	m_pStream->zalloc	= Z_NULL;
	m_pStream->zfree	= Z_NULL;
	m_pStream->opaque	= Z_NULL;

	/* zlib�̏������֐����Ă� */
	nResult = inflateInit (m_pStream);
	if (nResult != Z_OK) {
		delete m_pStream;
		m_pStream = NULL;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CmyZlib::Inflate												 */
/* ���e�F	��:���k����													 */
/* ���t�F	2005/02/02														 */
/* �߂�l�F	�𓀌�̃T�C�Y													 */
/* ========================================================================= */

DWORD CmyZlib::Inflate(
	LPBYTE pSrc,			/* [in] �𓀌� */
	DWORD dwSrcSize,		/* [in] �𓀌��T�C�Y */
	LPBYTE pDst,			/* [in] �𓀐� */
	DWORD dwDstSize)		/* [in] �𓀐�T�C�Y */
{
	DWORD dwRet;
	int nResult;

	dwRet = 0;
	m_pStream->next_in		= pSrc;
	m_pStream->avail_in		= dwSrcSize;
	m_pStream->next_out		= pDst;
	m_pStream->avail_out	= dwDstSize;
	inflateReset (m_pStream);

	/* �� */
	nResult = inflate (m_pStream, Z_FINISH);
	if (nResult == Z_STREAM_END) {
		/* �𓀌�̃T�C�Y���擾 */
		dwRet = m_pStream->total_out;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐����F	CmyZlib::InflateEnd												 */
/* ���e�F	��:�㏈��														 */
/* ���t�F	2005/02/02														 */
/* ========================================================================= */

void CmyZlib::InflateEnd(void)
{
	if (m_pStream == NULL) {
		return;
	}

	/* �㏈���֐����Ă� */
	inflateEnd (m_pStream);

	delete m_pStream;
	m_pStream = NULL;
}
/* Copyright(C)URARA-works 2005 */

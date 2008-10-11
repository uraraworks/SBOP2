/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTextEffect.cpp											 */
/* ���e			:�����G�t�F�N�g�N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/13													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTextEffect.h"

#define ANIMECOUNT	(30)		/* �A�j���[�V�����R�}�� */

/* ========================================================================= */
/* �֐���	:CInfoTextEffect::CInfoTextEffect								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

CInfoTextEffect::CInfoTextEffect()
{
	m_nType			= TEXTEFFECTTYPE_NORMAL;	/* �\����� */
	m_nColor		= 0;						/* �F */
	m_nSize			= TEXTEFFECTNUMSIZE_SMALL;	/* �T�C�Y */
	m_bAnimeEnd		= FALSE;					/* �A�j���[�V�����I���H */
	m_byAnimeNo		= 0;						/* �A�j���[�V�����R�}�ԍ� */
	m_byAnimeCount	= 0;						/* �A�j���[�V�����R�}�� */
	m_dwLastAnime	= 0;						/* �Ō�ɃA�j���[�V������������ */
	m_dwData		= -1;						/* �f�[�^ */
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::~CInfoTextEffect								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

CInfoTextEffect::~CInfoTextEffect()
{
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::TimerProc										 */
/* ���e		:���ԏ���														 */
/* �߂�l	:TRUE:�������� FALSE:�������Ă��Ȃ�								 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

BOOL CInfoTextEffect::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp, dwWait;

	bRet = FALSE;
	if (m_bAnimeEnd) {
		goto Exit;
	}
	dwWait = 50;
	dwTmp  = dwTime - m_dwLastAnime;
	if (m_byAnimeNo >= 5) {
		dwWait  = 100;
	}
	if (dwTmp < dwWait) {
		goto Exit;
	}
	m_dwLastAnime = dwTime;
	m_byAnimeNo ++;
	if (m_byAnimeNo == 5) {
		if (m_nSize >= TEXTEFFECTNUMSIZE_MEDIUM) {
			m_nSize --;
		}
	}

	/* �I���H */
	if (m_byAnimeNo >= m_byAnimeCount) {
		m_bAnimeEnd = TRUE;
	}

	bRet = TRUE;
Exit:
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::Set											 */
/* ���e		:�ݒ�(���l�f�[�^)												 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CInfoTextEffect::Set(DWORD dwData, int nType, int nColor, int nSize)
{
	if (nColor == 5) {
		m_strData = "0123";
	} else {
		m_strData.Format ("%d", dwData);
	}
	m_nType			= nType;
	m_nColor		= nColor;
	m_nSize			= nSize;
	m_bAnimeEnd		= FALSE;
	m_byAnimeNo		= 0;
//	m_byAnimeCount	= ANIMECOUNT + ((m_strData.GetLength () - 1) * 2) + 20;
	m_byAnimeCount	= ANIMECOUNT;
	m_dwLastAnime	= timeGetTime ();
	m_dwData		= dwData;
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::Set											 */
/* ���e		:�ݒ�(�����f�[�^)												 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CInfoTextEffect::Set(LPCSTR pszData, int nType, int nColor, int nSize)
{
	m_strData		= pszData;
	m_nType			= nType;
	m_nColor		= nColor;
	m_nSize			= nSize;
	m_bAnimeEnd		= FALSE;
	m_byAnimeNo		= 0;
	m_byAnimeCount	= ANIMECOUNT + 10;
	m_dwLastAnime	= timeGetTime ();
	m_dwData		= -1;
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::GetGrpCount									 */
/* ���e		:�摜�����擾													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

int CInfoTextEffect::GetGrpCount(void)
{
	int nRet;

	nRet = 0;
	if (m_bAnimeEnd) {
		goto Exit;
	}
	if (m_dwData == -1) {
		nRet = 1;
	} else {
		nRet = m_strData.GetLength ();
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::GetGrpNo										 */
/* ���e		:�w��ԍ��̐��l���擾											 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

int CInfoTextEffect::GetGrpNo(int nNo)
{
	char szTmp[2];

	ZeroMemory (szTmp, sizeof (szTmp));
	szTmp[0] = ((LPCSTR)m_strData)[nNo];

	return atoi (szTmp);
}


/* ========================================================================= */
/* �֐���	:CInfoTextEffect::GetDrawPos									 */
/* ���e		:�w��ԍ��̕\���ʒu�␳�l���擾									 */
/* �߂�l	:TRUE:�`�悷�� FALSE:�`�悵�Ȃ�									 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

BOOL CInfoTextEffect::GetDrawPos(int nNo, POINT &ptDst, BYTE &byLevel)
{
	BOOL bRet;
	int nTmp, anPos[ANIMECOUNT] = {0, -2, -3, -4, -5, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16, -17, -18, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28};

	bRet	= FALSE;
	byLevel	= 0;
	ptDst.x = ptDst.y = 0;

	nTmp = m_byAnimeNo;
	nTmp = min (nTmp, ANIMECOUNT - 1);
	ptDst.y = anPos[nTmp];
	if (nTmp >= 9) {
		byLevel = (nTmp - 9) * 5;
	}
	switch (m_nSize) {
	case TEXTEFFECTNUMSIZE_MEDIUM:
		ptDst.y -= 5;
		break;
	case TEXTEFFECTNUMSIZE_LARGE:
		ptDst.y -= 12;
		break;
	}

	bRet = TRUE;
	return bRet;
}

/* Copyright(C)URARA-works 2008 */

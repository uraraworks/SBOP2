/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:TextInput.h												 */
/* ���e			:�e�L�X�g���̓N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/05/16													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <stdio.h>
#include "TextInput.h"


/* ========================================================================= */
/* �֐���	:CTextInput::CTextInput											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

CTextInput::CTextInput()
{
	m_dwLine		= 0;
	m_pszFileName	= NULL;
	m_hFile			= INVALID_HANDLE_VALUE;
}


/* ========================================================================= */
/* �֐���	:CTextInput::~CTextInput										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

CTextInput::~CTextInput()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CTextInput::Create												 */
/* ���e		:������															 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

BOOL CTextInput::Create(
	LPCSTR pszFileName)		/* [in] �ǂݍ��ރt�@�C���� */
{
	BOOL bRet, bResult;

	bRet = FALSE;

	if (m_pszFileName) {
		goto Exit;
	}

	bResult = Open (pszFileName);
	if (bResult == FALSE) {
		goto Exit;
	}

	m_dwLine = 0;
	m_pszFileName = new char[strlen (pszFileName) + 1];
	strcpy (m_pszFileName, pszFileName);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CTextInput::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

void CTextInput::Destroy(void)
{
	if (m_pszFileName) {
		Close ();
		delete [] m_pszFileName;
		m_pszFileName = NULL;
	}
}


/* ========================================================================= */
/* �֐���	:CTextInput::SetLine											 */
/* ���e		:�ǂݍ��ލs��ݒ�												 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

void CTextInput::SetLine(DWORD dwLine)
{
	DWORD i;
	CmyString strTmp;

	m_dwLine = 0;
	SetFilePointer (m_hFile, 0, NULL, FILE_BEGIN);

	for (i = 0; i < dwLine; i ++) {
		strTmp = ReadLine ();
		if (strTmp.IsEmpty ()) {
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CTextInput::ReadLine											 */
/* ���e		:�w��s����1�s�ǂݍ���											 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

CmyString CTextInput::ReadLine(DWORD dwLine)
{
	SetLine (dwLine);
	return ReadProc ();
}


/* ========================================================================= */
/* �֐���	:CTextInput::ReadLine											 */
/* ���e		:���݈ʒu����1�s�ǂݍ���										 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

CmyString CTextInput::ReadLine(void)
{
	return ReadProc ();
}


/* ========================================================================= */
/* �֐���	:CTextInput::GetLineCount										 */
/* ���e		:�s�����擾														 */
/* ���t		:2006/05/16														 */
/* ���l		:���̊֐����ĂԂƌ��݈ʒu�͐擪�ɖ߂��Ă��܂��B					 */
/* ========================================================================= */

DWORD CTextInput::GetLineCount(void)
{
	DWORD dwRet;

	SetLine (-1);
	dwRet = m_dwLine;
	SetLine (0);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CTextInput::Open												 */
/* ���e		:�t�@�C�����J��													 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

BOOL CTextInput::Open(LPCSTR pszFileName)
{
	BOOL bRet;
	HANDLE hFile;

	bRet = FALSE;

	/* �t�@�C�����J�� */
	hFile = CreateFile (
			pszFileName,
			GENERIC_READ,
			0,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}

	m_hFile	= hFile;
	bRet	= TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CTextInput::Close												 */
/* ���e		:�t�@�C�������												 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

void CTextInput::Close(void)
{
	if (m_hFile == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle (m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}


/* ========================================================================= */
/* �֐���	:CTextInput::ReadProc											 */
/* ���e		:�ǂݍ��ݏ���													 */
/* ���t		:2006/05/16														 */
/* ���l		:CSV�I��1�s���ǂݍ���											 */
/* ========================================================================= */

CmyString CTextInput::ReadProc(void)
{
	char szTmp[2];
	BOOL bResult;
	BYTE byTmp1, byTmp2;
	DWORD dwSize;
	CmyString strTmp, strRet;

	szTmp[1] = 0;

	/* CSV�`���Ƃ���1�s�ǂݍ��ނ܂ł̃��[�v */
	while (1) {
		strTmp.Empty ();
		/* ���s�R�[�h���I�[�܂ł̃��[�v */
		while (1) {
			bResult = ReadFile (m_hFile, &byTmp1, 1, &dwSize, NULL);
			if ((bResult == FALSE) || (dwSize == 0)) {
				break;
			}
			if (byTmp1 == 0x0D) {
				bResult = ReadFile (m_hFile, &byTmp2, 1, &dwSize, NULL);
				if (bResult == FALSE) {
					break;
				}
				if (byTmp2 == 0x0A) {
					break;
				}
				/* �u\r\n�v���Z�b�g�Œǉ� */
				szTmp[0] = byTmp1;
				strTmp += szTmp;
				szTmp[0] = byTmp2;
				strTmp += szTmp;

			} else {
				szTmp[0] = byTmp1;
				strTmp += szTmp;
			}
		}
		strRet += strTmp;

		bResult = IsCSVCheck (strRet);
		/* CSV�`����1�s�Ƃ��Đ������H */
		if (bResult) {
			if (strRet.IsEmpty () == FALSE) {
				m_dwLine ++;
			}
			break;
		}
		/* �ǂݍ��߂Ȃ������H */
		if (strTmp.IsEmpty ()) {
			strRet.Empty ();
			break;
		}
	}

	return strRet;
}


/* ========================================================================= */
/* �֐���	:CTextInput::IsCSVCheck											 */
/* ���e		:CSV�`��������Ƃ��Đ�����������								 */
/* ���t		:2006/05/16														 */
/* ========================================================================= */

BOOL CTextInput::IsCSVCheck(LPCSTR pszText)
{
	int i, nLen, nCountSection;
	BOOL bRet, bSection, bEnclose;
	LPCSTR pszTextTmp;

	bRet = FALSE;

	nLen			= strlen (pszText);	/* ������ */
	nCountSection	= 0;				/* �Z�N�V�������̈ʒu */
	bSection		= TRUE;				/* �Z�N�V�������ɂ��邩���� */
	bEnclose		= FALSE;			/* �u"�v�ň͂܂ꒆ������ */
	pszTextTmp		= pszText;			/* ��ƒ��̕�����|�C���^ */

	for (i = 0; i < nLen; i ++) {
		pszTextTmp = &pszText[i];
		/* �S�p�����H */
		if (IsDBCSLeadByte (*pszTextTmp)) {
			i ++;
			nCountSection ++;
			continue;
		}

		switch (*pszTextTmp) {
		case ',':		/* ��؂蕶���H */
			/* �u"�v�̒��ɂ���H */
			if (bEnclose) {
				break;
			}
			/* �E�o */
			bEnclose		= FALSE;
			nCountSection	= 0;
			continue;

		case '"':		/* ������͂� */
			/* �u"�v�̒��ɂ���H */
			if (bEnclose) {
				/* ���̕������u"�v�H */
				if ((i != nLen - 1) && (*(pszTextTmp + 1) == '"')) {
					i ++;
					nCountSection ++;
					continue;
				}
				/* �E�o */
				bEnclose = FALSE;
				continue;

			} else {
				/* �擪�H */
				if (nCountSection == 0) {
					/* �˓� */
					bEnclose = TRUE;
					continue;
				}
			}
			break;

		case '\r':
			/* ���̕������u\n�v�H */
			if ((i != nLen - 1) && (*(pszTextTmp + 1) == '\n')) {
				/* �u"�v�̒��ɂ͂��Ȃ��H */
				if (bEnclose == FALSE) {
					/* �I�� */
					i = nLen;
					bSection = FALSE;
					continue;
				}
				i ++;
				nCountSection ++;
				continue;
			}
			break;
		}
		bSection = TRUE;
		nCountSection ++;
	}

	/* �u"�v�̒��ɂ͂��Ȃ��H */
	if (bEnclose == FALSE) {
		bRet = TRUE;
	}

	return bRet;
}

/* Copyright(C)URARA-works 2006 */

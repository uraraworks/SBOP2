/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:ParamUtil.cpp												 */
/* ���e			:��؂蕶���t���̃p�����[�^�����N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "ParamUtil.h"


/* ========================================================================= */
/* �֐���	:CParamUtil::CParamUtil											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

CParamUtil::CParamUtil()
{
}


/* ========================================================================= */
/* �֐���	:CParamUtil::~CParamUtil										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

CParamUtil::~CParamUtil()
{
}


/* ========================================================================= */
/* �֐���	:CParamUtil::Set												 */
/* ���e		:�p�����[�^�ݒ�													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

void CParamUtil::Set(LPCSTR pszText, char cSeparator)
{
	int i, nCount;
	LPSTR pszTextTmp;
	LPCSTR pszTmp, pszStart;
	CmyString strParam;

	m_astrParam.RemoveAll ();
	if (pszText == NULL) {
		return;
	}

	nCount = strlen (pszText);
	pszStart = pszTmp = pszText;
	pszTextTmp = new char[nCount + 1];

	for (i = 0; i < nCount; i ++) {
		if ((i == nCount - 1) || (pszTmp[i + 1] == cSeparator)) {
			ZeroMemory (pszTextTmp, nCount + 1);
			CopyMemory (pszTextTmp, pszStart, &pszTmp[i] - pszStart + 1);
			pszStart = &pszTmp[i + 2];
			strParam = pszTextTmp;
			m_astrParam.Add (strParam);
		}
	}
	SAFE_DELETE_ARRAY (pszTextTmp);
}


/* ========================================================================= */
/* �֐���	:CParamUtil::GetCount											 */
/* ���e		:�p�����[�^���擾												 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

int CParamUtil::GetCount(void)
{
	return m_astrParam.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CParamUtil::GetParam											 */
/* ���e		:�p�����[�^�擾													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

LPCSTR CParamUtil::GetParam(int nNo)
{
	int nCount;
	LPCSTR pszRet;

	pszRet = NULL;

	nCount = GetCount ();
	if (nNo >= nCount) {
		goto Exit;
	}

	pszRet = m_astrParam[nNo];
Exit:
	return pszRet;
}

/* Copyright(C)URARA-works 2007 */

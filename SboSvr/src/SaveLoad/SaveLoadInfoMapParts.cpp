/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoMapParts.cpp									 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݃N���X(�}�b�v�p�[�c���) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapParts.h"
#include "SaveLoadInfoMapParts.h"


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoMapParts::CSaveLoadInfoMapParts					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

CSaveLoadInfoMapParts::CSaveLoadInfoMapParts()
{
	/* �t�@�C�����̐ݒ� */
	SetFileName ("SBODATA\\SboDataMapParts.dat");
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoMapParts::~CSaveLoadInfoMapParts					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

CSaveLoadInfoMapParts::~CSaveLoadInfoMapParts()
{
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoMapParts::SetHeaderInfo							 */
/* ���e		:�w�b�_����ݒ�												 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CSaveLoadInfoMapParts::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	char szTmp[256];
	LPCSTR pszName;
	CInfoAnime AnimeTmp;

	CSaveLoadInfoBase::SetHeaderInfo (pInfo);

	for (i = 0; ; i ++) {
		pszName = AnimeTmp.GetName (i);
		if (pszName == NULL) {
			break;
		}
		wsprintf (szTmp, "%s%s", PREFIX_INFOANIME, pszName);
		AddHeaderInfo (szTmp);
	}
}

/* Copyright(C)URARA-works 2007 */

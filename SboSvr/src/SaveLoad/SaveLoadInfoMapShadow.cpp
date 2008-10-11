/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoMapShadow.cpp									 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݃N���X(�}�b�v�e���) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapShadow.h"
#include "SaveLoadInfoMapShadow.h"


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoMapShadow::CSaveLoadInfoMapShadow					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CSaveLoadInfoMapShadow::CSaveLoadInfoMapShadow()
{
	/* �t�@�C�����̐ݒ� */
	SetFileName ("SBODATA\\SboDataMapShadow.dat");
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoMapShadow::~CSaveLoadInfoMapShadow				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CSaveLoadInfoMapShadow::~CSaveLoadInfoMapShadow()
{
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoMapShadow::SetHeaderInfo							 */
/* ���e		:�w�b�_����ݒ�												 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CSaveLoadInfoMapShadow::SetHeaderInfo(PCInfoBase pInfo)
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

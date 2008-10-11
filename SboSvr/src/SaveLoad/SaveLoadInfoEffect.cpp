/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoEffect.cpp										 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݃N���X(�G�t�F�N�g���) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEffect.h"
#include "SaveLoadInfoEffect.h"


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoEffect::CSaveLoadInfoEffect						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CSaveLoadInfoEffect::CSaveLoadInfoEffect()
{
	/* �t�@�C�����̐ݒ� */
	SetFileName ("SBODATA\\SboDataEffect.dat");
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoEffect::~CSaveLoadInfoEffect						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CSaveLoadInfoEffect::~CSaveLoadInfoEffect()
{
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoEffect::SetHeaderInfo								 */
/* ���e		:�w�b�_����ݒ�												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CSaveLoadInfoEffect::SetHeaderInfo(PCInfoBase pInfo)
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

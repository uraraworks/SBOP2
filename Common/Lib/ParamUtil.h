/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:ParamUtil.h												 */
/* ���e			:��؂蕶���t���̃p�����[�^�����N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/02													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */
class CParamUtil
{
public:
			CParamUtil();								/* �R���X�g���N�^ */
	virtual ~CParamUtil();								/* �f�X�g���N�^ */

	void	Set			(LPCSTR pszText, char cSeparator=' ');	/* �p�����[�^�ݒ� */
	int		GetCount	(void);									/* �p�����[�^���擾 */
	LPCSTR	GetParam	(int nNo);								/* �p�����[�^�擾 */


protected:
	CmyStringArray		m_astrParam;					/* �������ꂽ�p�����[�^ */
};

/* Copyright(C)URARA-works 2007 */

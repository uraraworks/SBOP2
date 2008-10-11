/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibSboSoundLoader.h										 */
/* ���e			:SBO�T�E���h�f�[�^���C�u�����ǂݍ��݃N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/03													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �֐��|�C���^��`															 */
/* ========================================================================= */

typedef int		(*LIBSBOSOUNDGetSoundCount)			(void);
typedef int		(*LIBSBOSOUNDGetSoundResourceID)	(int nNo);
typedef DWORD	(*LIBSBOSOUNDGetSoundID)			(int nNo);
typedef int		(*LIBSBOSOUNDGetSoundNo)			(DWORD dwSoundID);
typedef LPCSTR	(*LIBSBOSOUNDGetSoundName)			(DWORD dwSoundID);


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibSboSoundLoader
{
public:
			CLibSboSoundLoader();					/* �R���X�g���N�^ */
	virtual	~CLibSboSoundLoader();					/* �f�X�g���N�^ */

	void	Load (void);									/* �ǂݍ��� */
	void	Free (void);									/* �J�� */

	int		GetSoundCount		(void);						/* ���ʉ������擾 */
	int		GetSoundResourceID	(int nNo);					/* ���ʉ��̃��\�[�XID���擾 */
	DWORD	GetSoundID			(int nNo);					/* ���ʉ�ID���擾 */
	int		GetSoundNo			(DWORD dwSoundID);			/* ���ʉ��̃C���f�b�N�X���擾 */
	LPCSTR	GetSoundName		(DWORD dwSoundID);			/* ���ʉ������擾 */


protected:
	HMODULE				m_hLib;		/* DLL�̃��W���[���n���h�� */

	/* �ȉ��֐��|�C���^ */
	LIBSBOSOUNDGetSoundCount		m_pGetSoundCount;
	LIBSBOSOUNDGetSoundResourceID	m_pGetSoundResourceID;
	LIBSBOSOUNDGetSoundID			m_pGetSoundID;
	LIBSBOSOUNDGetSoundNo			m_pGetSoundNo;
	LIBSBOSOUNDGetSoundName			m_pGetSoundName;
} CLibSboSoundLoader, *PCLibSboSoundLoader;

/* Copyright(C)URARA-works 2007 */

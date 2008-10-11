/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:InfoBase.h													 */
/* ���e			:�����N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/06/06													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoBase
{
public:
			CInfoBase();											/* �R���X�g���N�^ */
	virtual ~CInfoBase();											/* �f�X�g���N�^ */

	virtual int		GetElementCount		(void);								/* �v�f�����擾 */
	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */


public:
	int			m_nElementCount;					/* �v�f�� */
} CInfoBase, *PCInfoBase;
typedef CmyArray<PCInfoBase, PCInfoBase>	  ARRAYBASEINFO;
typedef CmyArray<PCInfoBase, PCInfoBase>	*PARRAYBASEINFO;

/* Copyright(C)URARA-works 2005 */

/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:myArray.h													 */
/* ���e			:MFC��CArray�݂����Ȕz��N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/01/18													 */
/* ========================================================================= */

#pragma once

#include "myString.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */
template<class TYPE, class ARG_TYPE> class CmyArray
{
public:
			CmyArray();									/* �R���X�g���N�^ */
	virtual ~CmyArray();								/* �f�X�g���N�^ */

	int		GetSize		(void);										/* �v�f�����擾 */
	void	RemoveAll	(void);										/* �S�v�f���폜 */
	TYPE	GetAt		(int nIndex);								/* �w��v�f���擾 */
	void	SetAt		(int nIndex, ARG_TYPE newElement);			/* �w��v�f���X�V */
	int		Add			(ARG_TYPE newElement);						/* �v�f��ǉ� */
	void	InsertAt	(int nIndex, ARG_TYPE newElement);			/* �w��ʒu�ɒǉ� */
	void	RemoveAt	(int nIndex, int nCount = 1);				/* �w��v�f���폜 */
	void	Copy		(CmyArray *pSrc);							/* �R�s�[ */

	TYPE& operator [](int nIndex);								/* �w��v�f�֒��ڃA�N�Z�X */


private:
	TYPE		*m_pData;					/* �f�[�^ */
	int			m_nCount;					/* �v�f�� */
};

typedef CmyArray<int, int>				  ARRAYINT;
typedef CmyArray<int, int>				*PARRAYINT;
typedef CmyArray<DWORD, DWORD>			  ARRAYDWORD;
typedef CmyArray<DWORD, DWORD>			*PARRAYDWORD;
typedef CmyArray<BYTE, BYTE>			  ARRAYBYTE;
typedef CmyArray<BYTE, BYTE>			*PARRAYBYTE;
typedef CmyArray<CmyString, CmyString>	CmyStringArray;

/* �����������C���N���[�h */
#include "myArrayImpl.h"

/* Copyright(C)URARA-works 2005 */

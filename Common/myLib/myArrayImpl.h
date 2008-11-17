/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:MyArrayImpl.h												 */
/* ���e			:MFC��CArray�݂����Ȕz��N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/01/18													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::CmyArray								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
CmyArray<TYPE, ARG_TYPE>::CmyArray()
{
	m_pData = NULL;
	m_nCount = 0;
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::~CmyArray							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
CmyArray<TYPE, ARG_TYPE>::~CmyArray()
{
	RemoveAll ();
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::GetSize								 */
/* ���e		:�v�f�����擾													 */
/* �߂�l	:�v�f��															 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
int CmyArray<TYPE, ARG_TYPE>::GetSize(void)
{
	return m_nCount;
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::RemoveAll							 */
/* ���e		:�S�v�f���폜													 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
void CmyArray<TYPE, ARG_TYPE>::RemoveAll(void)
{
	if (m_pData) {
		delete [] m_pData;
		m_pData = NULL;
	}
	m_nCount = 0;
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::GetAt								 */
/* ���e		:�w��v�f���擾													 */
/* �߂�l	:�v�f�̃f�[�^													 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
TYPE CmyArray<TYPE, ARG_TYPE>::GetAt(
	int nIndex)		/* [in] �v�fID */
{
	TYPE Ret;

//	ZeroMemory (&Ret, sizeof (TYPE));

	if (nIndex >= m_nCount) {
		goto Exit;
	}

	Ret = m_pData[nIndex];

Exit:
	return Ret;
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::SetAt								 */
/* ���e		:�w��v�f���X�V													 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
void CmyArray<TYPE, ARG_TYPE>::SetAt(
	int nIndex,					/* [in] �v�fID */
	ARG_TYPE newElement)		/* [in] �V�����f�[�^ */
{
	if (nIndex >= m_nCount) {
		goto Exit;
	}

	m_pData[nIndex] = newElement;

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::Add									 */
/* ���e		:�v�f��ǉ�														 */
/* �߂�l	:�ǉ����ꂽ�v�fID												 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
int CmyArray<TYPE, ARG_TYPE>::Add(
	ARG_TYPE newElement)		/* [in] �ǉ�����f�[�^ */
{
	int i;
	TYPE *pData;

	/* �ǉ����̗v�f���m�� */
	pData = new TYPE[m_nCount + 1];

	/* �ǉ��O�̃f�[�^���R�s�[ */
	for (i = 0; i < m_nCount; i ++) {
		pData[i] = m_pData[i];
	}
	pData[m_nCount] = newElement;

	if (m_pData) {
		/* ���܂ł̃f�[�^��j�� */
		delete [] m_pData;
	}

	m_pData = pData;
	m_nCount ++;

	/* �V����ID��Ԃ� */
	return (m_nCount - 1);
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::InsertAt								 */
/* ���e		:�w��ʒu�ɒǉ�													 */
/* ���t		:2005/08/19														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
void CmyArray<TYPE, ARG_TYPE>::InsertAt(
	int nIndex,					/* [in] �v�fID */
	ARG_TYPE newElement)		/* [in] �ǉ�����f�[�^ */
{
	int i, nTmp;
	TYPE *pData;

	/* �ǉ����̗v�f���m�� */
	pData = new TYPE[m_nCount + 1];

	nTmp = 0;
	/* �ǉ��O�̃f�[�^���R�s�[ */
	for (i = 0; i < m_nCount + 1; i ++) {
		if (i == nIndex) {
			continue;
		}
		CopyMemory (&pData[i], &m_pData[nTmp], sizeof (TYPE));
		nTmp ++;
	}
	pData[nIndex] = newElement;

	if (m_pData) {
		/* ���܂ł̃f�[�^��j�� */
		delete [] m_pData;
	}

	m_pData = pData;
	m_nCount ++;
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::RemoveAt								 */
/* ���e		:�w��v�f���폜													 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
void CmyArray<TYPE, ARG_TYPE>::RemoveAt(
	int nIndex,		/* [in] �v�fID */
	int nCount)		/* [in] �폜�� */
{
	int i, j, nID;
	TYPE *pData;

	/* �w�蕪�����v�f���폜���郋�[�v */
	for (j = 0; j < nCount; j ++) {
		/* �z�񂩂�͂ݏo���H */
		if (nIndex >= m_nCount) {
			break;
		}

		/* �폜�������̃f�[�^���m�� */
		pData = new TYPE[m_nCount - 1];
		/* �w��v�f�ȊO���R�s�[���郋�[�v */
		for (i = 0, nID = 0; i < m_nCount; i ++) {
			if (nIndex == i) {
				continue;
			}
			pData[nID] = m_pData[i];
			nID ++;
		}
		if (m_pData) {
			delete [] m_pData;
		}
		m_pData = pData;
		m_nCount --;
	}
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::Copy									 */
/* ���e		:�R�s�[															 */
/* ���t		:2005/06/03														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
void CmyArray<TYPE, ARG_TYPE>::Copy(
	CmyArray *pSrc)		/* [in] �R�s�[�� */
{
	int i, nCount;

	RemoveAll ();
	nCount = pSrc->GetSize ();
	for (i = 0; i < nCount; i ++) {
		Add (pSrc->GetAt (i));
	}
}


/* ========================================================================= */
/* �֐���	:CmyArray<TYPE, ARG_TYPE>::operator []							 */
/* ���e		:�w��v�f�֒��ڃA�N�Z�X											 */
/* �߂�l	:�v�f															 */
/* ���t		:2005/01/18														 */
/* ========================================================================= */

template<class TYPE, class ARG_TYPE>
TYPE& CmyArray<TYPE, ARG_TYPE>::operator [](
	int nIndex)		/* [in] �v�fID */
{
	return m_pData[nIndex];
}

/* Copyright(C)URARA-works 2005 */

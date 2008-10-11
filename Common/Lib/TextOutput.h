/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	TextOutput.h												 */
/* ���e�F		�e�L�X�g�o�̓N���X ��`�t�@�C��								 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/04/01													 */
/* ========================================================================= */

#ifndef _TEXTOUTPUT_H
#define _TEXTOUTPUT_H

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CTextOutput
{
public:
			CTextOutput();											/* �R���X�g���N�^ */
	virtual ~CTextOutput();											/* �f�X�g���N�^ */

	BOOL Create		(LPCSTR pszFileName, BOOL bHeader, BOOL m_bReturn);		/* ������ */
	void Destroy	(void);													/* �j�� */
	void Write		(LPCSTR format, ...);									/* �������� */


protected:
	void WriteProc	(LPCSTR pszText);										/* �������ݏ��� */


protected:
	LPSTR				m_pszFileName;			/* �t�@�C���� */
	BOOL				m_bHeader;				/* �w�b�_���������� */
	BOOL				m_bReturn;				/* ���s�R�[�h���������� */
	CRITICAL_SECTION	m_csWrite;				/* �������ݔr���p */
};

#endif
/* Copyright(C)URARA-works 2005 */

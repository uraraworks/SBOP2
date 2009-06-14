/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C�����F	RouteSearch.h												 */
/* ���e�F		�o�H�T���N���X ��`�t�@�C��									 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2009/06/13													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */

/* ������� */
typedef struct _SEARCHINFO {
	POINT	pt;					/* ���W */
	BYTE	byDirection;		/* ���� */
	BYTE	byState;			/* ��� */
	WORD	wScore;				/* �X�R�A */
} SEARCHINFO, *PSEARCHINFO;
typedef CmyArray<SEARCHINFO, SEARCHINFO>		ARRAYSEARCHINFO;

/* ����������� */
typedef struct _SEARCHPROCINFO {
	int		nState,				/* ������� */
			nDirection,			/* ���� */
			nTurn;				/* �^�[�� */
	POINT	ptNow,				/* ���݈ʒu */
			ptStart,			/* �J�n�ʒu */
			ptEnd;				/* �ŏI�ʒu */
	SIZE	sizeMap;			/* �����p�}�b�v�T�C�Y */
	PBYTE	pSearchMap;			/* �������}�b�v */
	PWORD	pMap;				/* �����͈̓}�b�v */
	ARRAYSEARCHINFO	aInfo;		/* ������� */
} SEARCHPROCINFO, *PSEARCHPROCINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CRouteSearch
{
public:
			CRouteSearch();						/* �R���X�g���N�^ */
	virtual ~CRouteSearch();					/* �f�X�g���N�^ */

	void	SetStartPos		(int x, int y);								/* �J�n�ʒu��ݒ� */
	void	SetEndPos		(int x, int y);								/* �ŏI�ʒu��ݒ� */
	void	SetSize			(int cx, int cy);							/* �͈̓T�C�Y��ݒ� */
	void	SetBlock		(int x, int y, BOOL bSet);					/* ��Q����ݒ� */
	void	Search			(void);										/* ���� */
	BOOL	GetPos			(int &nDstDirection, POINT &ptDstPos);		/* ���̍��W���擾 */


private:
	void	InitProcInfo	(void);								/* ������������������ */
	void	ChgState		(int nState);						/* ��ԑJ�� */
	void	Proc			(void);								/* �������� */
	BOOL	ProcSEARCH		(void);								/* ��������(������) */
	BOOL	ProcSEARCHMOVE	(void);								/* ��������(�����ړ���) */
	void	InfoCleanup		(int x, int y);						/* �������œK�� */
//	void	ProcMOVE		(void);								/* ��������(�ړ���) */


private:
	SEARCHPROCINFO	m_ProcInfo;			/* ����������� */
};

/* Copyright(C)URARA-works 2009 */

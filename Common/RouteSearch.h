/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C�����F	RouteSearch.h												 */
/* ���e�F		�o�H�T���N���X ��`�t�@�C��									 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2009/06/13													 */
/* ========================================================================= */

#pragma once

class CInfoMapBase;

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */

/* �������� */
typedef struct _SEARCHRESULT {
	POINT	pt;					/* ���W */
	BYTE	byDirection;		/* ���� */
} SEARCHRESULT, *PSEARCHRESULT;
typedef CmyArray<SEARCHRESULT, SEARCHRESULT>	  ARRAYSEARCHRESULT;
typedef CmyArray<SEARCHRESULT, SEARCHRESULT>	*PARRAYSEARCHRESULT;

/* ������� */
typedef struct _SEARCHINFO {
	POINT	pt;					/* ���W */
	BYTE	byDirection;		/* ���� */
	BYTE	byState;			/* ��� */
	WORD	wScore;				/* �X�R�A */
} SEARCHINFO, *PSEARCHINFO;
typedef CmyArray<SEARCHINFO, SEARCHINFO>		ARRAYSEARCHINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CRouteSearch
{
public:
			CRouteSearch();								/* �R���X�g���N�^ */
	virtual ~CRouteSearch();							/* �f�X�g���N�^ */

	void	SetMapInfo		(CInfoMapBase *pInfoMap);			/* �����Ώۃ}�b�v��� */
	void	SetSearchArea	(const RECT &rcSearch);				/* �����͈͂�ݒ� */
	void	SetStartPos		(int x, int y);						/* �J�n�ʒu��ݒ� */
	void	SetEndPos		(int x, int y);						/* �ŏI�ʒu��ݒ� */
	PARRAYSEARCHRESULT	Search	(void);								/* ���� */


private:
	void	InitProcInfo	(void);								/* ������������������ */
	void	ProcSEARCH		(void);								/* ��������(������) */
	BOOL	ProcSEARCHMOVE	(void);								/* ��������(�����ړ���) */
	void	InfoCleanup		(int x, int y);						/* �������œK�� */


private:
	int		m_nState;					/* ������� */
	POINT	m_ptNow,					/* ���݈ʒu */
			m_ptStart,					/* �J�n�ʒu */
			m_ptEnd;					/* �ŏI�ʒu */
	PWORD	m_pMap;						/* �������}�b�v�e���|���� */
	SIZE	m_sizeMap;					/* �����͈̓T�C�Y */
	RECT	m_rcSearch;					/* �����͈� */
	CInfoMapBase		*m_pInfoMap;	/* �����Ώۃ}�b�v��� */
	ARRAYSEARCHINFO		m_aInfo;		/* ������� */
	ARRAYSEARCHRESULT	m_aResult;		/* �������� */
};

/* Copyright(C)URARA-works 2009 */

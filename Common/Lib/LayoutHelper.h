/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	LayoutHelper.h												 */
/* ���e�F		�E�B���h�E�T�C�Y�ύX���̔z�u�⏕�N���X ��`�t�@�C��			 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/06/17													 */
/* ========================================================================= */

#pragma once

#pragma	warning(disable:4786)

#include <vector>
#include <map>

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

/* �␳�t���O */
#define	LH_CTRL_X			0x00000001		// X������������
#define	LH_CTRL_Y			0x00000002		// Y������������
#define	LH_CTRL_WIDTH		0x00000004		// ����������
#define	LH_CTRL_HEIGHT		0x00000008		// ������������

#define	LH_GRP_X			0x00000001		// X������������
#define	LH_GRP_Y			0x00000002		// Y������������
#define	LH_GRP_WIDTH		0x00000004		// ����������
#define	LH_GRP_HEIGHT		0x00000008		// ������������

// �O���[�v�ړ��t���O
#define	LH_GRP_NOXMOVE		0x00000001		// �������ړ��Ȃ�
#define	LH_GRP_NOYMOVE		0x00000002		// �c�����ړ��Ȃ�
#define	LH_GRP_NOMOVE		(LH_GRP_NOXMOVE | LH_GRP_NOYMOVE)	// �ړ��Ȃ�
#define	LH_GRP_NOWSIZE		0x00000004		// ���ݒ�Ȃ�
#define	LH_GRP_NOHSIZE		0x00000008		// �����ݒ�Ȃ�
#define	LH_GRP_NOSIZE		(LH_GRP_NOWSIZE | LH_GRP_NOHSIZE)	// �T�C�Y�ݒ�Ȃ�


/* ========================================================================= */
/* �\���̂̒�`																 */
/* ========================================================================= */

struct LH_CTRL_LAYOUT {
	HWND	hWnd;				/* �E�B���h�E�n���h�� */
	int		x;					/* �ʒu(X) */
	int		y;					/* �ʒu(Y) */
	int		cx;					/* �� */
	int		cy;					/* ���� */
	DWORD	dwFlags;			/* �␳�t���O */
	UINT	uGroup;				// ��������O���[�vID
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CLayoutHelper
{
public:
			CLayoutHelper();							/* �R���X�g���N�^ */
	virtual	~CLayoutHelper();							/* �f�X�g���N�^ */

	void	Initialize			(HWND hBase);									/* ������ */
	BOOL	RegisterControl		(HWND hWnd, DWORD dwFlags, UINT uGroup = -1);	/* �R���g���[���̓o�^ */
	BOOL	RegisterControl		(UINT uID, DWORD dwFlags, UINT uGroup = -1);	/* �R���g���[���̓o�^ */
	BOOL	UnregisterControl	(HWND hWnd);									/* �R���g���[���̉��� */
	BOOL	UnregisterControl	(UINT uID);										/* �R���g���[���̉��� */
	void	DoLayout			(int cx, int cy);								/* �X�V */

	BOOL	AddGroup			(UINT uGroup, DWORD dwFlags);					/* �O���[�v�̒ǉ� */
	BOOL	DeleteGroup			(UINT uGroup);									/* �O���[�v�̍폜 */
	BOOL	SetGroup			(HWND hWnd, UINT uGroup);						/* �O���[�v�̐ݒ� */
	BOOL	SetGroup			(UINT uID, UINT uGroup);						/* �O���[�v�̐ݒ� */
	BOOL	RemoveGroup			(HWND hWnd);									/* �O���[�v�̉��� */
	BOOL	RemoveGroup			(UINT uID);										/* �O���[�v�̉��� */
	BOOL	GetGroupRect		(UINT uGroup, RECT* pRect);						/* �O���[�v�̋�`���擾 */
	void	MoveGroup			(UINT uGroup, int x, int y, int cx, int cy, DWORD dwFlags = 0);	/* �O���[�v�̈ړ� */
	void	OffsetGroup			(UINT uGroup, int left, int top, int right, int bottom);		/* �O���[�v�̃T�C�Y�ύX */


private:
	HWND	m_hBase;							/* ���ƂȂ�E�B���h�E�n���h�� */
	SIZE	m_sizeBase;							/* ���T�C�Y */
	std::vector<LH_CTRL_LAYOUT>	m_vLayout;		/* �o�^���ꂽ���C�A�E�g��� */
	std::map<UINT, DWORD>	m_mapGroup;			/* �o�^���ꂽ�O���[�v��� */
};

/* Copyright(C)URARA-works 2005 */

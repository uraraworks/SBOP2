/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����		:LayoutHelper.cpp										 */
/* ���e		:	�E�B���h�E�T�C�Y�ύX���̔z�u�⏕�N���X �����t�@�C��			 */
/* �쐬		:	�N����N���t�����(URARA-works)								 */
/* �쐬�J�n��		:2005/06/17												 */
/* ========================================================================= */

#include "stdafx.h"
#include "LayoutHelper.h"

typedef	std::vector<LH_CTRL_LAYOUT>::iterator	LayoutIterator;
typedef	std::map<UINT, DWORD>::iterator			GroupIterator;


/* ========================================================================= */
/* �֐���	:CLayoutHelper::CLayoutHelper									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

CLayoutHelper::CLayoutHelper()
{
	m_hBase = NULL;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::~CLayoutHelper									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

CLayoutHelper::~CLayoutHelper()
{
	m_vLayout.clear();
	m_mapGroup.clear();
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::Initialize										 */
/* ���e		:������															 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

void CLayoutHelper::Initialize(HWND hBase)
{
	RECT rc;

	m_hBase = hBase;

	GetClientRect (m_hBase, &rc);
	m_sizeBase.cx = rc.right - rc.left;
	m_sizeBase.cy = rc.bottom - rc.top;

	m_vLayout.clear();
	m_mapGroup.clear();
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::RegisterControl									 */
/* ���e		:�R���g���[���̓o�^												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::RegisterControl(HWND hWnd, DWORD dwFlags, UINT uGroup)
{
	if (IsWindow(hWnd) == FALSE) {
		return FALSE;
	}

	RECT rc;
	LH_CTRL_LAYOUT lhLayout;

	GetWindowRect (hWnd, &rc);
	ScreenToClient (m_hBase, (LPPOINT)&rc);
	ScreenToClient (m_hBase, ((LPPOINT)&rc) + 1);

	lhLayout.hWnd		= hWnd;
	lhLayout.x			= rc.left;
	lhLayout.y			= rc.top;
	lhLayout.cx			= rc.right - rc.left;
	lhLayout.cy			= rc.bottom - rc.top;
	lhLayout.dwFlags	= dwFlags;
	lhLayout.uGroup		= uGroup;

	m_vLayout.push_back (lhLayout);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::RegisterControl									 */
/* ���e		:�R���g���[���̓o�^												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::RegisterControl(UINT uID, DWORD dwFlags, UINT uGroup)
{
	HWND	hWnd = GetDlgItem(m_hBase, uID);
	if (hWnd == NULL) {
		return FALSE;
	}

	return RegisterControl(hWnd, dwFlags, uGroup);
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::UnregisterControl								 */
/* ���e		:�R���g���[���̉���												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::UnregisterControl(HWND hWnd)
{
	for (LayoutIterator it = m_vLayout.begin (); it != m_vLayout.end (); ++ it) {
		if (it->hWnd == hWnd) {
			m_vLayout.erase (it);
			return TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::UnregisterControl								 */
/* ���e		:�R���g���[���̉���												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::UnregisterControl(UINT uID)
{
	HWND	hWnd = GetDlgItem(m_hBase, uID);
	if (hWnd == NULL) {
		return FALSE;
	}

	return UnregisterControl(hWnd);
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::DoLayout										 */
/* ���e		:�X�V															 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

void CLayoutHelper::DoLayout(int cx, int cy)
{
	int	dx = cx - m_sizeBase.cx;
	int	dy = cy - m_sizeBase.cy;
	m_sizeBase.cx = cx;
	m_sizeBase.cy = cy;

	HDWP hdwp = BeginDeferWindowPos (m_vLayout.size ());

	for (LayoutIterator it = m_vLayout.begin (); it != m_vLayout.end (); ++ it) {
		DWORD	dwFlags;
		GroupIterator	itGroup = m_mapGroup.find(it->uGroup);
		if (itGroup != m_mapGroup.end()) {
			dwFlags = itGroup->second;
			dwFlags &= it->dwFlags;
		} else {
			dwFlags = it->dwFlags;
		}
		RECT rc;
		SetRect (&rc, it->x, it->y, it->x + it->cx, it->y + it->cy);
		if ((dwFlags & LH_CTRL_X) != 0) {
			rc.left += dx;
			rc.right += dx;
		}
		if ((dwFlags & LH_CTRL_Y) != 0) {
			rc.top += dy;
			rc.bottom += dy;
		}
		if ((dwFlags & LH_CTRL_WIDTH) != 0) {
			rc.right += dx;
		}
		if ((dwFlags & LH_CTRL_HEIGHT) != 0) {
			rc.bottom += dy;
		}
		it->x = rc.left;
		it->y = rc.top;
		it->cx = rc.right - rc.left;
		it->cy = rc.bottom - rc.top;
		hdwp = DeferWindowPos (hdwp, it->hWnd, NULL, it->x, it->y, it->cx, it->cy, SWP_NOOWNERZORDER | SWP_NOZORDER);
	}

	EndDeferWindowPos (hdwp);
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::AddGroup										 */
/* ���e		:�O���[�v�̒ǉ�													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::AddGroup(UINT uGroup, DWORD dwFlags)
{
	// �O���[�v��ǉ�
	m_mapGroup[uGroup] = dwFlags;

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::DeleteGroup										 */
/* ���e		:�O���[�v�̍폜													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::DeleteGroup(UINT uGroup)
{
	/* �O���[�v���폜 */
	GroupIterator	itGroup = m_mapGroup.find(uGroup);
	if (itGroup == m_mapGroup.end()) {
		return FALSE;
	}
	m_mapGroup.erase(itGroup);
	/* �R���g���[������O���[�v���폜 */
	for (LayoutIterator it = m_vLayout.begin(); it != m_vLayout.end(); ++it) {
		if (it->uGroup == uGroup) {
			it->uGroup = -1;
		}
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::SetGroup										 */
/* ���e		:�O���[�v�̐ݒ�													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::SetGroup(HWND hWnd, UINT uGroup)
{
	if (m_mapGroup.find(uGroup) == m_mapGroup.end()) {
		return FALSE;
	}

	for (LayoutIterator it = m_vLayout.begin(); it != m_vLayout.end(); ++it) {
		if ((it->uGroup == uGroup) && (it->hWnd == hWnd)) {
			it->uGroup = uGroup;
			return TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::SetGroup										 */
/* ���e		:�O���[�v�̐ݒ�													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::SetGroup(UINT uID, UINT uGroup)
{
	HWND	hWnd = GetDlgItem(m_hBase, uID);
	if (hWnd == NULL) {
		return FALSE;
	}

	return SetGroup(hWnd, uGroup);
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::RemoveGroup										 */
/* ���e		:�O���[�v�̉���													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::RemoveGroup(HWND hWnd)
{
	for (LayoutIterator it = m_vLayout.begin(); it != m_vLayout.end(); ++it) {
		if (it->hWnd == hWnd) {
			it->uGroup = -1;
			return TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::RemoveGroup										 */
/* ���e		:�O���[�v�̉���													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::RemoveGroup(UINT uID)
{
	HWND	hWnd = GetDlgItem(m_hBase, uID);
	if (hWnd == NULL) {
		return FALSE;
	}

	return RemoveGroup(hWnd);
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::GetGroupRect									 */
/* ���e		:�O���[�v�̋�`���擾											 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CLayoutHelper::GetGroupRect(UINT uGroup, RECT* pRect)
{
	if ((uGroup == -1) || (m_mapGroup.find(uGroup) == m_mapGroup.end())) {
		return FALSE;
	}

	BOOL	bExist = FALSE;
	RECT	rc;
	SetRect(&rc, 0, 0, 0, 0);
	for (LayoutIterator it = m_vLayout.begin(); it != m_vLayout.end(); ++it) {
		if (it->uGroup == uGroup) {
			RECT	rcTemp = rc;
			RECT	rcCtrl;
			SetRect(&rcCtrl, it->x, it->y, it->x + it->cx, it->y + it->cy);
			UnionRect(&rc, &rcTemp, &rcCtrl);
			bExist = TRUE;
		}
	}

	if (bExist != FALSE) {
		*pRect = rc;
	}

	return bExist;
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::MoveGroup										 */
/* ���e		:�O���[�v�̈ړ�													 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

void CLayoutHelper::MoveGroup(UINT uGroup, int x, int y, int cx, int cy, DWORD dwFlags)
{
	RECT	rc;
	if (GetGroupRect(uGroup, &rc) == FALSE) {
		return;
	}

	x = ((dwFlags & LH_GRP_NOXMOVE) != 0) ? x - rc.left : 0;
	y = ((dwFlags & LH_GRP_NOYMOVE) != 0) ? y - rc.top : 0;
	cx = ((dwFlags & LH_GRP_NOWSIZE) != 0) ? cx - (rc.right - rc.left) : 0;
	cy = ((dwFlags & LH_GRP_NOHSIZE) != 0) ? cy - (rc.bottom - rc.top) : 0;

	HDWP	hdwp = BeginDeferWindowPos(m_vLayout.size());

	for (LayoutIterator it = m_vLayout.begin(); it != m_vLayout.end(); ++it) {
		if (it->uGroup == uGroup) {
			GetWindowRect(it->hWnd, &rc);
			ScreenToClient(m_hBase, (LPPOINT)&rc);
			ScreenToClient(m_hBase, ((LPPOINT)&rc) + 1);
			if ((it->dwFlags & LH_CTRL_X) != 0) {
				it->x = rc.left + x;
			}
			if ((it->dwFlags & LH_CTRL_Y) != 0) {
				it->y = rc.top + y;
			}
			if ((it->dwFlags & LH_CTRL_WIDTH) != 0) {
				it->cx = (rc.right - rc.left) + cx;
			}
			if ((it->dwFlags & LH_CTRL_HEIGHT) != 0) {
				it->cy = (rc.bottom - rc.top) + cy;
			}
			hdwp = DeferWindowPos(hdwp, it->hWnd, NULL, it->x, it->y, it->cx, it->cy, SWP_NOOWNERZORDER | SWP_NOZORDER);
		}
	}

	EndDeferWindowPos(hdwp);
}


/* ========================================================================= */
/* �֐���	:CLayoutHelper::OffsetGroup										 */
/* ���e		:�O���[�v�̃T�C�Y�ύX											 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

void CLayoutHelper::OffsetGroup(UINT uGroup, int left, int top, int right, int bottom)
{
	HDWP	hdwp = BeginDeferWindowPos(m_vLayout.size());

	for (LayoutIterator it = m_vLayout.begin(); it != m_vLayout.end(); ++it) {
		if (it->uGroup == uGroup) {
			RECT	rc;
			GetWindowRect(it->hWnd, &rc);
			ScreenToClient(m_hBase, (LPPOINT)&rc);
			ScreenToClient(m_hBase, ((LPPOINT)&rc) + 1);
			if ((it->dwFlags & LH_CTRL_X) != 0) {
				it->x += left;
			}
			if ((it->dwFlags & LH_CTRL_Y) != 0) {
				it->y += top;
			}
			if ((it->dwFlags & LH_CTRL_WIDTH) != 0) {
				it->cx = (rc.right - rc.left) + (right - left);
			}
			if ((it->dwFlags & LH_CTRL_HEIGHT) != 0) {
				it->cy = (rc.bottom - rc.top) + (bottom - top);
			}
			hdwp = DeferWindowPos(hdwp, it->hWnd, NULL, it->x, it->y, it->cx, it->cy, SWP_NOOWNERZORDER | SWP_NOZORDER);
		}
	}

	EndDeferWindowPos(hdwp);
}

/* Copyright(C)URARA-works 2005 */

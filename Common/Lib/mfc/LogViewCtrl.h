#ifndef	__LOGVIEWCTRL_H__
#define	__LOGVIEWCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// �\���ݒ�
struct LOGVIEWCTRLSETTING {
	TCHAR		szFontText[LF_FACESIZE];	// �t�H���g��(�{��)
	TCHAR		szFontHeader[LF_FACESIZE];	// �t�H���g��(�w�b�_)
	int			nFontPoint;					// �t�H���g�T�C�Y(�|�C���g)
	COLORREF	crText;						// �e�L�X�g�����F
	COLORREF	crBack;						// �e�L�X�g�w�i�F
	COLORREF	crSelectText;				// �I�𕶎��F
	COLORREF	crSelectBack;				// �I��w�i�F
	COLORREF	crLinkText;					// �����N�����F
	COLORREF	crLinkBack;					// �����N�w�i�F
	BOOL		bCancelSelectAfterCopy;		// �R�s�[������A�I���������邩
	int			nWheelDelta;				// �}�E�X�z�C�[���ł̈ړ���
	int			nLinkNotifyType;			// �����N�ʒm���
};

// �s���
struct LINEINFO {
	SYSTEMTIME	stDateTime;					// ����
	CString		strText;					// �s�e�L�X�g
	COLORREF	crFore;						// �O�i�F
	COLORREF	crBack;						// �w�i�F
	struct TOKEN {
		int		nStart;						// �P��J�n�ʒu
		int		nLen;						// �P��̒���(�o�C�g)
		int		nType;						// �P��̎��
	};
	CArray<TOKEN, TOKEN&>	aTokens;		// �P����

	void operator=(const LINEINFO& li) {
		stDateTime = li.stDateTime;
		strText = li.strText;
		crFore = li.crFore;
		crBack = li.crBack;
		aTokens.Copy(li.aTokens);
	}
};

// �ʒm���
struct NMLVCLINK {
	NMHDR	hdr;
	LPCTSTR	pszLink;
};

// �e�L�X�g�ێ����E
#define	LVC_TEXT_LIMIT_INFINITE			0	// ������(�������ˑ�)
#define	LVC_TEXT_LIMIT_LINE				1	// �s������
#define	LVC_TEXT_LIMIT_SIZE				2	// �T�C�Y����

// �����N�ʒm���
#define	LVC_LINK_NOTIFY_NONE			0	// �����N�ʒm�Ȃ�
#define	LVC_LINK_NOTIFY_DOUBLE_CLICK	1	// �_�u���N���b�N�Œʒm
#define	LVC_LINK_NOTIFY_TRIPLE_CLICK	2	// �g���v���N���b�N�Œʒm

// �ʒm�R�[�h
#define	LVCN_LINK						100	// �����N�ʒm�R�[�h

/////////////////////////////////////////////////////////////////////////////
// CLogViewCtrl �E�B���h�E

class CLogViewCtrl : public CWnd
{
public:
	CLogViewCtrl();
	virtual ~CLogViewCtrl();

	DECLARE_DYNCREATE(CLogViewCtrl)

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CLogViewCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	BOOL	AddLine(LPCTSTR pszLine, COLORREF crFore = 0xffffffff, COLORREF crBack = 0xffffffff, BOOL bRedraw = TRUE);
	void	Redraw();
	void	Clear();
	void	SetSetting(const LOGVIEWCTRLSETTING* pSetting);
	void	GetSetting(LOGVIEWCTRLSETTING* pSetting);
	void	SetTextLimit(int nLimit, DWORD dwSize);

protected:
	CArray<LINEINFO, LINEINFO&>	m_aTexts;	// �s���
	int		m_nLimitMode;					// �e�L�X�g���E
	DWORD	m_dwLimitSize;					// �e�L�X�g���E�l
	DWORD	m_dwTextSize;					// ���݂̃e�L�X�g�̃T�C�Y
	CSize	m_sizeChar;						// ���p1�����̕��ƍ���
	CSize	m_sizeView;						// �\���ł���T�C�Y
	CSize	m_sizeHeader;					// �w�b�_�̕\���T�C�Y
	CFont	m_fontText;						// �t�H���g
	CFont	m_fontHeader;					// �t�H���g
	int		m_nSelect;						// �I�����(0:�I���Ȃ�/1:�����I��/2:�s�I��)
	int		m_nSelectCharStart;				// �����I���J�n�ʒu
	int		m_nSelectCharEnd;				// �����I���I���ʒu
	int		m_nSelectLineStart;				// �s�I���J�n�ʒu
	int		m_nSelectLineEnd;				// �s�I���I���ʒu
	CMenu	m_menuPopup;					// �R���e�L�X�g���j���[
	DWORD	m_dwTripleClick;				// �g���v���N���b�N����p
	CPoint	m_ptTripleClick;				// �g���v���N���b�N����p

	LOGVIEWCTRLSETTING	m_stSetting;		// �ݒ�

	void	RecalcParam(const SIZE* psizeClient = NULL);
	int		GetTextHeight(CDC* pDC, int nIndex);
	void	LimitText();
	BOOL	GetSelectPos(CPoint point, int* pnLine, int* pnChar);
	void	UpdateSelectParam(int nLine, int nChar);
	int		FindToken(int nLine, int nChar);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CLogViewCtrl)
	afx_msg HRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg void OnMenuCopy();
	afx_msg void OnMenuAllSelect();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif

/// @file ListCtrlItemInfo.h
/// @brief アイテム一覧リストコントロールクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/24
/// @copyright Copyright(C)URARA-works 2007

#pragma once

class CMgrData;
class CLibInfoItem;

// クラス宣言

typedef class CListCtrlItemInfo : public CListCtrl
{
public:
			CListCtrlItemInfo();						// コンストラクタ
	virtual ~CListCtrlItemInfo();						// デストラクタ


	BOOL	Create(CWnd *pParent, CMgrData *pMgrData);	// 作成
	void	Renew(void);								// 一覧を更新


protected:


protected:
	CWnd			*m_pWndParent;			// 親ウィンドウ
	CMgrData		*m_pMgrData;			// データ管理
	CLibInfoItem	*m_pLibInfoItem;		// アイテム情報



public:
	//{{AFX_VIRTUAL(CListCtrlItemInfo)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListCtrlItemInfo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CListCtrlItemInfo, *PCListCtrlItemInfo;

//{{AFX_INSERT_LOCATION}}

/// @file ParamUtil.h
/// @brief 区切り文字付きのパラメータ分解クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/02
/// @copyright Copyright(C)URARA-works 2007

#pragma once
class CParamUtil
{
public:
			CParamUtil();	// コンストラクタ
	virtual ~CParamUtil();	// デストラクタ

	void	Set(LPCSTR pszText, char cSeparator=' ');	// パラメータ設定
	int	GetCount(void);	// パラメータ数取得
	LPCSTR	GetParam(int nNo);	// パラメータ取得

protected:
	CmyStringArray	m_astrParam;	// 分解されたパラメータ
};


/// @file WindowSTYLESELECT.cpp
/// @brief 容姿選択ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/10
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoCharBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "WindowSEX.h"
#include "WindowHAIRTYPE.h"
#include "WindowHAIRCOLOR.h"
#include "WindowEYECOLOR.h"
#include "WindowCHARNAME.h"
#include "WindowSTYLESELECT.h"
#include "Platform/SdlFont.h"

namespace {

// 名前/性別/髪型の入力欄の幅。名前は最大15文字入るので96pxでは足りない。
const int ITEMFRAME_WIDTH = 120;

// WindowCHARNAME.cppのGetFontTextWidthCN()と同等。名前欄の確定/変換中/カーソルのX位置計算に使う。
static int GetFontTextWidthCN(HFONT hFont, LPCTSTR pszText)
{
	int nWidth, nHeight, nLen;

	if (pszText == NULL) {
		return 0;
	}
	nLen = (int)_tcslen(pszText);
	if (nLen <= 0) {
		return 0;
	}
	nWidth = 0;
	nHeight = 0;
	if (SdlFontGetTextExtent((void*)hFont, pszText, nLen, &nWidth, &nHeight)) {
		return nWidth;
	}
	return nLen * 8;
}


// 指定幅に収まるところまで文字列を切り詰めて返す（欄からはみ出して他の枠に重なるのを防ぐ）
static CString TrimTextToWidth(HFONT hFont, LPCTSTR pszText, int nMaxWidth)
{
	CString strRet;
	int i, nLen;

	if ((pszText == NULL) || (pszText[0] == 0)) {
		return strRet;
	}
	if (GetFontTextWidthCN(hFont, pszText) <= nMaxWidth) {
		strRet = pszText;
		return strRet;
	}

	nLen = (int)_tcslen(pszText);
	for (i = 1; i <= nLen; i ++) {
		CString strTry;

		strTry = CString(pszText).Left(i);
		if (GetFontTextWidthCN(hFont, (LPCTSTR)strTry) > nMaxWidth) {
			break;
		}
		strRet = strTry;
	}
	return strRet;
}

}

CWindowSTYLESELECT::CWindowSTYLESELECT()
{
	m_bAnime	= FALSE;
	m_nPos	= 0;
	m_nPosMax	= 4;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_STYLESELECT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;

	m_nDirection	= 0;
	m_nSex	= SEX_MALE;
	m_nSexBack	= SEX_MALE;
	m_wHairTypeID	= 1;
	m_wHairTypeIDBack	= 1;
	m_wHairColorID	= 1;
	m_wHairColorIDBack	= 1;
	m_wEyeColorID	= 1;
	m_wEyeColorIDBack	= 1;

	m_pMgrWindow	= NULL;
	m_pMgrDraw	= NULL;
	m_pWindowSEX	= NULL;
	m_pWindowHAIRTYPE	= NULL;
	m_pWindowHAIRCOLOR	= NULL;
	m_pWindowEYECOLOR	= NULL;
	m_pWindowCHARNAME	= NULL;

	m_pInfoCharCli = new CInfoCharCli;
}


CWindowSTYLESELECT::~CWindowSTYLESELECT()
{
	SAFE_DELETE(m_pInfoCharCli);
}


void CWindowSTYLESELECT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pMgrWindow	= m_pMgrData->GetMgrWindow();
	m_pMgrDraw	= m_pMgrData->GetMgrDraw();

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	m_pInfoCharCli->Create(m_pMgrData);
	m_pInfoCharCli->m_dwMotionTypeID = 2;
	m_pInfoCharCli->MakeCharGrp();
	m_pMgrData->GetLibInfoChar()->RenewMotionInfo(m_pInfoCharCli);

	// 名前入力の別ウィンドウ化を廃し、名前欄(座標(40,104))の位置に埋め込み表示する
	m_pMgrWindow->MakeWindowCHARNAME();
	m_pWindowCHARNAME = (PCWindowCHARNAME)m_pMgrWindow->GetWindow(WINDOWTYPE_CHARNAME);
	m_pWindowCHARNAME->SetEmbedded(m_ptViewPos.x + 32 + 40, m_ptViewPos.y + 32 + 102, ITEMFRAME_WIDTH);
	m_pMgrWindow->SetActive();	// CHARNAMEはm_bInput=FALSEなので、このタイミングで親がアクティブに戻る
	m_pWindowCHARNAME->SetActive(TRUE);	// 初期カーソルは名前欄(m_nPos==0)なのでフォーカスを与える

	PostWindowMessage(1);
}


void CWindowSTYLESELECT::SetFamilyType(WORD wFamilyID)
{
	m_pInfoCharCli->m_wFamilyID = wFamilyID;
	m_pInfoCharCli->MakeCharGrp();
}


void CWindowSTYLESELECT::SetInfoChar(CInfoCharCli *pInfoCharCli)
{
	m_pInfoCharCli->Copy(pInfoCharCli);
	m_pInfoCharCli->MakeCharGrp();

	m_nSex	= m_pInfoCharCli->m_nSex;
	m_wHairTypeID	= m_pInfoCharCli->m_wGrpIDHairType;
	m_wHairColorID	= m_pInfoCharCli->m_wGrpIDHairColor;
	m_wEyeColorID	= m_pInfoCharCli->m_wGrpIDEyeColor;
}


void CWindowSTYLESELECT::OnWindowMsg(int nType, DWORD dwPara)
{
	BOOL bMake, bAnime;

	bMake	= FALSE;
	bAnime	= FALSE;

	switch (nType) {
	case WINDOWTYPE_SEX:	// 性別
		switch (dwPara) {
		case 0:
			m_nSex = m_pWindowSEX->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_SEX);
			m_pWindowSEX = NULL;
			break;
		case (DWORD)-1:
			m_nSex = m_nSexBack;
			break;
		default:
			m_nSex = m_pWindowSEX->GetType();
			break;
		}
		m_pInfoCharCli->m_nSex = m_nSex;
		bMake = TRUE;
		break;
	case WINDOWTYPE_HAIRTYPE:	// 髪型
		switch (dwPara) {
		case 0:
			m_wHairTypeID = m_pWindowHAIRTYPE->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_HAIRTYPE);
			m_pWindowHAIRTYPE = NULL;
			m_wHairTypeIDBack = m_wHairTypeID;
			bAnime = TRUE;
			break;
		case (DWORD)-1:
			m_wHairTypeID = m_wHairTypeIDBack;
			m_pWindowHAIRTYPE = NULL;
			bAnime = TRUE;
			break;
		default:
			m_wHairTypeID = m_pWindowHAIRTYPE->GetType();
			break;
		}
		m_pInfoCharCli->m_wGrpIDHairType = m_wHairTypeID;
		bMake = TRUE;
		break;
	case WINDOWTYPE_HAIRCOLOR:	// 髪色
		switch (dwPara) {
		case 0:
			m_wHairColorID = m_pWindowHAIRCOLOR->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_HAIRCOLOR);
			m_pWindowHAIRCOLOR = NULL;
			m_wHairColorIDBack = m_wHairColorID;
			bAnime = TRUE;
			break;
		case (DWORD)-1:
			m_wHairColorID = m_wHairColorIDBack;
			m_pWindowHAIRCOLOR = NULL;
			bAnime = TRUE;
			break;
		default:
			m_wHairColorID = m_pWindowHAIRCOLOR->GetType();
			break;
		}
		m_pInfoCharCli->m_wGrpIDHairColor = m_wHairColorID;
		bMake = TRUE;
		break;
	case WINDOWTYPE_EYECOLOR:	// 目色
		switch (dwPara) {
		case 0:
			m_wEyeColorID = m_pWindowEYECOLOR->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_EYECOLOR);
			m_pWindowEYECOLOR = NULL;
			m_wEyeColorIDBack = m_wEyeColorID;
			break;
		case (DWORD)-1:
			m_wEyeColorID = m_wEyeColorIDBack;
			m_pWindowEYECOLOR = NULL;
			break;
		default:
			m_wEyeColorID = m_pWindowEYECOLOR->GetType();
			break;
		}
		m_pInfoCharCli->m_wGrpIDEyeColor = m_wEyeColorID;
		bMake = TRUE;
		break;
	case WINDOWTYPE_CHARNAME:	// キャラ名入力(dwPara==0のEnter確定時のみそのまま登録を試みる)
		if (dwPara == 0) {
			m_pInfoCharCli->m_strCharName = m_pWindowCHARNAME->m_strName;
			// 埋め込みウィンドウなのでここでは消さず、名前欄として表示させ続ける
			Redraw();
			TryRegist();
		}
		break;
	}

	if (bMake) {
		m_pInfoCharCli->MakeCharGrp();
		Redraw();
	}
	if (bAnime) {
		m_bAnime	= FALSE;
		m_nDirection	= 4;
		m_dwLastTimerProc	= 0;
		m_pInfoCharCli->m_nDirection = 1;
	}
}


void CWindowSTYLESELECT::Draw(PCImg32 pDst)
{
	int i, nCount, nDirection;
	HDC hDC;
	COLORREF clText;
	POINT nCursorPos[] = {
		40, 104,
		40, 152,
		40, 200,
//	40, 248,
		136, 292
	};
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	nCount = 3;
	DrawFrame(2);
	for (i = 0; i < nCount; i ++) {
		DrawFrame2(nCursorPos[i].x, nCursorPos[i].y, ITEMFRAME_WIDTH, 8, 2);
	}
	DrawFrame(176, 112, 100, 136, 2);

	clText	= RGB(124, 123, 232);
	hDC	= m_pDib->Lock();

	TextOut4(hDC, m_hFont14, 88,	24,	_T("新規キャラクター作成"), clText);

	TextOut4(hDC, m_hFont14, nCursorPos[0].x + 26, nCursorPos[0].y - 24, _T("名前"), clText);
	// 埋め込みCWindowCHARNAMEはcanvasに描画しない(親が後から上に描いて隠してしまうため)ので、
	// 子から確定文字列/IME変換中文字列を取得してここで描く(性別・髪型と同じ描き方)
	if (m_pWindowCHARNAME != NULL) {
		LPCTSTR pszName, pszComposition;
		CString strNameView;
		int nNameX, nX, nLimitX;

		nNameX = 40;
		nLimitX = 40 + ITEMFRAME_WIDTH;

		pszName = m_pWindowCHARNAME->GetName();
		pszComposition = m_pWindowCHARNAME->GetComposition();

		// 最大15文字は欄幅に収まらないことがあるので、欄からはみ出す分は描かずに切る
		strNameView = TrimTextToWidth(m_hFont14, pszName, ITEMFRAME_WIDTH);
		TextOut2(hDC, m_hFont14, nNameX, nCursorPos[0].y - 2, (LPCTSTR)strNameView, clText);

		nX = nNameX + GetFontTextWidthCN(m_hFont14, (LPCTSTR)strNameView);
		if (nX > nLimitX) {
			nX = nLimitX;
		}
		if ((pszComposition != NULL) && (pszComposition[0] != _T('\0'))) {
			TextOut2(hDC, m_hFont14, nX, nCursorPos[0].y - 2, pszComposition, RGB(40, 80, 180));
			nX += GetFontTextWidthCN(m_hFont14, pszComposition);
			if (nX > nLimitX) {
				nX = nLimitX;
			}
		}
		if ((m_nPos == 0) && m_pWindowCHARNAME->IsFocused() && (m_nCursorAnime == 0)) {
			TextOut2(hDC, m_hFont14, nX, nCursorPos[0].y - 2, _T("|"), clText);
		}

		// TimerProcで前回描画時との差分を見て再描画要否を判定するために控えておく
		m_strNameDrawn = pszName;
		m_strCompositionDrawn = pszComposition;
	}
	TextOut4(hDC, m_hFont14, nCursorPos[1].x + 26, nCursorPos[1].y - 24, _T("性別"), clText);
	strTmp.Empty();
	switch (m_nSex) {
	case SEX_MALE:	strTmp = "♂　オトコ";	break;
	case SEX_FEMALE:	strTmp = "♀　オンナ";	break;
	}
	TextOut2(hDC, m_hFont14, 40, nCursorPos[1].y - 2, strTmp, clText);
	TextOut4(hDC, m_hFont14, nCursorPos[2].x + 26, nCursorPos[2].y - 24, _T("髪型"), clText);
	strTmp.Format(_T("%02d"), m_wHairTypeID);
//	strTmp = m_pMgrData->GetHairTypeName(m_wHairTypeID);
	TextOut2(hDC, m_hFont14, 40, nCursorPos[2].y - 2, strTmp, clText);
#if 0
	// ※項目番号は旧番号（髪色/目色は現状未使用のため描画しない）
	TextOut4(hDC, m_hFont14, nCursorPos[2].x + 26, nCursorPos[2].y - 24, _T("髪の色"), clText);
//	strTmp.Format(_T("%02d"), m_wHairColorID);
	strTmp = m_pMgrData->GetHairColorName(m_wHairColorID);
	TextOut2(hDC, m_hFont14, 40, nCursorPos[2].y - 2, strTmp, clText);
	TextOut4(hDC, m_hFont14, nCursorPos[3].x + 26, nCursorPos[3].y - 24, _T("目の色"), clText);
//	strTmp.Format(_T("%02d"), m_wEyeColorID);
	strTmp = m_pMgrData->GetEyeColorName(m_wEyeColorID);
	TextOut2(hDC, m_hFont14, 40, nCursorPos[3].y - 2, strTmp, clText);
#endif

	TextOut4(hDC, m_hFont14, 132,	288,	_T("登録"), clText);

	m_pDib->Unlock();

	DrawCursor(nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime();

	nDirection = m_pInfoCharCli->m_nDirection;
	if ((m_nPos <= 1) || (m_nPos >= 3)) {
		// 髪型項目(=2)の時だけ回転プレビューの向きを見せる
		nDirection = 1;
	}
	m_pMgrDraw->DrawChar(m_pDib, 193, 177, m_pInfoCharCli);

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowSTYLESELECT::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime, dwWait;
	int nDirectionTbl[] = {1, 2, 0, 3};
	int nCursorAnimeBack;

	bRet = FALSE;

	// カーソル点滅(m_nCursorAnime)を進める。名前欄のカーソル点滅・埋め込みCHARNAMEの
	// 入力内容の反映はDraw()側で行うため、ここで変化を検出したら再描画させる
	nCursorAnimeBack = m_nCursorAnime;
	CWindowBase::TimerProc();
	if (nCursorAnimeBack != m_nCursorAnime) {
		m_dwTimeDrawStart = 0;
		bRet = TRUE;
	}
	if ((m_pWindowCHARNAME != NULL) &&
		((m_strNameDrawn != m_pWindowCHARNAME->GetName()) || (m_strCompositionDrawn != m_pWindowCHARNAME->GetComposition()))) {
		m_dwTimeDrawStart = 0;
		bRet = TRUE;
	}

	if (m_bAnime == FALSE) {
		goto Exit;
	}

	dwWait = 1500;
	if (m_nDirection == 0) {
		dwWait = 3000;
	}

	dwTime = timeGetTime();
	if (dwTime - m_dwLastTimerProc < dwWait) {
		goto Exit;
	}
	m_dwLastTimerProc = dwTime;
	m_nDirection ++;
	m_nDirection = (m_nDirection >= 4) ? 0 : m_nDirection;
	m_pInfoCharCli->m_nDirection = nDirectionTbl[m_nDirection];

	m_dwTimeDrawStart = 0;
	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	if (m_nPos == 2) {
		// 髪型項目に移った時だけ向き回転アニメを開始する
		m_nDirection	= 4;
		m_dwLastTimerProc	= 0;
		m_pInfoCharCli->m_nDirection = 1;
	}
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	if (m_pWindowCHARNAME != NULL) {
		m_pWindowCHARNAME->SetActive(m_nPos == 0);
	}
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 1) {
		goto Exit;
	}
	m_nPos ++;
	if (m_nPos == 2) {
		// 髪型項目に移った時だけ向き回転アニメを開始する
		m_nDirection	= 4;
		m_dwLastTimerProc	= 0;
		m_pInfoCharCli->m_nDirection = 1;
	}
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	if (m_pWindowCHARNAME != NULL) {
		m_pWindowCHARNAME->SetActive(m_nPos == 0);
	}
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	if (m_pWindowCHARNAME != NULL) {
		m_pWindowCHARNAME->SetActive(m_nPos == 0);
	}
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowSTYLESELECT::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	if (m_pWindowCHARNAME != NULL) {
		m_pWindowCHARNAME->SetActive(m_nPos == 0);
	}
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowSTYLESELECT::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);

	switch (m_nPos) {
	case 0:	// 名前
		// 別ウィンドウを開かず既に埋め込みで入力可能なので何もしない
		break;
	case 1:	// 性別
		m_pMgrWindow->MakeWindowSEX();
		m_pWindowSEX = (PCWindowSEX)m_pMgrWindow->GetWindow(WINDOWTYPE_SEX);
		m_pWindowSEX->SetType(m_nSex);
		break;
	case 2:	// 髪型
		m_pMgrWindow->MakeWindowHAIRTYPE();
		m_pWindowHAIRTYPE = (PCWindowHAIRTYPE)m_pMgrWindow->GetWindow(WINDOWTYPE_HAIRTYPE);
		m_pWindowHAIRTYPE->SetType(m_wHairTypeID);
		m_bAnime = TRUE;
		break;
#if 0
	// ※項目番号は旧番号（髪色/目色は現状未使用）
	case 2:	// 髪色
		m_pMgrWindow->MakeWindowHAIRCOLOR();
		m_pWindowHAIRCOLOR = (PCWindowHAIRCOLOR)m_pMgrWindow->GetWindow(WINDOWTYPE_HAIRCOLOR);
		m_pWindowHAIRCOLOR->SetType(m_wHairColorID);
		m_bAnime = TRUE;
		break;
	case 3:	// 目色
		m_pMgrWindow->MakeWindowEYECOLOR();
		m_pWindowEYECOLOR = (PCWindowEYECOLOR)m_pMgrWindow->GetWindow(WINDOWTYPE_EYECOLOR);
		m_pWindowEYECOLOR->SetType(m_wEyeColorID);
		break;
#endif
	case 3:	// 登録
		TryRegist();
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::TryRegist(void)
{
	BOOL bResult;
	CLibInfoCharBase LibInfoChar;

	if (m_pWindowCHARNAME != NULL) {
		// 埋め込み入力欄の内容は Enter 確定時にしか流れてこないので、
		// 「名前を打った後そのまま登録項目へ移動した」場合に取りこぼさないよう毎回引き取る
		m_pInfoCharCli->m_strCharName = m_pWindowCHARNAME->m_strName;
	}
	if (m_pInfoCharCli->m_strCharName.IsEmpty()) {
		m_pMgrWindow->MakeWindowMSG("名前を入力してください", 3000);
		return FALSE;
	}
	bResult = LibInfoChar.NameCheck(m_pInfoCharCli->m_strCharName);
	if (bResult == FALSE) {
		m_pMgrWindow->MakeWindowMSG("名前に空白は使えません", 3000);
		return FALSE;
	}
	PostWindowMessage(0);
	return TRUE;
}


BOOL CWindowSTYLESELECT::HandleSDLKeyDown(UINT vk)
{
	if ((m_nPos == 0) && (m_pWindowCHARNAME != NULL)) {
		// 名前欄フォーカス中はCHARNAMEへ転送する(親はCMgrWindowのアクティブ判定を保持したまま)
		return m_pWindowCHARNAME->HandleSDLKeyDown(vk);
	}
	return CWindowBase::HandleSDLKeyDown(vk);
}


void CWindowSTYLESELECT::HandleSDLTextInput(LPCSTR pszText)
{
	if ((m_nPos == 0) && (m_pWindowCHARNAME != NULL)) {
		m_pWindowCHARNAME->HandleSDLTextInput(pszText);
		return;
	}
	CWindowBase::HandleSDLTextInput(pszText);
}


void CWindowSTYLESELECT::HandleSDLTextEditing(LPCSTR pszText)
{
	if ((m_nPos == 0) && (m_pWindowCHARNAME != NULL)) {
		m_pWindowCHARNAME->HandleSDLTextEditing(pszText);
		return;
	}
	CWindowBase::HandleSDLTextEditing(pszText);
}


BOOL CWindowSTYLESELECT::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_CANCEL);
	if (m_pWindowCHARNAME != NULL) {
		// 埋め込みの子ウィンドウを残さないよう画面終了時に確実に片付ける
		m_pMgrWindow->Delete(WINDOWTYPE_CHARNAME);
		m_pWindowCHARNAME = NULL;
	}
	PostWindowMessage(-1);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}

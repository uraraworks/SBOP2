/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:TextInput.h												 */
/* 内容			:テキスト入力クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/05/16													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <stdio.h>
#include "TextInput.h"


/* ========================================================================= */
/* 関数名	:CTextInput::CTextInput											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

CTextInput::CTextInput()
{
	m_dwLine		= 0;
	m_pszFileName	= NULL;
	m_hFile			= INVALID_HANDLE_VALUE;
}


/* ========================================================================= */
/* 関数名	:CTextInput::~CTextInput										 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

CTextInput::~CTextInput()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CTextInput::Create												 */
/* 内容		:初期化															 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

BOOL CTextInput::Create(
	LPCSTR pszFileName)		/* [in] 読み込むファイル名 */
{
	BOOL bRet, bResult;

	bRet = FALSE;

	if (m_pszFileName) {
		goto Exit;
	}

	bResult = Open (pszFileName);
	if (bResult == FALSE) {
		goto Exit;
	}

	m_dwLine = 0;
	m_pszFileName = new char[strlen (pszFileName) + 1];
	strcpy (m_pszFileName, pszFileName);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CTextInput::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

void CTextInput::Destroy(void)
{
	if (m_pszFileName) {
		Close ();
		delete [] m_pszFileName;
		m_pszFileName = NULL;
	}
}


/* ========================================================================= */
/* 関数名	:CTextInput::SetLine											 */
/* 内容		:読み込む行を設定												 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

void CTextInput::SetLine(DWORD dwLine)
{
	DWORD i;
	CmyString strTmp;

	m_dwLine = 0;
	SetFilePointer (m_hFile, 0, NULL, FILE_BEGIN);

	for (i = 0; i < dwLine; i ++) {
		strTmp = ReadLine ();
		if (strTmp.IsEmpty ()) {
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CTextInput::ReadLine											 */
/* 内容		:指定行から1行読み込み											 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

CmyString CTextInput::ReadLine(DWORD dwLine)
{
	SetLine (dwLine);
	return ReadProc ();
}


/* ========================================================================= */
/* 関数名	:CTextInput::ReadLine											 */
/* 内容		:現在位置から1行読み込み										 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

CmyString CTextInput::ReadLine(void)
{
	return ReadProc ();
}


/* ========================================================================= */
/* 関数名	:CTextInput::GetLineCount										 */
/* 内容		:行数を取得														 */
/* 日付		:2006/05/16														 */
/* 備考		:この関数を呼ぶと現在位置は先頭に戻ってしまう。					 */
/* ========================================================================= */

DWORD CTextInput::GetLineCount(void)
{
	DWORD dwRet;

	SetLine (-1);
	dwRet = m_dwLine;
	SetLine (0);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CTextInput::Open												 */
/* 内容		:ファイルを開く													 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

BOOL CTextInput::Open(LPCSTR pszFileName)
{
	BOOL bRet;
	HANDLE hFile;

	bRet = FALSE;

	/* ファイルを開く */
	hFile = CreateFile (
			pszFileName,
			GENERIC_READ,
			0,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}

	m_hFile	= hFile;
	bRet	= TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CTextInput::Close												 */
/* 内容		:ファイルを閉じる												 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

void CTextInput::Close(void)
{
	if (m_hFile == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle (m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}


/* ========================================================================= */
/* 関数名	:CTextInput::ReadProc											 */
/* 内容		:読み込み処理													 */
/* 日付		:2006/05/16														 */
/* 備考		:CSV的に1行分読み込み											 */
/* ========================================================================= */

CmyString CTextInput::ReadProc(void)
{
	char szTmp[2];
	BOOL bResult;
	BYTE byTmp1, byTmp2;
	DWORD dwSize;
	CmyString strTmp, strRet;

	szTmp[1] = 0;

	/* CSV形式として1行読み込むまでのループ */
	while (1) {
		strTmp.Empty ();
		/* 改行コードか終端までのループ */
		while (1) {
			bResult = ReadFile (m_hFile, &byTmp1, 1, &dwSize, NULL);
			if ((bResult == FALSE) || (dwSize == 0)) {
				break;
			}
			if (byTmp1 == 0x0D) {
				bResult = ReadFile (m_hFile, &byTmp2, 1, &dwSize, NULL);
				if (bResult == FALSE) {
					break;
				}
				if (byTmp2 == 0x0A) {
					break;
				}
				/* 「\r\n」をセットで追加 */
				szTmp[0] = byTmp1;
				strTmp += szTmp;
				szTmp[0] = byTmp2;
				strTmp += szTmp;

			} else {
				szTmp[0] = byTmp1;
				strTmp += szTmp;
			}
		}
		strRet += strTmp;

		bResult = IsCSVCheck (strRet);
		/* CSV形式の1行として正しい？ */
		if (bResult) {
			if (strRet.IsEmpty () == FALSE) {
				m_dwLine ++;
			}
			break;
		}
		/* 読み込めなかった？ */
		if (strTmp.IsEmpty ()) {
			strRet.Empty ();
			break;
		}
	}

	return strRet;
}


/* ========================================================================= */
/* 関数名	:CTextInput::IsCSVCheck											 */
/* 内容		:CSV形式文字列として正しいか判定								 */
/* 日付		:2006/05/16														 */
/* ========================================================================= */

BOOL CTextInput::IsCSVCheck(LPCSTR pszText)
{
	int i, nLen, nCountSection;
	BOOL bRet, bSection, bEnclose;
	LPCSTR pszTextTmp;

	bRet = FALSE;

	nLen			= strlen (pszText);	/* 文字列長 */
	nCountSection	= 0;				/* セクション内の位置 */
	bSection		= TRUE;				/* セクション内にいるか判定 */
	bEnclose		= FALSE;			/* 「"」で囲まれ中か判定 */
	pszTextTmp		= pszText;			/* 作業中の文字列ポインタ */

	for (i = 0; i < nLen; i ++) {
		pszTextTmp = &pszText[i];
		/* 全角文字？ */
		if (IsDBCSLeadByte (*pszTextTmp)) {
			i ++;
			nCountSection ++;
			continue;
		}

		switch (*pszTextTmp) {
		case ',':		/* 区切り文字？ */
			/* 「"」の中にいる？ */
			if (bEnclose) {
				break;
			}
			/* 脱出 */
			bEnclose		= FALSE;
			nCountSection	= 0;
			continue;

		case '"':		/* 文字列囲み */
			/* 「"」の中にいる？ */
			if (bEnclose) {
				/* 次の文字も「"」？ */
				if ((i != nLen - 1) && (*(pszTextTmp + 1) == '"')) {
					i ++;
					nCountSection ++;
					continue;
				}
				/* 脱出 */
				bEnclose = FALSE;
				continue;

			} else {
				/* 先頭？ */
				if (nCountSection == 0) {
					/* 突入 */
					bEnclose = TRUE;
					continue;
				}
			}
			break;

		case '\r':
			/* 次の文字が「\n」？ */
			if ((i != nLen - 1) && (*(pszTextTmp + 1) == '\n')) {
				/* 「"」の中にはいない？ */
				if (bEnclose == FALSE) {
					/* 終了 */
					i = nLen;
					bSection = FALSE;
					continue;
				}
				i ++;
				nCountSection ++;
				continue;
			}
			break;
		}
		bSection = TRUE;
		nCountSection ++;
	}

	/* 「"」の中にはいない？ */
	if (bEnclose == FALSE) {
		bRet = TRUE;
	}

	return bRet;
}

/* Copyright(C)URARA-works 2006 */

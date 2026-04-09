/// @file TextInput.h
/// @brief テキスト入力クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/05/16
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include <stdio.h>
#include "TextInput.h"

CTextInput::CTextInput()
{
	m_dwLine	= 0;
	m_pszFileName	= NULL;
	m_pFile	= NULL;
}

CTextInput::~CTextInput()
{
	Destroy();
}

BOOL CTextInput::Create(
	LPCSTR pszFileName)	// [in] 読み込むファイル名
{
	BOOL bRet, bResult;

	bRet = FALSE;

	if (m_pszFileName) {
		goto Exit;
	}

	bResult = Open(pszFileName);
	if (bResult == FALSE) {
		goto Exit;
	}

	m_dwLine = 0;
	m_pszFileName = new char[strlen(pszFileName) + 1];
	strcpy(m_pszFileName, pszFileName);

	bRet = TRUE;
Exit:
	return bRet;
}

void CTextInput::Destroy(void)
{
	if (m_pszFileName) {
		Close();
		delete [] m_pszFileName;
		m_pszFileName = NULL;
	}
}

void CTextInput::SetLine(DWORD dwLine)
{
	DWORD i;
	CmyString strTmp;

	// ファイル未オープン時は何もしない（Create 失敗を許容するため）
	if (m_pFile == NULL) {
		return;
	}

	m_dwLine = 0;
	fseek(m_pFile, 0, SEEK_SET);

	for (i = 0; i < dwLine; i ++) {
		strTmp = ReadLine();
		if (strTmp.IsEmpty()) {
			break;
		}
	}
}

CmyString CTextInput::ReadLine(DWORD dwLine)
{
	SetLine(dwLine);
	return ReadProc();
}

CmyString CTextInput::ReadLine(void)
{
	return ReadProc();
}

DWORD CTextInput::GetLineCount(void)
{
	DWORD dwRet;

	SetLine(-1);
	dwRet = m_dwLine;
	SetLine(0);

	return dwRet;
}

BOOL CTextInput::Open(LPCSTR pszFileName)
{
	BOOL bRet;

	bRet = FALSE;

	// ファイルをバイナリ読み込みモードで開く
	m_pFile = fopen(pszFileName, "rb");
	if (m_pFile == NULL) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

void CTextInput::Close(void)
{
	if (m_pFile == NULL) {
		return;
	}

	fclose(m_pFile);
	m_pFile = NULL;
}

CmyString CTextInput::ReadProc(void)
{
	char szTmp[2];
	BOOL bResult;
	BYTE byTmp1, byTmp2;
	DWORD dwSize;
	CmyString strTmp, strRet;

	// ファイル未オープン時は空文字列を返す（Create 失敗を許容するため）
	if (m_pFile == NULL) {
		return strRet;
	}

	szTmp[1] = 0;

	// CSV形式として1行読み込むまでのループ
	while (1) {
		strTmp.Empty();
		// 改行コードか終端までのループ
		while (1) {
			// 1バイト読み込む（戻り値0で読み終わり）
			dwSize = (DWORD)fread(&byTmp1, 1, 1, m_pFile);
			if (dwSize == 0) {
				break;
			}
			if (byTmp1 == 0x0D) {
				// 次の1バイトを読み込む
				dwSize = (DWORD)fread(&byTmp2, 1, 1, m_pFile);
				if (dwSize == 0) {
					break;
				}
				if (byTmp2 == 0x0A) {
					break;
				}
				// 「\r\n」をセットで追加
				szTmp[0] = byTmp1;
				strTmp += (LPCSTR)szTmp;
				szTmp[0] = byTmp2;
				strTmp += (LPCSTR)szTmp;

			} else {
				szTmp[0] = byTmp1;
				strTmp += (LPCSTR)szTmp;
			}
		}
		strRet += (LPCSTR)strTmp;

		bResult = IsCSVCheck(strRet);
		// CSV形式の1行として正しい？
		if (bResult) {
			if (strRet.IsEmpty() == FALSE) {
				m_dwLine ++;
			}
			break;
		}
		// 読み込めなかった？
		if (strTmp.IsEmpty()) {
			strRet.Empty();
			break;
		}
	}

	return strRet;
}

BOOL CTextInput::IsCSVCheck(LPCSTR pszText)
{
	int i, nLen, nCountSection;
	BOOL bRet, bSection, bEnclose;
	LPCSTR pszTextTmp;

	bRet = FALSE;

	nLen	= strlen(pszText);	// 文字列長
	nCountSection	= 0;	// セクション内の位置
	bSection	= TRUE;	// セクション内にいるか判定
	bEnclose	= FALSE;	// 「"」で囲まれ中か判定
	pszTextTmp	= pszText;	// 作業中の文字列ポインタ

	for (i = 0; i < nLen; i ++) {
		pszTextTmp = &pszText[i];
		// 全角文字？
		if (IsDBCSLeadByte(*pszTextTmp)) {
			i ++;
			nCountSection ++;
			continue;
		}

		switch (*pszTextTmp) {
		case ',':	// 区切り文字？
			// 「"」の中にいる？
			if (bEnclose) {
				break;
			}
			// 脱出
			bEnclose	= FALSE;
			nCountSection	= 0;
			continue;

		case '"':	// 文字列囲み
			// 「"」の中にいる？
			if (bEnclose) {
				// 次の文字も「"」？
				if ((i != nLen - 1) && (*(pszTextTmp + 1) == '"')) {
					i ++;
					nCountSection ++;
					continue;
				}
				// 脱出
				bEnclose = FALSE;
				continue;

			} else {
				// 先頭？
				if (nCountSection == 0) {
					// 突入
					bEnclose = TRUE;
					continue;
				}
			}
			break;

		case '\r':
			// 次の文字が「\n」？
			if ((i != nLen - 1) && (*(pszTextTmp + 1) == '\n')) {
				// 「"」の中にはいない？
				if (bEnclose == FALSE) {
					// 終了
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

	// 「"」の中にはいない？
	if (bEnclose == FALSE) {
		bRet = TRUE;
	}

	return bRet;
}


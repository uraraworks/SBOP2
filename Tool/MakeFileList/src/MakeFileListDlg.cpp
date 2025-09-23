/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MakeFileListDlg.cpp										 */
/* 内容			:メインダイアログクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "GetMD5File.h"
#include "MakeFileList.h"
#include "MakeFileListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* CMakeFileListDlgクラスの設定												 */
/* ========================================================================= */

void CMakeFileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeFileListDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMakeFileListDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeFileListDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CMakeFileListDlg::CMakeFileListDlg								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/08/26														 */
/* ========================================================================= */

CMakeFileListDlg::CMakeFileListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeFileListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeFileListDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


/* ========================================================================= */
/* 関数名	:CMakeFileListDlg::OnInitDialog									 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/08/26														 */
/* ========================================================================= */

BOOL CMakeFileListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

	ReadIniFile ();
	MakeHashList ();

	EndDialog (IDCANCEL);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CMakeFileListDlg::ReadIniFile									 */
/* 内容		:iniファイル読み込み											 */
/* 日付		:2007/08/26														 */
/* ========================================================================= */

void CMakeFileListDlg::ReadIniFile(void)
{
	int i;
	char szFileName[MAX_PATH], szTmp[MAX_PATH];
	CString strTmp;

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	/* ファイルリスト読み込み */
	for (i = 0; ; i ++) {
		strTmp.Format ("%d", i + 1);
		ZeroMemory (szTmp, sizeof (szTmp));
		GetPrivateProfileString ("FileList", strTmp, "", szTmp, sizeof (szTmp) - 1, szFileName);
		if (strlen (szTmp) <= 0) {
			break;
		}
		m_astrFileList.push_back (szTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CMakeFileListDlg::MakeHashList									 */
/* 内容		:ファイルハッシュリスト作成										 */
/* 日付		:2007/08/26														 */
/* ========================================================================= */

void CMakeFileListDlg::MakeHashList(void)
{
	int i, nCount;
	char szPath[MAX_PATH], szHash[33];
	DWORD dwFileSize;
	HANDLE hFile;
	CString strFileName, strTmp;
	CGetMD5File GetMD5;
	CStdioFile destFile;
	CStdArray<DWORD, DWORD> adwFileSize;

	GetModuleFilePath (szPath, sizeof (szPath));

	nCount = m_astrFileList.size();
	for (i = 0; i < nCount; i ++) {
		strFileName.Format ("%s%s", szPath, m_astrFileList[i]);

		GetMD5.Init ();
		GetMD5.Update (strFileName);
		ZeroMemory (szHash, sizeof (szHash));
		GetMD5.GetStr (szHash);
		if (strlen (szHash) <= 0) {
			continue;
		}
		m_astrHashList.push_back (szHash);

		dwFileSize = 0;
		hFile = CreateFile (strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			dwFileSize = GetFileSize (hFile, NULL);
			CloseHandle (hFile);
		}
		adwFileSize.push_back (dwFileSize);
	}

	/* ハッシュリストを保存 */
	strFileName.Format ("%sSBOHashList.txt", szPath);
	destFile.Open (strFileName, CFile::modeWrite | CFile::modeCreate, NULL);

	nCount = m_astrFileList.size();
	for (i = 0; i < nCount; i ++) {
		strTmp.Format ("%s,%u,%s\n", m_astrHashList[i], adwFileSize[i], m_astrFileList[i]);
		destFile.WriteString (strTmp);
	}
	destFile.Close ();
}

/* Copyright(C)URARA-works 2007 */

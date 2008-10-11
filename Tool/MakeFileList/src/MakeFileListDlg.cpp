/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MakeFileListDlg.cpp										 */
/* ���e			:���C���_�C�A���O�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/26													 */
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
/* CMakeFileListDlg�N���X�̐ݒ�												 */
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
/* �֐���	:CMakeFileListDlg::CMakeFileListDlg								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/26														 */
/* ========================================================================= */

CMakeFileListDlg::CMakeFileListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeFileListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeFileListDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


/* ========================================================================= */
/* �֐���	:CMakeFileListDlg::OnInitDialog									 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/08/26														 */
/* ========================================================================= */

BOOL CMakeFileListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�

	ReadIniFile ();
	MakeHashList ();

	EndDialog (IDCANCEL);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CMakeFileListDlg::ReadIniFile									 */
/* ���e		:ini�t�@�C���ǂݍ���											 */
/* ���t		:2007/08/26														 */
/* ========================================================================= */

void CMakeFileListDlg::ReadIniFile(void)
{
	int i;
	char szFileName[MAX_PATH], szTmp[MAX_PATH];
	CString strTmp;

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	/* �t�@�C�����X�g�ǂݍ��� */
	for (i = 0; ; i ++) {
		strTmp.Format ("%d", i + 1);
		ZeroMemory (szTmp, sizeof (szTmp));
		GetPrivateProfileString ("FileList", strTmp, "", szTmp, sizeof (szTmp) - 1, szFileName);
		if (strlen (szTmp) <= 0) {
			break;
		}
		m_astrFileList.Add (szTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CMakeFileListDlg::MakeHashList									 */
/* ���e		:�t�@�C���n�b�V�����X�g�쐬										 */
/* ���t		:2007/08/26														 */
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
	CArray <DWORD, DWORD> adwFileSize;

	GetModuleFilePath (szPath, sizeof (szPath));

	nCount = m_astrFileList.GetSize ();
	for (i = 0; i < nCount; i ++) {
		strFileName.Format ("%s%s", szPath, m_astrFileList[i]);

		GetMD5.Init ();
		GetMD5.Update (strFileName);
		ZeroMemory (szHash, sizeof (szHash));
		GetMD5.GetStr (szHash);
		if (strlen (szHash) <= 0) {
			continue;
		}
		m_astrHashList.Add (szHash);

		dwFileSize = 0;
		hFile = CreateFile (strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			dwFileSize = GetFileSize (hFile, NULL);
			CloseHandle (hFile);
		}
		adwFileSize.Add (dwFileSize);
	}

	/* �n�b�V�����X�g��ۑ� */
	strFileName.Format ("%sSBOHashList.txt", szPath);
	destFile.Open (strFileName, CFile::modeWrite | CFile::modeCreate, NULL);

	nCount = m_astrFileList.GetSize ();
	for (i = 0; i < nCount; i ++) {
		strTmp.Format ("%s,%u,%s\n", m_astrHashList[i], adwFileSize[i], m_astrFileList[i]);
		destFile.WriteString (strTmp);
	}
	destFile.Close ();
}

/* Copyright(C)URARA-works 2007 */

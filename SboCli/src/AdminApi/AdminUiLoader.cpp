#include "stdafx.h"

#include "AdminUiLoader.h"
#include "MgrData.h"
#include "PacketBase.h"
#include "UraraSockTCPSBO.h"
#include "InfoTalkEvent.h"

namespace
{
	void WriteAdminUiLoaderDebug(LPCSTR pszText)
	{
		OutputDebugStringA (pszText);
	}

	BOOL IsValidModule(const SboAdminUiModule* pModule)
	{
		if (pModule == NULL) {
			WriteAdminUiLoaderDebug ("[AdminUiLoader] module is null\n");
			return FALSE;
		}
		if ((pModule->cb < sizeof(SboAdminUiModule)) || (pModule->apiVersion != SBO_ADMINUI_API_VERSION_1)) {
			WriteAdminUiLoaderDebug ("[AdminUiLoader] module header mismatch\n");
			return FALSE;
		}
		if (!(pModule->Create && pModule->Destroy && pModule->Notify && pModule->GetWindow)) {
			WriteAdminUiLoaderDebug ("[AdminUiLoader] module function table incomplete\n");
			return FALSE;
		}
		return TRUE;
	}
}

CAdminUiLoader::CAdminUiLoader()
{
	ZeroMemory(&m_Host, sizeof(m_Host));
	ZeroMemory(&m_Module, sizeof(m_Module));
	m_hModule = NULL;
	m_pDestroyFactory = NULL;
	m_pMgrData = NULL;
	m_bLoadedFromDll = FALSE;
}

CAdminUiLoader::~CAdminUiLoader()
{
	Destroy();
}

BOOL CAdminUiLoader::Create(HWND hWndParent, CMgrData* pMgrData)
{
	SboCreateAdminUiModuleProc pCreateFactory;
	BOOL bCreated;
	TCHAR szExePath[MAX_PATH];
	TCHAR szExeDir[MAX_PATH];
	TCHAR szCandidatePath[MAX_PATH];
	LPCTSTR apszCandidates[] = {
		_T("SboCliAdminMfc.dll"),
		_T(".\\SboCliAdminMfc.dll"),
		_T(".\\SboCli\\Debug\\SboCliAdminMfc.dll"),
		_T(".\\SboCli\\Release\\SboCliAdminMfc.dll"),
		NULL
	};
	int i;

	Destroy();
	if (pMgrData == NULL) {
		return FALSE;
	}

	m_pMgrData = pMgrData;
	m_Host.cb = sizeof(m_Host);
	m_Host.apiVersion = SBO_ADMINUI_API_VERSION_1;
	m_Host.userData = this;
	m_Host.GetMainWindow = HostGetMainWindow;
	m_Host.GetMgrData = HostGetMgrData;
	m_Host.SetAdminWindow = HostSetAdminWindow;
	/* DLL が AdminWindow を自前管理するため、LocalAdminUi 系 Host API は不要 */
	m_Host.CreateLocalAdminUi = NULL;
	m_Host.DestroyLocalAdminUi = NULL;
	m_Host.NotifyLocalAdminUi = NULL;
	m_Host.GetLocalAdminUiWindow = NULL;
	m_Host.SendAdminPacket = HostSendAdminPacket;
	m_Host.SetAdminNotifyTypes = HostSetAdminNotifyTypes;
	m_Host.GetMoveNoBlock = HostGetMoveNoBlock;
	m_Host.SetMoveNoBlock = HostSetMoveNoBlock;
	m_Host.SetViewGrid = HostSetViewGrid;
	m_Host.GetMapData = HostGetMapData;
	m_Host.GetWndMapSize = HostGetWndMapSize;
	m_Host.SetWndMapSize = HostSetWndMapSize;
	m_Host.GetSelectMapPartsID = HostGetSelectMapPartsID;
	m_Host.SetSelectMapPartsID = HostSetSelectMapPartsID;
	m_Host.GetInfoTalkEvent = HostGetInfoTalkEvent;

	m_hModule = NULL;

	for (i = 0; apszCandidates[i] != NULL; i ++) {
		m_hModule = LoadLibrary (apszCandidates[i]);
		if (m_hModule) {
			WriteAdminUiLoaderDebug ("[AdminUiLoader] loaded admin ui dll from candidate path\n");
			break;
		}
	}

	if (m_hModule == NULL) {
		szExePath[0] = 0;
		szExeDir[0] = 0;
		if (GetModuleFileName (NULL, szExePath, _countof (szExePath)) > 0) {
			_tcscpy_s (szExeDir, _countof (szExeDir), szExePath);
			for (i = (int)_tcslen (szExeDir) - 1; i >= 0; i --) {
				if ((szExeDir[i] == _T('\\')) || (szExeDir[i] == _T('/'))) {
					szExeDir[i] = 0;
					break;
				}
			}
			if (szExeDir[0]) {
				_stprintf_s (szCandidatePath, _countof (szCandidatePath), _T("%s\\SboCliAdminMfc.dll"), szExeDir);
				m_hModule = LoadLibrary (szCandidatePath);
				if (m_hModule) {
					WriteAdminUiLoaderDebug ("[AdminUiLoader] loaded admin ui dll from exe directory\n");
				}
			}
		}
	}

	if (m_hModule == NULL) {
		/* DLL が存在しない場合は管理者UI なしで動作（非管理者プレイ継続） */
		WriteAdminUiLoaderDebug ("[AdminUiLoader] failed to load admin ui dll\n");
		return FALSE;
	}

	pCreateFactory = (SboCreateAdminUiModuleProc)GetProcAddress(m_hModule, "CreateAdminUiModule");
	if (pCreateFactory == NULL) {
		pCreateFactory = (SboCreateAdminUiModuleProc)GetProcAddress(m_hModule, "_CreateAdminUiModule@8");
	}
	m_pDestroyFactory = (SboDestroyAdminUiModuleProc)GetProcAddress(m_hModule, "DestroyAdminUiModule");
	if (m_pDestroyFactory == NULL) {
		m_pDestroyFactory = (SboDestroyAdminUiModuleProc)GetProcAddress(m_hModule, "_DestroyAdminUiModule@4");
	}
	if (pCreateFactory == NULL) {
		WriteAdminUiLoaderDebug ("[AdminUiLoader] CreateAdminUiModule export not found\n");
	}
	if (m_pDestroyFactory == NULL) {
		WriteAdminUiLoaderDebug ("[AdminUiLoader] DestroyAdminUiModule export not found\n");
	}

	if (pCreateFactory && pCreateFactory(&m_Host, &m_Module) && IsValidModule(&m_Module)) {
		bCreated = m_Module.Create(m_Module.context, &m_Host, hWndParent);
		if (bCreated) {
			m_bLoadedFromDll = TRUE;
			WriteAdminUiLoaderDebug ("[AdminUiLoader] admin ui created successfully\n");
			return TRUE;
		}
		WriteAdminUiLoaderDebug ("[AdminUiLoader] module Create returned FALSE\n");
	} else {
		WriteAdminUiLoaderDebug ("[AdminUiLoader] factory creation failed\n");
	}

	if (m_pDestroyFactory) {
		m_pDestroyFactory(&m_Module);
	}
	ZeroMemory(&m_Module, sizeof(m_Module));
	m_pDestroyFactory = NULL;
	FreeLibrary(m_hModule);
	m_hModule = NULL;
	return FALSE;
}

void CAdminUiLoader::Destroy(void)
{
	if (m_Module.Destroy) {
		m_Module.Destroy(m_Module.context);
	}

	if (m_bLoadedFromDll) {
		if (m_pDestroyFactory) {
			m_pDestroyFactory(&m_Module);
		}
	}

	if (m_hModule) {
		FreeLibrary(m_hModule);
	}

	ZeroMemory(&m_Module, sizeof(m_Module));
	ZeroMemory(&m_Host, sizeof(m_Host));
	m_hModule = NULL;
	m_pDestroyFactory = NULL;
	m_bLoadedFromDll = FALSE;
	m_pMgrData = NULL;
}

void CAdminUiLoader::Notify(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_Module.Notify) {
		m_Module.Notify(m_Module.context, message, wParam, lParam);
	}
}

void CAdminUiLoader::Show(void)
{
	if (m_Module.Show) {
		m_Module.Show(m_Module.context);
	}
}

void CAdminUiLoader::Hide(void)
{
	if (m_Module.Hide) {
		m_Module.Hide(m_Module.context);
	}
}

HWND CAdminUiLoader::GetWindow(void) const
{
	if (m_Module.GetWindow) {
		return m_Module.GetWindow(m_Module.context);
	}
	return NULL;
}

BOOL CAdminUiLoader::IsLoadedFromDll(void) const
{
	return m_bLoadedFromDll;
}

HWND __stdcall CAdminUiLoader::HostGetMainWindow(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return NULL;
	}
	return pLoader->m_pMgrData->GetMainWindow();
}

void* __stdcall CAdminUiLoader::HostGetMgrData(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if (pLoader == NULL) {
		return NULL;
	}
	return pLoader->m_pMgrData;
}

void __stdcall CAdminUiLoader::HostSetAdminWindow(void* userData, HWND hWndAdmin)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return;
	}
	pLoader->m_pMgrData->SetAdminWindow(hWndAdmin);
}

BOOL __stdcall CAdminUiLoader::HostSendAdminPacket(void* userData, void* pPacket)
{
	CAdminUiLoader* pLoader;
	CPacketBase* pPacketBase;

	pLoader = (CAdminUiLoader*)userData;
	pPacketBase = (CPacketBase*)pPacket;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL) || (pPacketBase == NULL)) {
		return FALSE;
	}
	if (pLoader->m_pMgrData->GetUraraSockTCP() == NULL) {
		return FALSE;
	}
	pLoader->m_pMgrData->GetUraraSockTCP()->Send(pPacketBase);
	return TRUE;
}

void __stdcall CAdminUiLoader::HostSetAdminNotifyTypes(void* userData, int nTypeL, int nTypeR, int nTypeRR)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return;
	}
	pLoader->m_pMgrData->SetAdminNotifyTypeL(nTypeL);
	pLoader->m_pMgrData->SetAdminNotifyTypeR(nTypeR);
	pLoader->m_pMgrData->SetAdminNotifyTypeRR(nTypeRR);
}

BOOL __stdcall CAdminUiLoader::HostGetMoveNoBlock(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return FALSE;
	}
	return pLoader->m_pMgrData->GetMoveNoBlock();
}

void __stdcall CAdminUiLoader::HostSetMoveNoBlock(void* userData, BOOL bMoveNoBlock)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return;
	}
	pLoader->m_pMgrData->SetMoveNoBlock(bMoveNoBlock);
}

void __stdcall CAdminUiLoader::HostSetViewGrid(void* userData, int nViewGrid)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return;
	}
	pLoader->m_pMgrData->SetViewGrid((BYTE)nViewGrid);
}

void* __stdcall CAdminUiLoader::HostGetMapData(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return NULL;
	}
	return pLoader->m_pMgrData->GetMap();
}

void __stdcall CAdminUiLoader::HostGetWndMapSize(void* userData, int* pCx, int* pCy)
{
	CAdminUiLoader* pLoader;
	SIZE sz;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		if (pCx) { *pCx = 0; }
		if (pCy) { *pCy = 0; }
		return;
	}
	sz = pLoader->m_pMgrData->GetWndMap();
	if (pCx) { *pCx = sz.cx; }
	if (pCy) { *pCy = sz.cy; }
}

void __stdcall CAdminUiLoader::HostSetWndMapSize(void* userData, int cx, int cy)
{
	CAdminUiLoader* pLoader;
	SIZE sz;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return;
	}
	sz.cx = cx;
	sz.cy = cy;
	pLoader->m_pMgrData->SetWndMap(sz);
}

DWORD __stdcall CAdminUiLoader::HostGetSelectMapPartsID(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return 0;
	}
	return pLoader->m_pMgrData->GetSelectMapPartsID();
}

void __stdcall CAdminUiLoader::HostSetSelectMapPartsID(void* userData, DWORD dwPartsID)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return;
	}
	pLoader->m_pMgrData->SetSelectMapPartsID(dwPartsID);
}

void* __stdcall CAdminUiLoader::HostGetInfoTalkEvent(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if ((pLoader == NULL) || (pLoader->m_pMgrData == NULL)) {
		return NULL;
	}
	return pLoader->m_pMgrData->GetInfoTalkEvent();
}

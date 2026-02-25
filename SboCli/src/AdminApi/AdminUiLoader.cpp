#include "stdafx.h"

#include "AdminUiLoader.h"
#include "MgrData.h"
#include "AdminWindow.h"
#include "PacketBase.h"
#include "UraraSockTCPSBO.h"

namespace
{
	BOOL __stdcall FallbackCreate(void* context, const SboAdminUiHost* host, HWND hWndParent)
	{
		UNREFERENCED_PARAMETER(context);

		if ((host == NULL) || (host->CreateLocalAdminUi == NULL)) {
			return FALSE;
		}
		return host->CreateLocalAdminUi(host->userData, hWndParent);
	}

	void __stdcall FallbackDestroy(void* context)
	{
		SboAdminUiHost* pHost;

		pHost = (SboAdminUiHost*)context;
		if (pHost && pHost->DestroyLocalAdminUi) {
			pHost->DestroyLocalAdminUi(pHost->userData);
		}
	}

	void __stdcall FallbackShow(void* context)
	{
		UNREFERENCED_PARAMETER(context);
	}

	void __stdcall FallbackHide(void* context)
	{
		UNREFERENCED_PARAMETER(context);
	}

	void __stdcall FallbackNotify(void* context, UINT message, WPARAM wParam, LPARAM lParam)
	{
		SboAdminUiHost* pHost;

		pHost = (SboAdminUiHost*)context;
		if (pHost && pHost->NotifyLocalAdminUi) {
			pHost->NotifyLocalAdminUi(pHost->userData, message, wParam, lParam);
		}
	}

	HWND __stdcall FallbackGetWindow(void* context)
	{
		SboAdminUiHost* pHost;

		pHost = (SboAdminUiHost*)context;
		if ((pHost == NULL) || (pHost->GetLocalAdminUiWindow == NULL)) {
			return NULL;
		}
		return pHost->GetLocalAdminUiWindow(pHost->userData);
	}

	BOOL IsValidModule(const SboAdminUiModule* pModule)
	{
		if (pModule == NULL) {
			return FALSE;
		}
		if ((pModule->cb < sizeof(SboAdminUiModule)) || (pModule->apiVersion != SBO_ADMINUI_API_VERSION_1)) {
			return FALSE;
		}
		return (pModule->Create && pModule->Destroy && pModule->Notify && pModule->GetWindow);
	}

	void CreateFallbackModule(SboAdminUiHost* pHost, SboAdminUiModule* pModule)
	{
		ZeroMemory(pModule, sizeof(*pModule));
		pModule->cb = sizeof(*pModule);
		pModule->apiVersion = SBO_ADMINUI_API_VERSION_1;
		pModule->context = pHost;
		pModule->Create = FallbackCreate;
		pModule->Destroy = FallbackDestroy;
		pModule->Show = FallbackShow;
		pModule->Hide = FallbackHide;
		pModule->Notify = FallbackNotify;
		pModule->GetWindow = FallbackGetWindow;
	}
}

CAdminUiLoader::CAdminUiLoader()
{
	ZeroMemory(&m_Host, sizeof(m_Host));
	ZeroMemory(&m_Module, sizeof(m_Module));
	m_hModule = NULL;
	m_pDestroyFactory = NULL;
	m_pMgrData = NULL;
	m_pLocalAdminWindow = NULL;
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
	m_Host.CreateLocalAdminUi = HostCreateLocalAdminUi;
	m_Host.DestroyLocalAdminUi = HostDestroyLocalAdminUi;
	m_Host.NotifyLocalAdminUi = HostNotifyLocalAdminUi;
	m_Host.GetLocalAdminUiWindow = HostGetLocalAdminUiWindow;
	m_Host.SendAdminPacket = HostSendAdminPacket;
	m_Host.SetAdminNotifyTypes = HostSetAdminNotifyTypes;
	m_Host.GetMoveNoBlock = HostGetMoveNoBlock;
	m_Host.SetMoveNoBlock = HostSetMoveNoBlock;
	m_Host.SetViewGrid = HostSetViewGrid;

	m_hModule = LoadLibrary(_T("SboCliAdminMfc.dll"));
	if (m_hModule) {
		pCreateFactory = (SboCreateAdminUiModuleProc)GetProcAddress(m_hModule, "CreateAdminUiModule");
		m_pDestroyFactory = (SboDestroyAdminUiModuleProc)GetProcAddress(m_hModule, "DestroyAdminUiModule");

		if (pCreateFactory && pCreateFactory(&m_Host, &m_Module) && IsValidModule(&m_Module)) {
			bCreated = m_Module.Create(m_Module.context, &m_Host, hWndParent);
			if (bCreated) {
				m_bLoadedFromDll = TRUE;
				return TRUE;
			}
		}

		if (m_pDestroyFactory) {
			m_pDestroyFactory(&m_Module);
		}
		ZeroMemory(&m_Module, sizeof(m_Module));
		m_pDestroyFactory = NULL;
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

	CreateFallbackModule(&m_Host, &m_Module);
	if (m_Module.Create(m_Module.context, &m_Host, hWndParent) == FALSE) {
		ZeroMemory(&m_Module, sizeof(m_Module));
		return FALSE;
	}
	m_bLoadedFromDll = FALSE;
	return TRUE;
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

	DestroyLocalAdminUiInternal();

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

BOOL __stdcall CAdminUiLoader::HostCreateLocalAdminUi(void* userData, HWND hWndParent)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if (pLoader == NULL) {
		return FALSE;
	}
	return pLoader->CreateLocalAdminUiInternal(hWndParent);
}

void __stdcall CAdminUiLoader::HostDestroyLocalAdminUi(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if (pLoader) {
		pLoader->DestroyLocalAdminUiInternal();
	}
}

void __stdcall CAdminUiLoader::HostNotifyLocalAdminUi(void* userData, UINT message, WPARAM wParam, LPARAM lParam)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if (pLoader) {
		pLoader->NotifyLocalAdminUiInternal(message, wParam, lParam);
	}
}

HWND __stdcall CAdminUiLoader::HostGetLocalAdminUiWindow(void* userData)
{
	CAdminUiLoader* pLoader;

	pLoader = (CAdminUiLoader*)userData;
	if (pLoader == NULL) {
		return NULL;
	}
	return pLoader->GetLocalAdminUiWindowInternal();
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

BOOL CAdminUiLoader::CreateLocalAdminUiInternal(HWND hWndParent)
{
	if ((m_pMgrData == NULL) || (m_pLocalAdminWindow != NULL)) {
		return (m_pLocalAdminWindow != NULL);
	}

	m_pLocalAdminWindow = new CAdminWindow;
	if (m_pLocalAdminWindow == NULL) {
		return FALSE;
	}
	if (m_pLocalAdminWindow->Create(hWndParent, m_pMgrData, &m_Host) == FALSE) {
		delete m_pLocalAdminWindow;
		m_pLocalAdminWindow = NULL;
		return FALSE;
	}
	m_pMgrData->SetAdminWindow(m_pLocalAdminWindow->GetSafeHwnd());
	return TRUE;
}

void CAdminUiLoader::DestroyLocalAdminUiInternal(void)
{
	if (m_pLocalAdminWindow) {
		m_pLocalAdminWindow->Destroy();
		delete m_pLocalAdminWindow;
		m_pLocalAdminWindow = NULL;
	}
	if (m_pMgrData) {
		m_pMgrData->SetAdminWindow(NULL);
	}
}

void CAdminUiLoader::NotifyLocalAdminUiInternal(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndAdmin;

	hWndAdmin = GetLocalAdminUiWindowInternal();
	if (hWndAdmin && ::IsWindow(hWndAdmin)) {
		m_pLocalAdminWindow->PostMessage(message, wParam, lParam);
	}
}

HWND CAdminUiLoader::GetLocalAdminUiWindowInternal(void) const
{
	HWND hWndAdmin;

	if (m_pLocalAdminWindow == NULL) {
		return NULL;
	}
	hWndAdmin = m_pLocalAdminWindow->GetSafeHwnd();
	if ((hWndAdmin == NULL) || (::IsWindow(hWndAdmin) == FALSE)) {
		return NULL;
	}
	return hWndAdmin;
}

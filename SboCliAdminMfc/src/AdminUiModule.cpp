#include "StdAfx.h"

#include "../include/AdminUiApi.h"
#include "AdminWindow.h"

namespace
{
	void WriteAdminUiModuleDebug(LPCSTR pszText)
	{
		OutputDebugStringA(pszText);
	}

	struct SDllAdminUiContext
	{
		SboAdminUiHost host;
		CAdminWindow* pAdminWindow;
	};

	BOOL __stdcall DllCreate(void* context, const SboAdminUiHost* host, HWND hWndParent)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		SDllAdminUiContext* pCtx;
		CMgrData* pMgrData;

		pCtx = (SDllAdminUiContext*)context;
		if (pCtx == NULL) {
			WriteAdminUiModuleDebug("[AdminUiModule] context is null\n");
			return FALSE;
		}
		if (pCtx->pAdminWindow != NULL) {
			WriteAdminUiModuleDebug("[AdminUiModule] admin window already exists\n");
			return TRUE;
		}

		// Host から CMgrData を取得
		pMgrData = NULL;
		if (pCtx->host.GetMgrData) {
			pMgrData = (CMgrData*)pCtx->host.GetMgrData(pCtx->host.userData);
		}

		pCtx->pAdminWindow = new CAdminWindow;
		if (pCtx->pAdminWindow == NULL) {
			WriteAdminUiModuleDebug("[AdminUiModule] failed to allocate admin window\n");
			return FALSE;
		}
		if (pCtx->pAdminWindow->Create(hWndParent, pMgrData, &pCtx->host) == FALSE) {
			WriteAdminUiModuleDebug("[AdminUiModule] CAdminWindow::Create returned FALSE\n");
			delete pCtx->pAdminWindow;
			pCtx->pAdminWindow = NULL;
			return FALSE;
		}
		WriteAdminUiModuleDebug("[AdminUiModule] admin window created\n");

		// ホスト側に AdminWindow の HWND を通知
		if (pCtx->host.SetAdminWindow) {
			pCtx->host.SetAdminWindow(pCtx->host.userData, pCtx->pAdminWindow->GetSafeHwnd());
		}
		return TRUE;
	}

	void __stdcall DllDestroy(void* context)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		SDllAdminUiContext* pCtx;

		pCtx = (SDllAdminUiContext*)context;
	if ((pCtx == NULL) || (pCtx->pAdminWindow == NULL)) {
		return;
	}
	pCtx->pAdminWindow->Destroy();
	pCtx->pAdminWindow = NULL;

		// ホスト側の AdminWindow 参照をクリア
		if (pCtx->host.SetAdminWindow) {
			pCtx->host.SetAdminWindow(pCtx->host.userData, NULL);
		}
	}

	void __stdcall DllShow(void* context)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		SDllAdminUiContext* pCtx;

		pCtx = (SDllAdminUiContext*)context;
		if (pCtx && pCtx->pAdminWindow) {
			pCtx->pAdminWindow->Show();
		}
	}

	void __stdcall DllHide(void* context)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		SDllAdminUiContext* pCtx;

		pCtx = (SDllAdminUiContext*)context;
		if (pCtx && pCtx->pAdminWindow) {
			pCtx->pAdminWindow->Hide();
		}
	}

	void __stdcall DllNotify(void* context, UINT message, WPARAM wParam, LPARAM lParam)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		SDllAdminUiContext* pCtx;
		HWND hWndAdmin;

		pCtx = (SDllAdminUiContext*)context;
		if ((pCtx == NULL) || (pCtx->pAdminWindow == NULL)) {
			return;
		}
		hWndAdmin = pCtx->pAdminWindow->GetSafeHwnd();
		if (hWndAdmin && ::IsWindow(hWndAdmin)) {
			pCtx->pAdminWindow->PostMessage(message, wParam, lParam);
		}
	}

	HWND __stdcall DllGetWindow(void* context)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		SDllAdminUiContext* pCtx;
		HWND hWndAdmin;

		pCtx = (SDllAdminUiContext*)context;
		if ((pCtx == NULL) || (pCtx->pAdminWindow == NULL)) {
			return NULL;
		}
		hWndAdmin = pCtx->pAdminWindow->GetSafeHwnd();
		if ((hWndAdmin == NULL) || (::IsWindow(hWndAdmin) == FALSE)) {
			return NULL;
		}
		return hWndAdmin;
	}
}

extern "C" __declspec(dllexport) BOOL __stdcall CreateAdminUiModule(const SboAdminUiHost* host, SboAdminUiModule* outModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SDllAdminUiContext* pCtx;

	if ((host == NULL) || (outModule == NULL)) {
		WriteAdminUiModuleDebug("[AdminUiModule] invalid CreateAdminUiModule args\n");
		return FALSE;
	}
	if ((host->cb < sizeof(SboAdminUiHost)) || (host->apiVersion != SBO_ADMINUI_API_VERSION_1)) {
		WriteAdminUiModuleDebug("[AdminUiModule] host header mismatch\n");
		return FALSE;
	}

	pCtx = new SDllAdminUiContext;
	if (pCtx == NULL) {
		WriteAdminUiModuleDebug("[AdminUiModule] failed to allocate module context\n");
		return FALSE;
	}
	ZeroMemory(pCtx, sizeof(*pCtx));
	pCtx->host = *host;
	pCtx->pAdminWindow = NULL;

	ZeroMemory(outModule, sizeof(*outModule));
	outModule->cb = sizeof(*outModule);
	outModule->apiVersion = SBO_ADMINUI_API_VERSION_1;
	outModule->context = pCtx;
	outModule->Create = DllCreate;
	outModule->Destroy = DllDestroy;
	outModule->Show = DllShow;
	outModule->Hide = DllHide;
	outModule->Notify = DllNotify;
	outModule->GetWindow = DllGetWindow;
	WriteAdminUiModuleDebug("[AdminUiModule] module exported successfully\n");
	return TRUE;
}

extern "C" __declspec(dllexport) void __stdcall DestroyAdminUiModule(SboAdminUiModule* module)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SDllAdminUiContext* pCtx;

	if (module == NULL) {
		return;
	}
	pCtx = (SDllAdminUiContext*)module->context;
	if (pCtx) {
		pCtx->pAdminWindow = NULL;
		delete pCtx;
	}
	ZeroMemory(module, sizeof(*module));
}

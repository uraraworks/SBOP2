#include "StdAfx.h"

#include "../include/AdminUiApi.h"

namespace
{
	struct SDllAdminUiContext
	{
		SboAdminUiHost host;
		BOOL bCreated;
	};

	BOOL __stdcall DllCreate(void* context, const SboAdminUiHost* host, HWND hWndParent)
	{
		SDllAdminUiContext* pCtx;

		UNREFERENCED_PARAMETER(host);
		pCtx = (SDllAdminUiContext*)context;
		if ((pCtx == NULL) || (pCtx->host.CreateLocalAdminUi == NULL)) {
			return FALSE;
		}
		pCtx->bCreated = pCtx->host.CreateLocalAdminUi(pCtx->host.userData, hWndParent);
		return pCtx->bCreated;
	}

	void __stdcall DllDestroy(void* context)
	{
		SDllAdminUiContext* pCtx;

		pCtx = (SDllAdminUiContext*)context;
		if ((pCtx == NULL) || (pCtx->host.DestroyLocalAdminUi == NULL)) {
			return;
		}
		if (pCtx->bCreated) {
			pCtx->host.DestroyLocalAdminUi(pCtx->host.userData);
			pCtx->bCreated = FALSE;
		}
	}

	void __stdcall DllShow(void* context)
	{
		UNREFERENCED_PARAMETER(context);
	}

	void __stdcall DllHide(void* context)
	{
		UNREFERENCED_PARAMETER(context);
	}

	void __stdcall DllNotify(void* context, UINT message, WPARAM wParam, LPARAM lParam)
	{
		SDllAdminUiContext* pCtx;

		pCtx = (SDllAdminUiContext*)context;
		if ((pCtx == NULL) || (pCtx->host.NotifyLocalAdminUi == NULL)) {
			return;
		}
		pCtx->host.NotifyLocalAdminUi(pCtx->host.userData, message, wParam, lParam);
	}

	HWND __stdcall DllGetWindow(void* context)
	{
		SDllAdminUiContext* pCtx;

		pCtx = (SDllAdminUiContext*)context;
		if ((pCtx == NULL) || (pCtx->host.GetLocalAdminUiWindow == NULL)) {
			return NULL;
		}
		return pCtx->host.GetLocalAdminUiWindow(pCtx->host.userData);
	}
}

extern "C" __declspec(dllexport) BOOL __stdcall CreateAdminUiModule(const SboAdminUiHost* host, SboAdminUiModule* outModule)
{
	SDllAdminUiContext* pCtx;

	if ((host == NULL) || (outModule == NULL)) {
		return FALSE;
	}
	if ((host->cb < sizeof(SboAdminUiHost)) || (host->apiVersion != SBO_ADMINUI_API_VERSION_1)) {
		return FALSE;
	}
	if ((host->CreateLocalAdminUi == NULL) || (host->DestroyLocalAdminUi == NULL) ||
		(host->NotifyLocalAdminUi == NULL) || (host->GetLocalAdminUiWindow == NULL)) {
		return FALSE;
	}

	pCtx = new SDllAdminUiContext;
	if (pCtx == NULL) {
		return FALSE;
	}
	ZeroMemory(pCtx, sizeof(*pCtx));
	pCtx->host = *host;
	pCtx->bCreated = FALSE;

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
	return TRUE;
}

extern "C" __declspec(dllexport) void __stdcall DestroyAdminUiModule(SboAdminUiModule* module)
{
	SDllAdminUiContext* pCtx;

	if (module == NULL) {
		return;
	}
	pCtx = (SDllAdminUiContext*)module->context;
	if (pCtx) {
		if (pCtx->bCreated && pCtx->host.DestroyLocalAdminUi) {
			pCtx->host.DestroyLocalAdminUi(pCtx->host.userData);
		}
		delete pCtx;
	}
	ZeroMemory(module, sizeof(*module));
}

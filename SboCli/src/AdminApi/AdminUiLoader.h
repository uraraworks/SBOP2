#pragma once

#include "AdminUiApi.h"

class CMgrData;
class CAdminWindow;

class CAdminUiLoader
{
public:
	CAdminUiLoader();
	~CAdminUiLoader();

	BOOL Create(HWND hWndParent, CMgrData* pMgrData);
	void Destroy(void);
	void Notify(UINT message, WPARAM wParam, LPARAM lParam);
	void Show(void);
	void Hide(void);
	HWND GetWindow(void) const;
	BOOL IsLoadedFromDll(void) const;

private:
	static HWND __stdcall HostGetMainWindow(void* userData);
	static void* __stdcall HostGetMgrData(void* userData);
	static void __stdcall HostSetAdminWindow(void* userData, HWND hWndAdmin);
	static BOOL __stdcall HostCreateLocalAdminUi(void* userData, HWND hWndParent);
	static void __stdcall HostDestroyLocalAdminUi(void* userData);
	static void __stdcall HostNotifyLocalAdminUi(void* userData, UINT message, WPARAM wParam, LPARAM lParam);
	static HWND __stdcall HostGetLocalAdminUiWindow(void* userData);
	static BOOL __stdcall HostSendAdminPacket(void* userData, void* pPacket);
	static void __stdcall HostSetAdminNotifyTypes(void* userData, int nTypeL, int nTypeR, int nTypeRR);
	static BOOL __stdcall HostGetMoveNoBlock(void* userData);
	static void __stdcall HostSetMoveNoBlock(void* userData, BOOL bMoveNoBlock);
	static void __stdcall HostSetViewGrid(void* userData, int nViewGrid);

	BOOL CreateLocalAdminUiInternal(HWND hWndParent);
	void DestroyLocalAdminUiInternal(void);
	void NotifyLocalAdminUiInternal(UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetLocalAdminUiWindowInternal(void) const;

private:
	SboAdminUiHost m_Host;
	SboAdminUiModule m_Module;
	HMODULE m_hModule;
	SboDestroyAdminUiModuleProc m_pDestroyFactory;
	CMgrData* m_pMgrData;
	CAdminWindow* m_pLocalAdminWindow;
	BOOL m_bLoadedFromDll;
};

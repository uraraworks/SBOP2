#pragma once

/* Admin UI module ABI (Phase 1) */

enum
{
	SBO_ADMINUI_API_VERSION_1 = 1
};

typedef struct tagSboAdminUiHost
{
	unsigned int cb;
	unsigned int apiVersion;
	void* userData;
	HWND (__stdcall *GetMainWindow)(void* userData);
	void* (__stdcall *GetMgrData)(void* userData);
	void (__stdcall *SetAdminWindow)(void* userData, HWND hWndAdmin);
	BOOL (__stdcall *CreateLocalAdminUi)(void* userData, HWND hWndParent);
	void (__stdcall *DestroyLocalAdminUi)(void* userData);
	void (__stdcall *NotifyLocalAdminUi)(void* userData, UINT message, WPARAM wParam, LPARAM lParam);
	HWND (__stdcall *GetLocalAdminUiWindow)(void* userData);
	BOOL (__stdcall *SendAdminPacket)(void* userData, void* pPacket);
	void (__stdcall *SetAdminNotifyTypes)(void* userData, int nTypeL, int nTypeR, int nTypeRR);
	BOOL (__stdcall *GetMoveNoBlock)(void* userData);
	void (__stdcall *SetMoveNoBlock)(void* userData, BOOL bMoveNoBlock);
	void (__stdcall *SetViewGrid)(void* userData, int nViewGrid);
	/* WndMap 用 Host API */
	void* (__stdcall *GetMapData)(void* userData);
	void  (__stdcall *GetWndMapSize)(void* userData, int* pCx, int* pCy);
	void  (__stdcall *SetWndMapSize)(void* userData, int cx, int cy);
	DWORD (__stdcall *GetSelectMapPartsID)(void* userData);
	void  (__stdcall *SetSelectMapPartsID)(void* userData, DWORD dwPartsID);
	/* TalkEvent 取得（戻り値は CInfoTalkEvent* を void* として渡す） */
	void* (__stdcall *GetInfoTalkEvent)(void* userData);
} SboAdminUiHost;

typedef BOOL (__stdcall *SboAdminUiCreateProc)(void* context, const SboAdminUiHost* host, HWND hWndParent);
typedef void (__stdcall *SboAdminUiDestroyProc)(void* context);
typedef void (__stdcall *SboAdminUiShowProc)(void* context);
typedef void (__stdcall *SboAdminUiHideProc)(void* context);
typedef void (__stdcall *SboAdminUiNotifyProc)(void* context, UINT message, WPARAM wParam, LPARAM lParam);
typedef HWND (__stdcall *SboAdminUiGetWindowProc)(void* context);

typedef struct tagSboAdminUiModule
{
	unsigned int cb;
	unsigned int apiVersion;
	void* context;
	SboAdminUiCreateProc Create;
	SboAdminUiDestroyProc Destroy;
	SboAdminUiShowProc Show;
	SboAdminUiHideProc Hide;
	SboAdminUiNotifyProc Notify;
	SboAdminUiGetWindowProc GetWindow;
} SboAdminUiModule;

typedef BOOL (__stdcall *SboCreateAdminUiModuleProc)(const SboAdminUiHost* host, SboAdminUiModule* outModule);
typedef void (__stdcall *SboDestroyAdminUiModuleProc)(SboAdminUiModule* module);

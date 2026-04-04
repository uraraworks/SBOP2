/// @file DInputUtil.h
/// @brief ジョイスティックユーティリティクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/30
/// @copyright Copyright(C)URARA-works 2007

#pragma once
#include <vector>

// 定数の定義

#define BUTTON_NULL		0x00000000
#define BUTTON_UP		0x00000001
#define BUTTON_DOWN		0x00000002
#define BUTTON_LEFT		0x00000004
#define BUTTON_RIGHT	0x00000008
#define BUTTON_BUTTON1	0x00000010
#define BUTTON_BUTTON2	0x00000020


// 構造体の定義

// デバイス情報
typedef struct _DINPUTDEVICEINFO {
	GUID		guidInstance;
	CmyString	strName;
} DINPUTDEVICEINFO, *PDINPUTDEVICEINFO;


// クラス宣言

// SDL_Joystick の前方宣言（SDL.h を直接インクルードしないため）
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

typedef class CDInputUtil
{
public:
			CDInputUtil();								// コンストラクタ
	virtual ~CDInputUtil();							// デストラクタ

	BOOL Create(void);								// 作成
	void Destroy(void);								// 破棄

	BOOL	IsUseDevice(void);						// デバイスを使用しているか判定
	DWORD	GetKeyState(void);						// 入力状態を取得
	void	SetDevice(int nNo);						// 使用デバイスを設定
	void	SetDevice(GUID &stSrc);					// 使用デバイスを設定
	BOOL	Enum(void);								// デバイスを列挙
	int		GetDeviceCount(void);					// デバイス数を取得
	BOOL	GetDeviceName(int nNo, CmyString &strDst);	// デバイス名を取得
	BOOL	GetGUID(int nNo, GUID &stDst);			// 指定デバイスのGUIDを取得


protected:
	void	DeleteDeviceInfo(int nNo);					// デバイス情報を削除
	void	DeleteAllDeviceInfo(void);					// デバイス情報を全て削除
	void	AddDeviceInfo(PDINPUTDEVICEINFO pDeviceInfo);	// デバイス情報を追加


protected:
	SDL_Joystick				*m_pJoystick;		// 使用中のジョイスティック
	std::vector<PDINPUTDEVICEINFO>	m_aDeviceInfo;	// デバイス情報
} CDInputUtil, *PCDInputUtil;

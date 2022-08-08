
//
// 入力処理 [input.cpp]
// Author : 
//

#include "input.h"
#include "debugproc.h"

// マクロ定義
#define	NUM_KEY_MAX			(256)



// プロトタイプ宣言

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);



// グローバル変数
// 
//keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ


// 入力処理の初期化
HRESULT InitInput(HINSTANCE hInst, HWND hWnd){
	HRESULT hr;

	if(!g_pDInput){
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	return S_OK;
}


// 入力処理の終了処理
void UninitInput(void){
	// キーボードの終了処理
	UninitKeyboard();

	if(g_pDInput){
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}


// 入力処理の更新処理
void UpdateInput(void){
	// キーボードの更新
	UpdateKeyboard();
	
}


// キーボードの初期化
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd){
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL){
		MessageBox(hWnd, "キーボードを接続してください。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr)){
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr)){
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}


// キーボードの終了処理
void UninitKeyboard(void){
	if(g_pDIDevKeyboard){
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}


// キーボードの更新
HRESULT UpdateKeyboard(void){
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr)){
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++){
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if(g_keyState[cnt]){
				g_keyStateRepeatCnt[cnt]++;
				if(g_keyStateRepeatCnt[cnt] >= 20){
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else{
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}


// キーボードのプレス状態を取得
bool GetKeyboardPress(int key){
	return (g_keyState[key] & 0x80) ? true : false;
}


// キーボードのトリガー状態を取得
bool GetKeyboardTrigger(int key){
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}


// キーボードのリピート状態を取得
bool GetKeyboardRepeat(int key){
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}


// キーボードのリリ－ス状態を取得
bool GetKeyboardRelease(int key){
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}





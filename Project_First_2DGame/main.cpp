#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "text.h"
#include "input.h"
#include "sound.h"

#include "game.h"
#include "title.h"
#include "result.h"
#include "menue.h"
//マクロ定義
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "my_fast_DX11_game"

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);



//グローバル変数
static float g_bgm_volume = 0.05;
static float g_ce_volume = 0.05;
static SCORE g_score;

//デバッグ用
#ifdef _DEBUG
int g_CountFPS;
char g_DebugStr[2048] = WINDOW_NAME;

#endif

int g_Mode = MODE_RESULT;
bool g_is_menue = FALSE;


//プロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}






//メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);	//無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		//無くても良いけど、警告が出る（未使用宣言）

	//時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;


	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	HWND hWnd;
	MSG msg;

	//ウィンドウクラスの登場
	RegisterClassEx(&wcex);

	//ウィンドウの作成
	hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									//ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// ウィンドウモードかフルスクリーンモードかの処理
	BOOL mode = TRUE;

	//int id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);
	//switch (id)
	//{
	//case IDYES:		// YesならWindowモードで起動
	//	mode = TRUE;
	//	break;
	//case IDNO:		// Noならフルスクリーンモードで起動
	//	mode = FALSE;	// 環境によって動かない事がある
	//	break;
	//case IDCANCEL:	// CANCELなら終了
	//default:
	//	return -1;
	//	break;
	//}

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);	//分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	//システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	//ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ(イベントドリブン型ループ)
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{//PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				//メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	//1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				//FPSを測定した時刻を保存
				dwFrameCount = 0;							//カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	//1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	//処理した時刻を保存

#ifdef _DEBUG	//デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif

				Update();			//更新処理
				Draw();				//描画処理


				dwFrameCount++;
			}
		}
	}
	timeEndPeriod(1);				//分解能を戻す

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//終了処理
	Uninit();

	return (int)msg.wParam;
}



//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {

	//描画の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//カメラの初期化
	InitCamera();

	//ライトを有効化
	SetLightEnable(FALSE);

	//テキストの初期化
	Init_text();

	//入力の初期化
	InitInput(hInstance,hWnd);

	//サウンドの初期化
	InitSound(hWnd);

	SetMode(MODE_RESULT);

	Set_bgmVolume(g_bgm_volume);
	Set_seVolume(g_ce_volume);

	SCORE* temp;
	temp = Get_score();
	temp->time = 0;
	temp->del_enemy = 0;

	return S_OK;
}

//終了処理
void Uninit(void) {
	//カメラの終了処理
	UninitCamera();

	//レンダラーの終了処理
	UninitRenderer();

	//テキストの終了処理
	Uninit_text();

	//入力の終了処理
	UninitInput();
}

//更新処理
void Update(void) {

	//カメラ更新
	UpdateCamera();

	//入力の初期化
	UpdateInput();
	switch (g_Mode) {
	case (MODE_TITLE):
		Update_title();
		break;
	case (MODE_GAME):
		Update_game();
		break;
	case (MODE_RESULT):
		Update_result();
		break;
	case (MODE_MENUE):
		Update_menue();
		break;
	default:
		break;
	}


}
//描画処理
void Draw(void) {

	//バックバッファクリア
	Clear();

	SetCamera();

	//2Dの物を描画する処理
	SetViewPort(TYPE_FULL_SCREEN);

	//Z比較なし
	SetDepthEnable(FALSE);

	//ライティングを無効
	SetLightEnable(FALSE);

	switch (g_Mode){
	case (MODE_TITLE):
		Draw_title();
		break;
	case (MODE_GAME):
		Draw_game();
		break;
	case (MODE_RESULT):
		Draw_result();
		break;
	case (MODE_MENUE):
		Draw_menue();
		break;
	default:
		break;
	}
	Draw_text();

#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	//バックバッファ、フロントバッファ入れ替え
	Present();
}


void SetMode(int mode) {
	Uninit_title();
	Uninit_game();
	Uninit_result();
	Uninit_menue();


	if (mode != MODE_MENUE) {
		StopSound();
	}
	switch (mode){
	case (MODE_TITLE):
		Init_title();
		PlaySound(SOUND_LABEL_BGM_title00);
		break;
	case (MODE_GAME):
		Init_game();
		PlaySound(SOUND_LABEL_BGM_main00);
		break;
	case (MODE_RESULT):
		Init_result();
		break;
	case(MODE_MENUE):
		Init_menue();
		break;
	default:
		PostQuitMessage(WM_QUIT);
		break;
	}
	g_Mode = mode;
}
int GetMode(void) {
	return g_Mode;
}

int GetFPS(void) {
	return g_CountFPS;
}

void Set_bgmVolume(float vol) {
	vol = clamp(vol, 0.0f, 1.0f);
	Sound_Volume(SOUND_LABEL_BGM_title00, vol);
	Sound_Volume(SOUND_LABEL_BGM_main00, vol);
}
void Set_seVolume(float vol) {
	g_ce_volume = clamp(vol, 0.0f, 1.0f);
}

float Get_bgmVolume(void) {
	return g_bgm_volume;
}
float Get_seVolume(void) {
	return g_ce_volume;
}
SCORE* Get_score() {
	return &g_score;
}

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
//�}�N����`
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "my_fast_DX11_game"

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);



//�O���[�o���ϐ�
static float g_bgm_volume = 0.05;
static float g_ce_volume = 0.05;
static SCORE g_score;

//�f�o�b�O�p
#ifdef _DEBUG
int g_CountFPS;
char g_DebugStr[2048] = WINDOW_NAME;

#endif

int g_Mode = MODE_RESULT;
bool g_is_menue = FALSE;


//�v���V�[�W��
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






//���C���֐�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);	//�����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		//�����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	//���Ԍv���p
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

	//�E�B���h�E�N���X�̓o��
	RegisterClassEx(&wcex);

	//�E�B���h�E�̍쐬
	hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									//�E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//�E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// �E�B���h�E���[�h���t���X�N���[�����[�h���̏���
	BOOL mode = TRUE;

	//int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);
	//switch (id)
	//{
	//case IDYES:		// Yes�Ȃ�Window���[�h�ŋN��
	//	mode = TRUE;
	//	break;
	//case IDNO:		// No�Ȃ�t���X�N���[�����[�h�ŋN��
	//	mode = FALSE;	// ���ɂ���ē����Ȃ���������
	//	break;
	//case IDCANCEL:	// CANCEL�Ȃ�I��
	//default:
	//	return -1;
	//	break;
	//}

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);	//����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	//�V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	//�E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v(�C�x���g�h���u���^���[�v)
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{//PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				//���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	//1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							//�J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	//1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	//��������������ۑ�

#ifdef _DEBUG	//�f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif

				Update();			//�X�V����
				Draw();				//�`�揈��


				dwFrameCount++;
			}
		}
	}
	timeEndPeriod(1);				//����\��߂�

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//�I������
	Uninit();

	return (int)msg.wParam;
}



//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {

	//�`��̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	//�J�����̏�����
	InitCamera();

	//���C�g��L����
	SetLightEnable(FALSE);

	//�e�L�X�g�̏�����
	Init_text();

	//���͂̏�����
	InitInput(hInstance,hWnd);

	//�T�E���h�̏�����
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

//�I������
void Uninit(void) {
	//�J�����̏I������
	UninitCamera();

	//�����_���[�̏I������
	UninitRenderer();

	//�e�L�X�g�̏I������
	Uninit_text();

	//���͂̏I������
	UninitInput();
}

//�X�V����
void Update(void) {

	//�J�����X�V
	UpdateCamera();

	//���͂̏�����
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
//�`�揈��
void Draw(void) {

	//�o�b�N�o�b�t�@�N���A
	Clear();

	SetCamera();

	//2D�̕���`�悷�鏈��
	SetViewPort(TYPE_FULL_SCREEN);

	//Z��r�Ȃ�
	SetDepthEnable(FALSE);

	//���C�e�B���O�𖳌�
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
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
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


#include "main.h"
#include "sound.h"
#include "input.h"
#include "text.h"
#include "game.h"

#include "title_hud.h"


//マクロ定義 
#define WIDTH (SCREEN_WIDTH)	// 背景サイズ
#define HEIGHT (SCREEN_HEIGHT)	// 
#define LAYER_MAX (1)				// テクスチャの数



enum  {
	SELECT_START,
	SELECT_LOAD,
	SELECT_OPTION,
	SELECT_EXIT,
	SELECT_MAX,
};

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// テクスチャ情報
static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

static int g_select;
static TEXT g_text[SELECT_MAX];
static TEXT g_temp_text;


HRESULT Init_title_hud(void) {

	g_select = 0; //今どこを選択しているか

	for (int i = 0; i < SELECT_MAX; i++) {
		g_text[i].pos = XMFLOAT2(WIDTH / 2, 220+(i*80));
		g_text[i].size = 40;
		g_text[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	}
	g_temp_text = g_text[g_select];
	g_Load = TRUE;

	return S_OK;
}
void Uninit_title_hud(void) {
	if (g_Load) {

	}

}
void Update_title_hud(void) {
	g_text[g_select] = g_temp_text;
	int temp1 = g_select;
	if (GetKeyboardTrigger(DIK_W)) {
		g_select--;
	}
	else if (GetKeyboardTrigger(DIK_S)) {
		g_select++;

	}
	if (GetKeyboardTrigger(DIK_A)) {

	}
	else if (GetKeyboardTrigger(DIK_D)) {

	}
	int temp2 = g_select;
	g_select = clamp(g_select, 0, SELECT_EXIT);
	if (g_select != temp2) PlaySound(SOUND_LABEL_SE_select_no);	//選択肢の最大を超えたら
	if (g_select != temp1) PlaySound(SOUND_LABEL_SE_select_mv);	//選択肢の最大を超えずに移動したら
	g_temp_text = g_text[g_select];
	g_text[g_select].size = 50;
	g_text[g_select].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	if (GetKeyboardTrigger(DIK_SPACE)) {
		PlaySound(SOUND_LABEL_SE_select_ok);
		switch (g_select)
		{
		case (SELECT_START):
			SetMode(MODE_GAME);
			break;
		case (SELECT_LOAD):
			SetMode(MODE_GAME);
			break;
		case (SELECT_OPTION):
			SetMode(MODE_MENUE);
			break;
		case (SELECT_EXIT):
			SetMode(MODE_MAX);
			break;
		default:
			g_select = SELECT_EXIT;
			break;
		}
	}

}
void Draw_title_hud(void) {
	char temp1[] = "ＳＴＡＲＴ";
	char temp2[] = "ＬＯＡＤ";
	char temp3[] = "ＯＰＴＩＯＮ";
	char temp4[] = "ＥＸＩＴ";

	int i = 0;
	SetText(g_text[i], temp1);
	i++;
	SetText(g_text[i], temp2);
	i++;
	SetText(g_text[i], temp3);
	i++;
	SetText(g_text[i], temp4);
}
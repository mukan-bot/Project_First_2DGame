#include "game.h"
#include "main.h"

#include "MAP.h"
#include "set_map.h"
#include "bg.h"
#include "Visual_tile.h"
#include "Ground_tile.h"
#include "collision.h"
#include "enemy.h"
#include "atk.h"
#include "boss_haze.h"
#include "result_bg.h"

//マクロ定義



//グローバル変数
static float g_scroll = 0.0f;
static float g_plus_scroll = 0.0f;


HRESULT Init_map(void) {
	InitBG();
	Init_Visual_tile();
	Init_Ground_tile();
	Init_set_map();
	g_scroll = 0.0f;

	Init_result_bg();
	return S_OK;
}

void Uninit_map(void) {
	UninitBG();
	Uninit_Visual_tile();
	Uninit_Ground_tile();

	Uninit_result_bg();
}

void Update_map(void) {
	UpdateBG();
}

void Draw_map(void) {
	if (Get_isBoss) {
		DrawBG();
	}
	else {
		Draw_result_bg();
	}
	Draw_Visual_tile();
	Draw_Ground_tile();
} 

bool Set_Scroll(float scroll_f){
	g_scroll += scroll_f;
	float temp = scroll_f * SCREEN_WIDTH;

	g_plus_scroll = temp;


	Update_Visual_tile(temp);
	Update_Ground_tile(temp);
	Update_eMove(temp);
	Update_Collision(temp);
	Update_sATK(temp);
	return TRUE;
}

float Get_Scroll() {
	return g_scroll;
}
float* Get_aScroll() {
	return &g_scroll;
}

float Get_PlusScroll() {
	return g_plus_scroll;
}
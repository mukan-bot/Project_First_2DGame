#include "game.h"
#include "main.h"

#include "player.h"
#include "enemy.h"

#include "collision.h"
#include "MAP.h"
#include "HUD.h"
#include "atk.h"
#include "effect.h"
#include "boss_map.h"

#include "set_map.h"



static int g_count = 0;
static float* g_a_scroll; //スクロール監視用

HRESULT Init_game(void) {
	Init_enemy();
	Init_map();
	Init_player();

	Init_ATK();
	Init_effect();

	Init_collision();

	g_a_scroll = Get_aScroll();

	Init_boss_map();



	return S_OK;
}
void Uninit_game(void) {
	Uninit_map();
	Uninit_enemy();
	Uninit_boss_map();
	Uninit_player();
	Uninit_HUD();
	Uninit_ATK();
	Uninit_effect();
}
void Update_game(void) {
	Update_map();
	Update_enemy();
	Update_boss_map();
	Update_player();
	Update_ATK();
	Update_effect();
	Update_HUD();


	Anime_Update();


	SCORE* temp_scr = Get_score();
	if (temp_scr->time_count > 60) {
		temp_scr->time++;
		temp_scr->time_count = 0;
	}
	temp_scr->time_count++;




	float temp_s = *g_a_scroll * SCREEN_WIDTH;

#ifdef _DEBUG
	PrintDebugProc("スクロール：%f\n", temp_s);
	PrintDebugProc("クリアスクロール：%d", GAME_CLEAR_POS);
#endif // _DEBUG


	if (temp_s > GAME_CLEAR_POS) {
		Set_boss();
		//temp_scr->is_clear = TRUE;
		//SetMode(MODE_RESULT);
	}
	


}
void Draw_game(void) {
	Draw_map();
	Draw_enemy();
	Draw_boss_map();
	Draw_player();
	Draw_ATK();
	Draw_effect();

	Draw_HUD();



}

void Anime_Update(){
}

//値の最大値と最小値を指定出来る
float clamp(float value, float min, float max) {
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	return value;
}
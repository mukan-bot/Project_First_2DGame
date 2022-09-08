#include "game.h"
#include "main.h"

#include "player.h"
#include "enemy.h"

#include "collision.h"
#include "MAP.h"
#include "HUD.h"
#include "atk.h"
#include "effect.h"
#include "boss_haze.h"
#include "input.h"

#include "set_map.h"



static int g_count = 0;
static float* g_a_scroll; //スクロール監視用
static bool g_is_boos;

HRESULT Init_game(void) {
	Init_enemy();
	Init_map();
	Init_player();

	Init_ATK();
	Init_effect();

	Init_collision();

	g_a_scroll = Get_aScroll();

	Init_boss_haze();

	g_is_boos = FALSE;

	return S_OK;
}
void Uninit_game(void) {
	Uninit_map();
	Uninit_enemy();
	Uninit_boss_haze();
	Uninit_player();
	Uninit_HUD();
	Uninit_ATK();
	Uninit_effect();
}
void Update_game(void) {
	Update_map();
	Update_enemy();
	Update_player();
	Update_ATK();
	Update_effect();
	Update_boss_haze();
	Update_HUD();


	Anime_Update();


	SCORE* temp_scr = Get_score();
	if (temp_scr->time_count > 60) {
		temp_scr->time++;
		temp_scr->time_count = 0;
	}
	temp_scr->time_count++;


#ifdef _DEBUG
	STATUS* temp = Get_pStatus();
	//MPを最大にする
	if (GetKeyboardPress(DIK_0)) {
		temp->mp = 1;
	}
	//HPを最大にする
	if (GetKeyboardPress(DIK_9)) {
		temp->mp = 1;
	}
	//一番最初の場所からボス前まで移動(２回押すと通り抜ける)
	if (GetKeyboardTrigger(DIK_8)) {
		Set_Scroll(6);
	}


#endif // _DEBUG



	float temp_s = *g_a_scroll * SCREEN_WIDTH;

	if ((temp_s > GAME_CLEAR_POS+1000)&&(!g_is_boos)) {
		Set_Enemy(ENEMY_BOSS, XMFLOAT2(900, 400));
		g_is_boos = TRUE;
	}

}
void Draw_game(void) {
	Draw_map();
	Draw_enemy();
	Draw_player();
	Draw_ATK();
	Draw_effect();

	Draw_boss_haze();
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

bool Get_isBoss(void) {
	return g_is_boos;
}
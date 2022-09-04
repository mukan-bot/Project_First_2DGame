#include "game.h"
#include "main.h"

#include "player.h"
#include "enemy.h"

#include "collision.h"
#include "MAP.h"
#include "HUD.h"
#include "atk.h"
#include "effect.h"

#include "set_map.h"

#define GAME_CLEAR_POS (4000)


static int g_count = 0;

HRESULT Init_game(void) {
	Init_enemy();
	Init_map();
	Init_player();

	Init_ATK();
	Init_effect();

	Init_collision();






	return S_OK;
}
void Uninit_game(void) {
	Uninit_map();
	Uninit_enemy();
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
	Update_HUD();


	Anime_Update();


	SCORE* temp = Get_score();
	if (temp->time_count > 60) {
		temp->time++;
		temp->time_count = 0;
	}
	temp->time_count++;
	PrintDebugProc("%f", Get_Scroll() * SCREEN_WIDTH);

}
void Draw_game(void) {
	Draw_map();
	Draw_enemy();
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
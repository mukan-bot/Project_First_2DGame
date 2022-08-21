#include "game.h"
#include "main.h"

#include "player.h"
#include "enemy.h"

#include "collision.h"
#include "MAP.h"
#include "HUD.h"
#include "atk.h"

#include "set_map.h"


HRESULT Init_game(void) {
	Init_map();
	Init_enemy();
	Init_player();

	Init_ATK();

	Init_collision();

	Set_map();


	return S_OK;
}
void Uninit_game(void) {
	Uninit_map();
	Uninit_enemy();
	Uninit_player();
	Uninit_HUD();
	Uninit_ATK();
}
void Update_game(void) {
	Update_map();
	Update_enemy();
	Update_player();
	Update_ATK();
	Update_HUD();

	Anime_Update();
}
void Draw_game(void) {
	Draw_map();
	Draw_enemy();
	Draw_player();
	Draw_ATK();

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
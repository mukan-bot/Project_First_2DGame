#include "game.h"
#include "main.h"

#include "player.h"
#include "collision.h"
#include "test.h"
#include "MAP.h"



HRESULT Init_game(void) {
	Init_map();
	Init_player();
	Init_test();

	Init_collision();
	return S_OK;
}
void Uninit_game(void) {
	Uninit_map();
	Uninit_player();
	Uninit_test();
}
void Update_game(void) {
	Update_map();
	Update_player();
	Update_test();
}
void Draw_game(void) {
	Draw_map();
	Anime_Update();
	Draw_test();
	Draw_player();
}

void Anime_Update(){
	
	Anime_player();
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
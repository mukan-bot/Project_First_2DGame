#include "game.h"

#include "player.h"
#include "collision.h"
#include "test.h"



HRESULT Init_game(void) {
	Init_player();
	Init_test();


	Init_collision();
	return S_OK;
}
void Uninit_game(void) {
	Uninit_player();
	Uninit_test();
}
void Update_game(void) {
	Update_player();
	Update_test();
}
void Draw_game(void) {
	Draw_player();
	Draw_test();
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
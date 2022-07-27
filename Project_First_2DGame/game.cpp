#include "game.h"

#include "player.h"
#include "collision.h"



HRESULT Init_game(void) {
	Init_player();
	Init_collision();
	return S_OK;
}
void Uninit_game(void) {
	Uninit_player();
}
void Update_game(void) {
	Update_player();
}
void Draw_game(void) {
	Draw_player();
}
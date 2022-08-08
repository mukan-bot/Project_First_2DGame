#include "game.h"
#include "main.h"

#include "bg.h"



HRESULT Init_map(void) {
	InitBG();
	return S_OK;
}
void Uninit_map(void) {
	UninitBG();
}
void Update_map(void) {
	UpdateBG();
}
void Draw_map(void) {
	DrawBG();
} 



#include "game.h"
#include "main.h"

#include "MAP.h"
#include "bg.h"
#include "Visual_tile.h"

//マクロ定義



//グローバル変数
static float g_scroll = 0.0f;


HRESULT Init_map(void) {
	InitBG();
	Init_Visual_tile();
	g_scroll = 0.0f;
	return S_OK;
}
void Uninit_map(void) {
	UninitBG();
	Uninit_Visual_tile();
}
void Update_map(void) {
#ifdef _DEBUG
	float temp = Get_Scroll();
	PrintDebugProc("%f", temp);
#endif
	UpdateBG();

	
}
void Draw_map(void) {
	DrawBG();
	Draw_Visual_tile();
} 

bool Set_Scroll(float scroll_f){
	g_scroll += scroll_f;

	Update_Visual_tile();
	return TRUE;
}

float Get_Scroll() {
	return g_scroll;
}

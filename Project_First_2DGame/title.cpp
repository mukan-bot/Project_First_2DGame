//作成者：竹内大翔

#include "main.h"
#include "title.h"
#include "title_bg.h"
#include "title_logo.h"
#include "title_hud.h"



HRESULT Init_title(void) {
	Init_title_bg();
	Init_title_logo();
	Init_title_hud();
	return S_OK;
}
void Uninit_title(void) {
	Uninit_title_bg();
	Uninit_title_logo();
	Uninit_title_hud();
}
void Update_title(void) {
	Update_title_bg();
	Update_title_logo();
	Update_title_hud();
}
void Draw_title(void) {
	Draw_title_bg();
	Draw_title_logo();
	Draw_title_hud();
}
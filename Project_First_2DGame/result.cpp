#include "main.h"
#include "result.h"
#include "result_bg.h"
#include "result_anime.h"
#include "result_score.h"



HRESULT Init_result(void) {
	Init_result_bg();
	Init_result_anime();
	Init_result_score();
	
	return S_OK;
}
void Uninit_result(void) {
	Uninit_result_bg();
	Uninit_result_anime();
	Uninit_result_score();
}
void Update_result(void) {
	Update_result_bg();
	if(Update_result_anime())Set_resurt_score();
	Update_result_score();
}
void Draw_result(void) {
	Draw_result_bg();
	Draw_result_anime();
	Draw_result_score();
}
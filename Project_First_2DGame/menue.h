#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//マクロ定義
enum {
	OPTION_BGM_VOLUME,
	OPTION_SE_VOLUME,
	OPTION_OK,
	OPTION_EXIT,
	OPTION_MAX,
};


//プロトタイプ宣言
HRESULT Init_menue(void);
void Uninit_menue(void);
void Update_menue(void);
void Draw_menue(void);
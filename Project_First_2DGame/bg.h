#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct BG{
	main_obj obj;
	int texNo = 0;
	float scrollSpeed = 0.0f;

};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

BG* GetBG(void);

void Set_ScrollBG(float set_scroll);



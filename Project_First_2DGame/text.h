#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


// マクロ定義

#define TEXT_MAX	(1280)	//最大文字数

//構造体定義
struct TEXT {
	XMFLOAT2 pos;
	float size;
	XMFLOAT4 color;
};




// プロトタイプ宣言
HRESULT Init_text(void);
void Uninit_text(void);
void Draw_text(void);
void SetText(TEXT text_p, char text[]);
void SetText_d(TEXT text_p, int d);
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

#include "game.h"

//構造体定義
struct TILE {
	main_obj obj;
	int tile_no = -1;
	int anime_speed = 0;//アニメションが何フレームずつ進むか
	int anime_count = 0;//今のアニメーションのどこを再生しているか
	int anime_frame = 0;//アニメションがなんコマか
};


//プロトタイプ宣言
HRESULT Init_map(void);
void Uninit_map(void);
void Update_map(void);
void Draw_map(void);

bool Set_Scroll(float scroll_f);
float Get_Scroll();


#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "collision.h"


struct PLAYER {
	main_obj obj;
	COLLISION col;
	animation anime;
	float speed;

};



// プロトタイプ宣言-------
HRESULT Init_player(void);
void Uninit_player(void);
void Update_player(void);
void Draw_player(void);
void Anime_player(void);
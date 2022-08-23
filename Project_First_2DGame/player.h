#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "collision.h"

//マクロ定義
//アニメーション上から
enum {
	IDLE_ANIME,
	RUN_ANIME,
	DAMAGE_ANIME,
	ANIME_MAX
};


//構造体定義
struct PLAYER {
	main_obj obj;
	COLLISION col;//攻撃など
	COLLISION col_U;//上
	COLLISION col_D;//下
	COLLISION col_L;//左
	COLLISION col_R;//右
	animation anime;
	bool is_jump;
	bool is_hitD;
	bool is_hitU;
	bool is_hitL;
	bool is_hitR;
	XMFLOAT2 line_pos;
	XMFLOAT2 line_vec;
};





// プロトタイプ宣言-------
HRESULT Init_player(void);
void Uninit_player(void);
void Update_player(void);
void Draw_player(void);
void Anime_player(void);

void Set_P_Anime(int anime);

STATUS* Get_pStatus(void);
PLAYER* Get_Player(void);
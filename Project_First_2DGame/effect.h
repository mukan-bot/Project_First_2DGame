#pragma once

#include "game.h"

//マクロ定義
enum effect {
	EFFECT_TYPE_ICE_SPEAR,
	EFFECT_TYPE_PURPLE_EXPLOSION,
	EFFECT_TYPE_STAR_EXPLOSION,
	EFFECT_TYPE_sSTAE_EXPLOSION,
	EFFECT_TYPE_EXPLOSION,
	EFFECT_TYPE_FIRE_BULLET_1,
	EFFECT_TYPE_FIRE_BULLET_2,
	EFFECT_TYPE_FIRE_BULLET_3,
	EFFECT_TYPE_FIRE_BULLET_4,
	EFFECT_TYPE_MAX,
};

//構造体定義
struct EFFECT {
	main_obj obj;
	animation anime;
	XMFLOAT2* pos;
	bool loop;
	int no;
	XMFLOAT4 color;
	bool is_Rside;
};

//プロトタイプ宣言
HRESULT Init_effect(void);
void Uninit_effect(void);
void Update_effect(void);
void Draw_effect(void);
int Set_effect(XMFLOAT2* pos, bool is_rside, float size, int effect_type, bool loop, int fps);
void Del_effect(int no);
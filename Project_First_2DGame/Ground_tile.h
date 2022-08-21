#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "collision.h"

#include "MAP.h"

//構造体定義
struct G_TILE{
	TILE tile;
	COLLISION col;

};


//プロトタイプ宣言
HRESULT Init_Ground_tile(void);
void Uninit_Ground_tile(void);
void Draw_Ground_tile(void);
void Update_Ground_tile(float plus);


int SetG_TILE(int tex_no, XMFLOAT3 pos_size);//見た目用設置
bool DelG_TILE(int count);//見た目用削除
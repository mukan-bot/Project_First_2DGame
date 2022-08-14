#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//プロトタイプ宣言
HRESULT Init_Visual_tile(void);
void Uninit_Visual_tile(void);
void Draw_Visual_tile(void);
void Update_Visual_tile();


int SetV_tile(int tex_no, XMFLOAT3 pos_size);//見た目用設置
bool DelV_tile(int count);//見た目用削除
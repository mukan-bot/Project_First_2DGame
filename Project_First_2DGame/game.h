#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

// マクロ定義


//構造体定義
struct obj
{
	texture tex;
	polygon pol;
	bool use = FALSE;
};

//テクスチャ系の変数
struct texture {
	float x = 0.00f;	// テクスチャの左上X座標
	float y = 0.00f;	// テクスチャの左上Y座標
	float w = 1.0f;		// テクスチャの幅
	float h = 1.0f;		// テクスチャの高さ
};

//ポリゴン系の変数
struct polygon {
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ポリゴンの座標
	float w = 1;		//幅
	float h = 1;		//高さ
};






// プロトタイプ宣言
HRESULT Init_game(void);
void Uninit_game(void);
void Update_game(void);
void Draw_game(void);
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


// マクロ定義

#define TEXT_MAX	(40)	//最大文字数


//構造体定義
struct TEXT
{
	XMFLOAT3	pos;		// ポリゴンの座標
	float		w, h;		// 幅と高さ

	float tw = 0.0207333333333333;	// テクスチャの幅
	float th = 0.0823333333333333;	// テクスチャの高さ
	float tx = 0.00f;	// テクスチャの左上X座標
	float ty = 0.00f;	// テクスチャの左上Y座標

	bool use = FALSE;
};






// プロトタイプ宣言
void SetText(float x, float y, float size, char text[TEXT_MAX]);
HRESULT Init_text(void);
void Uninit_text(void);
void Update_text(void);
void Draw_text(void);
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

				//エネミーの位置やテクスチャー座標を反映
	float px = 200;	// エネミーの表示位置X
	float py = 200;	// エネミーの表示位置Y
	float pw = 200;		// エネミーの表示幅
	float ph = 200;		// エネミーの表示高さ

	float tw = 0.016f;	// テクスチャの幅
	float th = 1.0f;	// テクスチャの高さ
	float tx = 0.0f;	// テクスチャの左上X座標
	float ty = 0.0f;	// テクスチャの左上Y座標

	bool use = FALSE;
};






// プロトタイプ宣言
void SetText(float x, float y, char text[]);
HRESULT Init_text(void);
void Uninit_text(void);
void Update_text(void);
void Draw_text(void);
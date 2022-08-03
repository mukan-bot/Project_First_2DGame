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

	float tw = 0.0207333333333333f;	// テクスチャの幅
	float th = 0.0823333333333333f;	// テクスチャの高さ
	float tx = 0.00f;	// テクスチャの左上X座標
	float ty = 0.00f;	// テクスチャの左上Y座標
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	bool use = FALSE;
};






// プロトタイプ宣言
HRESULT Init_text(void);
void Uninit_text(void);
void Draw_text(void);
//x座標,y座標,サイズ,色,テキスト
void SetText(float x, float y, float size, XMFLOAT4 color, char text[TEXT_MAX]);
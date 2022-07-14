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
	bool		use;
};






// プロトタイプ宣言
void SetText(float x, float y, char[TEXT_MAX]);

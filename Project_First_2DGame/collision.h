#pragma once

#include "main.h"
#include "game.h"

#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//マクロ定義
enum {
	GROUND,
};
enum {
	BOX,
	CAPSULE,//長さ０で円になる
};

//構造体定義
struct COLLISION {
	//座標1
	XMFLOAT2 pos;
	//座標2（カプセルのみ）
	XMFLOAT2 pos2;
	//角度
	float angle;
	//大きさ (サークルの時は両方同じ値を入れる)
	XMFLOAT2 size;
	//形
	int shape;
	//種類
	int type;
	//識別番号（入力不要SetCollisionの戻り値）
	int specific_numbers;
};

//プロトタイプ宣言
HRESULT Init_collision(void);
int SetCollision(COLLISION* collision);
bool GetCollision(COLLISION collision);
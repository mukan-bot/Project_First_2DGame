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
	//ベクトル：長さ（カプセルのみサークルは0.01）
	XMFLOAT2 vec;
	//大きさ (カプセル(サークル)のときはｘに幅、ｙは0)
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
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
	CIRCLE,
};

//構造体定義
struct COLLISION {
	//座標1
	XMFLOAT2 pos;
	//大きさ (サークルのときはｘに幅、ｙは0)
	XMFLOAT2 size;
	//形
	int shape;
	//種類
	int type;
	//識別番号（入力不要SetCollisionの戻り値）
	int specific_numbers = -1;
};

//プロトタイプ宣言
HRESULT Init_collision(void);
int SetCollision(COLLISION* collision);
bool delete_Collision(int numbers);

bool CheckHit(COLLISION collision);
bool CheckHit_lines(XMFLOAT2 pos1, XMFLOAT2 vec1, XMFLOAT2 pos2, XMFLOAT2 vec2);//二つの線分の当たり判定(pos1:線分1,vec1:ベクトル1,lpos2:線分2vec2:ベクトル2)
float distance_line_point(XMFLOAT2 pos, XMFLOAT2 vec, XMFLOAT2 point);// 線分と点の最短距離の２乗
float distance_point(XMFLOAT2 pos1, XMFLOAT2 pos2);//2点間の距離
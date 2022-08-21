#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

// マクロ定義-------
#define PLUS_G (0.01f);//重力の加速度

//構造体定義-------

//テクスチャ系の変数
struct texture {
	int texNo = 0;
	float x = 0.00f;	// テクスチャの左上X座標
	float y = 0.00f;	// テクスチャの左上Y座標
	float w = 1.0f;		// テクスチャの幅
	float h = 1.0f;		// テクスチャの高さ
};

//ポリゴン系の変数
struct polygon {
	float w = 1.0f;		//幅
	float h = 1.0f;		//高さ
};

//アニメーション系の変数
struct animation {
	int anime_FPS = 0;//Animationの更新頻度
	int count_FPS = 0;//前回の更新から何フレームたったか
	int anime_frame = 0;//Animationの今のコマ
	int anime_frame_max = 0;//Animationの最大コマ数
	int which_anime = 0;//今どのアニメーションを再生しているか

	int anime_count; //横分割数
	int anime_number;//縦分割数
	
};



struct main_obj{
	texture tex;
	polygon pol;
	bool use = FALSE;
	XMFLOAT2 pos = XMFLOAT2(0.0f, 0.0f);		// ポリゴンの座標

};

struct STATUS {
	float hp;
	float mp;
	float plus_mp;//MPの回復
	float speed;//移動速度
	float jump_speed;
	float gravity;
	float temp_gravity;//重力加速度をplusしたあとの値
	float atk;
	float matk;
};

// プロトタイプ宣言-------
HRESULT Init_game(void);
void Uninit_game(void);
void Update_game(void);
void Draw_game(void);
void Anime_Update(void);

float clamp(float value, float min, float max);
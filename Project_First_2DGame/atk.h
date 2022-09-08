#pragma once

#include "game.h"

#include "collision.h"

//マクロ定義
enum {
	STANDARD_ATK,			//通常攻撃
	LINE_ATK,				//直線上に飛んでいく攻撃
	STANDARD_ATK_ENEMY,		//プレイヤー向かって飛んでいく攻撃
	STANDARD_ATK_BOSS,		//BOSS用の攻撃
	MAX_ATK,//攻撃の種類の最大数
};






//構造体定義
struct attack {
	main_obj obj;
	COLLISION col;
	XMFLOAT2 start_pos;
	XMFLOAT2 vec;
	int atk_type;	//どの攻撃か
	bool is_Rside;	//右側に対する攻撃ならtrue
	int count_frame;
	int effect_no;
	int no;
};

struct attack_status {
	XMFLOAT2 vec;			//攻撃が飛んでいく場所（is_Rsideがfalseのときxをマイナスにする）
	float atk_power;		//攻撃力（最大1.0f）
	float minus_mp;			//消費MP（最大1.0f）
	int frame;				//フレーム数（少ないと早い）少なすぎると判定抜けるかも
	COLLISION col;
	main_obj obj;
	XMFLOAT4 color;
	bool is_g_collision;	//地面を貫通するか

};


//プロトタイプ宣言
HRESULT Init_ATK(void);
void Uninit_ATK(void);
void Update_ATK(void);
void Update_sATK(float plus);
void Draw_ATK(void);

float CheckDamage(COLLISION collision);

//誰に当たる攻撃か,どの攻撃か,右側に対する攻撃ならtrue,発射場所
float Set_ATK(int hit_type, int atk_type, bool is_Rside, XMFLOAT2 start_pos);

//攻撃別のステータスの所得
attack_status Get_Atk_status(int atk_type);

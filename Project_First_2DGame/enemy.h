#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "collision.h"

//マクロ定義
enum {				//敵の種類
	ENEMY_HUNTRESS,		//基本の敵
	ENEMY_TYPE_MAX,	//敵の種類の数
};
//アニメーション上から
enum {
	E_NO_ANIME,
	E_ATK1_ANIME,
	E_ATK2_ANIME,
	E_DEATH_ANIME,
	E_RUN_ANIME,
	E_IDLE_ANIME,
	E_DAMAGE_ANIME,
	E_ANIME_MAX
};

//構造体定義
struct ENEMY {
	main_obj obj;
	COLLISION col;//全体
	COLLISION col_U;//上
	COLLISION col_D;//下
	COLLISION col_L;//左
	COLLISION col_R;//右
	animation anime;
	bool is_run_R = TRUE;
	bool is_jump;
	bool is_hitD;
	bool is_hitU;
	bool is_hitL;
	bool is_hitR;
	STATUS status;
	int enemy_type;
	int enemy_no;
	int count_atk = 0;	
	int atk_waiting = 0;//攻撃までのラグ（アニメーションとのズレ）
	int atk_waiting_count = 0;//攻撃までのラグのフレームのカウント

	int select_atk = -1; //キャラクターが攻撃途中で離れたときに攻撃が戻らないように
};
struct ANIME_TYPE {
	int anime_type;
	int frame = 0;

	bool anime_loop;
};
struct ENEMY_STATUS {
	float size;
	XMFLOAT2 col_size;
	XMFLOAT2 aspect;//大きいところを1.0fとする
	ENEMY enemy;
	ANIME_TYPE anime_type[E_ANIME_MAX];//アニメーションの種類を上から順に指定

};





// プロトタイプ宣言-------
HRESULT Init_enemy(void);
void Uninit_enemy(void);
void Update_enemy(void);
void Draw_enemy(void);
void Set_E_Anime(int enemy_no, int anime);
int Set_Enemy(int enemy_type,XMFLOAT2 pos);

void Update_eMove(float plus);


ENEMY* Get_Enemy(void);
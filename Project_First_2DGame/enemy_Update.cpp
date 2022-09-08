#include "enemy_Update.h"
#include "enemy.h"

#include "collision.h"
#include "atk.h"
#include "player.h"
#include "MAP.h"

//グローバル変数
PLAYER* g_player;	//プレイヤーのアドレス
float* g_scl;		//実質のプレイヤーのx座標
SCORE* g_score;

//プロトタイプ宣言
void huntress_Update(ENEMY* enemy);
void boss_Update(ENEMY* enemy);


void Init_Enemy_s_Update(void) {
	g_player = Get_Player();
	g_scl = Get_aScroll();
	g_score = Get_score();
}

void Enemy_s_Update(ENEMY* enemy) {
	switch (enemy->enemy_type)
	{
	case (ENEMY_HUNTRESS):
		huntress_Update(enemy);
		break;
	case(ENEMY_BOSS):
		boss_Update(enemy);
		break;
	default:
		break;
	}

}


void huntress_Update(ENEMY* enemy) {
	//プレイヤーのいる方を向く
	if (g_player->obj.pos.x < enemy->obj.pos.x) enemy->is_run_R = FALSE;
	else enemy->is_run_R = TRUE;

	//ダメージがあったらhpをマイナスして、アニメーションを流す
	float temp = CheckDamage(enemy->col);
	if (temp != 0.0f) {
		enemy->status.hp -= temp;

		Set_E_Anime(enemy->enemy_no, E_DAMAGE_ANIME);

		//プレイヤーの攻撃が当たったら攻撃をリセットする
		enemy->count_atk = 0; 
		enemy->atk_waiting_count = 0;
		enemy->select_atk = -1;
	}
	//プレイヤーとの距離を所得する
	temp = distance_line_point(g_player->line_pos, g_player->line_vec, enemy->obj.pos);

	if (enemy->select_atk == -1) {
		if (temp < 200) enemy->select_atk = 1;
		else if (temp < 10) enemy->select_atk = 2;
		else enemy->select_atk = -1;
	}

	switch (enemy->select_atk){
	case(1):
		if (g_player->obj.pos.y > enemy->obj.pos.y - (enemy->obj.pol.h / 2)) {
			//連射できないようにする。
			if (enemy->count_atk > (rand() % 50) + 70) {
				if (enemy->atk_waiting_count > enemy->atk_waiting) {
					Set_ATK(ATK_ENEMY, STANDARD_ATK_ENEMY, enemy->is_run_R, enemy->obj.pos);
					enemy->count_atk = 0;
					enemy->atk_waiting_count = 0;
					enemy->select_atk = -1;
				}
				else {
					if (enemy->anime.which_anime != E_ATK1_ANIME)Set_E_Anime(enemy->enemy_no, E_ATK1_ANIME);
				}
				enemy->atk_waiting_count++;
			}
		}
		break;

	case(2):
		if (g_player->obj.pos.y > enemy->obj.pos.y - (enemy->obj.pol.h / 2)) {
			//連射できないようにする。
			if (enemy->count_atk > (rand() % 50) + 30) {
				if (enemy->atk_waiting_count > enemy->atk_waiting) {
					Set_ATK(ATK_ENEMY, STANDARD_ATK, enemy->is_run_R, enemy->obj.pos);
					enemy->count_atk = 0;
					enemy->atk_waiting_count = 0;
					enemy->select_atk = -1;
				}
				else {
					if (enemy->anime.which_anime != E_ATK1_ANIME)Set_E_Anime(enemy->enemy_no, E_ATK1_ANIME);
				}
				enemy->atk_waiting_count++;
			}
		}
		break;
	}



	if (enemy->status.hp < 0.0f) {
		enemy->obj.use = FALSE;
		g_score->del_enemy++;

	}
	if (!enemy->is_hitD) {
		enemy->obj.pos.y += 1;
	}
	enemy->count_atk++;
}



void boss_Update(ENEMY* enemy) {

	//プレイヤーのいる方を向く
	if (g_player->obj.pos.x < enemy->obj.pos.x) enemy->is_run_R = FALSE;
	else enemy->is_run_R = TRUE;


	if (enemy->is_hitD) {
		//enemy->obj.pos.y -= enemy->status.jump_speed;
	}
	else {
		enemy->obj.pos.y += enemy->status.jump_speed;
	}

	if (enemy->count_atk > 140) {
		switch (rand() % 3){
		case(0):
			Set_ATK(ATK_ENEMY, STANDARD_ATK_BOSS, enemy->is_run_R, enemy->obj.pos);
			enemy->count_atk = 0;
			break;
		case(1):
			XMFLOAT2 pos = XMFLOAT2(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
			Set_Enemy(ENEMY_HUNTRESS, pos);
			enemy->count_atk = 0;
			break;
		case(2):
			Set_ATK(ATK_ENEMY, STANDARD_ATK_ENEMY, enemy->is_run_R, enemy->obj.pos);
			Set_ATK(ATK_ENEMY, STANDARD_ATK_ENEMY, !enemy->is_run_R, enemy->obj.pos);
			enemy->count_atk = 0;
			break;
		default:
			break;
		}
		//if ((rand() % 5) != 0) {
		//	Set_ATK(ATK_ENEMY, STANDARD_ATK_BOSS, enemy->is_run_R, enemy->obj.pos);
		//	enemy->count_atk = 0;
		//}
		//else {
		//	XMFLOAT2 pos = XMFLOAT2(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		//	Set_Enemy(ENEMY_HUNTRESS, pos);
		//}
	}
	

	enemy->count_atk++;



	//HPがゼロだったらリザルトへ
	enemy->status.hp -= CheckDamage(enemy->col);
	if (enemy->status.hp < 0.0f) {
		g_score->is_clear = TRUE;
		SetMode(MODE_RESULT);
	}
}
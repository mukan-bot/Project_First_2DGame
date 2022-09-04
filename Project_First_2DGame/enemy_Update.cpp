#include "enemy_Update.h"
#include "enemy.h"

#include "collision.h"
#include "atk.h"
#include "player.h"
#include "MAP.h"

//グローバル変数
PLAYER* g_player;	//プレイヤーのアドレス
float* g_scl;		//実質のプレイヤーのx座標

//プロトタイプ宣言
void huntress_Update(ENEMY* enemy);


void Init_Enemy_s_Update(void) {
	g_player = Get_Player();
	g_scl = Get_aScroll();
}

void Enemy_s_Update(ENEMY* enemy) {
	switch (enemy->enemy_type)
	{
	case (ENEMY_HUNTRESS):
		huntress_Update(enemy);
	default:
		break;
	}
}


void huntress_Update(ENEMY* enemy) {
<<<<<<< HEAD
	PrintDebugProc("asdfasdf");
	if (g_player->obj.pos.x < enemy->obj.pos.x) enemy->is_run_R = FALSE;
	else enemy->is_run_R = TRUE;

=======
	Set_ATK(ATK_ENEMY, LINE_ATK, TRUE, enemy->obj.pos);
>>>>>>> 5bdf4e18536df02258517fc9586dddd736b4405a
	enemy->status.hp -= CheckDamage(enemy->col);
	if (enemy->status.hp < 0.0f) {
		enemy->obj.use = FALSE;
	}
	if (!enemy->is_hitD) {
		enemy->obj.pos.y += 1;
	}
}
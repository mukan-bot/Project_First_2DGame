#include "enemy_Update.h"
#include "enemy.h"

#include "collision.h"
#include "atk.h"

//プロトタイプ宣言
void huntress_Update(ENEMY* enemy);


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
	
	//Set_ATK(ATK_ENEMY, LINE_ATK, TRUE, enemy->obj.pos);
	enemy->status.hp -= CheckDamage(enemy->col);
	if (enemy->status.hp < 0.0f) {
		enemy->obj.use = FALSE;
	}
	if (!enemy->is_hitD) {
		enemy->obj.pos.y-=0.001;
	}
}
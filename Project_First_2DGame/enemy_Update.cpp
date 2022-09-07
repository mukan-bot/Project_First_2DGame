#include "enemy_Update.h"
#include "enemy.h"

#include "collision.h"
#include "atk.h"
#include "player.h"
#include "MAP.h"

//�O���[�o���ϐ�
PLAYER* g_player;	//�v���C���[�̃A�h���X
float* g_scl;		//�����̃v���C���[��x���W
SCORE* g_score;

//�v���g�^�C�v�錾
void huntress_Update(ENEMY* enemy);


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
	default:
		break;
	}

}


void huntress_Update(ENEMY* enemy) {
	//�v���C���[�̂����������
	if (g_player->obj.pos.x < enemy->obj.pos.x) enemy->is_run_R = FALSE;
	else enemy->is_run_R = TRUE;

	//�_���[�W����������hp���}�C�i�X���āA�A�j���[�V�����𗬂�
	float temp = CheckDamage(enemy->col);
	if (temp != 0.0f) {
		enemy->status.hp -= temp;

		Set_E_Anime(enemy->enemy_no, E_DAMAGE_ANIME);

		//�v���C���[�̍U��������������U�������Z�b�g����
		enemy->count_atk = 0; 
		enemy->atk_waiting_count = 0;
		enemy->select_atk = -1;
	}
	//�v���C���[�Ƃ̋�������������
	temp = distance_line_point(g_player->line_pos, g_player->line_vec, enemy->obj.pos);

	if (enemy->select_atk == -1) {
		if (temp < 200) enemy->select_atk = 1;
		else if (temp < 10) enemy->select_atk = 2;
		else enemy->select_atk = -1;
	}

	switch (enemy->select_atk){
	case(1):
		if (g_player->obj.pos.y > enemy->obj.pos.y - (enemy->obj.pol.h / 2)) {
			//�A�˂ł��Ȃ��悤�ɂ���B
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
			//�A�˂ł��Ȃ��悤�ɂ���B
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
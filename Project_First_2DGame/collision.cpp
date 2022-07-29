#include "collision.h"
#include "text.h"

//�}�N����`
#define MAX_COLLISION (256)


//�O���[�o���ϐ�
COLLISION* g_collision[MAX_COLLISION];
//����������������������
int g_collision_count;



HRESULT Init_collision(void) {
	g_collision_count = 0;
	return S_OK;
}

//�߂�l-1�͐������s�i������g_collision_count�̒l�j
int SetCollision(COLLISION* collision) {
	if (g_collision_count < MAX_COLLISION) {
		collision->specific_numbers = g_collision_count;
		g_collision[g_collision_count] = collision;
		g_collision[g_collision_count]->specific_numbers = g_collision_count;
		g_collision_count++;
		return g_collision_count - 1;
	}
	return -1;
}


bool GetCollision(COLLISION collision) {
	
	//�l���g���₷���悤�ɂ܂Ƃ߂Ă���collision
	XMFLOAT4 temp;
	float pos_pos2_range;
	if (collision.shape == BOX) {
		//��,�E,��,��
		temp = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
	}

	for (int i = 0; i <= g_collision_count; i++) {

		//type����v���Ă��Ȃ��ꍇi�Ɂ{�P���Č����Ĉ�v������̂�T�� or ���ʃi���o�[�����������蔻����X�L�b�v
		while ((g_collision[i]->type != collision.type)||(g_collision[i]->specific_numbers==collision.specific_numbers)) {
			

			if (i < g_collision_count) {
				return FALSE;
			}
			i++;
		}
		//�l���g���₷���悤�ɂ܂Ƃ߂Ă���g_collision
		XMFLOAT4 temp_g;
		//��,�E,��,��
		temp_g = XMFLOAT4(g_collision[i]->pos.x - (g_collision[i]->size.x / 2.0f), g_collision[i]->pos.x + (g_collision[i]->size.x / 2.0f), g_collision[i]->pos.y - (g_collision[i]->size.y / 2.0f), g_collision[i]->pos.y + (g_collision[i]->size.y / 2.0f));

		if (g_collision[i]->shape == BOX) {
			//BOX��BOX
			if (collision.shape == BOX) {
				if ((temp_g.x < temp.y) && (temp_g.y > temp.x) && (temp_g.z < temp.w) && (temp_g.w > temp.z)) {
					return TRUE;
				}
			} 		
		}
	}
	return FALSE;
}
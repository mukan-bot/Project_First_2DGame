#include "collision.h"
#include "text.h"

//�}�N����`
#define MAX_COLLISION (256)
#define PI (3.14159265f)//�~����

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
	XMFLOAT4 temp_c;
	if (collision.shape == BOX) {
		//��,�E,��,��
		temp_c = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
	}
	else if (collision.shape == CAPSULE) {
		
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
				if ((temp_g.x < temp_c.y) && (temp_g.y > temp_c.x) && (temp_g.z < temp_c.w) && (temp_g.w > temp_c.z)) {
					return TRUE;
				}
			} 	
			else if (collision.shape == CAPSULE) {
				XMFLOAT2 pos_g[5];	//BOX�̎l���ƒ��S�̍��W�����Ă���
				pos_g[0] = g_collision[i]->pos;	//���S�_
				pos_g[1] = XMFLOAT2(g_collision[i]->pos.x - g_collision[i]->size.x / 2, g_collision[i]->pos.y - g_collision[i]->size.y / 2);	//����
				pos_g[2] = XMFLOAT2(g_collision[i]->pos.x + g_collision[i]->size.x / 2, g_collision[i]->pos.y - g_collision[i]->size.y / 2);	//�E��
				pos_g[3] = XMFLOAT2(g_collision[i]->pos.x - g_collision[i]->size.x / 2, g_collision[i]->pos.y + g_collision[i]->size.y / 2);	//����
				pos_g[4] = XMFLOAT2(g_collision[i]->pos.x + g_collision[i]->size.x / 2, g_collision[i]->pos.y + g_collision[i]->size.y / 2);	//�E��
				
				//�l���Ƃ̔���
				for (int i = 1; i < 5; i++) {
					float dx = pos_g[i].x - collision.pos.x;				// ����
					float dy = pos_g[i].y - collision.pos.y;				// ����
					float t = (collision.vec.x * dx + collision.vec.y * dy) / (collision.vec.x * collision.vec.x + collision.vec.y * collision.vec.y);
					t = clamp(t, 0.0f, 1.0f);
					XMFLOAT2 min;
					min.x = collision.vec.x * t + collision.pos.x;
					min.y = collision.vec.y * t + collision.pos.y;
					float fDistSpr = (min.x - pos_g[i].x) * (min.x - pos_g[i].x) + (min.y - pos_g[i].y) * (min.y - pos_g[i].y); //�����̓��

					if (fDistSpr < collision.size.x) {
						//OutputDebugString("te");
						return TRUE;
					}
				}
				//���S�Ƃ̔���傫������̎��̂��蔲���h�~
				float dx = pos_g[0].x - collision.pos.x;				// ����
				float dy = pos_g[0].y - collision.pos.y;				// ����
				float t = (collision.vec.x * dx + collision.vec.y * dy) / (collision.vec.x * collision.vec.x + collision.vec.y * collision.vec.y);
				t = clamp(t, 0.0f, 1.0f);
				XMFLOAT2 min;
				min.x = collision.vec.x * t + collision.pos.x;
				min.y = collision.vec.y * t + collision.pos.y;
				float fDistSpr = (min.x - pos_g[0].x) * (min.x - pos_g[0].x) + (min.y - pos_g[0].y) * (min.y - pos_g[0].y); //�����̓��

				if (fDistSpr < collision.size.x + g_collision[i]->size.x) {
					OutputDebugString("te");
					return TRUE;
				}
				//else if (fDistSpr < collision.size.x + g_collision[i]->size.y * collision.size.x + g_collision[i]->size.y) {
				//	//OutputDebugString("te");
				//	return TRUE;
				//}

			}
		}
	}
	return FALSE;
}
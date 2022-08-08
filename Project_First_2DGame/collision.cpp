#include "collision.h"
#include "text.h"
#include "main.h"

//�}�N����`
#define MAX_COLLISION (256)	//�����蔻��̌��E



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

//����̍폜
bool delete_Collision(int numbers) {
	int i = numbers;
	if (g_collision[i]->specific_numbers == 0) {
		return FALSE;
	}
	while (g_collision[i]->specific_numbers == -1) {
		g_collision[i] = g_collision[i + 1];
		i++;
	}
	return TRUE;
}

bool CheckHit(COLLISION collision) {
	
	//�l���g���₷���悤�ɂ܂Ƃ߂Ă���collision
	XMFLOAT4 temp_c;
	if (collision.shape == BOX) {
		//��,�E,��,��
		temp_c = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
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
			//BOX��CIRCLE
			else if (collision.shape == CIRCLE) {
				XMFLOAT2 pos_g[4];	//BOX�̎l���ƒ��S�̍��W�����Ă���
				pos_g[0] = XMFLOAT2(g_collision[i]->pos.x - g_collision[i]->size.x / 2, g_collision[i]->pos.y - g_collision[i]->size.y / 2);	//����
				pos_g[1] = XMFLOAT2(g_collision[i]->pos.x + g_collision[i]->size.x / 2, g_collision[i]->pos.y - g_collision[i]->size.y / 2);	//�E��
				pos_g[2] = XMFLOAT2(g_collision[i]->pos.x - g_collision[i]->size.x / 2, g_collision[i]->pos.y + g_collision[i]->size.y / 2);	//����
				pos_g[3] = XMFLOAT2(g_collision[i]->pos.x + g_collision[i]->size.x / 2, g_collision[i]->pos.y + g_collision[i]->size.y / 2);	//�E��
				float temp;
				for (int j = 0; j < 8; j += 2) {
					temp = distance_line_point(pos_g[j], XMFLOAT2(pos_g[j+1].x - pos_g[j].x, pos_g[j+1].y - pos_g[j].y), collision.pos);
					if (temp < collision.size.x * collision.size.x) {
						return TRUE;
					}
				}
			}
		}
		else {
			//CIRCLE��BOX
			if (collision.shape == BOX) {
				XMFLOAT2 pos_c[4];	//BOX�̎l���ƒ��S�̍��W�����Ă���
				pos_c[0] = XMFLOAT2(collision.pos.x - collision.size.x / 2, collision.pos.y - collision.size.y / 2);	//����
				pos_c[1] = XMFLOAT2(collision.pos.x + collision.size.x / 2, collision.pos.y - collision.size.y / 2);	//�E��
				pos_c[2] = XMFLOAT2(collision.pos.x - collision.size.x / 2, collision.pos.y + collision.size.y / 2);	//����
				pos_c[3] = XMFLOAT2(collision.pos.x + collision.size.x / 2, collision.pos.y + collision.size.y / 2);	//�E��
				float temp;
				for (int j = 0; j < 8; j += 2) {
					temp = distance_line_point(pos_c[j], XMFLOAT2(pos_c[j + 1].x - pos_c[j].x, pos_c[j + 1].y - pos_c[j].y), g_collision[i]->pos);
					if (temp < g_collision[i]->size.x * g_collision[i]->size.x) {
						return TRUE;
					}
				}

			}
			//CIRCLE��CIRCLE
			else if (collision.shape == CIRCLE) {
				float temp = distance_point(g_collision[i]->pos, collision.pos);
				float r = g_collision[i]->size.x + collision.size.x;
				if (temp < r * r) {
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}



bool CheckHit_lines(XMFLOAT2 pos1, XMFLOAT2 vec1, XMFLOAT2 pos2, XMFLOAT2 vec2) {


	//���s�̑Ώ�:����������肾���ǉ�ʔ͈͍l������덷
	if (vec1.x == 0.0f) {
		vec1.x += 0.001f;
	}
	if (vec1.y == 0.0f) {
		vec1.y += 0.001f;
	}
	if (vec2.x == 0.0f) {
		vec2.x += 0.001f;
	}
	if (vec2.y == 0.0f) {
		vec2.y += 0.001f;
	}


	float t1 = (vec1.y - pos1.y) / (vec1.x - pos1.x);
	float t2 = (vec2.y - pos2.y) / (vec2.x - pos2.x);

	float x = 0.0f;
	float y = 0.0f;

	if (t1 != t2) {
		x = (pos2.y - pos1.y + t1 * pos1.x - t2 * pos2.x) / (t1 - t2);
		y = t1 * (x - pos1.x) + pos1.y;
	}
	float r1 = (x - pos1.x) / (vec1.x - pos1.x);
	float r2 = (x - pos2.x) / (vec2.x - pos2.x);
	bool hit = ((0 < r1) && (r1 < 1) && (0 < r2) && (r2 < 1));
	return hit;
}


float distance_line_point(XMFLOAT2 pos, XMFLOAT2 vec, XMFLOAT2 point) {
	float			dx, dy;// �ʒu�̍���
	float			t;
	float			mx, my;// �ŏ��̋�����^������W

	float			fDistSqr;

	dx = point.x - pos.x;// ����
	dy = point.y - pos.y;// ����
	t = clamp((vec.x * dx + vec.y * dy) / (vec.x * vec.x + vec.y * vec.y), 0.0f, 1.0f);
	mx = vec.x * t + pos.x;	// �ŏ��ʒu��^������W
	my = vec.y * t + pos.y;
	fDistSqr = (mx - point.x) * (mx - point.x) +(my - point.y) * (my - point.y);	
	return fDistSqr;
}

float distance_point(XMFLOAT2 pos1, XMFLOAT2 pos2) {
	float			dx, dy;// �ʒu�̍���

	float			fDistSqr;

	dx = pos1.x - pos2.x;// ����
	dy = pos1.y - pos2.y;// ����
	fDistSqr = dx * dx + dy * dy;// �����̂Q��

	return fDistSqr;
}
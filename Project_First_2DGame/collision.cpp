#include "collision.h"

//�}�N����`
#define MAX_COLLISION (256)


//�O���[�o���ϐ�
COLLISION g_collision[MAX_COLLISION];
//����Ɏg���l�����Ă���
XMFLOAT4 g_collision_range[MAX_COLLISION];
//����������������������
int g_collision_count;



HRESULT Init_collision(void) {
	g_collision_count = 0;
	return S_OK;
}

//�߂�l-1�͐������s�i������g_collision_count�̒l�j
int SetCollision(COLLISION collision) {
	if (g_collision_count < MAX_COLLISION) {
		g_collision[g_collision_count] = collision;
		g_collision[g_collision_count].specific_numbers = g_collision_count;
		//����Ɏg���l���܂Ƃ߂Ă���
		switch (collision.shape)
		{
		case (BOX):
			//                                              ���[                                    �E�[                                    ��                                   ��
			g_collision_range[g_collision_count] = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x - (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
			break;

		case (CIRCLE):
			//                                              �|�W�V�������@�@�|�W�V�������@�@���a�@�@�@�@�@�@  �@ ���a
			g_collision_range[g_collision_count] = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x / 2.0f, 0.0f);
			break;

		case (CAPSULE):
			//                                              �|�W�V�������@�@�|�W�V�������@�@�����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
			g_collision_range[g_collision_count] = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x - collision.size.y, collision.size.y);
		}

		g_collision_count++;
		return g_collision_count - 1;
	}
	return -1;
}


bool GetCollision(COLLISION collision) {
	//����Ɏg���l���܂Ƃ߂Ă���
	XMFLOAT4 collision_range;
	switch (collision.shape)
	{
	case (BOX):
		//                         ���[                                    �E�[                                        ��                                   ��
		collision_range = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x - (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
		break;

	case (CIRCLE):
		//                         �|�W�V�������@�@�|�W�V�������@�@���a�@�@�@�@�@�@  �@ 
		collision_range = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x / 2.0f, 0.0f);
		break;

	case (CAPSULE):
		//                         �|�W�V�������@�@�|�W�V�������@�@�����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		collision_range = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x - collision.size.y, collision.size.y);
	}
	for (int i = 0; i < g_collision_count; i++) {
		//type����v���Ă��Ȃ��ꍇi�Ɂ{�P���Č����Ĉ�v������̂�T��
		while (g_collision[i].type != collision.type){
			i++;
		}

		if (g_collision[i].shape == BOX) {
			if ((g_collision_range[i].x < collision_range.y) && (g_collision_range[i].y > collision_range.x) && (g_collision_range[i].z < collision_range.w) && (g_collision_range[i].w > collision_range.z)) {
				return TRUE;
			}
		}
		else if (g_collision[i].shape == CIRCLE) {
			if (collision.shape == BOX) {
				XMFLOAT4 tempBox = XMFLOAT4(g_collision_range[i].x + collision_range.z, g_collision_range[i].y + collision_range.z, g_collision_range[i].z + collision_range.z, g_collision_range[i].w + collision_range.z);
				float range = (g_collision_range[i].x - collision_range.x) * (g_collision_range[i].x - collision_range.x) + (g_collision_range[i].y - collision_range.y) * (g_collision_range[i].y - collision_range.y);

			}
			else if (collision.shape == CIRCLE) {
				float range = (g_collision_range[i].x - collision_range.x) * (g_collision_range[i].x - collision_range.x) + (g_collision_range[i].y - collision_range.y) * (g_collision_range[i].y - collision_range.y);
				if (range <= g_collision_range[i].z + collision_range.z) {
					return TRUE;
				}
			}
		}
		else if (g_collision[i].shape == CAPSULE) {

		}
	}
	return FALSE;
}
#pragma once

#include "main.h"
#include "game.h"

#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//�}�N����`
enum {
	GROUND,
};
enum {
	BOX,
	CIRCLE,
};

//�\���̒�`
struct COLLISION {
	//���W1
	XMFLOAT2 pos;
	//�傫�� (�T�[�N���̂Ƃ��͂��ɕ��A����0)
	XMFLOAT2 size;
	//�`
	int shape;
	//���
	int type;
	//���ʔԍ��i���͕s�vSetCollision�̖߂�l�j
	int specific_numbers = -1;
};

//�v���g�^�C�v�錾
HRESULT Init_collision(void);
int SetCollision(COLLISION* collision);
bool delete_Collision(int numbers);

bool CheckHit(COLLISION collision);
bool CheckHit_lines(XMFLOAT2 pos1, XMFLOAT2 vec1, XMFLOAT2 pos2, XMFLOAT2 vec2);//��̐����̓����蔻��(pos1:����1,vec1:�x�N�g��1,lpos2:����2vec2:�x�N�g��2)
float distance_line_point(XMFLOAT2 pos, XMFLOAT2 vec, XMFLOAT2 point);// �����Ɠ_�̍ŒZ�����̂Q��
float distance_point(XMFLOAT2 pos1, XMFLOAT2 pos2);//2�_�Ԃ̋���
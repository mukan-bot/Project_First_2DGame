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
	CAPSULE,//�����O�ŉ~�ɂȂ�
};

//�\���̒�`
struct COLLISION {
	//���W1
	XMFLOAT2 pos;
	//���W2�i�J�v�Z���̂݁j
	XMFLOAT2 pos2;
	//�p�x
	float angle;
	//�傫�� (�T�[�N���̎��͗��������l������)
	XMFLOAT2 size;
	//�`
	int shape;
	//���
	int type;
	//���ʔԍ��i���͕s�vSetCollision�̖߂�l�j
	int specific_numbers;
};

//�v���g�^�C�v�錾
HRESULT Init_collision(void);
int SetCollision(COLLISION* collision);
bool GetCollision(COLLISION collision);
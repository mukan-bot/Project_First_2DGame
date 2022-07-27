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
	CAPSULE,
};

//�\���̒�`
struct COLLISION {
	//���W
	XMFLOAT2 pos;
	//�p�x
	float angle;
	//�傫�� (�T�[�N���̎��͗��������l������)
	XMFLOAT2 size;
	//�`
	int shape;
	//���
	int type;
	//����
	int specific_numbers;
};

//�v���g�^�C�v�錾
HRESULT Init_collision(void);
int SetCollision(COLLISION collision);
bool GetCollision(COLLISION collision);
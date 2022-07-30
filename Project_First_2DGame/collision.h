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
	//�x�N�g���F�����i�J�v�Z���̂݃T�[�N����0.01�j
	XMFLOAT2 vec;
	//�傫�� (�J�v�Z��(�T�[�N��)�̂Ƃ��͂��ɕ��A����0)
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
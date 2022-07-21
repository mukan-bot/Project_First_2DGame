#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

// �}�N����`


//�\���̒�`
struct obj
{
	texture tex;
	polygon pol;
	bool use = FALSE;
};

//�e�N�X�`���n�̕ϐ�
struct texture {
	float x = 0.00f;	// �e�N�X�`���̍���X���W
	float y = 0.00f;	// �e�N�X�`���̍���Y���W
	float w = 1.0f;		// �e�N�X�`���̕�
	float h = 1.0f;		// �e�N�X�`���̍���
};

//�|���S���n�̕ϐ�
struct polygon {
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �|���S���̍��W
	float w = 1;		//��
	float h = 1;		//����
};






// �v���g�^�C�v�錾
HRESULT Init_game(void);
void Uninit_game(void);
void Update_game(void);
void Draw_game(void);
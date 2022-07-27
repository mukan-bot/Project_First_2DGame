#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

// �}�N����`-------


//�\���̒�`-------

//�e�N�X�`���n�̕ϐ�
struct texture {
	float x = 0.00f;	// �e�N�X�`���̍���X���W
	float y = 0.00f;	// �e�N�X�`���̍���Y���W
	float w = 1.0f;		// �e�N�X�`���̕�
	float h = 1.0f;		// �e�N�X�`���̍���
};

//�|���S���n�̕ϐ�
struct polygon {
	float w = 1.0f;		//��
	float h = 1.0f;		//����
};



struct main_obj{
	texture tex;
	polygon pol;
	bool use = FALSE;
	XMFLOAT2 pos = XMFLOAT2(0.0f, 0.0f);		// �|���S���̍��W

};



// �v���g�^�C�v�錾-------
HRESULT Init_game(void);
void Uninit_game(void);
void Update_game(void);
void Draw_game(void);
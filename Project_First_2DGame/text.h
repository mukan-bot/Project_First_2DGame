#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


// �}�N����`

#define TEXT_MAX	(40)	//�ő啶����


//�\���̒�`
struct TEXT
{
	XMFLOAT3	pos;		// �|���S���̍��W
	float		w, h;		// ���ƍ���

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = 200;	// �G�l�~�[�̕\���ʒuX
	float py = 200;	// �G�l�~�[�̕\���ʒuY
	float pw = 200;		// �G�l�~�[�̕\����
	float ph = 200;		// �G�l�~�[�̕\������

	float tw = 0.016f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.0f;	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W

	bool use = FALSE;
};






// �v���g�^�C�v�錾
void SetText(float x, float y, char text[]);
HRESULT Init_text(void);
void Uninit_text(void);
void Update_text(void);
void Draw_text(void);
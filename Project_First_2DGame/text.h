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

	float tw = 0.0207333333333333f;	// �e�N�X�`���̕�
	float th = 0.0823333333333333f;	// �e�N�X�`���̍���
	float tx = 0.00f;	// �e�N�X�`���̍���X���W
	float ty = 0.00f;	// �e�N�X�`���̍���Y���W
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	bool use = FALSE;
};






// �v���g�^�C�v�錾
HRESULT Init_text(void);
void Uninit_text(void);
void Draw_text(void);
//x���W,y���W,�T�C�Y,�F,�e�L�X�g
void SetText(float x, float y, float size, XMFLOAT4 color, char text[TEXT_MAX]);
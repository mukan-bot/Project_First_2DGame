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
	bool		use;
};






// �v���g�^�C�v�錾
void SetText(float x, float y, char[TEXT_MAX]);

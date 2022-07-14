#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


// �}�N����`
struct TITLE
{
	XMFLOAT3	pos;		// �|���S���̍��W
	float		w, h;		// ���ƍ���
	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	float		scrl;		// �X�N���[��
	XMFLOAT3	old_pos;	// �P�t���O�̃|���S���̍��W
	float		scrl2;		// �X�N���[��
};


#define	SKY_H	(150.0f)	// ��̃|���S���̍���



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTITLE(void);
void UninitTITLE(void);
void UpdateTITLE(void);
void DrawTITLE(void);


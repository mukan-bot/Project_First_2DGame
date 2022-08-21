#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "collision.h"

#include "MAP.h"

//�\���̒�`
struct G_TILE{
	TILE tile;
	COLLISION col;

};


//�v���g�^�C�v�錾
HRESULT Init_Ground_tile(void);
void Uninit_Ground_tile(void);
void Draw_Ground_tile(void);
void Update_Ground_tile(float plus);


int SetG_TILE(int tex_no, XMFLOAT3 pos_size);//�����ڗp�ݒu
bool DelG_TILE(int count);//�����ڗp�폜
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//�v���g�^�C�v�錾
HRESULT Init_Visual_tile(void);
void Uninit_Visual_tile(void);
void Draw_Visual_tile(void);
void Update_Visual_tile();


int SetV_tile(int tex_no, XMFLOAT3 pos_size);//�����ڗp�ݒu
bool DelV_tile(int count);//�����ڗp�폜
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

#include "game.h"

//�\���̒�`
struct TILE {
	main_obj obj;
	int tile_no = -1;
	int anime_speed = 0;//�A�j���V���������t���[�����i�ނ�
	int anime_count = 0;//���̃A�j���[�V�����̂ǂ����Đ����Ă��邩
	int anime_frame = 0;//�A�j���V�������Ȃ�R�}��
};


//�v���g�^�C�v�錾
HRESULT Init_map(void);
void Uninit_map(void);
void Update_map(void);
void Draw_map(void);

bool Set_Scroll(float scroll_f);
float Get_Scroll();


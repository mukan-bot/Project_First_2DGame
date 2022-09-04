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

//�X�N���[���̉����Z
bool Set_Scroll(float scroll_f);
//�X�N���[���̒l�i�v���C���[�̂����W�j�̏���
float Get_Scroll();
//�X�N���[���̒l�i�v���C���[�̂����W�j�̏����i�A�h���X�j
float* Get_aScroll();
//���̃t���[���łǂꂾ���X�N���[�������������̏���
float Get_PlusScroll();


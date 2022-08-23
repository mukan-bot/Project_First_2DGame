#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

// �}�N����`-------
#define PLUS_G (0.01f);//�d�͂̉����x

//�\���̒�`-------

//�A�j���[�V�����n�̕ϐ�
struct animation {
	int anime_FPS = 0;//Animation�̍X�V�p�x
	int count_FPS = 0;//�O��̍X�V���牽�t���[����������
	int anime_frame = 0;//Animation�̍��̃R�}
	int anime_frame_max = 0;//Animation�̍ő�R�}��
	int which_anime = 0;//���ǂ̃A�j���[�V�������Đ����Ă��邩

	int anime_count; //��������
	int anime_number;//�c������

};

struct STATUS {
	float hp;
	float mp;
	float plus_mp;//MP�̉�
	float speed;//�ړ����x
	float jump_speed;
	float gravity;
	float temp_gravity;//�d�͉����x��plus�������Ƃ̒l
	float atk;
	float matk;
};

// �v���g�^�C�v�錾-------
HRESULT Init_game(void);
void Uninit_game(void);
void Update_game(void);
void Draw_game(void);
void Anime_Update(void);

float clamp(float value, float min, float max);
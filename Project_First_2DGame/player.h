#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "collision.h"

//�}�N����`
//�A�j���[�V�����ォ��
enum {
	IDLE_ANIME,
	RUN_ANIME,
	DAMAGE_ANIME,
	ANIME_MAX
};


//�\���̒�`
struct PLAYER {
	main_obj obj;
	COLLISION col;//�U���Ȃ�
	COLLISION col_U;//��
	COLLISION col_D;//��
	COLLISION col_L;//��
	COLLISION col_R;//�E
	animation anime;
	bool is_jump;
	bool is_hitD;
	bool is_hitU;
	bool is_hitL;
	bool is_hitR;
	XMFLOAT2 line_pos;
	XMFLOAT2 line_vec;
};





// �v���g�^�C�v�錾-------
HRESULT Init_player(void);
void Uninit_player(void);
void Update_player(void);
void Draw_player(void);
void Anime_player(void);

void Set_P_Anime(int anime);

STATUS* Get_pStatus(void);
PLAYER* Get_Player(void);
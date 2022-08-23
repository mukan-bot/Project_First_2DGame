#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "collision.h"

//�}�N����`
enum {				//�G�̎��
	ENEMY_HUNTRESS,		//��{�̓G
	ENEMY_TYPE_MAX,	//�G�̎�ނ̐�
};
//�A�j���[�V�����ォ��
enum {
	E_NO_ANIME,
	E_ATK1_ANIME,
	E_ATK2_ANIME,
	E_DEATH_ANIME,
	E_RUN_ANIME,
	E_IDLE_ANIME,
	E_DAMAGE_ANIME,
	E_ANIME_MAX
};

//�\���̒�`
struct ENEMY {
	main_obj obj;
	COLLISION col;//�S��
	COLLISION col_U;//��
	COLLISION col_D;//��
	COLLISION col_L;//��
	COLLISION col_R;//�E
	animation anime;
	bool is_run_R = TRUE;
	bool is_jump;
	bool is_hitD;
	bool is_hitU;
	bool is_hitL;
	bool is_hitR;
	STATUS status;
	int enemy_type;

};
struct ANIME_TYPE {
	int anime_type;
	int frame = 0;
};
struct ENEMY_STATUS {
	STATUS status;
	float size;
	float col_size;
	XMFLOAT2 aspect;//�傫���Ƃ����1.0f�Ƃ���
	main_obj obj;
	COLLISION col;//�S��
	COLLISION col_U;//��
	COLLISION col_D;//��
	COLLISION col_L;//��
	COLLISION col_R;//�E
	animation anime;
	ANIME_TYPE anime_type[E_ANIME_MAX];//�A�j���[�V�����̎�ނ��ォ�珇�Ɏw��

};





// �v���g�^�C�v�錾-------
HRESULT Init_enemy(void);
void Uninit_enemy(void);
void Update_enemy(void);
void Draw_enemy(void);
void Set_E_Anime(int enemy_no, int anime);
int Set_Enemy(int enemy_type,XMFLOAT2 pos);

void Update_eMove(float plus);


ENEMY* Get_Enemy(void);
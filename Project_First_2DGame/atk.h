#pragma once

#include "game.h"

#include "collision.h"

//�}�N����`
enum {
	STANDARD_ATK,			//�ʏ�U��
	LINE_ATK,				//������ɔ��ł����U��
	STANDARD_ATK_ENEMY,		//�v���C���[�������Ĕ��ł����U��
	STANDARD_ATK_BOSS,		//BOSS�p�̍U��
	MAX_ATK,//�U���̎�ނ̍ő吔
};






//�\���̒�`
struct attack {
	main_obj obj;
	COLLISION col;
	XMFLOAT2 start_pos;
	XMFLOAT2 vec;
	int atk_type;	//�ǂ̍U����
	bool is_Rside;	//�E���ɑ΂���U���Ȃ�true
	int count_frame;
	int effect_no;
	int no;
};

struct attack_status {
	XMFLOAT2 vec;			//�U�������ł����ꏊ�iis_Rside��false�̂Ƃ�x���}�C�i�X�ɂ���j
	float atk_power;		//�U���́i�ő�1.0f�j
	float minus_mp;			//����MP�i�ő�1.0f�j
	int frame;				//�t���[�����i���Ȃ��Ƒ����j���Ȃ�����Ɣ��蔲���邩��
	COLLISION col;
	main_obj obj;
	XMFLOAT4 color;
	bool is_g_collision;	//�n�ʂ��ђʂ��邩

};


//�v���g�^�C�v�錾
HRESULT Init_ATK(void);
void Uninit_ATK(void);
void Update_ATK(void);
void Update_sATK(float plus);
void Draw_ATK(void);

float CheckDamage(COLLISION collision);

//�N�ɓ�����U����,�ǂ̍U����,�E���ɑ΂���U���Ȃ�true,���ˏꏊ
float Set_ATK(int hit_type, int atk_type, bool is_Rside, XMFLOAT2 start_pos);

//�U���ʂ̃X�e�[�^�X�̏���
attack_status Get_Atk_status(int atk_type);

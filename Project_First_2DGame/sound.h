//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_main00,	// BGM1
	SOUND_LABEL_BGM_title00,
	SOUND_LABEL_SE_select_mv,
	SOUND_LABEL_SE_select_no,
	SOUND_LABEL_SE_select_ok,

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);//���x���w��
void StopSound(void);//���ׂĒ�~

void Sound_Volume(int label, float vol);
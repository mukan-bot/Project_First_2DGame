//作成者：竹内大翔

#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_main00,	// BGM1
	SOUND_LABEL_BGM_title00,
	SOUND_LABEL_SE_select_mv,
	SOUND_LABEL_SE_select_no,
	SOUND_LABEL_SE_select_ok,
	SOUND_LABEL_SE_plakyer_result,
	SOUND_LABEL_SE_standard_atk,
	SOUND_LABEL_SE_line_atk,
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);//ラベル指定
void StopSound(void);//すべて停止

void Sound_BGM_Volume(float vol);
void Sound_SE_Volume(float vol);

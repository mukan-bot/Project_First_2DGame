#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct BG{
	main_obj obj;
	int texNo = 0;
	float scrollSpeed = 0.0f;

};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

BG* GetBG(void);

void Set_ScrollBG(float set_scroll);



#pragma once
#include "game.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"


struct PLAYER {
	main_obj obj;

};



// �v���g�^�C�v�錾-------
HRESULT Init_player(void);
void Uninit_player(void);
void Update_player(void);
void Draw_player(void);

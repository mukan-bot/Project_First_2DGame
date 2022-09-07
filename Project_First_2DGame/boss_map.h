#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


HRESULT Init_boss_map(void);
void Uninit_boss_map(void);
void Update_boss_map(void);
void Draw_boss_map(void);
void Set_boss();
void white_mv(float plus);
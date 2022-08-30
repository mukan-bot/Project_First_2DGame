#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

#include "game.h"
//マクロ定義
#define SNAP_SIZE (50)
#define MAP_X_SIZE (11)
#define MAP_Y_SIZE (11)

//プロトタイプ宣言
void Init_set_map(void);
void Set_map(float plus);
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

#include "game.h"
//�}�N����`
#define SNAP_SIZE (50)
#define MAP_X_SIZE (11)
#define MAP_Y_SIZE (12)//�\������镪�͂P�P�}�b�v�̐��`�̂��߁{�P
#define MAP_SIZE (10)

//�v���g�^�C�v�錾
void Init_set_map(void);
void Set_map(float plus);
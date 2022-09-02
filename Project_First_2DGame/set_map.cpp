#include "set_map.h"

#include <time.h>

#include "Ground_tile.h"
#include "Visual_tile.h"
#include "MAP.h"
#include "enemy.h"

//マクロ定義
#define MAP_LOAD (3)
enum {
	MAP_TYPE_ENPTY,
	MAP_TYPE_SOIL,
	MAP_TYPE_LEFT,
	MAP_TYPE_FLATTO,
	MAP_TYPE_4,
	MAP_TYPE_MAX,
};


//グローバル変数
static int g_map_tile[MAP_SIZE];
static int g_temp_map[MAP_X_SIZE * MAP_SIZE][MAP_Y_SIZE];
static float g_plus_count = 0;
static int g_plus_map = 0;

static int x = 0;
static int y = 0;

static int g_map[MAP_TYPE_MAX][MAP_Y_SIZE][MAP_X_SIZE] = {
	{//パターン０空白
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	{//パターン１地中
	{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	},
	{//パターン２右端
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
	{ 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	},
	{//パターン３平
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	},
	{//パターン４右端
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0},
	{ 0, 0,13,14,16, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	},
};

//プロトタイプ宣言

void Init_set_map(void) {
	int i = 0;
	g_map_tile[i] = MAP_TYPE_SOIL;
	i++;
	g_map_tile[i] = MAP_TYPE_LEFT;
	i++;
	g_map_tile[i] = MAP_TYPE_FLATTO;
	i++;
	while (i<MAP_SIZE)
	{
		g_map_tile[i] = rand() % (MAP_TYPE_FLATTO - MAP_TYPE_MAX) +MAP_TYPE_FLATTO;
		3;
		i++;

	}



	for (int i = 0; i < MAP_SIZE; i++) {
		for (int i_x = MAP_X_SIZE * i; i_x < MAP_X_SIZE + (MAP_X_SIZE * i); i_x++) {
			for (int i_y = 0-(1*i); i_y < MAP_Y_SIZE; i_y++) {

				x = SNAP_SIZE / 2 + (SNAP_SIZE * (i_x));
				x -= MAP_X_SIZE * SNAP_SIZE;
				y = SNAP_SIZE / 2 + (SNAP_SIZE * i_y);
				y += SNAP_SIZE * i;

				PrintDebugProc("%d:%d\n", i_x, i_y);

				SetG_TILE(g_map[g_map_tile[i]][i_y][i_x], XMFLOAT3(x, y, SNAP_SIZE));
				switch (g_map[g_map_tile[i]][i_y][i_x]){

					//草・岩を生やす
				case(1):
				case(2):
				case(3):
				case(4):
					if (g_map[g_map_tile[i]][i_y + 1][i_x] == 0) {//今の場所（地面）の上が０（空白）だったらランダムで草or石を置く
						if (rand() % 12 == 0) {
							SetV_tile(rand() % 2, XMFLOAT3(x, y - SNAP_SIZE, SNAP_SIZE));
						}
						if (rand() % 30 == 0) {
							Set_Enemy(ENEMY_HUNTRESS, XMFLOAT2(x, y - SNAP_SIZE));
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

}

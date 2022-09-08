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
	MAP_TYPE_5,
	MAP_TYPE_6,
	MAP_TYPE_7,
	MAP_TYPE_8,
	MAP_TYPE_9,
	MAP_TYPE_10,
	MAP_TYPE_11,
	MAP_TYPE_MAX,
};


//グローバル変数
static int g_map_tile[MAP_SIZE];
static int g_temp_map[MAP_X_SIZE * MAP_SIZE][MAP_Y_SIZE];
static float g_plus_count = 0;
static int g_plus_map = 0;



//マップを一塊ごとにまとめる
static short g_map[MAP_TYPE_MAX][MAP_Y_SIZE][MAP_X_SIZE] = {
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
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},
	{//パターン１地中
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
	{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
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
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2},
	},
	{//パターン４
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0},
	{ 0, 0,13,14,16, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 1, 4, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 5, 8, 0, 0},
	{ 2, 2, 2, 2, 2, 2, 2, 6, 6, 2, 2},
	},
	{//パターン５
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 1, 2, 2, 3, 4, 0, 0},
	{ 0, 0, 0, 0, 5, 6, 6, 6, 8, 0, 0},
	{ 0, 0, 0, 0, 9, 6, 6, 6,12, 0, 0},
	{ 2, 2, 2, 2, 6, 6, 6, 6, 6, 2, 2},
	},
	{//パターン６
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0},
	{ 0, 5, 6, 6, 6, 4, 0, 0, 0, 0, 0},
	{ 0, 9, 6, 6, 6, 8, 0, 0, 0, 0, 0},
	{ 0, 9, 6, 6, 6, 8, 0, 0, 0, 0, 0},
	{ 2, 6, 6, 6, 6, 6, 2, 2, 2, 2, 2},
	},
	{//パターン７
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 1, 2, 4, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 5, 6, 8, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 9, 6,12, 0, 0, 0, 0},
	{ 2, 2, 2, 2, 6, 6, 6, 2, 2, 2, 2},
	},
	{//パターン８
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0},
	{ 0, 0, 1, 2, 2, 6, 6, 2, 4, 0, 0},
	{ 0, 0,13,14,15,14,15,14,16, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	},
	{//パターン９
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0},
	{ 0, 0,13,14,16, 0, 1, 2, 4, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 5, 6, 8, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 5, 6, 8, 0, 0},
	{ 2, 2, 2, 2, 2, 2, 6, 6, 6, 2, 2},
	},
	{//パターン１０
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 1, 2, 4, 0, 0},
	{ 0, 0, 1, 4, 0, 0, 5, 6, 8, 0, 0},
	{ 0, 0, 5, 8, 0, 0, 5, 6, 8, 0, 0},
	{ 2, 2, 6, 6, 2, 2, 6, 6, 6, 2, 2},
	},
	{//パターン１１
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 1, 2, 2, 4, 0, 0, 0},
	{ 0, 0, 0, 0, 5, 6, 6, 8, 0, 0, 0},
	{ 0, 0, 1, 2, 6, 6, 6, 8, 0, 0, 0},
	{ 0, 0, 5, 6, 6, 6, 6, 8, 0, 0, 0},
	{ 0, 0, 5, 6, 6, 6, 6, 8, 0, 0, 0},
	{ 2, 2, 6, 6, 6, 6, 6, 6, 2, 2, 2},
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
		g_map_tile[i] = rand() % (MAP_TYPE_FLATTO - MAP_TYPE_MAX) + MAP_TYPE_FLATTO;
		i++;
	}

	g_map_tile[MAP_SIZE - 3] = MAP_TYPE_FLATTO;
	g_map_tile[MAP_SIZE - 2] = MAP_TYPE_FLATTO;
	g_map_tile[MAP_SIZE - 1] = MAP_TYPE_FLATTO;

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int iy = 0; iy < MAP_Y_SIZE; iy++) {
			for (int ix = 0; ix < MAP_X_SIZE; ix++) {
				g_temp_map[ix+(MAP_X_SIZE*i)][iy] = g_map[g_map_tile[i]][iy][ix];
			}
		}
	}

	int x = SNAP_SIZE/2;
	int y = SNAP_SIZE/2;
	int temp = SNAP_SIZE - (SCREEN_HEIGHT % SNAP_SIZE);
	int count = 0;
	for (int iy = 0; iy < MAP_Y_SIZE; iy++) {

		y = SNAP_SIZE / 2 + (iy * SNAP_SIZE);

		for (int ix = 0; ix < MAP_X_SIZE * MAP_SIZE; ix++) {

			x = SNAP_SIZE / 2 + (ix * SNAP_SIZE);
			x -= MAP_X_SIZE * SNAP_SIZE;

			SetG_TILE(g_temp_map[ix][iy], XMFLOAT3(x, y-temp, SNAP_SIZE + 1));

			//敵の表示
			if (ix > MAP_X_SIZE * 3) {
				if (g_temp_map[ix][iy] == 2) {
					if (rand() % 5 == 0) Set_Enemy(ENEMY_HUNTRESS, XMFLOAT2(x, y));
				}
			}

			//草と岩の配置（見た目）
			if (g_temp_map[ix][iy] == 2) {
				if (rand() % 3 == 0) SetV_tile(0, XMFLOAT3(x, (y - temp) - SNAP_SIZE, SNAP_SIZE + 1));
				else if (rand() % 5 == 0)SetV_tile(1, XMFLOAT3(x, (y - temp) - SNAP_SIZE, SNAP_SIZE + 1));
			}
		}

	}




}

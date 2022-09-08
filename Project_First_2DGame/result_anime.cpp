#include "main.h"
#include "result_anime.h"
#include "player.h"
#include "effect.h"
#include "sound.h"

//マクロ定義
enum {
	SCENE_MOVE_PLAYER,
	SCENE_ATK_START,
	SCENE_ATK_MOVE,
	SCENE_OK,
};
#define PLAYER_SPEED (180) //フレーム数
#define EFFECT_SET_MAX (20)
#define ATK_SPEED (30)
#define ATK_MOVE_SPEED (60)

//グローバル変数
static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

static PLAYER* g_player;
static XMFLOAT2 g_vec;
static XMFLOAT2 g_vec_temp;
static XMFLOAT2 g_effect_pos[EFFECT_SET_MAX];
static XMFLOAT2 g_effect_temp_pos[EFFECT_SET_MAX];
static XMFLOAT2 g_effect_temp2_pos[EFFECT_SET_MAX];
static int g_effect_count[EFFECT_SET_MAX];
static bool g_effect_use[EFFECT_SET_MAX];
static XMFLOAT2 g_effect_vec;
static int g_scene = SCENE_MOVE_PLAYER;
static int g_count;
static bool g_go_score;
static bool g_player_sound;

HRESULT Init_result_anime(void) {
	Init_player();
	Init_effect();
	g_player = Get_Player();
	g_player->obj.pos.x = -g_player->obj.pol.w;
	g_player->obj.pos.y = (SCREEN_HEIGHT - g_player->obj.pol.h / 2);
	g_vec = XMFLOAT2(150.0f, 100.0f);
	g_vec_temp = g_player->obj.pos;
	g_scene = SCENE_MOVE_PLAYER;
	g_count = 0;
	g_effect_vec = XMFLOAT2(240, 300);
	g_go_score = FALSE;
	for (int i = 0; i < EFFECT_SET_MAX; i++) {
		g_effect_pos[i] = XMFLOAT2((rand() % SCREEN_WIDTH)- g_effect_vec.x, rand() % (SCREEN_HEIGHT / 2));
		g_effect_temp_pos[i] = XMFLOAT2(g_effect_pos[i].x + g_effect_vec.x, g_effect_pos[i].y + g_effect_vec.y);
		g_effect_count[i] = 0;
		g_effect_use[i] = FALSE;
	}
	g_player_sound = FALSE;

	SCORE* temp = Get_score();
	if (!temp->is_clear) {
		g_scene = SCENE_OK;
	}

	g_Load = TRUE;
	return S_OK;
}
void Uninit_result_anime(void) {
	if (g_Load == FALSE) return;
	Uninit_player();
	Uninit_effect();

	g_Load = FALSE;
}
bool Update_result_anime(void) {
	int atk_i = 0;
	int temp;
	bool tbool = FALSE;
	switch (g_scene)
	{
	//プレイヤーを移動させる
	case(SCENE_MOVE_PLAYER):
		g_player->obj.pos.x = g_vec_temp.x + (g_vec.x - g_vec_temp.x) / PLAYER_SPEED * g_count;
		g_player->obj.pos.y = g_vec_temp.y + (g_vec.x - g_vec_temp.y) / PLAYER_SPEED * g_count;
		if (g_count > PLAYER_SPEED) {
			g_count = 0;
			g_scene = SCENE_ATK_START;
			for (int i = 0; i < EFFECT_SET_MAX; i++) {
				g_effect_temp2_pos[i] = g_effect_pos[i];
			}
		}
		g_count++;
		break;
	//プレイヤーに攻撃を送信する
	case(SCENE_ATK_START):
		if (g_count == ATK_SPEED) {
			for (int i = 0; i < EFFECT_SET_MAX; i++) {
				Set_effect(&g_effect_pos[i], TRUE, 100, EFFECT_TYPE_FIRE_BULLET_3, FALSE, ATK_MOVE_SPEED);
			}
			g_scene = SCENE_ATK_MOVE;
			g_count = 0;
		}
		g_count++;
		break;
	//攻撃を移動させる
	case(SCENE_ATK_MOVE):
		for (int i = 0; i < EFFECT_SET_MAX; i++) {
			g_effect_pos[i].x = g_effect_temp2_pos[i].x + (g_effect_temp_pos[i].x - g_effect_temp2_pos[i].x) / ATK_MOVE_SPEED * g_count;
			g_effect_pos[i].y = g_effect_temp2_pos[i].y + (g_effect_temp_pos[i].y - g_effect_temp2_pos[i].y) / ATK_MOVE_SPEED * g_count;
		}
		g_count++;
		if (g_count > ATK_MOVE_SPEED) {
			g_scene = SCENE_OK;
			g_count = 0;
			for (int i = 0; i < EFFECT_SET_MAX; i++) {
				Set_effect(&g_effect_pos[i], TRUE, rand() % 300, EFFECT_TYPE_EXPLOSION, FALSE, rand() % ATK_MOVE_SPEED);
				g_effect_count[i] = rand() % ATK_MOVE_SPEED + 10;
			}
		}
		break;
	//アニメーションの完了
	case (SCENE_OK):

		SCORE* temp_s = Get_score();

		if (temp_s->is_clear) {
			for (int i = 0; i < EFFECT_SET_MAX; i++) {
				if (g_effect_count[i] >= (rand() % ATK_MOVE_SPEED + 20) + ATK_MOVE_SPEED) {
					Set_effect(&g_effect_pos[i], TRUE, rand() % 300, EFFECT_TYPE_FIRE_BULLET_4, FALSE, rand() % ATK_MOVE_SPEED);
					g_effect_count[i] = 0;
				}
				g_effect_count[i]++;
			}
		}

		if (!g_go_score) {
			g_go_score = TRUE;
			tbool = TRUE;
			g_player_sound = TRUE;     
		}
		break;

	}
	if (g_player_sound) {
		PlaySound(SOUND_LABEL_SE_plakyer_result);
		g_player_sound = FALSE;
	}
	return tbool;
}
void Draw_result_anime(void) {
	Update_player();
	Update_effect();
	Draw_player();
	Draw_effect();
}
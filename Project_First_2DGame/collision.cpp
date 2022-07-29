#include "collision.h"
#include "text.h"

//マクロ定義
#define MAX_COLLISION (256)


//グローバル変数
COLLISION* g_collision[MAX_COLLISION];
//今何個判定を作ったか数える
int g_collision_count;



HRESULT Init_collision(void) {
	g_collision_count = 0;
	return S_OK;
}

//戻り値-1は生成失敗（成功はg_collision_countの値）
int SetCollision(COLLISION* collision) {
	if (g_collision_count < MAX_COLLISION) {
		collision->specific_numbers = g_collision_count;
		g_collision[g_collision_count] = collision;
		g_collision[g_collision_count]->specific_numbers = g_collision_count;
		g_collision_count++;
		return g_collision_count - 1;
	}
	return -1;
}


bool GetCollision(COLLISION collision) {
	
	//値を使いやすいようにまとめておくcollision
	XMFLOAT4 temp;
	float pos_pos2_range;
	if (collision.shape == BOX) {
		//左,右,上,下
		temp = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
	}

	for (int i = 0; i <= g_collision_count; i++) {

		//typeが一致していない場合iに＋１して言って一致するものを探す or 識別ナンバーが同じ当たり判定をスキップ
		while ((g_collision[i]->type != collision.type)||(g_collision[i]->specific_numbers==collision.specific_numbers)) {
			

			if (i < g_collision_count) {
				return FALSE;
			}
			i++;
		}
		//値を使いやすいようにまとめておくg_collision
		XMFLOAT4 temp_g;
		//左,右,上,下
		temp_g = XMFLOAT4(g_collision[i]->pos.x - (g_collision[i]->size.x / 2.0f), g_collision[i]->pos.x + (g_collision[i]->size.x / 2.0f), g_collision[i]->pos.y - (g_collision[i]->size.y / 2.0f), g_collision[i]->pos.y + (g_collision[i]->size.y / 2.0f));

		if (g_collision[i]->shape == BOX) {
			//BOX対BOX
			if (collision.shape == BOX) {
				if ((temp_g.x < temp.y) && (temp_g.y > temp.x) && (temp_g.z < temp.w) && (temp_g.w > temp.z)) {
					return TRUE;
				}
			} 		
		}
	}
	return FALSE;
}
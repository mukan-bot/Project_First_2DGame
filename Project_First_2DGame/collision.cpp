#include "collision.h"
#include "text.h"

//マクロ定義
#define MAX_COLLISION (256)
#define PI (3.14159265f)//円周率

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
	XMFLOAT4 temp_c;
	if (collision.shape == BOX) {
		//左,右,上,下
		temp_c = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
	}
	else if (collision.shape == CAPSULE) {
		
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
				if ((temp_g.x < temp_c.y) && (temp_g.y > temp_c.x) && (temp_g.z < temp_c.w) && (temp_g.w > temp_c.z)) {
					return TRUE;
				}
			} 	
			else if (collision.shape == CAPSULE) {
				XMFLOAT2 pos_g[5];	//BOXの四隅と中心の座標を入れておく
				pos_g[0] = g_collision[i]->pos;	//中心点
				pos_g[1] = XMFLOAT2(g_collision[i]->pos.x - g_collision[i]->size.x / 2, g_collision[i]->pos.y - g_collision[i]->size.y / 2);	//左上
				pos_g[2] = XMFLOAT2(g_collision[i]->pos.x + g_collision[i]->size.x / 2, g_collision[i]->pos.y - g_collision[i]->size.y / 2);	//右上
				pos_g[3] = XMFLOAT2(g_collision[i]->pos.x - g_collision[i]->size.x / 2, g_collision[i]->pos.y + g_collision[i]->size.y / 2);	//左下
				pos_g[4] = XMFLOAT2(g_collision[i]->pos.x + g_collision[i]->size.x / 2, g_collision[i]->pos.y + g_collision[i]->size.y / 2);	//右下
				
				//四隅との判定
				for (int i = 1; i < 5; i++) {
					float dx = pos_g[i].x - collision.pos.x;				// ⊿ｘ
					float dy = pos_g[i].y - collision.pos.y;				// ⊿ｙ
					float t = (collision.vec.x * dx + collision.vec.y * dy) / (collision.vec.x * collision.vec.x + collision.vec.y * collision.vec.y);
					t = clamp(t, 0.0f, 1.0f);
					XMFLOAT2 min;
					min.x = collision.vec.x * t + collision.pos.x;
					min.y = collision.vec.y * t + collision.pos.y;
					float fDistSpr = (min.x - pos_g[i].x) * (min.x - pos_g[i].x) + (min.y - pos_g[i].y) * (min.y - pos_g[i].y); //距離の二乗

					if (fDistSpr < collision.size.x) {
						//OutputDebugString("te");
						return TRUE;
					}
				}
				//中心との判定大きい判定の時のすり抜け防止
				float dx = pos_g[0].x - collision.pos.x;				// ⊿ｘ
				float dy = pos_g[0].y - collision.pos.y;				// ⊿ｙ
				float t = (collision.vec.x * dx + collision.vec.y * dy) / (collision.vec.x * collision.vec.x + collision.vec.y * collision.vec.y);
				t = clamp(t, 0.0f, 1.0f);
				XMFLOAT2 min;
				min.x = collision.vec.x * t + collision.pos.x;
				min.y = collision.vec.y * t + collision.pos.y;
				float fDistSpr = (min.x - pos_g[0].x) * (min.x - pos_g[0].x) + (min.y - pos_g[0].y) * (min.y - pos_g[0].y); //距離の二乗

				if (fDistSpr < collision.size.x + g_collision[i]->size.x) {
					OutputDebugString("te");
					return TRUE;
				}
				//else if (fDistSpr < collision.size.x + g_collision[i]->size.y * collision.size.x + g_collision[i]->size.y) {
				//	//OutputDebugString("te");
				//	return TRUE;
				//}

			}
		}
	}
	return FALSE;
}
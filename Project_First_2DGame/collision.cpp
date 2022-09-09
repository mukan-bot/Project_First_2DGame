//作成者：竹内大翔

#include "collision.h"
#include "text.h"
#include "main.h"

#include "atk.h"

//マクロ定義
#define MAX_COLLISION (16384)	//当たり判定の限界

//グローバル変数
COLLISION g_collision[MAX_COLLISION];
//今何個判定を作ったか数える
int g_collision_count=0;



HRESULT Init_collision(void) {
	g_collision_count = 0;
	return S_OK;
}

//戻り値-1は生成失敗（成功はg_collision_countの値）
int SetCollision(COLLISION* collision) {
	if (g_collision_count < MAX_COLLISION) {
		if (g_collision[g_collision_count].specific_numbers == -1) {
			collision->specific_numbers = g_collision_count;
			g_collision[g_collision_count] = *collision;
			g_collision[g_collision_count].specific_numbers = g_collision_count;
			g_collision_count++;
			return g_collision_count - 1;
		}
	}
	for (int i = 0; i < MAX_COLLISION; i++) {//削除したコリジョンの番号を使うため
		if (g_collision[i].specific_numbers == -1) {
			collision->specific_numbers = i;
			g_collision[i] = *collision;
			g_collision[i].specific_numbers = i;
			return i;
		}
	}

	return -1;
}

//判定の削除
bool delete_Collision(int numbers) {
	if ((numbers < MAX_COLLISION) && g_collision[numbers].specific_numbers != -1) {
		g_collision[numbers].specific_numbers = -1;
		return TRUE;
	}
	return FALSE;
}

bool CheckHit(COLLISION collision) {
	//値を使いやすいようにまとめておくcollision
	XMFLOAT4 temp_c;
	XMFLOAT2 pos_c[4];	//BOXの四隅の座標を入れておく
	if (collision.shape == BOX) {
		//左,右,上,下
		temp_c = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));

		pos_c[0] = XMFLOAT2(collision.pos.x - collision.size.x / 2, collision.pos.y - collision.size.y / 2);	//左上
		pos_c[1] = XMFLOAT2(collision.pos.x + collision.size.x / 2, collision.pos.y - collision.size.y / 2);	//右上
		pos_c[2] = XMFLOAT2(collision.pos.x - collision.size.x / 2, collision.pos.y + collision.size.y / 2);	//左下
		pos_c[3] = XMFLOAT2(collision.pos.x + collision.size.x / 2, collision.pos.y + collision.size.y / 2);	//右下
	}
	else if (collision.shape == CIRCLE) {

	}

	for (int i = 0; i <= MAX_COLLISION; i++) {
		if (g_collision[i].specific_numbers != -1) {//使用していない物はスルー
			if ((g_collision[i].type == collision.type) && (g_collision[i].specific_numbers != collision.specific_numbers)){
				//値を使いやすいようにまとめておくg_collision
				XMFLOAT4 temp_g;
				//左,右,上,下
				temp_g = XMFLOAT4(g_collision[i].pos.x - (g_collision[i].size.x / 2.0f), g_collision[i].pos.x + (g_collision[i].size.x / 2.0f), g_collision[i].pos.y - (g_collision[i].size.y / 2.0f), g_collision[i].pos.y + (g_collision[i].size.y / 2.0f));

				if (g_collision[i].shape == BOX) {
					//BOX対BOX
					if (collision.shape == BOX) {
						if ((temp_g.x < temp_c.y) && (temp_g.y > temp_c.x) && (temp_g.z < temp_c.w) && (temp_g.w > temp_c.z)) {
							return TRUE;
						}
					}
					//BOX対CIRCLE
					else if (collision.shape == CIRCLE) {
						XMFLOAT2 pos_g[4];	//BOXの四隅の座標を入れておく
						pos_g[0] = XMFLOAT2(g_collision[i].pos.x - g_collision[i].size.x / 2, g_collision[i].pos.y - g_collision[i].size.y / 2);	//左上
						pos_g[1] = XMFLOAT2(g_collision[i].pos.x + g_collision[i].size.x / 2, g_collision[i].pos.y - g_collision[i].size.y / 2);	//右上
						pos_g[2] = XMFLOAT2(g_collision[i].pos.x - g_collision[i].size.x / 2, g_collision[i].pos.y + g_collision[i].size.y / 2);	//左下
						pos_g[3] = XMFLOAT2(g_collision[i].pos.x + g_collision[i].size.x / 2, g_collision[i].pos.y + g_collision[i].size.y / 2);	//右下
						float temp;
						for (int j = 0; j < 8; j += 2) {
							temp = distance_line_point(pos_g[j], XMFLOAT2(pos_g[j + 1].x - pos_g[j].x, pos_g[j + 1].y - pos_g[j].y), collision.pos);
							if (temp < collision.size.x * collision.size.x) {
								return TRUE;
							}
						}
					}
				}
				else {
					//CIRCLE対BOX
					if (collision.shape == BOX) {
						float temp;
						for (int j = 0; j < 8; j += 2) {
							temp = distance_line_point(pos_c[j], XMFLOAT2(pos_c[j + 1].x - pos_c[j].x, pos_c[j + 1].y - pos_c[j].y), g_collision[i].pos);
							if (temp < g_collision[i].size.x * g_collision[i].size.x) {
								return TRUE;
							}
						}

					}
					//CIRCLE対CIRCLE
					else if (collision.shape == CIRCLE) {
						float temp = distance_point(g_collision[i].pos, collision.pos);
						float r = g_collision[i].size.x + collision.size.x;
						if (temp < r * r) {
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

 



bool CheckHit_lines(XMFLOAT2 pos1, XMFLOAT2 vec1, XMFLOAT2 pos2, XMFLOAT2 vec2) {


	//平行の対処:少し無理やりだけど画面範囲考えたら誤差
	if (vec1.x == 0.0f) {
		vec1.x += 0.001f;
	}
	if (vec1.y == 0.0f) {
		vec1.y += 0.001f;
	}
	if (vec2.x == 0.0f) {
		vec2.x += 0.001f;
	}
	if (vec2.y == 0.0f) {
		vec2.y += 0.001f;
	}


	float t1 = (vec1.y - pos1.y) / (vec1.x - pos1.x);
	float t2 = (vec2.y - pos2.y) / (vec2.x - pos2.x);

	float x = 0.0f;
	float y = 0.0f;

	if (t1 != t2) {
		x = (pos2.y - pos1.y + t1 * pos1.x - t2 * pos2.x) / (t1 - t2);
		y = t1 * (x - pos1.x) + pos1.y;
	}
	float r1 = (x - pos1.x) / (vec1.x - pos1.x);
	float r2 = (x - pos2.x) / (vec2.x - pos2.x);

	r1 = r1 / SCREEN_WIDTH;
	r2 = r2 / SCREEN_WIDTH;

	bool hit = ((0 < r1) && (r1 < 1) && (0 < r2) && (r2 < 1));
	return hit;
}


float distance_line_point(XMFLOAT2 pos, XMFLOAT2 vec, XMFLOAT2 point) {
	float dx, dy;// 位置の差分
	float t;
	float mx, my;// 最小の距離を与える座標

	float fDistSqr;

	dx = point.x - pos.x;// ⊿ｘ
	dy = point.y - pos.y;// ⊿ｙ
	t = clamp((vec.x * dx + vec.y * dy) / (vec.x * vec.x + vec.y * vec.y), 0.0f, 1.0f);
	mx = vec.x * t + pos.x;	// 最小位置を与える座標
	my = vec.y * t + pos.y;
	fDistSqr = (mx - point.x) * (mx - point.x) +(my - point.y) * (my - point.y);	
	fDistSqr = fDistSqr / SCREEN_WIDTH;
	return fDistSqr;
}

float distance_point(XMFLOAT2 pos1, XMFLOAT2 pos2) {
	float			dx, dy;// 位置の差分

	float			fDistSqr;

	dx = pos1.x - pos2.x;// ⊿ｘ
	dy = pos1.y - pos2.y;// ⊿ｙ
	fDistSqr = dx * dx + dy * dy;// 距離の２乗

	return fDistSqr;
}



void Update_Collision(float plus) {//マップのスクロールに合わせた移動
	for (int i = 0; i < MAX_COLLISION; i++) {
		if (g_collision[i].specific_numbers != -1) {
			g_collision[i].pos.x -= plus;
		}
	}
}


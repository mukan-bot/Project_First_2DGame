#include "collision.h"

//マクロ定義
#define MAX_COLLISION (256)


//グローバル変数
COLLISION g_collision[MAX_COLLISION];
//判定に使う値を入れておく
XMFLOAT4 g_collision_range[MAX_COLLISION];
//今何個判定を作ったか数える
int g_collision_count;



HRESULT Init_collision(void) {
	g_collision_count = 0;
	return S_OK;
}

//戻り値-1は生成失敗（成功はg_collision_countの値）
int SetCollision(COLLISION collision) {
	if (g_collision_count < MAX_COLLISION) {
		g_collision[g_collision_count] = collision;
		g_collision[g_collision_count].specific_numbers = g_collision_count;
		//判定に使う値をまとめておく
		switch (collision.shape)
		{
		case (BOX):
			//                                              左端                                    右端                                    上                                   下
			g_collision_range[g_collision_count] = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x - (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
			break;

		case (CIRCLE):
			//                                              ポジションｘ　　ポジションｙ　　半径　　　　　　  　 半径
			g_collision_range[g_collision_count] = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x / 2.0f, 0.0f);
			break;

		case (CAPSULE):
			//                                              ポジションｘ　　ポジションｙ　　長さ　　　　　　　　　　　　　　　　幅
			g_collision_range[g_collision_count] = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x - collision.size.y, collision.size.y);
		}

		g_collision_count++;
		return g_collision_count - 1;
	}
	return -1;
}


bool GetCollision(COLLISION collision) {
	//判定に使う値をまとめておく
	XMFLOAT4 collision_range;
	switch (collision.shape)
	{
	case (BOX):
		//                         左端                                    右端                                        上                                   下
		collision_range = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x - (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));
		break;

	case (CIRCLE):
		//                         ポジションｘ　　ポジションｙ　　半径　　　　　　  　 
		collision_range = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x / 2.0f, 0.0f);
		break;

	case (CAPSULE):
		//                         ポジションｘ　　ポジションｙ　　長さ　　　　　　　　　　　　　　　　幅
		collision_range = XMFLOAT4(collision.pos.x, collision.pos.y, collision.size.x - collision.size.y, collision.size.y);
	}
	for (int i = 0; i < g_collision_count; i++) {
		//typeが一致していない場合iに＋１して言って一致するものを探す
		while (g_collision[i].type != collision.type){
			i++;
		}

		if (g_collision[i].shape == BOX) {
			if ((g_collision_range[i].x < collision_range.y) && (g_collision_range[i].y > collision_range.x) && (g_collision_range[i].z < collision_range.w) && (g_collision_range[i].w > collision_range.z)) {
				return TRUE;
			}
		}
		else if (g_collision[i].shape == CIRCLE) {
			if (collision.shape == BOX) {
				XMFLOAT4 tempBox = XMFLOAT4(g_collision_range[i].x + collision_range.z, g_collision_range[i].y + collision_range.z, g_collision_range[i].z + collision_range.z, g_collision_range[i].w + collision_range.z);
				float range = (g_collision_range[i].x - collision_range.x) * (g_collision_range[i].x - collision_range.x) + (g_collision_range[i].y - collision_range.y) * (g_collision_range[i].y - collision_range.y);

			}
			else if (collision.shape == CIRCLE) {
				float range = (g_collision_range[i].x - collision_range.x) * (g_collision_range[i].x - collision_range.x) + (g_collision_range[i].y - collision_range.y) * (g_collision_range[i].y - collision_range.y);
				if (range <= g_collision_range[i].z + collision_range.z) {
					return TRUE;
				}
			}
		}
		else if (g_collision[i].shape == CAPSULE) {

		}
	}
	return FALSE;
}
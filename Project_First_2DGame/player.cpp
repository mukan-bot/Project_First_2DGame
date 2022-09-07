 #include "player.h"
#include "input.h"

#include "MAP.h"
#include "Visual_tile.h"
#include "Ground_tile.h"
#include "HUD.h"
#include "atk.h"
#include "effect.h"

#include "camera.h"
//マクロ定義
#define TEXTURE_MAX	(2)
#define ANIME_NUMBER (3.0f)//縦分割数
#define ANIME_COUNT (8.0f) //横分割数

//アニメーション
#define IDLE_FRAME (6)
#define RUN_FRAME (8)
#define DAMAGE_FRAME (3)
#define ANIME_FPS (20)




#define SIZE	(100.0f)

#define COL_SIZE_W (30)//横
#define COL_SIZE_H (90)//縦

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/player.png",
	"data/TEXTURE/test.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ



static PLAYER g_Player;
static bool g_is_run_R = TRUE;
static STATUS g_status;
static int g_result_count;	//リザルトに入るまでのカウント
static XMFLOAT2 g_delete_effect;	//死亡時のエフェクトの表示場所を保存しておく


static int g_anime_frame[ANIME_MAX] = { IDLE_FRAME,
										RUN_FRAME,
										DAMAGE_FRAME };

//プロトタイプ宣言
void Update_col(void);




HRESULT Init_player(void) {
	ID3D11Device* pDevice = GetDevice();

	//テクスチャの生成
	for (int i = 0; i < TEXTURE_MAX; i++) {
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
		g_TexturName[i],
		NULL,
		NULL,
		&g_Texture[i],
		NULL);
	}
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	


	//player変数の初期化
	g_Player.obj.pos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	g_Player.obj.use = TRUE;
	g_Player.obj.pol.w = SIZE;
	g_Player.obj.pol.h = SIZE;
	g_Player.obj.tex.x = 0.0f;
	g_Player.obj.tex.y = 0.0f;
	g_Player.obj.tex.w = 1.0f/ANIME_COUNT;
	g_Player.obj.tex.h = 1.0f/ ANIME_NUMBER;

	g_Player.is_hitD = FALSE;
	g_Player.is_hitU = FALSE;
	g_Player.is_hitL = FALSE;
	g_Player.is_hitR = FALSE;

	g_Player.is_jump = FALSE;


	//ステータスの初期化
	g_status.hp = 1.0f;
	g_status.mp = 1.0f;
	g_status.plus_mp = 0.005f;
	g_status.matk = 1.5f;
	g_status.speed = 0.003;
	g_status.jump_speed = 4.5;
	g_status.gravity = 3;
	g_status.temp_gravity = 0;



	//当たり判定
	//当たり判定全体
	g_Player.col.pos = g_Player.obj.pos;
	g_Player.col.shape = BOX;
	g_Player.col.size = XMFLOAT2(COL_SIZE_W, COL_SIZE_H);
	g_Player.col.type = ATK_ENEMY;
	//当たり判定下
	g_Player.col_D.pos.x = g_Player.obj.pos.x;
	g_Player.col_D.pos.y = g_Player.obj.pos.y + COL_SIZE_H / 2;
	g_Player.col_D.shape = BOX;
	g_Player.col_D.size = XMFLOAT2(COL_SIZE_W - 10, 0.01f);//-10は必要のないものと当たらないようにするため
	g_Player.col_D.type = GROUND;
	//当たり判定上
	g_Player.col_U.pos.x = g_Player.obj.pos.x;
	g_Player.col_U.pos.y = g_Player.obj.pos.y - COL_SIZE_H / 2;
	g_Player.col_U.shape = BOX;
	g_Player.col_U.size = XMFLOAT2(COL_SIZE_W - 10, 0.01f);//-10は必要のないものと当たらないようにするため
	g_Player.col_U.type = GROUND;	
	//当たり判定右
	g_Player.col_R.pos.x = g_Player.obj.pos.y;
	g_Player.col_R.pos.y = g_Player.obj.pos.x + COL_SIZE_W / 2;
	g_Player.col_R.shape = BOX;
	g_Player.col_R.size = XMFLOAT2(0.01f, COL_SIZE_H - 10);//-10は必要のないものと当たらないようにするため
	g_Player.col_R.type = GROUND;
	//当たり判定左
	g_Player.col_L.pos.x = g_Player.obj.pos.y;
	g_Player.col_L.pos.y = g_Player.obj.pos.x - COL_SIZE_W / 2;
	g_Player.col_L.shape = BOX;
	g_Player.col_L.size = XMFLOAT2(0.01f, COL_SIZE_H - 10);//-10は必要のないものと当たらないようにするため
	g_Player.col_L.type = GROUND;
	//ライン
	g_Player.line_pos = g_Player.obj.pos;
	g_Player.line_pos.y -= SIZE / 2;
	g_Player.line_vec = XMFLOAT2(0.0f, SIZE);

	//Animation
	g_Player.anime.anime_frame = 0;
	g_Player.anime.anime_FPS = ANIME_FPS;
	g_Player.anime.count_FPS = 0;
	g_Player.anime.which_anime = IDLE_ANIME;

	g_result_count = 0;
	
	Init_HUD();

	return S_OK;
}



void Uninit_player(void) {
	if (g_Load == FALSE) return;
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++){
		if (g_Texture[i])		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	g_Load = FALSE;
}

void Update_player(void) {
	if (GetMode() == MODE_GAME) {

		//HPの減算
		float temp_hp = CheckDamage(g_Player.col);
		if (temp_hp > 0.0f) {
			g_status.hp -= temp_hp;
			Set_P_Anime(DAMAGE_ANIME);
		}

		//ＨＰが０の場合リザルトモードに
		if (g_status.hp < 0.0f) {
			if (g_result_count == 140) {
				SCORE* temp = Get_score();
				temp->is_clear = FALSE;
				SetMode(MODE_RESULT);
			}
			else if (g_result_count == 0) {
				g_delete_effect = g_Player.obj.pos;
				g_Player.obj.pos = XMFLOAT2(-200, -200);//プレイヤーを画面外に出す（表示を消す）
				Set_effect(&g_delete_effect, g_is_run_R, g_Player.obj.pol.h*2, EFFECT_TYPE_PURPLE_EXPLOSION, FALSE, 60);
			}
			g_result_count++;
		}


		XMFLOAT2 temp = g_Player.obj.pos;
		//プレイヤーがどこかにあたっているか
		if (CheckHit(g_Player.col_D)) {
			g_Player.is_hitD = TRUE;
		}
		else {
			g_Player.is_hitD = FALSE;
		}
		if (CheckHit(g_Player.col_U)) {
			g_Player.is_hitU = TRUE;
		}
		else {
			g_Player.is_hitU = FALSE;
		}
		if (CheckHit(g_Player.col_L)) {
			g_Player.is_hitL = TRUE;
		}
		else if (g_Player.obj.pos.x < g_Player.obj.pol.w/4) {
			g_Player.is_hitL = TRUE;
		}
		else {
			g_Player.is_hitL = FALSE;
		}
		if (CheckHit(g_Player.col_R)) {
			g_Player.is_hitR = TRUE;
		}
		else {
			g_Player.is_hitR = FALSE;
		}


		if (GetKeyboardPress(DIK_D)) {
			Set_P_Anime(RUN_ANIME);
			g_is_run_R = TRUE;
			if (!g_Player.is_hitR) {
				if (g_Player.obj.pos.x < SCREEN_WIDTH / 2) {
					g_Player.obj.pos.x += g_status.speed * 1000;
				}
				else {
					Set_Scroll(g_status.speed);
				}
			}
		}
		else if (GetKeyboardPress(DIK_A)) {
			Set_P_Anime(RUN_ANIME);
			g_is_run_R = FALSE;
			if (!g_Player.is_hitL) {
				
				float* temp = Get_aScroll();
				if (*temp <= 0.0f) {
					*temp = 0.0f;
					g_Player.obj.pos.x -= g_status.speed*1000;
					
				}
				else {
					Set_Scroll(-g_status.speed);
				}
			}
		}

		//移動していない時はアイドルアニメーションを再生
		else {
			Set_P_Anime(IDLE_ANIME);
		}

		//ジャンプや空中にいるときの判定
		{
			if (((GetKeyboardPress(DIK_W)) || GetKeyboardPress(DIK_SPACE)) && (g_status.mp > 0.0f)) {
				if (!g_Player.is_jump) {
					//ジャンプのはじめに0.2のMPがないとジャンプできない
					if (g_status.mp > 0.2f) {
						if (!g_Player.is_hitU) {//上が当たってたらジャンプできない
							g_Player.is_jump = TRUE;
						}
					}
				}
				else {
					g_Player.obj.pos.y -= g_status.jump_speed;
					g_status.mp -= 0.01;
					g_status.temp_gravity = 0.0f;
				}
			}
			else {
				g_Player.is_jump = FALSE;
			}

			//ジャンプ中でなくて地面にいないときは下に下がる
			if ((!g_Player.is_hitD) && (!g_Player.is_jump)) {
				float temp = g_status.gravity + g_status.temp_gravity;
				g_Player.obj.pos.y += temp;
				g_status.temp_gravity += PLUS_G;
			}
			//地面にいるとき
			else {
				g_status.temp_gravity = 0.0f;//加算されてた値をもとに戻す

			}
		}


		//攻撃
		if (GetKeyboardTrigger(DIK_E)) {
			
			g_status.mp -= Set_ATK(ATK_PLAYER, STANDARD_ATK, g_is_run_R, g_Player.obj.pos);
		}
		if (GetKeyboardTrigger(DIK_Q)) {
			g_status.mp -= Set_ATK(ATK_PLAYER, LINE_ATK, g_is_run_R, g_Player.obj.pos);
		}



		//mpが１以下で地面にいるときMPを回復
		if (g_status.mp <= 1.0f && g_Player.is_hitD) {
			g_status.mp += g_status.plus_mp;
		}

		if (g_status.mp < 0.0f) {//mpがマイナスにならないように
			g_status.mp = 0.0f;
		}

		//移動があった場合めり込んでいないか確認
		//めり込んでた場合移動
		if (g_Player.obj.pos.x != temp.x) {
			if (g_Player.obj.pos.x < temp.x) {
				if (g_Player.is_hitL) {
					g_Player.obj.pos.x += g_status.speed;
				}
			}
			else {
				if (g_Player.is_hitR) {
					Set_Scroll(-g_status.speed);
				}
			}
		}
		else {
			Set_Scroll(0.0f);
		}
		if (g_Player.obj.pos.y != temp.y) {
			if (g_Player.obj.pos.y < temp.y) {
				if (g_Player.is_hitU) {
					g_Player.obj.pos.y += g_status.gravity;
				}
			}
			else {
				if (g_Player.is_hitD) {
					g_Player.obj.pos.y -= g_status.gravity;
				}
			}
		}


		//コリジョンの更新
		Update_col();

		//アニメーションの更新
		Anime_player();
	}

	else {
		g_is_run_R = TRUE;
		Set_P_Anime(RUN_ANIME);
		//アニメーションの更新
		Anime_player();
	}
}

void Update_col(void) {
	//当たり判定
	g_Player.col.pos = g_Player.obj.pos;

	//当たり判定下
	g_Player.col_D.pos.x = g_Player.obj.pos.x;
	g_Player.col_D.pos.y = g_Player.obj.pos.y + COL_SIZE_H / 2;

	//当たり判定上
	g_Player.col_U.pos.x = g_Player.obj.pos.x;
	g_Player.col_U.pos.y = g_Player.obj.pos.y - COL_SIZE_H / 2;

	//当たり判定右
	g_Player.col_R.pos.x = g_Player.obj.pos.x + COL_SIZE_W / 2;
	g_Player.col_R.pos.y = g_Player.obj.pos.y;

	//当たり判定左
	g_Player.col_L.pos.x = g_Player.obj.pos.x - COL_SIZE_W / 2;
	g_Player.col_L.pos.y = g_Player.obj.pos.y;

	//ライン
	g_Player.line_pos = g_Player.obj.pos;
	g_Player.line_pos.y -= SIZE / 2;
}



void Anime_player(void) {
	if (g_Player.anime.count_FPS >= g_Player.anime.anime_FPS) {

		if (g_Player.anime.anime_frame >= g_anime_frame[g_Player.anime.which_anime]) {
			g_Player.anime.anime_frame = 0;
			//if (g_Player.anime.which_anime == DAMAGE_ANIME)Set_P_Anime(IDLE_ANIME); //ダメージのアニメーションの場合アニメーションをアイドルにする

			if ((!g_is_run_R) && (g_anime_frame[g_Player.anime.which_anime] < ANIME_COUNT)) {
				g_Player.anime.anime_frame = 2;
			}
		}


		g_Player.obj.tex.x = (1 / ANIME_COUNT) * g_Player.anime.anime_frame;
		
		g_Player.anime.anime_frame++;

		g_Player.anime.count_FPS = 0;

	}
	g_Player.anime.count_FPS++;



}

void Set_P_Anime(int anime) {
	if (g_Player.anime.which_anime != anime) {
		g_Player.anime.which_anime = anime;
		g_Player.obj.tex.y = (1 / ANIME_NUMBER) * g_Player.anime.which_anime;
	}

}


void Draw_player(void) {

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (g_Player.obj.use) {
		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		if (g_is_run_R) {
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, g_Player.obj.pos.x, g_Player.obj.pos.y, g_Player.obj.pol.w, g_Player.obj.pol.h, g_Player.obj.tex.x, g_Player.obj.tex.y, g_Player.obj.tex.w, g_Player.obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else {
			SetSpriteColor(g_VertexBuffer, g_Player.obj.pos.x, g_Player.obj.pos.y, g_Player.obj.pol.w, g_Player.obj.pol.h, g_Player.obj.tex.x, g_Player.obj.tex.y, -g_Player.obj.tex.w, g_Player.obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		}

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
	}
}


STATUS* Get_pStatus(void) {
	return &g_status;
}

PLAYER* Get_Player(void) {
	return &g_Player;
}



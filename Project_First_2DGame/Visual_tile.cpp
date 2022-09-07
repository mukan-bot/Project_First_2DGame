//機能のない見た目だけの物の表示

#include "MAP.h"
#include "Visual_tile.h"
#include <time.h>
#include "set_map.h"

//マクロ定義
#define TEXTUR_MAX (1)
#define TEXTUR_W (3.0f) //横分割数
#define TEXTUR_H (4.0f) //縦分割数
#define V_TILE_MAX (256)//Visualタイルの最大表示数


// グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTUR_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTUR_MAX] = {
	"data/TEXTURE/tile/visual_tile.png",
};

static TILE g_v_tile[V_TILE_MAX];

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

static int g_count = 0;


//プロトタイプ宣言


// 初期化処理
HRESULT Init_Visual_tile(void){
	ID3D11Device* pDevice = GetDevice();
	
	//テクスチャ生成
	for (int i = 0; i < TEXTUR_MAX; i++)
	{
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

	for (int i = 0; i < V_TILE_MAX; i++) {
		g_v_tile[i].obj.use = FALSE;
		g_v_tile[i].tile_no = -1;
		g_v_tile[i].obj.tex.texNo = 0;
		g_v_tile[i].obj.pos = XMFLOAT2(0.0f, 0.0f);
		g_v_tile[i].obj.pol.w = 400;
		g_v_tile[i].obj.pol.h = 400;
		g_v_tile[i].obj.tex.x = 0.0f;
		g_v_tile[i].obj.tex.y = 0.0f;
		g_v_tile[i].obj.tex.w = 1.0f / TEXTUR_W;
		g_v_tile[i].obj.tex.h = 1.0f / TEXTUR_H;

		////v_tileでは使わない（Animationなし）
		//g_v_tile[i].anime_count = 0;
		//g_v_tile[i].anime_speed = 0;
		//g_v_tile[i].anime_frame = 0;
	}


	srand(time(NULL));

	g_Load = TRUE;
	return S_OK;
}

// 終了処理
void Uninit_Visual_tile(void) {
	if (g_Load == FALSE) return;

	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTUR_MAX; i++) {
		if (g_Texture[i]) {
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	g_Load = FALSE;
}

void Update_Visual_tile(float plus) {
	for (int i = 0; i < V_TILE_MAX; i++) {
		if (g_v_tile[i].obj.use) {
			g_v_tile[i].obj.pos.x -= (plus);
		}
	}
}


// 描画処理
void Draw_Visual_tile(void) {
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);




	// 描画
	for (int i = 0; i < V_TILE_MAX; i++) {
		if (g_v_tile[i].obj.pos.x > SCREEN_WIDTH + g_v_tile[i].obj.pol.w) {
			continue;
		}
		if (g_v_tile[i].obj.pos.x < -g_v_tile[i].obj.pol.w) {
			continue;
		}
		if (g_v_tile[i].obj.use) {
			
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_v_tile[i].obj.tex.texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer,
				g_v_tile[i].obj.pos.x, g_v_tile[i].obj.pos.y, g_v_tile[i].obj.pol.w, g_v_tile[i].obj.pol.h,
				g_v_tile[i].obj.tex.x, g_v_tile[i].obj.tex.y, g_v_tile[i].obj.tex.w, g_v_tile[i].obj.tex.h,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}



int SetV_tile(int tex_no, XMFLOAT3 pos_size) {
	if (g_count < V_TILE_MAX) {
		if (!g_v_tile[g_count].obj.use) {
			g_v_tile[g_count].tile_no = g_count;
			g_v_tile[g_count].obj.use = TRUE;
			g_v_tile[g_count].obj.pos = XMFLOAT2(pos_size.x, pos_size.y);
			g_v_tile[g_count].obj.pol.w = pos_size.z - 0.01f;
			g_v_tile[g_count].obj.pol.h = pos_size.z - 0.01f;



			g_v_tile[g_count].obj.tex.x = g_v_tile[g_count].obj.tex.w * ((1 + tex_no) % 3);

			float temp;
			switch (tex_no)
			{
			case (0):
				temp = rand() % 3;
				g_v_tile[g_count].obj.tex.x = g_v_tile[g_count].obj.tex.w * temp;
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 0;
				break;
			case (1):
				temp = rand() % 3;
				g_v_tile[g_count].obj.tex.x = g_v_tile[g_count].obj.tex.w * temp;
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 1;
				break;
			case (2):
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 2;
				break;
			case (3):
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 2;
				break;
			case (4):
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 2;
				break;
			case (5):
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 3;
				break;
			case (6):
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 3;
				break;
			case (7):
				g_v_tile[g_count].obj.tex.y = g_v_tile[g_count].obj.tex.h * 3;
				break;
			default:
				break;
			}




			g_count++;
			return g_count - 1;
		}
	}
	for (int i = 0; i < V_TILE_MAX; i++) {//削除した番号を使うため
		if (!g_v_tile[i].obj.use) {
			g_v_tile[i].tile_no = i;
			g_v_tile[i].obj.use = TRUE;
			g_v_tile[i].obj.pos = XMFLOAT2(pos_size.x, pos_size.y);
			g_v_tile[i].obj.pol.w = pos_size.z;
			g_v_tile[i].obj.pol.h = pos_size.z;


			g_v_tile[i].obj.tex.x = g_v_tile[i].obj.tex.w * ((1+tex_no) % 3);


			float temp;
			switch (tex_no)
			{
			case (0):
				temp = rand() % 3;
				g_v_tile[i].obj.tex.x = g_v_tile[i].obj.tex.w * temp;
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * tex_no;
				break;
			case (1):
				temp = rand() % 3;
				g_v_tile[i].obj.tex.x = g_v_tile[i].obj.tex.w * temp;
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * tex_no;
				break;
			case (2):
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * 2;
				break;
			case (3):
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * 2;
				break;
			case (4):
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * 2;
				break;
			case (5):
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * 3;
				break;
			case (6):
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * 3;
				break;
			case (7):
				g_v_tile[i].obj.tex.y = g_v_tile[i].obj.tex.h * 3;
				break;
			default:
				break;
			}


			return i;
		}
	}
	return -1;

}

bool DelV_tile(int count) {
	if ((count < V_TILE_MAX) && g_v_tile[count].obj.use) {
		g_v_tile[count].obj.use = FALSE;
		return TRUE;
	}
	return FALSE;
}
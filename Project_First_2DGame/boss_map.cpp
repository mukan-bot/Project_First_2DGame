#include "main.h"
#include "game.h"
#include "boss_map.h"
#include "MAP.h"

//マクロ定義 
#define WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(3)				// 背景の枚数
#define TEXTURE_MAX	(4)			// テクスチャの数

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/background/layer_1.png",
	"data/TEXTURE/background/layer_3.png",
	"data/TEXTURE/background/city.png",

	"data/TEXTURE/effect/white_haze.png",

};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

main_obj g_boss_map[LAYER_MAX];

main_obj g_white_haze;

static bool g_BOSS;

HRESULT Init_boss_map(void) {
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < LAYER_MAX; i++)
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

	g_BOSS = FALSE;

	for (int i = 0; i < LAYER_MAX; i++) {
		g_boss_map[i].pos = XMFLOAT2(0.0f, 0.0f);
		g_boss_map[i].use = FALSE;
		g_boss_map[i].pol.w = WIDTH;
		g_boss_map[i].pol.h = HEIGHT;
		g_boss_map[i].tex.w = 1.0f;
		g_boss_map[i].tex.h = 1.0f;
		g_boss_map[i].tex.x = 0.0f;
		g_boss_map[i].tex.y = 0.0f;
		g_boss_map[i].tex.texNo = i;
		g_boss_map[i].tex.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	g_boss_map[1].use = FALSE;
	g_boss_map[2].tex.color = XMFLOAT4(0.4f, 0.3f, 0.3f, 1.0f);

	//白い霧
	g_white_haze.pos = XMFLOAT2(GAME_CLEAR_POS, SCREEN_HEIGHT/2);
	g_white_haze.use = TRUE;
	g_white_haze.pol.w = WIDTH;
	g_white_haze.pol.h = HEIGHT;
	g_white_haze.tex.w = 1.0f;
	g_white_haze.tex.h = 1.0f;
	g_white_haze.tex.x = 0.0f;
	g_white_haze.tex.y = 0.0f;
	g_white_haze.tex.texNo = 3;
	g_white_haze.tex.color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);


	return S_OK;
}
void Uninit_boss_map(void) {
	if (g_Load == FALSE) return;

	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < LAYER_MAX; i++) {
		if (g_Texture[i]) {
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	g_Load = FALSE;
}
void Update_boss_map(void) {
	g_white_haze.pol.w = 100;
	g_white_haze.pol.h = SCREEN_HEIGHT;
	//g_white_haze.tex.w = (rand()%100)/100;
	//g_white_haze.tex.h = (rand() % 100) / 100;
	//g_white_haze.tex.x = (rand() % 100) / 100;
	//g_white_haze.tex.y = (rand() % 100) / 100;
}
//白い霧のスクロール
void white_mv(float plus) {
	g_white_haze.pos.x -= plus;
}

void Draw_boss_map(void) {
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

	// 背景を描画
	for (int i = 0; i < LAYER_MAX; i++) {
		if (g_BOSS) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_boss_map[i].tex.texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_boss_map[i].pos.x, 0 - g_boss_map[i].pos.y, g_boss_map[i].pol.w, g_boss_map[i].pol.h,
				g_boss_map[i].tex.x, g_boss_map[i].tex.y, g_boss_map[i].tex.w, g_boss_map[i].tex.h,
				g_boss_map[i].tex.color);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	if (!g_BOSS) {
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_white_haze.tex.texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_white_haze.pos.x, g_white_haze.pos.y, g_white_haze.pol.w, g_white_haze.pol.h,
			g_white_haze.tex.x, g_white_haze.tex.y, g_white_haze.tex.w, g_white_haze.tex.h,
			g_white_haze.tex.color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}


void Set_boss() {
	g_BOSS = TRUE;
}
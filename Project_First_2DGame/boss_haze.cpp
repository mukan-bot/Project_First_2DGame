//作成者：竹内大翔

#include "main.h"
#include "game.h"
#include "boss_haze.h"
#include "MAP.h"


//マクロ定義 
#define WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(2)				// テクスチャの数
#define TEXTURE_MAX	(1)			// テクスチャの数

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/effect/white_haze.png",
};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

static main_obj g_boss_haze[LAYER_MAX];
static int g_count[LAYER_MAX];
static float g_alfa;

HRESULT Init_boss_haze(void) {
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
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

	for (int i = 0; i < LAYER_MAX; i++) {
		g_boss_haze[i].pos = XMFLOAT2(0, 0);
		g_boss_haze[i].use = TRUE;
		g_boss_haze[i].pol.w = SCREEN_WIDTH;
		g_boss_haze[i].pol.h = SCREEN_HEIGHT;
		g_boss_haze[i].tex.w = 1.0f;
		g_boss_haze[i].tex.h = 1.0f;		
		g_boss_haze[i].tex.x = 0;
		g_boss_haze[i].tex.y = 0;
		g_boss_haze[i].tex.texNo = 0;
	}

	g_alfa = 0;

	return S_OK;
}
void Uninit_boss_haze(void) {
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

void Update_boss_haze(void) {
	float temp = (Get_Scroll() * SCREEN_WIDTH) - GAME_CLEAR_POS;
	temp /= 1000.0f;
	clamp(temp, 0, 1);
	g_alfa = temp;

	if (Get_isBoss()) g_alfa = 0.00f;
}


void Draw_boss_haze(void) {
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
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_boss_haze[i].tex.texNo]);




		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			g_boss_haze[i].pos.x, g_boss_haze[i].pos.y, g_boss_haze[i].pol.w, g_boss_haze[i].pol.h,
			g_boss_haze[i].tex.x, g_boss_haze[i].tex.y, g_boss_haze[i].tex.w, g_boss_haze[i].tex.h,
			XMFLOAT4(1.00f, 1.00f, 1.00f, g_alfa));




		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}
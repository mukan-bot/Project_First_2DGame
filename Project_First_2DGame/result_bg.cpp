//作成者：竹内大翔

#include "main.h"
#include "result_bg.h"

//マクロ定義 
#define WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(3)				// テクスチャの数

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/background/layer_1.png",
	"data/TEXTURE/background/layer_3.png",
	"data/TEXTURE/background/city.png",
};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

main_obj g_result_bg[LAYER_MAX];


HRESULT Init_result_bg(void) {
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

	for (int i = 0; i < LAYER_MAX; i++) {
		g_result_bg[i].pos = XMFLOAT2(0.0f, 0.0f);
		g_result_bg[i].use = TRUE;
		g_result_bg[i].pol.w = WIDTH;
		g_result_bg[i].pol.h = HEIGHT;
		g_result_bg[i].tex.w = 1.0f;
		g_result_bg[i].tex.h = 1.0f;
		g_result_bg[i].tex.x = 0.0f;
		g_result_bg[i].tex.y = 0.0f;
		g_result_bg[i].tex.texNo = i;
		g_result_bg[i].tex.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	g_result_bg[1].use = FALSE;
	g_result_bg[2].tex.color = XMFLOAT4(0.4f, 0.3f, 0.3f, 1.0f);
	return S_OK;
}
void Uninit_result_bg(void) {
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
void Update_result_bg(void) {

}
void Draw_result_bg(void) {
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
		if (g_result_bg[i].use) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_result_bg[i].tex.texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_result_bg[i].pos.x, 0 - g_result_bg[i].pos.y, g_result_bg[i].pol.w, g_result_bg[i].pol.h,
				g_result_bg[i].tex.x, g_result_bg[i].tex.y, g_result_bg[i].tex.w, g_result_bg[i].tex.h,
				g_result_bg[i].tex.color);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}
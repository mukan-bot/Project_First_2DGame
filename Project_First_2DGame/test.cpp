#include "test.h"
#include "player.h"

//マクロ定義
#define TEXTURE_MAX	(1)

#define SIZE	(200.0f)

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/test.png",
};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static PLAYER g_test;

HRESULT Init_test(void) {
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

	//test変数の初期化
	g_test.obj.pos = XMFLOAT2(400.0f, 600.0f);
	g_test.obj.use = TRUE;
	g_test.obj.pol.h = SIZE;
	g_test.obj.pol.w = SIZE;
	g_test.obj.tex.x = 0.0f;
	g_test.obj.tex.y = 0.0f;
	g_test.obj.tex.w = 1.0f;
	g_test.obj.tex.h = 1.0f;

	//当たり判定
	g_test.col.pos = g_test.obj.pos;
	g_test.col.shape = BOX;
	g_test.col.size = XMFLOAT2(SIZE, SIZE);
	g_test.col.type = GROUND;
	SetCollision(&g_test.col);

	return S_OK;
}

void Uninit_test(void) {
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

void Update_test(void) {
	//if (GetCollision(g_test.col)) {
	//}
	//else {
	//	g_test.obj.pos.y--;
	//	
	//}
	//g_test.col.pos = g_test.obj.pos;
}
void Draw_test(void) {

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (g_test.obj.use) {
		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_test.obj.pos.x, g_test.obj.pos.y, g_test.obj.pol.w, g_test.obj.pol.h, g_test.obj.tex.x, g_test.obj.tex.y, g_test.obj.tex.w, g_test.obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}
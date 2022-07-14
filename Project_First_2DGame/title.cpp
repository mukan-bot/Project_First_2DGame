#include "title.h"


// マクロ定義
#define TEXTURE_WIDTH				(SCREEN_WIDTH)			// 背景サイズW
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)			// 背景サイズH
#define TEXTURE_MAX					(1)				// テクスチャの数

// グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/test.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static TITLE	g_TITLE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTITLE(void)
{
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


	// 変数の初期化
	g_TITLE.w = TEXTURE_WIDTH;
	g_TITLE.h = TEXTURE_HEIGHT;
	g_TITLE.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_TITLE.texNo = 0;

	g_TITLE.scrl = 0.0f;		// TEXスクロール
	g_TITLE.scrl2 = 0.0f;		// TEXスクロール

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTITLE(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
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

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTITLE(void)
{
	g_TITLE.old_pos = g_TITLE.pos;	// １フレ前の情報を保存


	//g_TITLE.scrl -= 0.0f;		// 0.005f;		// スクロール


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTITLE(void)
{
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

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TITLE.texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_TITLE.pos.x, 0 - g_TITLE.pos.y, g_TITLE.w, g_TITLE.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float	tx = (g_TITLE.pos.x - g_TITLE.old_pos.x) * ((float)SCREEN_WIDTH / TEXTURE_WIDTH);
		g_TITLE.scrl2 += tx * 0.01f;
		//g_TITLE.scrl2 += 0.003f;

		SetSpriteLTColor(g_VertexBuffer,
			0.0f, SKY_H / 2, SCREEN_WIDTH, SKY_H,
			g_TITLE.scrl2, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}

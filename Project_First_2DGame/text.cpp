#include "text.h"


// マクロ定義
#define TEXT_MAX	(40)	//最大文字数

#define TEXTURE_WIDTH				(200/2)	// キャラサイズ
#define TEXTURE_HEIGHT				(200/2)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(62)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
// グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/test.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static TEXT	g_TEXT;



///////////////////////////////////////////////////////////////////
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
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

	g_TEXT.pos = XMFLOAT3(400.0f, 400.0f, 0.0f);	// 中心点から表示
	g_TEXT.w = TEXTURE_WIDTH;
	g_TEXT.h = TEXTURE_HEIGHT;
	g_TEXT.texNo = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{

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
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	for (int i = 0; i < TEXT_MAX; i++) {
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


		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Player[i].w;		// プレイヤーの表示幅
		float ph = g_Player[i].h / 4;		// プレイヤーの表示高さ

		float tw = 1.0f;	// テクスチャの幅
		float th = 1.0f;	// テクスチャの高さ
		float tx = 0.0f;	// テクスチャの左上X座標
		float ty = 0.0f;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す


		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_TEXT.pos.x;	// プレイヤーの表示位置X
		float py = g_TEXT.pos.y;	// プレイヤーの表示位置Y
		float pw = g_TEXT.w;		// プレイヤーの表示幅
		float ph = g_TEXT.h;		// プレイヤーの表示高さ

		py += g_Player[i].jumpY;		// ジャンプ中の高さを足す

		// アニメーション用
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			g_Player[i].rot.z);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}




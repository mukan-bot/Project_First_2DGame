//作成者：竹内大翔

#include "bg.h"
#include "MAP.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(3)				// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/background/background_layer_1.png",
	"data/TEXTURE/background/background_layer_2.png",
	"data/TEXTURE/background/background_layer_3.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG	g_BG[LAYER_MAX];



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(void)
{
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


	//BG変数の初期化
	for (int i = 0; i < LAYER_MAX; i++) {
		g_BG[i].obj.pos = XMFLOAT2(0.0f, 0.0f);
		g_BG[i].obj.use = TRUE;
		g_BG[i].obj.pol.w = WIDTH;
		g_BG[i].obj.pol.h = HEIGHT;
		g_BG[i].obj.tex.x = 0.0f;
		g_BG[i].obj.tex.y = 0.0f;
		g_BG[i].obj.tex.w = 1.0f;
		g_BG[i].obj.tex.h = 1.0f;
		g_BG[i].obj.tex.texNo = i;
	}



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void){
	if (g_Load == FALSE) return;

	if (g_VertexBuffer){
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < LAYER_MAX; i++){
		if (g_Texture[i]){
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void){
	g_BG[2].obj.tex.x = Get_Scroll();
	g_BG[1].obj.tex.x = g_BG[2].obj.tex.x * 0.5f;
	g_BG[0].obj.tex.x = g_BG[1].obj.tex.x * 0.5f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void){
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG[i].obj.tex.texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_BG[i].obj.pos.x, 0 - g_BG[i].obj.pos.y, g_BG[i].obj.pol.w, g_BG[i].obj.pol.h,
			g_BG[i].obj.tex.x, g_BG[i].obj.tex.y, g_BG[i].obj.tex.w, g_BG[i].obj.tex.h,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}


//=============================================================================
// BG構造体の先頭アドレスを取得
//=============================================================================
BG* GetBG(void)
{
	return &g_BG[LAYER_MAX];
}

//背景のスクロールの値
void Set_ScrollBG(float set_scroll) {
	g_BG[2].scrollSpeed = set_scroll;
	g_BG[1].scrollSpeed = g_BG[2].scrollSpeed * 0.5f;
	g_BG[0].scrollSpeed = g_BG[1].scrollSpeed * 0.09f;
}


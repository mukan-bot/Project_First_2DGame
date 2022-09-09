//作成者：竹内大翔

#include "MAP.h"
#include "HUD.h"
#include "text.h"
#include "game.h"
#include "player.h"

//マクロ定義
#define TEXTUR_MAX (4)
#define BAR (3)
#define BAR_SIZE (0.15) //BARの大きさ調整（倍率）

// グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTUR_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTUR_MAX] = {
	"data/TEXTURE/HUD/bar0.png",
	"data/TEXTURE/HUD/bar1.png",
	"data/TEXTURE/HUD/bar2.png",
	
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ

static int g_count = 0;

static main_obj g_hud_bar[BAR];

static STATUS* g_pStatus;//プレイヤーのステータスを所得

//プロトタイプ宣言


// 初期化処理
HRESULT Init_HUD(void) {
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

	//HP&MPバー
	g_hud_bar[0].pol.w = 2775 * BAR_SIZE;
	g_hud_bar[0].pol.h = 375 * BAR_SIZE;
	g_hud_bar[0].tex.texNo = 0;
	g_hud_bar[0].tex.w = 1.0f;
	g_hud_bar[0].tex.h = 1.0f;
	g_hud_bar[0].tex.x = 0;
	g_hud_bar[0].tex.y = 0;
	g_hud_bar[0].pos = XMFLOAT2(10, 10);


	g_hud_bar[1].pol.w = 2725 * BAR_SIZE;
	g_hud_bar[1].pol.h = 375 * BAR_SIZE;
	g_hud_bar[1].tex.texNo = 1;
	g_hud_bar[1].tex.w = 0.5f;
	g_hud_bar[1].tex.h = 1.0f;
	g_hud_bar[1].tex.x = 0;
	g_hud_bar[1].tex.y = 0;
	g_hud_bar[1].pos = XMFLOAT2(g_hud_bar[0].pos.x + 24.0f * BAR_SIZE, g_hud_bar[0].pos.y);

	g_hud_bar[2].pol.w = 2725 * BAR_SIZE;
	g_hud_bar[2].pol.h = 375 * BAR_SIZE;
	g_hud_bar[2].tex.texNo = 2;
	g_hud_bar[2].tex.w = 0.5f;
	g_hud_bar[2].tex.h = 1.0f;
	g_hud_bar[2].tex.x = 0;
	g_hud_bar[2].tex.y = 0;
	g_hud_bar[2].pos = XMFLOAT2(g_hud_bar[0].pos.x + 24.0f * BAR_SIZE, g_hud_bar[0].pos.y);


	g_pStatus = Get_pStatus();

	g_Load = TRUE;
	return S_OK;
}

// 終了処理
void Uninit_HUD(void) {
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

void Update_HUD() {
	float temp;
	temp = (g_pStatus->hp/2);
	temp = temp + 0.5;
	g_hud_bar[1].tex.x = -temp;
	temp = (g_pStatus->mp/2);
	temp = temp + 0.5;
	g_hud_bar[2].tex.x = -temp;
}


// 描画処理
void Draw_HUD(void) {
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
	// 
	//HP_bar
	{
		for (int i = 1; i < BAR; i++) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_hud_bar[i].tex.texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer, g_hud_bar[i].pos.x, g_hud_bar[i].pos.y, g_hud_bar[i].pol.w, g_hud_bar[i].pol.h, g_hud_bar[i].tex.x, g_hud_bar[i].tex.y, g_hud_bar[i].tex.w, g_hud_bar[i].tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_hud_bar[0].tex.texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer, g_hud_bar[0].pos.x, g_hud_bar[0].pos.y, g_hud_bar[0].pol.w, g_hud_bar[0].pol.h, g_hud_bar[0].tex.x, g_hud_bar[0].tex.y, g_hud_bar[0].tex.w, g_hud_bar[0].tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}



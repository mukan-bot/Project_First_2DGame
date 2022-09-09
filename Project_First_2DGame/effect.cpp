//作成者：竹内大翔

#include "effect.h"

//マクロ定義
#define TEXTURE_MAX	(1)			// テクスチャの数
#define EFFECT_MAX (255)			//表示するエフェクトの最大数
#define EFFECT_UPDATE_FRAME (15)		//更新頻度

#define EFFECT_FRAME_MAX (60)	//コマ数

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/effect/64x64.png",
};


static BOOL g_Load = FALSE;				// 初期化を行ったかのフラグ

static EFFECT g_effect[EFFECT_TYPE_MAX];	//エフェクトの種類
static EFFECT g_temp_effect[EFFECT_MAX];	//エフェクト表示用
static XMFLOAT2 g_pos = XMFLOAT2(0, 0);

//初期化
HRESULT Init_effect(void){
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

	for (int i = 0; i < EFFECT_TYPE_MAX; i++) {
		g_effect[i].anime.anime_number = EFFECT_TYPE_MAX;
		g_effect[i].anime.anime_count = EFFECT_FRAME_MAX;
		g_effect[i].anime.anime_frame = 0;
		g_effect[i].anime.anime_FPS = EFFECT_UPDATE_FRAME;
		g_effect[i].anime.count_FPS = 0;
		g_effect[i].anime.which_anime = i;
		g_effect[i].obj.pos = XMFLOAT2(0.0f, 0.0f);
		g_effect[i].obj.pol.w = 0;
		g_effect[i].obj.pol.h = 0;
		g_effect[i].obj.tex.texNo = 0;
		g_effect[i].obj.tex.w = 1.0f / g_effect[i].anime.anime_count;
		g_effect[i].obj.tex.h = 1.0f / g_effect[i].anime.anime_number;
		g_effect[i].obj.tex.x = 0;
		g_effect[i].obj.tex.y = g_effect[i].obj.tex.h * g_effect[i].anime.which_anime;
		g_effect[i].obj.use = TRUE;
		g_effect[i].loop = FALSE;
		g_effect[i].is_Rside = TRUE;
	}
	int i = EFFECT_TYPE_ICE_SPEAR;
	g_effect[i].anime.anime_frame_max = 30;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_PURPLE_EXPLOSION;
	g_effect[i].anime.anime_frame_max = 32;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_STAR_EXPLOSION;
	g_effect[i].anime.anime_frame_max = 60;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_sSTAE_EXPLOSION;
	g_effect[i].anime.anime_frame_max = 60;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_EXPLOSION;
	g_effect[i].anime.anime_frame_max = 44;
	g_effect[i].color = XMFLOAT4(0.7f, 0.7f, 1.0f, 0.8f);
	i = EFFECT_TYPE_FIRE_BULLET_1;
	g_effect[i].anime.anime_frame_max = 45;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_FIRE_BULLET_2;
	g_effect[i].anime.anime_frame_max = 60;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_FIRE_BULLET_3;
	g_effect[i].anime.anime_frame_max = 45;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
	i = EFFECT_TYPE_FIRE_BULLET_4;
	g_effect[i].anime.anime_frame_max = 29;
	g_effect[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);


	return S_OK;
}

void Uninit_effect(void){
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
}

void Update_effect() {
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (g_temp_effect[i].obj.use) {
			if (g_temp_effect[i].anime.anime_FPS < g_temp_effect[i].anime.count_FPS) {
				if (g_temp_effect[i].anime.anime_frame == g_temp_effect[i].anime.anime_frame_max) {
					if (g_temp_effect[i].loop) {
						g_temp_effect[i].anime.anime_frame = 0;
					}
					else {
						Del_effect(g_temp_effect[i].no);
					}
				}
				g_temp_effect[i].anime.count_FPS = 0;
				g_temp_effect[i].obj.tex.x = g_temp_effect[i].obj.tex.w * g_temp_effect[i].anime.anime_frame;
				g_temp_effect[i].anime.anime_frame++;
			}
			g_temp_effect[i].anime.count_FPS++;
		}
	}
}




void Draw_effect(void) {
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

	for (int i = 0; i < EFFECT_MAX; i++) {
		if (g_temp_effect[i].obj.use) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_temp_effect[i].obj.tex.texNo]);
			if (g_temp_effect[i].is_Rside) {
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, g_temp_effect[i].pos->x, g_temp_effect[i].pos->y, g_temp_effect[i].obj.pol.w, g_temp_effect[i].obj.pol.h, g_temp_effect[i].obj.tex.x, g_temp_effect[i].obj.tex.y, g_temp_effect[i].obj.tex.w, g_temp_effect[i].obj.tex.h, g_temp_effect[i].color);
			}
			else {
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, g_temp_effect[i].pos->x, g_temp_effect[i].pos->y, g_temp_effect[i].obj.pol.w, g_temp_effect[i].obj.pol.h, g_temp_effect[i].obj.tex.x, g_temp_effect[i].obj.tex.y, g_temp_effect[i].obj.tex.w, g_temp_effect[i].obj.tex.h, g_temp_effect[i].color,3.14f);
			}
			if (GetMode() == MODE_RESULT) {
				SetSpriteColorRotation(g_VertexBuffer, g_temp_effect[i].pos->x, g_temp_effect[i].pos->y, g_temp_effect[i].obj.pol.w, g_temp_effect[i].obj.pol.h, g_temp_effect[i].obj.tex.x, g_temp_effect[i].obj.tex.y, g_temp_effect[i].obj.tex.w, g_temp_effect[i].obj.tex.h, g_temp_effect[i].color, 1);
			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


int Set_effect(XMFLOAT2* pos, bool is_rside,float size, int effect_type, bool loop, int fps) {
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (!g_temp_effect[i].obj.use) {
			g_temp_effect[i] = g_effect[effect_type];
			g_temp_effect[i].pos = pos;
			g_temp_effect[i].obj.pol.w = size;
			g_temp_effect[i].obj.pol.h = size;
			g_temp_effect[i].no = i;
			g_temp_effect[i].loop = loop;
			g_temp_effect[i].anime.anime_FPS = fps/g_effect[effect_type].anime.anime_frame_max;
			g_temp_effect[i].is_Rside = is_rside;
			return g_temp_effect[i].no;
		}
	}
}

void Del_effect(int no) {
	if (g_temp_effect[no].obj.use) {
		g_temp_effect[no] = g_effect[0];
		g_temp_effect[no].obj.use = FALSE;
		//g_temp_effect[no].pos = &g_pos;
	}
}
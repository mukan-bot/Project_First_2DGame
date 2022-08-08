#include "player.h"
#include "input.h"

#include "camera.h"
//マクロ定義
#define TEXTURE_MAX	(1)
#define ANIME_NUMBER (1.0f)
#define ANIME_COUNT (4.0f)

#define SIZE	(50.0f)

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/man0.png",
};

static CAMERA* g_Player_camera;

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static PLAYER g_Player;

HRESULT Init_player(void) {
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
	
	//カメラの所得
	g_Player_camera = GetCamera();

	//player変数の初期化
	g_Player.obj.pos = XMFLOAT2(400.0f, 200.0f);
	g_Player.obj.use = TRUE;
	g_Player.obj.pol.w = SIZE;
	g_Player.obj.pol.h = SIZE;
	g_Player.obj.tex.x = 0.0f;
	g_Player.obj.tex.y = 0.0f;
	g_Player.obj.tex.w = 1.0f/ANIME_COUNT;
	g_Player.obj.tex.h = 1.0f/ ANIME_NUMBER;
	g_Player.speed = 10;
	g_Player.camera_use = TRUE;


	//当たり判定
	g_Player.col.pos = g_Player.obj.pos;
	g_Player.col.shape = CIRCLE;
	g_Player.col.size = XMFLOAT2(SIZE/2, 0.0f);
	g_Player.col.type = GROUND;

	//Animation
	g_Player.anime.anime_frame = 0;
	g_Player.anime.anime_FPS = 20;


	SetCollision(&g_Player.col);
	return S_OK;
}

void Uninit_player(void) {
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

void Update_player(void) {
	if (GetKeyboardPress(DIK_D)) {
		g_Player.obj.pos.x += g_Player.speed;
	}
	if (!CheckHit(g_Player.col)) {
		g_Player.obj.pos.y++;
	}
	else {
		
	}
	g_Player.col.pos = g_Player.obj.pos;
	g_Player_camera->pos = XMFLOAT3(g_Player.obj.pos.x, g_Player.obj.pos.y, g_Player_camera->pos.z);
}

void Anime_player(void) {
	if (g_Player.anime.count_FPS == g_Player.anime.anime_FPS) {
		g_Player.anime.anime_frame++;
		g_Player.obj.tex.x = (1 / ANIME_COUNT) * g_Player.anime.anime_frame;
		g_Player.anime.count_FPS = 0;
	}
	g_Player.anime.count_FPS++;
}


void Draw_player(void) {

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (g_Player.obj.use) {
		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_Player.obj.pos.x, g_Player.obj.pos.y, g_Player.obj.pol.w, g_Player.obj.pol.h, g_Player.obj.tex.x, g_Player.obj.tex.y, g_Player.obj.tex.w, g_Player.obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}
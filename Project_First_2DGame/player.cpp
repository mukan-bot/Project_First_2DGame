#include "player.h"

//�}�N����`
#define TEXTURE_MAX	(1)

#define SIZE	(200.0f)

//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/man.png",
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static PLAYER g_Player;

HRESULT Init_player(void) {
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`���̐���
	for (int i = 0; i < TEXTURE_MAX; i++) {
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
		g_TexturName[i],
		NULL,
		NULL,
		&g_Texture[i],
		NULL);
	}
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	
	//player�ϐ��̏�����
	g_Player.obj.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Player.obj.use = TRUE;
	g_Player.obj.pol.h = SIZE;
	g_Player.obj.pol.w = SIZE;
	g_Player.obj.tex.x = 0.0f;
	g_Player.obj.tex.y = 0.0f;
	g_Player.obj.tex.w = 1.0f;
	g_Player.obj.tex.h = 1.0f;


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
}
void Draw_player(void) {

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (g_Player.obj.use) {
		// �}�e���A���ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, g_Player.obj.pos.x, g_Player.obj.pos.y, g_Player.obj.pol.w, g_Player.obj.pol.h, g_Player.obj.tex.x, g_Player.obj.tex.y, g_Player.obj.tex.w, g_Player.obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}
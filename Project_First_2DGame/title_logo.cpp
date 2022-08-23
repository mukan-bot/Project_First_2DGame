
#include "main.h"
#include "title_logo.h"


//�}�N����` 
#define WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(1)				// �e�N�X�`���̐�

//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/title_logo.png",
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

main_obj g_title_logo[LAYER_MAX];

HRESULT Init_title_logo(void) {
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < LAYER_MAX; i++) {
		g_title_logo[i].pos = XMFLOAT2(WIDTH / 2,100);
		g_title_logo[i].use = TRUE;
		g_title_logo[i].pol.h = 200;
		g_title_logo[i].pol.w = g_title_logo[i].pol.h * 1.6;
		g_title_logo[i].tex.w = 1.0f;
		g_title_logo[i].tex.h = 1.0f;
		g_title_logo[i].tex.x = 0.0f;
		g_title_logo[i].tex.y = 0.0f;
		g_title_logo[i].tex.texNo = i;
	}

	return S_OK;
}
void Uninit_title_logo(void) {
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
void Update_title_logo(void) {

}
void Draw_title_logo(void) {
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �w�i��`��
	for (int i = 0; i < LAYER_MAX; i++) {
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_title_logo[i].tex.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			g_title_logo[i].pos.x, g_title_logo[i].pos.y, g_title_logo[i].pol.w, g_title_logo[i].pol.h,
			g_title_logo[i].tex.x, g_title_logo[i].tex.y, g_title_logo[i].tex.w, g_title_logo[i].tex.h,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}
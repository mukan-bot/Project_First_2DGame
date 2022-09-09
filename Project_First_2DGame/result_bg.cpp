//�쐬�ҁF�|������

#include "main.h"
#include "result_bg.h"

//�}�N����` 
#define WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(3)				// �e�N�X�`���̐�

//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/background/layer_1.png",
	"data/TEXTURE/background/layer_3.png",
	"data/TEXTURE/background/city.png",
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

main_obj g_result_bg[LAYER_MAX];


HRESULT Init_result_bg(void) {
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
		if (g_result_bg[i].use) {
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_result_bg[i].tex.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_result_bg[i].pos.x, 0 - g_result_bg[i].pos.y, g_result_bg[i].pol.w, g_result_bg[i].pol.h,
				g_result_bg[i].tex.x, g_result_bg[i].tex.y, g_result_bg[i].tex.w, g_result_bg[i].tex.h,
				g_result_bg[i].tex.color);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}
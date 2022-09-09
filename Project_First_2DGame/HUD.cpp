//�쐬�ҁF�|������

#include "MAP.h"
#include "HUD.h"
#include "text.h"
#include "game.h"
#include "player.h"

//�}�N����`
#define TEXTUR_MAX (4)
#define BAR (3)
#define BAR_SIZE (0.15) //BAR�̑傫�������i�{���j

// �O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTUR_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTUR_MAX] = {
	"data/TEXTURE/HUD/bar0.png",
	"data/TEXTURE/HUD/bar1.png",
	"data/TEXTURE/HUD/bar2.png",
	
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

static int g_count = 0;

static main_obj g_hud_bar[BAR];

static STATUS* g_pStatus;//�v���C���[�̃X�e�[�^�X������

//�v���g�^�C�v�錾


// ����������
HRESULT Init_HUD(void) {
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//HP&MP�o�[
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

// �I������
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


// �`�揈��
void Draw_HUD(void) {
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

	// �`��
	// 
	//HP_bar
	{
		for (int i = 1; i < BAR; i++) {
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_hud_bar[i].tex.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer, g_hud_bar[i].pos.x, g_hud_bar[i].pos.y, g_hud_bar[i].pol.w, g_hud_bar[i].pol.h, g_hud_bar[i].tex.x, g_hud_bar[i].tex.y, g_hud_bar[i].tex.w, g_hud_bar[i].tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_hud_bar[0].tex.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer, g_hud_bar[0].pos.x, g_hud_bar[0].pos.y, g_hud_bar[0].pol.w, g_hud_bar[0].pol.h, g_hud_bar[0].tex.x, g_hud_bar[0].tex.y, g_hud_bar[0].tex.w, g_hud_bar[0].tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}



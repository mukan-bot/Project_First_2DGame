//????????

#include "main.h"
#include "text.h"
#include "result_score.h"

//�}�N����` 
#define WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(1)				// �e�N�X�`���̐�
enum {
	SCORE_TIME,
	SCORE_DEL_ENEMY,
	SCORE_MAX,
};

//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/background/White.png",
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

main_obj g_result_score;
TEXT g_result_text[(SCORE_MAX * 2) + 1]; //


HRESULT Init_result_score(void) {
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

	g_result_score.pol.w = SCREEN_WIDTH / 2;
	g_result_score.pol.h = SCREEN_HEIGHT / 1.5f;
	g_result_score.pos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	g_result_score.tex.color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	g_result_score.tex.texNo = 0;
	g_result_score.tex.w = 1.0f;
	g_result_score.tex.h = 1.0f;
	g_result_score.tex.x = 0.0f;
	g_result_score.tex.y = 0.0f;
	g_result_score.use = FALSE;

	for (int i = 0; i < (SCORE_MAX*2)+1; i++) {
		g_result_text[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f);
		g_result_text[i].size = 40;
		g_result_text[i].pos = XMFLOAT2(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4)+(70*i));
	}
	g_result_text[0].size = 40;
	g_result_text[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	return S_OK;
}
void Uninit_result_score(void) {
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
void Update_result_score(void) {
	if (g_result_score.use) {
		SCORE* temp = Get_score();
		if (temp->is_clear) {

			char text0[] = "�f�`�l�d �b�k�d�`�q";
			SetText(g_result_text[0], text0);
		}
		else {
			char text0[] = "�f�`�l�d �n�u�d�q";
			SetText(g_result_text[0], text0);
		}
		char text1[] = "�s�h�l�d�@�b�n�t�m�s";
		char text2[] = "�j�h�k�k�@�b�n�t�m�s";
		SetText(g_result_text[1], text1);
		SetText_d(g_result_text[2], temp->time);
		SetText(g_result_text[3], text2);
		SetText_d(g_result_text[4], temp->del_enemy);
	}
}
void Draw_result_score(void) {
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
	if (g_result_score.use) {
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_result_score.tex.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			g_result_score.pos.x, g_result_score.pos.y, g_result_score.pol.w, g_result_score.pol.h,
			g_result_score.tex.x, g_result_score.tex.y, g_result_score.tex.w, g_result_score.tex.h,
			g_result_score.tex.color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



	//�G���^�[�L�[�Ői�߂邱�Ƃ�`����
	TEXT temp;
	temp.size = 20;
	temp.pos = XMFLOAT2(800, 520);
	temp.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	char temp_text[] = "�o���������� �d�m�s�d�q �j����";
	SetText(temp, temp_text);


}

void Set_resurt_score(void) {
	g_result_score.use = TRUE;
}
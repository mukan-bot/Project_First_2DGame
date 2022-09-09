//�쐬�ҁF�|������

#include "main.h"
#include "game.h"
#include "input.h"
#include "menue.h"

#include "text.h"
#include "sound.h"

//�}�N����` 
#define WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(3)				// �e�N�X�`���̐�


//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/background/background_layer_1.png",
	"data/TEXTURE/background/background_layer_2.png",
	"data/TEXTURE/background/background_layer_3.png",
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

main_obj g_menue[LAYER_MAX];

static TEXT g_textO[OPTION_MAX];
static TEXT g_op_textO[OPTION_MAX];
static TEXT g_temp_textO[2];
static int g_selectO;
float g_temp_bgm_vol;
float g_temp_se_vol;

HRESULT Init_menue(void) {
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
		g_menue[i].pos = XMFLOAT2(0.0f, 0.0f);
		g_menue[i].use = TRUE;
		g_menue[i].pol.w = WIDTH;
		g_menue[i].pol.h = HEIGHT;
		g_menue[i].tex.w = 1.0f;
		g_menue[i].tex.h = 1.0f;
		g_menue[i].tex.x = 0.0f;
		g_menue[i].tex.y = 0.0f;
		g_menue[i].tex.texNo = i;
	}

	g_selectO = 0;	
	g_temp_bgm_vol = Get_bgmVolume();
	g_temp_se_vol = Get_seVolume();

	for (int i = 0; i < OPTION_MAX; i++) {
		g_textO[i].pos = XMFLOAT2(WIDTH / 2, 60 + (i * 140));
		g_textO[i].size = 50;
		g_textO[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);


		g_op_textO[i].pos = XMFLOAT2(WIDTH / 2, 125 + (i * 140));
		g_op_textO[i].size = 50;
		g_op_textO[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	}
	g_temp_textO[0] = g_textO[g_selectO];
	g_temp_textO[1] = g_op_textO[g_selectO];

	g_textO[OPTION_OK].pos.y += 30;


	return S_OK;
}
void Uninit_menue(void) {
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
void Update_menue(void) {

	g_textO[g_selectO] = g_temp_textO[0];
	g_op_textO[g_selectO] = g_temp_textO[1];
	int temp1 = g_selectO;
	if (GetKeyboardTrigger(DIK_W)) {
		g_selectO--;
	}
	else if (GetKeyboardTrigger(DIK_S)) {
		g_selectO++;

	}
	if (GetKeyboardPress(DIK_SPACE)) {
		if (GetKeyboardTrigger(DIK_A)) {
			switch (g_selectO) {
			case (OPTION_BGM_VOLUME):
				g_temp_bgm_vol -= 0.10f;
				break;
			case(OPTION_SE_VOLUME):
				g_temp_se_vol -= 0.10f;
				break;
			default:
				break;
			}
		}
		else if (GetKeyboardTrigger(DIK_D)) {
			switch (g_selectO) {
			case (OPTION_BGM_VOLUME):
				g_temp_bgm_vol += 0.10f;
				break;
			case(OPTION_SE_VOLUME):
				g_temp_se_vol += 0.10f;
				break;
			default:
				break;
			}
		}
	}
	else {
		if (GetKeyboardTrigger(DIK_A)) {
			switch (g_selectO) {
			case (OPTION_BGM_VOLUME):
				g_temp_bgm_vol -= 0.01f;
				break;
			case(OPTION_SE_VOLUME):
				g_temp_se_vol -= 0.01f;
				break;
			default:
				break;
			}
		}
		else if (GetKeyboardTrigger(DIK_D)) {
			switch (g_selectO) {
			case (OPTION_BGM_VOLUME):
				g_temp_bgm_vol += 0.01f;
				break;
			case(OPTION_SE_VOLUME):
				g_temp_se_vol += 0.01f;
				break;
			default:
				break;
			}
		}
	}
	g_temp_bgm_vol = clamp(g_temp_bgm_vol, 0.00f, 1.00f);
	g_temp_se_vol = clamp(g_temp_se_vol, 0.00f, 1.00f);

	int temp2 = g_selectO;
	g_selectO = clamp(g_selectO, 0, OPTION_MAX - 1);
	if (g_selectO != temp2) PlaySound(SOUND_LABEL_SE_select_no);	//�I�����̍ő�𒴂�����
	if (g_selectO != temp1) PlaySound(SOUND_LABEL_SE_select_mv);	//�I�����̍ő�𒴂����Ɉړ�������
	g_temp_textO[0] = g_textO[g_selectO];
	g_temp_textO[1] = g_op_textO[g_selectO];
	g_textO[g_selectO].size = 50;
	g_textO[g_selectO].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_op_textO[g_selectO].size = 50;
	g_op_textO[g_selectO].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	if ((GetKeyboardTrigger(DIK_SPACE)) || (GetKeyboardTrigger(DIK_RETURN))) {
		if (g_selectO == OPTION_OK) {
			Set_bgmVolume(g_temp_bgm_vol);
			Set_seVolume(g_temp_se_vol);
			SetMode(MODE_TITLE);
		}
		else if (g_selectO == OPTION_EXIT) {
			SetMode(MODE_TITLE);
		}
	}

}
void Draw_menue(void) {

	char text1[] = "�a�f�l �u�n�k�t�l�d";
	char text2[] = "�r�d �u�n�k�t�l�d";
	char text3[] = "�n�j";
	char text4[] = "�d�w�h�s";
	char text5[] = "�n�e�e";
	int i = 0;
	SetText(g_textO[i], text1);
	if (g_temp_bgm_vol * 100 != 0) {
		SetText_d(g_op_textO[i], g_temp_bgm_vol * 100);
	}
	else {
		SetText(g_op_textO[i], text5);
	}
	i++;
	SetText(g_textO[i], text2);
	if (g_temp_se_vol * 100 != 0) {
		SetText_d(g_op_textO[i], g_temp_se_vol * 100);
	}
	else {
		SetText(g_op_textO[i], text5);
	}
	i++;
	SetText(g_textO[i], text3);
	i++;
	SetText(g_textO[i], text4);


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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_menue[i].tex.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_menue[i].pos.x, 0 - g_menue[i].pos.y, g_menue[i].pol.w, g_menue[i].pol.h,
			g_menue[i].tex.x, g_menue[i].tex.y, g_menue[i].tex.w, g_menue[i].tex.h,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}
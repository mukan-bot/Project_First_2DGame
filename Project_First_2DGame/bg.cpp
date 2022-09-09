//�쐬�ҁF�|������

#include "bg.h"
#include "MAP.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define HEIGHT				(SCREEN_HEIGHT)	// 
#define LAYER_MAX					(3)				// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[LAYER_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[LAYER_MAX] = {
	"data/TEXTURE/background/background_layer_1.png",
	"data/TEXTURE/background/background_layer_2.png",
	"data/TEXTURE/background/background_layer_3.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static BG	g_BG[LAYER_MAX];



//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(void)
{
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


	//BG�ϐ��̏�����
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
// �I������
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
// �X�V����
//=============================================================================
void UpdateBG(void){
	g_BG[2].obj.tex.x = Get_Scroll();
	g_BG[1].obj.tex.x = g_BG[2].obj.tex.x * 0.5f;
	g_BG[0].obj.tex.x = g_BG[1].obj.tex.x * 0.5f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void){
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG[i].obj.tex.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_BG[i].obj.pos.x, 0 - g_BG[i].obj.pos.y, g_BG[i].obj.pol.w, g_BG[i].obj.pol.h,
			g_BG[i].obj.tex.x, g_BG[i].obj.tex.y, g_BG[i].obj.tex.w, g_BG[i].obj.tex.h,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}


//=============================================================================
// BG�\���̂̐擪�A�h���X���擾
//=============================================================================
BG* GetBG(void)
{
	return &g_BG[LAYER_MAX];
}

//�w�i�̃X�N���[���̒l
void Set_ScrollBG(float set_scroll) {
	g_BG[2].scrollSpeed = set_scroll;
	g_BG[1].scrollSpeed = g_BG[2].scrollSpeed * 0.5f;
	g_BG[0].scrollSpeed = g_BG[1].scrollSpeed * 0.09f;
}


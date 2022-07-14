#include "text.h"


// �}�N����`
#define TEXT_MAX	(40)	//�ő啶����

#define TEXTURE_WIDTH				(200/2)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(200/2)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(62)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
// �O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/test.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static TEXT	g_TEXT;



///////////////////////////////////////////////////////////////////
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_TEXT.pos = XMFLOAT3(400.0f, 400.0f, 0.0f);	// ���S�_����\��
	g_TEXT.w = TEXTURE_WIDTH;
	g_TEXT.h = TEXTURE_HEIGHT;
	g_TEXT.texNo = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{

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

	g_Load = FALSE;
}




//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	for (int i = 0; i < TEXT_MAX; i++) {
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


		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
		float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
		float pw = g_Player[i].w;		// �v���C���[�̕\����
		float ph = g_Player[i].h / 4;		// �v���C���[�̕\������

		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;	// �e�N�X�`���̍���
		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�


		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_TEXT.pos.x;	// �v���C���[�̕\���ʒuX
		float py = g_TEXT.pos.y;	// �v���C���[�̕\���ʒuY
		float pw = g_TEXT.w;		// �v���C���[�̕\����
		float ph = g_TEXT.h;		// �v���C���[�̕\������

		py += g_Player[i].jumpY;		// �W�����v���̍����𑫂�

		// �A�j���[�V�����p
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			g_Player[i].rot.z);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}



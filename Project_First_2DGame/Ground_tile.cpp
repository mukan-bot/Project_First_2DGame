//�@�\�̂Ȃ������ڂ����̕��̕\��

#include "MAP.h"
#include "Ground_tile.h"
#include <time.h>
#include "collision.h"
#include "set_map.h"


//�}�N����`
#define TEXTUR_MAX (1)
#define TEXTUR_W (4.0f) //��������
#define TEXTUR_H (4.0f) //�c������
#define G_TILE_MAX (16384)//Ground�^�C���̍ő�\����


// �O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTUR_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTUR_MAX] = {
	"data/TEXTURE/tile/ground_tile.png",
};

static G_TILE g_g_tile[G_TILE_MAX];

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

static int g_count = 0;

//�v���g�^�C�v�錾


// ����������
HRESULT Init_Ground_tile(void) {
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

	for (int i = 0; i < G_TILE_MAX; i++) {
		g_g_tile[i].tile.obj.use = FALSE;
		g_g_tile[i].tile.tile_no = -1;
		g_g_tile[i].tile.obj.tex.texNo = 0;
		g_g_tile[i].tile.obj.pos = XMFLOAT2(0.0f, 0.0f);
		g_g_tile[i].tile.obj.pol.w = 400;
		g_g_tile[i].tile.obj.pol.h = 400;
		g_g_tile[i].tile.obj.tex.x = 0.0f;
		g_g_tile[i].tile.obj.tex.y = 0.0f;
		g_g_tile[i].tile.obj.tex.w = 1.0f / TEXTUR_W;
		g_g_tile[i].tile.obj.tex.h = 1.0f / TEXTUR_H;


		//�����蔻��
		g_g_tile[i].col.pos = g_g_tile[i].tile.obj.pos;
		g_g_tile[i].col.shape = BOX;
		g_g_tile[i].col.size = XMFLOAT2(0.0f, 0.0f);
		g_g_tile[i].col.type = GROUND;
	}


	srand(time(NULL));

	g_Load = TRUE;
	return S_OK;
}

// �I������
void Uninit_Ground_tile(void) {
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

void Update_Ground_tile(float plus) {
	for (int i = 0; i < G_TILE_MAX; i++) {
		if (g_g_tile[i].tile.obj.use) {
			g_g_tile[i].tile.obj.pos.x -= (plus);
		}
	}
}


// �`�揈��
void Draw_Ground_tile(void) {
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_g_tile[0].tile.obj.tex.texNo]);


	float temp = Get_Scroll();
	PrintDebugProc("%f\n", temp);

	// �`��
	for (int i = 0; i < G_TILE_MAX; i++) {
		if (g_g_tile[i].tile.obj.pos.x > SCREEN_WIDTH + SNAP_SIZE) {
			continue;
		}

		if (g_g_tile[i].tile.obj.pos.x < -SNAP_SIZE) {
			continue;
		}
		if (g_g_tile[i].tile.obj.use) {

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer,
				g_g_tile[i].tile.obj.pos.x, g_g_tile[i].tile.obj.pos.y, g_g_tile[i].tile.obj.pol.w, g_g_tile[i].tile.obj.pol.h,
				g_g_tile[i].tile.obj.tex.x, g_g_tile[i].tile.obj.tex.y, g_g_tile[i].tile.obj.tex.w, g_g_tile[i].tile.obj.tex.h,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}



int SetG_TILE(int tex_no, XMFLOAT3 pos_size) {
	int temp = tex_no - 1;
	if (temp == -1) {
		return -1;
	}
	if (tex_no < G_TILE_MAX) {
		if (g_count < G_TILE_MAX) {
			if (!g_g_tile[g_count].tile.obj.use) {
				g_g_tile[g_count].tile.tile_no = g_count;
				g_g_tile[g_count].tile.obj.use = TRUE;
				g_g_tile[g_count].tile.obj.pos = XMFLOAT2(pos_size.x, pos_size.y);
				g_g_tile[g_count].tile.obj.pol.w = pos_size.z;
				g_g_tile[g_count].tile.obj.pol.h = pos_size.z;

				//�����蔻��
				g_g_tile[g_count].col.pos = g_g_tile[g_count].tile.obj.pos;
				g_g_tile[g_count].col.size = XMFLOAT2(g_g_tile[g_count].tile.obj.pol.w, g_g_tile[g_count].tile.obj.pol.h);
				if (SetCollision(&g_g_tile[g_count].col) == -1) {
					return g_count - 1;
				}

				g_g_tile[g_count].tile.obj.tex.x = g_g_tile[g_count].tile.obj.tex.w * (temp);
				g_g_tile[g_count].tile.obj.tex.y = g_g_tile[g_count].tile.obj.tex.h * (int(temp / TEXTUR_H));

				g_count++;
				return g_count - 1;
			}
		}
		for (int i = 0; i < G_TILE_MAX; i++) {//�폜�����ԍ����g������
			if (!g_g_tile[i].tile.obj.use) {
				g_g_tile[i].tile.tile_no = i;
				g_g_tile[i].tile.obj.use = TRUE;
				g_g_tile[i].tile.obj.pos = XMFLOAT2(pos_size.x, pos_size.y);
				g_g_tile[i].tile.obj.pol.w = pos_size.z;
				g_g_tile[i].tile.obj.pol.h = pos_size.z;

				//�����蔻��
				g_g_tile[i].col.pos = XMFLOAT2(pos_size.x, pos_size.y);
				g_g_tile[i].col.size = XMFLOAT2(pos_size.z, pos_size.z);
				if (SetCollision(&g_g_tile[i].col) == -1) {
					return -1;
				}

				g_g_tile[i].tile.obj.tex.x = g_g_tile[i].tile.obj.tex.w * (temp - TEXTUR_W);
				g_g_tile[i].tile.obj.tex.y = g_g_tile[i].tile.obj.tex.h * temp;


				return i;
			}
		}
	}
	return -1;

}

bool DelG_TILE(int count) {
	if ((count < G_TILE_MAX) && g_g_tile[count].tile.obj.use) {
		g_g_tile[count].tile.obj.use = FALSE;
		delete_Collision(g_g_tile[count].col.specific_numbers);
		return TRUE;
	}
	return FALSE;
}
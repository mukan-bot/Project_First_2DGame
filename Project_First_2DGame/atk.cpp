#include "atk.h"

#include "game.h"
#include "player.h"
#include "MAP.h"
#include "collision.h"
#include "effect.h"
#include "sound.h"




//�}�N����`
#define ATKs_MAX	(255) //��x�ɏo����U���̌��E



// �O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[MAX_ATK] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[MAX_ATK] = {
	"data/TEXTURE/attack/standard_atk.png",
	"data/TEXTURE/attack/line_atk.png",
	"data/TEXTURE/attack/standard_atk.png",
	"data/TEXTURE/attack/standard_atk.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

static attack g_atk[ATKs_MAX];

static attack_status g_atk_status[MAX_ATK];	//�U���̓������ʂɕێ�����

static int use_atk[ATKs_MAX];

static PLAYER* g_player;	//�v���C���[�̏��̏���

//�v���g�^�C�v�錾
void Update_ATK_col(int i);


// ����������
HRESULT Init_ATK(void) {
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < MAX_ATK; i++)
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

	g_player = Get_Player();

	for (int i = 0; i < ATKs_MAX; i++) {
		g_atk[i].obj.use = FALSE;
		g_atk[i].obj.pos = XMFLOAT2(0, 0);
		g_atk[i].obj.pol.w = 0;
		g_atk[i].obj.pol.h = 0;
		g_atk[i].obj.tex.texNo = 0;
		g_atk[i].obj.tex.w = 1.0;
		g_atk[i].obj.tex.h = 1.0;
		g_atk[i].obj.tex.x = 0;
		g_atk[i].obj.tex.y = 0;
		g_atk[i].atk_type = STANDARD_ATK;
		g_atk[i].is_Rside = TRUE;
		g_atk[i].col.pos = g_atk[i].obj.pos;
		g_atk[i].col.size = XMFLOAT2(0.0f, 0.0f);
		g_atk[i].col.shape = GROUND;
		g_atk[i].col.type = BOX;
		g_atk[i].count_frame = 0;
		g_atk[i].no = -1;
		
	}

	for (int i = 0; i < MAX_ATK; i++) {
		g_atk_status[i].col.pos = XMFLOAT2(0, 0);
		g_atk_status[i].col.type = ATK_ENEMY;

		g_atk_status[i].obj.use = FALSE;
		g_atk_status[i].obj.pos = XMFLOAT2(0, 0);
	}
	int i = 0;
	i = STANDARD_ATK;
	g_atk_status[i].atk_power = 0.1;
	g_atk_status[i].minus_mp = 0.0f;
	g_atk_status[i].frame = 5 ;
	g_atk_status[i].vec = XMFLOAT2(3, 0);
	g_atk_status[i].col.size = XMFLOAT2(10, 80);
	g_atk_status[i].col.shape = BOX;
	g_atk_status[i].obj.pol.w = 20;
	g_atk_status[i].obj.pol.h = 100;
	g_atk_status[i].obj.tex.texNo = i;
	g_atk_status[i].obj.tex.w = 1.0f;
	g_atk_status[i].obj.tex.h = 1.0f;
	g_atk_status[i].obj.tex.x = 0;
	g_atk_status[i].obj.tex.y = 0;
	g_atk_status[i].obj.pos = XMFLOAT2(30, 0);
	g_atk_status[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.4f);

	i = LINE_ATK;
	g_atk_status[i].atk_power = 0.20f;
	g_atk_status[i].minus_mp = 0.1f;
	g_atk_status[i].frame = 40;
	g_atk_status[i].vec = XMFLOAT2(320, 0);
	g_atk_status[i].col.size = XMFLOAT2(50, 20);
	g_atk_status[i].col.shape = BOX;
	g_atk_status[i].obj.pol.w = 60;
	g_atk_status[i].obj.pol.h = 25;
	g_atk_status[i].obj.tex.texNo = i;
	g_atk_status[i].obj.tex.w = 1.0f;
	g_atk_status[i].obj.tex.h = 1.0f;
	g_atk_status[i].obj.tex.x = 0;
	g_atk_status[i].obj.tex.y = 0;
	g_atk_status[i].obj.pos = XMFLOAT2(30, 0);
	g_atk_status[i].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	i = STANDARD_ATK_ENEMY;
	g_atk_status[i].atk_power = 0.1;
	g_atk_status[i].minus_mp = 0.0f;
	g_atk_status[i].frame = 50;
	g_atk_status[i].vec = XMFLOAT2(300, 0);
	g_atk_status[i].col.size = XMFLOAT2(10, 80);
	g_atk_status[i].col.shape = BOX;
	g_atk_status[i].obj.pol.w = 20;
	g_atk_status[i].obj.pol.h = 100;
	g_atk_status[i].obj.tex.texNo = i;
	g_atk_status[i].obj.tex.w = 1.0f;
	g_atk_status[i].obj.tex.h = 1.0f;
	g_atk_status[i].obj.tex.x = 0;
	g_atk_status[i].obj.tex.y = 0;
	g_atk_status[i].obj.pos = XMFLOAT2(30, 0);
	g_atk_status[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.4f);

	i = STANDARD_ATK_BOSS;
	g_atk_status[i].atk_power = 0.1;
	g_atk_status[i].minus_mp = 0.0f;
	g_atk_status[i].frame = 50;
	g_atk_status[i].vec = XMFLOAT2(300, 0);
	g_atk_status[i].col.size = XMFLOAT2(20, 300);
	g_atk_status[i].col.shape = BOX;
	g_atk_status[i].obj.pol.w = 60;
	g_atk_status[i].obj.pol.h = 300;
	g_atk_status[i].obj.tex.texNo = i;
	g_atk_status[i].obj.tex.w = 1.0f;
	g_atk_status[i].obj.tex.h = 1.0f;
	g_atk_status[i].obj.tex.x = 0;
	g_atk_status[i].obj.tex.y = 0;
	g_atk_status[i].obj.pos = XMFLOAT2(30, 0);
	g_atk_status[i].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f);


	g_Load = TRUE;
	return S_OK;
}

// �I������
void Uninit_ATK(void) {
	if (g_Load == FALSE) return;

	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < MAX_ATK; i++) {
		if (g_Texture[i]) {
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	g_Load = FALSE;
}



// �`�揈��
void Draw_ATK(void) {
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
	for (int i = 0; i < ATKs_MAX; i++) {
		if (g_atk[i].obj.use) {
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_atk[i].obj.tex.texNo]);

			if (g_atk[i].is_Rside) {
				switch (g_atk[i].atk_type)
				{
				default:
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColor(g_VertexBuffer, g_atk[i].obj.pos.x, g_atk[i].obj.pos.y, g_atk[i].obj.pol.w, g_atk[i].obj.pol.h, g_atk[i].obj.tex.x, g_atk[i].obj.tex.y, g_atk[i].obj.tex.w, g_atk[i].obj.tex.h, g_atk_status[g_atk[i].atk_type].color);

					break;
				}
			}
			else {
				switch (g_atk[i].atk_type)
				{
				default:
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColor(g_VertexBuffer, g_atk[i].obj.pos.x, g_atk[i].obj.pos.y, g_atk[i].obj.pol.w, g_atk[i].obj.pol.h, g_atk[i].obj.tex.x, g_atk[i].obj.tex.y, -g_atk[i].obj.tex.w, g_atk[i].obj.tex.h, g_atk_status[g_atk[i].atk_type].color);

					break;
				}
				
			}
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

}



void Update_ATK(void) {
	for (int i = 0; i < ATKs_MAX; i++) {
		if (g_atk[i].obj.use==TRUE) {
			if (g_atk[i].count_frame > g_atk_status[g_atk[i].atk_type].frame) {
				g_atk[i].obj.use = FALSE;
				Del_effect(g_atk[i].no);
			}

			bool is_move = FALSE;	//�ړ��̑I��(FALSE�Ȃ���`��Ԃ̈ړ����s��)

			

			switch (g_atk[i].atk_type)
			{
			case(STANDARD_ATK):
				break;
			case(LINE_ATK):
				break;
			case(STANDARD_ATK_ENEMY):
				is_move = TRUE;
				g_atk[i].obj.pos.x = g_atk[i].start_pos.x + (g_atk[i].vec.x - g_atk[i].start_pos.x) / g_atk_status[g_atk[i].atk_type].frame * g_atk[i].count_frame;
				g_atk[i].obj.pos.y = g_atk[i].start_pos.y + (g_atk[i].vec.y - g_atk[i].start_pos.y) / g_atk_status[g_atk[i].atk_type].frame * g_atk[i].count_frame;

			}

			if (!is_move) {	//���`��Ԃňړ�
				g_atk[i].obj.pos.x = g_atk[i].start_pos.x + (g_atk[i].vec.x - g_atk[i].start_pos.x) / g_atk_status[g_atk[i].atk_type].frame * g_atk[i].count_frame;
				g_atk[i].obj.pos.y = g_atk[i].start_pos.y + (g_atk[i].vec.y - g_atk[i].start_pos.y) / g_atk_status[g_atk[i].atk_type].frame * g_atk[i].count_frame;
			}
			Update_ATK_col(i);

			COLLISION temp = g_atk[i].col;
			temp.type = GROUND;
			if (CheckHit(temp)) {
				g_atk[i].obj.use = FALSE;
				Del_effect(g_atk[i].no);
			}
			

			g_atk[i].count_frame++;

			Update_ATK_col(i);
		}
	}
}


void Update_sATK(float plus) {
	for (int i = 0; i < ATKs_MAX; i++) {
		if (g_atk[i].obj.use == TRUE) {
			g_atk[i].start_pos.x -= plus;
			g_atk[i].vec.x -= plus;

		}
	}
}



void Update_ATK_col(int i) {
	g_atk[i].col.pos = g_atk[i].obj.pos;
}


float Set_ATK(int hit_type, int atk_type, bool is_Rside, XMFLOAT2 start_pos) {
	int i = 0;
	while (g_atk[i].obj.use){
		i++;
		if (i > ATKs_MAX) {
			return 0;
		}
	}
	g_atk[i].obj = g_atk_status[atk_type].obj;
	if (is_Rside) {
		g_atk[i].obj.pos = XMFLOAT2(start_pos.x + g_atk_status[atk_type].obj.pos.x, start_pos.y + g_atk_status[atk_type].obj.pos.y);
		g_atk[i].start_pos.x = start_pos.x + g_atk_status[atk_type].obj.pos.x;
		g_atk[i].start_pos.y = start_pos.y + g_atk_status[atk_type].obj.pos.y;
		g_atk[i].vec = XMFLOAT2(g_atk[i].start_pos.x + g_atk_status[atk_type].vec.x, g_atk[i].start_pos.y + g_atk_status[atk_type].vec.y);
	}
	else {
		g_atk[i].obj.pos = XMFLOAT2(start_pos.x - g_atk_status[atk_type].obj.pos.x, start_pos.y + g_atk_status[atk_type].obj.pos.y);
		g_atk[i].start_pos.x = start_pos.x - g_atk_status[atk_type].obj.pos.x;
		g_atk[i].start_pos.y = start_pos.y - g_atk_status[atk_type].obj.pos.y;
		g_atk[i].vec = XMFLOAT2(g_atk[i].start_pos.x - g_atk_status[atk_type].vec.x, g_atk[i].start_pos.y - g_atk_status[atk_type].vec.y);
	}
	g_atk[i].col = g_atk_status[atk_type].col;
	g_atk[i].col.type = hit_type;
	g_atk[i].atk_type = atk_type;
	g_atk[i].is_Rside = is_Rside;
	g_atk[i].obj.use = TRUE;
	g_atk[i].count_frame = 0;
	g_atk[i].no = -1;
	Update_ATK_col(i);
	switch (atk_type)
	{
	case (STANDARD_ATK):
		PlaySound(SOUND_LABEL_SE_standard_atk);
		break;
	case(LINE_ATK):
		g_atk[i].no = Set_effect(&g_atk[i].obj.pos, is_Rside, 100, EFFECT_TYPE_ICE_SPEAR, FALSE, g_atk_status[atk_type].frame);
		PlaySound(SOUND_LABEL_SE_line_atk);
		break;
	case(STANDARD_ATK_ENEMY):
		PlaySound(SOUND_LABEL_SE_standard_atk);
		break;
	case(STANDARD_ATK_BOSS):
		PlaySound(SOUND_LABEL_SE_standard_atk);
	default:
		break;
	}
	return g_atk_status[atk_type].minus_mp;
}





float CheckDamage(COLLISION collision) {
	//�l���g���₷���悤�ɂ܂Ƃ߂Ă���collision
	XMFLOAT4 temp_c;
	XMFLOAT2 pos_c[4];	//BOX�̎l���̍��W�����Ă���
	if (collision.shape == BOX) {
		//��,�E,��,��
		temp_c = XMFLOAT4(collision.pos.x - (collision.size.x / 2.0f), collision.pos.x + (collision.size.x / 2.0f), collision.pos.y - (collision.size.y / 2.0f), collision.pos.y + (collision.size.y / 2.0f));

		pos_c[0] = XMFLOAT2(collision.pos.x - collision.size.x / 2, collision.pos.y - collision.size.y / 2);	//����
		pos_c[1] = XMFLOAT2(collision.pos.x + collision.size.x / 2, collision.pos.y - collision.size.y / 2);	//�E��
		pos_c[2] = XMFLOAT2(collision.pos.x - collision.size.x / 2, collision.pos.y + collision.size.y / 2);	//����
		pos_c[3] = XMFLOAT2(collision.pos.x + collision.size.x / 2, collision.pos.y + collision.size.y / 2);	//�E��
	}
	else if (collision.shape == CIRCLE) {

	}
	bool temp = FALSE;
	int i = 0;
	for (i; i <= ATKs_MAX; i++) {
		if (g_atk[i].obj.use) {//�g�p���Ă��Ȃ����̓X���[

			if (g_atk[i].col.type == collision.type){
				//�l���g���₷���悤�ɂ܂Ƃ߂Ă���g_collision
				XMFLOAT4 temp_g;
				//��,�E,��,��
				temp_g = XMFLOAT4(g_atk[i].col.pos.x - (g_atk[i].col.size.x / 2.0f), g_atk[i].col.pos.x + (g_atk[i].col.size.x / 2.0f), g_atk[i].col.pos.y - (g_atk[i].col.size.y / 2.0f), g_atk[i].col.pos.y + (g_atk[i].col.size.y / 2.0f));

				if (g_atk[i].col.shape == BOX) {
					//BOX��BOX
					if (collision.shape == BOX) {

						if ((temp_g.x < temp_c.y) && (temp_g.y > temp_c.x) && (temp_g.z < temp_c.w) && (temp_g.w > temp_c.z)) {
							temp = TRUE;
						}
					}
					//BOX��CIRCLE
					else if (collision.shape == CIRCLE) {
						XMFLOAT2 pos_g[4];	//BOX�̎l���̍��W�����Ă���
						pos_g[0] = XMFLOAT2(g_atk[i].col.pos.x - g_atk[i].col.size.x / 2, g_atk[i].col.pos.y - g_atk[i].col.size.y / 2);	//����
						pos_g[1] = XMFLOAT2(g_atk[i].col.pos.x + g_atk[i].col.size.x / 2, g_atk[i].col.pos.y - g_atk[i].col.size.y / 2);	//�E��
						pos_g[2] = XMFLOAT2(g_atk[i].col.pos.x - g_atk[i].col.size.x / 2, g_atk[i].col.pos.y + g_atk[i].col.size.y / 2);	//����
						pos_g[3] = XMFLOAT2(g_atk[i].col.pos.x + g_atk[i].col.size.x / 2, g_atk[i].col.pos.y + g_atk[i].col.size.y / 2);	//�E��
						float temp;
						for (int j = 0; j < 8; j += 2) {
							temp = distance_line_point(pos_g[j], XMFLOAT2(pos_g[j + 1].x - pos_g[j].x, pos_g[j + 1].y - pos_g[j].y), collision.pos);
							if (temp < collision.size.x * collision.size.x) {
								temp = TRUE;
							}
						}
					}
				}
				else {
					//CIRCLE��BOX
					if (collision.shape == BOX) {
						float temp;
						for (int j = 0; j < 8; j += 2) {
							temp = distance_line_point(pos_c[j], XMFLOAT2(pos_c[j + 1].x - pos_c[j].x, pos_c[j + 1].y - pos_c[j].y), g_atk[i].col.pos);
							if (temp < g_atk[i].col.size.x * g_atk[i].col.size.x) {
								temp = TRUE;
							}
						}

					}
					//CIRCLE��CIRCLE
					else if (collision.shape == CIRCLE) {
						float temp = distance_point(g_atk[i].col.pos, collision.pos);
						float r = g_atk[i].col.size.x + collision.size.x;
						if (temp < r * r) {
							temp = TRUE;
						}
					}
				}
			}
		}
		if (temp) {
			g_atk[i].obj.use = FALSE;
			Del_effect(g_atk[i].no);
			return g_atk_status[g_atk[i].atk_type].atk_power;
		}
	}

	return 0.0f;
}




attack_status Get_Atk_status(int atk_type) {
	return g_atk_status[atk_type];
}
#include "enemy.h"
#include "input.h"

#include "MAP.h"
#include "Visual_tile.h"
#include "Ground_tile.h"
#include "HUD.h"
#include "atk.h"

#include "enemy_Update.h"

//�}�N����`
#define TEXTURE_MAX	(2)

#define ENEMY_MAX (5)	//�G�̍ő吔


//�A�j���[�V����
#define ANIME_FPS (15)




#define SIZE	(100.0f)


//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/enemy/huntress.png",
	"data/TEXTURE/test.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O



static ENEMY g_Enemy[ENEMY_MAX];
static ENEMY_STATUS g_Enemy_status[ENEMY_TYPE_MAX];


//�v���g�^�C�v�錾
void Update_E_col(int i);
void Anime_enemy(int i);




HRESULT Init_enemy(void) {
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

	for (int i = 0; i < ENEMY_MAX; i++) {

		//enemy�ϐ��̏�����
		g_Enemy[i].obj.pos = XMFLOAT2(400.0f, 200.0f);
		g_Enemy[i].obj.use = FALSE;
		g_Enemy[i].obj.pol.w = SIZE;
		g_Enemy[i].obj.pol.h = SIZE;
		g_Enemy[i].obj.tex.x = 0.0f;
		g_Enemy[i].obj.tex.y = 0.0f;
		g_Enemy[i].obj.tex.w = 1.0f;
		g_Enemy[i].obj.tex.h = 1.0f;

		g_Enemy[i].is_hitD = FALSE;
		g_Enemy[i].is_hitU = FALSE;
		g_Enemy[i].is_hitL = FALSE;
		g_Enemy[i].is_hitR = FALSE;

		g_Enemy[i].is_jump = FALSE;


		//�����蔻��
		//�����蔻��S��
		g_Enemy[i].col.pos = g_Enemy[i].obj.pos;
		g_Enemy[i].col.shape = BOX;
		g_Enemy[i].col.size = XMFLOAT2(0, 0);
		g_Enemy[i].col.type = ATK_ENEMY;
		//�����蔻�艺
		g_Enemy[i].col_D.pos.x = g_Enemy[i].obj.pos.x;
		g_Enemy[i].col_D.pos.y = g_Enemy[i].obj.pos.y;
		g_Enemy[i].col_D.shape = BOX;
		g_Enemy[i].col_D.size = XMFLOAT2(0, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy[i].col_D.type = GROUND;
		//�����蔻���
		g_Enemy[i].col_U.pos.x = g_Enemy[i].obj.pos.x;
		g_Enemy[i].col_U.pos.y = g_Enemy[i].obj.pos.y;
		g_Enemy[i].col_U.shape = BOX;
		g_Enemy[i].col_U.size = XMFLOAT2(0, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy[i].col_U.type = GROUND;
		//�����蔻��E
		g_Enemy[i].col_R.pos.x = g_Enemy[i].obj.pos.y;
		g_Enemy[i].col_R.pos.y = g_Enemy[i].obj.pos.x;
		g_Enemy[i].col_R.shape = BOX;
		g_Enemy[i].col_R.size = XMFLOAT2(0.01f, 0);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy[i].col_R.type = GROUND;
		//�����蔻�荶
		g_Enemy[i].col_L.pos.x = g_Enemy[i].obj.pos.y;
		g_Enemy[i].col_L.pos.y = g_Enemy[i].obj.pos.x;
		g_Enemy[i].col_L.shape = BOX;
		g_Enemy[i].col_L.size = XMFLOAT2(1.01f,0);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy[i].col_L.type = GROUND;


		//Animation
		g_Enemy[i].anime.anime_frame = 0;
		g_Enemy[i].anime.anime_FPS = ANIME_FPS;
		g_Enemy[i].anime.count_FPS = 0;
		g_Enemy[i].anime.which_anime = E_IDLE_ANIME;


	}


	//�X�e�[�^�X�̏�����
	for (int i = 0; i < ENEMY_TYPE_MAX; i++) {
		g_Enemy_status[i].status.hp = 1.0f;
		g_Enemy_status[i].status.mp = 1.0f;
		g_Enemy_status[i].status.plus_mp = 0.005f;
		g_Enemy_status[i].status.atk = 20.0f;
		g_Enemy_status[i].status.matk = 1.5f;
		g_Enemy_status[i].status.speed = 3;
		g_Enemy_status[i].status.jump_speed = 3;
		g_Enemy_status[i].status.gravity = 3;
		g_Enemy_status[i].status.temp_gravity = 0;
		g_Enemy_status[i].aspect = XMFLOAT2(1.0f, 1.0f);
		g_Enemy_status[i].size = 100;
		g_Enemy_status[i].col_size = 90;

		g_Enemy_status[i].anime.anime_count = 1;
		g_Enemy_status[i].anime.anime_number = 1;
		for (int j = 0; j < E_ANIME_MAX; j++) {
			g_Enemy_status[i].anime_type[j].anime_type = E_NO_ANIME;
			g_Enemy_status[i].anime_type[j].frame = 0;
		}

		g_Enemy_status[i].obj.pos = XMFLOAT2(0.0f, 0.0f);
		g_Enemy_status[i].obj.use = FALSE;
		g_Enemy_status[i].obj.pol.w = g_Enemy_status[i].size * g_Enemy_status[i].aspect.x;
		g_Enemy_status[i].obj.pol.h = g_Enemy_status[i].size * g_Enemy_status[i].aspect.y;
		g_Enemy_status[i].obj.tex.x = 0.0f;
		g_Enemy_status[i].obj.tex.y = 0.0f;
		g_Enemy_status[i].obj.tex.w = 1.0f / g_Enemy_status[i].anime.anime_count;//�c������
		g_Enemy_status[i].obj.tex.h = 1.0f / g_Enemy_status[i].anime.anime_number;//��������
		//�����蔻��
		//�����蔻��S��
		g_Enemy_status[i].col.shape = BOX;
		g_Enemy_status[i].col.size = XMFLOAT2(g_Enemy_status[i].size * g_Enemy_status[i].aspect.x, g_Enemy_status[i].size * g_Enemy_status[i].aspect.y);
		g_Enemy_status[i].col.type = ATK_ENEMY;
		//�����蔻�艺
		g_Enemy_status[i].col_D.shape = BOX;
		g_Enemy_status[i].col_D.size = XMFLOAT2(g_Enemy_status[i].size * g_Enemy_status[i].aspect.x - 10, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].col_D.type = GROUND;
		//�����蔻���
		g_Enemy_status[i].col_U.shape = BOX;
		g_Enemy_status[i].col_U.size = XMFLOAT2(g_Enemy_status[i].size * g_Enemy_status[i].aspect.x - 10, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].col_U.type = GROUND;
		//�����蔻��E
		g_Enemy_status[i].col_R.shape = BOX;
		g_Enemy_status[i].col_R.size = XMFLOAT2(0.01f, g_Enemy_status[i].size * g_Enemy_status[i].aspect.y - 10);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].col_R.type = GROUND;
		//�����蔻�荶
		g_Enemy_status[i].col_L.shape = BOX;
		g_Enemy_status[i].col_L.size = XMFLOAT2(1.01f, g_Enemy_status[i].size * g_Enemy_status[i].aspect.y - 10);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].col_L.type = GROUND;

	}
	int i;
	//�ʂ̐ݒ�
	i = ENEMY_HUNTRESS;
	g_Enemy_status[i].aspect = XMFLOAT2(1.0f, 0.5f);
	g_Enemy_status[i].size = 200;
	g_Enemy_status[i].col_size = 90;

	g_Enemy_status[i].anime.anime_count = 8;
	g_Enemy_status[i].anime.anime_number = 6;
	g_Enemy_status[i].anime_type[0].anime_type = E_ATK1_ANIME;
	g_Enemy_status[i].anime_type[0].frame = 5;
	g_Enemy_status[i].anime_type[1].anime_type = E_ATK2_ANIME;
	g_Enemy_status[i].anime_type[1].frame = 7;
	g_Enemy_status[i].anime_type[2].anime_type = E_DEATH_ANIME;
	g_Enemy_status[i].anime_type[2].frame = 8;
	g_Enemy_status[i].anime_type[3].anime_type = E_RUN_ANIME;
	g_Enemy_status[i].anime_type[3].frame = 8;
	g_Enemy_status[i].anime_type[4].anime_type = E_IDLE_ANIME;
	g_Enemy_status[i].anime_type[4].frame = 8;
	g_Enemy_status[i].anime_type[5].anime_type = E_DAMAGE_ANIME;
	g_Enemy_status[i].anime_type[5].frame = 9;

	g_Enemy_status[i].obj.pos = XMFLOAT2(0.0f, 0.0f);
	g_Enemy_status[i].obj.use = FALSE;
	g_Enemy_status[i].obj.pol.w = g_Enemy_status[i].size * g_Enemy_status[i].aspect.x;
	g_Enemy_status[i].obj.pol.h = g_Enemy_status[i].size * g_Enemy_status[i].aspect.y;
	g_Enemy_status[i].obj.tex.x = 0.0f;
	g_Enemy_status[i].obj.tex.y = 0.0f;
	g_Enemy_status[i].obj.tex.w = 1.0f / g_Enemy_status[i].anime.anime_count;//�c������
	g_Enemy_status[i].obj.tex.h = 1.0f / g_Enemy_status[i].anime.anime_number;//��������
	//�����蔻��
	//�����蔻��S��
	g_Enemy_status[i].col.shape = BOX;
	g_Enemy_status[i].col.size = XMFLOAT2(g_Enemy_status[i].size * g_Enemy_status[i].aspect.x, g_Enemy_status[i].size * g_Enemy_status[i].aspect.y);
	g_Enemy_status[i].col.type = ATK_PLAYER;
	//�����蔻�艺
	g_Enemy_status[i].col_D.shape = BOX;
	g_Enemy_status[i].col_D.size = XMFLOAT2(g_Enemy_status[i].size * g_Enemy_status[i].aspect.x - 10, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
	g_Enemy_status[i].col_D.type = GROUND;
	//�����蔻���
	g_Enemy_status[i].col_U.shape = BOX;
	g_Enemy_status[i].col_U.size = XMFLOAT2(g_Enemy_status[i].size * g_Enemy_status[i].aspect.x - 10, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
	g_Enemy_status[i].col_U.type = GROUND;
	//�����蔻��E
	g_Enemy_status[i].col_R.shape = BOX;
	g_Enemy_status[i].col_R.size = XMFLOAT2(0.01f, g_Enemy_status[i].size * g_Enemy_status[i].aspect.y - 10);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
	g_Enemy_status[i].col_R.type = GROUND;
	//�����蔻�荶
	g_Enemy_status[i].col_L.shape = BOX;
	g_Enemy_status[i].col_L.size = XMFLOAT2(1.01f, g_Enemy_status[i].size * g_Enemy_status[i].aspect.y - 10);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
	g_Enemy_status[i].col_L.type = GROUND;





	//SetCollision(&g_Enemy.col);

	Init_HUD();

	return S_OK;
}



void Uninit_enemy(void) {
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

void Update_enemy(void) {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_Enemy[i].obj.use) {
			//Enemy���ǂ����ɂ������Ă��邩
			if (CheckHit(g_Enemy[i].col_D)) {
				g_Enemy[i].is_hitD = TRUE;
			}
			else {
				g_Enemy[i].is_hitD = FALSE;
			}
			if (CheckHit(g_Enemy[i].col_U)) {
				g_Enemy[i].is_hitU = TRUE;
			}
			else {
				g_Enemy[i].is_hitU = FALSE;
			}
			if (CheckHit(g_Enemy[i].col_L)) {
				g_Enemy[i].is_hitL = TRUE;
			}
			else {
				g_Enemy[i].is_hitL = FALSE;
			}
			if (CheckHit(g_Enemy[i].col_R)) {
				g_Enemy[i].is_hitR = TRUE;
			}
			else {
				g_Enemy[i].is_hitR = FALSE;
			}

			Enemy_s_Update(&g_Enemy[i]);

			Update_E_col(i);

			Anime_enemy(i);
		}
	}
}



void Update_E_col(int i) {
	//�����蔻��
	g_Enemy[i].col.pos = g_Enemy[i].obj.pos;

	//�����蔻�艺
	g_Enemy[i].col_D.pos.x = g_Enemy[i].obj.pos.x;
	g_Enemy[i].col_D.pos.y = g_Enemy[i].obj.pos.y + g_Enemy_status[g_Enemy[i].enemy_type].size * g_Enemy_status[g_Enemy[i].enemy_type].aspect.x / 2;

	//�����蔻���
	g_Enemy[i].col_U.pos.x = g_Enemy[i].obj.pos.x;
	g_Enemy[i].col_U.pos.y = g_Enemy[i].obj.pos.y - g_Enemy_status[g_Enemy[i].enemy_type].size * g_Enemy_status[g_Enemy[i].enemy_type].aspect.x / 2;

	//�����蔻��E
	g_Enemy[i].col_R.pos.x = g_Enemy[i].obj.pos.x + g_Enemy_status[g_Enemy[i].enemy_type].size * g_Enemy_status[g_Enemy[i].enemy_type].aspect.y / 2;
	g_Enemy[i].col_R.pos.y = g_Enemy[i].obj.pos.y;

	//�����蔻�荶
	g_Enemy[i].col_L.pos.x = g_Enemy[i].obj.pos.x - g_Enemy_status[g_Enemy[i].enemy_type].size * g_Enemy_status[g_Enemy[i].enemy_type].aspect.y / 2;
	g_Enemy[i].col_L.pos.y = g_Enemy[i].obj.pos.y;
}

void Update_eMove(float plus) {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_Enemy[i].obj.use) {
			g_Enemy[i].obj.pos.x -= Get_PlusScroll();
		}
	}
}

void Anime_enemy(int i) {
	if (g_Enemy[i].anime.count_FPS >= g_Enemy[i].anime.anime_FPS) {
		if (g_Enemy[i].anime.anime_frame >= g_Enemy[i].anime.anime_frame_max) {
			g_Enemy[i].anime.anime_frame = 0;
		}

		g_Enemy[i].obj.tex.x = g_Enemy[i].obj.tex.w * g_Enemy[i].anime.anime_frame;
		g_Enemy[i].anime.anime_frame++;
		g_Enemy[i].anime.count_FPS = 0;
	}

	g_Enemy[i].anime.count_FPS++;
}

void Set_E_Anime(int enemy_no,int anime) {
	g_Enemy[enemy_no].anime.which_anime = anime;
	int i = 0;
	while (g_Enemy_status[g_Enemy[enemy_no].enemy_type].anime_type[i].anime_type!=anime){
		i++;
	}
	g_Enemy[enemy_no].anime.anime_frame_max = g_Enemy_status[g_Enemy[enemy_no].enemy_type].anime_type[i].frame;
	g_Enemy[enemy_no].obj.tex.y = g_Enemy[enemy_no].obj.tex.h * i;
}




void Draw_enemy(void) {

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_Enemy[i].obj.use) {
			// �}�e���A���ݒ�
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			SetMaterial(material);


			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			if (g_Enemy[i].is_run_R) {
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, g_Enemy[i].obj.pos.x, g_Enemy[i].obj.pos.y, g_Enemy[i].obj.pol.w, g_Enemy[i].obj.pol.h, g_Enemy[i].obj.tex.x, g_Enemy[i].obj.tex.y, g_Enemy[i].obj.tex.w, g_Enemy[i].obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else {
				SetSpriteColor(g_VertexBuffer, g_Enemy[i].obj.pos.x, g_Enemy[i].obj.pos.y, g_Enemy[i].obj.pol.w, g_Enemy[i].obj.pol.h, g_Enemy[i].obj.tex.x, g_Enemy[i].obj.tex.y, -g_Enemy[i].obj.tex.w, g_Enemy[i].obj.tex.h, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			}

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
		}
	}
}




ENEMY* Get_Enemy(void) {
	return &g_Enemy[0];
}


int Set_Enemy(int enemy_type, XMFLOAT2 pos) {
	int i = 0;
	while (g_Enemy[i].obj.use) {
		i++;
		if (i > ENEMY_MAX) {
			return -1;//���s
		}
	}
	g_Enemy[i].enemy_type = enemy_type;
	g_Enemy[i].status = g_Enemy_status[enemy_type].status;
	g_Enemy[i].obj = g_Enemy_status[enemy_type].obj;
	g_Enemy[i].obj.pos = pos;
	g_Enemy[i].col = g_Enemy_status[enemy_type].col;
	g_Enemy[i].col_D = g_Enemy_status[enemy_type].col;
	g_Enemy[i].col_U = g_Enemy_status[enemy_type].col;
	g_Enemy[i].col_R = g_Enemy_status[enemy_type].col;
	g_Enemy[i].col_L = g_Enemy_status[enemy_type].col;
	Set_E_Anime(i, E_IDLE_ANIME);
	g_Enemy[i].obj.use = TRUE;
	Update_E_col(i);
	SetCollision(&g_Enemy[i].col);
	while (g_Enemy[i].is_hitD && g_Enemy[i].is_hitU && g_Enemy[i].is_hitL && g_Enemy[i].is_hitR){
		//�����ʒu���߂荞��ł����ꍇ�Ɉړ�
		if (CheckHit(g_Enemy[i].col_D)) {
			g_Enemy[i].is_hitD = TRUE;
			g_Enemy[i].obj.pos.y--;
		}
		else {
			g_Enemy[i].is_hitD = FALSE;
		}
		if (CheckHit(g_Enemy[i].col_U)) {
			g_Enemy[i].is_hitU = TRUE;
			g_Enemy[i].obj.pos.y--;
		}
		else {
			g_Enemy[i].is_hitU = FALSE;
		}
		if (CheckHit(g_Enemy[i].col_L)) {
			g_Enemy[i].is_hitL = TRUE;
			g_Enemy[i].obj.pos.x--;
		}
		else {
			g_Enemy[i].is_hitL = FALSE;
		}
		if (CheckHit(g_Enemy[i].col_R)) {
			g_Enemy[i].is_hitR = TRUE;
			g_Enemy[i].obj.pos.x--;
		}
		else {
			g_Enemy[i].is_hitR = FALSE;
		}
		Update_E_col(i);
	}
	return i;//�����i�G�l�~�[�̔ԍ��j
}

bool DEL_Enemy(int i) {
	if (g_Enemy[i].obj.use) {
		g_Enemy[i].obj.use = FALSE;
		return TRUE;//����
	}
	return FALSE;//���s
}
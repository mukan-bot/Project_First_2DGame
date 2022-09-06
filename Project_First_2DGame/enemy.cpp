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

#define ENEMY_MAX (100)	//�G�̍ő吔







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

		//Animation
		g_Enemy[i].anime.anime_frame = 0;
		g_Enemy[i].anime.anime_FPS = 0;
		g_Enemy[i].anime.count_FPS = 0;
		g_Enemy[i].anime.which_anime = E_IDLE_ANIME;

		g_Enemy[i].count_atk = 0;
	}


	int i;
	//�ʂ̐ݒ�
	//�n���g���X�̐ݒ�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	i = ENEMY_HUNTRESS;
	g_Enemy_status[i].enemy.status.hp = 1.0f;
	g_Enemy_status[i].enemy.status.mp = 1.0f;
	g_Enemy_status[i].enemy.status.plus_mp = 0.005f;
	g_Enemy_status[i].enemy.status.atk = 20.0f;
	g_Enemy_status[i].enemy.status.matk = 1.5f;
	g_Enemy_status[i].enemy.status.speed = 3;
	g_Enemy_status[i].enemy.status.jump_speed = 3;
	g_Enemy_status[i].enemy.status.gravity = 3;
	g_Enemy_status[i].enemy.status.temp_gravity = 0;

	g_Enemy_status[i].aspect = XMFLOAT2(1.0f, 0.5f);
	g_Enemy_status[i].size = 200;
	g_Enemy_status[i].col_size = XMFLOAT2(50, 90);


	g_Enemy_status[i].enemy.anime.anime_count = 8;
	g_Enemy_status[i].enemy.anime.anime_number = 6;
	g_Enemy_status[i].enemy.anime.anime_FPS = 15;
	g_Enemy_status[i].enemy.anime.which_anime = E_IDLE_ANIME;
	g_Enemy_status[i].enemy.atk_waiting = g_Enemy_status[i].enemy.anime.anime_FPS * 2.5;
	g_Enemy_status[i].enemy.atk_waiting_count = 0;


	g_Enemy_status[i].anime_type[0].anime_type = E_ATK1_ANIME;
	g_Enemy_status[i].anime_type[0].frame = 5;
	g_Enemy_status[i].anime_type[0].anime_loop = FALSE;

	g_Enemy_status[i].anime_type[1].anime_type = E_ATK2_ANIME;
	g_Enemy_status[i].anime_type[1].frame = 7;
	g_Enemy_status[i].anime_type[1].anime_loop = FALSE;

	g_Enemy_status[i].anime_type[2].anime_type = E_DEATH_ANIME;
	g_Enemy_status[i].anime_type[2].frame = 8;
	g_Enemy_status[i].anime_type[2].anime_loop = FALSE;

	g_Enemy_status[i].anime_type[3].anime_type = E_RUN_ANIME;
	g_Enemy_status[i].anime_type[3].frame = 8;
	g_Enemy_status[i].anime_type[3].anime_loop = TRUE;

	g_Enemy_status[i].anime_type[4].anime_type = E_IDLE_ANIME;
	g_Enemy_status[i].anime_type[4].frame = 8;
	g_Enemy_status[i].anime_type[4].anime_loop = TRUE;

	g_Enemy_status[i].anime_type[5].anime_type = E_DAMAGE_ANIME;
	g_Enemy_status[i].anime_type[5].frame = 3;
	g_Enemy_status[i].anime_type[5].anime_loop = FALSE;


	g_Enemy_status[i].enemy.obj.pos = XMFLOAT2(0.0f, 0.0f);
	g_Enemy_status[i].enemy.obj.use = FALSE;
	g_Enemy_status[i].enemy.obj.pol.w = g_Enemy_status[i].size * g_Enemy_status[i].aspect.x;
	g_Enemy_status[i].enemy.obj.pol.h = g_Enemy_status[i].size * g_Enemy_status[i].aspect.y;
	g_Enemy_status[i].enemy.obj.tex.x = 0.0f;
	g_Enemy_status[i].enemy.obj.tex.y = 0.0f;
	g_Enemy_status[i].enemy.obj.tex.w = 1.0f / g_Enemy_status[i].enemy.anime.anime_count;//�c������
	g_Enemy_status[i].enemy.obj.tex.h = 1.0f / g_Enemy_status[i].enemy.anime.anime_number;//��������




	for (i = 0; i < ENEMY_TYPE_MAX; i++) {
		//�����蔻��
		//�����蔻��S��
		g_Enemy_status[i].enemy.col.shape = BOX;
		g_Enemy_status[i].enemy.col.size = g_Enemy_status[i].col_size;
		g_Enemy_status[i].enemy.col.type = ATK_PLAYER;
		//�����蔻�艺
		g_Enemy_status[i].enemy.col_D.shape = BOX;
		g_Enemy_status[i].enemy.col_D.size = XMFLOAT2(g_Enemy_status[i].col_size.x - 10, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].enemy.col_D.type = GROUND;
		//�����蔻���
		g_Enemy_status[i].enemy.col_U.shape = BOX;
		g_Enemy_status[i].enemy.col_U.size = XMFLOAT2(g_Enemy_status[i].col_size.x - 10, 0.01f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].enemy.col_U.type = GROUND;
		//�����蔻��E
		g_Enemy_status[i].enemy.col_R.shape = BOX;
		g_Enemy_status[i].enemy.col_R.size = XMFLOAT2(0.01f, g_Enemy_status[i].col_size.y - 10.0f);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].enemy.col_R.type = GROUND;
		//�����蔻�荶
		g_Enemy_status[i].enemy.col_L.shape = BOX;
		g_Enemy_status[i].enemy.col_L.size = XMFLOAT2(0.01f, g_Enemy_status[i].col_size.y - 10);//-10�͕K�v�̂Ȃ����̂Ɠ�����Ȃ��悤�ɂ��邽��
		g_Enemy_status[i].enemy.col_L.type = GROUND;

	}

	//�G�l�~�[�̌ʂ̓�������������鏊�̏�����
	Init_Enemy_s_Update();

	return S_OK;
}



void Uninit_enemy(void) {
	if (g_Load == FALSE) return;
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
			g_Enemy[i].enemy_no = i;

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

			//�G�l�~�[�̌ʂ̓�������������鏊�̍X�V����
			Enemy_s_Update(&g_Enemy[i]);

			//�G�l�~�[�̓����蔻��̍X�V
			Update_E_col(i);

			//�A�j���[�V�����̍X�V
			Anime_enemy(i);
		}
	}
}



void Update_E_col(int i) {
	//�����蔻��
	g_Enemy[i].col.pos = g_Enemy[i].obj.pos;

	//�����蔻�艺
	g_Enemy[i].col_D.pos.x = g_Enemy[i].obj.pos.x;
	g_Enemy[i].col_D.pos.y = g_Enemy[i].obj.pos.y + g_Enemy_status[g_Enemy[i].enemy_type].col_size.y / 2;

	//�����蔻���
	g_Enemy[i].col_U.pos.x = g_Enemy[i].obj.pos.x;
	g_Enemy[i].col_U.pos.y = g_Enemy[i].obj.pos.y - g_Enemy_status[g_Enemy[i].enemy_type].col_size.y / 2;

	//�����蔻��E
	g_Enemy[i].col_R.pos.x = g_Enemy[i].obj.pos.x + g_Enemy_status[g_Enemy[i].enemy_type].col_size.x / 2;
	g_Enemy[i].col_R.pos.y = g_Enemy[i].obj.pos.y;

	//�����蔻�荶
	g_Enemy[i].col_L.pos.x = g_Enemy[i].obj.pos.x - g_Enemy_status[g_Enemy[i].enemy_type].col_size.x / 2;
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
			if (g_Enemy_status[g_Enemy[i].enemy_type].anime_type[g_Enemy[i].anime.which_anime].anime_loop) {
				g_Enemy[i].anime.anime_frame = 0;
			}
			else {
				Set_E_Anime(i, E_IDLE_ANIME);
			}
		}
		if (g_Enemy[i].is_run_R) {
			g_Enemy[i].obj.tex.x = g_Enemy[i].obj.tex.w * g_Enemy[i].anime.anime_frame;
		}
		else {
			g_Enemy[i].obj.tex.x = g_Enemy[i].obj.tex.w + (g_Enemy[i].obj.tex.w * g_Enemy[i].anime.anime_frame);
		}
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
		if (i > E_ANIME_MAX) return;
	}
	g_Enemy[enemy_no].anime.anime_frame_max = g_Enemy_status[g_Enemy[enemy_no].enemy_type].anime_type[i].frame;
	g_Enemy[enemy_no].anime.anime_frame = 0;
	g_Enemy[enemy_no].anime.count_FPS = 0;
	g_Enemy[enemy_no].obj.tex.y = g_Enemy[enemy_no].obj.tex.h * i;
	if (g_Enemy[enemy_no].is_run_R) {
		g_Enemy[enemy_no].obj.tex.x = 0.0f;
	}
	else {

		g_Enemy[enemy_no].obj.tex.x = g_Enemy[enemy_no].obj.tex.w;
	}
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


	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	for (int i = 0; i < ENEMY_MAX; i++) {

		if (g_Enemy[i].obj.pos.x > SCREEN_WIDTH + g_Enemy[i].obj.pol.w) {
			continue;
		}
		if (g_Enemy[i].obj.pos.x < -g_Enemy[i].obj.pol.w) {
			continue;
		}


		if (g_Enemy[i].obj.use) {


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

#ifdef _DEBUG
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
			SetSpriteColor(g_VertexBuffer, g_Enemy[i].col_D.pos.x, g_Enemy[i].col_D.pos.y, g_Enemy[i].col_D.size.x + 1, g_Enemy[i].col_D.size.y + 1, 0, 0, 1, 1, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			GetDeviceContext()->Draw(4, 0);

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
			SetSpriteColor(g_VertexBuffer, g_Enemy[i].col_U.pos.x, g_Enemy[i].col_U.pos.y, g_Enemy[i].col_U.size.x + 1, g_Enemy[i].col_U.size.y + 1, 0, 0, 1, 1, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			GetDeviceContext()->Draw(4, 0);

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
			SetSpriteColor(g_VertexBuffer, g_Enemy[i].col_L.pos.x, g_Enemy[i].col_L.pos.y, g_Enemy[i].col_L.size.x + 1, g_Enemy[i].col_L.size.y + 1, 0, 0, 1, 1, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			GetDeviceContext()->Draw(4, 0);

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
			SetSpriteColor(g_VertexBuffer, g_Enemy[i].col_R.pos.x, g_Enemy[i].col_R.pos.y, g_Enemy[i].col_R.size.x + 1, g_Enemy[i].col_R.size.y + 1, 0, 0, 1, 1, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			GetDeviceContext()->Draw(4, 0);
#endif // __DEBUG




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
	g_Enemy[i] = g_Enemy_status[enemy_type].enemy;
	g_Enemy[i].enemy_type = enemy_type;
	g_Enemy[i].obj.pos = pos;
	g_Enemy[i].obj.use = TRUE;

	Set_E_Anime(i, E_IDLE_ANIME);

	Update_E_col(i);
	//�����ʒu���߂荞��ł����ꍇ�Ɉړ�
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
	while (g_Enemy[i].is_hitD || g_Enemy[i].is_hitU || g_Enemy[i].is_hitL || g_Enemy[i].is_hitR){
		//�����ʒu���߂荞��ł����ꍇ�Ɉړ�
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

		g_Enemy[i].obj.pos.y--;

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
#include "player.h"

//�}�N����`
#define TEXTURE_MAX	(1)

//�O���[�o���ϐ�
static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/font.png",
};

HRESULT Init_player(void) {
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`���̐���
	for (int i = 0; i < TEXTURE_MAX; i++) {
		
	}
	return S_OK;
}
void Uninit_player(void) {

}
void Update_player(void) {

}
void Draw_player(void) {

}
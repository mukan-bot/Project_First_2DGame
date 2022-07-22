#include "player.h"

//マクロ定義
#define TEXTURE_MAX	(1)

//グローバル変数
static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/font.png",
};

HRESULT Init_player(void) {
	ID3D11Device* pDevice = GetDevice();

	//テクスチャの生成
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
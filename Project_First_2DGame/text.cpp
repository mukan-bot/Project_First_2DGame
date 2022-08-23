#include "text.h"

#include <stdio.h>

// マクロ定義

#define TEXTURE_WIDTH				(80)	// サイズ
#define TEXTURE_HEIGHT				(120)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(62)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

//構造体定義
struct TEXT_PRIVATE
{
	XMFLOAT2	pos;		// ポリゴンの座標
	float		w, h;		// 幅と高さ

	float tw = 0.0207333333333333f;	// テクスチャの幅
	float th = 0.0823333333333333f;	// テクスチャの高さ
	float tx = 0.00f;	// テクスチャの左上X座標
	float ty = 0.00f;	// テクスチャの左上Y座標
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	bool use = FALSE;
};



// グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報


static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/font.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static TEXT_PRIVATE	g_TEXT[TEXT_MAX];

char g_textlist[293][48][3] = {//使える文字列
	{"あ","い","う","え","お","か","き","く","け","こ","さ","し","す","せ","そ","た","ち","つ","て","と","な","に","ぬ","ね","の","は","ひ","ふ","へ","ほ","ま","み","む","め","も","や","ゆ","よ","ら","り","る","れ","ろ","わ","ゐ","ゑ","を","ん",
	}, {"が","ぎ","ぐ","げ","ご","ざ","じ","ず","ぜ","ぞ","だ","ぢ","づ","で","ど","ば","び","ぶ","べ","ぼ","ぱ","ぴ","ぷ","ぺ","ぽ",
	}, {"ぁ","ぃ","ぅ","ぇ","ぉ","っ","ゃ","ゅ","ょ",
	}, {"ア","イ","ウ","エ","オ","カ","キ","ク","ケ","コ","サ","シ","ス","セ","ソ","タ","チ","ツ","テ","ト","ナ","ニ","ヌ","ネ","ノ","ハ","ヒ","フ","ヘ","ホ","マ","ミ","ム","メ","モ","ヤ","ユ","ヨ","ラ","リ","ル","レ","ロ","ワ","ヰ","ヱ","ヲ","ン",
	}, {"ガ","ギ","グ","ゲ","ゴ","ザ","ジ","ズ","ゼ","ゾ","ダ","ヂ","ヅ","デ","ド","バ","ビ","ブ","ベ","ボ","パ","ピ","プ","ペ","ポ",
	}, {"ァ","ィ","ゥ","ェ","ォ","ッ","ャ","ュ","ョ",
	}, {"、","。","ー","～","「","」","【","】","『","』","゛","゜",
	}, {"♪","♭","■","□","◆","◇","△","▲","▽","▼","◎","○","●","◯","‥","…","・","∴","∵","※","×","〒",
	}, {"！","”","＃","＄","％","＆","’","（","）","＝","~","｜","｀","｛","＋","＊","｝","＜","＞","？","＿","－","＾","\\","＠","［","；","：","］","，","．","／",
	}, {"ａ","ｂ","ｃ","ｄ","ｅ","ｆ","ｇ","ｈ","ｉ","ｊ","ｋ","ｌ","ｍ","ｎ","ｏ","ｐ","ｑ","ｒ","ｓ","ｔ","ｕ","ｖ","ｗ","ｘ","ｙ","ｚ",
	}, {"Ａ","Ｂ","Ｃ","Ｄ","Ｅ","Ｆ","Ｇ","Ｈ","Ｉ","Ｊ","Ｋ","Ｌ","Ｍ","Ｎ","Ｏ","Ｐ","Ｑ","Ｒ","Ｓ","Ｔ","Ｕ","Ｖ","Ｗ","Ｘ","Ｙ","Ｚ",
	}, {"０","１","２","３","４","５","６","７","８","９","　"
	}
};
//あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわゐゑをん
//がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ
//ぁぃぅぇぉっゃゅょ
//アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヰヱヲン
//ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポ
//ァィゥェォッャュョ
//、。ー～「」【】『』゛゜
//♪♭■□◆◇△▲▽▼◎○●◯‥…・∴∵※×〒
//！”＃＄％＆’（）＝~｜｀｛＋＊｝＜＞？＿－＾\＠［；：］，．／\
//ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ
//ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ
//０１２３４５６７８９

char g_textlist_half[293][48][3] = {
	{"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", }, {
	"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",}, {
	"1","2","3","4","5","6","7","8","9"," "
	},
};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init_text(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < TEXT_MAX; i++) {
		g_TEXT[i].pos = XMFLOAT2(0.0f, 0.0f);	// 中心点から表示
		g_TEXT[i].w = TEXTURE_WIDTH;
		g_TEXT[i].h = TEXTURE_HEIGHT;
		//48ｘ１２文字だから１で割ってテクスチャの幅を決める
		g_TEXT[i].tw = 1 / 48.0f;
		g_TEXT[i].th = 1 / 12.0f;
		g_TEXT[i].use = FALSE;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit_text(void)
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




// 描画処理
void Draw_text(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	for (int i = 0; i < TEXT_MAX; i++) {
		if (g_TEXT[i].use) {
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);


			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, g_TEXT[i].pos.x, g_TEXT[i].pos.y, g_TEXT[i].w, g_TEXT[i].h, g_TEXT[i].tx, g_TEXT[i].ty, g_TEXT[i].tw, g_TEXT[i].th, g_TEXT[i].color);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			g_TEXT[i].use = FALSE;

		}
	}
}


//テキストの表示(全角のみ)漢字×
void SetText(TEXT text_p, char text[]) {
	text_p.pos.x -= ((strlen(text) - 1) * (text_p.size/2)) / 2;
	for (int i = 0; i < strlen(text); i++) {
		for (int j = 0; j < 293; j++) {
			for (int l = 0; l < 48; l++) {
				//全角文字を判定
				if ((int)text[i] == (int)g_textlist[j][l][0] && (int)text[i + 1] == (int)g_textlist[j][l][1]) {
					for (int z = 0; z < TEXT_MAX; z++) {
						if (!g_TEXT[z].use) {
							g_TEXT[z].color = text_p.color;
							g_TEXT[z].pos = text_p.pos;
							g_TEXT[z].w = text_p.size;
							g_TEXT[z].color = text_p.color;

							g_TEXT[z].h = (text_p.size / 8) * 12;//高さが１２だからsizeを元に計算	
							g_TEXT[z].pos.x += (text_p.size/2) * i;
							g_TEXT[z].use = TRUE;
							g_TEXT[z].tx = g_TEXT[z].tw * l;	// テクスチャの左上X座標
							g_TEXT[z].ty = g_TEXT[z].th * j;	// テクスチャの左上Y座標
							i++;
							break;
						}
					}
				}
			}
		}
	}
}
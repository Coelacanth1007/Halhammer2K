//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "score.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH						(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT						(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX							(6)				// テクスチャの数

#define RESULT_WIDTH						(1725)			// ロゴサイズ
#define RESULT_HEIGHT						(72)			// 

#define RESULT_RETRY_WIDTH					(555)			// 
#define RESULT_RETRY_HEIGHT					(54)			// 

#define RESULT_TITLE_WIDTH					(477)			// 
#define RESULT_TITLE_HEIGHT					(54)			// 

#define RESULT_EXIT_WIDTH					(396)			// 
#define RESULT_EXIT_HEIGHT					(54)			// 

#define RESULT_CUR_WIDTH					(192)			// 
#define RESULT_CUR_HEIGHT					(119)			// 


enum TEX_LIST
{
	TEX_BG,
	TEX_NUB,
	TEX_RETRY,
	TEX_TITLE,
	TEX_EXIT,
	TEX_SKULL,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/result/bg001.png",
	"data/TEXTURE/result/number100x200.png",
	"data/TEXTURE/result/retry_ui.png",
	"data/TEXTURE/result/title_ui.png",
	"data/TEXTURE/result/exit_ui.png",
	"data/TEXTURE/result/skull.png",

};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;
int								resultCur;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

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


	// 変数の初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_RESULT);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{

	if (g_Load == FALSE) return;

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
// 更新処理
//=============================================================================
void UpdateResult(void)
{

	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		resultCur = (++resultCur + RESULT_MAX) % RESULT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_UP))
	{
		resultCur = (--resultCur + RESULT_MAX) % RESULT_MAX;
	}

	switch (resultCur)
	{

	case RESULT_REPLAY:
		g_TexNo = TEX_SKULL;
		g_Pos = XMFLOAT3(670.0f, 250.0f, 0.0f);
		if (GetKeyboardPress(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{

			SetFade(FADE_OUT, MODE_GAME);

		}
		break;

	case RESULT_TITLE:
		g_TexNo = TEX_SKULL;
		g_Pos = XMFLOAT3(670.0f, 350.0f, 0.0f);
		if (GetKeyboardPress(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{

			SetFade(FADE_OUT, MODE_TITLE);

		}
		break;

	case RESULT_EXIT:
		g_TexNo = TEX_SKULL;
		g_Pos = XMFLOAT3(670.0f, 450.0f, 0.0f);
		if (GetKeyboardPress(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{

			GameLoop();
		}

	}


#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
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

	// リザルトの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_BG]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// TEX_RETRYを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_RETRY]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, 250.0f, RESULT_RETRY_WIDTH / 2, RESULT_RETRY_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// TEX_TITLEを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_TITLE]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, 350.0f, RESULT_TITLE_WIDTH / 2, RESULT_TITLE_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// TEX_EXIT背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_EXIT]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, 450.0f, RESULT_EXIT_WIDTH / 2, RESULT_EXIT_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// リザルトのロゴを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_SKULL]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, RESULT_CUR_WIDTH / 2, RESULT_CUR_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);
													
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


	// スコア表示
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_NUB]);

		// 桁数分処理する
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// スコアの位置やテクスチャー座標を反映
			float pw = 16*4;			// スコアの表示幅
			float ph = 32*4;			// スコアの表示高さ
			float px = 600.0f - i*pw;	// スコアの表示位置X
			float py = 100.0f;			// スコアの表示位置Y

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			number /= 10;
		}

	}



}





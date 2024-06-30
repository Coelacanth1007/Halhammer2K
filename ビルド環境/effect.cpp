//=============================================================================
//
// パーティクル処理 [effect.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "light.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(2)			// テクスチャの数

#define	EFFECT_SIZE_X		(20.0f)		// 頂点サイズ
#define	EFFECT_SIZE_Y		(20.0f)		// 頂点サイズ
#define	VALUE_MOVE_EFFECT	(5.0f)		// 移動速度

#define	MAX_EFFECT		(512)		// パーティクル最大数

//#define	DISP_SHADOW						// 影の表示
#undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命
	BOOL			use;			// 使用しているかどうか
	int				type;

} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;		// 頂点バッファ

static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;					// テクスチャ番号

static EFFECT					g_aEffect[MAX_EFFECT];		// パーティクルワーク
static XMFLOAT3					g_posBase;						// ビルボード発生位置
static float					g_fWidthBase = 5.0f;			// 基準の幅
static float					g_fHeightBase = 10.0f;			// 基準の高さ
static float					g_roty = 0.0f;					// 移動方向
static float					g_spd = 0.0f;					// 移動スピード

static char *g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
	"data/TEXTURE/effect001.png",
};

static BOOL							g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	// 頂点情報の作成
	MakeVertexEffect();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// パーティクルワークの初期化
	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aEffect[nCntEffect].material, sizeof(g_aEffect[nCntEffect].material));
		g_aEffect[nCntEffect].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aEffect[nCntEffect].fSizeX = EFFECT_SIZE_X;
		g_aEffect[nCntEffect].fSizeY = EFFECT_SIZE_Y;
		g_aEffect[nCntEffect].nIdxShadow = -1;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].use = FALSE;
	}

	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	//PLAYER *pPlayer = GetPlayer();
	//g_posBase = pPlayer->pos;

	{
		for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
		{
			if(g_aEffect[nCntEffect].use)
			{// 使用中
				switch (g_aEffect[nCntEffect].type)
				{
				case 0 :
					g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
					g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
					g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

					//g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
					//if(g_aEffect[nCntEffect].pos.y <= g_aEffect[nCntEffect].fSizeY / 2)
					//{// 着地した
					//	g_aEffect[nCntEffect].pos.y = g_aEffect[nCntEffect].fSizeY / 2;
					//	g_aEffect[nCntEffect].move.y = -g_aEffect[nCntEffect].move.y * 0.75f;
					//}

					g_aEffect[nCntEffect].move.x += (0.0f - g_aEffect[nCntEffect].move.x) * 0.015f;
					g_aEffect[nCntEffect].move.y += (0.0f - g_aEffect[nCntEffect].move.y) * 0.015f;
					//g_aEffect[nCntEffect].move.y -= 0.25f;
					g_aEffect[nCntEffect].move.z += (0.0f - g_aEffect[nCntEffect].move.z) * 0.015f;

					break;
				case 1 :
					g_aEffect[nCntEffect].scale.x += g_aEffect[nCntEffect].scale.x * 0.01f;
					g_aEffect[nCntEffect].scale.y += g_aEffect[nCntEffect].scale.y * 0.01f;
					g_aEffect[nCntEffect].scale.z += g_aEffect[nCntEffect].scale.z * 0.01f;

					break;
				}

#ifdef DISP_SHADOW
				if(g_aEffect[nCntEffect].nIdxShadow != -1)
				{// 影使用中
					float colA;

					// 影の位置設定
					SetPositionShadow(g_aEffect[nCntEffect].nIdxShadow, XMFLOAT3(g_aEffect[nCntEffect].pos.x, 0.1f, g_aEffect[nCntEffect].pos.z));

					// 影の色の設定
					colA = g_aEffect[nCntEffect].material.Diffuse.w;
					SetColorShadow(g_aEffect[nCntEffect].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
				}
#endif

				g_aEffect[nCntEffect].nLife--;
				if(g_aEffect[nCntEffect].nLife <= 0)
				{
					g_aEffect[nCntEffect].use = FALSE;
					ReleaseShadow(g_aEffect[nCntEffect].nIdxShadow);
					g_aEffect[nCntEffect].nIdxShadow = -1;
				}
				else
				{
					if(g_aEffect[nCntEffect].nLife <= 80)
					{
						g_aEffect[nCntEffect].material.Diffuse.x = 0.0f - (float)(80 - g_aEffect[nCntEffect].nLife) / 80 * 0.8f;
						g_aEffect[nCntEffect].material.Diffuse.y = 0.7f - (float)(80 - g_aEffect[nCntEffect].nLife) / 80 * 0.7f;
						g_aEffect[nCntEffect].material.Diffuse.z = 0.7f - (float)(80 - g_aEffect[nCntEffect].nLife) / 80 * 0.2f;
					}

					if(g_aEffect[nCntEffect].nLife <= 20)
					{
						// α値設定
						g_aEffect[nCntEffect].material.Diffuse.w -= 0.05f;
						if(g_aEffect[nCntEffect].material.Diffuse.w < 0.0f)
						{
							g_aEffect[nCntEffect].material.Diffuse.w = 0.0f;
						}
					}
				}
			}
		}

		// パーティクル発生
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200 ) / 100.0f - g_fWidthBase;
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 300 / 100.0f + g_fHeightBase;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 100 + 150;  

			fSize = (float)(rand() % 30 + 20);

			pos.y = fSize / 2;

			// ビルボードの設定
			//SetEffect(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ライティングを無効に
	SetLightEnable(FALSE);

	// 加算合成に設定
	SetBlendState(BLEND_MODE_ADD);

	// Z比較無し
	SetDepthEnable(FALSE);

	// フォグ無効
	SetFogEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(g_aEffect[nCntEffect].use)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_aEffect[nCntEffect].type]);

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			//// 処理が速いしお勧め
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// スケールを反映
			mtxScl = XMMatrixScaling(g_aEffect[nCntEffect].scale.x, g_aEffect[nCntEffect].scale.y, g_aEffect[nCntEffect].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_aEffect[nCntEffect].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(TRUE);

	// フォグ有効
	SetFogEnable( GetFogEnable() );

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorEffect(int nIdxEffect, XMFLOAT4 col)
{
	g_aEffect[nIdxEffect].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type)
{
	int nIdxEffect = -1;

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(!g_aEffect[nCntEffect].use)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot   = { 0.0f, 0.0f, 0.0f };
			g_aEffect[nCntEffect].scale = { 1.0f, 1.0f, 1.0f };
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].material.Diffuse = col;
			g_aEffect[nCntEffect].fSizeX = fSizeX;
			g_aEffect[nCntEffect].fSizeY = fSizeY;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].use = TRUE;
			g_aEffect[nCntEffect].type = type;
			nIdxEffect = nCntEffect;

#ifdef DISP_SHADOW
			// 影の設定
			g_aEffect[nCntEffect].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
			if(g_aEffect[nCntEffect].nIdxShadow != -1)
			{
				SetColorShadow(g_aEffect[nCntEffect].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxEffect;
}

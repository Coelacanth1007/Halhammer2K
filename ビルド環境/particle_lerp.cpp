//=============================================================================
//
// 線形補間パーティクル処理 [particle_lerp.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "particle_lerp.h"
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数
#define	MAX_PARTICLE		(1028)		// パーティクル最大数
#define	PARTICLE_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_SIZE_Y		(40.0f)		// 頂点サイズ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticleLerp(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer*				g_VertexBuffer = NULL;					// 頂点バッファ
static ID3D11ShaderResourceView*	g_Texture[TEXTURE_MAX] = { NULL };		// テクスチャ情報
static int							g_TexNo;								// テクスチャ番号

static PARTICLE_LERP				g_Particle[MAX_PARTICLE];				// パーティクルワーク

static char* g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
	//"data/TEXTURE/effect001.png",
	//"data/TEXTURE/effect003.png",
};

static BOOL			g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticleLerp(void)
{
	// 頂点情報の作成
	MakeVertexParticleLerp();

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
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		g_Particle[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Particle[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Particle[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Particle[i].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_Particle[i].material, sizeof(g_Particle[i].material));
		g_Particle[i].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_Particle[i].life = 0;
		g_Particle[i].use = FALSE;
	}

	g_Load = TRUE;

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitParticleLerp(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i] != NULL)
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
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
void UpdateParticleLerp(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		// 使用中
		if (g_Particle[i].use == FALSE) continue;

		// 移動処理
		{
			XMVECTOR nPos = XMLoadFloat3(&g_Particle[i].sPos);	// XMVECTORへ変換
			XMVECTOR Pos = XMLoadFloat3(&g_Particle[i].ePos) - nPos;	// 座標を計算している
			Pos *= g_Particle[i].lerpTime;
			// 計算して求めた座標を現在の移動テーブルに足している
			XMStoreFloat3(&g_Particle[i].pos, nPos + Pos);
			g_Particle[i].lerpTime += 1.0f / g_Particle[i].life;
		}

		// 残留時間を超えたらパーティクルを未使用状態にする
		{
			g_Particle[i].life--;
			if (g_Particle[i].life <= 0)
			{
				g_Particle[i].use = FALSE;
			}
			else
			{
				if (g_Particle[i].life <= 80)
				{
					float colorX = g_Particle[i].RGBa.x;
					float colorY = g_Particle[i].RGBa.y;
					float colorZ = g_Particle[i].RGBa.z;
					g_Particle[i].material.Diffuse.x = colorX - (float)(80 - g_Particle[i].life) / 80 * colorX;
					g_Particle[i].material.Diffuse.y = colorY - (float)(80 - g_Particle[i].life) / 80 * colorY;
					g_Particle[i].material.Diffuse.z = colorZ - (float)(80 - g_Particle[i].life) / 80 * colorZ;
				}

				// 残留時間が20フレーム以下の場合、α値を下げていく
				if (g_Particle[i].life <= 20)
				{
					// α値設定
					g_Particle[i].material.Diffuse.w -= 0.05f;
					if (g_Particle[i].material.Diffuse.w < 0.0f)
					{
						g_Particle[i].material.Diffuse.w = 0.0f;
					}
				}
			}
		}

	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawParticleLerp(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	SetLightEnable(FALSE);				// ライティングを無効に
	SetBlendState(BLEND_MODE_ADD);		// 加算合成に設定
	SetDepthEnable(FALSE);				// Z比較無し
	SetFogEnable(FALSE);				// フォグ無効

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_Particle[i].use)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);
			//if (GetViewPortType() == TYPE_LEFT_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[0].mtxView);
			//if (GetViewPortType() == TYPE_RIGHT_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[1].mtxView);
			//if (GetViewPortType() == TYPE_UP_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[0].mtxView);
			//if (GetViewPortType() == TYPE_DOWN_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[1].mtxView);

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			// ビルボード
			// 処理が速いしお勧め
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
			mtxScl = XMMatrixScaling(g_Particle[i].scl.x, g_Particle[i].scl.y, g_Particle[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Particle[i].pos.x, g_Particle[i].pos.y, g_Particle[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_Particle[i].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	SetLightEnable(TRUE);						// ライティングを有効に
	SetBlendState(BLEND_MODE_ALPHABLEND);		// 通常ブレンドに戻す
	SetDepthEnable(TRUE);						// Z比較有効
	SetFogEnable(GetFogEnable());				// フォグ有効
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticleLerp(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//頂点バッファの中身を埋める
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2,  PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3( PARTICLE_SIZE_X / 2,  PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3( PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

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
void SetColorParticleLerp(int index, XMFLOAT4 color)
{
	g_Particle[index].material.Diffuse = color;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetParticleLerp(XMFLOAT3 pos, XMFLOAT3 ePos, XMFLOAT3 scl, XMFLOAT4 col, int life)
{
	int idx = -1;

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_Particle[i].use == FALSE)
		{
			g_Particle[i].pos = pos;
			g_Particle[i].scl = scl;
			g_Particle[i].material.Diffuse = col;
			g_Particle[i].life = life;
			g_Particle[i].RGBa = col;
			g_Particle[i].sPos = pos;
			g_Particle[i].ePos = ePos;
			g_Particle[i].lerpTime = 0.0f;
			g_Particle[i].use = TRUE;

			idx = i;

			break;
		}
	}

	return idx;
}


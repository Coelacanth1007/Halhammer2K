//=============================================================================
//
// 空処理 [sky.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "sky.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_SKY		"data/MODEL/sky.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define SKY_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define SKY_OFFSET_Y		(7.0f)							// プレイヤーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SKY				g_Sky;						// プレイヤー

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSky(void)
{
	LoadModel(MODEL_SKY, &g_Sky.model);
	g_Sky.load = true;

	g_Sky.pos = { 0.0f, 0.0f, 0.0f };
	g_Sky.rot = { 0.0f, 0.0f, 0.0f };
	g_Sky.scl = { 5.0f, 5.0f, 5.0f };

	g_Sky.spd  = 0.0f;			// 移動スピードクリア
	g_Sky.size = SKY_SIZE;	// 当たり判定の大きさ

	g_Sky.use = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSky(void)
{
	// モデルの解放処理
	if (g_Sky.load)
	{
		UnloadModel(&g_Sky.model);
		g_Sky.load = false;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSky(void)
{
	CAMERA *cam = GetCamera();

	g_Sky.rot.y += 0.001f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSky(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Sky.scl.x, g_Sky.scl.y, g_Sky.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Sky.rot.x, g_Sky.rot.y + XM_PI, g_Sky.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Sky.pos.x, g_Sky.pos.y, g_Sky.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Sky.mtxWorld, mtxWorld);


	// モデル描画
	DrawModel(&g_Sky.model);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_NONE);

	SetLightEnable(TRUE);

}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
SKY *GetSky(void)
{
	return &g_Sky;
}


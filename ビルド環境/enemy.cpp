//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "orc.h"
#include "shadow.h"
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/drone.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる

#define COUNT_MAX			(200)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー

int g_Enemy_load = 0;

BOOL					g_load;
DX11_MODEL				g_model;				// モデル情報
XMFLOAT4				g_diffuse[MODEL_MAX_MATERIAL];	// モデルの色



static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame

	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(400.0f, ENEMY_OFFSET_Y + 20.0f,  0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },
	{ XMFLOAT3(400.0f, ENEMY_OFFSET_Y,  400.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },

	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y + 20.0f,400.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },
	{ XMFLOAT3(-400.0f, ENEMY_OFFSET_Y,  400.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },

	{ XMFLOAT3(-400.0f, ENEMY_OFFSET_Y,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },
	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y + 20.0f,400.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
};




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{

	LoadModel(MODEL_ENEMY, &g_model);
	g_load = TRUE;

	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_model, &g_diffuse[0]);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 7.0f, 20.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].spd  = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		
		g_Enemy[i].time = 0.1f * i;			// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);			// 再生する行動データテーブルのレコード数をセット

		g_Enemy[i].use = TRUE;			// TRUE:生きてる

		g_Enemy[i].moveCnt = 0;

	}

	 srand((unsigned int)time(NULL));

	//// エネミーが線形補間で動かしてみる
	//g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_load==TRUE)
	{
		UnloadModel(&g_model);
		g_load = FALSE;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{


	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes

			float speedTime = 0.5f;		// エネミーの移動倍率

			XMFLOAT3 oldPos = g_Enemy[i].pos;
			// droneのランダム移動処理
			g_Enemy[i].pos.x -= sinf(g_Enemy[i].rot.y) * speedTime;
			g_Enemy[i].pos.z += cosf(g_Enemy[i].rot.y) * speedTime;


			g_Enemy[i].moveCnt++;

			if (g_Enemy[i].moveCnt >= COUNT_MAX)
			{
				g_Enemy[i].rot.y = rand() % (int)(XM_2PI * 100) * 0.01f - XM_PI;
				g_Enemy[i].moveCnt = 0;
			}

			// 壁に当たったら地図の中心に戻る
			if (g_Enemy[i].pos.x < MAP_LEFT || g_Enemy[i].pos.x > MAP_RIGHT ||
				g_Enemy[i].pos.z < MAP_DOWN || g_Enemy[i].pos.z > MAP_TOP)
			{
				g_Enemy[i].pos = oldPos;

				g_Enemy[i].rot.y += XM_PI;
			}

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}




#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// モデルの色を変更できるよ！半透明にもできるよ。
		for (int j = 0; j < g_model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_model, j, g_diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}

void CreateEnemy(void)
{
	ORC* orc = GetOrc();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE) continue;

		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		switch (i)
		{
		case 0:
			pos = XMFLOAT3(orc->pos.x - 50.0f, ENEMY_OFFSET_Y, orc->pos.z - 50.0f);

			break;
		case 1:
			pos = XMFLOAT3(orc->pos.x - 100.0f, ENEMY_OFFSET_Y, orc->pos.z - 100.0f);
			break;

		case 2:
			pos = XMFLOAT3(orc->pos.x + 50.0f, ENEMY_OFFSET_Y, orc->pos.z + 50.0f);
			break;

		case 3:
			pos = XMFLOAT3(orc->pos.x + 100.0f, ENEMY_OFFSET_Y, orc->pos.z + 100.0f);
			break;

		case 4:
			pos = XMFLOAT3(orc->pos.x , ENEMY_OFFSET_Y, orc->pos.z + 100.0f);
			break;

		}

		g_Enemy[i].moveCnt = 0;
		g_Enemy[i].pos = pos;
		g_Enemy[i].shadowIdx = CreateShadow(g_Enemy[i].pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		g_Enemy[i].use = TRUE;
	}

}


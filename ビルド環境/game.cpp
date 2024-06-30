//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "orc.h"
#include "enemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "bullet.h"
#include "score.h"
#include "particle.h"
#include "collision.h"
#include "debugproc.h"
#include "sky.h"
#include "particle_lerp.h"
#include "ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 50, 50, 100.0f, 100.0f);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化

	InitPlayer();

	// オークの初期化
	InitOrc();

	// エネミーの初期化
	InitEnemy();

	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// 空の初期化
	InitSky();

	// 木を生やす
	InitTree();

	// 弾の初期化
	InitBullet();

	// スコアの初期化
	InitScore();

	// パーティクルの初期化
	InitParticle();
	InitParticleLerp();

	InitUi();

	// BGM再生
	//PlaySound(SOUND_LABEL_BGM_sample001);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// パーティクルの終了処理
	UninitParticleLerp();
	UninitParticle();

	// スコアの終了処理
	UninitScore();

	// 弾の終了処理
	UninitBullet();

	// 木の終了処理
	UninitTree();

	// 空の終了処理
	UninitSky();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	// エネミーの終了処理
	UninitEnemy();

	// オークの終了処理
	UninitOrc();

	// プレイヤーの終了処理
	UninitPlayer();

	UninitUi();

	// 影の終了処理
	UninitShadow();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	// 地面処理の更新
	UpdateMeshField();

	// プレイヤーの更新処理
	UpdatePlayer();

	// オークの更新処理
	UpdateOrc();

	// エネミーの更新処理
	UpdateEnemy();

	// 壁処理の更新
	UpdateMeshWall();

	// 空の更新処理
	UpdateSky();

	// 木の更新処理
	UpdateTree();

	// 弾の更新処理
	UpdateBullet();

	// パーティクルの更新処理
	UpdateParticle();
	UpdateParticleLerp();

	// 影の更新処理
	UpdateShadow();

	// 当たり判定処理
	CheckHit();

	// スコアの更新処理
	UpdateScore();

	UpdateUi();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame0(void)
{
	// 3Dの物を描画する処理
	
	// 空の描画処理
	DrawSky();

	// 地面の描画処理
	DrawMeshField();

	// 影の描画処理
	DrawShadow();

	// エネミーの描画処理
	DrawEnemy();

	// エネミーの描画処理
	DrawOrc();

	// プレイヤーの描画処理
	DrawPlayer();

	// 弾の描画処理
	DrawBullet();

	// 壁の描画処理
	DrawMeshWall();

	// 木の描画処理
	DrawTree();

	// パーティクルの描画処理
	DrawParticle();
	DrawParticleLerp();


	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// スコアの描画処理
	DrawScore();

	DrawUi();

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// プレイヤー視点
	pos = GetPlayer()->pos;
	//pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY* enemy		 = GetEnemy();		// エネミーのポインターを初期化
	ORC* orc			 = GetOrc();		// エネミーのポインターを初期化
	PLAYER* player		 = GetPlayer();	// プレイヤーのポインターを初期化
	BULLET* bullet		 = GetBullet();	// 弾のポインターを初期化
	PARTICLE* particle	 = GetParticle(); //パーティクルのポインターを初期化

	int enemy_count = 0;
	int orc_count = 0;

	int time = 0;
	// 敵とパーティクル
	for (int i = 0; i < MAX_ENEMY; i++)
	{

		// プレイヤーが死んでたら次の奴へ
		if (enemy[i].use == FALSE) continue;

		enemy_count++;

		for (int j = 0; j < MAX_PARTICLE; j++)
		{
			//敵が死んでたら次の奴へ
			if (particle[j].use == FALSE) continue;

			//BCの当たり判定
			if (CollisionBC(enemy[i].pos, particle[j].pos, enemy[i].size, 10.0f))
			{

				// 敵キャラクターは倒される
				enemy[i].use = FALSE;
				ReleaseShadow(enemy[i].shadowIdx);



				// スコアを足す
				AddScore(100);

			}
		}
	}

	// エネミーが全滅したらオークが攻撃られれる状態に入る

	if (enemy_count == 0)
	{
		orc[0].attackable = TRUE;
	}


	// オークとパーティクル
	for (int i = 0; i < MAX_ORC; i++)
	{

		if (orc[0].use == FALSE) continue;

		orc_count++;

		if (orc[0].attackable == TRUE)
		{

			for (int j = 0; j < MAX_PARTICLE; j++)
			{
				//敵が死んでたら次の奴へ
				if (particle[j].use == FALSE) continue;

				//BCの当たり判定
				if (CollisionBC(orc[i].pos, particle[j].pos, orc[i].size, 10.0f))
				{

					orc[i].hp--;

					if (orc[i].hp <= 0)
					{
						// 敵キャラクターは倒される
						orc[i].use = FALSE;
						ReleaseShadow(orc[i].shadowIdx);

						// スコアを足す
						AddScore(100);
					}

				}

			}

		}

	}

	// オークが死んだらリザルト画面に遷移
		if (orc_count <= 0)
		{
			//time++;
			//if (time >= 60)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}
		}



		//BCの当たり判定
		if (CollisionBC(orc[0].pos, player[0].pos, orc[0].size, 10.0f) )
		{
			if (player[0].inviCnt == 0)
			{
				player[0].inviCnt = INVI_TIME;

				player[0].hp -= 1;


				if (player[0].hp <= 0)
				{

					// プレイヤーが倒れるとき
					player[0].use = FALSE;
					ReleaseShadow(player[0].shadowIdx);

					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
		}
		


}



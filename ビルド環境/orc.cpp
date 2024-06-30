//=============================================================================
//
// 敵BOSS処理 [player.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "orc.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "keyframe.h"
#include "fade.h"
#include "enemy.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ORC		"data/MODEL/orc/orc_body.obj"	// 読み込むモデル名
#define	MODEL_ORC_HEAD	"data/MODEL/orc/orc_head.obj"	// 読み込むモデル名
#define	MODEL_ORC_LARM	"data/MODEL/orc/orc_arml.obj"	// 読み込むモデル名
#define	MODEL_ORC_RARM	"data/MODEL/orc/orc_armr.obj"	// 読み込むモデル名
#define	MODEL_ORC_LLEG	"data/MODEL/orc/orc_legl.obj"	// 読み込むモデル名
#define	MODEL_ORC_RLEG	"data/MODEL/orc/orc_legr.obj"	// 読み込むモデル名
#define	MODEL_ORC_LFOOT	"data/MODEL/orc/orc_footl.obj"	// 読み込むモデル名
#define	MODEL_ORC_RFOOT	"data/MODEL/orc/orc_footr.obj"	// 読み込むモデル名


// アニメイションのディレクトリ

// 待機状態
#define ANIM_IDLE_BODY			"data/ANIMDATA/orc/idle/orc_body1.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_HEAD			"data/ANIMDATA/orc/idle/orc_head1.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_LARM			"data/ANIMDATA/orc/idle/orc_handl.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_RARM			"data/ANIMDATA/orc/idle/orc_handr.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_LLEG			"data/ANIMDATA/orc/idle/orc_legl.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_RLEG			"data/ANIMDATA/orc/idle/orc_legr1.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_LFOOT			"data/ANIMDATA/orc/idle/orc_footl.csv"			// 読み込みキーフレーム
#define ANIM_IDLE_RFOOT			"data/ANIMDATA/orc/idle/orc_footr1.csv"			// 読み込みキーフレーム

// 移動状態
#define ANIM_MOVE_BODY			"data/ANIMDATA/orc/walk/walk_body.csv"			// 読み込みキーフレーム
#define ANIM_MOVE_HEAD			"data/ANIMDATA/orc/walk/walk_head.csv"			// 読み込みキーフレーム
#define ANIM_MOVE_LARM			"data/ANIMDATA/orc/walk/walk_arm_l.csv"			// 読み込みキーフレーム
#define ANIM_MOVE_RARM			"data/ANIMDATA/orc/walk/walk_arm_r.csv"			// 読み込みキーフレーム
#define ANIM_MOVE_LLEG			"data/ANIMDATA/orc/walk/walk_leg_l.csv"			// 読み込みキーフレーム
#define ANIM_MOVE_RLEG			"data/ANIMDATA/orc/walk/walk_leg_r.csv"			// 読み込みキーフレーム
#define ANIM_MOVE_LFOOT			"data/ANIMDATA/orc/walk/walk_foot_l.csv"		// 読み込みキーフレーム
#define ANIM_MOVE_RFOOT			"data/ANIMDATA/orc/walk/walk_foot_r.csv"		// 読み込みキーフレーム

// 攻撃状態
#define ANIM_ATTACK_BODY		"data/ANIMDATA/orc/attack/attack_body.csv"		// 読み込みキーフレーム
#define ANIM_ATTACK_HEAD		"data/ANIMDATA/orc/attack/attack_head.csv"		// 読み込みキーフレーム
#define ANIM_ATTACK_LARM		"data/ANIMDATA/orc/attack/attack_arm_l.csv"		// 読み込みキーフレーム
#define ANIM_ATTACK_RARM		"data/ANIMDATA/orc/attack/attack_arm_r.csv"		// 読み込みキーフレーム
#define ANIM_ATTACK_LLEG		"data/ANIMDATA/orc/attack/attack_leg_l.csv"		// 読み込みキーフレーム
#define ANIM_ATTACK_RLEG		"data/ANIMDATA/orc/attack/attack_leg_r.csv"		// 読み込みキーフレーム
#define ANIM_ATTACK_LFOOT		"data/ANIMDATA/orc/attack/attack_foot_l.csv"	// 読み込みキーフレーム
#define ANIM_ATTACK_RFOOT		"data/ANIMDATA/orc/attack/attack_foot_r.csv"	// 読み込みキーフレーム

#define	VALUE_MOVE			(1.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define ORC_SHADOW_SIZE	(2.0f)							// 影の大きさ
#define ORC_OFFSET_Y		(34.0f)							// プレイヤーの足元をあわせる

#define ORC_PARTS_MAX	(9)								// プレイヤーのパーツの数


#define ORC_HP			(1000)
// アニメーションのリスト
enum ANIM_LIST
{	

	IDLE_BODY,
	IDLE_HEAD,
	IDLE_LARM,
	IDLE_RARM,
	IDLE_LLEG,
	IDLE_RLEG,
	IDLE_LFOOT,
	IDLE_RFOOT,

	MOVE_BODY,
	MOVE_HEAD,
	MOVE_LARM,
	MOVE_RARM,
	MOVE_LLEG,
	MOVE_RLEG,
	MOVE_LFOOT,
	MOVE_RFOOT,

	ATK_BODY,
	ATK_HEAD,
	ATK_LARM,
	ATK_RARM,
	ATK_LLEG,
	ATK_RLEG,
	ATK_LFOOT,
	ATK_RFOOT,

	ANIM_MAX,
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ORC		g_Orc;						// プレイヤー

static ORC		g_Parts[ORC_PARTS_MAX];		// プレイヤーのパーツ用

// アニメーションのディレクトリの配列
static char*g_AnimDir[]=
{
	ANIM_IDLE_BODY,
	ANIM_IDLE_HEAD,
	ANIM_IDLE_LARM,
	ANIM_IDLE_RARM,
	ANIM_IDLE_LLEG,
	ANIM_IDLE_RLEG,
	ANIM_IDLE_LFOOT,
	ANIM_IDLE_RFOOT,

	ANIM_MOVE_BODY,
	ANIM_MOVE_HEAD,
	ANIM_MOVE_LARM,
	ANIM_MOVE_RARM,
	ANIM_MOVE_LLEG,
	ANIM_MOVE_RLEG,
	ANIM_MOVE_LFOOT,
	ANIM_MOVE_RFOOT,

	ANIM_ATTACK_BODY,
	ANIM_ATTACK_HEAD,
	ANIM_ATTACK_LARM,
	ANIM_ATTACK_RARM,
	ANIM_ATTACK_LLEG,
	ANIM_ATTACK_RLEG,
	ANIM_ATTACK_LFOOT,
	ANIM_ATTACK_RFOOT,

};

// プレイヤーの階層アニメーションデータ
// 
static std::vector<INTERPOLATION_DATA> orc_body;
static std::vector<INTERPOLATION_DATA> orc_head;
static std::vector<INTERPOLATION_DATA> orc_hand;
static std::vector<INTERPOLATION_DATA> orc_handr;
static std::vector<INTERPOLATION_DATA> idle_legl;
static std::vector<INTERPOLATION_DATA> idle_legr;
static std::vector<INTERPOLATION_DATA> orc_footl;
static std::vector<INTERPOLATION_DATA> orc_footr;

static std::vector<INTERPOLATION_DATA> walk_body;
static std::vector<INTERPOLATION_DATA> walk_head;
static std::vector<INTERPOLATION_DATA> walk_arm_l;
static std::vector<INTERPOLATION_DATA> walk_arm_r;
static std::vector<INTERPOLATION_DATA> walk_leg_l;
static std::vector<INTERPOLATION_DATA> walk_leg_r;
static std::vector<INTERPOLATION_DATA> walk_foot_l;
static std::vector<INTERPOLATION_DATA> walk_foot_r;

static std::vector<INTERPOLATION_DATA> attack_body;
static std::vector<INTERPOLATION_DATA> attack_head;
static std::vector<INTERPOLATION_DATA> attack_arm_l;
static std::vector<INTERPOLATION_DATA> attack_arm_r;
static std::vector<INTERPOLATION_DATA> attack_leg_l;
static std::vector<INTERPOLATION_DATA> attack_leg_r;
static std::vector<INTERPOLATION_DATA> attack_foot_l;
static std::vector<INTERPOLATION_DATA> attack_foot_r;

static std::vector<INTERPOLATION_DATA>* g_TblAdr[] =
{
	&orc_body,
	&orc_head,
	&orc_hand,
	&orc_handr,
	&idle_legl ,
	&idle_legr ,
	&orc_footl,
	&orc_footr,

	&walk_body,
	&walk_head,
	&walk_arm_l,
	&walk_arm_r,
	&walk_leg_l,
	&walk_leg_r,
	&walk_foot_l,
	&walk_foot_r,

	&attack_body,
	&attack_head,
	&attack_arm_l,
	&attack_arm_r,
	&attack_leg_l,
	&attack_leg_r,
	&attack_foot_l,
	&attack_foot_r,
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitOrc(void)
{
	// アニメーションのSCVファイルから
	for (int i = 0; i < ANIM_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}
	//LoadModel(MODEL_ORC, &g_Orc.model);
	g_Orc.load = TRUE;

	g_Orc.pos = { 100.0f, ORC_OFFSET_Y, 200.0f };
	g_Orc.rot = { 0.0f, 0.0f, 0.0f };
	g_Orc.scl = { 1.0f, 1.0f, 1.0f };

	g_Orc.spd = 0.0f;			// 移動スピードクリア
	g_Orc.size = ORC_SIZE;	// 当たり判定の大きさ

	g_Orc.hp = ORC_HP;

	g_Orc.use = TRUE;

	g_Orc.moving = FALSE;

	g_Orc.attack = FALSE;

	g_Orc.attackable = FALSE;

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Orc.pos;
	pos.y -= (ORC_OFFSET_Y - 0.1f);
	g_Orc.shadowIdx = CreateShadow(pos, ORC_SHADOW_SIZE, ORC_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号




	// 階層アニメーション用の初期化処理
	g_Orc.parent = NULL;			// 本体（親）なのでNULLを入れる

	// パーツの初期化
	for (int i = 0; i < ORC_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(0.0f, 0.0f, 0.0f);

		// 親子関係
		g_Parts[i].parent = &g_Orc;		// ← ここに親のアドレスを入れる
	//	g_Parts[腕].parent= &g_Orc;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = 0;
	}

	g_Parts[0].use = TRUE;
	//g_Parts[0].parent = &g_Orc;	// 親をセット
	g_Parts[0].tblNo = 0;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0].tblMax = (int)g_TblAdr[g_Parts[0].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[0].load = 1;
	LoadModel(MODEL_ORC, &g_Parts[0].model);

	g_Parts[1].use = TRUE;
	//g_Parts[1].parent = &g_Parts[0];	// 親をセット
	g_Parts[1].tblNo = 1;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tblMax = (int)g_TblAdr[g_Parts[1].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[1].load = 1;
	LoadModel(MODEL_ORC_HEAD, &g_Parts[1].model);

	g_Parts[2].use = TRUE;
	//g_Parts[2].parent = &g_Parts[0];	// 親をセット
	g_Parts[2].tblNo = 2;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[2].tblMax = (int)g_TblAdr[g_Parts[2].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[2].load = 1;
	LoadModel(MODEL_ORC_LARM, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	//g_Parts[3].parent = &g_Parts[0];	// 親をセット
	g_Parts[3].tblNo = 3;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[3].tblMax = (int)g_TblAdr[g_Parts[3].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[3].load = 1;
	LoadModel(MODEL_ORC_RARM, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	//g_Parts[4].parent = &g_Parts[0];	// 親をセット
	g_Parts[4].tblNo = 4;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[4].tblMax = (int)g_TblAdr[g_Parts[4].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[4].load = 1;
	LoadModel(MODEL_ORC_LLEG, &g_Parts[4].model);

	g_Parts[5].use = TRUE;
	//g_Parts[5].parent = &g_Parts[0];	// 親をセット
	g_Parts[5].tblNo = 5;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[5].tblMax = (int)g_TblAdr[g_Parts[5].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[5].load = 1;
	LoadModel(MODEL_ORC_RLEG, &g_Parts[5].model);
	
	g_Parts[6].use = TRUE;
	//g_Parts[6].parent = &g_Parts[0];	// 親をセット
	g_Parts[6].tblNo = 6;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[6].tblMax = (int)g_TblAdr[g_Parts[6].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[6].load = 1;
	LoadModel(MODEL_ORC_LFOOT, &g_Parts[6].model);
	
	g_Parts[7].use = TRUE;
	//g_Parts[7].parent = &g_Parts[0];	// 親をセット
	g_Parts[7].tblNo = 7;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[7].tblMax = (int)g_TblAdr[g_Parts[7].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[7].load = 1;
	LoadModel(MODEL_ORC_RFOOT, &g_Parts[7].model);
		
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOrc(void)
{
	// モデルの解放処理
	if (g_Orc.load)
	{
		UnloadModel(&g_Orc.model);
		g_Orc.load = FALSE;
	}

	// パーツの解放処理
	for (int i = 0; i < ORC_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// パーツの解放処理
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOrc(void)
{
	CAMERA *cam = GetCamera();


	PLAYER* player = GetPlayer();

	// フォーミング処理
	if (player->use == TRUE)
	{
		XMVECTOR epos = XMLoadFloat3(&g_Orc.pos);
		XMVECTOR vec = XMLoadFloat3(&player[0].pos) - epos;		//自分とプレイヤーとの差分を求めている
		float angle = atan2f(vec.m128_f32[2], vec.m128_f32[0]);	//その差分を使って角度を求めている
		float speed = 0.3f;										//スピードはちょっと遅くしてみる

		g_Orc.pos.x += cosf(angle) * speed;				//angleの方向へ移動
		g_Orc.pos.z += sinf(angle) * speed;				//angleの方向へ移動

		g_Orc.rot.y = -(angle + (XM_PI * 2) / 4);
		
		g_Orc.moving = TRUE;

	}
	
	if (g_Orc.attack == FALSE) g_Orc.moving == TRUE ? g_Orc.state = ORC_STATE_MOVE : g_Orc.state = ORC_STATE_IDLE;


	// attack距離判定
	XMFLOAT3 end = player[0].pos;
	XMFLOAT3 start = g_Orc.pos;
	XMVECTOR distV = XMVector3Length(XMVectorSubtract(XMLoadFloat3(&start), XMLoadFloat3(&end)));
	float distance;
	XMStoreFloat(&distance, distV);
	if(distance <= 80.0f)
	{
		g_Orc.state = ORC_STATE_ATK;
	}

	switch (g_Orc.state)
	{
	case ORC_STATE_IDLE:
		SetEnmTblIdle();
		break;

	case ORC_STATE_MOVE:
		SetEnmTblMove();
		break;

	case ORC_STATE_ATK:
		SetEnmTblAtk();
		AttackOrc();
		break;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Orc.pos.z = g_Orc.pos.x = 0.0f;
		g_Orc.rot.y = g_Orc.dir = 0.0f;
		g_Orc.spd = 0.0f;
	}
#endif

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Orc.pos;
	pos.y -= (ORC_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Orc.shadowIdx, pos);

	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		AttackOrc();
		g_Orc.state = ORC_STATE_ATK;
	}

	g_Orc.spd *= 0.5f;

	// 階層アニメーション
	for (int i = 0; i < ORC_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	// 線形補間の処理
			int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			//INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得
			std::vector<INTERPOLATION_DATA>tbl = *g_TblAdr[g_Parts[i].tblNo];

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frameを使て時間経過処理をする
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
			if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Parts[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる

				switch (g_Orc.setTbl)
				{
				case ORC_STATE_ATK:
					//g_Orc.tblMax = -1;
					g_Parts[i].tblMax = -1;
					g_Orc.setTbl = ORC_STATE_IDLE;
					break;
				}
			}
		}

	}

	if (g_Orc.hp <= ORC_HP/2 && g_Orc.hp >= ORC_HP/2-10)
	{
		g_Orc.attackable = FALSE;

		CreateEnemy();

		g_Orc.hp -= 30;

	}


	{	// ポイントライトのテスト
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Orc.pos;
		pos.y += 20.0f;
		pos.z -= 50.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}


	if (g_Orc.state != ORC_STATE_ATK)
	{
		g_Orc.state = ORC_STATE_IDLE;
	}

	XMFLOAT3 pMove = { 0.0f,0.0f, 0.0f};

	pMove.y += rand() % 300 / 100.0f ;

	if (g_Orc.attackable == FALSE)
	{
		SetOrcParticle(g_Orc.pos, pMove, XMFLOAT4(1.0f, 0.7f, 0.2f, 0.3f), 5.0f, 5.0f, 20);
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Orc:X:%f Y:%f Z:%f\n", g_Orc.pos.x, g_Orc.pos.y, g_Orc.pos.z);
	PrintDebugProc("Orc:hp:%d\n", g_Orc.hp);

#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOrc(void)
{

	if (g_Orc.use == FALSE) return;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// まずは基本となるＢＯＤＹを描画する

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Orc.scl.x, g_Orc.scl.y, g_Orc.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Orc.rot.x, g_Orc.rot.y + XM_PI, g_Orc.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Orc.pos.x, g_Orc.pos.y, g_Orc.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	// 自分を描画したときにしようしたマトリクスを保存しておく
	XMStoreFloat4x4(&g_Orc.mtxWorld, mtxWorld);


	//// モデル描画
	//DrawModel(&g_Orc.model);



	// パーツの階層アニメーション
	for (int i = 0; i < ORC_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ↑
			// g_Orc.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// モデル描画
		DrawModel(&g_Parts[i].model);

	}


	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
ORC *GetOrc(void)
{
	return &g_Orc;
}


void AttackOrc(void)
{

	// 状態遷移
	{
		BOOL ans = TRUE;

		// 全てのパーツの最大テーブルが-1になっている場合(≒アニメーションが終了している場合)、状態遷移させる
		for (int i = 0; i < ORC_PARTS_MAX; i++)
		{
			if (g_Parts[i].tblMax != -1) ans = FALSE;

		}

		if (ans == TRUE) g_Orc.state = ORC_STATE_IDLE;
	}
}

void SetEnmTblIdle(void)
{
	switch (g_Orc.setTbl)
	{
	case ORC_STATE_IDLE:
		break;
	default:
		for (int i = 0; i < ORC_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + IDLE_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Orc.setTbl = ORC_STATE_IDLE;
		break;
	}
}

void SetEnmTblMove(void)
{
	switch (g_Orc.setTbl)
	{
	case ORC_STATE_MOVE:
		break;
	default:
		for (int i = 0; i < ORC_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + MOVE_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Orc.setTbl = ORC_STATE_MOVE;
		break;
	}
}


void SetEnmTblAtk(void)
{
	switch (g_Orc.setTbl)
	{
	case ORC_STATE_ATK:
		break;
	default:
		for (int i = 0; i < ORC_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + ATK_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Orc.setTbl = ORC_STATE_ATK;
		break;
	}

}



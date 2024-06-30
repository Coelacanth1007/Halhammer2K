//=============================================================================
//
// モデル処理 [player.cpp]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "particle.h"
#include "particle_lerp.h"
#include "enemy.h"
#include "collision.h"
#include "keyframe.h"
#include "effect.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/marine/marine_body.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_HEAD	"data/MODEL/marine/marine_head.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_LARM	"data/MODEL/marine/marine_arm_l.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_RARM	"data/MODEL/marine/marine_arm_r.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_LHAND	"data/MODEL/marine/marine_hand_l.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_RHAND	"data/MODEL/marine/marine_hand_r.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_LLEG	"data/MODEL/marine/marine_leg_l.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_RLEG	"data/MODEL/marine/marine_leg_r.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_LFOOT	"data/MODEL/marine/marine_foot_l.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_RFOOT	"data/MODEL/marine/marine_foot_r.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_GUN	"data/MODEL/marine/plasmagun.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_BAG01	"data/MODEL/marine/bag01.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_BAG02	"data/MODEL/marine/bag02.obj"			// 読み込むモデル名

// アニメイションのディレクトリ
// 
// 待機状態
#define ANIM_IDLE_BODY			"data/ANIMDATA/marine/idle/marine_body.csv"
#define ANIM_IDLE_HEAD			"data/ANIMDATA/marine/idle/marine_head.csv"
#define ANIM_IDLE_LARM			"data/ANIMDATA/marine/idle/marine_arm_l.csv"
#define ANIM_IDLE_RARM			"data/ANIMDATA/marine/idle/marine_arm_r.csv"
#define ANIM_IDLE_LHAND			"data/ANIMDATA/marine/idle/marine_hand_l.csv"
#define ANIM_IDLE_RHAND			"data/ANIMDATA/marine/idle/marine_hand_r.csv"
#define ANIM_IDLE_LLEG			"data/ANIMDATA/marine/idle/marine_leg_l.csv"
#define ANIM_IDLE_RLEG			"data/ANIMDATA/marine/idle/marine_leg_r.csv"
#define ANIM_IDLE_LFOOT			"data/ANIMDATA/marine/idle/marine_foot_l.csv"
#define ANIM_IDLE_RFOOT			"data/ANIMDATA/marine/idle/marine_foot_r.csv"
#define ANIM_IDLE_GUN			"data/ANIMDATA/marine/idle/marine_plasmagun.csv"
#define ANIM_IDLE_BAG01			"data/ANIMDATA/marine/idle/marine_bag01.csv"
#define ANIM_IDLE_BAG02			"data/ANIMDATA/marine/idle/marine_bag02.csv"

// 移動状態
#define ANIM_MOVE_BODY			"data/ANIMDATA/marine/move/marine_body.csv"
#define ANIM_MOVE_HEAD			"data/ANIMDATA/marine/move/marine_head.csv"
#define ANIM_MOVE_LARM			"data/ANIMDATA/marine/move/marine_arm_l.csv"
#define ANIM_MOVE_RARM			"data/ANIMDATA/marine/move/marine_arm_r.csv"
#define ANIM_MOVE_LHAND			"data/ANIMDATA/marine/move/marine_hand_l.csv"
#define ANIM_MOVE_RHAND			"data/ANIMDATA/marine/move/marine_hand_r.csv"
#define ANIM_MOVE_LLEG			"data/ANIMDATA/marine/move/marine_leg_l.csv"
#define ANIM_MOVE_RLEG			"data/ANIMDATA/marine/move/marine_leg_r.csv"
#define ANIM_MOVE_LFOOT			"data/ANIMDATA/marine/move/marine_foot_l.csv"
#define ANIM_MOVE_RFOOT			"data/ANIMDATA/marine/move/marine_foot_r.csv"
#define ANIM_MOVE_GUN			"data/ANIMDATA/marine/move/marine_plasmagun.csv"
#define ANIM_MOVE_BAG01			"data/ANIMDATA/marine/move/marine_bag01.csv"
#define ANIM_MOVE_BAG02			"data/ANIMDATA/marine/move/marine_bag02.csv"

// 移動状態
#define ANIM_ATK_BODY			"data/ANIMDATA/marine/attack/marine_body.csv"
#define ANIM_ATK_HEAD			"data/ANIMDATA/marine/attack/marine_head.csv"
#define ANIM_ATK_LARM			"data/ANIMDATA/marine/attack/marine_arm_l.csv"
#define ANIM_ATK_RARM			"data/ANIMDATA/marine/attack/marine_arm_r.csv"
#define ANIM_ATK_LHAND			"data/ANIMDATA/marine/attack/marine_hand_l.csv"
#define ANIM_ATK_RHAND			"data/ANIMDATA/marine/attack/marine_hand_r.csv"
#define ANIM_ATK_LLEG			"data/ANIMDATA/marine/attack/marine_leg_l.csv"
#define ANIM_ATK_RLEG			"data/ANIMDATA/marine/attack/marine_leg_r.csv"
#define ANIM_ATK_LFOOT			"data/ANIMDATA/marine/attack/marine_foot_l.csv"
#define ANIM_ATK_RFOOT			"data/ANIMDATA/marine/attack/marine_foot_r.csv"
#define ANIM_ATK_GUN			"data/ANIMDATA/marine/attack/marine_plasmagun.csv"
#define ANIM_ATK_BAG01			"data/ANIMDATA/marine/attack/marine_bag01.csv"
#define ANIM_ATK_BAG02			"data/ANIMDATA/marine/attack/marine_bag02.csv"

// ジャンプ状態
#define ANIM_JUMP_BODY			"data/ANIMDATA/marine/jump/marine_body.csv"
#define ANIM_JUMP_HEAD			"data/ANIMDATA/marine/jump/marine_head.csv"
#define ANIM_JUMP_LARM			"data/ANIMDATA/marine/jump/marine_arm_l.csv"
#define ANIM_JUMP_RARM			"data/ANIMDATA/marine/jump/marine_arm_r.csv"
#define ANIM_JUMP_LHAND			"data/ANIMDATA/marine/jump/marine_hand_l.csv"
#define ANIM_JUMP_RHAND			"data/ANIMDATA/marine/jump/marine_hand_r.csv"
#define ANIM_JUMP_LLEG			"data/ANIMDATA/marine/jump/marine_leg_l.csv"
#define ANIM_JUMP_RLEG			"data/ANIMDATA/marine/jump/marine_leg_r.csv"
#define ANIM_JUMP_LFOOT			"data/ANIMDATA/marine/jump/marine_foot_l.csv"
#define ANIM_JUMP_RFOOT			"data/ANIMDATA/marine/jump/marine_foot_r.csv"
#define ANIM_JUMP_GUN			"data/ANIMDATA/marine/jump/marine_plasmagun.csv"
#define ANIM_JUMP_BAG01			"data/ANIMDATA/marine/jump/marine_bag01.csv"
#define ANIM_JUMP_BAG02			"data/ANIMDATA/marine/jump/marine_bag02.csv"

#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量

#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(17.0f)							// プレイヤーの足元をあわせる

#define PLAYER_PARTS_MAX	(13)								// プレイヤーのパーツの数



// パーツリスト
enum PARTS_LIST
{
	BODY,
	HEAD,
	LARM,
	RARM,
	LHAND,
	RHAND,
	LLEG,
	RLEG,
	LFOOT,
	RFOOT,
	GUN,
	BAG01,
	BAG02,
};

// アニメーションのリスト
enum ANIM_LIST
{

	IDLE_BODY,
	IDLE_HEAD,
	IDLE_LARM,
	IDLE_RARM,
	IDLE_LHAND,
	IDLE_RHAND,
	IDLE_LLEG,
	IDLE_RLEG,
	IDLE_LFOOT,
	IDLE_RFOOT,
	IDLE_GUN,
	IDLE_BAG01,
	IDLE_BAG02,

	MOVE_BODY,
	MOVE_HEAD,
	MOVE_LARM,
	MOVE_RARM,
	MOVE_LHAND,
	MOVE_RHAND,
	MOVE_LLEG,
	MOVE_RLEG,
	MOVE_LFOOT,
	MOVE_RFOOT,
	MOVE_GUN,
	MOVE_BAG01,
	MOVE_BAG02,

	ATK_BODY,
	ATK_HEAD,
	ATK_LARM,
	ATK_RARM,
	ATK_LHAND,
	ATK_RHAND,
	ATK_LLEG,
	ATK_RLEG,
	ATK_LFOOT,
	ATK_RFOOT,
	ATK_GUN,
	ATK_BAG01,
	ATK_BAG02,

	JUMP_BODY,
	JUMP_HEAD,
	JUMP_LARM,
	JUMP_RARM,
	JUMP_LHAND,
	JUMP_RHAND,
	JUMP_LLEG,
	JUMP_RLEG,
	JUMP_LFOOT,
	JUMP_RFOOT,
	JUMP_GUN,
	JUMP_BAG01,
	JUMP_BAG02,

	ANIM_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;						// プレイヤー

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

static float		roty = 0.0f;

static LIGHT		g_Light;

float				g_jumpPower;

// アニメーションのディレクトリの配列
static char* g_AnimDir[] =
{
	ANIM_IDLE_BODY,
	ANIM_IDLE_HEAD,
	ANIM_IDLE_LARM,
	ANIM_IDLE_RARM,
	ANIM_IDLE_LHAND,
	ANIM_IDLE_RHAND,
	ANIM_IDLE_LLEG,
	ANIM_IDLE_RLEG,
	ANIM_IDLE_LFOOT,
	ANIM_IDLE_RFOOT,
	ANIM_IDLE_GUN,
	ANIM_IDLE_BAG01,
	ANIM_IDLE_BAG02,

	ANIM_MOVE_BODY,
	ANIM_MOVE_HEAD,
	ANIM_MOVE_LARM,
	ANIM_MOVE_RARM,
	ANIM_MOVE_LHAND,
	ANIM_MOVE_RHAND,
	ANIM_MOVE_LLEG,
	ANIM_MOVE_RLEG,
	ANIM_MOVE_LFOOT,
	ANIM_MOVE_RFOOT,
	ANIM_MOVE_GUN,
	ANIM_MOVE_BAG01,
	ANIM_MOVE_BAG02,

	ANIM_ATK_BODY,
	ANIM_ATK_HEAD,
	ANIM_ATK_LARM,
	ANIM_ATK_RARM,
	ANIM_ATK_LHAND,
	ANIM_ATK_RHAND,
	ANIM_ATK_LLEG,
	ANIM_ATK_RLEG,
	ANIM_ATK_LFOOT,
	ANIM_ATK_RFOOT,
	ANIM_ATK_GUN,
	ANIM_ATK_BAG01,
	ANIM_ATK_BAG02,

	ANIM_JUMP_BODY,
	ANIM_JUMP_HEAD,
	ANIM_JUMP_LARM,
	ANIM_JUMP_RARM,
	ANIM_JUMP_LHAND,
	ANIM_JUMP_RHAND,
	ANIM_JUMP_LLEG,
	ANIM_JUMP_RLEG,
	ANIM_JUMP_LFOOT,
	ANIM_JUMP_RFOOT,
	ANIM_JUMP_GUN,
	ANIM_JUMP_BAG01,
	ANIM_JUMP_BAG02,

};

// プレイヤーの階層アニメーションデータ
// 
static std::vector<INTERPOLATION_DATA> idle_body;
static std::vector<INTERPOLATION_DATA> idle_head;
static std::vector<INTERPOLATION_DATA> idle_arml;
static std::vector<INTERPOLATION_DATA> idle_armr;
static std::vector<INTERPOLATION_DATA> idle_handl;
static std::vector<INTERPOLATION_DATA> idle_handr;
static std::vector<INTERPOLATION_DATA> idle_legl;
static std::vector<INTERPOLATION_DATA> idle_legr;
static std::vector<INTERPOLATION_DATA> idle_footl;
static std::vector<INTERPOLATION_DATA> idle_footr;
static std::vector<INTERPOLATION_DATA> idle_gun;
static std::vector<INTERPOLATION_DATA> idle_bag01;
static std::vector<INTERPOLATION_DATA> idle_bag02;

static std::vector<INTERPOLATION_DATA> move_body;
static std::vector<INTERPOLATION_DATA> move_head;
static std::vector<INTERPOLATION_DATA> move_arml;
static std::vector<INTERPOLATION_DATA> move_armr;
static std::vector<INTERPOLATION_DATA> move_handl;
static std::vector<INTERPOLATION_DATA> move_handr;
static std::vector<INTERPOLATION_DATA> move_legl;
static std::vector<INTERPOLATION_DATA> move_legr;
static std::vector<INTERPOLATION_DATA> move_footl;
static std::vector<INTERPOLATION_DATA> move_footr;
static std::vector<INTERPOLATION_DATA> move_gun;
static std::vector<INTERPOLATION_DATA> move_bag01;
static std::vector<INTERPOLATION_DATA> move_bag02;

static std::vector<INTERPOLATION_DATA> atk_body;
static std::vector<INTERPOLATION_DATA> atk_head;
static std::vector<INTERPOLATION_DATA> atk_arml;
static std::vector<INTERPOLATION_DATA> atk_armr;
static std::vector<INTERPOLATION_DATA> atk_handl;
static std::vector<INTERPOLATION_DATA> atk_handr;
static std::vector<INTERPOLATION_DATA> atk_legl;
static std::vector<INTERPOLATION_DATA> atk_legr;
static std::vector<INTERPOLATION_DATA> atk_footl;
static std::vector<INTERPOLATION_DATA> atk_footr;
static std::vector<INTERPOLATION_DATA> atk_gun;
static std::vector<INTERPOLATION_DATA> atk_bag01;
static std::vector<INTERPOLATION_DATA> atk_bag02;

static std::vector<INTERPOLATION_DATA> jump_body;
static std::vector<INTERPOLATION_DATA> jump_head;
static std::vector<INTERPOLATION_DATA> jump_arml;
static std::vector<INTERPOLATION_DATA> jump_armr;
static std::vector<INTERPOLATION_DATA> jump_handl;
static std::vector<INTERPOLATION_DATA> jump_handr;
static std::vector<INTERPOLATION_DATA> jump_legl;
static std::vector<INTERPOLATION_DATA> jump_legr;
static std::vector<INTERPOLATION_DATA> jump_footl;
static std::vector<INTERPOLATION_DATA> jump_footr;
static std::vector<INTERPOLATION_DATA> jump_gun;
static std::vector<INTERPOLATION_DATA> jump_bag01;
static std::vector<INTERPOLATION_DATA> jump_bag02;

static std::vector<INTERPOLATION_DATA>* g_TblAdr[] =
{
	&idle_body,
	& idle_head,
	& idle_arml,
	& idle_armr,
	& idle_handl,
	& idle_handr,
	& idle_legl,
	& idle_legr,
	& idle_footl,
	& idle_footr,
	& idle_gun,
	& idle_bag01,
	& idle_bag02,

	& move_body,
	& move_head,
	& move_arml,
	& move_armr,
	& move_handl,
	& move_handr,
	& move_legl,
	& move_legr,
	& move_footl,
	& move_footr,
	& move_gun,
	& move_bag01,
	& move_bag02,

	& atk_body,
	& atk_head,
	& atk_arml,
	& atk_armr,
	& atk_handl,
	& atk_handr,
	& atk_legl,
	& atk_legr,
	& atk_footl,
	& atk_footr,
	& atk_gun,
	& atk_bag01,
	& atk_bag02,

	& jump_body,
	& jump_head,
	& jump_arml,
	& jump_armr,
	& jump_handl,
	& jump_handr,
	& jump_legl,
	& jump_legr,
	& jump_footl,
	& jump_footr,
	& jump_gun,
	& jump_bag01,
	& jump_bag02,

};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	// アニメーションのSCVファイルから
	for (int i = 0; i < ANIM_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}

	g_Player.load = TRUE;
	//LoadModel(MODEL_PLAYER, &g_Player.model);

	g_Player.pos = XMFLOAT3(-10.0f, PLAYER_OFFSET_Y+50.0f, -50.0f);
	g_Player.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Player.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Player.spd = 0.0f;			// 移動スピードクリア

	g_Player.inviCnt = 0;
	g_Player.hp = PLAYER_LIFE;		// プレイヤーのHPの設定
	g_Player.use = TRUE;			// TRUE:生きてる
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	// キーを押した時のプレイヤーの向き
	roty = 0.0f;

	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる


	// 階層アニメーションの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = &g_Player;		// ← ここに親のアドレスを入れる

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = FALSE;
	}

	g_Parts[0].use = TRUE;
	//g_Parts[0].parent = &g_Player;	// 親をセット
	g_Parts[0].tblNo = 0;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0].tblMax = (int)g_TblAdr[g_Parts[0].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[0].load = 1;
	LoadModel(MODEL_PLAYER, &g_Parts[0].model);

	g_Parts[1].use = TRUE;
	//g_Parts[1].parent = &g_Parts[0];	// 親をセット
	g_Parts[1].tblNo = 1;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tblMax = (int)g_TblAdr[g_Parts[1].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[1].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &g_Parts[1].model);

	g_Parts[2].use = TRUE;
	//g_Parts[2].parent = &g_Parts[0];	// 親をセット
	g_Parts[2].tblNo = 2;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[2].tblMax = (int)g_TblAdr[g_Parts[2].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[2].load = 1;
	LoadModel(MODEL_PLAYER_LARM, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	//g_Parts[3].parent = &g_Parts[0];	// 親をセット
	g_Parts[3].tblNo = 3;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[3].tblMax = (int)g_TblAdr[g_Parts[3].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[3].load = 1;
	LoadModel(MODEL_PLAYER_RARM, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	//g_Parts[4].parent = &g_Parts[0];	// 親をセット
	g_Parts[4].tblNo = 4;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[4].tblMax = (int)g_TblAdr[g_Parts[4].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[4].load = 1;
	LoadModel(MODEL_PLAYER_LHAND, &g_Parts[4].model);

	g_Parts[5].use = TRUE;
	//g_Parts[5].parent = &g_Parts[0];	// 親をセット
	g_Parts[5].tblNo = 5;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[5].tblMax = (int)g_TblAdr[g_Parts[5].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[5].load = 1;
	LoadModel(MODEL_PLAYER_RHAND, &g_Parts[5].model);

	g_Parts[6].use = TRUE;
	//g_Parts[6].parent = &g_Parts[0];	// 親をセット
	g_Parts[6].tblNo = 6;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[6].tblMax = (int)g_TblAdr[g_Parts[6].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[6].load = 1;
	LoadModel(MODEL_PLAYER_LLEG, &g_Parts[6].model);

	g_Parts[7].use = TRUE;
	//g_Parts[7].parent = &g_Parts[0];	// 親をセット
	g_Parts[7].tblNo = 7;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[7].tblMax = (int)g_TblAdr[g_Parts[7].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[7].load = 1;
	LoadModel(MODEL_PLAYER_RLEG, &g_Parts[7].model);

	g_Parts[8].use = TRUE;
	//g_Parts[8].parent = &g_Parts[0];	// 親をセット
	g_Parts[8].tblNo = 8;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[8].tblMax = (int)g_TblAdr[g_Parts[8].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[8].load = 1;
	LoadModel(MODEL_PLAYER_RFOOT, &g_Parts[8].model);

	g_Parts[9].use = TRUE;
	//g_Parts[9].parent = &g_Parts[0];	// 親をセット
	g_Parts[9].tblNo = 9;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[9].tblMax = (int)g_TblAdr[g_Parts[9].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[9].load = 1;
	LoadModel(MODEL_PLAYER_RFOOT, &g_Parts[9].model);

	g_Parts[10].use = TRUE;
	//g_Parts[10].parent = &g_Parts[0];	// 親をセット
	g_Parts[10].tblNo = 10;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[10].tblMax = (int)g_TblAdr[g_Parts[10].tblNo]->size();	// 再生するアニメデータのレコード数をセット
	g_Parts[10].load = 1;
	LoadModel(MODEL_PLAYER_GUN, &g_Parts[10].model);

	g_Parts[11].use = TRUE;
	//g_Parts[11].parent = &g_Parts[0];	// 親をセット
	g_Parts[11].tblNo = 11;			// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[11].tblMax = sizeof(move_tbl_rleg) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[11].tblMax = (int)g_TblAdr[g_Parts[11].tblNo]->size();	// 再生するアニメデータのレコード数をセット

	g_Parts[11].load = 1;
	LoadModel(MODEL_PLAYER_BAG01, &g_Parts[11].model);

	g_Parts[12].use = TRUE;
	//g_Parts[12].parent = &g_Parts[0];	// 親をセット
	g_Parts[12].tblNo = 12;			// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[12].tblMax = sizeof(move_tbl_rleg) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[12].tblMax = (int)g_TblAdr[g_Parts[12].tblNo]->size();	// 再生するアニメデータのレコード数をセット

	g_Parts[12].load = 1;
	LoadModel(MODEL_PLAYER_BAG02, &g_Parts[12].model);


	// クォータニオンの初期化
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	if (g_Player.load == TRUE)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	// パーツの解放処理
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
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
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	// プレイヤーをカメラに向ける
	XMVECTOR CameraAt = XMLoadFloat3(&cam->at);
	XMVECTOR PlayerPos = XMLoadFloat3(&g_Player.pos);
	XMVECTOR vec = PlayerPos - CameraAt;

	float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]) ;
	
	g_Player.spd *= 0.7f;

	// 当たり判定の無敵処理
	if (g_Player.inviCnt > 0)
	{
		g_Player.inviCnt--;
	}


	// 移動処理
	if (g_Player.state != PLAYER_STATE_JUMP)
	{
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, STICK_L_LEFT))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer - XM_PIDIV2;
			g_Player.state = PLAYER_STATE_MOVE;
		}
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, STICK_L_RIGHT))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer + XM_PIDIV2;
			g_Player.state = PLAYER_STATE_MOVE;
		}
		if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, STICK_L_UP))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer;
			g_Player.state = PLAYER_STATE_MOVE;
		}
		if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, STICK_L_DOWN))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer + XM_PI;
			g_Player.state = PLAYER_STATE_MOVE;
		}
	}


	switch (g_Player.state)
	{
	case PLAYER_STATE_IDLE:
		SetTblIdle();
		break;

	case PLAYER_STATE_MOVE:
		SetTblMove();
		break;

	case PLAYER_STATE_ATK:
		SetTblAtk();
		AttackPlayer();
		break;

	case PLAYER_STATE_JUMP:
		SetTblJump();
		JumpPlayer();
		break;
	}


#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.spd = 0.0f;
		roty = 0.0f;
	}
#endif


	{	// 押した方向にプレイヤーを移動させる
		// 押した方向にプレイヤーを向かせている所

		g_Player.rot.y = anglePlayer;

		g_Player.pos.x -= sinf(roty) * g_Player.spd;
		g_Player.pos.z -= cosf(roty) * g_Player.spd;
	}


	// レイキャストして足元の高さを求める
	XMFLOAT3 HitPosition;		// 交点
	XMFLOAT3 Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);			// ぶつかったポリゴンの法線ベクトル（向き）
	if (g_Player.jump == FALSE)
	{
		BOOL ans = RayHitField(g_Player.pos, &HitPosition, &Normal);
		if (ans)
		{
			g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
		}
		else
		{
			g_Player.pos.y = PLAYER_OFFSET_Y;
			Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Player.pos;
	pos.y =  0.1f;
	SetPositionShadow(g_Player.shadowIdx, pos);



	// ジャンプ処理
	if (g_Player.state != PLAYER_STATE_ATK)
	{
		if ((GetKeyboardTrigger(DIK_SPACE)) && (g_Player.jump == FALSE) || IsButtonTriggered(0, BUTTON_A) && (g_Player.jump == FALSE))
		{
			g_Player.state = PLAYER_STATE_JUMP;
			g_Player.jump = TRUE;
			g_Player.jumpCnt = 0;

			for (int i = 0; i < PLAYER_PARTS_MAX; i++)
			{
				g_Parts[i].time = 0.0f;
			}
		}
	}


	// 弾発射処理
	if (g_Player.state != PLAYER_STATE_JUMP)
	{
		//if ((GetKeyboardTrigger(DIK_X) && (g_Player.atk == FALSE) || IsButtonTriggered(0, BUTTON_R)) && (g_Player.atk == FALSE))
		if ((GetKeyboardTrigger(DIK_X)  || IsButtonTriggered(0, BUTTON_R)) )
		{

			PlaySound(SOUND_LABEL_SE_Charge02);

			g_Player.state = PLAYER_STATE_ATK;
			g_Player.atk = TRUE;
			g_Player.gunCnt = 0;


			for (int i = 0; i < PLAYER_PARTS_MAX; i++)
			{
				g_Parts[i].time = 0.0f;
			}
		}
	}


	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	// 線形補間の処理
			int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
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
			
				switch (g_Player.setTbl)
				{
				// アニメイシヨンが終わったらIDLEに戻る
				case PLAYER_STATE_ATK:
					g_Parts[i].tblMax = -1;
					g_Player.state = PLAYER_STATE_IDLE;
					break;

				case PLAYER_STATE_JUMP:
					g_Parts[i].tblMax = -1;
					g_Player.state = PLAYER_STATE_IDLE;
					break;

				}

			}

		}

	}

	// ポイントライトのテスト
	{
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}

	if ((g_Player.state != PLAYER_STATE_ATK) && (g_Player.state != PLAYER_STATE_JUMP))
	{
		g_Player.state = PLAYER_STATE_IDLE;
	}


	//////////////////////////////////////////////////////////////////////
	// 姿勢制御
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

//	quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);




#ifdef _DEBUG

	if (GetKeyboardPress(DIK_1))
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

	// デバッグ表示
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{



	if (g_Player.use == FALSE) return;



	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	//// 縁取りの設定
	//SetFuchi(1);

	// モデル描画
	//DrawModel(&g_Player.model);



	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
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
																			// g_Player.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する
		if (g_Parts[i].use == FALSE) continue;

		for (int j= 0; j < g_Parts[i].model.SubsetNum;j++)
		{
			if (g_Player.inviCnt > 0)
			{
				SetModelDiffuse(&g_Parts[i].model, j, XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f));
			}
			else
			{
				SetModelDiffuse(&g_Parts[i].model, j, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			}
		}

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);


		// モデル描画
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

void AttackPlayer(void)
{
	
	// 状態遷移
	{
		BOOL ans = TRUE;

		// 全てのパーツの最大テーブルが-1になっている場合(≒アニメーションが終了している場合)、状態遷移させる
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].tblMax != -1) ans = FALSE;

		}

		if (ans == TRUE)
		{
			g_Player.state = PLAYER_STATE_IDLE;
		}
	}


	{
		g_Player.gunCnt++;

		XMFLOAT3 pos = g_Player.pos;
		pos.x -= sinf(g_Player.rot.y - XM_PIDIV2);
		pos.z -= cosf(g_Player.rot.y - XM_PIDIV2);

		pos.x -= sinf(g_Player.rot.y) * 27.5f;
		pos.z -= cosf(g_Player.rot.y) * 27.5f;

		XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);


		if (g_Player.gunCnt < 34 && g_Player.gunCnt > 16)
		{

			for (int i = 0; i < 30; i++)
			{
				move.y = (rand() % 3 - 1) * 0.1f;
				move.z = (rand() % 3 - 1) * 0.1f;
				move.x = (rand() % 3 - 1) * 0.1f;

				SetEffect(pos, move, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 30, 1);
				
				PlaySound(SOUND_LABEL_SE_Shot01);

			}

		}

		else if (g_Player.gunCnt > 34)
		{
			move = XMFLOAT3(-sinf(g_Player.rot.y) * 5.0f, 0.0f, -cosf(g_Player.rot.y) * 5.0f);
			SetParticle(pos, move, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 60);
			g_Player.atk = FALSE;

		}


	}


}


void JumpPlayer(void)
{

	// 状態遷移
	{
		BOOL ans = TRUE;

		// 全てのパーツの最大テーブルが-1になっている場合(≒アニメーションが終了している場合)、状態遷移させる
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].tblMax != -1) ans = FALSE;

		}

		if (ans == TRUE) g_Player.state = PLAYER_STATE_IDLE;
	}


	{
		

		g_Player.jumpCnt++;

		XMFLOAT3 pos = g_Player.pos;

		CAMERA* cam = GetCamera();

		// プレイヤーをカメラに向ける
		XMVECTOR CameraAt = XMLoadFloat3(&cam->at);
		XMVECTOR PlayerPos = XMLoadFloat3(&g_Player.pos);
		XMVECTOR vec = PlayerPos - CameraAt;

		float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]);

		g_jumpPower = 5.0f;

		if (g_Player.jumpCnt > 14 && g_Player.jumpCnt < 54)
		{
			if (g_Player.jump == TRUE)
			{
				//g_Player.pos.y += g_jumpPower;
				g_Player.spd = 2.0f*VALUE_MOVE;
				roty = anglePlayer;
				



				if (g_jumpPower >= 5.0f)
				{
					g_jumpPower -= 1.0f;
				}
				else
				{
					g_jumpPower = 0.0f;
				}

				// パーティクルの発生箇所を設定(※ジェットパックの座標を取得)
				XMFLOAT3 rPos = { g_Parts[BAG02].mtxWorld._41, g_Parts[BAG02].mtxWorld._42, g_Parts[BAG02].mtxWorld._43 };
				XMFLOAT3 lPos = rPos;

				// 右側（※プレイヤーの向きに合わせて右にずらす）
				rPos.x -= sinf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				rPos.z -= cosf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				rPos.y -= 5.0f;

				// 左側（※プレイヤーの向きに合わせて左にずらす）
				lPos.x -= sinf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				lPos.z -= cosf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				lPos.y -= 5.0f;

				// パーティクルの発生
				SetParticle(rPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 0.47f, 0.0f, 1.0f), 30);
				SetParticle(rPos, XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20);
				SetParticle(lPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 0.47f, 0.0f, 1.0f), 30);
				SetParticle(lPos, XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20);
				//SetParticle(lPos, XMFLOAT3(0.0f, -0.05f,  0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, 0.0f, 30);

			}
		}
		else if (g_Player.jumpCnt >= 54)
		{
			if (g_Player.jump == TRUE)
			{

				//g_Player.pos.y -= g_jumpPower;
				if (g_jumpPower < 5.0f)
				{
					g_jumpPower += 1.0f;
				}
				if (g_Player.pos.y < PLAYER_OFFSET_Y)
				{
					g_Player.pos.y = PLAYER_OFFSET_Y;
					g_Player.jump = FALSE;
					g_Player.state = PLAYER_STATE_IDLE;
				}


				// パーティクルの発生箇所を設定(※ジェットパックの座標を取得)
				XMFLOAT3 rPos = { g_Parts[RFOOT].mtxWorld._41, g_Parts[RFOOT].mtxWorld._42, g_Parts[RFOOT].mtxWorld._43 };
				XMFLOAT3 lPos = { g_Parts[LFOOT].mtxWorld._41, g_Parts[LFOOT].mtxWorld._42, g_Parts[LFOOT].mtxWorld._43 };

				//// 右側（※プレイヤーの向きに合わせて右にずらす）
				//rPos.x -= sinf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				//rPos.z -= cosf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				//rPos.y -= 5.0f;

				//// 左側（※プレイヤーの向きに合わせて左にずらす）
				//lPos.x -= sinf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				//lPos.z -= cosf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				//lPos.y -= 5.0f;

				// パーティクルの発生
				//SetParticle(rPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 60);
				//SetParticle(lPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 600);
				SetParticleLerp(rPos, lPos, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 60);
			}

		}
	}
}


void SetTblIdle(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_IDLE:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + IDLE_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_IDLE;
		break;
	}
}

void SetTblMove(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_MOVE:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + MOVE_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_MOVE;
		break;
	}
}


void SetTblAtk(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_ATK:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + ATK_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_ATK;
		break;
	}

}

void SetTblJump(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_JUMP:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + JUMP_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_JUMP;
		break;
	}

}
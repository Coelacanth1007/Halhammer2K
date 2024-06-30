//=============================================================================
//
// モデル処理 [player.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once
#include "model.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER		(1)					// プレイヤーの数

#define	PLAYER_SIZE		(5.0f)				// 当たり判定の大きさ

#define PLAYER_LIFE		(4)					// プレイヤーのライフ HP

#define INVI_TIME		(180)								// プレイヤーのパーツの数

enum  PLAYER_STATE_LIST {
	PLAYER_STATE_IDLE,
	PLAYER_STATE_MOVE,
	PLAYER_STATE_ATK,
	PLAYER_STATE_JUMP,

};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	float				spd;				// 移動スピード
	

	int					hp;					// HP
	BOOL				load;
	DX11_MODEL			model;				// モデル情報

	int					shadowIdx;			// 影のインデックス番号

	BOOL				use;

	float				size;

	int					state;		// playerの状態

	int					gunCnt;
	int					pressCnt;
	BOOL				charge;

	int					jumpCnt;
	BOOL				jump;
	BOOL				atk;

	// 階層アニメーション用のメンバー変数
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
	int					setTbl;		// セットしているアニメーションのテーブル

	// 親は、NULL、子供は親のアドレスを入れる
	PLAYER				*parent;			// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	// クォータニオン
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// 自分が立っている所

	int					inviCnt;
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

void AttackPlayer(void);
void JumpPlayer(void);


void SetTblIdle(void);
void SetTblMove(void);
void SetTblAtk(void);
void SetTblJump(void);

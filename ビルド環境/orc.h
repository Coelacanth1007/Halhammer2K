//=============================================================================
//
// 敵BOSS処理 [orc.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ORC		(1)					// プレイヤーの数

#define	ORC_SIZE		(25.0f)				// 当たり判定の大きさ

enum  ORC_STATE_LIST {

	ORC_STATE_IDLE,
	ORC_STATE_MOVE,
	ORC_STATE_ATK,

};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ORC
{
	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		rot;		// ポリゴンの向き(回転)
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4		mtxWorld;	// ワールドマトリックス

	BOOL			load;
	DX11_MODEL		model;		// モデル情報

	float			spd;		// 移動スピード
	float			dir;		// 向き
	float			size;		// 当たり判定の大きさ
	int				shadowIdx;	// 影のIndex

	int				hp;


	BOOL			use;
	BOOL			moving;
	BOOL			attack;

	int				state;		// orcの状態

	BOOL			attackable;		// orcが攻撃られれる状態

	// 階層アニメーション用のメンバー変数
	float			time;		// 線形補間用
	int				tblNo;		// 行動データのテーブル番号
	int				tblMax;		// そのテーブルのデータ数
	int				setTbl;		// セットしているアニメーションのテーブル

	// 親は、NULL、子供は親のアドレスを入れる(セーブ＆ロードの時は↓ここ気をつける事)
	ORC			*parent;	// 自分が親ならNULL、自分が子供なら親のorcアドレス

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitOrc(void);
void UninitOrc(void);
void UpdateOrc(void);
void DrawOrc(void);

ORC *GetOrc(void);

void AttackOrc(void);


void SetEnmTblIdle(void);
void SetEnmTblMove(void);
void SetEnmTblAtk(void);


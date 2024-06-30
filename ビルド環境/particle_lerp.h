//=============================================================================
//
// 線形補間パーティクル処理 [particle_lerp.h]
// Author : GP12B183 24 余　文杰 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PARTICLE_LERP
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scl;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	XMFLOAT4		RGBa;			// 保持用RGBa
	XMFLOAT3		sPos;			// 開始位置(線形補間用)
	XMFLOAT3		ePos;			// 目標位置(線形補間用)
	float			lerpTime;		// 線形補間用の時間
	int				life;			// 寿命
	BOOL			use;			// 使用しているかどうか
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT		InitParticleLerp(void);
void		UninitParticleLerp(void);
void		UpdateParticleLerp(void);
void		DrawParticleLerp(void);

int			SetParticleLerp(XMFLOAT3 pos, XMFLOAT3 tarPos, XMFLOAT3 scl, XMFLOAT4 col, int life);
void		SetColorParticleLerp(int index, XMFLOAT4 color);


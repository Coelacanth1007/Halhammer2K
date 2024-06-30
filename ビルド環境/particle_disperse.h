//=============================================================================
//
// 分散パーティクル処理 [particle_disperse.h]
// Author : GP11B132 26 山下 航
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PARTICLE_DISPERSE
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		scl;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	XMFLOAT4		RGBa;			// 保持用RGBa
	int				life;			// 寿命
	BOOL			use;			// 使用しているかどうか
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	 InitParticleDisperse(void);
void	 UninitParticleDisperse(void);
void	 UpdateParticleDisperse(void);
void	 DrawParticleDisperse(void);

void	 SetColorParticleDisperse(int index, XMFLOAT4 color);
int		 SetParticleDisperse(XMFLOAT3 pos, XMFLOAT3 scl, XMFLOAT3 move, XMFLOAT4 col, int life);


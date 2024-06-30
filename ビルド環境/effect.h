//=============================================================================
//
// パーティクル処理 [effect.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife , int type);
void SetColorEffect(int nIdxEffect, XMFLOAT4 col);


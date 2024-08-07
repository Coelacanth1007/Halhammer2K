//=============================================================================
//
// ライト処理 [light.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLight(void);
void UpdateLight(void);

void SetLightData(int index, LIGHT *light);
void SetFogData(FOG *fog);
BOOL GetFogEnable(void);

LIGHT *GetLightData(int index);


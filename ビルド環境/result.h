//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum
{
	RESULT_REPLAY,		// GAME画面に戻る
	RESULT_TITLE,		// TITLE画面に戻る
	RESULT_EXIT,		// TITLE画面に戻る
	RESULT_MAX,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);



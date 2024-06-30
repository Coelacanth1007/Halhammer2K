//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

enum
{
	START,			// GAME画面に進む
	TUTORIAL,		// TUTORIALに進む
	EXIT,			// ゲーム終了
	TITLE_MAX
};

struct TITLE_LOGO
{
	XMFLOAT3	pos;		// ポリゴンの座標
	BOOL		use;		//
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号

	float	countAnim;
	int		patternAnim;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);



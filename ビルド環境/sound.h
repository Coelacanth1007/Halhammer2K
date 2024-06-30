//=============================================================================
//
// サウンド処理 [sound.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_TITLE,		// BGM0
	SOUND_LABEL_BGM_BATTLE,		// BGM1
	SOUND_LABEL_BGM_RESULT,	// BGM2
	SOUND_LABEL_SE_Charge01,	// 爆発音
	SOUND_LABEL_SE_Charge02,	// 
	SOUND_LABEL_SE_Shot01,		// 
	SOUND_LABEL_SE_Shot02,		// 
	SOUND_LABEL_SE_laser000,	// 
	SOUND_LABEL_SE_lockon000,	// 
	SOUND_LABEL_SE_shot000,		// 
	SOUND_LABEL_SE_shot001,		// 
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);


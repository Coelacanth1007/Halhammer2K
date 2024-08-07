//=============================================================================
//
// キーフレームファイル読み込み処理 [keyframe.h]
// Author : GP12B183 24 余　文杰
//
//=============================================================================
#pragma once
#include <vector>


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void LoadAnimDataText(char* FileName, std::vector<INTERPOLATION_DATA>& tbl);
void LoadAnimDataCSV(char* FileName, std::vector<INTERPOLATION_DATA>& tbl);
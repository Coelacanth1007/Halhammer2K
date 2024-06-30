//=============================================================================
//
// �p�[�e�B�N������ [effect.h]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife , int type);
void SetColorEffect(int nIdxEffect, XMFLOAT4 col);


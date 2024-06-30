//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

enum
{
	START,			// GAME��ʂɐi��
	TUTORIAL,		// TUTORIAL�ɐi��
	EXIT,			// �Q�[���I��
	TITLE_MAX
};

struct TITLE_LOGO
{
	XMFLOAT3	pos;		// �|���S���̍��W
	BOOL		use;		//
	float		w, h;		// ���ƍ���
	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	float	countAnim;
	int		patternAnim;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);



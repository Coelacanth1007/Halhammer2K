//=============================================================================
//
// ���`��ԃp�[�e�B�N������ [particle_lerp.h]
// Author : GP12B183 24 �]�@���^ 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PARTICLE_LERP
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scl;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	XMFLOAT4		RGBa;			// �ێ��pRGBa
	XMFLOAT3		sPos;			// �J�n�ʒu(���`��ԗp)
	XMFLOAT3		ePos;			// �ڕW�ʒu(���`��ԗp)
	float			lerpTime;		// ���`��ԗp�̎���
	int				life;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT		InitParticleLerp(void);
void		UninitParticleLerp(void);
void		UpdateParticleLerp(void);
void		DrawParticleLerp(void);

int			SetParticleLerp(XMFLOAT3 pos, XMFLOAT3 tarPos, XMFLOAT3 scl, XMFLOAT4 col, int life);
void		SetColorParticleLerp(int index, XMFLOAT4 color);


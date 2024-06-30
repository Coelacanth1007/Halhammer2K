//=============================================================================
//
// ���U�p�[�e�B�N������ [particle_disperse.h]
// Author : GP11B132 26 �R�� �q
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PARTICLE_DISPERSE
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		scl;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	XMFLOAT4		RGBa;			// �ێ��pRGBa
	int				life;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT	 InitParticleDisperse(void);
void	 UninitParticleDisperse(void);
void	 UpdateParticleDisperse(void);
void	 DrawParticleDisperse(void);

void	 SetColorParticleDisperse(int index, XMFLOAT4 color);
int		 SetParticleDisperse(XMFLOAT3 pos, XMFLOAT3 scl, XMFLOAT3 move, XMFLOAT4 col, int life);


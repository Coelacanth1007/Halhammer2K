//=============================================================================
//
// �GBOSS���� [orc.h]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ORC		(1)					// �v���C���[�̐�

#define	ORC_SIZE		(25.0f)				// �����蔻��̑傫��

enum  ORC_STATE_LIST {

	ORC_STATE_IDLE,
	ORC_STATE_MOVE,
	ORC_STATE_ATK,

};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ORC
{
	XMFLOAT3		pos;		// �|���S���̈ʒu
	XMFLOAT3		rot;		// �|���S���̌���(��])
	XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���b�N�X

	BOOL			load;
	DX11_MODEL		model;		// ���f�����

	float			spd;		// �ړ��X�s�[�h
	float			dir;		// ����
	float			size;		// �����蔻��̑傫��
	int				shadowIdx;	// �e��Index

	int				hp;


	BOOL			use;
	BOOL			moving;
	BOOL			attack;

	int				state;		// orc�̏��

	BOOL			attackable;		// orc���U���������

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float			time;		// ���`��ԗp
	int				tblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int				tblMax;		// ���̃e�[�u���̃f�[�^��
	int				setTbl;		// �Z�b�g���Ă���A�j���[�V�����̃e�[�u��

	// �e�́ANULL�A�q���͐e�̃A�h���X������(�Z�[�u�����[�h�̎��́������C�����鎖)
	ORC			*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��orc�A�h���X

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitOrc(void);
void UninitOrc(void);
void UpdateOrc(void);
void DrawOrc(void);

ORC *GetOrc(void);

void AttackOrc(void);


void SetEnmTblIdle(void);
void SetEnmTblMove(void);
void SetEnmTblAtk(void);


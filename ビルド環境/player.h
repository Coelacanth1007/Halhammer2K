//=============================================================================
//
// ���f������ [player.h]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#pragma once
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�

#define	PLAYER_SIZE		(5.0f)				// �����蔻��̑傫��

#define PLAYER_LIFE		(4)					// �v���C���[�̃��C�t HP

#define INVI_TIME		(180)								// �v���C���[�̃p�[�c�̐�

enum  PLAYER_STATE_LIST {
	PLAYER_STATE_IDLE,
	PLAYER_STATE_MOVE,
	PLAYER_STATE_ATK,
	PLAYER_STATE_JUMP,

};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h
	

	int					hp;					// HP
	BOOL				load;
	DX11_MODEL			model;				// ���f�����

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	BOOL				use;

	float				size;

	int					state;		// player�̏��

	int					gunCnt;
	int					pressCnt;
	BOOL				charge;

	int					jumpCnt;
	BOOL				jump;
	BOOL				atk;

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
	int					setTbl;		// �Z�b�g���Ă���A�j���[�V�����̃e�[�u��

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;			// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// �����������Ă��鏊

	int					inviCnt;
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

void AttackPlayer(void);
void JumpPlayer(void);


void SetTblIdle(void);
void SetTblMove(void);
void SetTblAtk(void);
void SetTblJump(void);

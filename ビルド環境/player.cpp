//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "particle.h"
#include "particle_lerp.h"
#include "enemy.h"
#include "collision.h"
#include "keyframe.h"
#include "effect.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/marine/marine_body.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_HEAD	"data/MODEL/marine/marine_head.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LARM	"data/MODEL/marine/marine_arm_l.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RARM	"data/MODEL/marine/marine_arm_r.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LHAND	"data/MODEL/marine/marine_hand_l.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RHAND	"data/MODEL/marine/marine_hand_r.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LLEG	"data/MODEL/marine/marine_leg_l.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RLEG	"data/MODEL/marine/marine_leg_r.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LFOOT	"data/MODEL/marine/marine_foot_l.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RFOOT	"data/MODEL/marine/marine_foot_r.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GUN	"data/MODEL/marine/plasmagun.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BAG01	"data/MODEL/marine/bag01.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BAG02	"data/MODEL/marine/bag02.obj"			// �ǂݍ��ރ��f����

// �A�j���C�V�����̃f�B���N�g��
// 
// �ҋ@���
#define ANIM_IDLE_BODY			"data/ANIMDATA/marine/idle/marine_body.csv"
#define ANIM_IDLE_HEAD			"data/ANIMDATA/marine/idle/marine_head.csv"
#define ANIM_IDLE_LARM			"data/ANIMDATA/marine/idle/marine_arm_l.csv"
#define ANIM_IDLE_RARM			"data/ANIMDATA/marine/idle/marine_arm_r.csv"
#define ANIM_IDLE_LHAND			"data/ANIMDATA/marine/idle/marine_hand_l.csv"
#define ANIM_IDLE_RHAND			"data/ANIMDATA/marine/idle/marine_hand_r.csv"
#define ANIM_IDLE_LLEG			"data/ANIMDATA/marine/idle/marine_leg_l.csv"
#define ANIM_IDLE_RLEG			"data/ANIMDATA/marine/idle/marine_leg_r.csv"
#define ANIM_IDLE_LFOOT			"data/ANIMDATA/marine/idle/marine_foot_l.csv"
#define ANIM_IDLE_RFOOT			"data/ANIMDATA/marine/idle/marine_foot_r.csv"
#define ANIM_IDLE_GUN			"data/ANIMDATA/marine/idle/marine_plasmagun.csv"
#define ANIM_IDLE_BAG01			"data/ANIMDATA/marine/idle/marine_bag01.csv"
#define ANIM_IDLE_BAG02			"data/ANIMDATA/marine/idle/marine_bag02.csv"

// �ړ����
#define ANIM_MOVE_BODY			"data/ANIMDATA/marine/move/marine_body.csv"
#define ANIM_MOVE_HEAD			"data/ANIMDATA/marine/move/marine_head.csv"
#define ANIM_MOVE_LARM			"data/ANIMDATA/marine/move/marine_arm_l.csv"
#define ANIM_MOVE_RARM			"data/ANIMDATA/marine/move/marine_arm_r.csv"
#define ANIM_MOVE_LHAND			"data/ANIMDATA/marine/move/marine_hand_l.csv"
#define ANIM_MOVE_RHAND			"data/ANIMDATA/marine/move/marine_hand_r.csv"
#define ANIM_MOVE_LLEG			"data/ANIMDATA/marine/move/marine_leg_l.csv"
#define ANIM_MOVE_RLEG			"data/ANIMDATA/marine/move/marine_leg_r.csv"
#define ANIM_MOVE_LFOOT			"data/ANIMDATA/marine/move/marine_foot_l.csv"
#define ANIM_MOVE_RFOOT			"data/ANIMDATA/marine/move/marine_foot_r.csv"
#define ANIM_MOVE_GUN			"data/ANIMDATA/marine/move/marine_plasmagun.csv"
#define ANIM_MOVE_BAG01			"data/ANIMDATA/marine/move/marine_bag01.csv"
#define ANIM_MOVE_BAG02			"data/ANIMDATA/marine/move/marine_bag02.csv"

// �ړ����
#define ANIM_ATK_BODY			"data/ANIMDATA/marine/attack/marine_body.csv"
#define ANIM_ATK_HEAD			"data/ANIMDATA/marine/attack/marine_head.csv"
#define ANIM_ATK_LARM			"data/ANIMDATA/marine/attack/marine_arm_l.csv"
#define ANIM_ATK_RARM			"data/ANIMDATA/marine/attack/marine_arm_r.csv"
#define ANIM_ATK_LHAND			"data/ANIMDATA/marine/attack/marine_hand_l.csv"
#define ANIM_ATK_RHAND			"data/ANIMDATA/marine/attack/marine_hand_r.csv"
#define ANIM_ATK_LLEG			"data/ANIMDATA/marine/attack/marine_leg_l.csv"
#define ANIM_ATK_RLEG			"data/ANIMDATA/marine/attack/marine_leg_r.csv"
#define ANIM_ATK_LFOOT			"data/ANIMDATA/marine/attack/marine_foot_l.csv"
#define ANIM_ATK_RFOOT			"data/ANIMDATA/marine/attack/marine_foot_r.csv"
#define ANIM_ATK_GUN			"data/ANIMDATA/marine/attack/marine_plasmagun.csv"
#define ANIM_ATK_BAG01			"data/ANIMDATA/marine/attack/marine_bag01.csv"
#define ANIM_ATK_BAG02			"data/ANIMDATA/marine/attack/marine_bag02.csv"

// �W�����v���
#define ANIM_JUMP_BODY			"data/ANIMDATA/marine/jump/marine_body.csv"
#define ANIM_JUMP_HEAD			"data/ANIMDATA/marine/jump/marine_head.csv"
#define ANIM_JUMP_LARM			"data/ANIMDATA/marine/jump/marine_arm_l.csv"
#define ANIM_JUMP_RARM			"data/ANIMDATA/marine/jump/marine_arm_r.csv"
#define ANIM_JUMP_LHAND			"data/ANIMDATA/marine/jump/marine_hand_l.csv"
#define ANIM_JUMP_RHAND			"data/ANIMDATA/marine/jump/marine_hand_r.csv"
#define ANIM_JUMP_LLEG			"data/ANIMDATA/marine/jump/marine_leg_l.csv"
#define ANIM_JUMP_RLEG			"data/ANIMDATA/marine/jump/marine_leg_r.csv"
#define ANIM_JUMP_LFOOT			"data/ANIMDATA/marine/jump/marine_foot_l.csv"
#define ANIM_JUMP_RFOOT			"data/ANIMDATA/marine/jump/marine_foot_r.csv"
#define ANIM_JUMP_GUN			"data/ANIMDATA/marine/jump/marine_plasmagun.csv"
#define ANIM_JUMP_BAG01			"data/ANIMDATA/marine/jump/marine_bag01.csv"
#define ANIM_JUMP_BAG02			"data/ANIMDATA/marine/jump/marine_bag02.csv"

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(17.0f)							// �v���C���[�̑��������킹��

#define PLAYER_PARTS_MAX	(13)								// �v���C���[�̃p�[�c�̐�



// �p�[�c���X�g
enum PARTS_LIST
{
	BODY,
	HEAD,
	LARM,
	RARM,
	LHAND,
	RHAND,
	LLEG,
	RLEG,
	LFOOT,
	RFOOT,
	GUN,
	BAG01,
	BAG02,
};

// �A�j���[�V�����̃��X�g
enum ANIM_LIST
{

	IDLE_BODY,
	IDLE_HEAD,
	IDLE_LARM,
	IDLE_RARM,
	IDLE_LHAND,
	IDLE_RHAND,
	IDLE_LLEG,
	IDLE_RLEG,
	IDLE_LFOOT,
	IDLE_RFOOT,
	IDLE_GUN,
	IDLE_BAG01,
	IDLE_BAG02,

	MOVE_BODY,
	MOVE_HEAD,
	MOVE_LARM,
	MOVE_RARM,
	MOVE_LHAND,
	MOVE_RHAND,
	MOVE_LLEG,
	MOVE_RLEG,
	MOVE_LFOOT,
	MOVE_RFOOT,
	MOVE_GUN,
	MOVE_BAG01,
	MOVE_BAG02,

	ATK_BODY,
	ATK_HEAD,
	ATK_LARM,
	ATK_RARM,
	ATK_LHAND,
	ATK_RHAND,
	ATK_LLEG,
	ATK_RLEG,
	ATK_LFOOT,
	ATK_RFOOT,
	ATK_GUN,
	ATK_BAG01,
	ATK_BAG02,

	JUMP_BODY,
	JUMP_HEAD,
	JUMP_LARM,
	JUMP_RARM,
	JUMP_LHAND,
	JUMP_RHAND,
	JUMP_LLEG,
	JUMP_RLEG,
	JUMP_LFOOT,
	JUMP_RFOOT,
	JUMP_GUN,
	JUMP_BAG01,
	JUMP_BAG02,

	ANIM_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

static float		roty = 0.0f;

static LIGHT		g_Light;

float				g_jumpPower;

// �A�j���[�V�����̃f�B���N�g���̔z��
static char* g_AnimDir[] =
{
	ANIM_IDLE_BODY,
	ANIM_IDLE_HEAD,
	ANIM_IDLE_LARM,
	ANIM_IDLE_RARM,
	ANIM_IDLE_LHAND,
	ANIM_IDLE_RHAND,
	ANIM_IDLE_LLEG,
	ANIM_IDLE_RLEG,
	ANIM_IDLE_LFOOT,
	ANIM_IDLE_RFOOT,
	ANIM_IDLE_GUN,
	ANIM_IDLE_BAG01,
	ANIM_IDLE_BAG02,

	ANIM_MOVE_BODY,
	ANIM_MOVE_HEAD,
	ANIM_MOVE_LARM,
	ANIM_MOVE_RARM,
	ANIM_MOVE_LHAND,
	ANIM_MOVE_RHAND,
	ANIM_MOVE_LLEG,
	ANIM_MOVE_RLEG,
	ANIM_MOVE_LFOOT,
	ANIM_MOVE_RFOOT,
	ANIM_MOVE_GUN,
	ANIM_MOVE_BAG01,
	ANIM_MOVE_BAG02,

	ANIM_ATK_BODY,
	ANIM_ATK_HEAD,
	ANIM_ATK_LARM,
	ANIM_ATK_RARM,
	ANIM_ATK_LHAND,
	ANIM_ATK_RHAND,
	ANIM_ATK_LLEG,
	ANIM_ATK_RLEG,
	ANIM_ATK_LFOOT,
	ANIM_ATK_RFOOT,
	ANIM_ATK_GUN,
	ANIM_ATK_BAG01,
	ANIM_ATK_BAG02,

	ANIM_JUMP_BODY,
	ANIM_JUMP_HEAD,
	ANIM_JUMP_LARM,
	ANIM_JUMP_RARM,
	ANIM_JUMP_LHAND,
	ANIM_JUMP_RHAND,
	ANIM_JUMP_LLEG,
	ANIM_JUMP_RLEG,
	ANIM_JUMP_LFOOT,
	ANIM_JUMP_RFOOT,
	ANIM_JUMP_GUN,
	ANIM_JUMP_BAG01,
	ANIM_JUMP_BAG02,

};

// �v���C���[�̊K�w�A�j���[�V�����f�[�^
// 
static std::vector<INTERPOLATION_DATA> idle_body;
static std::vector<INTERPOLATION_DATA> idle_head;
static std::vector<INTERPOLATION_DATA> idle_arml;
static std::vector<INTERPOLATION_DATA> idle_armr;
static std::vector<INTERPOLATION_DATA> idle_handl;
static std::vector<INTERPOLATION_DATA> idle_handr;
static std::vector<INTERPOLATION_DATA> idle_legl;
static std::vector<INTERPOLATION_DATA> idle_legr;
static std::vector<INTERPOLATION_DATA> idle_footl;
static std::vector<INTERPOLATION_DATA> idle_footr;
static std::vector<INTERPOLATION_DATA> idle_gun;
static std::vector<INTERPOLATION_DATA> idle_bag01;
static std::vector<INTERPOLATION_DATA> idle_bag02;

static std::vector<INTERPOLATION_DATA> move_body;
static std::vector<INTERPOLATION_DATA> move_head;
static std::vector<INTERPOLATION_DATA> move_arml;
static std::vector<INTERPOLATION_DATA> move_armr;
static std::vector<INTERPOLATION_DATA> move_handl;
static std::vector<INTERPOLATION_DATA> move_handr;
static std::vector<INTERPOLATION_DATA> move_legl;
static std::vector<INTERPOLATION_DATA> move_legr;
static std::vector<INTERPOLATION_DATA> move_footl;
static std::vector<INTERPOLATION_DATA> move_footr;
static std::vector<INTERPOLATION_DATA> move_gun;
static std::vector<INTERPOLATION_DATA> move_bag01;
static std::vector<INTERPOLATION_DATA> move_bag02;

static std::vector<INTERPOLATION_DATA> atk_body;
static std::vector<INTERPOLATION_DATA> atk_head;
static std::vector<INTERPOLATION_DATA> atk_arml;
static std::vector<INTERPOLATION_DATA> atk_armr;
static std::vector<INTERPOLATION_DATA> atk_handl;
static std::vector<INTERPOLATION_DATA> atk_handr;
static std::vector<INTERPOLATION_DATA> atk_legl;
static std::vector<INTERPOLATION_DATA> atk_legr;
static std::vector<INTERPOLATION_DATA> atk_footl;
static std::vector<INTERPOLATION_DATA> atk_footr;
static std::vector<INTERPOLATION_DATA> atk_gun;
static std::vector<INTERPOLATION_DATA> atk_bag01;
static std::vector<INTERPOLATION_DATA> atk_bag02;

static std::vector<INTERPOLATION_DATA> jump_body;
static std::vector<INTERPOLATION_DATA> jump_head;
static std::vector<INTERPOLATION_DATA> jump_arml;
static std::vector<INTERPOLATION_DATA> jump_armr;
static std::vector<INTERPOLATION_DATA> jump_handl;
static std::vector<INTERPOLATION_DATA> jump_handr;
static std::vector<INTERPOLATION_DATA> jump_legl;
static std::vector<INTERPOLATION_DATA> jump_legr;
static std::vector<INTERPOLATION_DATA> jump_footl;
static std::vector<INTERPOLATION_DATA> jump_footr;
static std::vector<INTERPOLATION_DATA> jump_gun;
static std::vector<INTERPOLATION_DATA> jump_bag01;
static std::vector<INTERPOLATION_DATA> jump_bag02;

static std::vector<INTERPOLATION_DATA>* g_TblAdr[] =
{
	&idle_body,
	& idle_head,
	& idle_arml,
	& idle_armr,
	& idle_handl,
	& idle_handr,
	& idle_legl,
	& idle_legr,
	& idle_footl,
	& idle_footr,
	& idle_gun,
	& idle_bag01,
	& idle_bag02,

	& move_body,
	& move_head,
	& move_arml,
	& move_armr,
	& move_handl,
	& move_handr,
	& move_legl,
	& move_legr,
	& move_footl,
	& move_footr,
	& move_gun,
	& move_bag01,
	& move_bag02,

	& atk_body,
	& atk_head,
	& atk_arml,
	& atk_armr,
	& atk_handl,
	& atk_handr,
	& atk_legl,
	& atk_legr,
	& atk_footl,
	& atk_footr,
	& atk_gun,
	& atk_bag01,
	& atk_bag02,

	& jump_body,
	& jump_head,
	& jump_arml,
	& jump_armr,
	& jump_handl,
	& jump_handr,
	& jump_legl,
	& jump_legr,
	& jump_footl,
	& jump_footr,
	& jump_gun,
	& jump_bag01,
	& jump_bag02,

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	// �A�j���[�V������SCV�t�@�C������
	for (int i = 0; i < ANIM_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}

	g_Player.load = TRUE;
	//LoadModel(MODEL_PLAYER, &g_Player.model);

	g_Player.pos = XMFLOAT3(-10.0f, PLAYER_OFFSET_Y+50.0f, -50.0f);
	g_Player.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Player.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A

	g_Player.inviCnt = 0;
	g_Player.hp = PLAYER_LIFE;		// �v���C���[��HP�̐ݒ�
	g_Player.use = TRUE;			// TRUE:�����Ă�
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �L�[�����������̃v���C���[�̌���
	roty = 0.0f;

	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������


	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = FALSE;
	}

	g_Parts[0].use = TRUE;
	//g_Parts[0].parent = &g_Player;	// �e���Z�b�g
	g_Parts[0].tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0].tblMax = (int)g_TblAdr[g_Parts[0].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[0].load = 1;
	LoadModel(MODEL_PLAYER, &g_Parts[0].model);

	g_Parts[1].use = TRUE;
	//g_Parts[1].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[1].tblNo = 1;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[1].tblMax = (int)g_TblAdr[g_Parts[1].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[1].load = 1;
	LoadModel(MODEL_PLAYER_HEAD, &g_Parts[1].model);

	g_Parts[2].use = TRUE;
	//g_Parts[2].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[2].tblNo = 2;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[2].tblMax = (int)g_TblAdr[g_Parts[2].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[2].load = 1;
	LoadModel(MODEL_PLAYER_LARM, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	//g_Parts[3].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[3].tblNo = 3;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[3].tblMax = (int)g_TblAdr[g_Parts[3].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[3].load = 1;
	LoadModel(MODEL_PLAYER_RARM, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	//g_Parts[4].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[4].tblNo = 4;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[4].tblMax = (int)g_TblAdr[g_Parts[4].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[4].load = 1;
	LoadModel(MODEL_PLAYER_LHAND, &g_Parts[4].model);

	g_Parts[5].use = TRUE;
	//g_Parts[5].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[5].tblNo = 5;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[5].tblMax = (int)g_TblAdr[g_Parts[5].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[5].load = 1;
	LoadModel(MODEL_PLAYER_RHAND, &g_Parts[5].model);

	g_Parts[6].use = TRUE;
	//g_Parts[6].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[6].tblNo = 6;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[6].tblMax = (int)g_TblAdr[g_Parts[6].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[6].load = 1;
	LoadModel(MODEL_PLAYER_LLEG, &g_Parts[6].model);

	g_Parts[7].use = TRUE;
	//g_Parts[7].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[7].tblNo = 7;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[7].tblMax = (int)g_TblAdr[g_Parts[7].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[7].load = 1;
	LoadModel(MODEL_PLAYER_RLEG, &g_Parts[7].model);

	g_Parts[8].use = TRUE;
	//g_Parts[8].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[8].tblNo = 8;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[8].tblMax = (int)g_TblAdr[g_Parts[8].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[8].load = 1;
	LoadModel(MODEL_PLAYER_RFOOT, &g_Parts[8].model);

	g_Parts[9].use = TRUE;
	//g_Parts[9].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[9].tblNo = 9;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[9].tblMax = (int)g_TblAdr[g_Parts[9].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[9].load = 1;
	LoadModel(MODEL_PLAYER_RFOOT, &g_Parts[9].model);

	g_Parts[10].use = TRUE;
	//g_Parts[10].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[10].tblNo = 10;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[10].tblMax = (int)g_TblAdr[g_Parts[10].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[10].load = 1;
	LoadModel(MODEL_PLAYER_GUN, &g_Parts[10].model);

	g_Parts[11].use = TRUE;
	//g_Parts[11].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[11].tblNo = 11;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Parts[11].tblMax = sizeof(move_tbl_rleg) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[11].tblMax = (int)g_TblAdr[g_Parts[11].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[11].load = 1;
	LoadModel(MODEL_PLAYER_BAG01, &g_Parts[11].model);

	g_Parts[12].use = TRUE;
	//g_Parts[12].parent = &g_Parts[0];	// �e���Z�b�g
	g_Parts[12].tblNo = 12;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Parts[12].tblMax = sizeof(move_tbl_rleg) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[12].tblMax = (int)g_TblAdr[g_Parts[12].tblNo]->size();	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[12].load = 1;
	LoadModel(MODEL_PLAYER_BAG02, &g_Parts[12].model);


	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load == TRUE)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	// �p�[�c�̉������
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	// �v���C���[���J�����Ɍ�����
	XMVECTOR CameraAt = XMLoadFloat3(&cam->at);
	XMVECTOR PlayerPos = XMLoadFloat3(&g_Player.pos);
	XMVECTOR vec = PlayerPos - CameraAt;

	float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]) ;
	
	g_Player.spd *= 0.7f;

	// �����蔻��̖��G����
	if (g_Player.inviCnt > 0)
	{
		g_Player.inviCnt--;
	}


	// �ړ�����
	if (g_Player.state != PLAYER_STATE_JUMP)
	{
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, STICK_L_LEFT))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer - XM_PIDIV2;
			g_Player.state = PLAYER_STATE_MOVE;
		}
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, STICK_L_RIGHT))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer + XM_PIDIV2;
			g_Player.state = PLAYER_STATE_MOVE;
		}
		if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, STICK_L_UP))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer;
			g_Player.state = PLAYER_STATE_MOVE;
		}
		if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, STICK_L_DOWN))
		{
			g_Player.spd = VALUE_MOVE;
			roty = anglePlayer + XM_PI;
			g_Player.state = PLAYER_STATE_MOVE;
		}
	}


	switch (g_Player.state)
	{
	case PLAYER_STATE_IDLE:
		SetTblIdle();
		break;

	case PLAYER_STATE_MOVE:
		SetTblMove();
		break;

	case PLAYER_STATE_ATK:
		SetTblAtk();
		AttackPlayer();
		break;

	case PLAYER_STATE_JUMP:
		SetTblJump();
		JumpPlayer();
		break;
	}


#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.spd = 0.0f;
		roty = 0.0f;
	}
#endif


	{	// �����������Ƀv���C���[���ړ�������
		// �����������Ƀv���C���[���������Ă��鏊

		g_Player.rot.y = anglePlayer;

		g_Player.pos.x -= sinf(roty) * g_Player.spd;
		g_Player.pos.z -= cosf(roty) * g_Player.spd;
	}


	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 HitPosition;		// ��_
	XMFLOAT3 Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);			// �Ԃ������|���S���̖@���x�N�g���i�����j
	if (g_Player.jump == FALSE)
	{
		BOOL ans = RayHitField(g_Player.pos, &HitPosition, &Normal);
		if (ans)
		{
			g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
		}
		else
		{
			g_Player.pos.y = PLAYER_OFFSET_Y;
			Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y =  0.1f;
	SetPositionShadow(g_Player.shadowIdx, pos);



	// �W�����v����
	if (g_Player.state != PLAYER_STATE_ATK)
	{
		if ((GetKeyboardTrigger(DIK_SPACE)) && (g_Player.jump == FALSE) || IsButtonTriggered(0, BUTTON_A) && (g_Player.jump == FALSE))
		{
			g_Player.state = PLAYER_STATE_JUMP;
			g_Player.jump = TRUE;
			g_Player.jumpCnt = 0;

			for (int i = 0; i < PLAYER_PARTS_MAX; i++)
			{
				g_Parts[i].time = 0.0f;
			}
		}
	}


	// �e���ˏ���
	if (g_Player.state != PLAYER_STATE_JUMP)
	{
		//if ((GetKeyboardTrigger(DIK_X) && (g_Player.atk == FALSE) || IsButtonTriggered(0, BUTTON_R)) && (g_Player.atk == FALSE))
		if ((GetKeyboardTrigger(DIK_X)  || IsButtonTriggered(0, BUTTON_R)) )
		{

			PlaySound(SOUND_LABEL_SE_Charge02);

			g_Player.state = PLAYER_STATE_ATK;
			g_Player.atk = TRUE;
			g_Player.gunCnt = 0;


			for (int i = 0; i < PLAYER_PARTS_MAX; i++)
			{
				g_Parts[i].time = 0.0f;
			}
		}
	}


	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	// ���`��Ԃ̏���
			int nowNo = (int)g_Parts[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Parts[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			std::vector<INTERPOLATION_DATA>tbl = *g_TblAdr[g_Parts[i].tblNo];

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

			float nowTime = g_Parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frame���g�Ď��Ԍo�ߏ���������
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
			if ((int)g_Parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				g_Parts[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
			
				switch (g_Player.setTbl)
				{
				// �A�j���C�V�������I�������IDLE�ɖ߂�
				case PLAYER_STATE_ATK:
					g_Parts[i].tblMax = -1;
					g_Player.state = PLAYER_STATE_IDLE;
					break;

				case PLAYER_STATE_JUMP:
					g_Parts[i].tblMax = -1;
					g_Player.state = PLAYER_STATE_IDLE;
					break;

				}

			}

		}

	}

	// �|�C���g���C�g�̃e�X�g
	{
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}

	if ((g_Player.state != PLAYER_STATE_ATK) && (g_Player.state != PLAYER_STATE_JUMP))
	{
		g_Player.state = PLAYER_STATE_IDLE;
	}


	//////////////////////////////////////////////////////////////////////
	// �p������
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

//	quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);




#ifdef _DEBUG

	if (GetKeyboardPress(DIK_1))
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

	// �f�o�b�O�\��
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{



	if (g_Player.use == FALSE) return;



	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	//// �����̐ݒ�
	//SetFuchi(1);

	// ���f���`��
	//DrawModel(&g_Player.model);



	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
																			// ��
																			// g_Player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == FALSE) continue;

		for (int j= 0; j < g_Parts[i].model.SubsetNum;j++)
		{
			if (g_Player.inviCnt > 0)
			{
				SetModelDiffuse(&g_Parts[i].model, j, XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f));
			}
			else
			{
				SetModelDiffuse(&g_Parts[i].model, j, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			}
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);


		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

void AttackPlayer(void)
{
	
	// ��ԑJ��
	{
		BOOL ans = TRUE;

		// �S�Ẵp�[�c�̍ő�e�[�u����-1�ɂȂ��Ă���ꍇ(���A�j���[�V�������I�����Ă���ꍇ)�A��ԑJ�ڂ�����
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].tblMax != -1) ans = FALSE;

		}

		if (ans == TRUE)
		{
			g_Player.state = PLAYER_STATE_IDLE;
		}
	}


	{
		g_Player.gunCnt++;

		XMFLOAT3 pos = g_Player.pos;
		pos.x -= sinf(g_Player.rot.y - XM_PIDIV2);
		pos.z -= cosf(g_Player.rot.y - XM_PIDIV2);

		pos.x -= sinf(g_Player.rot.y) * 27.5f;
		pos.z -= cosf(g_Player.rot.y) * 27.5f;

		XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);


		if (g_Player.gunCnt < 34 && g_Player.gunCnt > 16)
		{

			for (int i = 0; i < 30; i++)
			{
				move.y = (rand() % 3 - 1) * 0.1f;
				move.z = (rand() % 3 - 1) * 0.1f;
				move.x = (rand() % 3 - 1) * 0.1f;

				SetEffect(pos, move, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 30, 1);
				
				PlaySound(SOUND_LABEL_SE_Shot01);

			}

		}

		else if (g_Player.gunCnt > 34)
		{
			move = XMFLOAT3(-sinf(g_Player.rot.y) * 5.0f, 0.0f, -cosf(g_Player.rot.y) * 5.0f);
			SetParticle(pos, move, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 60);
			g_Player.atk = FALSE;

		}


	}


}


void JumpPlayer(void)
{

	// ��ԑJ��
	{
		BOOL ans = TRUE;

		// �S�Ẵp�[�c�̍ő�e�[�u����-1�ɂȂ��Ă���ꍇ(���A�j���[�V�������I�����Ă���ꍇ)�A��ԑJ�ڂ�����
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].tblMax != -1) ans = FALSE;

		}

		if (ans == TRUE) g_Player.state = PLAYER_STATE_IDLE;
	}


	{
		

		g_Player.jumpCnt++;

		XMFLOAT3 pos = g_Player.pos;

		CAMERA* cam = GetCamera();

		// �v���C���[���J�����Ɍ�����
		XMVECTOR CameraAt = XMLoadFloat3(&cam->at);
		XMVECTOR PlayerPos = XMLoadFloat3(&g_Player.pos);
		XMVECTOR vec = PlayerPos - CameraAt;

		float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]);

		g_jumpPower = 5.0f;

		if (g_Player.jumpCnt > 14 && g_Player.jumpCnt < 54)
		{
			if (g_Player.jump == TRUE)
			{
				//g_Player.pos.y += g_jumpPower;
				g_Player.spd = 2.0f*VALUE_MOVE;
				roty = anglePlayer;
				



				if (g_jumpPower >= 5.0f)
				{
					g_jumpPower -= 1.0f;
				}
				else
				{
					g_jumpPower = 0.0f;
				}

				// �p�[�e�B�N���̔����ӏ���ݒ�(���W�F�b�g�p�b�N�̍��W���擾)
				XMFLOAT3 rPos = { g_Parts[BAG02].mtxWorld._41, g_Parts[BAG02].mtxWorld._42, g_Parts[BAG02].mtxWorld._43 };
				XMFLOAT3 lPos = rPos;

				// �E���i���v���C���[�̌����ɍ��킹�ĉE�ɂ��炷�j
				rPos.x -= sinf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				rPos.z -= cosf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				rPos.y -= 5.0f;

				// �����i���v���C���[�̌����ɍ��킹�č��ɂ��炷�j
				lPos.x -= sinf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				lPos.z -= cosf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				lPos.y -= 5.0f;

				// �p�[�e�B�N���̔���
				SetParticle(rPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 0.47f, 0.0f, 1.0f), 30);
				SetParticle(rPos, XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20);
				SetParticle(lPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 0.47f, 0.0f, 1.0f), 30);
				SetParticle(lPos, XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20);
				//SetParticle(lPos, XMFLOAT3(0.0f, -0.05f,  0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, 0.0f, 30);

			}
		}
		else if (g_Player.jumpCnt >= 54)
		{
			if (g_Player.jump == TRUE)
			{

				//g_Player.pos.y -= g_jumpPower;
				if (g_jumpPower < 5.0f)
				{
					g_jumpPower += 1.0f;
				}
				if (g_Player.pos.y < PLAYER_OFFSET_Y)
				{
					g_Player.pos.y = PLAYER_OFFSET_Y;
					g_Player.jump = FALSE;
					g_Player.state = PLAYER_STATE_IDLE;
				}


				// �p�[�e�B�N���̔����ӏ���ݒ�(���W�F�b�g�p�b�N�̍��W���擾)
				XMFLOAT3 rPos = { g_Parts[RFOOT].mtxWorld._41, g_Parts[RFOOT].mtxWorld._42, g_Parts[RFOOT].mtxWorld._43 };
				XMFLOAT3 lPos = { g_Parts[LFOOT].mtxWorld._41, g_Parts[LFOOT].mtxWorld._42, g_Parts[LFOOT].mtxWorld._43 };

				//// �E���i���v���C���[�̌����ɍ��킹�ĉE�ɂ��炷�j
				//rPos.x -= sinf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				//rPos.z -= cosf(g_Player.rot.y + XM_PIDIV2) * 7.0f;
				//rPos.y -= 5.0f;

				//// �����i���v���C���[�̌����ɍ��킹�č��ɂ��炷�j
				//lPos.x -= sinf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				//lPos.z -= cosf(g_Player.rot.y - XM_PIDIV2) * 7.0f;
				//lPos.y -= 5.0f;

				// �p�[�e�B�N���̔���
				//SetParticle(rPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 60);
				//SetParticle(lPos, XMFLOAT3(0.25f, 0.25f, 0.25f), XMFLOAT3(0.0f, -0.05f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 600);
				SetParticleLerp(rPos, lPos, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 60);
			}

		}
	}
}


void SetTblIdle(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_IDLE:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + IDLE_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_IDLE;
		break;
	}
}

void SetTblMove(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_MOVE:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + MOVE_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_MOVE;
		break;
	}
}


void SetTblAtk(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_ATK:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + ATK_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_ATK;
		break;
	}

}

void SetTblJump(void)
{
	switch (g_Player.setTbl)
	{
	case PLAYER_STATE_JUMP:
		break;
	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[i].use == FALSE)continue;

			g_Parts[i].time = 0.0f;
			g_Parts[i].tblNo = i + JUMP_BODY;
			g_Parts[i].tblMax = (int)g_TblAdr[g_Parts[i].tblNo]->size();

		}

		g_Player.setTbl = PLAYER_STATE_JUMP;
		break;
	}

}
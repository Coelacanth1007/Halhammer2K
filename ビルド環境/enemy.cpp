//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "orc.h"
#include "shadow.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/drone.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��

#define COUNT_MAX			(200)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// �G�l�~�[

int g_Enemy_load = 0;

BOOL					g_load;
DX11_MODEL				g_model;				// ���f�����
XMFLOAT4				g_diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F



static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame

	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(400.0f, ENEMY_OFFSET_Y + 20.0f,  0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },
	{ XMFLOAT3(400.0f, ENEMY_OFFSET_Y,  400.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },

	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y + 20.0f,400.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },
	{ XMFLOAT3(-400.0f, ENEMY_OFFSET_Y,  400.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },

	{ XMFLOAT3(-400.0f, ENEMY_OFFSET_Y,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },
	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y + 20.0f,400.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 8 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
};




//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{

	LoadModel(MODEL_ENEMY, &g_model);
	g_load = TRUE;

	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_model, &g_diffuse[0]);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 7.0f, 20.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		
		g_Enemy[i].time = 0.1f * i;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Enemy[i].use = TRUE;			// TRUE:�����Ă�

		g_Enemy[i].moveCnt = 0;

	}

	 srand((unsigned int)time(NULL));

	//// �G�l�~�[�����`��Ԃœ������Ă݂�
	//g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (g_load==TRUE)
	{
		UnloadModel(&g_model);
		g_load = FALSE;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{


	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes

			float speedTime = 0.5f;		// �G�l�~�[�̈ړ��{��

			XMFLOAT3 oldPos = g_Enemy[i].pos;
			// drone�̃����_���ړ�����
			g_Enemy[i].pos.x -= sinf(g_Enemy[i].rot.y) * speedTime;
			g_Enemy[i].pos.z += cosf(g_Enemy[i].rot.y) * speedTime;


			g_Enemy[i].moveCnt++;

			if (g_Enemy[i].moveCnt >= COUNT_MAX)
			{
				g_Enemy[i].rot.y = rand() % (int)(XM_2PI * 100) * 0.01f - XM_PI;
				g_Enemy[i].moveCnt = 0;
			}

			// �ǂɓ���������n�}�̒��S�ɖ߂�
			if (g_Enemy[i].pos.x < MAP_LEFT || g_Enemy[i].pos.x > MAP_RIGHT ||
				g_Enemy[i].pos.z < MAP_DOWN || g_Enemy[i].pos.z > MAP_TOP)
			{
				g_Enemy[i].pos = oldPos;

				g_Enemy[i].rot.y += XM_PI;
			}

			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}




#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// ���f���̐F��ύX�ł����I�������ɂ��ł����B
		for (int j = 0; j < g_model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_model, j, g_diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}

void CreateEnemy(void)
{
	ORC* orc = GetOrc();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE) continue;

		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		switch (i)
		{
		case 0:
			pos = XMFLOAT3(orc->pos.x - 50.0f, ENEMY_OFFSET_Y, orc->pos.z - 50.0f);

			break;
		case 1:
			pos = XMFLOAT3(orc->pos.x - 100.0f, ENEMY_OFFSET_Y, orc->pos.z - 100.0f);
			break;

		case 2:
			pos = XMFLOAT3(orc->pos.x + 50.0f, ENEMY_OFFSET_Y, orc->pos.z + 50.0f);
			break;

		case 3:
			pos = XMFLOAT3(orc->pos.x + 100.0f, ENEMY_OFFSET_Y, orc->pos.z + 100.0f);
			break;

		case 4:
			pos = XMFLOAT3(orc->pos.x , ENEMY_OFFSET_Y, orc->pos.z + 100.0f);
			break;

		}

		g_Enemy[i].moveCnt = 0;
		g_Enemy[i].pos = pos;
		g_Enemy[i].shadowIdx = CreateShadow(g_Enemy[i].pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		g_Enemy[i].use = TRUE;
	}

}


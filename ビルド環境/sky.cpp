//=============================================================================
//
// �󏈗� [sky.cpp]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "sky.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_SKY		"data/MODEL/sky.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define SKY_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define SKY_OFFSET_Y		(7.0f)							// �v���C���[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static SKY				g_Sky;						// �v���C���[

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSky(void)
{
	LoadModel(MODEL_SKY, &g_Sky.model);
	g_Sky.load = true;

	g_Sky.pos = { 0.0f, 0.0f, 0.0f };
	g_Sky.rot = { 0.0f, 0.0f, 0.0f };
	g_Sky.scl = { 5.0f, 5.0f, 5.0f };

	g_Sky.spd  = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Sky.size = SKY_SIZE;	// �����蔻��̑傫��

	g_Sky.use = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSky(void)
{
	// ���f���̉������
	if (g_Sky.load)
	{
		UnloadModel(&g_Sky.model);
		g_Sky.load = false;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSky(void)
{
	CAMERA *cam = GetCamera();

	g_Sky.rot.y += 0.001f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSky(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Sky.scl.x, g_Sky.scl.y, g_Sky.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Sky.rot.x, g_Sky.rot.y + XM_PI, g_Sky.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Sky.pos.x, g_Sky.pos.y, g_Sky.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Sky.mtxWorld, mtxWorld);


	// ���f���`��
	DrawModel(&g_Sky.model);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_NONE);

	SetLightEnable(TRUE);

}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
SKY *GetSky(void)
{
	return &g_Sky;
}


//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "orc.h"
#include "enemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "bullet.h"
#include "score.h"
#include "particle.h"
#include "collision.h"
#include "debugproc.h"
#include "sky.h"
#include "particle_lerp.h"
#include "ui.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 50, 50, 100.0f, 100.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����

	InitPlayer();

	// �I�[�N�̏�����
	InitOrc();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// ��̏�����
	InitSky();

	// �؂𐶂₷
	InitTree();

	// �e�̏�����
	InitBullet();

	// �X�R�A�̏�����
	InitScore();

	// �p�[�e�B�N���̏�����
	InitParticle();
	InitParticleLerp();

	InitUi();

	// BGM�Đ�
	//PlaySound(SOUND_LABEL_BGM_sample001);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �p�[�e�B�N���̏I������
	UninitParticleLerp();
	UninitParticle();

	// �X�R�A�̏I������
	UninitScore();

	// �e�̏I������
	UninitBullet();

	// �؂̏I������
	UninitTree();

	// ��̏I������
	UninitSky();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �I�[�N�̏I������
	UninitOrc();

	// �v���C���[�̏I������
	UninitPlayer();

	UninitUi();

	// �e�̏I������
	UninitShadow();

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �I�[�N�̍X�V����
	UpdateOrc();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	// ��̍X�V����
	UpdateSky();

	// �؂̍X�V����
	UpdateTree();

	// �e�̍X�V����
	UpdateBullet();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();
	UpdateParticleLerp();

	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit();

	// �X�R�A�̍X�V����
	UpdateScore();

	UpdateUi();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	// 3D�̕���`�悷�鏈��
	
	// ��̕`�揈��
	DrawSky();

	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// �G�l�~�[�̕`�揈��
	DrawOrc();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawBullet();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �؂̕`�揈��
	DrawTree();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();
	DrawParticleLerp();


	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �X�R�A�̕`�揈��
	DrawScore();

	DrawUi();

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayer()->pos;
	//pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY* enemy		 = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
	ORC* orc			 = GetOrc();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER* player		 = GetPlayer();	// �v���C���[�̃|�C���^�[��������
	BULLET* bullet		 = GetBullet();	// �e�̃|�C���^�[��������
	PARTICLE* particle	 = GetParticle(); //�p�[�e�B�N���̃|�C���^�[��������

	int enemy_count = 0;
	int orc_count = 0;

	int time = 0;
	// �G�ƃp�[�e�B�N��
	for (int i = 0; i < MAX_ENEMY; i++)
	{

		// �v���C���[������ł��玟�̓z��
		if (enemy[i].use == FALSE) continue;

		enemy_count++;

		for (int j = 0; j < MAX_PARTICLE; j++)
		{
			//�G������ł��玟�̓z��
			if (particle[j].use == FALSE) continue;

			//BC�̓����蔻��
			if (CollisionBC(enemy[i].pos, particle[j].pos, enemy[i].size, 10.0f))
			{

				// �G�L�����N�^�[�͓|�����
				enemy[i].use = FALSE;
				ReleaseShadow(enemy[i].shadowIdx);



				// �X�R�A�𑫂�
				AddScore(100);

			}
		}
	}

	// �G�l�~�[���S�ł�����I�[�N���U��������Ԃɓ���

	if (enemy_count == 0)
	{
		orc[0].attackable = TRUE;
	}


	// �I�[�N�ƃp�[�e�B�N��
	for (int i = 0; i < MAX_ORC; i++)
	{

		if (orc[0].use == FALSE) continue;

		orc_count++;

		if (orc[0].attackable == TRUE)
		{

			for (int j = 0; j < MAX_PARTICLE; j++)
			{
				//�G������ł��玟�̓z��
				if (particle[j].use == FALSE) continue;

				//BC�̓����蔻��
				if (CollisionBC(orc[i].pos, particle[j].pos, orc[i].size, 10.0f))
				{

					orc[i].hp--;

					if (orc[i].hp <= 0)
					{
						// �G�L�����N�^�[�͓|�����
						orc[i].use = FALSE;
						ReleaseShadow(orc[i].shadowIdx);

						// �X�R�A�𑫂�
						AddScore(100);
					}

				}

			}

		}

	}

	// �I�[�N�����񂾂烊�U���g��ʂɑJ��
		if (orc_count <= 0)
		{
			//time++;
			//if (time >= 60)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}
		}



		//BC�̓����蔻��
		if (CollisionBC(orc[0].pos, player[0].pos, orc[0].size, 10.0f) )
		{
			if (player[0].inviCnt == 0)
			{
				player[0].inviCnt = INVI_TIME;

				player[0].hp -= 1;


				if (player[0].hp <= 0)
				{

					// �v���C���[���|���Ƃ�
					player[0].use = FALSE;
					ReleaseShadow(player[0].shadowIdx);

					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
		}
		


}



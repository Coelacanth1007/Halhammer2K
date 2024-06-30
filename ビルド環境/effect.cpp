//=============================================================================
//
// �p�[�e�B�N������ [effect.cpp]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "light.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(2)			// �e�N�X�`���̐�

#define	EFFECT_SIZE_X		(20.0f)		// ���_�T�C�Y
#define	EFFECT_SIZE_Y		(20.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_EFFECT	(5.0f)		// �ړ����x

#define	MAX_EFFECT		(512)		// �p�[�e�B�N���ő吔

//#define	DISP_SHADOW						// �e�̕\��
#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nIdxShadow;		// �eID
	int				nLife;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���
	int				type;

} EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEffect(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;		// ���_�o�b�t�@

static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;					// �e�N�X�`���ԍ�

static EFFECT					g_aEffect[MAX_EFFECT];		// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;						// �r���{�[�h�����ʒu
static float					g_fWidthBase = 5.0f;			// ��̕�
static float					g_fHeightBase = 10.0f;			// ��̍���
static float					g_roty = 0.0f;					// �ړ�����
static float					g_spd = 0.0f;					// �ړ��X�s�[�h

static char *g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
	"data/TEXTURE/effect001.png",
};

static BOOL							g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
{
	// ���_���̍쐬
	MakeVertexEffect();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// �p�[�e�B�N�����[�N�̏�����
	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aEffect[nCntEffect].material, sizeof(g_aEffect[nCntEffect].material));
		g_aEffect[nCntEffect].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aEffect[nCntEffect].fSizeX = EFFECT_SIZE_X;
		g_aEffect[nCntEffect].fSizeY = EFFECT_SIZE_Y;
		g_aEffect[nCntEffect].nIdxShadow = -1;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].use = FALSE;
	}

	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	//PLAYER *pPlayer = GetPlayer();
	//g_posBase = pPlayer->pos;

	{
		for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
		{
			if(g_aEffect[nCntEffect].use)
			{// �g�p��
				switch (g_aEffect[nCntEffect].type)
				{
				case 0 :
					g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
					g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
					g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

					//g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
					//if(g_aEffect[nCntEffect].pos.y <= g_aEffect[nCntEffect].fSizeY / 2)
					//{// ���n����
					//	g_aEffect[nCntEffect].pos.y = g_aEffect[nCntEffect].fSizeY / 2;
					//	g_aEffect[nCntEffect].move.y = -g_aEffect[nCntEffect].move.y * 0.75f;
					//}

					g_aEffect[nCntEffect].move.x += (0.0f - g_aEffect[nCntEffect].move.x) * 0.015f;
					g_aEffect[nCntEffect].move.y += (0.0f - g_aEffect[nCntEffect].move.y) * 0.015f;
					//g_aEffect[nCntEffect].move.y -= 0.25f;
					g_aEffect[nCntEffect].move.z += (0.0f - g_aEffect[nCntEffect].move.z) * 0.015f;

					break;
				case 1 :
					g_aEffect[nCntEffect].scale.x += g_aEffect[nCntEffect].scale.x * 0.01f;
					g_aEffect[nCntEffect].scale.y += g_aEffect[nCntEffect].scale.y * 0.01f;
					g_aEffect[nCntEffect].scale.z += g_aEffect[nCntEffect].scale.z * 0.01f;

					break;
				}

#ifdef DISP_SHADOW
				if(g_aEffect[nCntEffect].nIdxShadow != -1)
				{// �e�g�p��
					float colA;

					// �e�̈ʒu�ݒ�
					SetPositionShadow(g_aEffect[nCntEffect].nIdxShadow, XMFLOAT3(g_aEffect[nCntEffect].pos.x, 0.1f, g_aEffect[nCntEffect].pos.z));

					// �e�̐F�̐ݒ�
					colA = g_aEffect[nCntEffect].material.Diffuse.w;
					SetColorShadow(g_aEffect[nCntEffect].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
				}
#endif

				g_aEffect[nCntEffect].nLife--;
				if(g_aEffect[nCntEffect].nLife <= 0)
				{
					g_aEffect[nCntEffect].use = FALSE;
					ReleaseShadow(g_aEffect[nCntEffect].nIdxShadow);
					g_aEffect[nCntEffect].nIdxShadow = -1;
				}
				else
				{
					if(g_aEffect[nCntEffect].nLife <= 80)
					{
						g_aEffect[nCntEffect].material.Diffuse.x = 0.0f - (float)(80 - g_aEffect[nCntEffect].nLife) / 80 * 0.8f;
						g_aEffect[nCntEffect].material.Diffuse.y = 0.7f - (float)(80 - g_aEffect[nCntEffect].nLife) / 80 * 0.7f;
						g_aEffect[nCntEffect].material.Diffuse.z = 0.7f - (float)(80 - g_aEffect[nCntEffect].nLife) / 80 * 0.2f;
					}

					if(g_aEffect[nCntEffect].nLife <= 20)
					{
						// ���l�ݒ�
						g_aEffect[nCntEffect].material.Diffuse.w -= 0.05f;
						if(g_aEffect[nCntEffect].material.Diffuse.w < 0.0f)
						{
							g_aEffect[nCntEffect].material.Diffuse.w = 0.0f;
						}
					}
				}
			}
		}

		// �p�[�e�B�N������
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200 ) / 100.0f - g_fWidthBase;
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 300 / 100.0f + g_fHeightBase;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 100 + 150;  

			fSize = (float)(rand() % 30 + 20);

			pos.y = fSize / 2;

			// �r���{�[�h�̐ݒ�
			//SetEffect(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// ���Z�����ɐݒ�
	SetBlendState(BLEND_MODE_ADD);

	// Z��r����
	SetDepthEnable(FALSE);

	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(g_aEffect[nCntEffect].use)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_aEffect[nCntEffect].type]);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			//// ������������������
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_aEffect[nCntEffect].scale.x, g_aEffect[nCntEffect].scale.y, g_aEffect[nCntEffect].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_aEffect[nCntEffect].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);

	// �t�H�O�L��
	SetFogEnable( GetFogEnable() );

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEffect(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorEffect(int nIdxEffect, XMFLOAT4 col)
{
	g_aEffect[nIdxEffect].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
//=============================================================================
int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type)
{
	int nIdxEffect = -1;

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(!g_aEffect[nCntEffect].use)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot   = { 0.0f, 0.0f, 0.0f };
			g_aEffect[nCntEffect].scale = { 1.0f, 1.0f, 1.0f };
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].material.Diffuse = col;
			g_aEffect[nCntEffect].fSizeX = fSizeX;
			g_aEffect[nCntEffect].fSizeY = fSizeY;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].use = TRUE;
			g_aEffect[nCntEffect].type = type;
			nIdxEffect = nCntEffect;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_aEffect[nCntEffect].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
			if(g_aEffect[nCntEffect].nIdxShadow != -1)
			{
				SetColorShadow(g_aEffect[nCntEffect].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxEffect;
}

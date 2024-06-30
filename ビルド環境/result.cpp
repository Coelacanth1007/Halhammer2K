//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : GP12B183 24 �]�@���^
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "score.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH						(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT						(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX							(6)				// �e�N�X�`���̐�

#define RESULT_WIDTH						(1725)			// ���S�T�C�Y
#define RESULT_HEIGHT						(72)			// 

#define RESULT_RETRY_WIDTH					(555)			// 
#define RESULT_RETRY_HEIGHT					(54)			// 

#define RESULT_TITLE_WIDTH					(477)			// 
#define RESULT_TITLE_HEIGHT					(54)			// 

#define RESULT_EXIT_WIDTH					(396)			// 
#define RESULT_EXIT_HEIGHT					(54)			// 

#define RESULT_CUR_WIDTH					(192)			// 
#define RESULT_CUR_HEIGHT					(119)			// 


enum TEX_LIST
{
	TEX_BG,
	TEX_NUB,
	TEX_RETRY,
	TEX_TITLE,
	TEX_EXIT,
	TEX_SKULL,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/result/bg001.png",
	"data/TEXTURE/result/number100x200.png",
	"data/TEXTURE/result/retry_ui.png",
	"data/TEXTURE/result/title_ui.png",
	"data/TEXTURE/result/exit_ui.png",
	"data/TEXTURE/result/skull.png",

};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;
int								resultCur;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_RESULT);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{

	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{

	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		resultCur = (++resultCur + RESULT_MAX) % RESULT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_UP))
	{
		resultCur = (--resultCur + RESULT_MAX) % RESULT_MAX;
	}

	switch (resultCur)
	{

	case RESULT_REPLAY:
		g_TexNo = TEX_SKULL;
		g_Pos = XMFLOAT3(670.0f, 250.0f, 0.0f);
		if (GetKeyboardPress(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{

			SetFade(FADE_OUT, MODE_GAME);

		}
		break;

	case RESULT_TITLE:
		g_TexNo = TEX_SKULL;
		g_Pos = XMFLOAT3(670.0f, 350.0f, 0.0f);
		if (GetKeyboardPress(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{

			SetFade(FADE_OUT, MODE_TITLE);

		}
		break;

	case RESULT_EXIT:
		g_TexNo = TEX_SKULL;
		g_Pos = XMFLOAT3(670.0f, 450.0f, 0.0f);
		if (GetKeyboardPress(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{

			GameLoop();
		}

	}


#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ���U���g�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_BG]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// TEX_RETRY��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_RETRY]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, 250.0f, RESULT_RETRY_WIDTH / 2, RESULT_RETRY_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// TEX_TITLE��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_TITLE]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, 350.0f, RESULT_TITLE_WIDTH / 2, RESULT_TITLE_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// TEX_EXIT�w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_EXIT]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, 450.0f, RESULT_EXIT_WIDTH / 2, RESULT_EXIT_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// ���U���g�̃��S��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_SKULL]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, RESULT_CUR_WIDTH / 2, RESULT_CUR_HEIGHT / 2, 0.0f, 0.0f, 1.0f, 1.0f);
													
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// �X�R�A�\��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEX_NUB]);

		// ��������������
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float pw = 16*4;			// �X�R�A�̕\����
			float ph = 32*4;			// �X�R�A�̕\������
			float px = 600.0f - i*pw;	// �X�R�A�̕\���ʒuX
			float py = 100.0f;			// �X�R�A�̕\���ʒuY

			float tw = 1.0f / 10;		// �e�N�X�`���̕�
			float th = 1.0f / 1;		// �e�N�X�`���̍���
			float tx = x * tw;			// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}



}





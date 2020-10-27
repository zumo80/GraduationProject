//=============================================================================
//
// �`���b�g�̃^�u���� [ chatTab.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "chatTab.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"
#include "texture.h"
#include "polygon2D.h"
#include "ImGui/imgui.h"			// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"	// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"	// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================
#define POS_OPENTAB		(D3DXVECTOR2(980.0f, 720.0f))	// �J�����^�u�̍��W
#define POS_CLOSETAB	(D3DXVECTOR2(1280.0f, 720.0f))	// �����^�u�̍��W

#define SIZE_TABBACK	(D3DXVECTOR3(300.0f, 720.0f, 0.0f))	// �^�u�w�i�̃T�C�Y
#define SIZE_TABBUTTON	(D3DXVECTOR3(40.0f, 100.0f, 0.0f))	// �^�u�J�{�^���̃T�C�Y

#define TIME_TABMOVE	(10)	// �^�u�J�̎���

#define COLOR_TABCLICK	(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// �^�u�N���b�N���̃J���[

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
D3DXVECTOR2 CChatTab::m_TabPos = ZeroVector2;
CChatTab::TABSTATE CChatTab::m_tabState = CChatTab::TABSTATE_CLOSED;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CChatTab::CChatTab()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CChatTab::~CChatTab()
{

}

//=============================================================================
// �`���b�g�^�u�̃N���b�N
//=============================================================================
void CChatTab::ClickTab(void)
{
	// �X���C�h���Ă���
	if (m_nCntState > 0)
		return;

	// ��Ԃ�ύX
	switch (m_tabState)
	{
	case TABSTATE_OPENED:
		m_tabState = TABSTATE_CLOSING;
		break;
	case TABSTATE_CLOSED:
		m_tabState = TABSTATE_OPENNING;
		break;
	}
	// �J���[�X�V
	m_pPolyTab->SetCol(COLOR_TABCLICK);
}

//=============================================================================
// �`���b�g�^�u�̃X���C�h
//=============================================================================
void CChatTab::SlideTab(void)
{
	// �ŏ������ʂ�
	if (m_nCntState == 0)
	{
		// �^�u�̈ړ��ʂ��v�Z
		switch (m_tabState)
		{
		case TABSTATE_OPENNING:
			m_moveDest = (POS_OPENTAB - m_TabPos) / TIME_TABMOVE;
			break;
		case TABSTATE_CLOSING:
			m_moveDest = (POS_CLOSETAB - m_TabPos) / TIME_TABMOVE;
			break;
		}
	}

	// �J���[�X�V
	if (m_nCntState == TIME_TABMOVE - 1)
		m_pPolyTab->SetCol(WhiteColor);

	// �J�E���^�����Z
	m_nCntState++;
	// ���ԂŃX���C�h�I��
	if (m_nCntState > TIME_TABMOVE)
	{
		// �J�E���^�����Z�b�g
		m_nCntState = 0;
		// ��Ԃ�ύX
		switch (m_tabState)
		{
		case TABSTATE_OPENNING:
			m_tabState = TABSTATE_OPENED;
			m_pPolyTab->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABCLOSE));
			break;
		case TABSTATE_CLOSING:
			m_tabState = TABSTATE_CLOSED;
			m_pPolyTab->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));
			break;
		}
		// �����̏I��
		return;
	}

	// �ړ��l�����Z���A���W�X�V
	m_TabPos += m_moveDest;
	m_pPolyBack->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
	m_pPolyTab->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
}

//=============================================================================
// ����
//=============================================================================
CChatTab * CChatTab::Create(void)
{
	// �������m��
	CChatTab *pTab = new CChatTab();

	// �m�ێ��s
	if (!pTab)
		return nullptr;

	// ������
	pTab->Init();
	// �l��Ԃ�
	return pTab;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CChatTab::Init(void)
{
	// �v�f�̏�����
	m_TabPos = POS_CLOSETAB;
	m_moveDest = ZeroVector2;
	m_tabState = TABSTATE_CLOSED;
	m_nCntState = 0;

	// �w�i�̐���
	m_pPolyBack = CPolygon2D::Create();
	m_pPolyBack->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
	m_pPolyBack->SetSize(SIZE_TABBACK);
	m_pPolyBack->SetPosStart(CPolygon2D::POSSTART_BOTTOM_LEFT);
	m_pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BACK));

	// �J�^�u�̐���
	m_pPolyTab = CPolygon2D::Create();
	m_pPolyTab->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
	m_pPolyTab->SetSize(SIZE_TABBUTTON);
	m_pPolyTab->SetPosStart(CPolygon2D::POSSTART_BOTTOM_RIGHT);
	m_pPolyTab->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CChatTab::Uninit(void)
{
	if (m_pPolyBack)
	{
		m_pPolyBack->Uninit();
		delete m_pPolyBack;
		m_pPolyBack = nullptr;
	}

	if (m_pPolyTab)
	{
		m_pPolyTab->Uninit();
		delete m_pPolyTab;
		m_pPolyTab = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CChatTab::Update(void)
{
	CMouse *pMouse = CManager::GetMouse();	
	CKeyboard *pKey = CManager::GetKeyboard();
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)pMouse->GetMouseX(), (float)pMouse->GetMouseY());

	// �^�u�̃X���C�h
	if (m_tabState == TABSTATE_CLOSING || m_tabState == TABSTATE_OPENNING)
		SlideTab();

	// ���N���b�N��F5�Ń^�u�̊J����
	if ((pMouse->GetTrigger(0) && m_pPolyTab->ReturnHit(mousePos)) ||
		pKey->GetTrigger(DIK_F5))
		ClickTab();

	if (m_pPolyBack)
		m_pPolyBack->Update();

	if (m_pPolyTab)
		m_pPolyTab->Update();
}

//=============================================================================
// �`��
//=============================================================================
void CChatTab::Draw(void)
{
	if (m_pPolyBack)
		m_pPolyBack->Draw();

	if (m_pPolyTab)
		m_pPolyTab->Draw();
}
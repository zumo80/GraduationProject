//------------------------------------------------------------------------------
//
//���W���[���̃����v�ƃ��C���[�̂�̏���  [Module_No2_LampAndWire.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "module_No2_LampAndWire.h"
#include "module_parts_No2_Wire.h"
#include "modelinfo.h"
#include "manager.h"
#include "keyboard.h"
#include "game.h"
#include "particle.h"
#include "mouse.h"
#include "sound.h"

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define WURE_OFFSET					(D3DXVECTOR3(-37.0f,0.0f,-23.0f))
#define WURE_ITNERVAL				(D3DXVECTOR3(12.0f,0.0f,0.0f))
#define WIRE_NUM					(6)
#define LAMP_OFFSET					(D3DXVECTOR3(-28.0f,-5.0f,-30.0f))
#define LAMP_INTERVAL				(D3DXVECTOR3(9.0f,0.0f,0.0f))
#define LAMP_NUM					(8)

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CModule_No2_LampAndWire::CModule_No2_LampAndWire()
{
	m_NowSelectWire = WIRE::RED;
	m_nSelectPlace = 0;

	m_nRedLampNum = 0;
	m_nBlueLampNum = 0;
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CModule_No2_LampAndWire::~CModule_No2_LampAndWire()
{

}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CModule_No2_LampAndWire::Init()
{
	//���f�����ݒ�
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO2));

	//���C���[����
	CreateWire();

	//���W���[�����̃����v����
	CreateModuleLamp();

	//�N���A�����v����
	CModule_Base::CreateLamp();

	//�J�b�g���郏�C���[�ݒ�
	SetToCutWire();

	//������
	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::Update()
{
	//���_�擾
	D3DXVECTOR3 posorigin = CHossoLibrary::CalcMtxToVector3(*GetMtxWorldPtr());


	//�����v�_��
	for (int nCnt = 0; nCnt < LAMP_NUM; nCnt++)
	{
		//�I�t�Z�b�g���W���Z�o
		D3DXVec3TransformCoord(&posorigin, &(LAMP_OFFSET + D3DXVECTOR3(LAMP_INTERVAL.x * nCnt, 0.0f, 0.0f)), GetMtxWorldPtr());

		if (nCnt < m_nRedLampNum)
		{
			//�ԃ����v
			CParticle::CreateFromText(posorigin, ZeroVector3, CParticleParam::EFFECT_MODULE_3_LED, RedColor);
		}
		else
		{
			//�����v
			CParticle::CreateFromText(posorigin, ZeroVector3, CParticleParam::EFFECT_MODULE_3_LED, BlueColor);
		}
	}

	CSceneX::Update();
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::ShowDebugInfo()
{
#ifdef _DEBUG
	CModule_Base::ShowDebugInfo();

#endif //DEBUG
}

//------------------------------------------------------------------------------
//���W���[������
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::Operation_Keyboard()
{
	int nSelectNumOld = m_nSelectPlace;

	//�I������
	CHossoLibrary::Selecting(m_nSelectPlace, nSelectNumOld, 6, 1);

	//�I������
	CModule_Base::ModuleParts_Select<CModule_Parts_No2_Wire>(m_pWireList, m_nSelectPlace);

	//���W���[������
	CModule_Base::Operation_Keyboard();

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------Module_No_3----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "RedLampNum >> [%d]\n", m_nRedLampNum);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "BlueLampNum >> [%d]\n", m_nBlueLampNum);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "Flag >>> [%d]  [%d]  [%d]  [%d]  [%d]  [%d]\n",
		m_pWireList[0]->GetClearFlag(), m_pWireList[1]->GetClearFlag(), m_pWireList[2]->GetClearFlag(),
		m_pWireList[3]->GetClearFlag(), m_pWireList[4]->GetClearFlag(), m_pWireList[5]->GetClearFlag());

}


//------------------------------------------------------------------------------
//���W���[������@�}�E�X
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::Operation_Mouse()
{
	//���C�̔���
	CHossoLibrary::RayCollision_ModuleSelect(m_pWireList.begin(), m_pWireList.end(), (int&)m_nSelectPlace);

	//�}�E�X����
	CModule_Base::Operation_Mouse();

}

//------------------------------------------------------------------------------
//���W���[���A�N�V����
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::ModuleAction()
{
	//�I��ԍ���-1�Ƃ���������
	if (m_nSelectPlace < 0)
	{
		return;
	}
	//nullcheck
	if (m_pWireList[m_nSelectPlace].get())
	{
		//���C���[�J�b�g����ĂȂ���
		if (!m_pWireList[m_nSelectPlace]->GetWireCut())
		{

			//���Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_CUT);

			//�ԈႦ�����C���[��������
			if (!m_pWireList[m_nSelectPlace]->GetClearFlag())
			{
				//���s
				Module_Failed();
			}

			//���C���[�J�b�g
			WireCut();

			//�N���A�������`�F�b�N
			CheckClear();
		}
	}
}

//------------------------------------------------------------------------------
//���W���[���̑I������
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::ModuleCancel()
{
	//�I������
	CModule_Base::ModuleParts_Select<CModule_Parts_No2_Wire>(m_pWireList, -1);
}


//------------------------------------------------------------------------------
//���C���[����
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::CreateWire()
{
	//���[�J���ϐ��@���C���[��6�{��6�F�o���o��
	Vec<WIRE> wirecol = { WIRE::RED,WIRE::BLUE,WIRE::GREEN ,WIRE::YELLOW,WIRE::WHITE, WIRE::BLACK };

	//�V���b�t��
	CHossoLibrary::Vec_Shuffle(wirecol);

	for(int nCnt = 0;  nCnt < WIRE_NUM; nCnt++)
	{
		m_pWireList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No2_Wire>
			(WURE_OFFSET + D3DXVECTOR3(nCnt * WURE_ITNERVAL.x, 0.0f, 0.0f), GetMtxWorldPtr()));

		m_pWireList[m_pWireList.size() - 1]->SetWire(wirecol[nCnt]);
	}

}
//------------------------------------------------------------------------------
//�����v����
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::CreateModuleLamp()
{
	//�Ԃ������v�������_���ݒ�
	m_nRedLampNum = CHossoLibrary::RandomRangeUnsigned(0, LAMP_NUM);

	//�Ԃ������v������t�Z���Đ������v�ݒ�
	m_nBlueLampNum = LAMP_NUM - m_nRedLampNum;




}

//------------------------------------------------------------------------------
//�؂�ׂ����C���[�ݒ�
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::SetToCutWire()
{
	auto Itr_Begin = m_pWireList.begin();
	auto Itr_End = m_pWireList.end();

	//�Ԃ������v������ɐ؂�R�[�h�ݒ�
	//�ڍׂ͎d�l���Q�l
	switch (m_nRedLampNum)
	{
	case 4:
		SetCutWire_FromColor(WIRE::BLACK, Itr_Begin, Itr_End, true, false, true, true);		//���̗���
		SetCutWire_FromColor(WIRE::YELLOW, Itr_Begin, Itr_End, true, false, true, false);	//���̍�
		SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true, false, false, true);		//�Ԃ̉E
		SetCutWire_FromPlace(3);															//������4�Ԗ�

		break;

	case 1:
		SetCutWire_FromColor(WIRE::GREEN, Itr_Begin, Itr_End, true, false, true, true);		//�΂̗���
		SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true, false, true, true);		//���̗���
		SetCutWire_FromPlace(4);															//�E����2�Ԗ�
		SetCutWire_FromPlace(0);															//���[
		SetCutWire_FromPlace(5);															//�E�[

		break;

	case 2:
		SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true, false, true, true);				//���̗���
		SetCutWire_FromColor(WIRE::YELLOW, Itr_Begin, Itr_End, true, false, false, true);			//���̉E
		SetCutWire_FromPlace(4);																	//������5�Ԗ�
		break;


		//�r���I�p�^�[��
	default:
		//�����v��������
		if (m_nRedLampNum < m_nBlueLampNum)
		{
			SetCutWire_FromColor(WIRE::GREEN, Itr_Begin, Itr_End, true, true, true, true);			//�΂Ɨ΂̗���
			SetCutWire_FromColor(WIRE::BLUE, Itr_Begin, Itr_End, true);								//�̗���
			SetCutWire_FromPlace(2);																//������3�Ԗ�
		}
		//�ԃ����v��������
		else
		{
			SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true);							//��
			SetCutWire_FromColor(WIRE::YELLOW, Itr_Begin, Itr_End, true);							//��
			SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true, false, true, true);			//�Ԃ̗���
			SetCutWire_FromPlace(1);																//�E����5�Ԗ�

		}

		break;
	}


	//�����v���[������ɐ؂�R�[�h�ݒ�
	SetCutWire_FromLampRule();

}

//------------------------------------------------------------------------------
//�؂�ׂ����C���[�ݒ�
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::WireCut()
{
	m_pWireList[m_nSelectPlace]->SetWireCut(true);
	m_pWireList[m_nSelectPlace]->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_NO1_KEYPAD));
	m_pWireList[m_nSelectPlace]->SetClearFlag(false);

}

//------------------------------------------------------------------------------
//�؂�ׂ����C���[�ݒ�
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::CheckClear()
{
	int nCount = std::count_if(m_pWireList.begin(), m_pWireList.end(),
		[](S_ptr<CModule_Parts_No2_Wire> ptr) {return ptr->GetClearFlag(); });

	if (nCount <= 0)
	{
		Module_Clear();
	}
}

//------------------------------------------------------------------------------
//�؂�ׂ����C���[�ݒ� �ꏊ����ɐݒ�
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::SetCutWire_FromPlace(int nPlace)
{
	if (nPlace < WIRE_NUM)
	{
		m_pWireList[nPlace] -> SetClearFlag(true);
	}
}

//------------------------------------------------------------------------------
//�؂�ׂ����C���[�ݒ� �����v���[������ɐݒ�
//------------------------------------------------------------------------------
void CModule_No2_LampAndWire::SetCutWire_FromLampRule()
{
	auto Itr_Begin = m_pWireList.begin();
	auto Itr_End = m_pWireList.end();

	//�����v3�ȏ�
	if (m_nBlueLampNum >= 3)
	{
		SetCutWire_FromColor(WIRE::BLUE, Itr_Begin, Itr_End, false);				//��؂�Ȃ�

		//�����v5�ȏ�
		if (m_nBlueLampNum >= 5)
		{
			SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true);				//�Ԃ�؂�

			//�S�������v
			if (m_nBlueLampNum >= LAMP_NUM)
			{
				SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true);		//����؂�
				SetCutWire_FromColor(WIRE::BLACK, Itr_Begin, Itr_End, true);		//����؂�

			}
		}
	}

	//�ԃ����v3�ȏ�
	if (m_nRedLampNum >= 3)
	{
		SetCutWire_FromColor(WIRE::BLACK, Itr_Begin, Itr_End, false);				//����؂�Ȃ�

		//�ԃ����v5�ȏ�
		if (m_nRedLampNum >= 5)
		{
			SetCutWire_FromColor(WIRE::GREEN, Itr_Begin, Itr_End, true);			//����؂�

			//�S���ԃ����v
			if (m_nRedLampNum >= LAMP_NUM)
			{
				SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true);			//�Ԃ�؂�
				SetCutWire_FromColor(WIRE::BLUE, Itr_Begin, Itr_End, true);			//��؂�
			}
		}
	}

}
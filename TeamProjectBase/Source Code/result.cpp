//------------------------------------------------------------------------------
//
//リザルト処理  [result.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "scene2D.h"
#include "game.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CResult::CResult()
{
	m_nCntResult = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CResult::~CResult()
{
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CResult::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	//ロゴ
	CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 100.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_LOGO), CScene::OBJTYPE_UI);

	//バー
	CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 150.0f, 0.0f), D3DXVECTOR3(1300.0f, 5.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_BAR), CScene::OBJTYPE_UI);

	//クリアしたか
	CGame::GetClearFlag() ?
		CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 200.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_CLEAR), CScene::OBJTYPE_UI) :
		CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 200.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_FAILED), CScene::OBJTYPE_UI);


	//プレイタイム
	CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(300.0f, 500.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_TIMER), CScene::OBJTYPE_UI);

	//バー
	CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR3(1300.0f, 5.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_BAR), CScene::OBJTYPE_UI);




	//CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 600.0f, 0.0f), D3DXVECTOR3(800.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_PRESSBUTTON), CScene::OBJTYPE_UI);

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CResult::Update()
{
	//カウンタ++
	m_nCntResult++;

	//フェードしてない時
	if (CManager::GetRenderer()->GetFade()->GetFadeState() == CFade::FADE_NONE)
	{
		//何かボタン押したとき
		if (CHossoLibrary::CheckAnyButton())
		{
				//ステート変更
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TITLE);
				CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}
}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CResult::Draw()
{

}
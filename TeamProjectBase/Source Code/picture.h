//*************************************************************************************************************
//
// 絵の処理 [picture.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _PICTURE_H_
#define _PICTURE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CPaintingPen;
class CPicture : public CScene
{
public:
	// フラグ
	enum
	{
		FLAG_NONE = -1,			// 無し
		FLAG_DISP,				// 描画
		FLAG_UPDATE,			// 更新
		FLAG_MAX				// 最大
	};
	// マスクフラグの値
	enum
	{
		MASK_NONE = 0,			// なし
		MASK_DISP = 0b0001,		// 描画
		MASK_UPDATE = 0b0010,	// 更新
		MASK_MAX = 0b0011,		// 最大
	};

	/* メンバ関数 */
	inline                  CPicture() :CScene(){}																			// コンストラクタ
	inline                  ~CPicture();																					// デストラクタ
	HRESULT                 Init();																							// 初期化
	void                    Update();																						// 更新
	void                    Draw();																							// 描画
	inline void             ShowDebugInfo() {};																				// デバッグ情報表記

	static void             InitStaticMember(void);																			// 静的メンバの初期化
	static void             UninitStaticMember(void);																		// 静的メンバの終了
	static HRESULT          LoadError(void);																				// エラーの検出

	void                    MatrixCal(void);																				// マトリックスの計算
	void                    Writing(void);																					// 書き込み処理
	static void             Reading(LPDIRECT3DTEXTURE9 pTexture,CString& file);																			// 読み込み
	static void             TexterReadFromLine(CONST_STRING cnpLine, void*pOut);											// テクスチャ情報の1行から情報を読み取る

	// 設定関数
	static void             SetFromString(CONST_STRING str);																// 文字列から設定する
	inline void             SetPos(CONST D3DXVECTOR3 &pos) { m_trans.pos = pos; }											// 位置の設定
	inline void             SetRot(CONST D3DXVECTOR3 &rot) { m_trans.rot = rot; }											// 向きの設定
	inline void             SetScal(CONST D3DXVECTOR3 &scal) { m_trans.scal = scal; }										// スケールの設定
	inline void             SetParent(D3DMATRIX *pMtxParent) { m_pMtxParent = pMtxParent; }									// 親のマトリックス
	inline void             SetFlag(const int nMask) { m_Flags.data = nMask; }												// フラグの設定
	inline void             BuildFlag(const int nFlagIndex) { Mlf_bit_set(m_Flags.data, nFlagIndex); }					// フラグを建てる
	inline void             BreakFlag(const int nFlagIndex) { Mlf_bit_clear(m_Flags.data, nFlagIndex); }				// フラグを壊す

	// 取得関数
	inline UVSHORT*         GetFlag(void) { return &m_Flags.data; }															// フラグの取得
	inline CPaintingPen*    GetPaintPen(void) { return m_pPen; }														// ペンの取得
	// クリエイト関数
	static std::shared_ptr<CPicture>Create(D3DMATRIX *pMtxParent);															// 生成
	static HRESULT          MakeTexture(LPDIRECT3DDEVICE9 pDevice, CONST_STRING TextureFile , LPDIRECT3DTEXTURE9 *ppTexture);// テクスチャの作成

private:
	/* 内部で使う関数 */
	void                    MakeTexture(LPDIRECT3DDEVICE9 pDevice);															// テクスチャの作成
	void                    MakeVertex(LPDIRECT3DDEVICE9 pDevice);															// 頂点情報の作成
	void                    InitTextureColor(void);																			// テクスチャカラーの初期化
	static void             ReadFromLine(CONST_STRING Line);																// 1行から情報を読み取る
	static void             CreatePixelPos(void);																			// ピクセル位置の生成
	static void             ReleasePixelPos(void);																			// ピクセル位置の開放
	static void             ReleasePen(void);																				// ペンの開放
	void                    PaintProc(void);																				// 塗る処理
	bool                    GetMousePosOnPicture(void);																		// 絵上のマウス位置の取得
	bool                    ItIsPressingButtons(void);																		// タブレットのボタンを押しているか
	/* メンバ変数 */
	static INTEGER2         m_nNumPixelBlock;																				// ピクセル数
	static FLOAT2           m_size;																							// 大きさ
	static D3DXVECTOR3      m_PlaneNor;																						// 平面の法線
	static D3DXVECTOR2      m_PixelSize;																					// ピクセルサイズ
	static UINT             m_nNumDataMax;																					// 最大データ数
	static D3DXVECTOR2      m_PixelSizehalf;																				// ピクセルサイズの半分
	static D3DXVECTOR2*     m_pPixelPos;																					// ピクセル位置のポインタ
	static UINT             m_nNumMakeFile;																					// ファイルを作った回数
	static CString          m_WriteToFile;																					// 書き込み先のァイル名
	static D3DXVECTOR3      m_SetingPos;																					// 設定用の位置
	CPaintingPen*           m_pPen;																							// ペンのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtexBuff;																					// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9      m_pTexture;																						// テクスチャポインタ
	BITS4                   m_Flags;																						// フラグ
	TRANSFORM               m_trans;																						// トランス情報
	D3DMATRIX*              m_pMtxParent;																					// 親のマトリックス
};


#endif // !_PICTURE_H_

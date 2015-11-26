//================================================================================
// テクスチャマネージャー処理 [TextureManager.cpp]
// Author :岩澤 剛志
// テクスチャマネージャーモジュールです。
//================================================================================

//================================================================================
// インクルードガード
//================================================================================
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

//================================================================================
// インクルード
//================================================================================
#include "../render/directx9/directx9.h"
#include <stdio.h>
#include <memory>
#include <list>
using namespace std;

//================================================================================
// 前方宣言
//================================================================================
class Texture;
class TextureCreator;

//================================================================================
// クラス
//================================================================================
class TextureManager
{
	public:
		//================================================================================
		// テクスチャポインタの取得
		// 戻り値 : テクスチャポインタ
		// 引数1 : ファイルパス
		//================================================================================
		static IDirect3DTexture9* GetTexture(const char* file_pass);

		//================================================================================
		// テクスチャクリエーターの作成と取得
		// 戻り値 : テクスチャクリエーターID
		// 引数1 : 横幅
		// 引数2 : 縦幅
		// 引数3 : クリアカラー
		//================================================================================
		static int CreateTextureCreator(const unsigned int& w, const unsigned int& h, const D3DXCOLOR& setColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//================================================================================
		// テクスチャクリエーターの取得
		// 戻り値 : テクスチャクリエーターポインタ
		// 引数1 : テクスチャID
		//================================================================================
		static TextureCreator* GetTextureCreator(const int& texId);

		//================================================================================
		// 全テクスチャ解放
		// 戻り値 : なし
		// 引数1 : なし
		//================================================================================
		static void AllRelease(void);

	private:
		//================================================================================
		// コンストラクタ
		// 戻り値 : なし
		// 引数1 : なし
		//================================================================================
		TextureManager(void){}

		//================================================================================
		// デストラクタ
		// 戻り値 : なし
		// 引数1 : なし
		//================================================================================
		~TextureManager(void){}

		//================================================================================
		// 列挙宣言
		//================================================================================
		typedef enum
		{
			TYPE_NONE = 0,
			TYPE_LOAD,		// すでにあるテクスチャを読み込んで来るクラス
			TYPE_CREATOR,	// テクスチャを作ってくれるクラス
		}CLASS_TYPE;

		//================================================================================
		// 構造体宣言
		//================================================================================
		struct TEXTURE_BOX
		{
			union
			{
				Texture*		texture_;
				TextureCreator*	texture_creator_;
			};
			CLASS_TYPE class_type_;

			// コンストラクタ
			TEXTURE_BOX(void) :
				texture_(nullptr),
				class_type_(TYPE_NONE)
			{
			}
		};

		//================================================================================
		// 変数宣言
		//================================================================================
		static list<TEXTURE_BOX>		texture_box_list_;		// テクスチャボックスリスト
};

#endif // _TEXTURE_MANAGER_H_
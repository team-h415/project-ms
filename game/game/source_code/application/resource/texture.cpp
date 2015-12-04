//================================================================================
// テクスチャ処理 [Texture.cpp]
// Author : TAKESI IWASAWA
//================================================================================


//================================================================================
// インクルードファイル
//================================================================================
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "texture.h"

//================================================================================
// コンストラクタ
//================================================================================
Texture::Texture(const char* pass) :
	tex_(nullptr),
	tex_pass_(nullptr)
{
	// パスnullptrチェック
	if(pass == nullptr)
	{
		return;
	}

	// テクスチャロード
	if(!Load(pass))
	{
		return;
	}

	// テクスチャネーム置き換え
	int name_len(strnlen(pass, 255) + sizeof(char));
	tex_pass_ = new char[name_len];
	strcpy_s(tex_pass_, name_len, pass);
}

//================================================================================
// デストラクタ
//================================================================================
Texture::~Texture(void)
{
	// 解放
	SAFE_RELEASE(tex_);
	SAFE_DELETE_ARRAY(tex_pass_);
}

//================================================================================
// テクスチャ取得
//================================================================================
bool Texture::Load(const char* pass)
{
	// デバイスの取得
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// テクスチャ情報取得
	D3DXIMAGE_INFO	info;
	D3DXGetImageInfoFromFile(pass, &info);

	// 読み込み
	if(FAILED(D3DXCreateTextureFromFileEx(	device,
											pass,
											info.Width,
											info.Height,
											1,
											0,
											D3DFMT_A8R8G8B8,
											D3DPOOL_MANAGED,
											D3DX_FILTER_NONE,
											D3DX_FILTER_NONE,
											0,
											nullptr,
											nullptr,
											&tex_)))
	{
		std::string warning;
		warning = pass;
		warning += ": このファイルが見つかりません";
		ASSERT_WARNING(warning.c_str());

		return false;
	}

	return true;
}

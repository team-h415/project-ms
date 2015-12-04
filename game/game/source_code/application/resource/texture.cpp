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
Texture::Texture(const char* path) :
	tex_(nullptr),
	tex_path_(nullptr)
{
	// パスnullptrチェック
	if(path == nullptr)
	{
		return;
	}

	// テクスチャロード
	if(!Load(path))
	{
		return;
	}

	// テクスチャネーム置き換え
	int name_len(strnlen(path, 255) + sizeof(char));
	tex_path_ = new char[name_len];
	strcpy_s(tex_path_, name_len, path);
}

//================================================================================
// デストラクタ
//================================================================================
Texture::~Texture(void)
{
	// 解放
	SAFE_RELEASE(tex_);
	SAFE_DELETE_ARRAY(tex_path_);
}

//================================================================================
// テクスチャ取得
//================================================================================
bool Texture::Load(const char* path)
{
	// デバイスの取得
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// テクスチャ情報取得
	D3DXIMAGE_INFO	info;
	D3DXGetImageInfoFromFile(path, &info);

	// 読み込み
	if(FAILED(D3DXCreateTextureFromFileEx(	device,
											path,
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
		warning = path;
		warning += ": このファイルが見つかりません";
		ASSERT_WARNING(warning.c_str());

		return false;
	}

	return true;
}

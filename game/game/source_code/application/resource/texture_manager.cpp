//================================================================================
// テクスチャマネージャー処理 [TextureManager.cpp]
// Author :岩澤 剛志
// テクスチャマネージャーモジュールです。
//================================================================================

//================================================================================
// インクルード
//================================================================================
#include "../../common/common.h"
#include "../render/renderer.h"
#include "texture.h"
#include "texture_creator.h"
#include "texture_manager.h"


//================================================================================
// 変数宣言
//================================================================================
list<TextureManager::TEXTURE_BOX>		TextureManager::texture_box_list_;		// テクスチャボックスリスト


//================================================================================
// テクスチャの取得
//================================================================================
IDirect3DTexture9* TextureManager::GetTexture(const char* file_path)
{
	if(file_path == nullptr)
	{
		return nullptr;
	}

	// イテレータ変数の確保
	int i(0);
	list<TEXTURE_BOX>::iterator it(texture_box_list_.begin());
	char* get_path(nullptr);
	for(i = 0; it != texture_box_list_.end(); i++, it++)
	{
		get_path = nullptr;
		switch(it->class_type_)
		{
			case TYPE_LOAD:
				get_path = (it->texture_)->GetTexturePath();
				if(get_path == nullptr)
				{
					continue;
				}
				// ファイルパス比較
				if(strcmp(file_path, get_path) == 0)
				{
					return it->texture_->GetTexture();
				}
				break;
	
			default:
				break;
		}
	}
	// 新しく追加
	TEXTURE_BOX textureBox;
	textureBox.texture_ = new Texture(file_path);
	textureBox.class_type_ = TYPE_LOAD;
	texture_box_list_.push_back(textureBox);
	
	return textureBox.texture_->GetTexture();
}


//================================================================================
// テクスチャクリエーターの作成と取得
//================================================================================
int TextureManager::CreateTextureCreator(const unsigned int& w, const unsigned int& h, const D3DXCOLOR& setColor)
{
	// イテレータ変数の確保
	int i(0);
	list<TEXTURE_BOX>::iterator it(texture_box_list_.begin());
	char* get_path(nullptr);
	for(i = 0; it != texture_box_list_.end(); i++, it++)
	{
		get_path = nullptr;
		switch(it->class_type_)
		{
			case TYPE_NONE:
				it->texture_creator_ = new TextureCreator();
				it->texture_creator_->Setup(w, h, setColor);
				it->class_type_ = TYPE_CREATOR;
				return i;

			default:
				break;
		}
	}

	// 変数用意
	TEXTURE_BOX textureBox;
	textureBox.texture_creator_ = new TextureCreator();
	textureBox.texture_creator_->Setup(w, h, setColor);
	textureBox.class_type_ = TYPE_CREATOR;
	texture_box_list_.push_back(textureBox);

	return i;
}


//================================================================================
// テクスチャクリエーターの取得
//================================================================================
TextureCreator* TextureManager::GetTextureCreator(const int& texId)
{
	if(texId <= -1)
	{
		return nullptr;
	}

	// イテレータ変数の確保
	list<TEXTURE_BOX>::iterator it(texture_box_list_.begin());
	for(int i = 0; i != texId; i++)
	{
		if(it == texture_box_list_.end())
		{
			return nullptr;
		}
		it++;
	}
	switch(it->class_type_)
	{
		case TYPE_CREATOR:
			return it->texture_creator_;

		default:
			return nullptr;
	}
}


//================================================================================
// 全テクスチャ解放
//================================================================================
void TextureManager::AllRelease(void)
{
	// イテレータ変数の確保
	list<TEXTURE_BOX>::iterator it;
	for(it = texture_box_list_.begin(); it != texture_box_list_.end(); it++)
	{
		switch(it->class_type_)
		{
			case TYPE_LOAD:
				SAFE_DELETE(it->texture_);
				break;
			case TYPE_CREATOR:
				SAFE_DELETE(it->texture_creator_);
				break;
			default:
				break;
		}
	}
	texture_box_list_.clear();
}
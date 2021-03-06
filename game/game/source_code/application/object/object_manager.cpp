//=========================================================
// object_manager.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "../math/vector.h"
#include "object.h"
#include "../shader/shader.h"
#include "objects/model/x_model.h"
#include "objects/bullet/bullet.h"
#include "objects/bullet/bomb.h"
#include "objects/sprite/blind.h"
#include "object_factory.h"
#include "object_manager.h"


//-------------------------------------
// warning
//-------------------------------------
#pragma warning (disable:4996)


//-------------------------------------
// variable
//-------------------------------------
int ObjectManager::object_count_ = 0;


//-------------------------------------
// ObjectManager()
//-------------------------------------
ObjectManager::ObjectManager()
{
	for (int i = 0; i < LAYER_MAX; i++){
		objects_[i].clear();
		draw_enable_[i] = true;
	}
}


//-------------------------------------
// ~ObjectManager()
//-------------------------------------
ObjectManager::~ObjectManager()
{
	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
			SAFE_DELETE((*it).second);
		}
		objects_[i].clear();
	}
}


//-------------------------------------
// Update()
//-------------------------------------
void ObjectManager::Update()
{
	object_count_ = 0;
	
	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end();){
			if ((*it).second->this_delete()){
				SAFE_DELETE((*it).second);
				objects_[i].erase(it++);
			}
			else
			{
				it++;
			}
		}
	}
	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
			(*it).second->Update();
			object_count_++;
		}
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void ObjectManager::Draw()
{
	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
            if (draw_enable_[i])
			(*it).second->Draw();
		}
	}
}


//-------------------------------------
// Create()
//-------------------------------------
Object *ObjectManager::Create(
	const OBJECT_PARAMETER_DESC &parameter)
{
	if (Search(parameter.name_)) return nullptr;
	for (int i = 0; i < LAYER_MAX; i++){
		if (i == parameter.layer_){
			objects_[i][parameter.name_] = ObjectFactory::Create(parameter);
			return objects_[i][parameter.name_];
		}
	}
	ASSERT_ERROR("指定したレイヤーが見つからない為、生成できませんでした");
	return nullptr;
}

Object *ObjectManager::Create(
	const OBJECT_PARAMETER_DESC &parameter,
	const std::string &object_path)
{
	if (Search(parameter.name_)) return nullptr;
	for (int i = 0; i < LAYER_MAX; i++){
		if (i == parameter.layer_){
			objects_[i][parameter.name_] = ObjectFactory::Create(parameter,object_path);
			return objects_[i][parameter.name_];
		}
	}
	ASSERT_ERROR("指定したレイヤーが見つからない為、生成できませんでした");
	return nullptr;
}


//-------------------------------------
// Get()
//-------------------------------------
Object *ObjectManager::Get(
	const std::string &name)
{
	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
			if ((*it).first == name){
				return (*it).second;
			}
		}
	}
	return nullptr;
}


//-------------------------------------
// 未使用バレット取得
//-------------------------------------
// 未使用のバレットを取得するための特殊メソッド
// ObjectManager::Get(
//     "オブジェクトの名前");
Bullet *ObjectManager::GetNoUseBullet()
{
	for(auto it = objects_[LAYER_BULLET].begin(); it != objects_[LAYER_BULLET].end(); ++it){
		if((*it).second->use() == false){
			return dynamic_cast<Bullet*>((*it).second);
		}
	}
	ASSERT_WARNING("全部のバレットが活動しているぞ　はーと");
	return nullptr;
}


//-------------------------------------
// 未使用ボム取得
//-------------------------------------
// 未使用のバレットを取得するための特殊メソッド
// ObjectManager::Get(
//     "オブジェクトの名前");
Bomb *ObjectManager::GetNoUseBomb()
{
	for (auto it = objects_[LAYER_BOMB].begin(); it != objects_[LAYER_BOMB].end(); ++it){
		if ((*it).second->use() == false){
			return dynamic_cast<Bomb*>((*it).second);
		}
	}
	ASSERT_WARNING("全部のボムが活動しているぞ　ほし");
	return nullptr;
}

//-------------------------------------
// 未使用ブラインド取得
//-------------------------------------
// 未使用のブラインドを取得するための特殊メソッド
// ObjectManager::Get(
//     "オブジェクトの名前");
Blind *ObjectManager::GetNoUseBlind()
{
	for (auto it = objects_[LAYER_BLIND].begin(); it != objects_[LAYER_BLIND].end(); ++it){
		if ((*it).second->use() == false){
			return dynamic_cast<Blind*>((*it).second);
		}
	}
	//ASSERT_WARNING("全部のブラインドが活動しているぞ　ほし");
	return nullptr;
}



//-------------------------------------
// Search()
//-------------------------------------
bool ObjectManager::Search(
	const std::string &name)
{
	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
			if ((*it).first == name){
				if (name == "notice"){
					continue;
				}
				std::string warning;
				warning = name;
				warning += ": この名前のオブジェクトは作成済みです";
				ASSERT_WARNING(warning.c_str());
				return true;
			}
		}
	}
	return false;
}


//-------------------------------------
// Clear()
//-------------------------------------
void ObjectManager::Clear(
	OBJECT_LAYER layer)
{
	for (int i = 0; i < LAYER_MAX; i++){
		if (i == layer){
			for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
				SAFE_DELETE((*it).second);
			}
			objects_[i].clear();
		}
	}
}


//-------------------------------------
// ExportObjectParameter()
//-------------------------------------
void ObjectManager::ExportObjectParameter(
	const std::string &file_path)
{
	FILE *file = fopen(file_path.c_str(), "wt");

	for (int i = 0; i < LAYER_MAX; i++){
		for (auto it = objects_[i].begin(); it != objects_[i].end(); ++it){
			fprintf(file, "\n");
			fprintf(file, "name : %s\n", (*it).first.c_str());
			fprintf(file, "pos : %3.2f %3.2f %3.2f\n",
				(*it).second->parameter().position_.x_,
				(*it).second->parameter().position_.y_,
				(*it).second->parameter().position_.z_);
			fprintf(file, "rot : %3.2f %3.2f %3.2f\n",
				(*it).second->parameter().rotation_.x_,
				(*it).second->parameter().rotation_.y_,
				(*it).second->parameter().rotation_.z_);
		}
	}
	fclose(file);
}


//-------------------------------------
// end of file
//-------------------------------------

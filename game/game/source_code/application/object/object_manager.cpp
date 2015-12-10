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
#include "object_factory.h"
#include "object_manager.h"


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
			(*it).second->Draw();
		}
	}
}


//-------------------------------------
// Create()
//-------------------------------------
Object *ObjectManager::Create(
	const std::string &name,
	const OBJECT_PARAMETER_DESC &parameter)
{
	if (Search(name)) return nullptr;
	for (int i = 0; i < LAYER_MAX; i++){
		if (i == parameter.layer_){
			objects_[i][name] = ObjectFactory::Create(parameter);
			return objects_[i][name];
		}
	}
	ASSERT_ERROR("指定したレイヤーが見つからない為、生成できませんでした");
	return nullptr;
}

Object *ObjectManager::Create(
	const std::string &name,
	const OBJECT_PARAMETER_DESC &parameter,
	const std::string &object_path)
{
	if (Search(name)) return nullptr;
	for (int i = 0; i < LAYER_MAX; i++){
		if (i == parameter.layer_){
			objects_[i][name] = ObjectFactory::Create(parameter,object_path);
			return objects_[i][name];
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
// end of file
//-------------------------------------

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
#include "objects/notice/bullet.h"
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
	const OBJECT_PARAMETER_DESC &parameter)
{
	if (Search(parameter.name_)) return nullptr;
	for (int i = 0; i < LAYER_MAX; i++){
		if (i == parameter.layer_){
			objects_[i][parameter.name_] = ObjectFactory::Create(parameter);
			return objects_[i][parameter.name_];
		}
	}
	ASSERT_ERROR("�w�肵�����C���[��������Ȃ��ׁA�����ł��܂���ł���");
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
	ASSERT_ERROR("�w�肵�����C���[��������Ȃ��ׁA�����ł��܂���ł���");
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
// ���g�p�o���b�g�擾
//-------------------------------------
// ���g�p�̃o���b�g���擾���邽�߂̓��ꃁ�\�b�h
// ObjectManager::Get(
//     "�I�u�W�F�N�g�̖��O");
Bullet *ObjectManager::GetNoUseBullet()
{
	for(auto it = objects_[LAYER_BULLET].begin(); it != objects_[LAYER_BULLET].end(); ++it){
		if((*it).second->use() == false){
			return dynamic_cast<Bullet*>((*it).second);
		}
	}
	ASSERT_WARNING("�S���̃o���b�g���������Ă��邼�@�́[��");
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
				warning += ": ���̖��O�̃I�u�W�F�N�g�͍쐬�ς݂ł�";
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

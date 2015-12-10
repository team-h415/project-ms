//=========================================================
// camera_manager.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "camera.h"
#include "camera_manager.h"
#include "camera_factory.h"


//-------------------------------------
// CameraManager()
//-------------------------------------
CameraManager::CameraManager()
{
	cameras_.clear();
}


//-------------------------------------
// ~CameraManager()
//-------------------------------------
CameraManager::~CameraManager()
{
	for (auto it = cameras_.begin(); it != cameras_.end(); ++it){
		SAFE_DELETE((*it).second);
	}
	cameras_.clear();
}


//-------------------------------------
// Update()
//-------------------------------------
void CameraManager::Update()
{
	for (auto it = cameras_.begin(); it != cameras_.end(); ++it){
		(*it).second->Update();
	}
}


//-------------------------------------
// Set()
//-------------------------------------
void CameraManager::Set(
	const std::string &name)
{
	for (auto it = cameras_.begin(); it != cameras_.end(); ++it){
		if ((*it).first == name){
			(*it).second->Set();
			return;
		}
	}
	ASSERT_WARNING("指定したカメラは存在しません");
}


//-------------------------------------
// Create()
//-------------------------------------
Camera *CameraManager::Create(
	const std::string &type,
	const std::string &name,
	const CAMERA_PARAMETER_DESC &parameter)
{
	if (Search(name)) return nullptr;
	cameras_[name] = CameraFactory::Create(
		type, parameter);
	return cameras_[name];
}


//-------------------------------------
// Get()
//-------------------------------------
Camera *CameraManager::Get(
	const std::string &name)
{
	for (auto it = cameras_.begin(); it != cameras_.end(); ++it){
		if ((*it).first == name){
			return (*it).second;
		}
	}
	ASSERT_WARNING("同一名のカメラが存在しません");
	return nullptr;
}


//-------------------------------------
// Search()
//-------------------------------------
bool CameraManager::Search(
	const std::string &name)
{
	for (auto it = cameras_.begin(); it != cameras_.end(); ++it){
		if ((*it).first == name){
			ASSERT_WARNING("同一名のカメラが存在します");
			return true;
		}
	}
	return false;
}


//-------------------------------------
// end of file
//-------------------------------------

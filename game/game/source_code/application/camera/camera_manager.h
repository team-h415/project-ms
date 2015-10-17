//=========================================================
// camera_manager.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __CameraManager_H__
#define __CameraManager_H__


//-------------------------------------
// class
//-------------------------------------
class CameraManager
{
public:
	CameraManager();
	virtual ~CameraManager();
	void Update();
	void Set(
		const std::string &name);

	//-------------------------------------
	// �J��������
	//-------------------------------------
	// �J�����̐������s���܂��B
	// ex)
	// camera_manager_->Create(
	//     "Perspective",
	//     "MainCamera",
	//     camera_parameter);
	Camera *Create(
		const std::string &type,
		const std::string &name,
		const CAMERA_PARAMETER_DESC &parameter);

	//-------------------------------------
	// �J�����擾
	//-------------------------------------
	// �J�����I�u�W�F�N�g���擾���܂�
	// ex)
	// camera_manager_->Get(
	//     "MainCamera");
	Camera *Get(
		const std::string &name);

private:
	bool Search(
		const std::string &name);
	std::map<std::string, Camera*>cameras_;
};


#endif //__CameraManager_H__


//-------------------------------------
// end of file
//-------------------------------------

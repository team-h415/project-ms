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
	// �X�V
	void Update();
	// �Z�b�g
	void Set(
		const std::string &name);
	// �J��������
	Camera *Create(
		const std::string &type,
		const std::string &name,
		const CAMERA_PARAMETER_DESC &parameter);
private:
	// ���ꖼ�J�����`�F�b�N
	bool Search(
		const std::string &name);
	std::map<std::string, Camera*>cameras_;
};


#endif //__CameraManager_H__


//-------------------------------------
// end of file
//-------------------------------------

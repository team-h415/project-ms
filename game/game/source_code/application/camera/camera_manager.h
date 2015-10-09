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
	// 更新
	void Update();
	// セット
	void Set(
		const std::string &name);
	// カメラ生成
	Camera *Create(
		const std::string &type,
		const std::string &name,
		const CAMERA_PARAMETER_DESC &parameter);
private:
	// 同一名カメラチェック
	bool Search(
		const std::string &name);
	std::map<std::string, Camera*>cameras_;
};


#endif //__CameraManager_H__


//-------------------------------------
// end of file
//-------------------------------------

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
	static CameraManager* Get();
	static void Delete();

	void Update();
	void Set(
		const std::string &name);

	//-------------------------------------
	// カメラ生成
	//-------------------------------------
	// カメラの生成を行います。
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
	// カメラ取得
	//-------------------------------------
	// カメラオブジェクトを取得します
	// ex)
	// camera_manager_->Get(
	//     "MainCamera");
	Camera *Get(
		const std::string &name);

private:
	CameraManager();
	~CameraManager();

	static CameraManager* camera_manager_;

	bool Search(
		const std::string &name);
	std::unordered_map<std::string, Camera*>cameras_;
};


#endif //__CameraManager_H__


//-------------------------------------
// end of file
//-------------------------------------

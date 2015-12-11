//=========================================================
// oltho_camera.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __OlthoCamera_H__
#define __OlthoCamera_H__


//-------------------------------------
// class
//-------------------------------------
class OlthoCamera : public Camera
{
public:
	OlthoCamera(
		const CAMERA_PARAMETER_DESC &parameter);
	virtual ~OlthoCamera();

	// 更新
	void Update();
	// セット
	void Set();
private:
};


#endif //__OlthoCamera_H__


//-------------------------------------
// end of file
//-------------------------------------

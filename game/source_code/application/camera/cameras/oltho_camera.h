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

	// �X�V
	void Update();
	// �Z�b�g
	void Set();
private:
};


#endif //__OlthoCamera_H__


//-------------------------------------
// end of file
//-------------------------------------

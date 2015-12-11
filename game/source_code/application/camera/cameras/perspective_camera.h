//=========================================================
// perspective_camera.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __PerspectiveCamera_H__
#define __PerspectiveCamera_H__


//-------------------------------------
// class
//-------------------------------------
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(
		const CAMERA_PARAMETER_DESC &parameter);
	virtual ~PerspectiveCamera();

	// �X�V
	void Update();
	// �Z�b�g
	void Set();
private:
};


#endif //__PerspectiveCamera_H__


//-------------------------------------
// end of file
//-------------------------------------

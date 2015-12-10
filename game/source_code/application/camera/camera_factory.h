//=========================================================
// camera_factory.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __CameraFactory_H__
#define __CameraFactory_H__


//-------------------------------------
// class
//-------------------------------------
class CameraFactory
{
public:
	static Camera *Create(
		const std::string &type,
		const CAMERA_PARAMETER_DESC &parameter);
};


#endif //__CameraFactory_H__


//-------------------------------------
// end of file
//-------------------------------------

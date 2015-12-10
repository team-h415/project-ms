//=========================================================
// camera_factory.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "camera.h"
#include "camera_factory.h"
#include "cameras/perspective_camera.h"
#include "cameras/oltho_camera.h"


//-------------------------------------
// Create()
//-------------------------------------
Camera *CameraFactory::Create(
	const std::string &type,
	const CAMERA_PARAMETER_DESC &parameter)
{
	Camera *camera = nullptr;
	if (type == "Perspective"){
		camera = new PerspectiveCamera(parameter);
		return camera;
	}
	if (type == "Oltho"){
		camera = new OlthoCamera(parameter);
		return camera;
	}

	ASSERT_ERROR("指定したタイプのカメラは存在しません");
	return camera;
}

//-------------------------------------
// end of file
//-------------------------------------

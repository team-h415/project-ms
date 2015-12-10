//=========================================================
// message.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../common/common.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../object.h"
#include "sprite2d.h"
#include "message.h"
#include "../../../resource/texture_manager.h"


//-------------------------------------
// Message()
//-------------------------------------
Message::Message(
	const OBJECT_PARAMETER_DESC &parameter) :
	Sprite2D(parameter)
{
}


//-------------------------------------
// ~Message()
//-------------------------------------
Message::~Message()
{
}


//-------------------------------------
// end of file
//-------------------------------------

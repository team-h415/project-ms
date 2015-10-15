//=========================================================
// object_factory.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../math/vector.h"
#include "object.h"
#include "object_factory.h"
#include "objects/sprite/sprite2d.h"
#include "objects/mesh/field.h"


//-------------------------------------
// ObjectFactory()
//-------------------------------------
Object *ObjectFactory::Create(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// ïœêîêÈåæ
	OBJECT_PARAMETER_DESC param = parameter;
	Object *object = nullptr;

	if (param.layer_ = LAYER_SPRITE_2D){
		object = new Sprite2D(parameter);
	}

	return object;
}


Object *ObjectFactory::Create(
	const OBJECT_PARAMETER_DESC &parameter,
	const std::string &object_path)
{
	// ïœêîêÈåæ
	OBJECT_PARAMETER_DESC param = parameter;
	Object *object = nullptr;

	if (param.layer_ = LAYER_MESH_FIELD){
		object = new Field(parameter);
		Field *field = dynamic_cast<Field*>(object);
		field->LoadMesh(object_path);
	}

	return object;
}


//-------------------------------------
// end of file
//-------------------------------------

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
#include "objects/model/x_model.h"
#include "objects/model/fbx_model.h"
#include "objects/sprite/timer.h"
#include "objects/sprite/water_gage.h"
#include "objects/model/fbx/fbx_grandfather.h"
#include "objects/model/fbx/fbx_child.h"
#include "objects/notice/bullet.h"


//-------------------------------------
// ObjectFactory()
//-------------------------------------
Object *ObjectFactory::Create(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// 変数宣言
	OBJECT_PARAMETER_DESC param = parameter;
	Object *object = nullptr;

	if (param.layer_ == LAYER_SPRITE_2D){
		object = new Sprite2D(parameter);
	}

    else if (param.layer_ == LAYER_TIMER){
        object = new Timer(parameter);
    }

	else if (param.layer_ == LAYER_MODEL_GRANDFATHER){
		object = new FbxGrandfather(parameter);
	}

	else if (param.layer_ == LAYER_MODEL_CHILD){
		object = new FbxChild(parameter);
	}

	else if (param.layer_ == LAYER_BULLET){
		object = new Bullet(parameter);
	}

	else{
		ASSERT_ERROR("無効なオブジェクト生成カテゴリです");
	}


	return object;
}


Object *ObjectFactory::Create(
	const OBJECT_PARAMETER_DESC &parameter,
	const std::string &object_path)
{
	// 変数宣言
	OBJECT_PARAMETER_DESC param = parameter;
	Object *object = nullptr;

	if (param.layer_ == LAYER_MESH_FIELD){
		object = new Field(parameter);
		Field *field = dynamic_cast<Field*>(object);
		field->LoadMesh(object_path);
	}
	 
	else if (param.layer_ == LAYER_MODEL_X){
		object = new XModel(parameter);
		XModel *model = dynamic_cast<XModel*>(object);
		model->LoadMesh(object_path);
	}

	else if (param.layer_ == LAYER_MODEL_FBX){
		object = new FbxModel(parameter);
		FbxModel *model = dynamic_cast<FbxModel*>(object);
		model->Load(object_path);
	}

	else if (param.layer_ == LAYER_SPRITE_2D){
		object = new Sprite2D(parameter);
		Sprite2D *sprite = dynamic_cast<Sprite2D*>(object);
		sprite->SetTexture(object_path);
	}

    else if (param.layer_ == LAYER_WATER_GAGE){
        object = new WaterGage(parameter);

    }

	else{
		ASSERT_ERROR("無効なオブジェクト生成カテゴリです");
	}

	return object;
}


//-------------------------------------
// end of file
//-------------------------------------

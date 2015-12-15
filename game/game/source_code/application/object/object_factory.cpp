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
#include "objects/sprite/message.h"
#include "objects/sprite/countdown.h"
#include "objects/mesh/field.h"
#include "objects/model/x_model.h"
#include "objects/model/x/instancing_tree.h"
#include "objects/model/x/instancing_bench.h"
#include "objects/sprite3d/shadow.h"
#include "objects/sprite3d/lake.h"
#include "objects/model/fbx_model.h"
#include "objects/sprite/timer.h"
#include "objects/sprite/water_gage.h"
#include "objects/sprite/damage_effect.h"
#include "objects/model/fbx/fbx_player.h"
#include "objects/sprite/fort_gauge_manager.h"
#include "objects/model/fbx/fbx_grandfather.h"
#include "objects/model/fbx/fbx_child.h"
#include "objects/notice/bullet.h"
#include "objects/mesh/skydome.h"
#include "objects/model/x/x_fort.h"


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

    else if (param.layer_ == LAYER_DAMAGE_EFFECT){
        object = new DamageEffect(parameter);
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

	else if(param.layer_ == LAYER_TREE){
		object = new InstancingTree(parameter);
	}

	else if (param.layer_ == LAYER_BENCH){
		object = new InstancingBench(parameter);
	}

	else if(param.layer_ == LAYER_SHADOW){
		object = new Shadow(parameter);
	}

	else if (param.layer_ == LAYER_SPRITE_LAKE){
		object = new Lake(parameter);
	}

	else if (param.layer_ == LAYER_COUNTDOWN){
		object = new CountDown(parameter);
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

	else if (param.layer_ == LAYER_MESH_SKYDOME){
		object = new SkyDome(parameter);
		SkyDome *sky = dynamic_cast<SkyDome*>(object);
		sky->LoadMesh(object_path);
	}
	 
	else if (param.layer_ == LAYER_MODEL_X){
		object = new XModel(parameter);
		XModel *model = dynamic_cast<XModel*>(object);
		model->LoadMesh(object_path);
	}

	else if (param.layer_ == LAYER_MODEL_FORT){
		object = new XFort(parameter);
		XFort *fort = dynamic_cast<XFort*>(object);
		fort->LoadMesh(object_path);
	}

	else if (param.layer_ == LAYER_MODEL_FBX){
		object = new FbxModel(parameter);
		FbxModel *model = dynamic_cast<FbxModel*>(object);
		model->Load(object_path);
	}

    else if (param.layer_ == LAYER_WATER_GAGE){
        object = new WaterGage(parameter);
        WaterGage *sprite = dynamic_cast<WaterGage*>(object);
        sprite->SetTexture(object_path);
    }

    else if (param.layer_ == LAYER_FORT_GAUGE){
        object = new FortGaugeManager(parameter);
        FortGaugeManager *fort_sprite = dynamic_cast<FortGaugeManager*>(object);
        fort_sprite->SetTexture(object_path);
    }

	else if (param.layer_ == LAYER_SPRITE_2D){
		object = new Sprite2D(parameter);
		Sprite2D *sprite = dynamic_cast<Sprite2D*>(object);
		sprite->SetTexture(object_path);
	}

	else if (param.layer_ == LAYER_MESSAGE){
		object = new Message(parameter);
		Message *message = dynamic_cast<Message*>(object);
		message->SetTexture(object_path);
	}

	else{
		ASSERT_ERROR("無効なオブジェクト生成カテゴリです");
	}

	return object;
}


//-------------------------------------
// end of file
//-------------------------------------

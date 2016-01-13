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
#include "objects/sprite/message/message.h"
#include "objects/sprite/countdown.h"
#include "objects/mesh/field.h"
#include "objects/model/x_model.h"
#include "objects/model/x/instancing_tree.h"
#include "objects/model/x/instancing_bench.h"
#include "objects/model/x/instancing_playground.h"
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
#include "objects/bullet/bullet.h"
#include "objects/bullet/bomb.h"
#include "objects/mesh/skydome.h"
#include "objects/model/x/x_fort.h"
#include "../object/objects/sprite/blind.h"


//-------------------------------------
// ObjectFactory()
//-------------------------------------
Object *ObjectFactory::Create(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// 変数宣言
	OBJECT_PARAMETER_DESC param = parameter;
	Object *object = nullptr;

	switch (param.layer_)
	{
	case LAYER_SPRITE_LAKE:
		object = new Lake(parameter);
		break;
	case LAYER_MODEL_GRANDFATHER:
		object = new FbxGrandfather(parameter);
		break;
	case LAYER_MODEL_CHILD:
		object = new FbxChild(parameter);
		break;
	case LAYER_BULLET:
		object = new Bullet(parameter);
		break;
	case LAYER_BOMB:
		object = new Bomb(parameter);
		break;
	case LAYER_TREE:
		object = new InstancingTree(parameter);
		break;
	case LAYER_BENCH:
		object = new InstancingBench(parameter);
		break;
	case LAYER_PLAYGROUND:
		object = new InstancingPlaygound(parameter);
		break;
	case LAYER_SHADOW:
		object = new Shadow(parameter);
		break;
	case LAYER_DAMAGE_EFFECT:
		object = new DamageEffect(parameter);
		break;
	case LAYER_TIMER:
		object = new Timer(parameter);
		break;
	case LAYER_SPRITE_2D:
		object = new Sprite2D(parameter);
		break;
	case LAYER_FORT_GAUGE:
		break;
	case LAYER_COUNTDOWN:
		object = new CountDown(parameter);
		break;
	case LAYER_BLIND:
		object = new Blind(parameter);
		break;
	default:
		ASSERT_ERROR("無効なオブジェクト生成カテゴリです");
		break;
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

	switch (param.layer_)
	{
	case LAYER_MESH_SKYDOME:{
		object = new SkyDome(parameter);
		SkyDome *sky = dynamic_cast<SkyDome*>(object);
		sky->LoadMesh(object_path);
		break;
	}
	case LAYER_MESH_FIELD:{
		object = new Field(parameter);
		Field *field = dynamic_cast<Field*>(object);
		field->LoadMesh(object_path);
		break;
	}
	case LAYER_MODEL_X:{
		object = new XModel(parameter);
		XModel *x_model = dynamic_cast<XModel*>(object);
		x_model->LoadMesh(object_path);
		break;
	}
	case LAYER_MODEL_FBX:{
		object = new FbxModel(parameter);
		FbxModel *fbx_model = dynamic_cast<FbxModel*>(object);
		fbx_model->Load(object_path);
		break;
	}
	case LAYER_MODEL_FORT:{
		object = new XFort(parameter);
		XFort *fort = dynamic_cast<XFort*>(object);
		fort->LoadMesh(object_path);
		break;
	}
	case LAYER_MESSAGE:{
		object = new Message(parameter);
		Message *message = dynamic_cast<Message*>(object);
		message->SetTexture(object_path);
		break;
	}
	case LAYER_WATER_GAGE:{
		object = new WaterGage(parameter);
		WaterGage *water_gauge = dynamic_cast<WaterGage*>(object);
		water_gauge->SetTexture(object_path);
		break;
	}
	case LAYER_SPRITE_2D:{
		object = new Sprite2D(parameter);
		Sprite2D *sprite_2d = dynamic_cast<Sprite2D*>(object);
		sprite_2d->SetTexture(object_path);
		break;
	}
	case LAYER_FORT_GAUGE:{
		object = new FortGaugeManager(parameter);
		FortGaugeManager *fort_sprite = dynamic_cast<FortGaugeManager*>(object);
		fort_sprite->SetTexture(object_path);
		break;
	}
	default:{
		ASSERT_ERROR("無効なオブジェクト生成カテゴリです");
		break;
	}
	}

	return object;
}


//-------------------------------------
// end of file
//-------------------------------------

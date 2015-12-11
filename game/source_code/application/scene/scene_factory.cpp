//=========================================================
// scene_factory.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../../../../include/Effekseer.h"
#include "../../../../include/EffekseerRendererDX9.h"
#include "scene.h"
#include "scene_manager.h"
#include "scene_factory.h"
#include "scenes/loading.h"
#include "scenes/title.h"
#include "scenes/game.h"
#include "scenes/matching.h"
#include "scenes/result.h"


//-------------------------------------
// Create()
//-------------------------------------
Scene *SceneFactory::Create(
	const std::string &name)
{
	Scene *scene = nullptr;

	if (name == "Loading"){
		scene = new Loading();
		return scene;
	}
	if (name == "Game"){
		scene = new Game();
		return scene;
	}
	if (name == "Title"){
		scene = new Title();
		return scene;
	}
	if (name == "Matching"){
		scene = new Matching();
		return scene;
	}
	if (name == "Result"){
		scene = new Result();
		return scene;
	}

	ASSERT_ERROR("ÉVÅ[ÉìÇÃê∂ê¨ÇçsÇ¢Ç‹ÇπÇÒÇ≈ÇµÇΩ");
	return scene;
}

//-------------------------------------
// end of file
//-------------------------------------

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


Scene *SceneFactory::Create(
	const std::string &name)
{
	if (name == "Loading")
	{
		Scene *scene = new Loading();
		return scene;
	}
	if (name == "Game")
	{
		Scene *scene = new Game();
		return scene;
	}
	if (name == "Title")
	{
		Scene *scene = new Title();
		return scene;
	}

	ASSERT_ERROR("�V�[���̐������s���܂���ł���");
	return nullptr;
}

//-------------------------------------
// end of file
//-------------------------------------

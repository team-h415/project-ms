//=========================================================
// scene_factory.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENEFACTORY_H__
#define __APP_SCENE_SCENEFACTORY_H__


class SceneFactory
{
public:
	SceneFactory();
	virtual ~SceneFactory();

	static Scene *Create(const std::string &name);
};


#endif //__APP_SCENE_SCENEFACTORY_H__


//-------------------------------------
// end of file
//-------------------------------------

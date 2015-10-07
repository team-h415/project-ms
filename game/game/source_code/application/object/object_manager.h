//=========================================================
// object_manager.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __ObjectManager_H__
#define __ObjectManager_H__


//-------------------------------------
// class
//-------------------------------------
class ObjectManager
{
public:
	ObjectManager();
	virtual ~ObjectManager();
	void Update();
	void Draw();
	static Object *Create(
		const std::string &name,
		const OBJECT_PARAMETER_DESC &parameter);
private:
	static bool Search(const std::string &name);

	static std::map<std::string, Object*> objects_[LAYER_MAX];
};


#endif //__ObjectManager_H__


//-------------------------------------
// end of file
//-------------------------------------

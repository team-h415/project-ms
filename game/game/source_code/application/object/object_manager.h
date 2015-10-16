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

	//-------------------------------------
	// オブジェクト生成
	//-------------------------------------
	// 外部ファイルから読み込むデータのないオブジェクトを生成します。
	// ObjectManager::Create(
	//      "オブジェクトの名前", 
	//      設定するパラメータ);
	static Object *Create(
		const std::string &name,
		const OBJECT_PARAMETER_DESC &parameter);

	// 外部ファイルから読み込みが必要なオブジェクトを生成します。
	// ObjectManager::Create("
	//     オブジェクトの名前",
	//     設定するパラメータ,
	//     外部ファイルのパス);
	static Object *Create(
		const std::string &name,
		const OBJECT_PARAMETER_DESC &parameter,
		const std::string &object_path);

private:
	static bool Search(const std::string &name);
	static std::map<std::string, Object*> objects_[LAYER_MAX];
};


#endif //__ObjectManager_H__


//-------------------------------------
// end of file
//-------------------------------------

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
	// �I�u�W�F�N�g����
	//-------------------------------------
	// �O���t�@�C������ǂݍ��ރf�[�^�̂Ȃ��I�u�W�F�N�g�𐶐����܂��B
	// ObjectManager::Create(
	//      "�I�u�W�F�N�g�̖��O", 
	//      �ݒ肷��p�����[�^);
	static Object *Create(
		const std::string &name,
		const OBJECT_PARAMETER_DESC &parameter);

	// �O���t�@�C������ǂݍ��݂��K�v�ȃI�u�W�F�N�g�𐶐����܂��B
	// ObjectManager::Create("
	//     �I�u�W�F�N�g�̖��O",
	//     �ݒ肷��p�����[�^,
	//     �O���t�@�C���̃p�X);
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

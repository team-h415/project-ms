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
	Object *Create(
		const std::string &name,
		const OBJECT_PARAMETER_DESC &parameter);

	// �O���t�@�C������ǂݍ��݂��K�v�ȃI�u�W�F�N�g�𐶐����܂��B
	// ObjectManager::Create(
	//     "�I�u�W�F�N�g�̖��O",
	//     �ݒ肷��p�����[�^,
	//     �O���t�@�C���̃p�X);
	Object *Create(
		const std::string &name,
		const OBJECT_PARAMETER_DESC &parameter,
		const std::string &object_path);

	//-------------------------------------
	// �I�u�W�F�N�g�擾
	//-------------------------------------
	// �I�u�W�F�N�g�𖼑O���猟�����A�|�C���^��Ԃ��܂��B
	// ObjectManager::Get(
	//     "�I�u�W�F�N�g�̖��O");
	Object *Get(
		const std::string &name);

	//-------------------------------------
	// �w�背�C���[�̃I�u�W�F�N�g�폜
	//-------------------------------------
	// �w�肵�����C���[�̃R���e�i���N���A���܂�
	// ObjectManager::Clear(
	//     "�I�u�W�F�N�g�̃��C���[");
	void Clear(
		OBJECT_LAYER layer);

private:
	bool Search(const std::string &name);
	std::map<std::string, Object*> objects_[LAYER_MAX];
};


#endif //__ObjectManager_H__


//-------------------------------------
// end of file
//-------------------------------------

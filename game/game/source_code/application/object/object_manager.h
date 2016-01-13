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
// Forward declaration
//-------------------------------------
struct OBJECT_PARAMETER_DESC;
enum OBJECT_LAYER;
class Object;
class Bullet;
class Bomb;
class Blind;

//-------------------------------------
// class
//-------------------------------------
class ObjectManager
{
public:
	static ObjectManager* Get();
	static void Delete();

	void Update();
	void DeleteCheck();
	void Draw();

	//-------------------------------------
	// �I�u�W�F�N�g����
	//-------------------------------------
	// �O���t�@�C������ǂݍ��ރf�[�^�̂Ȃ��I�u�W�F�N�g�𐶐����܂��B
	// ObjectManager::Create(
	//      "�I�u�W�F�N�g�̖��O", 
	//      �ݒ肷��p�����[�^);
	Object *Create(
		const OBJECT_PARAMETER_DESC &parameter);

	// �O���t�@�C������ǂݍ��݂��K�v�ȃI�u�W�F�N�g�𐶐����܂��B
	// ObjectManager::Create(
	//     "�I�u�W�F�N�g�̖��O",
	//     �ݒ肷��p�����[�^,
	//     �O���t�@�C���̃p�X);
	Object *Create(
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
	// ���ׂẴI�u�W�F�N�g�̎g�p�t���O��OFF��
	//-------------------------------------
	// �w�背�C���[���̃I�u�W�F�N�g�����ׂĖ��g�p�ɂ��郁�\�b�h
	// ObjectManager::GetUseOffBomb(
	//     "�I�u�W�F�N�g�̃��C���[");
	void ObjectUseOffLayer(
		OBJECT_LAYER layer);

	//-------------------------------------
	// ���g�p�I�u�W�F�N�g�̎擾
	//-------------------------------------
	// �w�背�C���[���̖��g�p�I�u�W�F�N�g���擾���邽�߂̃��\�b�h
	// ObjectManager::GetUseOffBomb(
	//     "�I�u�W�F�N�g�̃��C���[");
	Object *GetUseOffLayer(
		OBJECT_LAYER layer);

	//-------------------------------------
	// �w�背�C���[�̃I�u�W�F�N�g�폜
	//-------------------------------------
	// �w�肵�����C���[�̃R���e�i���N���A���܂�
	// ObjectManager::Clear(
	//     "�I�u�W�F�N�g�̃��C���[");
	void Clear(
		OBJECT_LAYER layer);

	//-------------------------------------
	// �I�u�W�F�N�g�̐��𐔂���
	//-------------------------------------
	// ObjectManager::GetCount();
	static int GetCount(){
		return object_count_;
	}

	//-------------------------------------
	// �I�u�W�F�N�g�̏����t�@�C���ɏo��
	//-------------------------------------
	void ExportObjectParameter(
		const std::string &file_path);

	//-------------------------------------
	// �`��t���O�ݒ�
	//-------------------------------------
	void SetDrawEnable(
		int layer, bool flag){
		draw_enable_[layer] = flag;
	}

private:
	ObjectManager();
	~ObjectManager();

	static ObjectManager* object_manager_;

	bool Search(const std::string &name);
	std::unordered_map<std::string, Object*> objects_[LAYER_MAX];
	static int object_count_;
	bool draw_enable_[LAYER_MAX];
};


#endif //__ObjectManager_H__


//-------------------------------------
// end of file
//-------------------------------------

//=========================================================
// scene_manager.h
// author;ryuya nakamura
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENEMANAGER_H__
#define __APP_SCENE_SCENEMANAGER_H__


using namespace std;
class Fade;
class MyThread;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();
	// �V�[���̍X�V����
	void Update();
	// �V�[���̕`�揈��
	void Draw();
	// �V�[���̐؂�ւ�����
	static void RequestScene(const std::string &name);
	// ���݂̃V�[�����󂯓n��
	static Scene *GetCurrentScene(){
		return current_scene_;
	}
	// ���݂̃V�[�������擾
	static const std::string &GetCurrentSceneName(){
		return current_name_;
	}
private:
	// �V�[���̐���
	static Scene *Create(const std::string &name);
	// �V�[���؂�ւ����s
	void ChangeScene();
	// �V�[���쐬���s
	static void CreateScene();

	// ���݂̃V�[�����
	static Scene *current_scene_;
	// ���݂̃V�[����
	static std::string current_name_;
	// ���[�h��ʂ̃V�[�����
	static Scene *loading_scene_; 
	// ���N�G�X�g���ꂽ���̃V�[����
	static std::string next_name_;
	// ���N�G�X�g����t���O
	static bool next_request_;
	// �X���b�h�g�p�t���O
	static bool loading_;
	// �t�F�[�h
	static Fade *fade_;
	// �X���b�h
	static MyThread *thread_;

};

#endif //__APP_SCENE_SCENEMANAGER_H__


//-------------------------------------
// end of file
//-------------------------------------

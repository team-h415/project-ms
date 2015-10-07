//=========================================================
// application.h
// author:ryuya nakamura
// �A�v���P�[�V�����̊Ǘ�
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_APPLICATION_H__
#define __APP_APPLICATION_H__

//-------------------------------------
// class definitin
//-------------------------------------
class Renderer;
class Fps;
class SceneManager;
class Input;
class Application
{
public:

	// �A�v���P�[�V�����̎��s
	static void Run(
		HINSTANCE instance,
		int command_show);


private:
	// �R���X�g���N�^
	Application(
		HINSTANCE instance,
		int command_show);
	// �f�X�g���N�^
	virtual ~Application();

	// �X�V
	void Update();
	// �`��
	void Draw();

	// �����o�ϐ�
	Renderer *renderer_;
	static Fps *fps_;
	SceneManager *scene_manager_;
	Input *input_;
};


#endif //__APP_APPLICATION_H__


//-------------------------------------
// end of file
//-------------------------------------

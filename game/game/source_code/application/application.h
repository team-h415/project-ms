//=========================================================
// application.h
// author:ryuya nakamura
// アプリケーションの管理
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
class GameRenderer;
class Fps;
class SceneManager;
class Input;
class Application
{
public:

	// アプリケーションの実行
	static void Run(
		HINSTANCE instance,
		int command_show);


private:
	// コンストラクタ
	Application(
		HINSTANCE instance,
		int command_show);
	// デストラクタ
	virtual ~Application();

	// 更新
	void Update();
	// 描画
	void Draw();

	// メンバ変数
	GameRenderer *renderer_;
	static Fps *fps_;
	SceneManager *scene_manager_;
	Input *keyboard_;
	Input *gamepad_;
};


#endif //__APP_APPLICATION_H__


//-------------------------------------
// end of file
//-------------------------------------

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
class Renderer;
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
	Renderer *renderer_;
	static Fps *fps_;
	SceneManager *scene_manager_;
	Input *input_;
};


#endif //__APP_APPLICATION_H__


//-------------------------------------
// end of file
//-------------------------------------

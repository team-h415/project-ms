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
	// シーンの更新処理
	void Update();
	// シーンの描画処理
	void Draw();
	// シーンの切り替え処理
	static void RequestScene(const std::string &name);
	// 現在のシーンを受け渡す
	static Scene *GetCurrentScene(){
		return current_scene_;
	}
	// 現在のシーン名を取得
	static const std::string &GetCurrentSceneName(){
		return current_name_;
	}
private:
	// シーンの生成
	static Scene *Create(const std::string &name);
	// シーン切り替え実行
	void ChangeScene();
	// シーン作成実行
	static void CreateScene();

	// 現在のシーン情報
	static Scene *current_scene_;
	// 現在のシーン名
	static std::string current_name_;
	// ロード画面のシーン情報
	static Scene *loading_scene_; 
	// リクエストされた次のシーン名
	static std::string next_name_;
	// リクエスト判定フラグ
	static bool next_request_;
	// スレッド使用フラグ
	static bool loading_;
	// フェード
	static Fade *fade_;
	// スレッド
	static MyThread *thread_;

};

#endif //__APP_SCENE_SCENEMANAGER_H__


//-------------------------------------
// end of file
//-------------------------------------

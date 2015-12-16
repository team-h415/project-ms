//=========================================================
// effect_manager.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __EffectManager_H__
#define __EffectManager_H__


//-------------------------------------
// class
//-------------------------------------
using namespace Effekseer;
using namespace EffekseerRenderer;
class EffectManager
{
public:
	static EffectManager* Get();
	static void Delete();

	void Update();
	void Draw();
	void Create(
		const std::string &name,
		const std::string &path,
		const EFFECT_PARAMETER_DESC &parameter);
	void Play(
		const std::string &name);
	void Stop(
		const std::string &name);
	void StopAll();
	MyEffect *Get(
		const std::string &name);
	static int GetCount(){
		return effect_count_;
	}

private:
	void SetViewMatrix();
	void SetProjectionMatrix();
	EffekseerRenderer::Renderer *renderer_;
	Effekseer::Manager *manager_;
	std::map<std::string, MyEffect*> effects_;

	EffectManager(
		const int max_sprites);
	virtual ~EffectManager();

	static EffectManager* effect_manager_;
	static int effect_count_;
};


#endif //__EffectManager_H__


//-------------------------------------
// end of file
//-------------------------------------

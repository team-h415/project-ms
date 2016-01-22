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
// enum
//-------------------------------------
enum EFFECT_STATE
{
	EFFECT_ADD,
	EFFECT_SET_PARAM,
	EFFECT_STOP
};


//-------------------------------------
// struct
//-------------------------------------
struct STOCK_EFFECT_PARAM
{
	EFFECT_STATE state_;
	Vector3 position_;
	Vector3 rotation_;
	Vector3 scaling_;
	std::string name_;

	STOCK_EFFECT_PARAM()
	{
		state_ = EFFECT_STOP;
		position_ = {0.0f, 0.0f, 0.0f};
		rotation_ = {0.0f, 0.0f, 0.0f};
		scaling_ = {1.0f, 1.0f, 1.0f};
	};
};


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

	void StockEffect(STOCK_EFFECT_PARAM stock_param);

private:

	void AdaptationEffect();
	void SetViewMatrix();
	void SetProjectionMatrix();
	EffekseerRenderer::Renderer *renderer_;
	Effekseer::Manager *manager_;
	std::unordered_map<std::string, MyEffect*> effects_;
	std::list<STOCK_EFFECT_PARAM> stock_effect_;
	CRITICAL_SECTION critical_section_;

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

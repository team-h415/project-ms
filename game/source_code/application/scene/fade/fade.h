//=========================================================
// Fade.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_FADE_H__
#define __APP_SCENE_FADE_H__


//-------------------------------------
// struct
//-------------------------------------
enum FADE_PARAMETER
{
	FADE_NONE = 0,	// �t�F�[�h��
	FADE_IN,		// �t�F�[�h�C��(�Ó])
	FADE_OUT,		// �t�F�[�h�A�E�g(�����Ă�)
	FADE_MAX,
};


//-------------------------------------
// class
//-------------------------------------
class Fade
{
public:
	Fade();
	virtual ~Fade();
	void Update();
	static void Draw();
	static void SetFade(FADE_PARAMETER request){
		request_ = request;
	}
	static FADE_PARAMETER GetFade(){
		return param_;
	}

private:
	static Vertex2D vertex_[4];
	static FADE_PARAMETER param_;
	static FADE_PARAMETER request_;
	float alpha_;
};


#endif //__APP_SCENE_FADE_H__


//-------------------------------------
// end of file
//-------------------------------------

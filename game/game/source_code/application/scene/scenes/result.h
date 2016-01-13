//=========================================================
// result.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Result_H__
#define __Result_H__


//-------------------------------------
// class
//-------------------------------------
class Sprite2D;
class Result : public Scene
{
public:
	Result();
	virtual ~Result();
	void Initialize();
	void Update();
	void Draw();
private:
	Sprite2D*	background_;
};


#endif //__Result_H__


//-------------------------------------
// end of file
//-------------------------------------

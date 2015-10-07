//=========================================================
// sprite2d.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Sprite2D_H__
#define __Sprite2D_H__


//-------------------------------------
// class
//-------------------------------------
class Sprite2D : public Object
{
public:
	Sprite2D();
	virtual ~Sprite2D();

	void Update();
	void Draw();

private:
};


#endif //__Sprite2D_H__


//-------------------------------------
// end of file
//-------------------------------------

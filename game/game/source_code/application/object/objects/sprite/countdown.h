//=========================================================
// CountDown.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __CountDown_H__
#define __CountDown_H__


//-------------------------------------
// class
//-------------------------------------
class CountDown : public Object
{
public:
	CountDown(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~CountDown();
	void Update();
	void Draw();
	void ChangeTexture(int time){
		timer_ = time;
	}

private:
	void CalculateVertex();
	int timer_;
	Vertex2D *vertex_;
	LPDIRECT3DTEXTURE9 texture_[5];
};


#endif //__CountDown_H__


//-------------------------------------
// end of file
//-------------------------------------

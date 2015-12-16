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
	void Play(int time);

private:
	void CalculateVertex();
	int timer_;
	float alpah;
	int frame_count;
	Vertex2D *vertex_;
	Vector2 base_scaling_;
	LPDIRECT3DTEXTURE9 texture_[5];
};


#endif //__CountDown_H__


//-------------------------------------
// end of file
//-------------------------------------

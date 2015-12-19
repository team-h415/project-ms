//=========================================================
// message.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Message_H__
#define __Message_H__


//-------------------------------------
// class
//-------------------------------------
class Message : public Sprite2D
{
public:
	Message(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Message();
	void Update();
	int GetTimer(){
		return timer_;
	}
	void Play(){
		play_ = true;
		timer_ = 0;
		alpha_ = 0.0f;
	}

private:
	void CalculateVertex();
	int timer_;
	float alpha_;
	bool play_;
};


#endif //__Message_H__


//-------------------------------------
// end of file
//-------------------------------------

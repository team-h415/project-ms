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
private:
};


#endif //__Message_H__


//-------------------------------------
// end of file
//-------------------------------------

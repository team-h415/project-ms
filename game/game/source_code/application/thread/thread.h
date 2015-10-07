//=========================================================
// thread.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_THREAD_THREAD_H__
#define __APP_THREAD_THREAD_H__


//-------------------------------------
// class
//-------------------------------------
class Thread
{
public:
	Thread();
	virtual ~Thread();
	unsigned int Create(void *function);
private:
	HANDLE thread_;
};


#endif //__APP_THREAD_THREAD_H__


//-------------------------------------
// end of file
//-------------------------------------

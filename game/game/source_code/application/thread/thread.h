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
class MyThread
{
public:
	MyThread();
	virtual ~MyThread();
	unsigned int Create(void *function);
	void CloseThread(bool safe = true);
private:
	HANDLE thread_;
};


#endif //__APP_THREAD_THREAD_H__


//-------------------------------------
// end of file
//-------------------------------------

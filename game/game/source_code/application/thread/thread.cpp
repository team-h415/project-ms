//=========================================================
// thread.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "thread.h"



//-------------------------------------
// Thread()
//-------------------------------------
MyThread::MyThread()
{
}


//-------------------------------------
// ~Thread()
//-------------------------------------
MyThread::~MyThread()
{
	CloseThread();
}


//-------------------------------------
// Create()
//-------------------------------------
unsigned int MyThread::Create(void* function)
{
	unsigned int thread_id;
	thread_ = (HANDLE)_beginthreadex(
		NULL,
		0,
		(unsigned int(__stdcall*)(void*))function,
		NULL,
		0,
		&thread_id);
	if (!thread_)
	{
		ASSERT_ERROR("ÉXÉåÉbÉhê∂ê¨Ç…é∏îsÇµÇ‹ÇµÇΩ");
		return 0;
	}

	return (unsigned int)thread_;
}


//-------------------------------------
// ThreadEnd()
//-------------------------------------
void MyThread::CloseThread(bool safe)
{
	if(thread_)
	{
		if(safe)
		{
			WaitForSingleObject(thread_, INFINITE);
		}
		BOOL result;
		result = CloseHandle(thread_);
		thread_ = 0;
	}
}


//-------------------------------------
// end of file
//-------------------------------------

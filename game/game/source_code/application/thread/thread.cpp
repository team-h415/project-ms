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
Thread::Thread()
{
}


//-------------------------------------
// ~Thread()
//-------------------------------------
Thread::~Thread()
{
	HRESULT result;
	if (thread_)
	{
		WaitForSingleObject(thread_, INFINITE);
		result = CloseHandle(thread_);
	}
	else
	{
		ASSERT_WARNING("�X���b�h�͓����Ă��܂���");
	}
}


//-------------------------------------
// Create()
//-------------------------------------
unsigned int Thread::Create(void* function)
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
		ASSERT_ERROR("�X���b�h�����Ɏ��s���܂���");
		return 0;
	}

	return (unsigned int)thread_;
}


//-------------------------------------
// end of file
//-------------------------------------

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
	BOOL result;
	if (thread_)
	{
		WaitForSingleObject(thread_, INFINITE);
		result = CloseHandle(thread_);
		if (!result){
			ASSERT_ERROR("スレッドの破棄に失敗");
		}
	}
	else
	{
		ASSERT_WARNING("スレッドは動いていません");
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
		ASSERT_ERROR("スレッド生成に失敗しました");
		return 0;
	}

	return (unsigned int)thread_;
}


//-------------------------------------
// end of file
//-------------------------------------

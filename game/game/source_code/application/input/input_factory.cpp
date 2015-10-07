//=========================================================
// input_factory.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "input.h"
#include "inputs/keyboard.h"
#include "input_factory.h"


//-------------------------------------
// Create()
//-------------------------------------
Input *InputFactory::Create(
	const std::string &category)
{
	Input *input = NULL;
	if (category == "KeyBoard")
	{
		input = new KeyBoard;
	}

	if (!input)
	{
		ASSERT_WARNING("���̓f�o�C�X���쐬����܂���ł���");
	}

	return input;
}


//-------------------------------------
// end of file
//-------------------------------------

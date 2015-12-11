//=========================================================
// input_factory.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_INPUT_INPUTFACTORY_H__
#define __APP_INPUT_INPUTFACTORY_H__


//-------------------------------------
// class
//-------------------------------------
class InputFactory
{
public:
	static Input *Create(
		const std::string &category);
private:

};


#endif //__APP_INPUT_INPUTFACTORY_H__


//-------------------------------------
// end of file
//-------------------------------------

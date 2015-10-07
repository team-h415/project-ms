//=========================================================
// object_factory.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__


//-------------------------------------
// class
//-------------------------------------
class ObjectFactory
{
public:
	static Object *Create(
		const OBJECT_PARAMETER_DESC &parameter);
};


#endif //__ObjectFactory_H__


//-------------------------------------
// end of file
//-------------------------------------

//=========================================================
// MyMath.h
// author:ryuya nakamura
//=========================================================


//=========================================================
// include guard
//=========================================================
#pragma once
#ifndef __APP_MATH_MYMATH_H__
#define __APP_MATH_MYMATH_H__


//=========================================================
// class
//=========================================================
class MyMath
{
public:
	MyMath();
	virtual ~MyMath();
	
	template <typename T>
	
	static T Random_Range(T min, T max){
		T value;
		value = min + static_cast<T>(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
		return value;
	}
private:

};


#endif //__APP_MATH_MYMATH_H__


//=========================================================
// end of file
//=========================================================

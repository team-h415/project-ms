//=========================================================
// Vector.h
// author:ryuya nakamura
//=========================================================


//=========================================================
// include guard
//=========================================================
#pragma once
#ifndef __APP_MATH_VECTOR__
#define __APP_MATH_VECTOR__


//=========================================================
// struct
//=========================================================
struct Vector2
{
	Vector2() : x_(0.0f), y_(0.0f){}
	Vector2(float x, float y) :
		x_(x), y_(y){}
	float x_, y_;
};

struct Vector3
{
	Vector3() : x_(0.0f), y_(0.0f), z_(0.0f){}
	Vector3(float x, float y, float z) :
		x_(x), y_(y), z_(z){}
	Vector3(const Vector3 &v) :
		x_(v.x_), y_(v.y_), z_(v.z_){}

	Vector3 operator * (float) const;
	Vector3 operator / (float) const;
	Vector3 operator + (const Vector3&) const; 
	Vector3 operator - (const Vector3&) const;
	Vector3 operator + () const;
	Vector3 operator - () const;
	Vector3& operator += (const Vector3&);
	Vector3& operator -= (const Vector3&);
	Vector3& operator *= (float);
	Vector3& operator /= (float);
	float x_, y_, z_;
};

float Vec3LengthPow2(const Vector3 &in);
float Vec3Length(const Vector3 &in);
float Vec3LengthReverce(const Vector3 &in);
void Vec3Normalize(Vector3 &out, const Vector3 &in);


#endif //__ADD_YOUR_HANDS__


//=========================================================
// end of file
//=========================================================

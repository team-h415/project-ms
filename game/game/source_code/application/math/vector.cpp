//=========================================================
// Vector.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "vector.h"


//-------------------------------------
// operator*()
//-------------------------------------
Vector3 Vector3::operator*(float f) const
{
	return Vector3(x_ * f, y_ * f, z_ * f);
}


//-------------------------------------
// operator/()
//-------------------------------------
Vector3 Vector3::operator/(float f) const
{
	return Vector3(x_ / f, y_ / f, z_ / f);
}

//-------------------------------------
// operator+()
//-------------------------------------
Vector3 Vector3::operator+(const Vector3 &v) const
{
	return Vector3(x_ + v.x_, y_ + v.y_, z_ + v.z_);
}

Vector3 Vector3::operator+() const
{
	return *this;
}


//-------------------------------------
// operator-()
//-------------------------------------
Vector3 Vector3::operator-(const Vector3 &v) const
{
	return Vector3(x_ - v.x_, y_ - v.y_, z_ - v.z_);
}

Vector3 Vector3::operator-() const
{
	return *this;
}


//-------------------------------------
// operator*=
//-------------------------------------
Vector3& Vector3::operator*=(float f)
{
	x_ *= f;
	y_ *= f;
	z_ *= f;
	return *this;
}

//-------------------------------------
// operator/=
//-------------------------------------
Vector3& Vector3::operator /= (float f)
{
	float fInv = 1.0f / f;
	x_ *= fInv;
	y_ *= fInv;
	z_ *= fInv;
	return *this;
}

//-------------------------------------
// operator+=
//-------------------------------------
Vector3& Vector3::operator += (const Vector3& v)
{
	x_ += v.x_;
	y_ += v.y_;
	z_ += v.z_;
	return *this;
}

//-------------------------------------
// operator-=
//-------------------------------------
Vector3& Vector3::operator -= (const Vector3& v)
{
	x_ -= v.x_;
	y_ -= v.y_;
	z_ -= v.z_;
	return *this;
}


//-------------------------------------
// Vec3LengthPow2()
//-------------------------------------
float Vec3LengthPow2(const Vector3 &in)
{
	return ((in.x_ * in.x_) + (in.y_ * in.y_) + (in.z_ * in.z_));
}


//-------------------------------------
// Vec3Length()
//-------------------------------------
float Vec3Length(const Vector3 &in)
{
	return (sqrtf(Vec3LengthPow2(in)));
}


//-------------------------------------
// Vec3LengthReverce()
//-------------------------------------
float Vec3LengthReverce(const Vector3 &in)
{
	return (1.0f / Vec3Length(in));
}


//-------------------------------------
// Vec3Normalize()
//-------------------------------------
void Vec3Normalize(Vector3 &out, const Vector3 &in)
{
	float f = Vec3LengthReverce(in);
	out = in * f;
}


//-------------------------------------
// end of file
//-------------------------------------

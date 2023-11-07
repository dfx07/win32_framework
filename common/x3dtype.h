////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x3dtype.h
* @create   Nov 15, 2023
* @brief    Geometry3D type define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef X3DTYPE_H
#define X3DTYPE_H

#include "xgeodef.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// TYPE BASE VEC DEFINE

template<typename T>
struct tagVec<3, T>
{
	typedef T	value_type;
	typedef tagVec<3, T> type;

public:
	union { value_type x, r, s; };
	union { value_type y, g, t; };
	union { value_type z, b, p; };

public:
	tagVec<3, T>() : x(static_cast<T>(0))
	{

	}

	template<typename U>
	tagVec<3, T>(const tagVec<3, U>& v)
		: x(static_cast<T>(v.x)), y(static_cast<T>(v.y)),
		z(static_cast<T>(v.z))
	{

	}

	template<typename U>
	tagVec<3, T>(const tagVec<2, U>& v)
		: x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
	{

	}

	template<typename U>
	tagVec<3, T>(U xv, U yv, U zv)
		: x(static_cast<T>(xv)), y(static_cast<T>(yv)),
		z(static_cast<T>(zv))
	{

	}

	template<typename A, typename B, typename C>
	tagVec<3, T>(const A& xv, const B& yv, const C& zv)
		: x(static_cast<T>(xv)), y(static_cast<T>(yv)),
		z(static_cast<T>(zv))
	{

	}

public:
	template<typename U>
	constexpr tagVec<3, T>& operator=(const tagVec<3, U>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		return *this;
	}

	template<typename U>
	constexpr tagVec<3, T> operator+(const tagVec<3, U>& v) const
	{
		return tagVec<3, T>(x + v.x, y + v.y);
	}

	template<typename U>
	tagVec<3, T>& operator+=(const tagVec<3, U>& v)
	{
		x = x + v.x;
		y = y + v.y;
		z = z + v.z;
		return *this;
	}

	template<typename U>
	constexpr tagVec<3, T> operator-(tagVec<3, U> const& v) const
	{
		return tagVec<2, T>(x - v.x, y - v.y);
	}

	template<typename U>
	tagVec<3, T>& operator-=(const tagVec<3, U>& v)
	{
		x = x - v.x;
		y = y - v.y;
		z = z - v.z;
		return *this;
	}

	template<typename U>
	tagVec<3, T> operator/(const U& value) const
	{
		return tagVec<3, T>(static_cast<T>(x / value),
			static_cast<T>(y / value),
			static_cast<T>(z / value));
	}

	template<typename U>
	tagVec<3, T>& operator/=(const U& value)
	{
		x = static_cast<T>(x / value);
		y = static_cast<T>(y / value);
		z = static_cast<T>(z / value);
		return *this;
	}

	template<typename U>
	tagVec<3, T> operator*(const U& value) const
	{
		return tagVec<3, T>(static_cast<T>(x * value),
			static_cast<T>(y * value),
			static_cast<T>(z * value));
	}

	template<typename U>
	tagVec<3, T>& operator*=(const U& value)
	{
		x = static_cast<T>(x * value);
		y = static_cast<T>(y * value);
		z = static_cast<T>(z * value);
		return *this;
	}
};

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Type DEFINE

typedef tagVec<3, float>		Vec3D;
typedef tagVec<3, float>		Point3D;
typedef tagVec<3, float>		Color3;

typedef std::vector<Point3D>	VecPoint3D;

#endif //!X3DTYPE_H


////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgeotype.h
* @create   May 19, 2023
* @brief    Define type geometry 
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGEOTYPE_H
#define XGEOTYPE_H

#include "xmath.h"
#include "glm/glm.hpp"
#include <vector>

___BEGIN_NAMESPACE___

/////////////////////////////////////////////////////////////////////////////////////
// User define type

template<int dimention, typename T> struct tagVec;

template<typename T>
struct tagVec<2, T>
{
	typedef T	value_type;
	typedef tagVec<2, T> type;

public:
	value_type x;
	value_type y;

public:
	tagVec<2, T>() : x(static_cast<T>(0))
	{

	}

	template<typename U>
	tagVec<2, T>(const tagVec<2, U>& tagVec)
		: x(static_cast<T>(tagVec.x)), y(static_cast<T>(tagVec.y))
	{

	}

	template<typename U>
	tagVec<2, T>(U xv, U yv)
		: x(static_cast<T>(xv)), y(static_cast<T>(yv))
	{

	}

public:
	template<typename U>
	constexpr tagVec<2, T>& operator=(const tagVec<2, U>& tagVec)
	{
		x = static_cast<T>(tagVec.x);
		y = static_cast<T>(tagVec.y);
		return *this;
	}

	template<typename U>
	constexpr tagVec<2, T> operator+(const tagVec<2, U>& v) const
	{
		return tagVec<2, T>(x + v.x, y + v.y);
	}

	template<typename U>
	tagVec<2, T>& operator+=(const tagVec<2, U>& v)
	{
		x = x + v.x;
		y = y + v.y;
		return *this;
	}

	template<typename U>
	constexpr tagVec<2, T> operator-(tagVec<2, U> const& v) const
	{
		return tagVec<2, T>(x - v.x, y - v.y);
	}

	template<typename U>
	tagVec<2, T>& operator-=(const tagVec<2, U>& tagVec)
	{
		x = x - tagVec.x;
		y = y - tagVec.y;
		return *this;
	}

	template<typename U>
	tagVec<2, T> operator/(const U& value) const
	{
		return tagVec<2, T>(static_cast<T>(x / value),
							static_cast<T>(y / value));
	}

	template<typename U>
	tagVec<2, T>& operator/=(const U& value)
	{
		x = static_cast<T>(x / value);
		y = static_cast<T>(y / value);
		return *this;
	}

	template<typename U>
	tagVec<2, T> operator*(const U& value) const
	{
		return tagVec<2, T>(static_cast<T>(x * value),
							static_cast<T>(y * value));
	}

	template<typename U>
	tagVec<2, T>& operator*=(const U& value)
	{
		x = static_cast<T>(x * value);
		y = static_cast<T>(y * value);
		return *this;
	}
};

struct tagVec3D
{
	float x;
	float y;
	float z;
};

struct tagRect2D
{
	float x;
	float y;
	float width;
	float height;
};

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Define base type
typedef float		FLOAT;
typedef int			BOOL;
typedef int			INT;

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Define use GLM library
#ifdef GLM_SETUP_INCLUDED_1
	typedef glm::vec2				Vec2D;
	typedef glm::vec2				Point2D;
	typedef glm::vec3				Vec3D;
	typedef fox::tagRect2D			Rect2D;

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Define use User define
#else
	typedef fox::tagVec<2, FLOAT>	Vec2D;
	typedef fox::tagVec<2, FLOAT>	Point2D;
	typedef fox::tagVec3D			Vec3D;
	typedef fox::tagRect2D			Rect2D;
#endif
typedef std::vector<Vec2D>		VecPoint2D;


____END_NAMESPACE____

#endif // XGEOTYPE_H
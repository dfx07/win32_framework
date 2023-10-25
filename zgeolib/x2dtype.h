////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x2dtype.h
* @create   Nov 15, 2023
* @brief    Geometry2D type define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef X2DTYPE_H
#define X2DTYPE_H

#include "xgeodef.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////
// User define type

template<int dimention, typename T> struct tagVec;

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// TYPE BASE VEC DEFINE

template<typename T>
struct tagVec<2, T>
{
	typedef T	value_type;
	typedef tagVec<2, T> type;

public:
	union { value_type x, r, s ;};
	union { value_type y, g, t ;};

public:
	tagVec<2, T>() : x(static_cast<T>(0))
	{

	}

	template<typename U>
	tagVec<2, T>(const tagVec<2, U>& v)
		: x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
	{

	}

	template<typename U>
	tagVec<2, T>(U xv, U yv)
		: x(static_cast<T>(xv)), y(static_cast<T>(yv))
	{

	}

	template<typename A, typename B>
	tagVec<2, T>(const A& xv, const B& yv)
		: x(static_cast<T>(xv)), y(static_cast<T>(yv))
	{

	}

public:
	template<typename U>
	constexpr tagVec<2, T>& operator=(const tagVec<2, U>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
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
	tagVec<2, T>& operator-=(const tagVec<2, U>& v)
	{
		x = x - v.x;
		y = y - v.y;
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

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// RECTANGLE DEFINE

template<typename T>
struct tagRect
{
	typedef T	value_type;
	typedef tagRect<T> type;

public:
	value_type x;
	value_type y;
	value_type width;
	value_type height;

public:
	tagRect<T>() :x(static_cast<T>(0)), y(static_cast<T>(0)),
			width(static_cast<T>(0)), height(static_cast<T>(0))
	{

	}

	template<typename U>
	tagRect<T>(const U& _x, const U& _y, const U& w, const U& h) :
			x(static_cast<T>(_x)), y(static_cast<T>(_y)),
			width(static_cast<T>(w)), height(static_cast<T>(h))
	{

	}

	template<typename U>
	tagRect<T>(const U& _x, const U& _y, const tagVec<2, U>& _size) :
		x(static_cast<T>(_x)), y(static_cast<T>(_y)),
		width(static_cast<T>(_size.x)), height(static_cast<T>(_size.y))
	{

	}

	template<typename A, typename B>
	bool Contain(const A& _x, const B& _y)
	{
		return  ((_x >= x && _x <= x + width) &&
				(_y >= y && _y <= y + height));
	}

	tagVec<2, T> TopLeft()
	{
		return tagVec<2, T>(x, y);
	}

	tagVec<2, T> TopRight()
	{
		return tagVec<2, T>(x + width, y);
	}

	tagVec<2, T> BottomLeft()
	{
		return tagVec<2, T>(x, y - height);
	}

	tagVec<2, T> BottomRight()
	{
		return tagVec<2, T>(x + width, y - height);
	}
};

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// LINE DEFINE

template<typename T>
struct tagLine
{
	typedef T	value_type;
	typedef tagLine<T> type;

	typedef tagVec<2, T> value_type_point;

public:
	value_type_point ptStart;
	value_type_point ptEnd;

	tagLine()
	{
		ptStart = tagVec<2, T>(0, 0);
		ptEnd   = tagVec<2, T>(0, 0);
	}

	template<typename U, typename V>
	tagLine(const tagVec<2, U> ptS, const tagVec<2, V> ptE)
	{
		ptStart = value_type_point(ptS);
		ptEnd = value_type_point(ptE);
	}
};


/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Type DEFINE

typedef tagVec<2, float>	Vec2D;
typedef tagVec<2, float>	Point2D;
typedef tagRect<float>		Rect2D;
typedef std::vector<Vec2D>	VecPoint2D;

typedef tagLine<float>		Line2D;
typedef std::vector<Line2D>	VecLine2D;

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// ENUM DEFINE

enum EnumOrien
{
	COLLINEAR,
	LEFT,
	RIGHT,
};

#endif //!X2DTYPE_H


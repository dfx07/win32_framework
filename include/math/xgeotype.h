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

template<typename T>
struct tagVec<3, T>
{
	typedef T	value_type;
	typedef tagVec<3, T> type;

public:
	union { value_type x, r, s ;};
	union { value_type y, g, t ;};
	union { value_type z, b, p ;};

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
			x(static_cast<T>(_x)), y(static_cast<T>(_x)),
			width(static_cast<T>(w)), height(static_cast<T>(h))
	{

	}

	template<typename A, typename B>
	bool Contain(const A& _x, const B& _y)
	{
		return  ((_x >= x && _x <= x + width) &&
				(_y >= y && _y <= y + height));
	}
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
#ifdef GLM_SETUP_INCLUDED
	typedef glm::vec2				Vec2D;
	typedef glm::vec2				Point2D;
	typedef glm::vec3				Vec3D;
	typedef glm::vec3				Point3D;
	typedef glm::vec3				Color3;
	typedef fox::tagRect<FLOAT>		Rect2D;

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Define use User define
#else
	typedef fox::tagVec<2, FLOAT>	Vec2D;
	typedef fox::tagVec<2, FLOAT>	Point2D;
	typedef fox::tagVec<3, FLOAT>	Vec3D;
	typedef fox::tagVec<3, FLOAT>	Point3D;
	typedef fox::tagVec<3, FLOAT>	Color3;
	typedef fox::tagRect<FLOAT>		Rect2D;
#endif
typedef std::vector<Vec2D>		VecPoint2D;


/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Define use GLM library
struct VERTEX_3D 
{
	Point3D		pt;
	Color3		color;
	Vec2D		texture;

public:
	VERTEX_3D() : pt(0.f, 0.f, 0.f),
		color(0.f, 0.f, 0.f),
		texture(0.f, 0.f)
	{

	}

	VERTEX_3D(const Point3D& pt, const Color3& c = { 1.f, 1.f, 1.f },
			  const Vec2D& tx = {0.f, 0.f})
	{
		this->pt	= pt;
		this->color = c;
	}

	VERTEX_3D& operator=(const Point3D& pt)
	{
		this->pt	  = pt;
		this->color   = { 1.f, 1.f, 1.f };
		this->texture = { 0.f, 0.f };
		return *this;
	}
};


____END_NAMESPACE____

#endif // XGEOTYPE_H
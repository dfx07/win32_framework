////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgeotype.h
* @create   Nov 15, 2023
* @brief    Geometry3D type define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGEOTYPE_H
#define XGEOTYPE_H

#include "x2dtype.h"
#include "x3dtype.h"

/////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************/
// Define use GLM library
typedef struct _VERTEX_3D
{
	Point3D		pt;
	Color3		col;
	Vec2D		tex;

public:
	_VERTEX_3D() : pt(0.f, 0.f, 0.f),
		col(0.f, 0.f, 0.f),
		tex(0.f, 0.f)
	{

	}

	_VERTEX_3D(const Point3D& pt, const Color3& c = { 1.f, 1.f, 1.f },
		const Vec2D& tx = { 0.f, 0.f })
	{
		this->pt = pt;
		this->col = c;
		this->tex = tx;
	}

	_VERTEX_3D& operator=(const Point3D& pt)
	{
		this->pt = pt;
		this->col = { 1.f, 1.f, 1.f };
		this->tex = { 0.f, 0.f };
		return *this;
	}

} VERTEX_3D;

#endif //!X3DTYPE_H


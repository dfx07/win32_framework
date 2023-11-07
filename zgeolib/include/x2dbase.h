////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x2dbase.h
* @create   Oct 18, 2023
* @brief    Geometry2D base function
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef X2DBASE_H
#define X2DBASE_H

#include "x2dtype.h"

namespace geo{ namespace v2
{
	/*******************************************************************************
	*! @brief	: Calculate vector magnitude
	*! @param	: [in] v : Vector
	*! @return	: GFloat : magnitude	- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport GFloat	 mag(const Vec2D& v);

	/*******************************************************************************
	*! @brief  : Calculation of unit vector
	*! @param  : [in] v : vector
	*! @return : uinit vector
	*! @author : thuong.nv				- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport Vec2D	 normalize(const Vec2D& v);

	/*******************************************************************************
	@brief		Calculate the crossProduct of the two vector
	@param		[in] v1		: First vector
	@param		[in] v2		: Second vector
	@return		GFloat : Value cross product
	********************************************************************************/
	Dllexport GFloat	 cross(const Vec2D& v1, const Vec2D& v2);

	/*******************************************************************************
	*! @brief  : Calculate dot product between 2 vectors
	*! @param  : [in] fangle : angle GFloat
	*! @return : GFloat : dot product
	*! @author : thuong.nv				- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport GFloat	 dot(const Vec2D& v1, const Vec2D& v2);

	/*******************************************************************************
	*! @brief  : Calculate angle between 2 vectors (degree)
	*! @param  : [in] v1 : unit vector v1
	*! @param  : [in] v2 : unit vector v2
	*! @return : Degree angle between two vector [-180~180]
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport GFloat	 angle(const Vec2D& v1, const Vec2D& v2);

	/*******************************************************************************
	*! @brief  : move point use unit vector and distance
	*! @param  : [in] pt       : move point
	*! @param  : [in] vn       : unit vector ( move direction)
	*! @param  : [in] fDistance: distance
	*! @return : Point2D point after move
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport Point2D move(const Point2D& pt, const Vec2D& vn, const GFloat fDistance);

	/*******************************************************************************
	*! @brief  : Rotate vector (degree)
	*! @param  : [in] v      : unit vector
	*! @param  : [in] fDegree: degree angle
	*! @return : Vector2D after rotate
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport Vec2D	 rotate(const Vec2D& v, const GFloat fDegree);

	/*******************************************************************************
	*! @brief  : Rotate point (degree)
	*! @param  : [in] ptPivot : pivot point
	*! @param  : [in] ptRotate: rotation point
	*! @param  : [in] fDegree : degree angle
	*! @return : Point2D after rotate
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	Dllexport Point2D rotate(const Point2D& ptPivot, const Point2D& ptRotate, const GFloat fDegree);

	/********************************************************************************
	*! @brief  : Midpoint between 2 points
	*! @param  : [in] pt1 : point 1
	*! @param  : [in] pt2 : point 2
	*! @return : Point2D mid point
	*! @author : thuong.nv			- [Date] : 05/19/2023
	********************************************************************************/
	Dllexport Point2D mid_point(const Point2D& pt1, const Point2D& pt2);
}}

#endif //!X2DBASE_H
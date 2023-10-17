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
	@brief		Calculate vector magnitude
	@param		[in] v : Vector
	@return		float : magnitude	- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR float	 mag(const Vec2D& v);

	/*******************************************************************************
	*! @brief  : Calculation of unit vector
	*! @param  : [in] v : vector
	*! @return : uinit vector
	*! @author : thuong.nv				- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR Vec2D	 normalize(const Vec2D& v);

	/*******************************************************************************
	@brief		Calculate the crossProduct of the two vector
	@param		[in] v1		: First vector
	@param		[in] v2		: Second vector
	@return		float : Value cross product
	********************************************************************************/
	API_EXPR float	 cross(const Vec2D& v1, const Vec2D& v2);

	/*******************************************************************************
	*! @brief  : Calculate dot product between 2 vectors
	*! @param  : [in] fangle : angle float
	*! @return : float : dot product
	*! @author : thuong.nv				- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR float	 dot(const Vec2D& v1, const Vec2D& v2);

	/*******************************************************************************
	*! @brief  : Calculate angle between 2 vectors (degree)
	*! @param  : [in] v1 : unit vector v1
	*! @param  : [in] v2 : unit vector v2
	*! @return : Degree angle between two vector [-180~180]
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR float	 angle(const Vec2D& v1, const Vec2D& v2);

	/*******************************************************************************
	*! @brief  : move point use unit vector and distance
	*! @param  : [in] pt       : move point
	*! @param  : [in] vn       : unit vector ( move direction)
	*! @param  : [in] fDistance: distance
	*! @return : Point2D point after move
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR Point2D move(const Point2D& pt, const Vec2D& vn, const float fDistance);

	/*******************************************************************************
	*! @brief  : Rotate vector (degree)
	*! @param  : [in] v      : unit vector
	*! @param  : [in] fDegree: degree angle
	*! @return : Vector2D after rotate
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR Vec2D	 rotate(const Vec2D& v, const float fDegree);

	/*******************************************************************************
	*! @brief  : Rotate point (degree)
	*! @param  : [in] ptPivot : pivot point
	*! @param  : [in] ptRotate: rotation point
	*! @param  : [in] fDegree : degree angle
	*! @return : Point2D after rotate
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR Point2D rotate(const Point2D& ptPivot, const Point2D& ptRotate, const float fDegree);

	/*******************************************************************************
	*! @brief  : Convert from Radian to Degree
	*! @param  : [in] fRad : angle float (radian)
	*! @return : angle after convert
	********************************************************************************/
	API_EXPR float	 r2d(const float fRad);

	/*******************************************************************************
	*! @brief  : Convert from Degree to Radian
	*! @param  : [in] fDegree : angle float (degree)
	*! @return : angle after convert
	********************************************************************************/
	API_EXPR float	 d2r(const float fDegree);
}}

#endif //!X2DBASE_H
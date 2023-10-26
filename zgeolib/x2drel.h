////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x2drel.h
* @create   Nov 15, 2023
* @brief    Geometry2D type define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/

#ifndef X2D_REL_H
#define X2D_REL_H

#include "x2dbase.h"

namespace geo{ namespace v2
{
	/*******************************************************************************
	@brief		Check two vector is the same direction (parallel ||
	@param		[in] pt1		: vector 1 (not normal)
	@param		[in] pt2		: vector 2 (not normal)
	@param		[in] ftolerance : > math_epsilon
	@return		true : same | false : not same
	********************************************************************************/
	API_EXPR GBool	is_same_direction(const Vec2D& v1, const Point2D& v2, const GFloat& ftolerance = MATH_EPSILON);

	/*******************************************************************************
	@brief		Check if 2 vectors have the same sign (on the same 1/4)
	@param		[in] v1	: First vector
	@param		[in] v2	: Second vector
	@return		TRUE : same | FALSE : not same
	********************************************************************************/
	API_EXPR GBool	is_same_sign(const Vec2D& v1, const Point2D& v2);

	/********************************************************************************
	@brief		Check polygon is counter-clockwise
	@param		[in] polyg : polygon
	@return		TRUE : is CCW | FALSE : CW
	********************************************************************************/
	API_EXPR GBool	is_ccw(const VecPoint2D& poly);

	/********************************************************************************
	*! @brief  : Determine the orientation of these three points
	*! @param  : [in ] ptC :check point
	*! @param  : [out] pt1  : point on vector p1p2
	*! @param  : [out] pt2  : point on vector p1p2
	*! @return : EnumOrien | COLLINEAR : ptc and p1p2 collinear
	*!					   | RIGHT     : ptc on the right p1p2
	*!					   | LEFT      : ptc on the left  p1p2
	*! @author : thuong.nv			- [Date] : 08/07/2023
	********************************************************************************/
	API_EXPR EnumOrien get_orientation_point_vector(const Point2D& ptC, const Point2D& pt1, const Point2D& pt2);

	/********************************************************************************
	*! @brief  : Check point is in ray (ray : point start and unit vector)
	*! @param  : [in] pt  : start point of ray
	*! @param  : [in] vn  : unit vector of ray
	*! @param  : [in] ptc : point check
	*! @return : 0 : not in line segment
	*!           1 : in line segment
	*!           2 : same p1
	*!           3 : same p2
	*! @author : thuong.nv			- [Date] : 17/10/2023
	********************************************************************************/
	API_EXPR GInt get_rel_point_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);

	/********************************************************************************
	@brief		Check polygon is convex
	@param		[in] poly : polygon
	@return		TRUE : is convex | FALSE : no convex
	********************************************************************************/
	API_EXPR GBool	is_convex_polygon(const VecPoint2D& poly);

	/********************************************************************************
	@brief		Check point on a line segment given by two points
	@param		[in] pt1	: First point in line segment
	@param		[in] pt2	: Second point in line segment
	@param		[in] pt		: Point check
	@return		TRUE : inside | FALSE : outside
	*********************************************************************************/
	API_EXPR GBool is_point_in_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);

	/********************************************************************************
	@brief		Check snap point point to line segement
	@param		[in] pt1	: First point in line segment
	@param		[in] pt2	: Second point in line segment
	@param		[in] pt		: Point check
	@return		TRUE : inside | FALSE : outside
	********************************************************************************/
	API_EXPR GBool is_snap_point_to_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const GFloat& fSnap);

	/*******************************************************************************
	@brief		Check point on a line given by two points
	@param		[in] pt1	: First point in line
	@param		[in] pt2	: Second point in line
	@param		[in] pt		: Point check
	@return		TRUE : inside | FALSE : outside
	********************************************************************************/
	API_EXPR GBool is_point_in_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);

	/********************************************************************************
	@brief		Check snap point point to line
	@param		[in] pt1	: First point in line segment
	@param		[in] pt2	: Second point in line segment
	@param		[in] pt		: Point check
	@return		TRUE : inside | FALSE : outside
	*********************************************************************************/
	API_EXPR GBool is_snap_point_to_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const GFloat& fSnap);

	/********************************************************************************
	*! @brief  : Check point is in ray (ray : point start and unit vector)
	*! @param  : [in] pt  : start point of ray
	*! @param  : [in] vn  : unit vector of ray
	*! @param  : [in] ptc : point check
	*! @return : TRUE : in ray | FALSE : out ray
	*! @author : thuong.nv			- [Date] : 06/03/2023
	********************************************************************************/
	API_EXPR GBool is_point_in_ray(const Point2D& pt, const Vec2D& vn, const Point2D& ptc);
}}

#endif // !X2D_REL_H

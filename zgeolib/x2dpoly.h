////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x2dpoly.h
* @create   Nov 15, 2023
* @brief    Geometry2D poly
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/

#ifndef X2D_POLY_H
#define X2D_POLY_H

#include "x2dbase.h"

namespace geo {	namespace v2
{
	/********************************************************************************
	*! @brief  :Reverse polygon cw -> ccw | ccw -> cw
	*! @param  : [in] poly : polygon
	*! @author : thuong.nv			- [Date] : 25/10/2023
	********************************************************************************/
	API_EXPR void reverse_polygon(VecPoint2D& poly);

	/********************************************************************************
	*! @brief  : Inflat polygon certain distance
	*! @param  : [in ] poly    : poly inflat (CCW)
	*! @param  : [out] fOffset : distance away from center
	*! @return : VecPoint2D polygon after inflating
	*! @author : thuong.nv			- [Date] : 08/28/2023
	********************************************************************************/
	API_EXPR VecPoint2D infact_polygon(const VecPoint2D& poly, const GFloat& fOffset);

	/********************************************************************************
	*! @brief  : Inflat polygon certain distance (no copy)
	*! @param  : [in ] poly    : poly inflat (CCW)
	*! @param  : [out] fOffset : distance away from center
	*! @return : VecPoint2D polygon after inflating
	*! @author : thuong.nv			- [Date] : 08/28/2023
	********************************************************************************/
	API_EXPR void infact_polygonx(VecPoint2D& poly, const GFloat& fOffset);

	/*******************************************************************************
	*! @brief  : Area of a polygon with given n ordered vertices
	*! @param  : [in] points : list point polygon
	*! @return : float : area
	*! @author : thuong.nv			- [Date] : 08/03/2023
	*! @note   : shoelace formula | no hold
	********************************************************************************/
	API_EXPR GFloat get_area_polygon(const VecPoint2D& points);

	/*******************************************************************************
	*! @brief  : bounding box of a vector of points
	*! @param  : [in ] points : list point
	*! @param  : [out] ptMin  : min point bounding
	*! @param  : [out] ptMax  : max point bounding
	*! @author : thuong.nv			- [Date] : 05/19/2023
	********************************************************************************/
	API_EXPR void  get_bounding_box(const VecPoint2D& points, Point2D& ptMin, Point2D& ptMax);

}}

#endif // !X2D_POLY_H
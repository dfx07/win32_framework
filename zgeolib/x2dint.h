////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x2dint.h
* @create   Nov 15, 2023
* @brief    Geometry2D type define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/

#ifndef X2D_INT_H
#define X2D_INT_H

#include "x2dbase.h"

namespace geo { namespace v2
{
	/***********************************************************************************
	*! @brief  : Find the perpendicular projection of a point onto the line. (line = 2 point)
	*! @param  : [in] ptLine1  : point in line
	*! @param  : [in] ptLine2  : point in line
	*! @param  : [in] pt       : get point
	*! @return : Point2D perpendicular projection of a point
	*! @author : thuong.nv			- [Date] : 04/07/2023
	***********************************************************************************/
	API_EXPR Point2D get_projection_point_to_line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt);

	/***********************************************************************************
	*! @brief  : Find the perpendicular projection of a point onto the segment. (segment = 2 point)
	*! @param  : [in] ptSeg1  : point start in segment
	*! @param  : [in] ptSeg2  : point end in segment
	*! @param  : [in] pt      : get point
	*! @param  : [out] pptPerp		: perpendicular point
	*! @param  : [in] bCheckNearest : near segment point (seg1 or seg2)
	*! @return : 1 : Exist perpendicular point
	*!         : 2 : Not exist perpendicular point but nearlly ptSeg1 (bCheckNearest = TRUE)
	*!         : 3 : Not exist perpendicular point but nearlly ptSeg2 (bCheckNearest = TRUE)
	*!         : 0 : Not exist perpendicular point
	*! @author : thuong.nv			- [Date] : 05/11/2023
	***********************************************************************************/
	API_EXPR GInt get_projection_point_to_lsegment(const Point2D& ptSeg1, const Point2D& ptSeg2, const Point2D& pt, Point2D* ptPerp = NULL, GBool bCheckNearest = GFalse);
}}

#endif // !X2D_INT_H
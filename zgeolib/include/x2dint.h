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
	/********************************************************************************
	*! @brief  : Find the perpendicular projection of a point onto the line. (line = 2 point)
	*! @param  : [in] ptLine1  : point in line
	*! @param  : [in] ptLine2  : point in line
	*! @param  : [in] pt       : get point
	*! @return : Point2D perpendicular projection of a point
	*! @author : thuong.nv			- [Date] : 04/07/2023
	********************************************************************************/
	Dllexport Point2D get_projection_point_to_line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt);

	/********************************************************************************
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
	********************************************************************************/
	Dllexport GInt get_projection_point_to_lsegment(const Point2D& ptSeg1, const Point2D& ptSeg2, const Point2D& pt, Point2D* ptPerp = NULL, GBool bCheckNearest = GFalse);

	/********************************************************************************
	@brief		Get intersection point between line and line (line = 2 point)
	@param		[in]  pt1	 : Point in Line 1
	@param		[in]  pt2	 : Point in Line 1
	@param		[in]  pt3	 : Point in Line 2
	@param		[in]  pt4	 : Point in Line 2
	@param		[out] pInter : Intersection
	@return		TRUE : Intersect | FALSE : not intersect
	*********************************************************************************/
	Dllexport GBool intersect_2line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3, const Point2D& pt4, Point2D* pInter = NULL);

	/********************************************************************************
	@brief		Checks if two line segments intersect and returns the intersection point
	@param		[in]  pt1	 : Point in Line segment 1
	@param		[in]  pt2	 : Point in Line segment 1
	@param		[in]  pt3	 : Point in Line segment 2
	@param		[in]  pt4	 : Point in Line segment 2
	@param		[out] pInter : Intersection
	@return		TRUE : Intersect | FALSE : not intersect
	*********************************************************************************/
	Dllexport GBool intersect_2lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3, const Point2D& pt4, Point2D* pInter = NULL);

	/********************************************************************************
	@brief		Get intersection point between line and line segment (line = two point)
	@param		[in]  ptLine1 : Point on the line
	@param		[in]  ptLine2 : Point on the line
	@param		[in]  ptSeg1  : Point start on line segment
	@param		[in]  ptSeg2  : Point end in line segment
	@param		[out] pInter  : Intersection
	@return		TRUE : Intersect | FALSE : not intersect
	*********************************************************************************/
	Dllexport GBool intersect_line_lsegment(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& ptSeg1, const Point2D& ptSeg2, Point2D* pInter = NULL);  // line = two point

	/********************************************************************************
	@brief		Get intersection points between line and polygon (line = two point)
	@param		[in]  ptLine1    : Point on the line
	@param		[in]  ptLine2    : Point on the line
	@param		[in]  poly       : polygon
	@param		[out] vecInter   : point list of intersections
	@return		INT : intersection number.
	*********************************************************************************/
	Dllexport GInt  intersect_line_polygon(const Point2D& ptLine1, const Point2D& ptLine2, const VecPoint2D& poly, VecPoint2D* vecInter = NULL, GBool bOnlyCheck = GFalse);

	/********************************************************************************
	@brief		Get intersection points between line and rect (line = two point)
	@param		[in]  ptLine1 : Point on the line
	@param		[in]  ptLine2 : Point on the line
	@param		[in]  rect    : rectangle
	@param		[out] vecInter: point list of intersections
	@return		INT : intersection number.
	*********************************************************************************/
	Dllexport GInt  intersect_line_rectangle(const Point2D& ptLine1, const Point2D& ptLine2, const Rect2D& rect, VecPoint2D* vecInter = NULL, GBool bOnlyCheck = GFalse);

	/********************************************************************************
	@brief		get intersection point between line segment and polygon
	@param		[in]  pt1	    : Point on Line segment
	@param		[in]  pt2	    : Point on Line segment
	@param		[in]  poly	    : polygon
	@param		[out] vecInter  : point list of intersections
	@param		[in]  bOnlyCheck: only check if intersect or not
	@return		INT : intersection number.
	********************************************************************************/
	Dllexport GInt intersect_lsegment_polygon(const Point2D& pt1, const Point2D& pt2, const VecPoint2D& poly, VecPoint2D* vecInter = NULL, GBool bOnlyCheck = GFalse);
}}

#endif // !X2D_INT_H
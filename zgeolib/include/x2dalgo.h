////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     x2dalgo.h
* @create   Nov 15, 2023
* @brief    Geometry2D algorithm
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/

#ifndef X2D_ALOGO_H
#define X2D_ALOGO_H

#include "x2dbase.h"

namespace geo {	namespace v2
{
	/********************************************************************************
	*! @brief  :Reverse polygon cw -> ccw | ccw -> cw
	*! @param  : [in] poly : polygon
	*! @author : thuong.nv			- [Date] : 25/10/2023
	********************************************************************************/
	Dllexport VecPoint2D convex_hull_giftwap(const VecPoint2D& vecPoints);

	/********************************************************************************
	*! @brief  : Clipping Polygons use Sutherland–Hodgman algorithm [rect]
	*! @param  : [in]  poly           : polygon
	*! @param  : [in]  clipConvexPoly : convex clip polygon
	*! @return : VecPoint2D clip polygon
	*! @author : thuong.nv         - [Date] : 08/07/2023
	*! @note   : Polygon input is counterclockwise (CCW).
	*!           It works by extending each line of the convex clip polygon in turn and
	*!           selecting only vertices from the subject polygon that are on the visible side.
	*! @refer  : https://en.wikipedia.org/wiki/Sutherland–Hodgman_algorithm
	********************************************************************************/
	Dllexport VecPoint2D clip_polygon_hodgman(const VecPoint2D& poly, const VecPoint2D& clipConvexPoly);

	/********************************************************************************
	*! @brief  : Clipping Polygons use Weiler Atherton  algorithm
	*! @param  : [in]  _poly     : polygon
	*! @param  : [in]  _clipPoly : clip (polygon)
	*! @return : VecPolyList list poly clip
	*! @author : thuong.nv   - [Date] : 08/07/2023
	*! @note   : Polygon input is counterclockwise (CCW).
	*! @refer  : https://www.geeksforgeeks.org/weiler-atherton-polygon-clipping-algorithm/
	*********************************************************************************/
	Dllexport VecPolyList clip_polygon_weiler(const VecPoint2D& poly, const VecPoint2D& clipPoly);

	/********************************************************************************
	*! @brief  : Union two convex polygon (without hole)
	*! @param  : [in]  poly1 : convex polygon
	*! @param  : [in]  poly2 : convex polygon
	*! @return : VecPolyList list poly clip
	*! @author : thuong.nv   - [Date] : 13/11/2023
	*! @note   : Polygon input is counterclockwise (CCW). without holes
	*! @refer  : https://stackoverflow.com/questions/6844462/polygon-union-without-holes
	*********************************************************************************/
	Dllexport VecPoint2D union_polygon(const VecPoint2D& poly1, const VecPoint2D& poly2);

	/********************************************************************************
	*! @brief  : Cut line and polygon
	*! @param  : [in]  pt1  : Point in line
	*! @param  : [in]  pt2  : Point in line
	*! @param  : [in]  poly : polygon
	*! @return : VecPolyList list poly clip
	*! @author : thuong.nv   - [Date] : 29/10/2023
	*! @note   : Polygon input is counterclockwise (CCW).
	*********************************************************************************/
	Dllexport VecPolyList cut_line2poly(const Point2D& pt1, const Point2D& pt2, const VecPoint2D& poly);

	/********************************************************************************
	*! @brief  : The partition of a polygonal area (simple polygon) P into a set of triangles
	*! @param  : [in]  poly : polygon
	*! @return : VecPolyList (Every 3 consecutive points will be a triangle)
	*! @author : thuong.nv   - [Date] : 11/07/2023
	*! @note   : Polygon input is counterclockwise (CCW winding).
	*!           For convex or concave polygon
	*********************************************************************************/
	Dllexport VecPoint2D poly2trig_ear_clipping(const VecPoint2D& poly);

}}

#endif // !X2D_ALOGO_H
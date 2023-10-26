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
	API_EXPR VecPoint2D convex_hull_giftwap(const VecPoint2D& vecPoints);

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
	API_EXPR VecPoint2D clip_polygon_hodgman(const VecPoint2D& poly, const VecPoint2D& clipConvexPoly);

	/********************************************************************************
	*! @brief  : Clipping Polygons use Weiler Atherton  algorithm
	*! @param  : [in]  _poly     : polygon
	*! @param  : [in]  _clipPoly : clip (polygon)
	*! @return : VecPolyList list poly clip
	*! @author : thuong.nv   - [Date] : 08/07/2023
	*! @note   : Polygon input is counterclockwise (CCW).
	*! @refer  : https://www.geeksforgeeks.org/weiler-atherton-polygon-clipping-algorithm/
	*********************************************************************************/
	API_EXPR VecPolyList clip_polygon_weiler(const VecPoint2D& poly, const VecPoint2D& clipPoly);
}}

#endif // !X2D_ALOGO_H
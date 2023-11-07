#include "x2dpoly.h"
#include "x2dint.h"
#include <cassert>
#include "x2drel.h"
#include "xgeosp.h"

namespace geo {	namespace v2 {

/***********************************************************************************
@brief		Reverse polygon cw -> ccw | ccw -> cw
@param		[in] poly : polygon
***********************************************************************************/
Dllexport void reverse_polygon(VecPoint2D& poly)
{
	int nPolyCnt = static_cast<int>(poly.size());

	int nHalf = (nPolyCnt - 1) / 2;

	for (int i = 1; i <= nHalf; i++)
	{
		std::swap(poly[i], poly[nPolyCnt - i]);
	}
}

/***********************************************************************************
*! @brief  : Inflat polygon certain distance
*! @param  : [IN ] poly    : poly inflat (CCW)
*! @param  : [out] fOffset : distance away from center
*! @return : VecPoint2D polygon after inflating
*! @author : thuong.nv			- [Date] : 08/28/2023
***********************************************************************************/
Dllexport VecPoint2D infact_polygon(const VecPoint2D& poly, const GFloat& fOffset)
{
	int nPolyCnt = static_cast<int>(poly.size());
	int nPre = 0;

	Line2D lnTemp;
	VecLine2D vecLineInflat; vecLineInflat.reserve(nPolyCnt);

	for (int j = 0, i = nPolyCnt - 1; j < nPolyCnt; i = j++)
	{
		Vec2D vNorInflat = normalize(poly[j] - poly[i]);

		vNorInflat = rotate(vNorInflat, 90.f); 

		lnTemp.ptStart = move(poly[i], vNorInflat, fOffset);
		lnTemp.ptEnd   = move(poly[j], vNorInflat, fOffset);

		vecLineInflat.push_back(lnTemp);
	}

	VecPoint2D vecInflatPoly; vecInflatPoly.reserve(nPolyCnt);
	Point2D ptInter;
	int nLineCnt = static_cast<int>(vecLineInflat.size());

	for (int i = 0; i < nLineCnt; i++)
	{
		nPre = (nLineCnt + i - 1) % nLineCnt;

		if (intersect_2line(vecLineInflat[nPre].ptStart, vecLineInflat[nPre].ptEnd,
						    vecLineInflat[i].ptStart, vecLineInflat[i].ptEnd, &ptInter) == GTrue)
		{
			vecInflatPoly.push_back(ptInter);
		}
		else
		{
			vecInflatPoly.push_back(vecLineInflat[nPre].ptEnd);
		}
	}

	return vecInflatPoly;
}

/***********************************************************************************
*! @brief  : Inflat polygon certain distance (no copy)
*! @param  : [in ] poly    : poly inflat (CCW)
*! @param  : [out] fOffset : distance away from center
*! @return : VecPoint2D polygon after inflating
*! @author : thuong.nv			- [Date] : 08/28/2023
***********************************************************************************/
Dllexport void infact_polygonx(VecPoint2D& poly, const GFloat& fOffset)
{
	int nPolyCnt = static_cast<int>(poly.size());
	int nPre = 0;

	Line2D lnTemp;
	VecLine2D vecLineInflat; vecLineInflat.reserve(nPolyCnt);

	for (int j = 0, i = nPolyCnt - 1; j < nPolyCnt; i = j++)
	{
		Vec2D vNorInflat = normalize(poly[j] - poly[i]);

		vNorInflat = rotate(vNorInflat, 90.f);

		lnTemp.ptStart = move(poly[i], vNorInflat, fOffset);
		lnTemp.ptEnd   = move(poly[j], vNorInflat, fOffset);

		vecLineInflat.push_back(lnTemp);
	}

	VecPoint2D vecInflatPoly; vecInflatPoly.reserve(nPolyCnt);
	Point2D ptInter;
	int nLineCnt = static_cast<int>(vecLineInflat.size());

	for (int i = 0; i < nLineCnt; i++)
	{
		nPre = (nLineCnt + i - 1) % nLineCnt;

		if (intersect_2line(vecLineInflat[nPre].ptStart, vecLineInflat[nPre].ptEnd,
						    vecLineInflat[i].ptStart, vecLineInflat[i].ptEnd, &ptInter) == GTrue)
		{
			poly[i] = ptInter;
		}
		else
		{
			poly[i] = vecLineInflat[nPre].ptEnd;
		}
	}
}

/***********************************************************************************
*! @brief  : Area of a polygon with given n ordered vertices
*! @param  : [in] points : list point polygon
*! @return : float : area
*! @author : thuong.nv			- [Date] : 08/03/2023
*! @note   : shoelace formula | no hold
***********************************************************************************/
Dllexport GFloat get_area_polygon(const VecPoint2D& points)
{
	GFloat fArea = 0.f;

	int nPointCnts = static_cast<int>(points.size());

	if (nPointCnts <= 2)
	{
		assert(0);
		return 0.f;
	}

	int j = nPointCnts - 1;

	for (int i = 0; i < nPointCnts; i++)
	{
		fArea += (points[j].x + points[i].x) * (points[j].y - points[i].y);
		j = i;
	}

	// Return absolute value
	return fabsf(fArea / 2.f);
}

/***********************************************************************************
*! @brief  : bounding box of a vector of points
*! @param  : [in ] points : list point
*! @param  : [out] ptMin  : min point bounding
*! @param  : [out] ptMax  : max point bounding
*! @author : thuong.nv			- [Date] : 05/19/2023
***********************************************************************************/
Dllexport void get_bounding_box(const VecPoint2D& points, Point2D& ptMin, Point2D& ptMax)
{
	GFloat fMinX = std::numeric_limits<GFloat>::max();
	GFloat fMinY = std::numeric_limits<GFloat>::max();

	GFloat fMaxX = std::numeric_limits<GFloat>::min();
	GFloat fMaxY = std::numeric_limits<GFloat>::min();

	int nPointCnt = static_cast<int>(points.size());
	for (int i = 0; i < nPointCnt; i++)
	{
		fMinX = std::min<GFloat>(fMinX, points[i].x);
		fMinY = std::min<GFloat>(fMinY, points[i].y);
		fMaxX = std::max<GFloat>(fMaxX, points[i].x);
		fMaxY = std::max<GFloat>(fMaxY, points[i].y);
	}

	ptMin = Point2D(fMinX, fMinY);
	ptMax = Point2D(fMaxX, fMaxY);
}

/***********************************************************************************
@brief		check point inside polygon
@param		[in] pt		: Point check
@param		[in] poly	: Polygon
@return		TRUE : inside | FALSE : outside
@note		ref : https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
***********************************************************************************/
Dllexport GBool is_point_in_polygon(const Point2D& pt, const VecPoint2D& poly)
{
	if (poly.size() < 3)
	{
		assert(0);
		return GFalse;
	}

	GFloat fMinX = poly[0].x;
	GFloat fMaxX = poly[0].x;
	GFloat fMinY = poly[0].y;
	GFloat fMaxY = poly[0].y;

	int nPolyCount = static_cast<int>(poly.size());

	for (int i = 1; i < nPolyCount; i++)
	{
		fMinX = std::min<GFloat>(poly[i].x, fMinX);
		fMaxX = std::max<GFloat>(poly[i].x, fMaxX);
		fMinY = std::min<GFloat>(poly[i].y, fMinY);
		fMaxY = std::max<GFloat>(poly[i].y, fMaxY);
	}

	if (pt.x < fMinX || pt.x > fMaxX || pt.y < fMinY || pt.y > fMaxY)
		return GFalse;

	GBool bInside = GFalse;
	for (int i = 0, j = nPolyCount - 1; i < nPolyCount; j = i++)
	{
		if ((poly[i].y > pt.y) != (poly[j].y > pt.y) &&
			pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)
		{
			bInside = !bInside;
		}
		// case inside edge and collinear
		else if (is_equal(poly[i].y, pt.y, MATH_EPSILON) &&
				 is_equal(poly[j].y, pt.y, MATH_EPSILON))
		{
			if ((poly[i].x > pt.x) != (poly[j].x > pt.x))
			{
				return GTrue;
			}
		}
	}

	return bInside;
}

/***********************************************************************************
*! @brief	: Get the relationship between 2 polygons
*! @param	: [in]  poly1	 : first polygon
*! @param	: [in]  poly2	 : second polygon
*! @return	:| INVALID  (-1) : invalid param
*!			 | OUTSIDE  ( 0) : outside
*!			 | INTER    ( 1) : intersect
*!			 | INSIDE_1 ( 2) : poly1 inside poly2
*!			 | INSIDE_2 ( 3) : poly2 inside poly1
*! @note	: poly1 and poly 2 have number points greater than 3
***********************************************************************************/
Dllexport GInt rel_2polygon(const VecPoint2D& poly1, const VecPoint2D& poly2)
{
	if (poly1.size() < 3 || poly2.size() < 3)
	{
		assert(0);
		return -1;
	}

	int nPolyCount = static_cast<int>(poly1.size());

	// Case 1 : intersect - check 2 intersecting polygons
	for (int j = 0, i = nPolyCount - 1; j < nPolyCount; i = j++)
	{
		if (intersect_lsegment_polygon(poly1[i], poly1[j], poly2, NULL, GTrue) >= 1)
			return 1;
	}

	// Case 2 : inside - check poly1 inside poly2 
	if (GTrue == is_point_in_polygon(poly1[0], poly2))
		return 2;

	// Case 3 : inside - check poly2 inside poly1 
	if (GTrue == is_point_in_polygon(poly2[0], poly1))
		return 3;

	// Case 4 (default): outside
	return 0;
}

}}

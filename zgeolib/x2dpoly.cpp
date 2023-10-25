#include "x2dpoly.h"
#include "x2dint.h"
#include <cassert>

namespace geo {	namespace v2 {

/***********************************************************************************
@brief		Reverse polygon cw -> ccw | ccw -> cw
@param		[in] poly : polygon
***********************************************************************************/
API_EXPR void reverse_polygon(VecPoint2D& poly)
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
API_EXPR VecPoint2D infact_polygon(const VecPoint2D& poly, const GFloat& fOffset)
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
API_EXPR void infact_polygonx(VecPoint2D& poly, const GFloat& fOffset)
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
API_EXPR GFloat get_area_polygon(const VecPoint2D& points)
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
API_EXPR void get_bounding_box(const VecPoint2D& points, Point2D& ptMin, Point2D& ptMax)
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

}}

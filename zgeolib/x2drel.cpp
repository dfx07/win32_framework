#include "x2drel.h"
#include "x2dbase.h"
#include "xgeosp.h"
#include "x2dint.h"
#include <cassert>

namespace geo {namespace v2 {

/***********************************************************************************
@brief		Check two vector is the same direction (parallel ||
@param		[in] pt1		: vector 1 (not normal)
@param		[in] pt2		: vector 2 (not normal)
@param		[in] ftolerance : > math_epsilon
@return		true : same | false : not same
***********************************************************************************/
API_EXPR bool is_same_direction(const Vec2D& v1, const Point2D& v2, const GFloat& ftolerance /* = MATH_EPSILON*/)
{
	Vec2D vn1 = normalize(v1);
	Vec2D vn2 = normalize(v2);

	float fCrs = cross(vn1, vn2);

	// Point in straight line
	if (geo::is_equal(fCrs, 0.f, ftolerance))
	{
		return is_same_sign(vn1, vn2);
	}
	return GFalse;
}

/***********************************************************************************
@brief		Check point on a line segment given by two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
API_EXPR GBool is_same_sign(const Vec2D& v1, const Point2D& v2)
{
	return ((v1.x * v2.x) >= 0 && (v1.y * v2.y) >= 0) ? GTrue : GFalse;
}

/***********************************************************************************
@brief		Check polygon is counter-clockwise
@param		[in] polyg : polygon
@return		TRUE : is CCW | FALSE : CW
***********************************************************************************/
API_EXPR GBool is_ccw(const VecPoint2D& poly)
{
	int nPolyCnt = static_cast<int>(poly.size());

	int nNext; float fSum = 0.f;

	for (int i = 0; i < nPolyCnt; i++)
	{
		nNext = (i + 1) % nPolyCnt;
		fSum += (poly[nNext].x - poly[i].x) * (poly[nNext].y + poly[i].y);
	}

	return (fSum <= 0.f) ? GTrue : GFalse;
}

/***********************************************************************************
*! @brief  : Determine the orientation of these three points
*! @param  : [in ] ptC :check point
*! @param  : [out] pt1  : point on vector p1p2
*! @param  : [out] pt2  : point on vector p1p2
*! @return : EnumOrien | COLLINEAR : ptc and p1p2 collinear
*!					   | RIGHT     : ptc on the right p1p2
*!					   | LEFT      : ptc on the left  p1p2
*! @author : thuong.nv			- [Date] : 08/07/2023
***********************************************************************************/
API_EXPR EnumOrien get_orientation_point_vector(const Point2D& ptC, const Point2D& pt1, const Point2D& pt2)
{
	const float fOrin = (pt2.x - pt1.x) * (ptC.y - pt1.y) - (ptC.x - pt1.x) * (pt2.y - pt1.y);

	if (geo::is_equal(fOrin, 0.f, MATH_EPSILON)) return EnumOrien::COLLINEAR; /* ptc and p1p2 collinear */
	else if (fOrin < 0.f)	return EnumOrien::RIGHT; /* ptc on the right p1p2 */
	else	return EnumOrien::LEFT;	/* ptc on the left p1p2 */
}

/***********************************************************************************
@brief		Check polygon is convex
@param		[in] poly : polygon
@return		TRUE : is convex | FALSE : no convex
***********************************************************************************/
API_EXPR GBool is_convex_polygon(const VecPoint2D& poly)
{
	int nPolyCnt = static_cast<int>(poly.size());

	if (nPolyCnt < 3)
		return GFalse;

	EnumOrien ePreOrien = EnumOrien::COLLINEAR;

	for (int i = 0; i < nPolyCnt - 2; i++)
	{
		EnumOrien eCurOrien = get_orientation_point_vector(poly[i], poly[i + 1], poly[i + 2]);

		if (eCurOrien  != EnumOrien::COLLINEAR &&
			(ePreOrien != EnumOrien::COLLINEAR && eCurOrien != ePreOrien))
		{
			return GFalse;
		}

		ePreOrien = eCurOrien;
	}

	return GTrue;
}

/***********************************************************************************
@brief		Check point on a line segment given by two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
API_EXPR GBool is_point_in_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	Vec2D vp1p = pt - pt1; // Vector vp1p ;
	Vec2D vp2p = pt - pt2; // Vector vp2p ;

	GFloat fCrs = cross(vp1p, vp2p);

	// Point in straight line
	if (geo::is_equal(fCrs, 0.f, MATH_EPSILON))
	{
		// TODO : improve
		// Point in side line Segment 
		GFloat fp1pDistance  = mag(vp1p);
		GFloat fp2pDistance  = mag(vp2p);
		GFloat fp1p2Distance = mag(pt1 - pt2);

		if (fp1pDistance <= fp1p2Distance &&
			fp2pDistance <= fp1p2Distance)
		{
			return GTrue;
		}
	}
	return GFalse;
}

/***********************************************************************************
@brief		Check snap point point to line segement
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
API_EXPR GBool is_snap_point_to_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const GFloat& fSnap)
{
	Point2D ptProj;

	if (get_projection_point_to_lsegment(pt1, pt2, pt, &ptProj, true) >= 1)
	{
		float fDisProj = mag(ptProj - pt);

		return fDisProj <= std::abs(fSnap);
	}

	return false;
}

/***********************************************************************************
@brief		Check point on a line given by two points
@param		[in] pt1	: First point in line
@param		[in] pt2	: Second point in line
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
API_EXPR GBool is_point_in_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	Vec2D vp1p = pt - pt1; // Vector vp1p ;
	Vec2D vp2p = pt - pt2; // Vector vp2p ;

	GFloat fCrs = cross(vp1p, vp2p);

	// Point in straight line
	if (is_equal(fCrs, 0.f, MATH_EPSILON))
	{
		return GTrue;
	}
	return GFalse;
}

/***********************************************************************************
@brief		Check snap point point to line
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
API_EXPR GBool is_snap_point_to_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const GFloat& fSnap)
{
	Point2D ptProj = get_projection_point_to_line(pt1, pt2, pt);

	GFloat fDisProj = mag(ptProj - pt);

	return (fDisProj <= std::abs(fSnap))? GTrue : GFalse;
}

/***********************************************************************************
@brief		check point inside polygon
@param		[in] pt		: Point check
@param		[in] poly	: Polygon
@return		TRUE : inside | FALSE : outside
@note		ref : https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
***********************************************************************************/
API_EXPR GBool is_point_in_polygon(const Point2D& pt, const VecPoint2D& poly)
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
*! @brief  : Check point is in ray (ray : point start and unit vector)
*! @param  : [in] pt  : start point of ray
*! @param  : [in] vn  : unit vector of ray
*! @param  : [in] ptc : point check
*! @return : TRUE : in ray | FALSE : out ray
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
API_EXPR GBool is_point_in_ray(const Point2D& pt, const Vec2D& vn, const Point2D& ptc)
{
	Point2D pt2 = move(pt, vn, 1.f);

	if (is_point_in_line(pt, pt2, ptc))
	{
		// unit vector ptpc
		Vec2D vptpc = normalize(ptc - pt);

		return  (vptpc.x * vn.x) >= 0 &&
				(vptpc.y * vn.y) >= 0;
	}

	return GFalse;
}

}}
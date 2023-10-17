////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xalgorithm.h
* @create   Nov 15, 2023
* @brief    Arithmetic algorithm
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGEOMETRY_H
#define XGEOMETRY_H

#include "xgeotype.h"


#define GEO_XY_EPSILON		0.0001f
#define GEO_EPSILON			0.01f

___BEGIN_NAMESPACE___

//==================================================================================
//⮟⮟ Section : function geometry
//==================================================================================
____BEGIN_SECTION____(geo)

////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------
// Vector base function
//----------------------------------------------------------------------------------
____BEGIN_SECTION____(base)
FLOAT			mag(const Vec2D& v);
Vec2D			normalize(const Vec2D& v);
FLOAT			cross(const Vec2D& v1, const Vec2D& v2);
FLOAT			dot(const Vec2D& v1, const Vec2D& v2);
FLOAT			angle(const Vec2D& v1, const Vec2D& v2);

Point2D			move(const Point2D& pt, const Vec2D& vn, const FLOAT fDistance);
Vec2D			rotate(const Vec2D& v, const FLOAT fDegree);
Point2D			rotate(const Point2D& ptPivot, const Point2D& ptRotate, const FLOAT fDegree);
_____END_SECTION_____

////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------
// Geometry function
//-----------------------------------------------------------------------------------
____BEGIN_SECTION____(algo)

BOOL			is_zero(const Vec2D& v, const float& fepsilon);
BOOL			is_same_direction(const Vec2D& v1, const Point2D& v2);
BOOL			is_same_sign(const Vec2D& v1, const Point2D& v2);

Point2D			get_projection_point_to_line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt); // line = two point
INT				get_projection_point_to_lsegment(const Point2D& ptSeg1, const Point2D& ptSeg2, const Point2D& pt, Point2D* ptPerp = NULL, BOOL bCheckNearest = FALSE);// Segment = two point

BOOL			is_point_in_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);
BOOL			is_snap_point_to_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const float& fSnap);
BOOL			is_point_in_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);
BOOL			is_snap_point_to_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const float& fSnap);
BOOL			is_point_in_polygon(const Point2D& pt, const VecPoint2D& poly);
BOOL			is_point_in_ray(const Point2D& pt, const Vec2D& vn, const Point2D& ptc);                 // ray  = 1 point + unit vector

int				get_rel_point_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);
EnumOrien		get_orientation_point_vector(const Point2D& ptC, const Point2D& pt1, const Point2D& pt2);

BOOL			intersect_2line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3, const Point2D& pt4, Point2D* pInter = NULL);
BOOL			intersect_2lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3, const Point2D& pt4, Point2D* pInter = NULL);
BOOL			intersect_line_lsegment(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& ptSeg1, const Point2D& ptSeg2, Point2D* pInter = NULL);  // line = two point
BOOL			intersect_line_lsegment2(const Point2D& ptLine, const Vec2D& vnLine, const Point2D& ptSeg1, const Point2D& ptSeg2, Point2D* pInter = NULL);     // line = 1 point + unit vector
INT				intersect_line_Polygon(const Point2D& ptLine1, const Point2D& ptLine2, const VecPoint2D& poly, VecPoint2D* vecInter = NULL, BOOL bOnlyCheck = FALSE);
INT				IntersectLine2Rectangle(const Point2D& ptLine1, const Point2D& ptLine2, const Rect2D& rect, VecPoint2D* vecInter = NULL, BOOL bOnlyCheck = FALSE);
INT				IntersectSegment2Polygon(const Point2D& pt1, const Point2D& pt2, const VecPoint2D& poly, VecPoint2D* vecInter = NULL, BOOL bOnlyCheck = FALSE);

BOOL			IsInsidePolygon(const VecPoint2D& poly1, const VecPoint2D& poly2);
INT				GetRelation2Polygon(const VecPoint2D& vecPoly1, const VecPoint2D& vecPoly2);


_____END_SECTION_____

////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------
// Support function
//----------------------------------------------------------------------------------
____BEGIN_SECTION____(sp)
BOOL			is_equal(const FLOAT fVal1, const FLOAT fVal2, const FLOAT fEpsilon);
VecPoint2D		ConvertRectF2Points(const Rect2D& rect);
BOOL			is_ccw(const VecPoint2D& poly);
FLOAT			r2d(const FLOAT fRad);
FLOAT			d2r(const FLOAT fDegree);
_____END_SECTION_____
///////////////////////////////////////////////////////////////////////////////////


/***********************************************************************************
@brief		Check for two equal values using tolerance
@param		[in] fVal1		: First value
@param		[in] fVal1		: Second value
@param		[in] fEpsilon	: tolerance
@return		TRUE : equal | FALSE : no equal
***********************************************************************************/
inline BOOL is_equal(const FLOAT fVal1, const FLOAT fVal2, const FLOAT fEpsilon)
{
	FLOAT fDiff = fVal2 - fVal1;
	return (std::fabs(fDiff) <= fEpsilon);
}

/***********************************************************************************
@brief		Calculate vector magnitude
@param		[in] v : Vector
@return		FLOAT : magnitude
***********************************************************************************/
inline FLOAT mag(const Vec2D& v)
{
	return std::sqrtf(v.x * v.x + v.y * v.y);

}

/***********************************************************************************
*! @brief  : Calculation of unit vector
*! @param  : [in] v : vector
*! @return : uinit vector
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
inline Vec2D normalize(const Vec2D& v)
{
	FLOAT fmagnitude = mag(v);

	if (fmagnitude <= 0)
	{
		return Vec2D(0.f, 0.f);
	}
	return Vec2D(v.x / fmagnitude, v.y / fmagnitude);
}

/***********************************************************************************
@brief		Calculate the crossProduct of the two vector
@param		[in] v1		: First vector
@param		[in] v2		: Second vector
@return		FLOAT : Value cross product
***********************************************************************************/
inline FLOAT cross(const Vec2D& v1, const Vec2D& v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}

/***********************************************************************************
*! @brief  : Calculate dot product between 2 vectors
*! @param  : [in] fangle : angle float
*! @return : FLOAT : dot product
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
inline FLOAT dot(const Vec2D& v1, const Vec2D& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

/***********************************************************************************
*! @brief  : Calculate angle between 2 vectors (degree)
*! @param  : [in] v1 : unit vector v1
*! @param  : [in] v2 : unit vector v2
*! @return : Degree angle between two vector [-180~180]
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
inline FLOAT angle(const Vec2D& v1, const Vec2D& v2)
{
	FLOAT fdot = dot(v1, v2);
	FLOAT fDet = v1.x * v2.y - v1.y * v2.x;

	FLOAT fangle = std::atan2f(fDet, fdot);

	return geo::sp::r2d(fangle);
}

/***********************************************************************************
*! @brief  : move point use unit vector and distance
*! @param  : [in] pt       : move point
*! @param  : [in] vn       : unit vector ( move direction)
*! @param  : [in] fDistance: distance
*! @return : Point2D point after move
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
inline Point2D move(const Point2D& pt, const Vec2D& vn, const FLOAT fDistance)
{
	Point2D ptmove;

	ptmove.x = pt.x + vn.x * fDistance;
	ptmove.y = pt.y + vn.y * fDistance;

	return ptmove;
}

/***********************************************************************************
*! @brief  : Rotate vector (degree)
*! @param  : [in] v      : unit vector
*! @param  : [in] fDegree: degree angle
*! @return : Vector2D after rotate
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
inline Vec2D rotate(const Vec2D& v, const FLOAT fDegree)
{
	FLOAT fRadangle = geo::sp::d2r(fDegree);

	Vec2D vRoate;
	vRoate.x = std::cos(fRadangle) * v.x - sin(fRadangle) * v.y;
	vRoate.y = std::sin(fRadangle) * v.x + cos(fRadangle) * v.y;
	return vRoate;
}

/***********************************************************************************
*! @brief  : Rotate point (degree)
*! @param  : [in] ptPivot : pivot point
*! @param  : [in] ptRotate: rotation point
*! @param  : [in] fDegree : degree angle
*! @return : Point2D after rotate
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
inline Point2D	 rotate(const Point2D& ptPivot, const Point2D& ptRotate, const FLOAT fDegree)
{
	Vec2D vpipr = ptRotate - ptPivot;
	FLOAT fDistance = mag(vpipr);

	Vec2D vuint_pipr = normalize(vpipr);
	Vec2D vuint_rotate = rotate(vuint_pipr, fDegree);

	Point2D ptmove = move(ptPivot, vuint_rotate, fDistance);

	return ptmove;
}


inline BOOL is_zero(const Vec2D& v, const float& fepsilon)
{
	return is_equal(v.x, 0.f, fepsilon) && is_equal(v.x, 0.f, fepsilon);
}

/***********************************************************************************
@brief		Check point on a line segment given by two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
inline BOOL is_same_direction(const Vec2D& v1, const Point2D& v2)
{
	Vec2D vn1 = base::normalize(v1);
	Vec2D vn2 = base::normalize(v2);

	FLOAT fCrs = base::cross(vn1, vn2);

	// Point in straight line
	if (is_equal(fCrs, 0.f, MATH_EPSILON))
	{
		return TRUE;
	}
	return FALSE;
}

/***********************************************************************************
@brief		Check point on a line segment given by two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
inline BOOL is_same_sign(const Vec2D& v1, const Point2D& v2)
{
	return (v1.x * v2.x) >= 0 && (v1.y * v2.y) >= 0;
}

/***********************************************************************************
@brief		Check point on a line segment given by two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
inline BOOL is_point_in_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	Vec2D vp1p = pt - pt1; // Vector vp1p ;
	Vec2D vp2p = pt - pt2; // Vector vp2p ;

	FLOAT fCrs = cross(vp1p, vp2p);

	// Point in straight line
	if (is_equal(fCrs, 0.f, MATH_EPSILON))
	{
		// Point in side line Segment
		FLOAT fp1pDistance = mag(vp1p);
		FLOAT fp2pDistance = mag(vp2p);
		FLOAT fp1p2Distance = mag(pt1 - pt2);

		if (fp1pDistance <= fp1p2Distance &&
			fp2pDistance <= fp1p2Distance)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/***********************************************************************************
@brief		Check snap point point to line segement
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
inline BOOL is_snap_point_to_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const float& fSnap)
{
	Point2D ptProj;

	if (algo::get_projection_point_to_lsegment(pt1, pt2, pt, &ptProj, true) >= 1)
	{
		float fDisProj = geo::base::mag(ptProj - pt);

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
inline BOOL is_point_in_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	Vec2D vp1p = pt - pt1; // Vector vp1p ;
	Vec2D vp2p = pt - pt2; // Vector vp2p ;

	FLOAT fCrs = base::cross(vp1p, vp2p);

	// Point in straight line
	if (is_equal(fCrs, 0.f, MATH_EPSILON))
	{
		return TRUE;
	}
	return FALSE;
}

/***********************************************************************************
*! @brief  : Check point in line (line : 1 point + unit vector)
*! @param  : [in] pt   : point in line
*! @param  : [in] vn   : unit vector of line
*! @param  : [in] ptc  : point check
*! @return : TRUE : point in line | FALSE : no
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
inline BOOL is_point_in_line2(const Point2D& pt, const Vec2D& vn, const Point2D& ptc)
{
	Point2D ptpc = ptc - pt;

	FLOAT fcross = cross(vn, ptpc);

	// 3 collinear point if cross product isqual 0
	if (TRUE == is_equal(fcross, 0.f, MATH_EPSILON))
	{
		return TRUE;
	}

	return FALSE;
}


/***********************************************************************************
@brief		Check snap point point to line
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
inline BOOL is_snap_point_to_line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt, const float& fSnap)
{
	Point2D ptProj = algo::get_projection_point_to_line(pt1, pt2, pt);

	float fDisProj = geo::base::mag(ptProj - pt);

	return fDisProj <= std::abs(fSnap);
}


/***********************************************************************************
@brief		Check a point in the bound region between two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside bound | FALSE : outside bound
***********************************************************************************/
inline BOOL IsPointInAreaLineSegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	if (pt.x >= std::min<FLOAT>(pt1.x, pt2.x) &&
		pt.x <= std::max<FLOAT>(pt1.x, pt2.x) &&
		pt.y >= std::min<FLOAT>(pt1.y, pt2.y) &&
		pt.y <= std::max<FLOAT>(pt1.y, pt2.y))
	{
		return TRUE;
	}
	return FALSE;
}

/***********************************************************************************
@brief		check point inside polygon
@param		[in] pt		: Point check
@param		[in] poly	: Polygon
@return		TRUE : inside | FALSE : outside
@note		ref : https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
***********************************************************************************/
inline BOOL is_point_in_polygon(const Point2D& pt, const VecPoint2D& poly)
{
	if (poly.size() < 3)
	{
		assert(0);
		return FALSE;
	}

	FLOAT fMinX = poly[0].x;
	FLOAT fMaxX = poly[0].x;
	FLOAT fMinY = poly[0].y;
	FLOAT fMaxY = poly[0].y;

	int nPolyCount = static_cast<int>(poly.size());

	for (int i = 1; i < nPolyCount; i++)
	{
		fMinX = std::min<FLOAT>(poly[i].x, fMinX);
		fMaxX = std::max<FLOAT>(poly[i].x, fMaxX);
		fMinY = std::min<FLOAT>(poly[i].y, fMinY);
		fMaxY = std::max<FLOAT>(poly[i].y, fMaxY);
	}

	if (pt.x < fMinX || pt.x > fMaxX || pt.y < fMinY || pt.y > fMaxY)
		return FALSE;

	BOOL bInside = FALSE;
	for (int i = 0, j = nPolyCount - 1; i < nPolyCount; j = i++)
	{
		if ((poly[i].y > pt.y) != (poly[j].y > pt.y) &&
			pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)
		{
			bInside = !bInside;
		}

		// case inside edge and collinear
		else if (is_equal(poly[i].y, pt.y, GEO_XY_EPSILON) &&
				 is_equal(poly[j].y, pt.y, GEO_XY_EPSILON))
		{
			if ((poly[i].x > pt.x) != (poly[j].x > pt.x))
			{
				return TRUE;
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
inline BOOL is_point_in_ray(const Point2D& pt, const Vec2D& vn, const Point2D& ptc)
{
	Point2D pt2 = base::move(pt, vn, 1.f);

	if (is_point_in_line(pt, pt2, ptc))
	{
		// unit vector ptpc
		Vec2D vptpc = normalize(ptc - pt);

		return  (vptpc.x * vn.x) >= 0 &&
			(vptpc.y * vn.y) >= 0;
	}

	return FALSE;
}

/***********************************************************************************
*! @brief  : Check point is in ray (ray : point start and unit vector)
*! @param  : [in] pt  : start point of ray
*! @param  : [in] vn  : unit vector of ray
*! @param  : [in] ptc : point check
*! @return : 0 : not in line segment
*!           1 : in line segment
*!           2 : same p1
*!           3 : same p2
*! @author : thuong.nv			- [Date] : 17/10/2023
***********************************************************************************/
inline int	 get_rel_point_lsegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	Vec2D vp1p  = pt - pt1; // Vector vp1p ;
	Vec2D vp1p2 = pt2 - pt1; // Vector vp2p ;

	float fCrs = base::cross(vp1p, vp1p2);

	// Điểm không nằm trên đường thẳng
	if (!is_equal(fCrs, 0.f, GEO_EPSILON))
		return 0;

	float fDot_p1p_p1p2 = dot(vp1p2, vp1p);

	// Điểm p gần điểm p1
	if (fDot_p1p_p1p2 < 0)
		return 0;
	if (is_equal(fDot_p1p_p1p2, 0, GEO_EPSILON) == true)
		return 2;

	// Điểm p gần điểm p2
	float fDot_p1p2_p1p2 = dot(vp1p2, vp1p2);

	if (fDot_p1p_p1p2 > fDot_p1p2_p1p2)
		return 0;
	if (is_equal(fDot_p1p_p1p2, fDot_p1p2_p1p2, GEO_EPSILON) == true)
		return 2;

	return 1;
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
inline EnumOrien get_orientation_point_vector(const Point2D& ptC, const Point2D& pt1, const Point2D& pt2)
{
	const float fOrin = (pt2.x - pt1.x) * (ptC.y - pt1.y) - (ptC.x - pt1.x) * (pt2.y - pt1.y);

	if (is_equal(fOrin, 0.f, GEO_EPSILON))	return EnumOrien::COLLINEAR; /* ptc and p1p2 collinear */
	else if (fOrin < 0.f)					return EnumOrien::RIGHT; /* ptc on the right p1p2 */
	else									return EnumOrien::LEFT; /* ptc on the left p1p2 */
}

/***********************************************************************************
@brief		Get intersection point between line and line (line = 2 point)
@param		[in]  pt1	 : Point in Line 1
@param		[in]  pt2	 : Point in Line 1
@param		[in]  pt3	 : Point in Line 2
@param		[in]  pt4	 : Point in Line 2
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
inline BOOL intersect_2line(const Point2D& pt1,		// L1
					 const Point2D& pt2,		// L1
					 const Point2D& pt3,		// L2
					 const Point2D& pt4,		// L2
						   Point2D* pInter	/*= NULL*/)
{
	// Equation of the first straight line Segment  : ax +by = c
	FLOAT fa = pt2.y - pt1.y;
	FLOAT fb = pt1.x - pt2.x;
	FLOAT fc = fa * (pt1.x) + fb * (pt1.y);

	// Equation of the second straight line Segment : a1x +b1y = c1
	FLOAT fa1 = pt4.y - pt3.y;
	FLOAT fb1 = pt3.x - pt4.x;
	FLOAT fc1 = fa1 * (pt3.x) + fb1 * (pt3.y);

	FLOAT fDet = fa * fb1 - fb * fa1;

	Vec2D ptIntersect = Point2D(FLT_MAX, FLT_MAX);
	BOOL bInter = FALSE;

	// Check not parallel line Segment
	if (is_equal(fDet, 0.f, MATH_EPSILON) == FALSE)
	{
		ptIntersect.x = (fb1 * fc - fb * fc1) / fDet;
		ptIntersect.y = (fa * fc1 - fa1 * fc) / fDet;

		bInter = TRUE;
	}

	if (pInter)
	{
		*pInter = ptIntersect;
	}

	return bInter;
}

/***********************************************************************************
@brief		Checks if two line segments intersect and returns the intersection point
@param		[in]  pt1	 : Point in Line segment 1
@param		[in]  pt2	 : Point in Line segment 1
@param		[in]  pt3	 : Point in Line segment 2
@param		[in]  pt4	 : Point in Line segment 2
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
inline BOOL intersect_2lsegment(const Point2D& pt1,		// L1
						 const Point2D& pt2,		// L1
						 const Point2D& pt3,		// L2
						 const Point2D& pt4,		// L2
							   Point2D* pInter	/*= NULL*/)
{
	// Equation of the first straight line Segment  : ax +by = c
	FLOAT fa = pt2.y - pt1.y;
	FLOAT fb = pt1.x - pt2.x;
	FLOAT fc = fa * (pt1.x) + fb * (pt1.y);

	// Equation of the second straight line Segment : a1x +b1y = c1
	FLOAT fa1 = pt4.y - pt3.y;
	FLOAT fb1 = pt3.x - pt4.x;
	FLOAT fc1 = fa1 * (pt3.x) + fb1 * (pt3.y);

	FLOAT fDet = fa * fb1 - fb * fa1;

	Vec2D ptIntersect = Point2D(FLT_MAX, FLT_MAX);
	BOOL bInter = FALSE;

	// Check not parallel line Segment
	if (is_equal(fDet, 0.f, MATH_EPSILON) == FALSE)
	{
		ptIntersect.x = (fb1 * fc - fb * fc1) / fDet;
		ptIntersect.y = (fa * fc1 - fa1 * fc) / fDet;

		// Inside intersection 
		if (is_point_in_lsegment(pt1, pt2, ptIntersect) &&
			is_point_in_lsegment(pt3, pt4, ptIntersect))
		{
			bInter = TRUE;
		}
		else // Outside intersection
		{
			bInter = FALSE;
		}
	}

	if (pInter)
	{
		*pInter = ptIntersect;
	}

	return bInter;
}

/***********************************************************************************
@brief		Get intersection point between line and line segment (line = two point)
@param		[in]  ptLine1 : Point on the line
@param		[in]  ptLine2 : Point on the line
@param		[in]  ptSeg1  : Point start on line segment
@param		[in]  ptSeg2  : Point end in line segment
@param		[out] pInter  : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
inline BOOL IntersectLine2Segment(const Point2D& ptLine1,	// Point on the line
						   const Point2D& ptLine2,	// Point on the line
						   const Point2D& ptSeg1,	// Point start on line segment
						   const Point2D& ptSeg2,	// Point end on line segment
								 Point2D* pInter	/*= NULL*/)
{
	Point2D ptInter;

	if (TRUE == algo::intersect_2line(ptLine1, ptLine2, ptSeg1, ptSeg2, &ptInter))
	{
		// Check point inside line segment
		FLOAT fp1pDistance = mag(ptSeg1 - ptInter);
		FLOAT fp2pDistance = mag(ptSeg2 - ptInter);
		FLOAT fp1p2Distance = mag(ptSeg1 - ptSeg2);

		if (fp1pDistance <= fp1p2Distance &&
			fp2pDistance <= fp1p2Distance)
		{
			if (pInter)
			{
				*pInter = ptInter;
			}
			return TRUE;
		}
	}
	return FALSE;
}

/***********************************************************************************
@brief		Get intersection point between line and line segment (line = 1 point + unit vector)
@param		[in]  ptLine : Point on the line
@param		[in]  vnLine : Unit vector line
@param		[in]  ptSeg1 : Point start in line segment
@param		[in]  ptSeg2 : Point end in line segment
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
inline BOOL IntersectLine2Segment2(const Point2D&	ptLine,	// Point on the line
							const Vec2D&	vnLine,	// Unit vector line
							const Point2D&	ptSeg1,	// Point start in line segment
							const Point2D&	ptSeg2,	// Point end in line segment
								  Point2D*	pInter	/*= NULL*/)
{
	Point2D ptInter;
	Point2D ptLine2 = ptLine + vnLine;

	if (TRUE == IntersectLine2Segment(ptLine, ptLine2, ptSeg1, ptSeg2, &ptInter))
	{
		if (pInter)
		{
			*pInter = ptInter;
		}

		return TRUE;
	}

	return FALSE;
}

/***********************************************************************************
@brief		Get intersection points between line and polygon (line = two point)
@param		[in]  ptLine1    : Point on the line
@param		[in]  ptLine2    : Point on the line
@param		[in]  poly       : polygon
@param		[out] vecInter   : point list of intersections
@return		INT : intersection number.
***********************************************************************************/
inline INT IntersectLine2Polygon(const Point2D&	 ptLine1,	 //[in] Point on the line
						  const Point2D&	 ptLine2,	 //[in] Point on the line
						  const VecPoint2D&  poly,		 //[in] polygon
								VecPoint2D*  vecInter,  /*= NULL */   //[out] point list of intersections
								BOOL		 bOnlyCheck /*= FALSE*/) //[in]  only check if intersect or not
{
	if (poly.size() < 3)
	{
		assert(0);
		return 0;
	}

	int nNext, nInter = 0;
	int nPolyCount = static_cast<int>(poly.size());

	Point2D ptInter;
	for (int i = 0; i < nPolyCount; i++)
	{
		nNext = (i + 1) % nPolyCount; // next point index

		if (IntersectLine2Segment(ptLine1, ptLine2, poly[i], poly[nNext], &ptInter) == TRUE)
		{
			nInter++;

			if (vecInter)
			{
				vecInter->push_back(ptInter);
			}

			// Just intersect will return always.
			if (TRUE == bOnlyCheck)
			{
				break;
			}
		}
	}

	return nInter;
}

/***********************************************************************************
@brief		Get intersection points between line and rect (line = two point)
@param		[in]  ptLine1 : Point on the line
@param		[in]  ptLine2 : Point on the line
@param		[in]  rect    : rectangle
@param		[out] vecInter: point list of intersections
@return		INT : intersection number.
***********************************************************************************/
inline INT IntersectLine2Rectangle(const Point2D&		ptLine1,	 //[in] Point on the line
							const Point2D&		ptLine2,	 //[in] Point on the line
							const Rect2D&		rect,		//[in] rectangle
								  VecPoint2D*	vecInter,	/*= NULL */   //[out] point list of intersections
								  BOOL			bOnlyCheck	/*= FALSE*/) //[in]  only check if intersect or not
{
	VecPoint2D poly = std::move(sp::ConvertRectF2Points(rect));

	return IntersectLine2Polygon(ptLine1, ptLine2, poly, vecInter, bOnlyCheck);
}

/***********************************************************************************
@brief		get intersection point between line segment and polygon
@param		[in]  pt1	    : Point on Line segment
@param		[in]  pt2	    : Point on Line segment
@param		[in]  poly	    : polygon
@param		[out] vecInter  : point list of intersections
@param		[in]  bOnlyCheck: only check if intersect or not
@return		INT : intersection number.
***********************************************************************************/
inline INT IntersectSegment2Polygon(const Point2D&		pt1,	//[in] Point on the line
							 const Point2D&		pt2,	//[in] Point on the line
							 const VecPoint2D&	poly,	//[in] polygon
								   VecPoint2D*	vecInter,  /*= NULL*/    //[out] point list of intersections
								   BOOL			bOnlyCheck /*= FALSE*/) //[in]  only check if intersect or not
{
	if (poly.size() < 3)
	{
		assert(0);
		return FALSE;
	}

	int nNext, nInter = 0;
	int nPolyCount = static_cast<int>(poly.size());

	Point2D ptInter;
	for (int i = 0; i < nPolyCount; i++)
	{
		nNext = (i + 1) % nPolyCount; // next point index

		if (algo::intersect_2lsegment(pt1, pt2, poly[i], poly[nNext], &ptInter) == TRUE)
		{
			nInter++;

			if (vecInter)
			{
				vecInter->push_back(ptInter);
			}

			// Just intersect will return always.
			if (TRUE == bOnlyCheck)
			{
				break;
			}
		}
	}

	return nInter;
}

/***********************************************************************************
@brief		Check that a polygon is completely inside another polygon
@param		[in]  poly1	 : Polygon check
@param		[in]  poly2	 : Polygon outside
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
inline BOOL IsInsidePolygon(const VecPoint2D& poly1, const VecPoint2D& poly2)
{
	if (poly1.size() < 3 || poly2.size() < 3)
	{
		assert(0);
		return FALSE;
	}

	int nNext;
	int nPolyCount = static_cast<int>(poly1.size());

	// Check polygon onside or inside
	for (int i = 0; i < nPolyCount; i++)
	{
		nNext = (i + 1) % nPolyCount; // next point index

		if (IntersectSegment2Polygon(poly1[i], poly1[nNext], poly2, NULL, TRUE) >= 1)
			return FALSE;
	}

	// Check polygon outside
	if (!is_point_in_polygon(poly1[0], poly2))
		return FALSE;

	return TRUE;
}

#pragma warning(push)
#pragma warning(disable : 26812)
/***********************************************************************************
@brief		Get the relationship between 2 polygons
@param		[in]  poly1	 : first polygon
@param		[in]  poly2	 : second polygon
@return		| INVALID  (-1) : invalid param
			| OUTSIDE  ( 0) : outside
			| INTER    ( 1) : intersect
			| INSIDE_1 ( 2) : poly1 inside poly2
			| INSIDE_2 ( 3) : poly2 inside poly1
@note		poly1 and poly 2 have number points greater than 3
***********************************************************************************/
inline INT GetRelation2Polygon(const VecPoint2D& vecPoly1, const VecPoint2D& vecPoly2)
{
	if (vecPoly1.size() < 3 || vecPoly2.size() < 3)
	{
		assert(0);
		return -1;
	}

	int nNext;
	int nPolyCount = static_cast<int>(vecPoly1.size());

	// Case 1 : intersect - check 2 intersecting polygons
	for (int i = 0; i < nPolyCount; i++)
	{
		nNext = (i + 1) % nPolyCount; // next point index

		if (IntersectSegment2Polygon(vecPoly1[i], vecPoly1[nNext], vecPoly2, NULL, TRUE) >= 1)
			return 1;
	}

	// Case 2 : inside - check poly1 inside poly2 
	if (TRUE == is_point_in_polygon(vecPoly1[0], vecPoly2))
		return 2;

	// Case 3 : inside - check poly2 inside poly1 
	if (TRUE == is_point_in_polygon(vecPoly2[0], vecPoly1))
		return 3;

	// Case 4 (default): outside
	return 0;
}
#pragma warning(pop) 

/***********************************************************************************
*! @brief  : Find the perpendicular projection of a point onto the line. (line = 2 point)
*! @param  : [in] ptLine1  : point in line
*! @param  : [in] ptLine2  : point in line
*! @param  : [in] pt       : get point
*! @return : Point2D perpendicular projection of a point
*! @author : thuong.nv			- [Date] : 04/07/2023
***********************************************************************************/
inline Point2D get_projection_point_to_line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt)
{
	Vec2D ptPer;
	Vec2D vp1p2	= ptLine2 - ptLine1;  // p1p2
	Vec2D vp1p	= pt      - ptLine1;  // p1p
	Vec2D vp2p	= pt      - ptLine2;  // p2p

	FLOAT fDis = vp1p2.x * vp1p2.x + vp1p2.y * vp1p2.y;

	if (FALSE == is_equal(fDis, 0.f, MATH_EPSILON))
	{
		FLOAT fDet = dot(vp1p, vp1p2);

		FLOAT t = fDet / fDis;
		ptPer.x = ptLine1.x + t * (ptLine2.x - ptLine1.x);
		ptPer.y = ptLine1.y + t * (ptLine2.y - ptLine1.y);
	}
	else
	{
		ptPer = pt; // case 3 points coincide
	}

	return ptPer;
}

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
inline INT get_projection_point_to_lsegment(const  Point2D& ptSeg1, const Point2D& ptSeg2,
									 const  Point2D& pt,
											Point2D* pptPerp	  /* = NULL*/,
											BOOL	 bCheckNearest/* = FALSE*/)
{
	INT iRet = 0;

	Point2D ptPer = get_projection_point_to_line(ptSeg1, ptSeg2, pt);

	// Point in side line Segment
	FLOAT fdot1 = dot(ptSeg2 - ptSeg1, ptPer - ptSeg1);
	FLOAT fdot2 = dot(ptSeg1 - ptSeg2, ptPer - ptSeg2);

	if (fdot1 >= MATH_EPSILON && fdot2 >= MATH_EPSILON)
	{
		iRet = 1;
	}
	else if (bCheckNearest == TRUE)
	{
		FLOAT fDis1 = mag(ptSeg1 - ptPer);
		FLOAT fDis2 = mag(ptSeg2 - ptPer);
		iRet = (fDis1 <= fDis2) ? 2 : 3;
	}

	if (pptPerp)
	{
		if (iRet == 1)
		{
			*pptPerp = ptPer;
		}
		else if (iRet == 2)
		{
			*pptPerp = ptSeg1;
		}
		else if (iRet == 3)
		{
			*pptPerp = ptSeg2;
		}
	}

	return iRet;
}




/***********************************************************************************
@brief		Check polygon is counter-clockwise
@param		[in] polyg : polygon
@return		TRUE : is CCW | FALSE : CW
***********************************************************************************/
inline BOOL is_ccw(const VecPoint2D& poly)
{
	int nPolyCnt = static_cast<int>(poly.size());

	int nNext; float fSum = 0.f;

	for (int i = 0; i < nPolyCnt; i++)
	{
		nNext = (i + 1) % nPolyCnt;
		fSum += (poly[nNext].x - poly[i].x) * (poly[nNext].y + poly[i].y);
	}

	return (fSum <= 0.f) ? FALSE : TRUE;
}

/***********************************************************************************
@brief		Check polygon is convex
@param		[in] poly : polygon
@return		TRUE : is convex | FALSE : no convex
***********************************************************************************/
inline BOOL IsConvexPolygon(const VecPoint2D& poly)
{
	int nPolyCnt = static_cast<int>(poly.size());

	if (nPolyCnt < 3)
		return FALSE;

	EnumOrien ePreOrien = EnumOrien::COLLINEAR;

	for (int i = 0; i < nPolyCnt - 2; i++)
	{
		EnumOrien eCurOrien = get_orientation_point_vector(poly[i], poly[i + 1], poly[i + 2]);

		if (eCurOrien != EnumOrien::COLLINEAR &&
			(ePreOrien != EnumOrien::COLLINEAR && eCurOrien != ePreOrien))
		{
			return FALSE;
		}

		ePreOrien = eCurOrien;
	}

	return TRUE;
}

/***********************************************************************************
@brief		Convert a rect into a list of points
@param		[in]  rect	 : RectF
@return		List pointF
@note		It can be counterclockwise or clockwise
***********************************************************************************/
inline VecPoint2D ConvertRectF2Points(const Rect2D& rect)
{
	VecPoint2D vecPointPoly;

	vecPointPoly.push_back({ rect.x, rect.y });
	vecPointPoly.push_back({ rect.x + rect.width, rect.y});
	vecPointPoly.push_back({ rect.x + rect.width, rect.y + rect.height });
	vecPointPoly.push_back({ rect.x, rect.y + rect.height });

	return vecPointPoly;
}

/***********************************************************************************
*! @brief  : Convert from Radian to Degree
*! @param  : [in] fRad : angle float (radian)
*! @return : angle after convert
***********************************************************************************/
inline FLOAT r2d(const FLOAT fRad)
{
	return fRad * 180.f / float(PI);
}

/***********************************************************************************
*! @brief  : Convert from Degree to Radian
*! @param  : [in] fDegree : angle float (degree)
*! @return : angle after convert
***********************************************************************************/
inline FLOAT d2r(const FLOAT fDegree)
{
	return fDegree * float(PI) / 180.f;
}
_____END_SECTION_____

____END_NAMESPACE____

#endif // XGEOMETRY_H
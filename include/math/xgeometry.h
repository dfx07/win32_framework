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

#include "xsysdef.h"
#include "xmath.h"

#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <assert.h>

#define USE_GLM

#ifdef USE_GLM
	#include "glm/glm.hpp"
#else
	//#include "glm/glm.hpp"
#endif

#undef TRUE
	#define TRUE    true
#undef FALSE
	#define FALSE   false
#undef ASSERT
	#define ASSERT(expr)  assert(expr)

#define GEO_XY_EPSILON  0.0001f

typedef float		FLOAT;
typedef bool		BOOL;
typedef int			INT;

///////////////////////////////////////////////////////////////////////////////////
// Define use GLM
#ifdef USE_GLM
	typedef glm::vec2			Vec2D;
	typedef glm::vec2			Point2D;
	typedef glm::vec3			Vec3D;
	typedef std::vector<Vec2D>	VecPoint2D;
	typedef fox::tagRect2D		Rect2D;
///////////////////////////////////////////////////////////////////////////////////
// Define use User define
#else
	typedef fox::tagVec2D		Vec2D;
	typedef fox::tagVec2D		Point2D;
	typedef fox::tagVec3D		Vec3D;
	typedef std::vector<Vec2D>	VecPoint2D;
	typedef fox::tagRect2D		Rect2D;
#endif


___BEGIN_NAMESPACE___

//==================================================================================
//⮟⮟ Section : function geometry
//==================================================================================
____BEGIN_SECTION____(geo)

////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------
// Vector base function
//----------------------------------------------------------------------------------
FLOAT			GetMagnitude(const Vec2D& v);
Vec2D			NormalizeVector(const Vec2D& v);
FLOAT			GetCrossProduct(const Vec2D& v1, const Vec2D& v2);
FLOAT			GetDotProduct(const Vec2D& v1, const Vec2D& v2);
FLOAT			CalcAngle2Vector(const Vec2D& v1, const Vec2D& v2);

Point2D			Move(const Point2D& pt, const Vec2D& vn, const FLOAT fDistance);
Vec2D			Rotate(const Vec2D& v, const FLOAT fDegree);
Point2D			RotatePoint(const Point2D& ptPivot, const Point2D& ptRotate, const FLOAT fDegree);

//-----------------------------------------------------------------------------------
// Geometry function
//-----------------------------------------------------------------------------------
BOOL			IsPointInLineSegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);
BOOL			IsPointInAreaLineSegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt);
BOOL			IsPointInPolygon(const Point2D& pt, const VecPoint2D& poly);
BOOL			Intersect2Line(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3, const Point2D& pt4, Point2D* pInter = NULL);
BOOL			Intersect2Line2(const Point2D& ptLine1, const Vec2D& vnLine1, const Point2D& ptLine2, const Vec2D& vnLine2, Point2D* pInter = NULL);
BOOL			Intersect2Segment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3, const Point2D& pt4, Point2D* pInter = NULL);
BOOL			IntersectLine2Segment(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& ptSeg1, const Point2D& ptSeg2, Point2D* pInter = NULL);  // line = two point
BOOL			IntersectLine2Segment2(const Point2D& ptLine, const Vec2D& vnLine, const Point2D& ptSeg1, const Point2D& ptSeg2, Point2D* pInter = NULL);     // line = 1 point + unit vector
INT				IntersectLine2Polygon(const Point2D& ptLine1, const Point2D& ptLine2, const VecPoint2D& poly, VecPoint2D* vecInter = NULL, BOOL bOnlyCheck = FALSE);
INT				IntersectLine2Rectangle(const Point2D& ptLine1, const Point2D& ptLine2, const Rect2D& rect, VecPoint2D* vecInter = NULL, BOOL bOnlyCheck = FALSE);
INT				IntersectSegment2Polygon(const Point2D& pt1, const Point2D& pt2, const VecPoint2D& poly, VecPoint2D* vecInter = NULL, BOOL bOnlyCheck = FALSE);

BOOL			IsInsidePolygon(const VecPoint2D& poly1, const VecPoint2D& poly2);
INT				GetRelation2Polygon(const VecPoint2D& vecPoly1, const VecPoint2D& vecPoly2);

BOOL			IsPointInLine(const Point2D& pt1, const Point2D& pt2, const Point2D& ptc);            // line = two point
BOOL			IsPointInLine2(const Point2D& pt, const Vec2D& vn, const Point2D& ptc);               // line = 1 point + unit vector
Point2D			GetPerpPoint2Line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt); // line = two point

BOOL			IsPointInRay(const Point2D& pt, const Vec2D& vn, const Point2D& ptc);                 // ray  = 1 point + unit vector

//--------------------------------------------------------------------------------------
// Support function
//--------------------------------------------------------------------------------------
BOOL			IsEqual(const FLOAT fVal1, const FLOAT fVal2, const FLOAT fEpsilon);
VecPoint2D		ConvertRectF2Points(const Rect2D& rect);
FLOAT			ConvertRadToDeg(const FLOAT fRad);
FLOAT			ConvertDegToRad(const FLOAT fDegree);
///////////////////////////////////////////////////////////////////////////////////


/***********************************************************************************
@brief		Check for two equal values using tolerance
@param		[in] fVal1		: First value
@param		[in] fVal1		: Second value
@param		[in] fEpsilon	: tolerance
@return		TRUE : equal | FALSE : no equal
***********************************************************************************/
BOOL IsEqual(const FLOAT fVal1, const FLOAT fVal2, const FLOAT fEpsilon)
{
	FLOAT fDiff = fVal2 - fVal1;
	return (std::fabs(fDiff) <= fEpsilon);
}

/***********************************************************************************
@brief		Calculate vector magnitude
@param		[in] v : Vector
@return		FLOAT : magnitude
***********************************************************************************/
FLOAT GetMagnitude(const Vec2D& v)
{
	return std::sqrtf(v.x * v.x + v.y * v.y);
}

/***********************************************************************************
*! @brief  : Calculation of unit vector
*! @param  : [in] v : vector
*! @return : uinit vector
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
Vec2D NormalizeVector(const Vec2D& v)
{
	FLOAT fMagnitude = GetMagnitude(v);

	if (fMagnitude <= 0)
	{
		return Vec2D(0.f, 0.f);
	}
	return Vec2D(v.x / fMagnitude, v.y / fMagnitude);
}

/***********************************************************************************
@brief		Calculate the CrossProduct of the two vector
@param		[in] v1		: First vector
@param		[in] v2		: Second vector
@return		FLOAT : Value cross product
***********************************************************************************/
FLOAT GetCrossProduct(const Vec2D& v1, const Vec2D& v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}

/***********************************************************************************
*! @brief  : Calculate dot product between 2 vectors
*! @param  : [in] fAngle : angle float
*! @return : FLOAT : dot product
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
FLOAT GetDotProduct(const Vec2D& v1, const Vec2D& v2)
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
FLOAT CalcAngle2Vector(const Vec2D& v1, const Vec2D& v2)
{
	FLOAT fDot = GetDotProduct(v1, v2);
	FLOAT fDet = v1.x * v2.y - v1.y * v2.x;

	FLOAT fAngle = std::atan2f(fDet, fDot);

	return ConvertRadToDeg(fAngle);
}

/***********************************************************************************
*! @brief  : Move point use unit vector and distance
*! @param  : [in] pt       : move point
*! @param  : [in] vn       : unit vector ( move direction)
*! @param  : [in] fDistance: distance
*! @return : Point2D point after move
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
Point2D Move(const Point2D& pt, const Vec2D& vn, const FLOAT fDistance)
{
	Point2D ptMove;

	ptMove.x = pt.x + vn.x * fDistance;
	ptMove.y = pt.y + vn.y * fDistance;

	return ptMove;
}

/***********************************************************************************
*! @brief  : Rotate vector (degree)
*! @param  : [in] v      : unit vector
*! @param  : [in] fDegree: degree angle
*! @return : Vector2D after rotate
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
Vec2D Rotate(const Vec2D& v, const FLOAT fDegree)
{
	FLOAT fRadAngle = ConvertDegToRad(fDegree);

	Vec2D vRoate;
	vRoate.x = std::cos(fRadAngle) * v.x - sin(fRadAngle) * v.y;
	vRoate.y = std::sin(fRadAngle) * v.x + cos(fRadAngle) * v.y;
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
Point2D	 RotatePoint(const Point2D& ptPivot, const Point2D& ptRotate, const FLOAT fDegree)
{
	Vec2D vpipr = ptRotate - ptPivot;
	FLOAT fDistance = GetMagnitude(vpipr);

	Vec2D vuint_pipr = NormalizeVector(vpipr);
	Vec2D vuint_rotate = Rotate(vuint_pipr, fDegree);

	Point2D ptMove = Move(ptPivot, vuint_rotate, fDistance);

	return ptMove;
}

/***********************************************************************************
@brief		Check point on a line segment given by two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside | FALSE : outside
***********************************************************************************/
BOOL IsPointInLineSegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
{
	Vec2D vp1p = pt - pt1; // Vector vp1p ;
	Vec2D vp2p = pt - pt2; // Vector vp2p ;

	FLOAT fCrs = GetCrossProduct(vp1p, vp2p);

	// Point in straight line
	if (IsEqual(fCrs, 0.f, MATH_EPSILON))
	{
		// Point in side line Segment
		FLOAT fp1pDistance = GetMagnitude(vp1p);
		FLOAT fp2pDistance = GetMagnitude(vp2p);
		FLOAT fp1p2Distance = GetMagnitude(pt1 - pt2);

		if (fp1pDistance <= fp1p2Distance &&
			fp2pDistance <= fp1p2Distance)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/***********************************************************************************
@brief		Check a point in the bound region between two points
@param		[in] pt1	: First point in line segment
@param		[in] pt2	: Second point in line segment
@param		[in] pt		: Point check
@return		TRUE : inside bound | FALSE : outside bound
***********************************************************************************/
BOOL IsPointInAreaLineSegment(const Point2D& pt1, const Point2D& pt2, const Point2D& pt)
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
BOOL IsPointInPolygon(const Point2D& pt, const VecPoint2D& poly)
{
	if (poly.size() < 3)
	{
		ASSERT(0);
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
		else if (IsEqual(poly[i].y, pt.y, GEO_XY_EPSILON) &&
				 IsEqual(poly[j].y, pt.y, GEO_XY_EPSILON))
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
@brief		Get intersection point between line and line (line = 2 point)
@param		[in]  pt1	 : Point in Line 1
@param		[in]  pt2	 : Point in Line 1
@param		[in]  pt3	 : Point in Line 2
@param		[in]  pt4	 : Point in Line 2
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
BOOL Intersect2Line(const Point2D& pt1,		// L1
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
	if (IsEqual(fDet, 0.f, MATH_EPSILON) == FALSE)
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
@brief		Get intersection point between line and line segment
@param		[in]  ptLine1	 : Point on the line 1
@param		[in]  vnLine1	 : Unit vector line
@param		[in]  ptLine2	 : Point on the line 2
@param		[in]  vnLine2	 : Unit vector line
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
BOOL Intersect2Line2(const Point2D& ptLine1,   // Point on the line 1
					 const Vec2D&	vnLine1,   // Unit vector line
					 const Point2D& ptLine2,   // Point on the line 2
					 const Vec2D&	vnLine2,   // Unit vector line
						   Point2D* pInter     /*= NULL*/)
{
	Point2D ptInter;

	Point2D ptLine12 = ptLine1 + vnLine1;
	Point2D ptLine22 = ptLine2 + vnLine2;

	if (TRUE == Intersect2Line(ptLine1, ptLine12, ptLine2, ptLine22, &ptInter))
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
@brief		Checks if two line segments intersect and returns the intersection point
@param		[in]  pt1	 : Point in Line segment 1
@param		[in]  pt2	 : Point in Line segment 1
@param		[in]  pt3	 : Point in Line segment 2
@param		[in]  pt4	 : Point in Line segment 2
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
BOOL Intersect2Segment(const Point2D& pt1,		// L1
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
	if (IsEqual(fDet, 0.f, MATH_EPSILON) == FALSE)
	{
		ptIntersect.x = (fb1 * fc - fb * fc1) / fDet;
		ptIntersect.y = (fa * fc1 - fa1 * fc) / fDet;

		// Inside intersection 
		if (IsPointInLineSegment(pt1, pt2, ptIntersect) &&
			IsPointInLineSegment(pt3, pt4, ptIntersect))
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
BOOL IntersectLine2Segment(const Point2D& ptLine1,	// Point on the line
						   const Point2D& ptLine2,	// Point on the line
						   const Point2D& ptSeg1,	// Point start on line segment
						   const Point2D& ptSeg2,	// Point end on line segment
								 Point2D* pInter	/*= NULL*/)
{
	Point2D ptInter;

	if (TRUE == Intersect2Line(ptLine1, ptLine2, ptSeg1, ptSeg2, &ptInter))
	{
		// Check point inside line segment
		FLOAT fp1pDistance = GetMagnitude(ptSeg1 - ptInter);
		FLOAT fp2pDistance = GetMagnitude(ptSeg2 - ptInter);
		FLOAT fp1p2Distance = GetMagnitude(ptSeg1 - ptSeg2);

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
BOOL IntersectLine2Segment2(const Point2D&	ptLine,	// Point on the line
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
INT IntersectLine2Polygon(const Point2D&	 ptLine1,	 //[in] Point on the line
						  const Point2D&	 ptLine2,	 //[in] Point on the line
						  const VecPoint2D&  poly,		 //[in] polygon
								VecPoint2D*  vecInter,  /*= NULL */   //[out] point list of intersections
								BOOL		 bOnlyCheck /*= FALSE*/) //[in]  only check if intersect or not
{
	if (poly.size() < 3)
	{
		ASSERT(0);
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
INT IntersectLine2Rectangle(const Point2D&		ptLine1,	 //[in] Point on the line
							const Point2D&		ptLine2,	 //[in] Point on the line
							const Rect2D&		rect,		//[in] rectangle
								  VecPoint2D*	vecInter,	/*= NULL */   //[out] point list of intersections
								  BOOL			bOnlyCheck	/*= FALSE*/) //[in]  only check if intersect or not
{
	VecPoint2D poly = std::move(ConvertRectF2Points(rect));

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
INT IntersectSegment2Polygon(const Point2D&		pt1,	//[in] Point on the line
							 const Point2D&		pt2,	//[in] Point on the line
							 const VecPoint2D&	poly,	//[in] polygon
								   VecPoint2D*	vecInter,  /*= NULL*/    //[out] point list of intersections
								   BOOL			bOnlyCheck /*= FALSE*/) //[in]  only check if intersect or not
{
	if (poly.size() < 3)
	{
		ASSERT(0);
		return FALSE;
	}

	int nNext, nInter = 0;
	int nPolyCount = static_cast<int>(poly.size());

	Point2D ptInter;
	for (int i = 0; i < nPolyCount; i++)
	{
		nNext = (i + 1) % nPolyCount; // next point index

		if (Intersect2Segment(pt1, pt2, poly[i], poly[nNext], &ptInter) == TRUE)
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
BOOL IsInsidePolygon(const VecPoint2D& poly1, const VecPoint2D& poly2)
{
	if (poly1.size() < 3 || poly2.size() < 3)
	{
		ASSERT(0);
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
	if (!IsPointInPolygon(poly1[0], poly2))
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
INT GetRelation2Polygon(const VecPoint2D& vecPoly1, const VecPoint2D& vecPoly2)
{
	if (vecPoly1.size() < 3 || vecPoly2.size() < 3)
	{
		ASSERT(0);
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
	if (TRUE == IsPointInPolygon(vecPoly1[0], vecPoly2))
		return 2;

	// Case 3 : inside - check poly2 inside poly1 
	if (TRUE == IsPointInPolygon(vecPoly2[0], vecPoly1))
		return 3;

	// Case 4 (default): outside
	return 0;
}
#pragma warning(pop) 

/***********************************************************************************
*! @brief  : Check point in line (line : 2 point)
*! @param  : [in] pt1  : point in line
*! @param  : [in] pt2  : point in line
*! @param  : [in] ptc  : point check
*! @return : TRUE : point in line | FALSE : no
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
BOOL IsPointInLine(const Point2D& pt1, const Point2D& pt2, const Point2D& ptc)
{
	Vec2D vuint_line = NormalizeVector(pt1 - pt2);

	return IsPointInLine2(pt1, vuint_line, ptc);
}

/***********************************************************************************
*! @brief  : Check point in line (line : 1 point + unit vector)
*! @param  : [in] pt   : point in line
*! @param  : [in] vn   : unit vector of line
*! @param  : [in] ptc  : point check
*! @return : TRUE : point in line | FALSE : no
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
BOOL IsPointInLine2(const Point2D& pt, const Vec2D& vn, const Point2D& ptc)
{
	Point2D ptpc = ptc - pt;

	FLOAT fCross = GetCrossProduct(vn, ptpc);

	// 3 collinear point if cross product isqual 0
	if (TRUE == IsEqual(fCross, 0.f, MATH_EPSILON))
	{
		return TRUE;
	}

	return FALSE;
}

/***********************************************************************************
*! @brief  : Find the perpendicular projection of a point onto the line. (line = 2 point)
*! @param  : [in] ptLine1  : point in line
*! @param  : [in] ptLine2  : point in line
*! @param  : [in] pt       : get point
*! @return : Point2D perpendicular projection of a point
*! @author : thuong.nv			- [Date] : 04/07/2023
***********************************************************************************/
Point2D GetPerpPoint2Line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt)
{
	Vec2D ptPer;
	Vec2D vp1p2 = ptLine2 - ptLine1;  // p1p2
	Vec2D vp1p = pt - ptLine1;  // p1p
	Vec2D vp2p = pt - ptLine2;  // p2p

	FLOAT fDis = vp1p2.x * vp1p2.x + vp1p2.y * vp1p2.y;

	if (FALSE == IsEqual(fDis, 0.f, MATH_EPSILON))
	{
		FLOAT fDet = GetDotProduct(vp1p, vp1p2);

		FLOAT t = fDet / fDis;
		ptPer.x = ptLine1.x + t * (ptLine2.x - ptLine1.x);
		ptPer.y = ptLine1.y + t * (ptLine2.y - ptLine1.y);
	}
	else
	{
		ptPer = ptLine1; // case 3 points coincide
	}

	return ptPer;
}

/***********************************************************************************
*! @brief  : Check point is in ray (ray : point start and unit vector)
*! @param  : [in] pt  : start point of ray
*! @param  : [in] vn  : unit vector of ray
*! @param  : [in] ptc : point check
*! @return : TRUE : in ray | FALSE : out ray
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
BOOL IsPointInRay(const Point2D& pt, const Vec2D& vn, const Point2D& ptc)
{
	if (IsPointInLine2(pt, vn, ptc))
	{
		// unit vector ptpc
		Vec2D vptpc = NormalizeVector(ptc - pt);

		return  (vptpc.x * vn.x) >= 0 &&
			(vptpc.y * vn.y) >= 0;
	}

	return FALSE;
}

/***********************************************************************************
@brief		Convert a rect into a list of points
@param		[in]  rect	 : RectF
@return		List pointF
@note		It can be counterclockwise or clockwise
***********************************************************************************/
VecPoint2D ConvertRectF2Points(const Rect2D& rect)
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
FLOAT ConvertRadToDeg(const FLOAT fRad)
{
	return fRad * 180.f / float(PI);
}

/***********************************************************************************
*! @brief  : Convert from Degree to Radian
*! @param  : [in] fDegree : angle float (degree)
*! @return : angle after convert
***********************************************************************************/
FLOAT ConvertDegToRad(const FLOAT fDegree)
{
	return fDegree * float(PI) / 180.f;
}


____END_NAMESPACE____

____END_NAMESPACE____

#endif // XGEOMETRY_H
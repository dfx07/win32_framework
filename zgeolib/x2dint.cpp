#include "x2dint.h"
#include "xgeosp.h"
#include <cassert>
#include "x2drel.h"

namespace geo {	namespace v2 {

/***********************************************************************************
*! @brief  : Find the perpendicular projection of a point onto the line. (line = 2 point)
*! @param  : [in] ptLine1  : point in line
*! @param  : [in] ptLine2  : point in line
*! @param  : [in] pt       : get point
*! @return : Point2D perpendicular projection of a point
*! @author : thuong.nv			- [Date] : 04/07/2023
***********************************************************************************/
API_EXPR Point2D get_projection_point_to_line(const Point2D& ptLine1, const Point2D& ptLine2, const Point2D& pt)
{
	Vec2D ptPer;
	Vec2D vp1p2 = ptLine2 - ptLine1;  // p1p2
	Vec2D vp1p = pt - ptLine1;  // p1p
	Vec2D vp2p = pt - ptLine2;  // p2p

	GFloat fDis = vp1p2.x * vp1p2.x + vp1p2.y * vp1p2.y;

	if (GFalse == is_equal(fDis, 0.f, MATH_EPSILON))
	{
		GFloat fDet = dot(vp1p, vp1p2);

		GFloat t = fDet / fDis;
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
API_EXPR GInt get_projection_point_to_lsegment(const  Point2D& ptSeg1, const Point2D& ptSeg2,
											   const  Point2D& pt,
													  Point2D* pptPerp	    /* = NULL*/,
													  GBool	   bCheckNearest/* = FALSE*/)
{
	GInt iRet = 0;

	Point2D ptPer = get_projection_point_to_line(ptSeg1, ptSeg2, pt);

	// Point in side line Segment
	GFloat fdot1 = dot(ptSeg2 - ptSeg1, ptPer - ptSeg1);
	GFloat fdot2 = dot(ptSeg1 - ptSeg2, ptPer - ptSeg2);

	if (fdot1 >= MATH_EPSILON && fdot2 >= MATH_EPSILON)
	{
		iRet = 1;
	}
	else if (bCheckNearest == GTrue)
	{
		GFloat fDis1 = mag(ptSeg1 - ptPer);
		GFloat fDis2 = mag(ptSeg2 - ptPer);
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
@brief		Get intersection point between line and line (line = 2 point)
@param		[in]  pt1	 : Point in Line 1
@param		[in]  pt2	 : Point in Line 1
@param		[in]  pt3	 : Point in Line 2
@param		[in]  pt4	 : Point in Line 2
@param		[out] pInter : Intersection
@return		TRUE : Intersect | FALSE : not intersect
***********************************************************************************/
API_EXPR GBool intersect_2line(	const Point2D& pt1,		// L1
								const Point2D& pt2,		// L1
								const Point2D& pt3,		// L2
								const Point2D& pt4,		// L2
									  Point2D* pInter	/*= NULL*/)
{
	// Equation of the first straight line Segment  : ax +by = c
	GFloat fa = pt2.y - pt1.y;
	GFloat fb = pt1.x - pt2.x;
	GFloat fc = fa * (pt1.x) + fb * (pt1.y);

	// Equation of the second straight line Segment : a1x +b1y = c1
	GFloat fa1 = pt4.y - pt3.y;
	GFloat fb1 = pt3.x - pt4.x;
	GFloat fc1 = fa1 * (pt3.x) + fb1 * (pt3.y);

	GFloat fDet = fa * fb1 - fb * fa1;

	Vec2D ptIntersect = Point2D(FLT_MAX, FLT_MAX);
	GBool bInter = GFalse;

	// Check not parallel line Segment
	if (is_equal(fDet, 0.f, MATH_EPSILON) == GFalse)
	{
		ptIntersect.x = (fb1 * fc - fb * fc1) / fDet;
		ptIntersect.y = (fa * fc1 - fa1 * fc) / fDet;

		bInter = GTrue;
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
API_EXPR GBool intersect_2lsegment(	const Point2D& pt1,		// L1
									const Point2D& pt2,		// L1
									const Point2D& pt3,		// L2
									const Point2D& pt4,		// L2
										  Point2D* pInter	/*= NULL*/)
{
	Vec2D ptIntersect;

	GBool bInter = intersect_2line(pt1, pt2, pt3, pt4, &ptIntersect);

	if (bInter == GTrue)
	{
		// TODO : improve
		// Inside intersection 
		if (is_point_in_lsegment(pt1, pt2, ptIntersect) &&
			is_point_in_lsegment(pt3, pt4, ptIntersect))
		{
			bInter = GTrue;
		}
		else // Outside intersection
		{
			bInter = GFalse;
		}
	}

	if (pInter && bInter)
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
API_EXPR GBool intersect_line_lsegment( const Point2D& ptLine1,	// Point on the line
										const Point2D& ptLine2,	// Point on the line
										const Point2D& ptSeg1,	// Point start on line segment
										const Point2D& ptSeg2,	// Point end on line segment
											  Point2D* pInter	/*= NULL*/)
{
	Point2D ptInter;

	if (GTrue == intersect_2line(ptLine1, ptLine2, ptSeg1, ptSeg2, &ptInter))
	{
		// TODO : improve
		// Check point inside line segment
		GFloat fp1pDistance  = mag(ptSeg1 - ptInter);
		GFloat fp2pDistance  = mag(ptSeg2 - ptInter);
		GFloat fp1p2Distance = mag(ptSeg1 - ptSeg2);

		if (fp1pDistance <= fp1p2Distance &&
			fp2pDistance <= fp1p2Distance)
		{
			if (pInter)
			{
				*pInter = ptInter;
			}
			return GTrue;
		}
	}
	return GFalse;
}

/***********************************************************************************
@brief		Get intersection points between line and polygon (line = two point)
@param		[in]  ptLine1    : Point on the line
@param		[in]  ptLine2    : Point on the line
@param		[in]  poly       : polygon
@param		[out] vecInter   : point list of intersections
@return		INT : intersection number.
***********************************************************************************/
API_EXPR GInt intersect_line_polygon(const Point2D&		ptLine1,	 //[in] Point on the line
									 const Point2D&		ptLine2,	 //[in] Point on the line
									 const VecPoint2D&	poly,		 //[in] polygon
										   VecPoint2D*	vecInter,  /*= NULL */   //[out] point list of intersections
										   GBool		bOnlyCheck /*= FALSE*/) //[in]  only check if intersect or not
{
	if (poly.size() < 3)
	{
		assert(0);
		return 0;
	}

	int nInter = 0;
	int nPolyCount = static_cast<int>(poly.size());

	Point2D ptInter;
	for (int j = 0, i = nPolyCount - 1; j < nPolyCount; i = j++)
	{
		if (intersect_line_lsegment(ptLine1, ptLine2, poly[i], poly[j], &ptInter) == GTrue)
		{
			nInter++;

			if (vecInter)
			{
				vecInter->push_back(ptInter);
			}

			// Just intersect will return always.
			if (GTrue == bOnlyCheck)
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
API_EXPR GInt intersect_line_rectangle(	const Point2D&		ptLine1,	 //[in] Point on the line
										const Point2D&		ptLine2,	 //[in] Point on the line
										const Rect2D&		rect,		//[in] rectangle
											  VecPoint2D*	vecInter,	/*= NULL */   //[out] point list of intersections
											  GBool			bOnlyCheck	/*= FALSE*/) //[in]  only check if intersect or not
{
	VecPoint2D poly =  geo::convert_rectf_to_points(rect);

	return intersect_line_polygon(ptLine1, ptLine2, poly, vecInter, bOnlyCheck);
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
API_EXPR GInt intersect_lsegment_polygon(const Point2D&		pt1,	//[in] Point on the line
										 const Point2D&		pt2,	//[in] Point on the line
										 const VecPoint2D&	poly,	//[in] polygon
											   VecPoint2D*	vecInter,  /*= NULL*/    //[out] point list of intersections
											   GBool		bOnlyCheck /*= FALSE*/) //[in]  only check if intersect or not
{
	if (poly.size() < 3)
	{
		assert(0);
		return GFalse;
	}

	int nInter = 0;
	int nPolyCount = static_cast<int>(poly.size());

	Point2D ptInter;
	for (int j = 0, i = nPolyCount - 1; j < nPolyCount; i = j++)
	{
		if (intersect_2lsegment(pt1, pt2, poly[i], poly[j], &ptInter) == GTrue)
		{
			nInter++;

			if (vecInter)
			{
				vecInter->push_back(ptInter);
			}

			// Just intersect will return always.
			if (GTrue == bOnlyCheck)
			{
				break;
			}
		}
	}

	return nInter;
}

}}
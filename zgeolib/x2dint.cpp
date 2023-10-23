#include "x2dint.h"
#include "xgeosp.h"


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

}}
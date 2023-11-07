#include "x2dbase.h"
#include "xgeosp.h"
#include <iostream>

namespace geo {namespace v2
{
/***********************************************************************************
@brief		Calculate vector magnitude
@param		[in] v : Vector
@return		FLOAT : magnitude
***********************************************************************************/
Dllexport GFloat mag(const Vec2D& v)
{
	return std::sqrtf(v.x * v.x + v.y * v.y);
}

/***********************************************************************************
*! @brief  : Calculation of unit vector
*! @param  : [in] v : vector
*! @return : uinit vector
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
Dllexport Vec2D normalize(const Vec2D& v)
{
	float fmagnitude = mag(v);

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
Dllexport GFloat cross(const Vec2D& v1, const Vec2D& v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}

/***********************************************************************************
*! @brief  : Calculate dot product between 2 vectors
*! @param  : [in] fangle : angle float
*! @return : FLOAT : dot product
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
Dllexport GFloat dot(const Vec2D& v1, const Vec2D& v2)
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
Dllexport GFloat angle(const Vec2D& v1, const Vec2D& v2)
{
	GFloat fdot = dot(v1, v2);
	GFloat fDet = v1.x * v2.y - v1.y * v2.x;

	GFloat fangle = std::atan2f(fDet, fdot);

	return r2d(fangle);
}

/***********************************************************************************
*! @brief  : move point use unit vector and distance
*! @param  : [in] pt       : move point
*! @param  : [in] vn       : unit vector ( move direction)
*! @param  : [in] fDistance: distance
*! @return : Point2D point after move
*! @author : thuong.nv			- [Date] : 06/03/2023
***********************************************************************************/
Dllexport Point2D move(const Point2D& pt, const Vec2D& vn, const GFloat fDistance)
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
Dllexport Vec2D rotate(const Vec2D& v, const GFloat fDegree)
{
	GFloat fRadangle = d2r(fDegree);

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
Dllexport Point2D rotate(const Point2D& ptPivot, const Point2D& ptRotate, const GFloat fDegree)
{
	Vec2D vpipr = ptRotate - ptPivot;
	GFloat fDistance = mag(vpipr);

	Vec2D vuint_pipr = normalize(vpipr);
	Vec2D vuint_rotate = rotate(vuint_pipr, fDegree);

	Point2D ptmove = move(ptPivot, vuint_rotate, fDistance);

	return ptmove;
}

/***********************************************************************************
*! @brief  : Midpoint between 2 points
*! @param  : [in] pt1 : point 1
*! @param  : [in] pt2 : point 2
*! @return : Point2D mid point
*! @author : thuong.nv			- [Date] : 05/19/2023
***********************************************************************************/
Dllexport Point2D  mid_point(const Point2D& pt1, const Point2D& pt2)
{
	Point2D ptMid;

	ptMid.x = (pt1.x + pt2.x) / 2.f;
	ptMid.y = (pt1.y + pt2.y) / 2.f;

	return ptMid;
}

}}
#include "x2dbase.h"
#include <iostream>

namespace geo {namespace v2
{
/***********************************************************************************
@brief		Calculate vector magnitude
@param		[in] v : Vector
@return		FLOAT : magnitude
***********************************************************************************/
float mag(const Vec2D& v)
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
float cross(const Vec2D& v1, const Vec2D& v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}

/***********************************************************************************
*! @brief  : Calculate dot product between 2 vectors
*! @param  : [in] fangle : angle float
*! @return : FLOAT : dot product
*! @author : thuong.nv				- [Date] : 06/03/2023
***********************************************************************************/
float dot(const Vec2D& v1, const Vec2D& v2)
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
float angle(const Vec2D& v1, const Vec2D& v2)
{
	float fdot = dot(v1, v2);
	float fDet = v1.x * v2.y - v1.y * v2.x;

	float fangle = std::atan2f(fDet, fdot);

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
inline Point2D move(const Point2D& pt, const Vec2D& vn, const float fDistance)
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
inline Vec2D rotate(const Vec2D& v, const float fDegree)
{
	float fRadangle = d2r(fDegree);

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
inline Point2D	 rotate(const Point2D& ptPivot, const Point2D& ptRotate, const float fDegree)
{
	Vec2D vpipr = ptRotate - ptPivot;
	float fDistance = mag(vpipr);

	Vec2D vuint_pipr = normalize(vpipr);
	Vec2D vuint_rotate = rotate(vuint_pipr, fDegree);

	Point2D ptmove = move(ptPivot, vuint_rotate, fDistance);

	return ptmove;
}

/***********************************************************************************
*! @brief  : Convert from Radian to Degree
*! @param  : [in] fRad : angle float (radian)
*! @return : angle after convert
***********************************************************************************/
float r2d(const float fRad)
{
	return fRad * 180.f / float(PI);
}

/***********************************************************************************
*! @brief  : Convert from Degree to Radian
*! @param  : [in] fDegree : angle float (degree)
*! @return : angle after convert
***********************************************************************************/
float d2r(const float fDegree)
{
	return fDegree * float(PI) / 180.f;
}

}}
#include "xgeosp.h"
#include <iostream>

namespace geo {

/*******************************************************************************
@brief		Check for two equal values using tolerance
@param		[in] fVal1		: First value
@param		[in] fVal1		: Second value
@param		[in] fEpsilon	: tolerance
@return		TRUE : equal | FALSE : no equal
*******************************************************************************/
Dllexport GBool is_equal(const GFloat fVal1, const GFloat fVal2, const GFloat fEpsilon)
{
	GFloat fDiff = fVal2 - fVal1;
	return (std::fabs(fDiff) <= fEpsilon);
}

/*******************************************************************************
@brief		Convert a rect into a list of points
@param		[in]  rect	 : RectF
@return		List pointF
@note		It can be counterclockwise or clockwise
********************************************************************************/
Dllexport VecPoint2D convert_rectf_to_points(const Rect2D& rect)
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
Dllexport GFloat r2d(const GFloat& fRad)
{
	return fRad * 180.f / GFloat(PI);
}

/***********************************************************************************
*! @brief  : Convert from Degree to Radian
*! @param  : [in] fDegree : angle float (degree)
*! @return : angle after convert
***********************************************************************************/
Dllexport GFloat d2r(const GFloat& fDegree)
{
	return fDegree * GFloat(PI) / 180.f;
}
}


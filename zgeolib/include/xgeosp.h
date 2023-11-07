////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgeosp.h
* @create   Oct 18, 2023
* @brief    Geometry2D base function
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGEOSP_H
#define xGEOSP_H

#include "x2dtype.h"

namespace geo {
	/*******************************************************************************
	@brief		Check for two equal values using tolerance
	@param		[in] fVal1		: First value
	@param		[in] fVal1		: Second value
	@param		[in] fEpsilon	: tolerance
	@return		TRUE : equal | FALSE : no equal
	*******************************************************************************/
	Dllexport GBool is_equal(const GFloat fVal1, const GFloat fVal2, const GFloat fEpsilon);

	/*******************************************************************************
	@brief		Convert a rect into a list of points
	@param		[in]  rect	 : RectF
	@return		List pointF
	@note		It can be counterclockwise or clockwise
	********************************************************************************/
	Dllexport VecPoint2D	 convert_rectf_to_points(const Rect2D& rect);

	/*******************************************************************************
	*! @brief  : Convert from Radian to Degree
	*! @param  : [in] fRad : angle float (radian)
	*! @return : angle after convert
	********************************************************************************/
	Dllexport GFloat r2d(const GFloat& fRad);

	/*******************************************************************************
	*! @brief  : Convert from Degree to Radian
	*! @param  : [in] fDegree : angle float (degree)
	*! @return : angle after convert
	********************************************************************************/
	Dllexport GFloat d2r(const GFloat& fDegree);

}

#endif //!XGEOSP_H
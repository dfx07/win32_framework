////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xmath.h
* @create   Nov 15, 2023
* @brief    Math
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XMATH_H
#define XMATH_H

#include "xsysdef.h"

///////////////////////////////////////////////////////////////////////////////////
// Define math
#undef PI
	#define PI					 3.14159265359	/* PI	  */
#undef HPI
	#define HPI					 1.57079632679	/* Haft PI*/
#undef MATH_EPSILON
	#define MATH_EPSILON		 0.001f			/* epsilon*/

___BEGIN_NAMESPACE___

struct tagVec2D
{
	float x;
	float y;
};

struct tagVec3D
{
	float x;
	float y;
	float z;
};

struct tagRect2D
{
	float x;
	float y;
	float width;
	float height;
};

____END_NAMESPACE____

#endif // XGEOMETRY_H
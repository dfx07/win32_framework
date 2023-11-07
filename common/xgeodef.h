////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgeodef.h
* @create   Nov 15, 2023
* @brief    Library define system on window os
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGEODEF_H
#define XGEODEF_H

#include "xsysdef.h"

#ifndef PI
#define PI	3.14159265359	/* PI	  */
#endif

#ifndef HPI
#define HPI	1.57079632679	/* Haft PI*/
#endif

#ifndef MATH_EPSILON
#define MATH_EPSILON	0.001f	/* epsilon*/
#endif

#define GTrue  true
#define GFalse false

typedef float	GFloat;
typedef double	GDouble;
typedef int		GInt;
typedef bool	GBool;

/////////////////////////////////////////////////////////////////////////////////////
// User define type

template<int dimention, typename T> struct tagVec;


#endif // !XGEODEF_H

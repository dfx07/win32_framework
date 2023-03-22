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
#ifndef XALGORITHM_H
#define XALGORITHM_H

#include "xsysdef.h"

#include <iostream>
#include <math.h>
#include <algorithm>


#define PI					 3.14159265359	/* PI	  */
#define HPI					 1.57079632679	/* Haft PI*/

___BEGIN_NAMESPACE___

//==================================================================================
//⮟⮟ Section : function math
//==================================================================================
____BEGIN_SECTION____(math)

/***********************************************************************************
*! @brief  : clamp value from min to max
*! @param    [In] x : in range [min - max]
*! @param    [In] min
*! @param    [In] max 
*! @return : value clamp
*! @author : thuong.nv       - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double clamp(IN double x, IN double min, IN double max)
{
	return (x <= min) ? min : (x >= max ? max : x);
}

/***********************************************************************************
*! @brief  : map value in range [min_s - max_s] to [min_d - max_d]
*! @param    [In] x : value in range [min_s - max_s]
*! @param    [In] min_s, max_s
*! @param    [In] min_d, max_d
*! @return : value map in [min_d - max_d]
*! @author : thuong.nv       - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double hard_map(IN double x, IN double min_s, IN double max_s,
									IN double min_d, IN double max_d, IN double epsilon)
{
	if (fabs(max_s - min_s) <= epsilon) return min_d;

	auto value = ((x - min_s)*(max_d - min_d)) / (max_s - min_s) + min_d;
	return clamp(value, min_d, max_d);
}

/**********************************************************************************
*! @brief  : map value in range [min_s - max_s] to [min_d - max_d]
*! @param    [In] x : value in range [min_s - max_s]
*! @param    [In] min_s, max_s
*! @param    [In] min_d, max_d
*! @return : value map in [min_d - max_d] able out of range
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double soft_map(IN double x, IN double min_s, IN double max_s,
									IN double min_d, IN double max_d, IN double epsilon)
{
	auto A = (max_s - x);
	auto B = (min_s - x);

	if (fabs(A) <= epsilon) return max_d;
	else if (fabs(B) <= epsilon) return min_d;

	if (fabs(A - B) <= epsilon)
	{
		return min_d;
	}

	auto value = (A*min_d - B*max_d) / (A - B);
	return value;
}

_____END_SECTION_____

//==================================================================================
//⮟⮟ Section : function easing
//==================================================================================
____BEGIN_SECTION____(easing)

/***********************************************************************************
*! @brief  : EASING BACK
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double back_in(IN double t)
{
    const double c1 = 1.70158;
    const double c3 = c1 + 1;

    return c3*t*t*t - c1*t*t;
}

static double back_out(IN double t)
{
    const double c1 = 1.70158;
    const double c3 = c1 + 1;

    return 1 + c3 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
}

static double back_inout(IN double t)
{
    const double c1 = 1.70158;
    const double c2 = c1 * 1.525;

    return t < 0.5
        ? (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
        : (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}

/***********************************************************************************
*! @brief  : EASING Quint
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double quint_in(IN double t)
{
    return t * t * t * t * t;
}

static double quint_out(IN double t)
{
    return 1 - std::pow(1 - t, 5);
}

static double quint_inout(IN double t)
{
    return t < 0.5 ? 16 * t * t * t * t * t : 1 - std::pow(-2 * t + 2, 5) / 2;
}

/***********************************************************************************
*! @brief  : EASING Elastic
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double elastic_in(IN double t)
{
    const double c4 = (2 * PI) / 3.;

    return t == 0
        ? 0
        : t == 1
        ? 1
        : -std::pow(2, 10 * t - 10) * sin((t * 10 - 10.75) * c4);
}

static double elastic_out(IN double t)
{
    const double c4 = (2 * PI) / 3.;

    return t == 0
        ? 0
        : t == 1
        ? 1
        : std::pow(2, -10 * t) * sin((t * 10 - 0.75f) * c4) + 1;
}

static double elastic_inout(IN double t)
{
    const double c5 = (2 * PI) / 4.5;

    return t == 0
        ? 0
        : t == 1
        ? 1
        : t < 0.5
        ? -(std::pow(2,  20 * t - 10) * sin((20 * t - 11.125f) * c5)) / 2
        :  (std::pow(2, -20 * t + 10) * sin((20 * t - 11.125f) * c5)) / 2 + 1;
}

/***********************************************************************************
*! @brief  : EASING Quart
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double quart_in(IN double t)
{
    return t * t * t * t;
}

static double quart_out(IN double t)
{
    return 1 - std::pow(1 - t, 4);
}

static double quart_inout(IN double t)
{
    return t < 0.5 ? 8 * t * t * t * t : 1 - std::pow(-2 * t + 2, 4) / 2;
}

/***********************************************************************************
*! @brief  : EASING Bounce 
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double bounce_out(IN double t)
{
    const double  n1 = 7.5625;
    const double  d1 = 2.75;

    if (t < 1 / d1) {
        return n1 * t * t;
    } else if (t < 2.0 / d1) {
        return n1 * (t -= 1.5 / d1) * t + 0.75;
    } else if (t < 2.5 / d1) {
        return n1 * (t -= 2.25 / d1) * t + 0.9375;
    } else {
        return n1 * (t -= 2.625 / d1) * t + 0.984375;
    }
}

static double bounce_in(IN double t)
{
    return 1 - bounce_out(1 - t);
}

static double bounce_inout(IN double t)
{
    return t < 0.5
        ? (1 - bounce_out(1 - 2 * t)) / 2
        : (1 + bounce_out(2 * t - 1)) / 2;
}

/***********************************************************************************
*! @brief  : EASING Expo
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double expo_in(IN double t)
{
    return t == 0 ? 0 : std::pow(2, 10 * t - 10);
}

static double expo_out(IN double t)
{
    return t == 1 ? 1 : 1 - std::pow(2, -10 * t);
}

static double expo_inout(IN double t)
{
    return t == 0
        ? 0
        : t == 1
        ? 1
        : t < 0.5 ? std::pow(2, 20 * t - 10) / 2
        : (2 - std::pow(2, -20 * t + 10)) / 2;
}

/***********************************************************************************
*! @brief  : EASING Circ
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double circ_in(IN double t)
{
    return 1. - std::sqrt(1 - std::pow(t, 2));
}

static double circ_out(IN double t)
{
    return sqrt(1 - std::pow(t - 1, 2));
}

static double circ_inout(IN double t)
{
    return t < 0.5
        ? (1 - std::sqrt(1 - std::pow(2 * t, 2))) / 2
        : (std::sqrt(1. - std::pow(-2 * t + 2., 2)) + 1.) / 2;
}

/***********************************************************************************
*! @brief  : EASING Quad
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double quad_in(IN double t)
{
    return t * t;
}

static double quad_out(IN double t)
{
    return 1 - (1 - t) * (1 - t);
}

static double quad_inout(IN double t)
{
    return t < 0.5 ? 2 * t * t : 1. - std::pow(-2 * t + 2., 2) / 2.;
}

/***********************************************************************************
*! @brief  : EASING Sine
*! @param    [In] t : value in range [0 - 1]
*! @return : double value in range [0, 1]
*! @author : thuong.nv      - [Date] : 26/10/2022
*! @note   : N/A
***********************************************************************************/
static double sine_in(IN double t)
{
    return 1 - std::cos((t * PI) / 2);
}

static double sine_out(IN double t)
{
    return std::sin((t * PI) / 2);
}

static double sine_inout(IN double t)
{
    return -(std::cos(PI * t) - 1) / 2;
}
_____END_SECTION_____

____END_NAMESPACE____

#endif // XALGORITHM_H
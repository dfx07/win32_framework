////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xsysdef.h
* @create   Nov 15, 2022
* @brief    Library define system on window os
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XSYSDEF_H
#define XSYSDEF_H

#undef  IN
#define IN  /*INPUT */

#undef  OUT
#define OUT /*OUTPUT*/

//#undef  interface
//#define interface struct /*interface*/
#define NAMESPACE fox

#define ___BEGIN_NAMESPACE___		 namespace NAMESPACE{
#define ____END_NAMESPACE____							}

#define ____BEGIN_SECTION____(name)	 namespace name		{
#define _____END_SECTION_____							}

#define	____USE_NAMESPACE____		 using namespace NAMESPACE;
#define	_____USE_SECTION_____(name)	 using namespace NAMESPACE::##name;


#ifdef _EXPORT_
	#define Dllexport  __declspec( dllexport )
#else
	#define Dllexport
#endif

#define S2MS(s)						(s  * 1000)  /*Convert second -> millisecond*/
#define MS2S(ms)					(ms / 1000)  /*Convert millisecond -> second*/

#define SAFE_DELETE(p)				{delete   p; p = NULL;}
#define SAFE_DELETE_ARRAY(p)		{delete[] p; p = NULL;}
#define NULL_RETURN(p, r)			if(p == NULL) return r;
#define NULL_BREAK(p)				if(p == NULL) break;
#define NULL_CONTINUE(p)			if(p == NULL) continue;
#define CHECK_AND_EXE(ck, exe)		(ck == true)? exe : false;

#define CHECK_EXE_UPDATE(ck, exe)	if(ck == true){ ck = exe; };

#define MAX_DEVICENAME			32

/// ////////////////////////////////////////////////////////////////////////////////
/// Define struct system define
/// ////////////////////////////////////////////////////////////////////////////////
typedef struct tagMonitorInfo
{
	unsigned long	WIDTH	= 0;
	unsigned long	HEIGHT	= 0;
	unsigned long	DISFREQ	= 0;
	unsigned long	VERSION	= 0;
	wchar_t			NAME[MAX_DEVICENAME]{0};

} xMonitorInfo;


#endif // XSYSDEF_H

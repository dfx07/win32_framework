#pragma once

#ifdef DLL_EXPORT
	#define API_EXPR  __declspec( dllexport )
#else
	#define API_EXPR
#endif

#ifndef PI
	#define PI					 3.14159265359	/* PI	  */
#endif

#ifndef HPI
	#define HPI					 1.57079632679	/* Haft PI*/
#endif

#ifndef MATH_EPSILON
	#define MATH_EPSILON		 0.001f			/* epsilon*/
#endif
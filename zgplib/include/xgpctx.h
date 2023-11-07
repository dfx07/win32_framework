////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgpctx.h
* @create   Nov 07, 2023
* @brief    Define the basic classes that the control needs to inherit
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGPCTX_H
#define XGPCTX_H

#include "xsysdef.h"


____BEGIN_SECTION____(gp)

/**********************************************************************************
* ⮟⮟ Class name: IContext
* Base class for Device Context use draw gpu inheritance
***********************************************************************************/
class Dllexport IContext
{
public:
	virtual void* Render() = 0;
	virtual bool  CreateContext(int iAntialiasing = 0) = 0;
	virtual void  DeleteContext() = 0;
	virtual bool  MakeContext() = 0;
	virtual void  SwapBuffer() = 0;
};

____END_NAMESPACE____

#endif // !XGPCTX_H
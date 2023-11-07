////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgpwinctx.h
* @create   Nov 07, 2023
* @brief    Define the basic classes window context
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGPWINCTX_H
#define XGPWINCTX_H

#include "xgpctx.h"
#include <assert.h>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#define ADD_ATRIBUTE(attribs, name, value)\
{\
    assert((size_t) attribCount < attribsize);\
    attribs[attribCount++] = name;  \
    attribs[attribCount++] = value; \
}

#define END_ATRIBUTE(attribs)\
{\
    assert((size_t) attribCount < attribsize); \
    attribs[attribCount++] = 0;  \
}

____BEGIN_SECTION____(gp)


typedef struct WindowRender
{
	HWND	m_hWnd  = NULL;
	HDC		m_hDc   = NULL;
	HGLRC	m_hGLRC = NULL;

	bool	m_bCreated = false;	// create ok

} *WindowRenderPtr;

/**********************************************************************************
* ⮟⮟ Class name: WindowBase
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport WindowOpenGLContext : public IContext
{
protected:
	bool				m_bUseOpenGLEx;
	WindowRender		m_pRender;

public:
	WindowOpenGLContext();

	/*******************************************************************************
	*! @brief  : Get keyboard state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	bool InitContext(HWND hwnd, bool bUse);

protected:
	/*******************************************************************************
	*! @brief  : register window class use window context
	*! @param  : [In] strClassName : Name class
	*! @param  : [In] Proc		   : function hanle event
	*! @param  : [In] hInst		   : default GetModuleHandle(NULL)
	*! @return : 1 : OK , 0 : False
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	static int register_window_class(const wchar_t* strClassName, WNDPROC Proc, HINSTANCE hInst);

protected:
	/*******************************************************************************
	*! @brief  : Dummy handler function to create opengl
	*! @param  : [In] hwnd		: Name class
	*! @param  : [In] message	: default
	*! @param  : [In] wParam	: default
	*! @param  : [In] lParam	: default
	*! @return : 1 : OK , 0 : False
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	static LRESULT CALLBACK win_dummy_main_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/*******************************************************************************
	*! @brief  : load extension opengl library glew
	*! @param  : [In] strClassName : Name class
	*! @param  : [In] Proc		   : function hanle event
	*! @param  : [In] hInst		   : default GetModuleHandle(NULL)
	*! @return : 1: OK | 0 False;
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	static int load_opengl_extension();

	/*******************************************************************************
	*! @brief  : init and create OpenGL context
	*! @param  : [In] void
	*! @return : true : OK | false : False
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	********************************************************************************/
	bool make_opengl_context(int iAntialiasing = 0);

	/*******************************************************************************
	*! @brief  : check status opengl use window render
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	********************************************************************************/
	bool is_opengl_ok();

	/*******************************************************************************
	*! @brief  : initialize initial state for context
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	********************************************************************************/
	bool make_current_context();

	/*******************************************************************************
	*! @brief  : Remove the previously set context
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	********************************************************************************/
	void delete_opengl_context();

protected:

	/*******************************************************************************
	*! @brief  : Get render device
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	virtual void* Render();

	/*******************************************************************************
	*! @brief  : Create opengl context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	virtual bool CreateContext(int iAntialiasing = 0);

	/*******************************************************************************
	*! @brief  : Delete OpenGL context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	virtual void DeleteContext();

	/*******************************************************************************
	*! @brief  : Active OpenGL context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	virtual bool MakeContext();

	/*******************************************************************************
	*! @brief  : Swap buffer
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	********************************************************************************/
	virtual void SwapBuffer();
};

____END_NAMESPACE____

#endif // !XGPWINCTX_H
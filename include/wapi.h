////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     whanleapi.h
* @create   Nov 15, 2022
* @brief    Create and handle event control the window
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#include "window/xwindow.h"

___BEGIN_NAMESPACE___

//======================================================================================
//⮟⮟ API Class name: whandle window                                                    
//Control for window                                                                    
//======================================================================================

/***************************************************************************
*! @brief  : initialize the environment before initializing the window
*! @return : 0: Ok | 1: Register window failed | 2: create OpenGL failed
*! @author : thuong.nv          - [Date] : 05/03/2023
***************************************************************************/
int Dllexport init_window()
{
	if (!Window::register_class())
	{
		bool bOk = Window::register_window_class(GL_WIN_CLASS, Window::WndMainProc, GetModuleHandle(NULL));
		//bOk &= Window::register_window_class(GL_SUBWIN_CLASS, SubWindow::WndSubProc, GetModuleHandle(NULL));
		Window::register_class(true, bOk);
	}
	
	if (!Window::register_class())
	{
		_ASSERT(L"Register window class failed !");
		return 1;
	}
	
	if (!Window::opengl_extension())
	{
		bool bOk = Window::load_opengl_extension();
		Window::opengl_extension(true, bOk);
	}

	if (!Window::register_class())
	{
		_ASSERT(L"Created OpenGL window failed !");
		return 2;
	}

	return 0;
}

/***************************************************************************
*! @brief  : create window
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
***************************************************************************/
Window* Dllexport create_window(const wchar_t*	title,
								const int		xpos,
								const int		ypos,
								const int		width  = 640,
								const int		height = 480,
								const WindowSetting* setting = NULL)
{
	if (init_window() != 0)
		return NULL;

	Window* win = new Window(title, xpos, ypos, width, height, setting);
	if(!win->Create(GL_WIN_CLASS))
	{
		delete win;
		return NULL;
	}
	
	// create opengl context 
	if (!win->CreateOpenGLContext())
	{
		win->close();
		delete win;
		return NULL;
	}

	win->Show();

	return win;
}

Window* Dllexport create_window(Window* win)
{
	if (init_window() != 0)
		return NULL;

	if (!win->Create(GL_WIN_CLASS))
	{
		delete win;
		return NULL;
	}

	// create opengl context 
	if (!win->CreateOpenGLContext())
	{
		win->close();
		delete win;
		return NULL;
	}

	win->Show();

	return win;
}

void Dllexport destroy_window(Window* win)
{
	delete win;
}

____END_NAMESPACE____

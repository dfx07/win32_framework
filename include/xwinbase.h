////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xwinbase.h
* @create   Nov 15, 2022
* @brief    Create and handle event base window handle
* @note     For conditions of distribution and use, see copyright notice in readme.txt
***********************************************************************************/
#ifndef XWINBASE_H
#define XWINBASE_H

#include "controls/CGDIPlus/xwcontrol.h"

#include <string>
#include <map>
#include <vector>
#include <stack>

#include <GL/glew.h>
#include <GL/wglew.h>

___BEGIN_NAMESPACE___

/**********************************************************************************
* ⮟⮟ Class name: WindowSetting
* Base class for inherited window controls
***********************************************************************************/
struct WindowSetting
{
	bool				m_bFullScreen;
	bool				m_bGDIplus;
	bool				m_bOpenGL;       // State OpenGL
	int					m_iAntialiasing; // Antialiasing level = 8
	int					m_iModeDraw;	 // 1 : use thread draw opengl | 0 :use pipe main thread
	bool				m_bWriteInfo;
	int					m_iAphaTrans = -1;

	void set_default()
	{
		m_bFullScreen   = false;
		m_bGDIplus	    = false;
		m_bOpenGL	    = false;
		m_iAntialiasing = -1;
		m_iModeDraw		= 0;
		m_bWriteInfo	= false;
	}

	WindowSetting()
	{
		this->set_default();
	}

	WindowSetting(	const char* title,
					const int	xpos,
					const int	ypos, 
					const int	width = 640,
					const int	height = 480)
	{
		m_bFullScreen	= false;
		m_bGDIplus		= false;
		m_bOpenGL		= true;
		m_iAntialiasing = -1;
		m_iModeDraw		= 0;
		m_bWriteInfo	= false;
	}
};

/**********************************************************************************
* ⮟⮟ Class name: WindowEvent
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport WindowEvent
{
protected:
	typedef void(*typeFunOnDraw)         (WindowBase* win);
	typedef void(*typeFunOnCreated)      (WindowBase* win);
	typedef void(*typeFunOnDestroy)      (WindowBase* win);
	typedef void(*typeFunOnPaint)        (WindowBase* win);
	typedef void(*typeFunOnMouse)        (WindowBase* win, int button, int action);
	typedef void(*typeFunOnMouseRealt)   (WindowBase* win);
	typedef void(*typeFunOnMouseMove)    (WindowBase* win);
	typedef void(*typeFunOnKeyboard)     (WindowBase* win);
	typedef void(*typeFunOnProcess)      (WindowBase* win);
	typedef void(*typeFunOnResize)       (WindowBase* win);
	typedef void(*typeFunOnMouseScroll)  (WindowBase* win);

protected:
	typeFunOnDraw				m_funOnDraw        = NULL;
	typeFunOnCreated			m_funOnCreated     = NULL;
	typeFunOnDestroy			m_funOnDestroy     = NULL;
	typeFunOnPaint				m_funOnPaint       = NULL;
	typeFunOnMouse				m_funOnMouse       = NULL;
	typeFunOnMouseRealt			m_funOnMouseRealt  = NULL;
	typeFunOnMouseMove			m_funOnMouseMove   = NULL;
	typeFunOnKeyboard			m_funOnKeyboard    = NULL;
	typeFunOnProcess			m_funOnProcess     = NULL;
	typeFunOnResize				m_funOnResize      = NULL;
	typeFunOnMouseScroll		m_funOnMouseScroll = NULL;

public:
	void SetOnDrawfunc			  (typeFunOnDraw			funOnDraw)		{ m_funOnDraw		 = funOnDraw;	  }
	void SetOnCreatedfunc		  (typeFunOnCreated			funOnCreate)	{ m_funOnCreated	 = funOnCreate;   }
	void SetOnDestroyfunc		  (typeFunOnDestroy			funOnDestroy)	{ m_funOnDestroy	 = funOnDestroy;  }
	void SetOnPaintfunc			  (typeFunOnPaint			funOnPaint)		{ m_funOnPaint		 = funOnPaint;	  }
	void SetOnMouseButtonfunc	  (typeFunOnMouse			funOnMouse)		{ m_funOnMouse		 = funOnMouse;	  }
	void SetOnMouseButtonRealtfunc(typeFunOnMouseRealt		funOnMouse)		{ m_funOnMouseRealt  = funOnMouse;	  }
	void SetOnMouseMovefunc		  (typeFunOnMouseMove		funOnMouseMove)	{ m_funOnMouseMove	 = funOnMouseMove;}
	void SetOnKeyboardfunc		  (typeFunOnKeyboard		funOnKeyboard)	{ m_funOnKeyboard	 = funOnKeyboard; }
	void SetProcessfunc			  (typeFunOnProcess			funProcess)		{ m_funOnProcess	 = funProcess;	  }
	void SetOnResizefunc		  (typeFunOnResize			funOnResize)	{ m_funOnResize		 = funOnResize;	  }
	void SetOnMouseScrollfunc	  (typeFunOnMouseScroll		funOnScroll)	{ m_funOnMouseScroll = funOnScroll;   }

protected:
	virtual void OnCreated(WindowBase* base)							{ ON_FUNCTION_WINDOW(m_funOnCreated		, base)}
	virtual void OnCommand(WindowBase* base, int type)					{ }
	virtual void OnKeyBoard(WindowBase* base)							{ ON_FUNCTION_WINDOW(m_funOnKeyboard	, base)}
	virtual void OnMouseButton(WindowBase* base, int button, int action){ ON_FUNCTION_WINDOW(m_funOnMouse		, base, button, action)}
	virtual void OnMouseMove(WindowBase* base)							{ ON_FUNCTION_WINDOW(m_funOnMouseMove	, base)}
	virtual void OnProcess(WindowBase* base)							{ ON_FUNCTION_WINDOW(m_funOnProcess		, base)}
	virtual void OnMouseRealtime(WindowBase* base)						{ ON_FUNCTION_WINDOW(m_funOnMouseRealt	, base)}
	virtual void OnResize(WindowBase* base)								{ ON_FUNCTION_WINDOW(m_funOnResize		, base)}
	virtual void OnMouseScroll(WindowBase* base)						{ ON_FUNCTION_WINDOW(m_funOnMouseScroll	, base)}
	virtual void OnPaint(WindowBase* base)								{ ON_FUNCTION_WINDOW(m_funOnPaint		, base)}
};

/**********************************************************************************
* ⮟⮟ Class name: WindowBase
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport WindowOpenGLContext : public WindowContext
{
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"../build/lib/glew32.lib")

protected:
	bool				m_bUseOpenGLEx;

	WindowRender		m_pRender;

public:
	WindowOpenGLContext() : m_bUseOpenGLEx(false)
	{

	}

	/***************************************************************************
	*! @brief  : Get keyboard state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool InitContext(HWND hwnd, bool bUse)
	{
		m_pRender.m_hWnd = hwnd;
		m_bUseOpenGLEx = bUse;

		return true;
	}

protected:
	/***************************************************************************
	*! @brief  : register window class use window context
	*! @param  : [In] strClassName : Name class
	*! @param  : [In] Proc		   : function hanle event
	*! @param  : [In] hInst		   : default GetModuleHandle(NULL)
	*! @return : 1 : OK , 0 : False
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	static int register_window_class(const wchar_t* strClassName, WNDPROC Proc, HINSTANCE hInst)
	{
		WNDCLASSEXW  wClass;
		ZeroMemory(&wClass, sizeof(WNDCLASSEX));
		wClass.cbClsExtra		= NULL;
		wClass.cbSize			= sizeof(WNDCLASSEX);
		wClass.cbWndExtra		= NULL;
		wClass.hbrBackground	= (HBRUSH)(COLOR_MENU);
		wClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wClass.hIconSm			= NULL;
		wClass.hInstance		= hInst;
		wClass.lpfnWndProc		= (WNDPROC)Proc;
		wClass.lpszClassName	= strClassName;
		wClass.lpszMenuName		= NULL;
		wClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		if (!RegisterClassEx(&wClass))
			return 0;
		return 1;
	}

protected:
	/***************************************************************************
	*! @brief  : Dummy handler function to create opengl
	*! @param  : [In] hwnd		: Name class
	*! @param  : [In] message	: default
	*! @param  : [In] wParam	: default
	*! @param  : [In] lParam	: default
	*! @return : 1 : OK , 0 : False
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	static LRESULT CALLBACK win_dummy_main_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	/***************************************************************************
	*! @brief  : load extension opengl library glew
	*! @param  : [In] strClassName : Name class
	*! @param  : [In] Proc		   : function hanle event
	*! @param  : [In] hInst		   : default GetModuleHandle(NULL)
	*! @return : 1: OK | 0 False;
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	static int load_opengl_extension()
	{
		int bInitOk = FALSE;
		if (register_window_class(L"DummyClass", win_dummy_main_proc, GetModuleHandle(NULL)))
		{
			HWND hWnd_dummy = CreateWindowEx(
				0, L"DummyClass", L"Dummy OpenGL Window", 0,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				0, 0, NULL, 0);

			PIXELFORMATDESCRIPTOR pixelFormat = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
				32,                   // Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,                   // Number of bits for the depthbuffer
				8,                    // Number of bits for the stencilbuffer
				0,                    // Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};

			HDC hDC_dummy = GetDC(hWnd_dummy);
			int iPixelFormat = ChoosePixelFormat(hDC_dummy, &pixelFormat);
			SetPixelFormat(hDC_dummy, iPixelFormat, &pixelFormat);

			HGLRC hglrc_dummy = wglCreateContext(hDC_dummy);
			if (wglMakeCurrent(hDC_dummy, hglrc_dummy)) {

				if (glewInit() != GLEW_OK)  // Load library OpenGL extension
					bInitOk = FALSE;
				else
					bInitOk = TRUE;
			}

			wglMakeCurrent(hDC_dummy, 0);
			wglDeleteContext(hglrc_dummy);
			ReleaseDC(hWnd_dummy, hDC_dummy);
			DestroyWindow(hWnd_dummy);
		}

		return bInitOk;
	}

	/***************************************************************************
	*! @brief  : init and create OpenGL context
	*! @param  : [In] void
	*! @return : true : OK | false : False
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	bool make_opengl_context(int iAntialiasing = 0)
	{
		m_pRender.m_bCreated = false;

		NULL_RETURN(m_pRender.m_hWnd, false);

		HDC hDC = GetDC(m_pRender.m_hWnd);
		HGLRC hglrc = NULL;

		int iPixelFormat; unsigned int num_formats = 0;

		// Lấy mảng thuộc tính thiết lập để set style window 
		auto funGetFixelFormatAttribute = [&](int* attribs, int attribsize)
		{
			int attribCount = 0;

			ADD_ATRIBUTE(attribs, WGL_DRAW_TO_WINDOW_ARB, GL_TRUE);
			ADD_ATRIBUTE(attribs, WGL_SUPPORT_OPENGL_ARB, GL_TRUE);
			ADD_ATRIBUTE(attribs, WGL_DOUBLE_BUFFER_ARB, GL_TRUE);
			ADD_ATRIBUTE(attribs, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB);
			ADD_ATRIBUTE(attribs, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB);
			ADD_ATRIBUTE(attribs, WGL_COLOR_BITS_ARB, 32);
			ADD_ATRIBUTE(attribs, WGL_DEPTH_BITS_ARB, 24);
			ADD_ATRIBUTE(attribs, WGL_STENCIL_BITS_ARB, 8);

			if (iAntialiasing > 0)
			{
				ADD_ATRIBUTE(attribs, WGL_SAMPLE_BUFFERS_ARB, GL_TRUE); // Enable multisampling
				ADD_ATRIBUTE(attribs, WGL_SAMPLES_ARB, iAntialiasing);  // Number of samples
			}

			END_ATRIBUTE(attribs);
		};

		// Get pixel format attributes through "modern" extension
		if (m_bUseOpenGLEx)
		{
			int pixelAttribs[47];
			funGetFixelFormatAttribute(pixelAttribs, sizeof(pixelAttribs) / sizeof(pixelAttribs[0]));

			wglChoosePixelFormatARB(hDC, pixelAttribs, 0, 1, &iPixelFormat, &num_formats);

			PIXELFORMATDESCRIPTOR pfd;
			DescribePixelFormat(hDC, iPixelFormat, sizeof(pfd), &pfd);
			SetPixelFormat(hDC, iPixelFormat, &pfd);

			// Specify that we want to create an OpenGL x.x core profile context
			int gl_attribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
				WGL_CONTEXT_MINOR_VERSION_ARB, 5,
				WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0,
			};
			hglrc = wglCreateContextAttribsARB(hDC, 0, gl_attribs);
		}
		// Get pixel format attributes through legacy PFDs
		else
		{
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
				32,                   // Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,                   // Number of bits for the depthbuffer
				8,                    // Number of bits for the stencilbuffer
				0,                    // Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};
			iPixelFormat = ChoosePixelFormat(hDC, &pfd);
			SetPixelFormat(hDC, iPixelFormat, &pfd);

			hglrc = wglCreateContext(hDC);
		}

		m_pRender.m_hDc = hDC;
		m_pRender.m_hGLRC = hglrc;

		if (!make_current_context())
		{
			delete_opengl_context();
			return false;
		}

		return true;
	}

	/***************************************************************************
	*! @brief  : check status opengl use window render
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	bool is_opengl_ok()
	{
		return (m_pRender.m_hGLRC && m_pRender.m_bCreated);
	}

	/***************************************************************************
	*! @brief  : initialize initial state for context
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	bool make_current_context()
	{
		if (wglMakeCurrent(m_pRender.m_hDc, m_pRender.m_hGLRC))
		{
			m_pRender.m_bCreated = true;
		}
		else
		{
			m_pRender.m_bCreated = false;
		}
		return m_pRender.m_bCreated;
	}

	/***************************************************************************
	*! @brief  : Remove the previously set context
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	void delete_opengl_context()
	{
		ReleaseDC(m_pRender.m_hWnd, m_pRender.m_hDc);	// release device context
		wglDeleteContext(m_pRender.m_hGLRC);			// delete the rendering context
		m_pRender.m_bCreated = false;
		m_pRender.m_hDc	 = NULL;
		m_pRender.m_hGLRC = NULL;
	}

protected:

	/***************************************************************************
	*! @brief  : Get render device
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void*  Render() { return &m_pRender.m_hDc; }

	/***************************************************************************
	*! @brief  : Create opengl context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool CreateContext(int iAntialiasing = 0)
	{
		if (make_opengl_context(iAntialiasing))
		{
			return m_pRender.m_bCreated;
		}
		return false;
	}

	/***************************************************************************
	*! @brief  : Delete OpenGL context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void DeleteContext()
	{
		delete_opengl_context();
	}

	/***************************************************************************
	*! @brief  : Active OpenGL context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool MakeContext()
	{
		if (is_opengl_ok())
		{
			return wglMakeCurrent(m_pRender.m_hDc, m_pRender.m_hGLRC)== TRUE ? true : false;
		}
		return false;
	}

	/***************************************************************************
	*! @brief  : Swap buffer
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void SwapBuffer()
	{
		::SwapBuffers(m_pRender.m_hDc);
	}
};

/**********************************************************************************
* ⮟⮟ Class name: WindowBase - Created: 15/03/2023
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport WindowBase : public XControl
{
public:
	// Define struct data
	//*****************************************************
	typedef struct tagWndStatus
	{
		enum class PushType
		{
			ALL		,
			SIZE	,
			POSITION,
		};

		std::wstring	m_title;
		CRect			m_rect;
		PushType		m_iPushType;

		tagWndStatus()
		{
			m_title			= L"";
			m_rect.x		= 0;
			m_rect.y		= 0;
			m_rect.width	= 640;
			m_rect.height	= 480;
		}

	} WindowStatus;

	typedef std::stack<WindowStatus>	WindowStatusLog;
	typedef std::vector<Control*>	ControlList;
	typedef std::vector<WindowBase*>	ChildWindowList;
	typedef std::map<int, bool>			MapStatus;

	//*****************************************************

protected:
	WindowSetting				m_Setting;
	WindowStatus				m_CurStatus;
	WindowStatusLog				m_StatusLog;

	DWORD						m_dwStyle;
	DWORD						m_dwStyleEx;

	GdiplusToken				m_GdiPlusToken;
	int							m_zDeltaScroll;

	// children control list 
	ControlList					m_ControlList;	// control in window : button | label |..
	unsigned int				m_uiControlIDs; // last id control
	ChildWindowList				m_ChildWindowList;

	// process event
	MapStatus					m_MouseStatus;
	MapStatus					m_KeyboardStatus;

	bool						m_bClosed;

	GDIplusCtrlRender*			m_pRender = nullptr;
public:
	WindowBase(): XControl(), 
		m_uiControlIDs(1000), m_zDeltaScroll(0), m_bClosed(false),
		m_dwStyle(0), m_dwStyleEx(0)
	{

	}
//==============================================================================
//⮟⮟ Triển khai cập nhật trạng thái của window                                 
//==============================================================================
protected:
	/***************************************************************************
	*! @brief  : Check list status window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool IsEmptyStackWindowStatus()
	{
		return m_StatusLog.empty();
	}

	/***************************************************************************
	*! @brief  : Save current window status information
	*! @return : int : index in stack
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int PushWindowStatus(WindowStatus::PushType type = WindowStatus::PushType::ALL)
	{
		WindowStatus status;

		switch (type)
		{
		case tagWndStatus::PushType::SIZE:
		{
			status.m_rect = m_CurStatus.m_rect;
			break;
		}
		case tagWndStatus::PushType::POSITION:
		{
			status.m_rect = m_CurStatus.m_rect;
			break;
		}
		default:
		{
			status.m_title = m_CurStatus.m_title;
			status.m_rect = m_CurStatus.m_rect;
		}
			break;
		}

		m_StatusLog.push(status);

		return (int)m_StatusLog.size();
	}

	/***************************************************************************
	*! @brief  : Get and drop window status information
	*! @return : WindowStatus
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual WindowStatus PopWindowStatus()
	{
		WindowStatus status;
		if (!m_StatusLog.empty())
		{
			status = m_StatusLog.top();
			m_StatusLog.pop();
		}
		return status;
	}

//==============================================================================
//⮟⮟ Triển khai hàm xử lý GDI của window                                       
//==============================================================================
protected:
	/***************************************************************************
	*! @brief  : Init GDI plus
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void CreateGDIplus()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;

		Gdiplus::Status status = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		m_GdiPlusToken = GdiplusToken{ (status == Gdiplus::Status::Ok) ? gdiplusToken : NULL,
									(status == Gdiplus::Status::Ok) ? true : false };
	}

	/***************************************************************************
	*! @brief  : Destroy init GDIplus
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void DeleteGDIplus()
	{
		m_GdiPlusToken.Shutdown();
	}

//==============================================================================
//⮟⮟ Hàm xử lý chung cho window                                                
//==============================================================================
public:
	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetTitle(std::wstring title)
	{
		m_CurStatus.m_title = title;

		NULL_RETURN(m_hWnd, );
		::SetWindowText(m_hWnd, m_CurStatus.m_title.c_str());
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void SetSize(int width, int height)
	{
		m_CurStatus.m_rect.width = width;
		m_CurStatus.m_rect.height = height;

		NULL_RETURN(m_hWnd, );
		::SetWindowPos(m_hWnd, NULL, m_rect.x, m_rect.y, width, height, 
			SWP_NOMOVE | SWP_NOZORDER);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetPosition(int x, int y)
	{
		m_CurStatus.m_rect.x = x;
		m_CurStatus.m_rect.y = y;
		NULL_RETURN(m_hWnd, );
		::SetWindowPos(m_hWnd, NULL, m_CurStatus.m_rect.x, m_CurStatus.m_rect.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	/***************************************************************************
	*! @brief  : move window position
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void MovePosition(const int x, const int y)
	{
		m_CurStatus.m_rect.x += x;
		m_CurStatus.m_rect.y += y;
		SetPosition(m_CurStatus.m_rect.x, m_CurStatus.m_rect.y);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	std::wstring GetTitle()
	{
		return m_CurStatus.m_title.c_str();
	}

	/***************************************************************************
	*! @brief  : Show window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Show()
	{
		this->Visible(true);
	}

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Hide()
	{
		this->Visible(false);
	}

	/***************************************************************************
	*! @brief  : Close window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Close()
	{
		Send_Message(WM_CLOSE, 0, 0);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [left - top]
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetCursorPos(long& xpos, long& ypos)
	{
		POINT cursor_pos = get_cursor_in_screen();
		if (ScreenToClient(m_hWnd, &cursor_pos) == TRUE)
		{
			xpos = cursor_pos.x;
			ypos = cursor_pos.y;
			return true;
		}
		else
		{
			xpos = 0;
			ypos = 0;
			return false;
		}
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [left - top]
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	static POINT get_cursor_in_screen()
	{
		POINT cursor_pos = {0, 0};
		if (::GetCursorPos(&cursor_pos) == TRUE)
		{
			return cursor_pos;
		}
		else
		{
			return { 0,0 };
		}
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetCursorPosCenter(long& xpos, long& ypos)
	{
		POINT cursor_pos = {0, 0};
		if (GetCursorPos(cursor_pos.x, cursor_pos.y))
		{
			xpos = -(GetWidth() / 2 - cursor_pos.x);
			ypos =  (GetHeight()/ 2 - cursor_pos.y);
		}

		return true;
	}

	/***************************************************************************
	*! @brief  : Last scroll value
	*! @return : int : 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetMouseScroll() { return (int)m_zDeltaScroll; }

	/***************************************************************************
	*! @brief  : set parent window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetParent(WindowBase* win)
	{
		HWND hwnd = win ? win->GetHwnd() : NULL;

		NULL_RETURN(hwnd,);
		XControl::SetParent(hwnd);
	}

//==============================================================================
//⮟⮟ Hàm xử lý control	                                                        
//==============================================================================
public:
	/***************************************************************************
	*! @brief  : Hàm khỏi tạo style window trước khi tạo
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void UpdateStyle()
	{
		NULL_RETURN(m_hWnd, );

		SetWindowLong(m_hWnd, GWL_STYLE, m_dwStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, m_dwStyleEx);
	}

	/***************************************************************************
	*! @brief  : Hàm khỏi tạo style window trước khi tạo
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool CreateStyleWindow() = 0;

	/***************************************************************************
	*! @brief  : Hàm ảo tạo và hủy cửa sổ
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool HandleBeforeCreateWindow()
	{
		return true;
	}

	virtual bool HandleAfterCreateWindow()
	{
		return true;
	}

	virtual void InitProperties()
	{

	}

	/***************************************************************************
	*! @brief  : Hàm ảo tạo và hủy cửa sổ
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool CreateHandle(const wchar_t* strWndClassName) = 0;
	virtual void DestroyHandle() = 0;
	virtual void DestroyCustom() { return; }

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool Create(HWND hwndParent, const wchar_t* strClassname)
	{
		bool bRet = true;

		XControl::SetParent(hwndParent);

		CHECK_EXE_UPDATE(bRet, CreateStyleWindow());
		CHECK_EXE_UPDATE(bRet, HandleBeforeCreateWindow());
		CHECK_EXE_UPDATE(bRet, CreateHandle(strClassname));
		CHECK_EXE_UPDATE(bRet, HandleAfterCreateWindow());

		if (bRet) // it OK
		{
			this->OnInitControl();
			this->OnCreateSubWindow();
			this->InitProperties();
		}

		return bRet;
	}

	/***************************************************************************
	*! @brief  : Destroy window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Destroy()
	{
		// Người dùng định nghĩa
		this->DestroyCustom();

		// Hệ thống dọn dẹp
		this->DestroyControl();
		this->DestroySubWindow();
		this->DestroyHandle();
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int AddControl(Control* control)
	{
		NULL_RETURN(control, 0);

		// check control exist
		auto itFound = std::find(m_ControlList.begin(), m_ControlList.end(), control);

		if (itFound != m_ControlList.end())
		{
			assert(0);
			return 0;
		}

		// it will initialize later
		if (m_hWnd == NULL)
		{
			m_ControlList.push_back(control);
		}
		// if existed window handle then initiation control
		else
		{
			control->SetRenderControl(m_pRender);

			auto uiNextControlID = control->Create(m_hWnd, m_uiControlIDs);
			if (uiNextControlID != m_uiControlIDs)
			{
				m_ControlList.push_back(control);
			}
			else
			{
				return 0;
			}
		}
		return 1;
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	int AddSubWindow(WindowBase* subwindow)
	{
		NULL_RETURN(subwindow, 0);

		// it will initialize later
		if (m_hWnd == NULL)
		{
			m_ChildWindowList.push_back(subwindow);
		}
		// if existed window handle then initiation control
		else
		{
			if (subwindow->Create(m_hWnd, GL_SUBWIN_CLASS))
			{
				m_ChildWindowList.push_back(subwindow);
			}
			else
			{
				return 0;
			}
		}
		return 1;
	}

//==============================================================================
//⮟⮟ Hàm xử lý control	- internal                                              
//==============================================================================
protected:

	/***************************************************************************
	*! @brief  : Update status window show - hide
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual WindowType GetType()
	{
		return WindowType::NoDefine;
	}

	/***************************************************************************
	*! @brief  : Create subwindow
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void OnCreateSubWindow()
	{
		for (int i = 0; i < m_ChildWindowList.size(); i++)
		{
			if (!m_ChildWindowList[i]->IsCreated())
			{
				if (!m_ChildWindowList[i]->Create(m_hWnd, GL_SUBWIN_CLASS))
				{
					std::cerr << "[!] Init subwindow failed : " <<
						m_ChildWindowList[i]->GetTitle().c_str() << " ." << std::endl;
				}
			}
		}
	}

	/***************************************************************************
	*! @brief  : Khởi tạo toàn bộ control đã được thêm
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void OnInitControl()
	{
		for (int i = 0; i < m_ControlList.size(); i++)
		{
			if (!m_ControlList[i]->IsCreated())
			{
				int iNextControlID = m_ControlList[i]->Create(m_hWnd, m_uiControlIDs);

				if (iNextControlID == m_uiControlIDs)
				{
					std::cerr << "[!] Init control failed !" << std::endl;
				}

				m_uiControlIDs = iNextControlID;
			}
		}
	}

	/***************************************************************************
	*! @brief  : Xóa và hủy tòa bộ control được quản lý bởi window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void DestroyControl()
	{
		for (int i = 0; i < m_ControlList.size(); i++)
		{
			m_ControlList[i]->OnDestroy();
			SAFE_DELETE(m_ControlList[i]);
		}
		m_ControlList.clear();
	}

	/***************************************************************************
	*! @brief  : Xóa và hủy tòa bộ subwindow được quản lý bởi window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void DestroySubWindow()
	{
		//for (int i = 0; i < m_subwindows.size(); i++)
		//{
		//	m_subwindows[i]->Destroy();
		//	SAFE_DELETE(m_subwindows[i]);
		//}
		//m_subwindows.clear();
	}

	virtual void DestroyUserCustom()
	{
		return;
	}

//==============================================================================
//⮟⮟ Hàm xử lý opengl- internal                                                
//==============================================================================
protected:

	/***************************************************************************
	*! @brief  : Xóa và hủy tòa bộ subwindow được quản lý bởi window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool CreateOpenGLContext(bool bUseOpenGLEx = false)
	{
		return false;
	}

	/***************************************************************************
	*! @brief  : Xóa và hủy tòa bộ subwindow được quản lý bởi window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void DestroyOpenGLContext()
	{
		return;
	}

//==============================================================================
//⮟⮟ Hàm xử lý sự kiên liên quan đến ngoại vi                                  
//==============================================================================
public:

	/***************************************************************************
	*! @brief  : Get keyboard state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetKeyboardStatus(int keyboard)
	{
		return m_KeyboardStatus[keyboard];
	}

	/***************************************************************************
	*! @brief  : Get button state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetMouseButtonStatus(int btn)
	{
		return m_MouseStatus[btn];
	}

protected:
	/***************************************************************************
	*! @brief  : Get mouse state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetMouseButtonStatus(int btn, bool status)
	{
		m_MouseStatus[btn] = status;
	}

	/***************************************************************************
	*! @brief  : Get keyboard state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetKeyboardStatus(int key, bool status)
	{
		m_KeyboardStatus[key] = status;
	}
};

____END_NAMESPACE____

#endif // !XWINBASE_H
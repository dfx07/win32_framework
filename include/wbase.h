////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     wbase.h
* @create   Nov 15, 2022
* @brief    Create and handle event control the window
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef WBASE_H
#define WBASE_H

#include "wdef.h"
#include <string>

___BEGIN_NAMESPACE___


class Window;
class SubWindow;

#define addAtribute(attribs, name, value)\
{\
    assert((size_t) attribCount < attribsize);\
    attribs[attribCount++] = name;  \
    attribs[attribCount++] = value; \
}

#define addAtributeEnd(attribs)\
{\
    assert((size_t) attribCount < attribsize); \
    attribs[attribCount++] = 0;  \
}

#define ON_FUNCTION_WINDOW(bFunCheck, ...)\
{\
	if(bFunCheck) \
	{\
		return bFunCheck(__VA_ARGS__);\
	}\
}

enum class ControlType
{
	BUTTON		,
	MENUCONTEXT	,
	MENUBAR		,
	COMBOBOX	,
	LABEL		,
	CHECKBOX	,
};

enum class WindowType
{
	NoDefine,
	MainWin,
	SubWin ,
};

class WindowBase;
class Control;
class Window;
class SubWindow;

/**********************************************************************************
* ⮟⮟ Struct base window
* Base class for window handle inheritance
***********************************************************************************/
typedef struct tagWndStatus
{
	std::wstring	m_title;
	int				m_x;
	int				m_y;
	int				m_width;
	int				m_height;
	int				m_RelState;

	tagWndStatus()
	{
		m_title		= L"None";
		m_x			= 0;
		m_y			= 0;
		m_width		= 640;
		m_height	= 480;
		m_RelState	= 0;    // Trạng thái mặc định
	}
} WindowStatus;


typedef struct tagControlRect
{
	float     m_x;
	float     m_y;
	UINT      m_width;
	UINT      m_height;

public:
	tagControlRect(): m_x(0.f), m_y(0.f),
		m_width(0), m_height(0)
	{

	}

	void Set(float x, float y, UINT w, UINT h)
	{
		this->SetPos(x, y);
		this->SetSize(w, h);
	}

	void Set(int x, int y, UINT w, UINT h)
	{
		this->SetPos((float)x, (float)y);
		this->SetSize(w, h);
	}

	void SetPos(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	void SetSize(UINT w, UINT h)
	{
		m_width = w;
		m_height = h;
	}
} ControlRect;

/**********************************************************************************
* ⮟⮟ Class name: WindowBase
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport Control
{
protected:
	UINT         m_ID;			 // ID control
	HWND         m_hwnd;		 // win handle control
	HWND         m_hwndPar;		 // parent control
	ControlRect  m_rect;		 // rect control information

	bool         m_bEnable;		 // enable control
	bool         m_bVisble;		 // is visible

public:
	Control(): m_hwnd(NULL), m_ID(0),
		m_hwndPar(NULL)
	{
		m_bEnable = true;
		m_bVisble = true;
	}

protected:

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual ControlType GetType() = 0;

	virtual int IsCreated() { return m_hwnd ? TRUE : FALSE; }

	/***************************************************************************
	*! @brief  : Base function destroy window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void OnDestroy(){};

	/***************************************************************************
	*! @brief  : Active when user use BS_OWNERDRAW control
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Draw(LPDRAWITEMSTRUCT& ){};

	/***************************************************************************
	*! @brief  : Active when user use BS_OWNERDRAW control
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Event(WindowBase* window, WORD _id, WORD _event){};
	virtual bool ContainID(INT ID){ return false; };

protected:
	void SetParent(HWND hwnd) { m_hwndPar = hwnd; }
	void SetID(UINT uiID)	  { m_ID      = uiID; }

public:
	HWND  GetHwnd()  { return m_hwnd; }
	UINT  GetID()    { return m_ID;	  }

	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int OnInitControl() = 0;

	/***************************************************************************
	*! @brief  : function design pattern inti control
	*! @return : 0 : ok | 1 : init control failed
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	int InitControl(HWND parrent, UINT& uiID)
	{
		this->SetParent(parrent);
		this->SetID(uiID);

		int iCreatedCtrl = this->OnInitControl();

		// Init control done -> Ok
		if (iCreatedCtrl > 0 && IsCreated())
		{
			uiID += iCreatedCtrl;
			SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		}
		else
		{
			m_hwnd = NULL;
			this->SetParent(NULL);
			this->SetID(0);
			return 1;
		}

		return 0;
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Enable(bool bEnable)
	{
		m_bEnable = bEnable;
		::EnableWindow(m_hwnd, m_bEnable ? TRUE : FALSE);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Visible(bool bVisible)
	{
		m_bVisble = bVisible;
		::ShowWindow(m_hwnd, m_bVisble ? TRUE : FALSE);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetPosition(int x, int y)
	{
		m_rect.SetPos(x, y);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetSize(int width, int height)
	{
		m_rect.SetSize(width, height);
	}

	friend class Window;
	friend class WindowBase;
	friend class SubWindow;
};


/**********************************************************************************
* ⮟⮟ Class name: WindowBase
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport WindowOpenGLContext
{
protected:
	WindowRender		m_pRender;

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
	static WindowRender make_opengl_context(HWND hWnd, bool bUseOpenGLEx, int iAntialiasing = 0)
	{
		HDC   hDC   = GetDC(hWnd);
		HGLRC hglrc = NULL;

		int iPixelFormat; unsigned int num_formats = 0;

		// Lấy mảng thuộc tính thiết lập để set style window 
		auto funGetFixelFormatAttribute = [&](int* attribs, int attribsize)
		{
			int attribCount = 0;

			addAtribute(attribs, WGL_DRAW_TO_WINDOW_ARB, GL_TRUE);
			addAtribute(attribs, WGL_SUPPORT_OPENGL_ARB, GL_TRUE);
			addAtribute(attribs, WGL_DOUBLE_BUFFER_ARB, GL_TRUE);
			addAtribute(attribs, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB);
			addAtribute(attribs, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB);
			addAtribute(attribs, WGL_COLOR_BITS_ARB, 32);
			addAtribute(attribs, WGL_DEPTH_BITS_ARB, 24);
			addAtribute(attribs, WGL_STENCIL_BITS_ARB, 8);

			if (iAntialiasing > 0)
			{
				addAtribute(attribs, WGL_SAMPLE_BUFFERS_ARB, GL_TRUE); // Enable multisampling
				addAtribute(attribs, WGL_SAMPLES_ARB, iAntialiasing);  // Number of samples
			}
			addAtributeEnd(attribs);
		};

		// Get pixel format attributes through "modern" extension
		if (bUseOpenGLEx)
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

		WindowRender winRender = { hWnd, hDC, hglrc };

		if (!make_current_context(winRender))
		{
			delete_opengl_context(winRender);
		}

		return winRender;
	}

	/***************************************************************************
	*! @brief  : check status opengl use window render
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	static bool is_opengl_ok(WindowRender& pRender)
	{
		return (pRender.m_hGLRC && pRender.m_bCreated);
	}

	/***************************************************************************
	*! @brief  : initialize initial state for context
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	static bool make_current_context(WindowRender& pRender)
	{
		if (wglMakeCurrent(pRender.m_hDc, pRender.m_hGLRC))
		{
			pRender.m_bCreated = true;
			return true;
		}
		pRender.m_bCreated = false;
		return false;
	}

	/***************************************************************************
	*! @brief  : Remove the previously set context
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : use openGL extension previously established
	***************************************************************************/
	static void delete_opengl_context(WindowRender& pRender)
	{
		ReleaseDC(pRender.m_hWnd, pRender.m_hDc);	// release device context
		wglDeleteContext(pRender.m_hGLRC);			// delete the rendering context
		pRender.m_bCreated = false;
		pRender.m_hDc	= NULL;
		pRender.m_hGLRC = NULL;
	}

protected:
	virtual HDC  GetRenderHDC() { return m_pRender.m_hDc; }

	virtual bool MakeOpenGLContext(HWND hWnd, bool bUseOpenGLEx, int iAntialiasing = 0)
	{
		m_pRender = make_opengl_context(hWnd, bUseOpenGLEx, iAntialiasing);
		
		return m_pRender.m_bCreated;
	}

	void DeleteOpenGLContext()
	{
		delete_opengl_context(m_pRender);
	}

	/***************************************************************************
	*! @brief  : Active OpenGL context
	*! @return : void 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool MakeContext()
	{
		if (is_opengl_ok(m_pRender))
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
class Dllexport WindowBase
{
protected:
	HWND						m_hWnd;
	HWND						m_hWndParent;
	GdiplusToken				m_gdiToken;
	std::stack<WindowStatus>	m_stackStatus;

	std::wstring				m_title;
	int							m_x;
	int							m_y;
	int							m_width;	// The drawable width
	int							m_height;	// The drawable height

	bool						m_bClosed;

	int							m_iShow;	// window show status;
	int							m_zDeltaScroll;

	std::map<int, bool>			m_mouse;
	std::map<int, bool>			m_keyboard;

	std::vector<Control*>		m_controls;		// control in window : button | label |..
	unsigned int				m_uiControlIDs; // last id control

	std::vector<WindowBase*>	m_subwindows;



protected:
	WindowBase() : m_hWnd(NULL), m_hWndParent(NULL), m_x(0), m_y(0),
		m_width(0), m_height(0), m_title(L""), m_iShow(SW_SHOW),
		m_uiControlIDs(1), m_zDeltaScroll(0), m_bClosed(false)
	{

	}

//==============================================================================
//⮟⮟ Triển khai cập nhật trạng thái của window                                 
//==============================================================================
private:

	/***************************************************************************
	*! @brief  : Check list status window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool IsEmptyStackWindowStatus()
	{
		return m_stackStatus.empty();
	}

	/***************************************************************************
	*! @brief  : Save current window status information
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void PushWindowStatus()
	{
		WindowStatus status;
		status.m_title	= m_title;
		status.m_x		= m_x;
		status.m_y		= m_y;
		status.m_width	= m_width;
		status.m_height = m_height;
		status.m_RelState = 1;

		m_stackStatus.push(status);
	}

	/***************************************************************************
	*! @brief  : Get and drop window status information
	*! @return : WindowStatus
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual WindowStatus PopWindowStatus()
	{
		WindowStatus status;
		if (!m_stackStatus.empty())
		{
			status = m_stackStatus.top();
			m_stackStatus.pop();
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

		m_gdiToken = GdiplusToken{ (status == Gdiplus::Status::Ok) ? gdiplusToken : NULL,
									(status == Gdiplus::Status::Ok) ? true : false };
	}

	/***************************************************************************
	*! @brief  : Destroy init GDIplus
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void DeleteGDIplus()
	{
		m_gdiToken.Shutdown();
	}

//==============================================================================
//⮟⮟ Triển khai hàm xử lý nội bộ trong window                                  
//==============================================================================
protected:

	/***************************************************************************
	*! @brief  : Check window control is created
	*! @return : true : created | false : not create
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool IsCreated()
	{
		return m_hWnd ? true : false;
	}

	virtual bool Create(const wchar_t* strClassname)
	{
		return true;
	}

	/***************************************************************************
	*! @brief  : Check window control is created
	*! @return : true : created | false : not create
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Destroy()
	{
		return;
	}

	virtual bool CreateOpenGLContext(bool bUseOpenGLEx = false)
	{
		return false;
	}

	virtual void DestroyOpenGLContext()
	{
		return;
	}

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
	*! @brief  : Update status window show - hide
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void UpdateStatus(int iShow)
	{
		NULL_CHECK_RETURN(m_hWnd, );

		if (iShow)
			::ShowWindow(this->m_hWnd, SW_SHOW);
		else
			::ShowWindow(this->m_hWnd, SW_HIDE);
	}

	/***************************************************************************
	*! @brief  : Update title window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void UpdateTitle()
	{
		NULL_CHECK_RETURN(m_hWnd, );
		::SetWindowText(m_hWnd, m_title.c_str());
	}

	/***************************************************************************
	*! @brief  : Update window position
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void UpdateLocation()
	{
		NULL_CHECK_RETURN(m_hWnd, );
		::SetWindowPos(m_hWnd, NULL, m_x, m_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	/***************************************************************************
	*! @brief  : Update window position
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void UpdateSize()
	{
		NULL_CHECK_RETURN(m_hWnd, );
		::SetWindowPos(m_hWnd, NULL, m_x, m_y, 0, 0, SWP_NOMOVE | SWP_NOZORDER);
	}

	virtual void UpdateParent()
	{
		NULL_CHECK_RETURN(m_hWnd, );
		::SetParent(m_hWnd, m_hWndParent);
	}

//==============================================================================
//⮟⮟ Hàm thao tác chung cho mọi window	                                        
//==============================================================================
public:
	/***************************************************************************
	*! @brief  : Get window handle base
	*! @return : HWND
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	HWND  GetHwnd() { return m_hWnd; }

	/***************************************************************************
	*! @brief  : Set status window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Visible(bool bShow)
	{
		m_iShow = bShow ? SW_SHOW : SW_HIDE;
		this->UpdateStatus(m_iShow);
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
		this->Visible(true);
	}

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetWidth()  { return m_width; }

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetHeight() { return m_height; }

	/***************************************************************************
	*! @brief  : Last scroll value
	*! @return : int : 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetMouseScroll() { return (int)m_zDeltaScroll; }

	/***************************************************************************
	*! @brief  : Get current cursor position [left - top]
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetCursorPos(long& xpos, long& ypos)
	{
		POINT cursor_pos;
		if (::GetCursorPos(&cursor_pos) && ScreenToClient(m_hWnd, &cursor_pos))
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
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetCursorPosCenter(long& xpos, long& ypos)
	{
		POINT cursor_pos;
		if (::GetCursorPos(&cursor_pos) && ScreenToClient(m_hWnd, &cursor_pos))
		{
			xpos = -(m_width  / 2 - cursor_pos.x);
			ypos =  (m_height / 2 - cursor_pos.y);
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
	*! @brief  : Get keyboard state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetKeyboardStatus(int keyboard)
	{
		return m_keyboard[keyboard];
	}

	/***************************************************************************
	*! @brief  : Get button state
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetMouseButtonStatus(int btn)
	{
		return m_mouse[btn];
	}

	/*********************************SETTER************************************/

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetTitle(std::wstring title)
	{
		m_title = title;
		this->UpdateTitle();
	}

	/***************************************************************************
	*! @brief  : set window position
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetPosition(const int x, const int y)
	{
		m_x = x;
		m_y = y;
		this->UpdateLocation();
	}

	/***************************************************************************
	*! @brief  : Set size window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetSize(const int width, const int height)
	{
		m_width = width;
		m_height = height;
		this->UpdateSize();
	}

	/***************************************************************************
	*! @brief  : set parent window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetParent(WindowBase* win)
	{
		NULL_CHECK_RETURN(win->GetHwnd(),);

		m_hWndParent = win->GetHwnd();
		this->UpdateParent();
	}

	void SetParent(HWND parent)
	{
		NULL_CHECK_RETURN(parent, );

		m_hWndParent = parent;
		this->UpdateParent();
	}

//==============================================================================
//⮟⮟ Hàm xử lý control	                                                        
//==============================================================================
public:
	void SetStartIDControl(unsigned int id)
	{
		m_uiControlIDs = id;
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	int AddControl(Control* control)
	{
		NULL_CHECK_RETURN(control, 0);

		// it will initialize later
		if (m_hWnd == NULL)
		{
			m_controls.push_back(control);
		}
		// if existed window handle then initiation control
		else
		{
			if (!control->InitControl(m_hWnd, m_uiControlIDs))
			{
				m_controls.push_back(control);
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
		NULL_CHECK_RETURN(subwindow, 0);

		// it will initialize later
		if (m_hWnd == NULL)
		{
			m_subwindows.push_back(subwindow);
		}
		// if existed window handle then initiation control
		else
		{
			subwindow->SetParent(m_hWnd);
			if (!subwindow->Create(GL_SUBWIN_CLASS))
			{
				m_subwindows.push_back(subwindow);
			}
			else
			{
				return 0;
			}
		}
		return 1;
	}

protected:
	/***************************************************************************
	*! @brief  : Create subwindow
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void OnCreateSubWindow()
	{
		int err = 0;

		for (int i = 0; i < m_subwindows.size(); i++)
		{
			if (!m_subwindows[i]->IsCreated())
			{
				m_subwindows[i]->SetParent(this);
				err += m_subwindows[i]->Create(GL_SUBWIN_CLASS) ? 0 : 1;
			}
		}

		std::cerr << "[+] Init subwindow done !. " << err << " error." << std::endl;
	}

	/***************************************************************************
	*! @brief  : Khởi tạo toàn bộ control đã được thêm
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void OnInitControl()
	{
		int err = 0;

		for (int i = 0; i < m_controls.size(); i++)
		{
			if (!m_controls[i]->IsCreated())
			{
				err += (m_controls[i]->InitControl(m_hWnd, m_uiControlIDs) != 0)
					? 1: 0;
			}
		}

		std::cerr << "[+] Init control done !. " << err << " error." << std::endl;
	}

	/***************************************************************************
	*! @brief  : Xóa và hủy tòa bộ control được quản lý bởi window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void DestroyControl()
	{
		for (int i = 0; i < m_controls.size(); i++)
		{
			m_controls[i]->OnDestroy();
			SAFE_DELETE(m_controls[i]);
		}
		m_controls.clear();
	}


	/***************************************************************************
	*! @brief  : Xóa và hủy tòa bộ subwindow được quản lý bởi window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void DestroySubWindow()
	{
		for (int i = 0; i < m_subwindows.size(); i++)
		{
			m_subwindows[i]->Destroy();
			SAFE_DELETE(m_subwindows[i]);
		}
		m_subwindows.clear();
	}

//==============================================================================
//⮟⮟ Hàm xử lý nội bộ                                                          
//==============================================================================
protected:
	void SetMouseButtonStatus(int btn, bool status)
	{
		m_mouse[btn] = status;
	}

	void SetKeyboardStatus(int key, bool status)
	{
		m_keyboard[key] = status;
	}

	friend class Window;
	friend class SubWindow;
};

____END_NAMESPACE____

#endif // WBASE_H

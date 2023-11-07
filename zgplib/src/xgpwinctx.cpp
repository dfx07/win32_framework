#include "xgpwinctx.h"

____BEGIN_SECTION____(gp)

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"../build/lib/glew32.lib")

WindowOpenGLContext::WindowOpenGLContext()
	:m_bUseOpenGLEx(false)
{
	
}

/***************************************************************************
*! @brief  : Get keyboard state
*! @return : true : ok | false : not ok
*! @author : thuong.nv          - [Date] : 05/03/2023
***************************************************************************/
bool WindowOpenGLContext::InitContext(HWND hwnd, bool bUse)
{
	m_pRender.m_hWnd = hwnd;
	m_bUseOpenGLEx = bUse;

	return true;
}

/***************************************************************************
*! @brief  : register window class use window context
*! @param  : [In] strClassName : Name class
*! @param  : [In] Proc		   : function hanle event
*! @param  : [In] hInst		   : default GetModuleHandle(NULL)
*! @return : 1 : OK , 0 : False
*! @author : thuong.nv          - [Date] : 05/03/2023
***************************************************************************/
int WindowOpenGLContext::register_window_class(const wchar_t* strClassName, WNDPROC Proc, HINSTANCE hInst)
{
	WNDCLASSEX  wClass;
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

/*******************************************************************************
*! @brief  : Dummy handler function to create opengl
*! @param  : [In] hwnd		: Name class
*! @param  : [In] message	: default
*! @param  : [In] wParam	: default
*! @param  : [In] lParam	: default
*! @return : 1 : OK , 0 : False
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
LRESULT WindowOpenGLContext::win_dummy_main_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*******************************************************************************
*! @brief  : load extension opengl library glew
*! @param  : [In] strClassName : Name class
*! @param  : [In] Proc		   : function hanle event
*! @param  : [In] hInst		   : default GetModuleHandle(NULL)
*! @return : 1: OK | 0 False;
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
int WindowOpenGLContext::load_opengl_extension()
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

/*******************************************************************************
*! @brief  : init and create OpenGL context
*! @param  : [In] void
*! @return : true : OK | false : False
*! @author : thuong.nv          - [Date] : 05/03/2023
*! @note   : use openGL extension previously established
********************************************************************************/
bool WindowOpenGLContext::make_opengl_context(int iAntialiasing)
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

/*******************************************************************************
*! @brief  : check status opengl use window render
*! @return : true : ok | false : not ok
*! @author : thuong.nv          - [Date] : 05/03/2023
*! @note   : use openGL extension previously established
********************************************************************************/
bool WindowOpenGLContext::is_opengl_ok()
{
	return (m_pRender.m_hGLRC && m_pRender.m_bCreated);
}

/*******************************************************************************
*! @brief  : initialize initial state for context
*! @return : true : ok | false : not ok
*! @author : thuong.nv          - [Date] : 05/03/2023
*! @note   : use openGL extension previously established
********************************************************************************/
bool WindowOpenGLContext::make_current_context()
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

/*******************************************************************************
*! @brief  : Remove the previously set context
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
*! @note   : use openGL extension previously established
********************************************************************************/
void WindowOpenGLContext::delete_opengl_context()
{
	ReleaseDC(m_pRender.m_hWnd, m_pRender.m_hDc);	// release device context
	wglDeleteContext(m_pRender.m_hGLRC);			// delete the rendering context
	m_pRender.m_bCreated = false;
	m_pRender.m_hDc = NULL;
	m_pRender.m_hGLRC = NULL;
}

/*******************************************************************************
*! @brief  : Get render device
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
void* WindowOpenGLContext::Render()
{
	return &m_pRender.m_hDc;
}

/*******************************************************************************
*! @brief  : Create opengl context
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
bool WindowOpenGLContext::CreateContext(int iAntialiasing)
{
	if (make_opengl_context(iAntialiasing))
	{
		return m_pRender.m_bCreated;
	}
	return false;
}

/*******************************************************************************
*! @brief  : Delete OpenGL context
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
void WindowOpenGLContext::DeleteContext()
{
	delete_opengl_context();
}

/*******************************************************************************
*! @brief  : Active OpenGL context
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
bool WindowOpenGLContext::MakeContext()
{
	if (is_opengl_ok())
	{
		return wglMakeCurrent(m_pRender.m_hDc, m_pRender.m_hGLRC) == TRUE ? true : false;
	}
	return false;
}

/*******************************************************************************
*! @brief  : Swap buffer
*! @return : void
*! @author : thuong.nv          - [Date] : 05/03/2023
********************************************************************************/
void WindowOpenGLContext::SwapBuffer()
{
	::SwapBuffers(m_pRender.m_hDc);
}

_____END_SECTION_____

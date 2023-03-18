////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     wglhandle.h
* @create   Nov 15, 2022
* @brief    Create and handle event window
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef WHANLDE_H
#define WHANLDE_H

#include <assert.h>

#include <stdlib.h>
#include <string.h>
#include <unordered_map>

#include <map>
#include <stack>
#include <mutex>

#include <chrono>
#include <windows.h>

#include "GL/glew.h"
#include "GL/wglew.h"

#include "wcontrol.h"
#include "xsystype.h"
#include "wbase.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glew32.lib")




___BEGIN_NAMESPACE___

/// ////////////////////////////////////////////////////////////////////////////////
/// GDIplusToken struct - WINDOW
/// Provides a container GDI data
/// ////////////////////////////////////////////////////////////////////////////////

//Không sử dụng Graphics vì nó bị đỏ khi Bug khó chịu @^@
typedef Gdiplus::Graphics                   WndGDIplus;




enum class FontWeight
{
	Normal,
	Bold,
	Thin,
	Light,
};

struct WndProp
{
	bool                m_bFullScreen;
	bool                m_bGDIplus;
	bool                m_bOpenGL;       // State OpenGL
	int                 m_iAntialiasing; // Antialiasing level = 8
	int					m_iModeDraw;	 // 1 : use thread draw opengl | 0 :use pipe main thread
	bool				m_writeinfo;
	int					m_iAphaTrans = -1;


	DWORD               m_dwExStyle;     // Window Extended Style
	DWORD               m_dwStyle;       // Window Style


	void set_default()
	{
		m_bFullScreen   = false;
		m_bGDIplus	    = false;
		m_bOpenGL	    = false;
		m_iAntialiasing = -1;
		m_iModeDraw		= 0;
		m_writeinfo		= false;
	}

	WndProp()
	{
		this->set_default();
	}

	WndProp(const char* title, int xpos, int ypos, int width = 640, int height = 480):
		m_dwExStyle(0), m_dwStyle(0)
	{
		m_bFullScreen	= false;
		m_bGDIplus		= false;
		m_bOpenGL		= true;
		m_iAntialiasing = -1;
	}
};

class Window;
class ChilWindow;


//==================================================================================
// Class Window : Thông tin và ngữ cảnh của một handle                              
//==================================================================================
class Dllexport Window : public WindowBase, public WindowOpenGLContext, public WindowEvent
{
protected:

	static bool register_class(bool bSet = false, bool value = false)
	{
		static bool m_bRegisterClass = false;
		if (bSet == true) // set
		{
			m_bRegisterClass = value;
		}

		return m_bRegisterClass;
	}

	static bool opengl_extension(bool bSet = false, bool value = false)
	{
		static bool m_bLoadOpenGLEx = false;
		if (bSet == true) // set
		{
			m_bLoadOpenGLEx = value;
		}

		return m_bLoadOpenGLEx;
	}

private:
	std::stack<WndProp>    m_PropStack;

	std::vector<WindowBase>		m_ChilWinList;

private:
	WndProp                m_pProp;

	MSG					   m_msg;


	short                  m_zDeltaScroll;

private:
	bool				   m_bUseOpenGLEx;

private:
										// State information
	bool                   m_bShow;

	// System information : fps -frametime
	CFPSCounter			   m_fpscounter;

	// Text render 2D
	//GLWinFontRender        m_text_render;
	const char*            m_fontNameTextRender;
	unsigned int           m_fontSizeTextRender;
	bool                   m_bSysInfo;

	// System draw infor
	bool				   m_bUpdateRenderInfo; // update view , text render info :
	int					   m_iIdUpdate;
	std::mutex						m_renderinfo_mutex;
	std::condition_variable			m_sycn_renderinfo;

	std::wstring		   m_gpu_device_name;

	Safe_Thread			   m_drawthread;
	Safe_Thread			   m_processthread;

	// Font information
	std::wstring           m_fontName;
	unsigned int           m_fontSize;
	FontWeight             m_fontWeight;

private:
//======================================================================================
//⮟⮟ Triển khai xử lý thông điệp cho window                                            
//======================================================================================
	static LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window* win = (Window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		NULL_RETURN(win, DefWindowProc(hWnd, message, wParam, lParam));

		switch (message)
		{
		case WM_CLOSE:
		{
			win->m_bClosed = true;
			break;
		}
		case WM_DESTROY:
		{
			win->m_bClosed = true;
			win->OnDestroy();
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			win->SetKeyboardStatus((int)wParam, true);
			win->OnKeyBoard(win);
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			win->SetKeyboardStatus((int)wParam, false);
			win->OnKeyBoard(win);
			break;
		}
		case WM_LBUTTONUP:
		{
			win->SetMouseButtonStatus(VK_LBUTTON, false);
			win->OnMouseButton(win, GLMouse::LeftButton, GL_RELEASE);
			break;
		}
		case WM_RBUTTONUP:
		{
			win->SetMouseButtonStatus(VK_RBUTTON, false);
			win->OnMouseButton(win, GLMouse::RightButton, GL_RELEASE);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			win->SetMouseButtonStatus(VK_LBUTTON, true);
			win->OnMouseButton(win, GLMouse::LeftButton, GL_PRESSED);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			win->SetMouseButtonStatus(VK_RBUTTON, true);
			win->OnMouseButton(win, GLMouse::RightButton, GL_PRESSED);
			break;
		}
		case WM_MOUSEMOVE:
		{
			for (int i = 0; i < win->m_subwindows.size(); i++)
			{
				win->m_subwindows[i]->PushMessage(SUB_MOVE_WIN, SUB_PROCESS_MSG, 0);
			}

			win->OnMouseMove(win);
			break;
		}
		case WM_SIZE: //Check if the window has been resized
		{
			win->m_width  = LOWORD(lParam); // width
			win->m_height = HIWORD(lParam); // height
			win->UpdateRenderInfo();

			win->UpdateTitle();
			// cannot use opengl context in this tunnel
			win->OnResize(win);
			
			// Refresh screen when resize window in case one thread
			if (win->GetDrawMode() == 0)
				win->OnDraw();

			break;
		}
		case WM_SIZING:
		{
			win->UpdateTitle();
			break;
		}
		case WM_MOUSEWHEEL:
		{
			win->m_zDeltaScroll = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			win->OnMouseScroll(win);
			break;
		}
		case WM_COMMAND:
		{
			WORD wID  = LOWORD(wParam); // item, control, or accelerator identifier
			WORD wEvt = HIWORD(wParam); // item event
			HWND hwndControl = (HWND)lParam;  // handle of control
			if (hwndControl)
			{
				Control* ctrl = (Control*)(GetWindowLongPtr(hwndControl, GWLP_USERDATA));
				if (ctrl) ctrl->Event(win, wID, wEvt);
			}
			break;
		}
		case WM_DRAWITEM:
		{
			WORD wID = LOWORD(wParam); // item, control, or accelerator identifier
			LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
			Control* ctrl = (Control*)(GetWindowLongPtr(pdis->hwndItem, GWLP_USERDATA));
			if (ctrl)
				ctrl->Draw(pdis);
			break;
		}
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN: //In order to make those edges invisble when we use RoundRect(),
		{                //we make the color of our button's background match window's background
			SetBkMode((HDC)wParam, TRANSPARENT);
			HWND hwnd = (HWND)lParam;

			Control* ctrl = (Control*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (ctrl && ctrl->GetType() == ControlType::LABEL)
			{
				Label* lab = static_cast<Label*>(ctrl);
				lab->UpdateTextColor((HDC)wParam);
			}

			return (INT_PTR)GetStockObject((HOLLOW_BRUSH));
		}
		case WM_PAINT:
		{
			win->OnPaint(win);
			break;  //[BUG] always drawing
		}
		case WM_ERASEBKGND:
		{

			return TRUE;
		}
		default:
		{
			break;
		}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

private:
	// get gpu name use opengl 
	virtual std::wstring GetGPUDevice()
	{
		std::string gpu_device = "No Device";
		if ((char*)glGetString(GL_RENDERER))
		{
			gpu_device = (char*)glGetString(GL_RENDERER);
		}

		std::wstring utf16_gpu_device;
		utf16_gpu_device.resize(gpu_device.size() + 1, 0);
		int nWide = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, gpu_device.c_str(),
				(int)gpu_device.length(), (LPWSTR)utf16_gpu_device.c_str(), (int)utf16_gpu_device.size());
		utf16_gpu_device.resize(nWide);

		return utf16_gpu_device;
	}

private:
	virtual void OnDraw()
	{
		if (!MakeContext()) return;

		if (m_pProp.m_iModeDraw == 1)
		{
			if (m_bUpdateRenderInfo)
			{
				glViewport(0, 0, m_width, m_height);
				UpdateTextRender();

				m_gpu_device_name = this->GetGPUDevice();

				m_bUpdateRenderInfo = false;
				m_sycn_renderinfo.notify_all();
			}
		}
		else
		{
			glViewport(0, 0, m_width, m_height);
			UpdateTextRender();
		}

		this->UpdateInfo();

		CHECK_RUN_FUNCTION(m_funOnDraw, this);

		// Hiển thị thông tin fps - frametime và thông tin hệ thống nếu cần
		if (m_bSysInfo == true)
		{
			char strsys[80];
			sprintf_s(strsys, "fps: %d - frametime : %f ms", GetFPS(), GetFrameTime());
			this->WriteText(strsys, 10, 20);
		}

		this->SwapBuffer();
	}

protected:

	virtual WindowType GetType() { return WindowType::MainWin; }

//======================================================================================
//⮟⮟ Triển khai chính tạo và xử lý ngữ cảnh window  - important                        
//======================================================================================
private:
	/***************************************************************************
	*! @brief  : setup flag load OpenGL Extension
	*! @param  : [In] bUse : use OpenGL extension
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void UseOpenGLExtension(const bool bUse = true)
	{
		this->m_bUseOpenGLEx = bUse;
	}

//======================================================================================
//⮟⮟ Triển khai khởi tạo liên quan đến window                                          
//======================================================================================
private:
	//==================================================================================
	// Hàm tạo handle và thông số                                                       
	//==================================================================================
	bool CreateWindowHandle(const wchar_t* strWndClassName)
	{
		// Create GDI+ startup incantation
		if (m_pProp.m_bGDIplus)
		{
			this->CreateGDIplus();
		}

		// Kích thức thực tế của vùng có thể vẽ
		RECT wr = { 0, 0, m_width, m_height };     // set the size, but not the position
		AdjustWindowRect(&wr, m_pProp.m_dwStyle, FALSE);// adjust the size

		m_hWnd = CreateWindowEx( m_pProp.m_dwExStyle,        //
								 strWndClassName,            //
								 m_title.c_str(),            //
								 m_pProp.m_dwStyle,          //
								 m_x, m_y,                   // Postion 
								 wr.right - wr.left,         // Actual width size
								 wr.bottom - wr.top,         // Actual height size
								 NULL,                       //
								 NULL,                       //
								 NULL,                       //
								 NULL                        //
		);

		// Create window failed or associate failed
		if (!m_hWnd)
		{
			this->DeleteGDIplus();
			::DestroyWindow(m_hWnd);
			return false;
		}
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

		// Update size window after created
		RECT rect;
		if (GetClientRect(m_hWnd, &rect))
		{
			m_width = rect.right - rect.left;
			m_height = rect.bottom - rect.top;
		}
		return true;
	}


//======================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                         
//======================================================================================
protected:
	//==================================================================================
	// Khởi tạo window và thiết lập thông số                                            
	//==================================================================================
	bool Create(const wchar_t* strClassname)
	{
		this->SetStartIDControl(1000);

		bool ret = true;
		// Update get style window
		this->SetUpHint();

		// Create a window HWND use class name
		ret &= this->CreateWindowHandle(strClassname);

		this->SetTitle(GetTitleInfo());

		if (ret) // it OK
		{
			// Update get style window
			this->UpdateHint();

			// Active function user custom
			this->OnCreated(this);

			// Update title after created ok
			this->UpdateTitle();

			// Initialization control
			this->OnInitControl();

			// Initialization sub window
			this->OnCreateSubWindow();

			// Update and setup properties when everything is done
			this->InitProperties();
		}

		return ret;
	}

	//==================================================================================
	// Khởi tạo window và thiết lập thông số                                            
	//==================================================================================
	bool CreateOpenGLContext(bool bUseOpenGLEx = false)
	{
		// Set up use OpenGL extension
		this->UseOpenGLExtension(bUseOpenGLEx);

		// Any given OpenGL rendering context can be active at only one thread at a time.
		// if I create opengl context in main thread then this thread not active

		if (m_pProp.m_iModeDraw == 1)
		{
			if (!m_drawthread.is_created())
			{
				m_drawthread.create(&Window::ThreadDrawOpenGL, this);
				m_drawthread.detach();
			}

			if (!m_processthread.is_created())
			{
				m_processthread.create(&Window::ThreadProcessOpenGL, this);
			}
		}
		// create opengl main thread
		else
		{
			if (!MakeOpenGLContext(m_hWnd, bUseOpenGLEx, m_pProp.m_iAntialiasing))
			{
				return false;
			}

			this->ReloadTextRender();
		}

		return true;
	}

//=======================================================================================
//⮟⮟ Triển khai cập nhật trạng thái của window                                          
//=======================================================================================
private:
	//===================================================================================
	// Cập nhật trạng thái thời gian mỗi khi một frame trôi qua                          
	//===================================================================================
	void UpdateInfo()
	{
		m_fpscounter.update();
	}

	//===================================================================================
	// Khởi tạo trạng thái và thông tin thuộc tính khi đã tạo windows xong               
	//===================================================================================
	void InitProperties()
	{
		m_fpscounter.start();
	}

	//===================================================================================
	// send notify to draw thread and update render info
	//===================================================================================
	void UpdateRenderInfo()
	{
		if (m_pProp.m_iModeDraw == 1)
		{
			// update information draw in thread draw and notify ok to main thread
			m_bUpdateRenderInfo = true;
			std::unique_lock< std::mutex> lock(m_renderinfo_mutex);

			// wait for : draw thread update draw info ok
			while (m_bUpdateRenderInfo && m_drawthread.is_detach())
			{
				m_sycn_renderinfo.wait(lock);
			}
		}
		else
		{
			
		}
	}

	//===================================================================================
	// Cập nhật lại title window                                                         
	//===================================================================================
	std::wstring GetTitleInfo()
	{
		NULL_RETURN(m_hWnd, L"");

		wchar_t titlebuff[256];

		std::wstring gpu_name = L"Unknown";

		if (m_pProp.m_iModeDraw == 1)
			gpu_name = m_gpu_device_name.c_str();
		else
			gpu_name = this->GetGPUDevice();

		// m_gpu_deive_name updated in render thread
		swprintf_s(titlebuff, L"%s - %d x %d - %s",  m_title.c_str(), m_width, m_height,
			gpu_name.c_str());

		return titlebuff;
	}

	//===================================================================================
	// Cập nhật lại thông số cho text render window                                      
	//===================================================================================
	void UpdateTextRender()
	{
		//m_text_render.UpdateView(m_width, m_height);
	}

	//===================================================================================
	// Load và cập nhật lại text render trên window                                      
	//===================================================================================
	void ReloadTextRender()
	{
		// Setup Text render;
		//m_text_render.Init(this->GetHDC(), m_width, m_height);
		//m_text_render.LoadFont(m_fontNameTextRender, m_fontSizeTextRender);
	}


//======================================================================================
//⮟⮟ Triển khai chức năng hỗ trợ cho window                                            
//======================================================================================
public:

	//===================================================================================
	// Hiển thị text ra window                                                           
	//===================================================================================
	void WriteText(const char* text, int x, int y, float r = 1.0f, float g = 1.0f,
		float b = 1.0f, float a = 1.0f)
	{
		//m_text_render.Use();
		//m_text_render.Write(x, y, text, r, g, b, a);
		//m_text_render.DontUse();
	}

	//===================================================================================
	// Có hiển thị thông tin hệ thống lên window hay không                               
	//===================================================================================
	void WriteSystemInfo(bool bShow = true)
	{
		m_bSysInfo = bShow;
	}

public:
	const Window& operator=(const Window& win) = delete;

	Window(	const wchar_t* title, const int& xpos, const int& ypos,
			const int& width = 640, const int height = 480,
			const WndProp* prop = NULL) : WindowBase()
	{
		this->m_title  = title;
		this->m_x	   = xpos;
		this->m_y	   = ypos;
		this->m_width  = width;
		this->m_height = height;

		// setup default font
		this->m_fontName = L"Arial";
		this->m_fontSize = 12;
		this->m_fontWeight = FontWeight::Normal;

		// openGL information
		this->m_bUseOpenGLEx = false; // it reup when create window and load opengl extension
		this->m_bUpdateRenderInfo = false;
		this->m_iIdUpdate = 0;

		// setup default text render
		this->m_fontNameTextRender = "Consolas";
		this->m_fontSizeTextRender = 16;

		this->m_bSysInfo = false;
		this->m_bClosed = false;

		if (prop)
			SetPropAdvanced(*prop);
	}

	void SetPropAdvanced(const WndProp& prop)
	{
		this->m_pProp = prop;
	}

	void ExitFullScreen()
	{
		m_pProp.m_bFullScreen = false;
		this->UpdateHint();
	}

	void FullScreen()
	{
		m_pProp.m_bFullScreen = true;
		this->UpdateHint();
	}

	int       GetDrawMode() { return m_pProp.m_iModeDraw; }

	//==================================================================================
	// Sử dụng cho trường hợp dùng GDIplus (xóa sau khi sử dụng xong)                   
	//==================================================================================
	WndGDIplus* GetGraphicsFromHWND()
	{
		return Gdiplus::Graphics::FromHWND(m_hWnd);;
	}

	//==================================================================================
	// Lấy thời gian trôi qua từ frame trước sang frame hiện tại                        
	//==================================================================================
	double GetFrameTime()
	{
		return m_fpscounter.frametime();
	}

	//==================================================================================
	// Lấy FPS window                                                                   
	//==================================================================================
	unsigned int GetFPS()
	{
		return m_fpscounter.fps();
	}

//======================================================================================
//⮟⮟ Triển khai chức cập nhật thông tin trạng thái window                              
//======================================================================================
private:
	/***************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void UpdateStyleWindow()
	{
		m_pProp.m_dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE ;  // Window Extended Style
		m_pProp.m_dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_EX_TRANSPARENT;  // Windows Style
		//@@ WS_CLIPCHILDREN: Control của window sẽ không được vẽ khi SwapBuffer

		if (m_pProp.m_bFullScreen)                           // Are We Still In Fullscreen Mode?
		{
			// Window Extended Style
			m_pProp.m_dwExStyle = m_pProp.m_dwExStyle &~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE
				| WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			// Windows Style
			m_pProp.m_dwStyle = m_pProp.m_dwStyle &~(WS_CAPTION | WS_THICKFRAME);
		}

		if (m_bShow == false)
		{
			// dwMyFlags ^= dwSomeFlag; remove flag 
			m_pProp.m_dwStyle &= ~WS_VISIBLE;
		}
	}

	/***************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void UpdateHint()
	{
		if (m_pProp.m_bFullScreen)
		{
			this->PushWindowStatus();

			// Set new window style and size.
			SetWindowLong(m_hWnd, GWL_STYLE, m_pProp.m_dwStyle);
			SetWindowLong(m_hWnd, GWL_EXSTYLE, m_pProp.m_dwExStyle);

			// On expand, if we're given a window_rect, grow to it, otherwise do not resize.
			xMonitorInfo monitor = std::move(GetMonitorInfoEx());
			SetWindowPos(m_hWnd, NULL, 0, 0, monitor.WIDTH, monitor.HEIGHT,
								SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			// Update width ,height screen
			m_width  = monitor.WIDTH;
			m_height = monitor.HEIGHT;
		}
		else
		{
			if (!IsEmptyStackWindowStatus())
			{
				WindowStatus winState = PopWindowStatus();

				UpdateStyleWindow();

				if (ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL)
				{
					SetWindowLong(m_hWnd, GWL_STYLE, m_pProp.m_dwStyle);
					SetWindowLong(m_hWnd, GWL_EXSTYLE, m_pProp.m_dwExStyle);
					m_x = winState.m_x;
					m_y = winState.m_y;
					m_width = winState.m_width;
					m_height = winState.m_height;

					RECT wr = { 0, 0, m_width, m_height };           // set the size, but not the position
					AdjustWindowRect(&wr, m_pProp.m_dwStyle, FALSE); // adjust the size

					SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_x, m_y,
						(wr.right - wr.left), (wr.bottom - wr.top), SWP_SHOWWINDOW);

					// Update size window after created
					m_width = winState.m_width;
					m_height = winState.m_height;
				}
			}
		}

		if (m_pProp.m_iAphaTrans >= 0)
		{
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
			SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), m_pProp.m_iAphaTrans, LWA_ALPHA);
		}
	}

private:
	void SetUpHint()
	{
		this->UpdateStyleWindow();
	}

	//==================================================================================
	// Hàm hủy Window                                                                   
	//==================================================================================
	virtual void OnDestroy()
	{
		// Gọi hàm mở rộng trước
		if (m_funOnDestroy)
		{
			m_funOnDestroy(this);
		}

		// Xử lý hủy mặc định
		DeleteGDIplus();
		DeleteOpenGLContext();
		DestroyControl();       //N.V.Thuong 22.04.2022
		DestroySubWindow();
	}

private:

	virtual void ThreadDrawOpenGL()
	{
		// if I create opengl context in main thread then this thread not active
		this->CreateOpenGLContext();

		// Setup Text render;
		this->ReloadTextRender();
		
		while (!this->closed())
		{
			this->OnDraw();
			//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	virtual void ThreadProcessOpenGL()
	{
		while (!this->closed())
		{
			this->OnProcess(this);
		}
	}

public:

	// function draw opengl : makecontext + swapbuffer
	void draw()
	{
		if (m_pProp.m_iModeDraw == 1)
		{
			if (!m_drawthread.is_detach())
			{
				m_drawthread.detach();
			}
		}
		else
		{
			this->OnDraw();
		}
	}

	void process()
	{
		if (m_pProp.m_iModeDraw == 1)
		{
			if (!m_processthread.is_detach())
			{
				m_processthread.detach();
			}
		}
		else
		{
			this->OnProcess(this);
		}
	}

	void poll_event()
	{
		// Xử lý trong trường hợp vẽ liên tục
		// Check to see if any messages are waiting in the queue
		while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate the message and dispatch it to WindowProc()
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}

	void wait_event()
	{
		// Xử lý trong trường hợp vẽ theo sự kiện
		if (GetMessage(&m_msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}

	bool closed()
	{
		return m_bClosed;
	}

	void close()
	{
		PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
	}

	friend int     init_window();
	friend Window* create_window(const wchar_t* , int , int , int , int, const WndProp*);
	friend Window* create_window(Window*);
};



____END_NAMESPACE____


#endif // !WHANLDE_H

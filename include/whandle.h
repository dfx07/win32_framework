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


#define   GL_PRESSED    1
#define   GL_RELEASE    0

___BEGIN_NAMESPACE___

/// ////////////////////////////////////////////////////////////////////////////////
/// GDIplusToken struct - WINDOW
/// Provides a container GDI data
/// ////////////////////////////////////////////////////////////////////////////////

//Không sử dụng Graphics vì nó bị đỏ khi Bug khó chịu @^@
typedef Gdiplus::Graphics                   WndGDIplus;

enum GLKeyboard
{
    AltLeft         = 0x12,      // (18)
    AltRight        = 0x12,      // (18)
    CapsLock        = 0x14,      // (20)
    ControlLeft     = 0x11,      // (17)
    ControlRight    = 0x11,      // (17)
    OSLeft          = 0x5B,      // (91)
    OSRight         = 0x5C,      // (92)
    ShiftLeft       = 0x10,      // (16)
    ShiftRight      = 0x10,      // (16)
    Delete          = 0x2E,      // (46)
    Escapex         = 0x1B,      // (27)

    Digit1          = 0x31,      // (49)
    Digit2          = 0x32,      // (50)
    Digit3          = 0x33,      // (51)
    Digit4          = 0x34,      // (52)
    Digit5          = 0x35,      // (53)
    Digit6          = 0x36,      // (54)
    Digit7          = 0x37,      // (55)
    Digit8          = 0x38,      // (56)
    Digit9          = 0x39,      // (57)
    Digit0          = 0x30,      // (48)
    KeyA            = 0x41,      // (65)
    KeyB            = 0x42,      // (66)
    KeyC            = 0x43,      // (67)
    KeyD            = 0x44,      // (68)
    KeyE            = 0x45,      // (69)
    KeyF            = 0x46,      // (70)
    KeyG            = 0x47,      // (71)
    KeyH            = 0x48,      // (72)
    KeyI            = 0x49,      // (73)
    KeyJ            = 0x4A,      // (74)
    KeyK            = 0x4B,      // (75)
    KeyL            = 0x4C,      // (76)
    KeyM            = 0x4D,      // (77)
    KeyN            = 0x4E,      // (78)
    KeyO            = 0x4F,      // (79)
    KeyP            = 0x50,      // (80)
    KeyQ            = 0x51,      // (81)
    KeyR            = 0x52,      // (82)
    KeyS            = 0x53,      // (83)
    KeyT            = 0x54,      // (84)
    KeyU            = 0x55,      // (85)
    KeyV            = 0x56,      // (86)
    KeyW            = 0x57,      // (87)
    KeyX            = 0x58,      // (88)
    KeyY            = 0x59,      // (89)
    KeyZ            = 0x5A,      // (90)


    Left            = 0x25,      // (37)   Left arrow.
    Up              = 0x26,      // (38)   Up arrow.
    Right           = 0x27,      // (39)   Right arrow.
    Down            = 0x28,      // (40)   Down arrow.


    F1              = 0x70,      // (112)
    F2              = 0x71,      // (113)
    F3              = 0x72,      // (114)
    F4              = 0x73,      // (115)
    F5              = 0x74,      // (116)
    F6              = 0x75,      // (117)
    F7              = 0x76,      // (118)
    F8              = 0x77,      // (119)
    F9              = 0x78,      // (120)
    F10             = 0x79,      // (121)
    F11             = 0x7A,      // (122)
    F12             = 0x7B,      // (123)
    F13             = 0x7C,      // (124)
    F14             = 0x7D,      // (125)
    F15             = 0x7E,      // (126)
    F16             = 0x7F,      // (127)
    F17             = 0x80,      // (128)
    F18             = 0x81,      // (129)
    F19             = 0x82,      // (130)
    F20             = 0x83,      // (131)
    F21             = 0x84,      // (132)
    F22             = 0x85,      // (133)
    F23             = 0x86,      // (134)
    F24             = 0x87,      // (135)
};

enum GLMouse
{
    LeftButton      = 0x01,      // (1)
    RightButton     = 0x02,      // (2)
    MidButton       = 0x04,      // (4)
};

class Safe_Thread
{
private:
	std::thread		m_thread;
	std::string		m_thread_name;

	bool			m_bcreated;
	bool			m_bdetach;
public:
	
	explicit Safe_Thread() :
		m_thread(),
		m_thread_name("none"),
		m_bcreated(false),
		m_bdetach(false)
	{

	}

	template<class ...Args>
	void create(Args &&...args)
	{
		m_thread =  std::move(std::thread(std::forward<Args>(args)...));
		m_bcreated = true;
	}

	Safe_Thread(Safe_Thread &&other) noexcept
	{
		m_thread = std::move(other.m_thread);
	}

	~Safe_Thread()
	{
		join();
	}

	bool is_created()
	{
		return m_bcreated;
	}

	void setname(const std::string& name)
	{
		m_thread_name = name;
	}

	void join()
	{
		if (m_thread.joinable())
			m_thread.join();
	}

	bool is_detach()
	{
		return m_bdetach;
	}

	void detach()
	{
		if (m_thread.joinable())
		{
			m_thread.detach();
			m_bdetach = true;
		}
	}
};


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

		NULL_CHECK_RETURN(win, DefWindowProc(hWnd, message, wParam, lParam));

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
		NULL_CHECK_RETURN(m_hWnd, L"");

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


//==================================================================================
// Class SubWindow : Thông tin và ngữ cảnh của một handle                           
//==================================================================================
class Dllexport SubWindow : public WindowBase , public WindowEvent
{
protected:
	WndProp				m_pProp;

	GDIplusCtrlRender	m_render;
	HDC					m_hdc;

	// Window control information
	int					m_height_title_bar;
	Gdiplus::Rect		m_rect_title;
	Gdiplus::Rect		m_rect_body;
	Button*				m_btn_minimize;
	Button*				m_btn_close;


	bool				m_move_window;
	POINT				m_last_cursor_pos;

	bool				m_minimize_window;

public:
	SubWindow() : WindowBase()
	{
		m_move_window = false;
	}

	~SubWindow()
	{

	}

//======================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                         
//======================================================================================
protected:
	/***************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void UpdateStyleWindow()
	{
		m_pProp.m_dwExStyle = 0 ;  // Window Extended Style
		m_pProp.m_dwStyle = WS_CHILDWINDOW ;  // Windows Style

		if (m_iShow == false)
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
	void InitProperties()
	{
		m_move_window = false;
	}

public:
	bool CreateWindowHandle(const wchar_t* strWndClassName)
	{
		// Create GDI+ startup incantation
		this->CreateGDIplus();

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
								 m_hWndParent,               //
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

		m_hdc = GetDC(m_hWnd);
		m_render.Init(m_hdc, rect);
		m_render.LoadFont(L"Consolas");

		InitWindowControl();

		return true;
	}

//======================================================================================
//⮟⮟ Xử lý message window                                                              
//======================================================================================
protected:
	int ProcessEventDefault(Control* control)
	{
		NULL_CHECK_RETURN(control, 0);

		if (control == m_btn_minimize)
		{
			PushMessage(SUB_MINI_WIN, SUB_PROCESS_MSG, 0);
			return 1;
		}
		else if (control == m_btn_close)
		{
			PushMessage(SUB_CLOSE_WIN, SUB_PROCESS_MSG, 0);
			return 1;
		}

		return 0;
	}

	int ProcessMessage(UINT msgID, WPARAM wParam, LPARAM lParam)
	{
		switch (msgID)
		{
			case SUB_MOVE_WIN:
			{
				if (this->m_move_window == false)
				{
					std::cout << "1" << std::endl;
					break;
				}
				else
				{
					std::cout << "22" << std::endl;
				}
				

				POINT cursor_pos = GetCursorInScreen();
				POINT move = {	cursor_pos.x - m_last_cursor_pos.x,
								cursor_pos.y - m_last_cursor_pos.y };

				MovePosition(move.x, move.y);

				m_last_cursor_pos = cursor_pos;
				break;
			}

			case SUB_MINI_WIN:
			{
				if (m_minimize_window == true)
				{
					SetSize(m_rect_title.Width, m_rect_title.Height + 2);
				}
				else
				{
					SetSize(m_rect_title.Width, m_rect_title.Height + 2);
				}

				m_minimize_window = !m_minimize_window;
				break;
			}

			case SUB_CLOSE_WIN:
			{
				break;
			}

			default:
				break;
		}

		return 0;
	}

//======================================================================================
//⮟⮟ Triển khai xử lý thông điệp sub window                                            
//======================================================================================
protected:
	static LRESULT CALLBACK WndSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		SubWindow* subwin = (SubWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		NULL_CHECK_RETURN(subwin, DefWindowProc(hWnd, message, wParam, lParam));

		if (wParam == SUB_PROCESS_MSG)
		{
			return subwin->ProcessMessage(message, wParam, lParam);
		}

		switch (message)
		{
		case WM_CLOSE:
		{
			subwin->m_bClosed = true;
			break;
		}
		case WM_DESTROY:
		{
			subwin->m_bClosed = true;
			subwin->OnDestroy();
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			subwin->SetKeyboardStatus((int)wParam, true);
			subwin->OnKeyBoard(subwin);
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			subwin->SetKeyboardStatus((int)wParam, false);
			subwin->OnKeyBoard(subwin);
			break;
		}
		case WM_LBUTTONUP:
		{
			subwin->m_move_window = false;
			subwin->SetMouseButtonStatus(VK_LBUTTON, false);
			subwin->OnMouseButton(subwin, GLMouse::LeftButton, GL_RELEASE);
			break;
		}
		case WM_RBUTTONUP:
		{
			subwin->SetMouseButtonStatus(VK_RBUTTON, false);
			subwin->OnMouseButton(subwin, GLMouse::RightButton, GL_RELEASE);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			// process default
			long x, y;
			if (subwin->GetCursorPos(x, y))
			{
				if (subwin->m_rect_title.Contains(x, y))
				{
					subwin->m_move_window = true;
					subwin->m_last_cursor_pos = subwin->GetCursorInScreen();
				}
			}

			subwin->SetMouseButtonStatus(VK_LBUTTON, true);
			subwin->OnMouseButton(subwin, GLMouse::LeftButton, GL_PRESSED);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			subwin->SetMouseButtonStatus(VK_RBUTTON, true);
			subwin->OnMouseButton(subwin, GLMouse::RightButton, GL_PRESSED);
			break;
		}
		case WM_MOUSEMOVE:
		{
			subwin->OnMouseMove(subwin);
			if (subwin->m_move_window)
			{
				subwin->PushMessage(SUB_MOVE_WIN, SUB_PROCESS_MSG, 0);
			}
			break;
		}
		case WM_SIZE: //Check if the window has been resized
		{
			subwin->m_width  = LOWORD(lParam); // width
			subwin->m_height = HIWORD(lParam); // height
			//subwin->UpdateRenderInfo();

			subwin->UpdateTitle();
			// cannot use opengl context in this tunnel
			subwin->OnResize(subwin);
			
			// Refresh screen when resize window in case one thread
			//if (subwin->GetDrawMode() == 0)
			//	win->OnDraw();

			break;
		}
		case WM_SIZING:
		{
			subwin->UpdateTitle();
			break;
		}
		case WM_MOUSEWHEEL:
		{
			subwin->m_zDeltaScroll = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			subwin->OnMouseScroll(subwin);
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
				if (!subwin->ProcessEventDefault(ctrl))
				{
					ctrl->Event(subwin, wID, wEvt);
				}
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
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(109, 194, 222));
			SetBkColor(hdcStatic, RGB(0, 0, 0));
			return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));
			break;
		}
		case WM_PAINT:
		{
			DefWindowProc(hWnd, message, wParam, lParam);

			subwin->OnPaintDefault();
			subwin->OnPaint(subwin);
			return 0;  //[BUG] always drawing
		}
		case WM_ERASEBKGND:
		{
			//return TRUE;
		}
		default:
		{
			break;
		}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

private:
	fox::Color4			m_col_background;

//======================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                         
//======================================================================================
public:

	void InitWindowControl()
	{
		m_col_background = { 14.f, 14.f, 13.f, 255.f };

		GDIPLUS_DRAW_INFO_PTR pRenderInfo = static_cast<GDIPLUS_DRAW_INFO_PTR>(m_render.RenderInfo());
		NULL_CHECK_RETURN(pRenderInfo, );

		m_height_title_bar = 18;

		m_rect_title = { 1, 1, pRenderInfo->rect.Width - 2, m_height_title_bar };

		m_btn_minimize = new Button();
		m_btn_minimize->SetLabel(L"▼");
		m_btn_minimize->SetPosition(1, 1);
		m_btn_minimize->SetSize(m_height_title_bar, m_height_title_bar);
		m_btn_minimize->SetBackgroundColor(m_col_background);

		this->AddControl(m_btn_minimize);

		m_btn_close = new Button();
		m_btn_close->SetLabel(L"x");
		m_btn_close->SetPosition(pRenderInfo->rect.Width - (m_height_title_bar), 1);
		m_btn_close->SetSize(m_height_title_bar, m_height_title_bar);
		m_btn_close->SetBackgroundColor(m_col_background);

		this->AddControl(m_btn_close);
	}

	void OnPaintDefault()
	{
		GDIPLUS_DRAW_INFO_PTR pRenderInfo = static_cast<GDIPLUS_DRAW_INFO_PTR>(m_render.RenderInfo());
		NULL_CHECK_RETURN(pRenderInfo, );

		// 0 > Draw background color
		Gdiplus::Pen pen_back(Gdiplus::Color(255, 255, 255), 1);
		Gdiplus::SolidBrush brush_back(Gdiplus::Color(14, 14, 13));
		m_render.DrawRectangleFull(&pen_back, &brush_back, 0);

		// 1 > Draw title bar
		Gdiplus::Pen pen_title(Gdiplus::Color(28, 27, 27), 1);
		Gdiplus::SolidBrush brush_title(Gdiplus::Color(10, 10, 9));
		m_render.DrawRectangle(m_rect_title, &pen_title, &brush_title, 0);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::SolidBrush text_normal_color(Gdiplus::Color(255, 255, 255));
		m_render.DrawTextRect(m_rect_title, m_title.c_str(), &text_normal_color, &format, Gdiplus::PointF(m_height_title_bar + 1, 0));

		m_render.Flush();
	}

//======================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                         
//======================================================================================
public:
	virtual WindowType GetType() { return WindowType::SubWin; }

	//==================================================================================
	// Khởi tạo window và thiết lập thông số                                            
	//==================================================================================
	bool Create(const wchar_t* strClassname)
	{
		bool ret = true;

		this->UpdateStyleWindow();

		// Create a window HWND use class name
		ret &= this->CreateWindowHandle(strClassname);

		if (ret) // it OK
		{
			//// Update get style window
			//this->UpdateHint();

			//// Active function user custom
			//this->OnCreated();

			//// Update title after created ok
			//this->UpdateTitle();

			//// Initialization control
			this->OnInitControl();

			//// Update font control after initialization control
			//this->UpdateFont();

			//// Update and setup properties when everything is done
			this->InitProperties();
		}

		if (ret)
		{
			if (m_iShow)
			{
				this->Show();
			}
			else
			{
				this->Hide();
			}
		}

		return ret;
	}
	

	virtual void OnDestroy()
	{
		// Gọi hàm mở rộng trước
		CHECK_RUN_FUNCTION(m_funOnDestroy, this);

		// Xử lý hủy mặc định
		DeleteGDIplus();
		//DeleteOpenGLContext();
		DestroyControl();       //N.V.Thuong 22.04.2022
		DestroySubWindow();
	}

	friend int     init_window();
	friend Window* create_window(const wchar_t*, int, int, int, int, const WndProp*);
	friend Window* create_window(Window*);
};
____END_NAMESPACE____


#endif // !WHANLDE_H

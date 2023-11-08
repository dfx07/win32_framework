////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xwindow.h
* @create   Nov 15, 2022
* @brief    Create and handle event window handle
* @note     For conditions of distribution and use, see copyright notice in readme.txt
***********************************************************************************/
#ifndef XWINDOW_H
#define XWINDOW_H

#include "xsystype.h"
#include "xwinbase.h"
#include "xgpwinctx.h"

#include "xgpwintext_render.h"
#include <vector>
#include <stack>
#include <mutex>

___BEGIN_NAMESPACE___

class Camera;

/***********************************************************************************
* ⮟⮟ Class name: Window
* Thông tin và ngữ cảnh của một handle
***********************************************************************************/
class Dllexport Window : public WindowBase, public gp::WindowOpenGLContext, public WindowEvent
{
protected:

	static bool register_class(bool bSet = false, bool value = false)
	{
		static bool m_bRegisterClass = false;
		if (bSet == true)
		{
			m_bRegisterClass = value;
		}

		return m_bRegisterClass;
	}

	static bool opengl_extension(bool bSet = false, bool value = false)
	{
		static bool m_bLoadOpenGLEx = false;
		if (bSet == true)
		{
			m_bLoadOpenGLEx = value;
		}

		return m_bLoadOpenGLEx;
	}

private:
	std::wstring			m_title;

	MSG						m_msg;

private:
	// System information : fps -frametime
	CFPSCounter				m_fpscounter;

	// Text render 2D
	gp::GLWinFontRender		m_text_render;
	const char*				m_fontNameTextRender;
	unsigned int			m_fontSizeTextRender;

	// System draw infor
	bool					m_bOpenGLCreated; // update view , text render info :

	std::mutex				m_renderinfo_mutex;
	std::condition_variable		m_sycn_renderinfo;

	CSafeThread				m_drawthread;

	int						m_iDrawMode; // 0 : normal | 1 : use draw thread
	std::wstring			m_gpu_device_name;

	// Font information
	std::wstring			m_fontName;
	unsigned int			m_fontSize;


	// Camera
	Camera*					m_pCamera;

private:
//==================================================================================
//⮟⮟ Triển khai xử lý thông điệp cho window                                        
//==================================================================================
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
			for (int i = 0; i < win->m_ChildWindowList.size(); i++)
			{
				win->m_ChildWindowList[i]->Send_Message(static_cast<UINT>(MOVE_WIN), 
					static_cast<WPARAM>(PROCESS_MSG), 0);
			}

			win->OnMouseMove(win);
			break;
		}
		case WM_SIZE: //Check if the window has been resized
		{
			win->m_CurStatus.m_rect.width  = LOWORD(lParam); // width
			win->m_CurStatus.m_rect.height = HIWORD(lParam); // height

			win->m_text_render.UpdateView(win->m_CurStatus.m_rect.width, win->m_CurStatus.m_rect.height);

			win->UpdateTitle();

			// Cannot use OpenGL context in this tunnel
			win->OnResize(win);
			
			// Refresh screen when resize window in case one thread
			if (win->m_iDrawMode == 0)
			{
				win->OnDraw();
			}

			break;
		}
		case WM_SIZING:
		{
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
			NULL_BREAK(hwndControl);

			auto pControl = (ControlBase*)(GetWindowLongPtr(hwndControl, GWLP_USERDATA));
			NULL_BREAK(pControl);

			pControl->OnCommand(win, wID, wEvt);

			break;
		}
		case WM_DRAWITEM:
		{
			WORD wID = LOWORD(wParam);
			LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
			auto pControl = (ControlBase*)(GetWindowLongPtr(pdis->hwndItem, GWLP_USERDATA));
			NULL_BREAK(pControl);

			//pControl->Draw(win->m);

			break;
		}
		case WM_CTLCOLORSTATIC: //In order to make those edges invisble when we use RoundRect(),
		case WM_CTLCOLORBTN:    //we make the color of our button's background match window's background
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			HWND hwndControl = (HWND)lParam;

			auto pControl = (ControlBase*)(GetWindowLongPtr(hwndControl, GWLP_USERDATA));
			//if (ctrl && ctrl->GetType() == ControlType::LABEL)
			//{
			//	Label* lab = static_cast<Label*>(ctrl);
			//	lab->UpdateTextColor((HDC)wParam);
			//}

			return (INT_PTR)GetStockObject((HOLLOW_BRUSH));
		}
		case WM_PAINT:
		{
			win->OnPaint(win);
			break;
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
	/*******************************************************************************
	*! @brief  : Get name gpu device - only use when created opengl context
	*! @return : std::wstring : device name
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual std::wstring GetNameGPUDevice()
	{
		if (m_bOpenGLCreated == false)
			return L"OpenGL context not created";

		std::string gpu_device = "No Device";
		if ((char*)glGetString(GL_RENDERER))
		{
			gpu_device = (char*)glGetString(GL_RENDERER);
		}

		std::wstring utf16_gpu_device;
		utf16_gpu_device.resize(gpu_device.size() + 1, 0);

		int nWide = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			gpu_device.c_str(), (int)gpu_device.length(),
			(LPWSTR)utf16_gpu_device.c_str(), (int)utf16_gpu_device.size());

		utf16_gpu_device.resize(nWide);

		return utf16_gpu_device;
	}

	/*******************************************************************************
	*! @brief  : update title follow format
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void UpdateTitle()
	{
		std::wstring title = GetTitleInfo();
		SetTitle(title.c_str());
	}

	/*******************************************************************************
	*! @brief  : update title follow format
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void UpdateWindowDraw()
	{
		// Hiển thị thông tin fps - frametime và thông tin hệ thống nếu cần
		if (m_Setting.m_bWriteInfo == true)
		{
			char strsys[80];
			long x, y;
			if (GetCursorPosCenter(x, y))
			{
				sprintf_s(strsys, "fps: %d - frametime : %f ms - mouse : %d:%d", GetFPS(), GetFrameTime(), x, y);
			}
			else
			{
				sprintf_s(strsys, "fps: %d - frametime : %f ms", GetFPS(), GetFrameTime());
			}
			WriteText(strsys, 10, 20);
		}
	}

private:
	/*******************************************************************************
	*! @brief  : draw function window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void OnDraw()
	{
		if (!MakeContext()) return;

		glViewport(0, 0, m_CurStatus.m_rect.width, m_CurStatus.m_rect.height);

		this->UpdateInfo();

		CHECK_RUN_FUNCTION(m_funOnDraw, this);

		this->UpdateWindowDraw();

		this->SwapBuffer();
	}

protected:

	virtual WindowType GetType() { return WindowType::MainWin; }

//==================================================================================
//⮟⮟ Triển khai chính tạo và xử lý ngữ cảnh window  - important                    
//==================================================================================
public:
	void SetCamera(Camera* pCam)
	{
		m_pCamera = pCam;
	}

	void* GetCamera()
	{
		return static_cast<void*>(m_pCamera);
	}

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetWidth() { return m_CurStatus.m_rect.width; }

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetHeight() { return m_CurStatus.m_rect.height; }

//==================================================================================
//⮟⮟ Triển khai khởi tạo liên quan đến window                                      
//==================================================================================
private:

	/*******************************************************************************
	*! @brief  : Hàm khỏi tạo style window trước khi tạo
	*! @return : bool
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool CreateStyleWindow()
	{
		m_dwStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE ;  // Window Extended Style
		m_dwStyle   = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_EX_TRANSPARENT;  // Windows Style
		//@@ WS_CLIPCHILDREN: Control của window sẽ không được vẽ khi SwapBuffer

		if (m_Setting.m_bFullScreen)  // Are We Still In Fullscreen Mode?
		{
			m_dwStyleEx = m_dwStyleEx &~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE // Window Extended Style
				| WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			
			m_dwStyle = m_dwStyle &~(WS_CAPTION | WS_THICKFRAME); // Windows Style
		}

		if (m_bVisble == false)
		{
			// dwMyFlags ^= dwSomeFlag; remove flag 
			m_dwStyle &= ~WS_VISIBLE;
		}

		return true;
	}

	/*******************************************************************************
	*! @brief  : Xử lý trước khi khởi tạo window
	*! @return : bool
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool HandleBeforeCreateWindow()
	{
		return true;
	}

	/*******************************************************************************
	*! @brief  : Xử lý sau khi tạo window thành công
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool HandleAfterCreateWindow()
	{
		if (m_Setting.m_bFullScreen)
		{
			if (PushWindowStatus() > 0)
			{
				// Set new window style and size.
				this->UpdateStyle();

				// On expand, if we're given a window_rect, grow to it, otherwise do not resize.
				xMonitorInfo monitor = std::move(GetMonitorInfoEx());
				SetWindowPos(m_hWnd, NULL, 0, 0, monitor.WIDTH, monitor.HEIGHT,
					SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

				// Update width ,height screen
				m_rect.width = monitor.WIDTH;
				m_rect.height = monitor.HEIGHT;
			}
		}
		else
		{
			if (!IsEmptyStackWindowStatus())
			{
				WindowStatus winState = PopWindowStatus();

				CreateStyleWindow();

				if (ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL)
				{
					this->UpdateStyle();

					m_CurStatus = winState;

					// set the size, but not the position
					RECT wr = { 0, 0, (LONG)m_CurStatus.m_rect.width, (LONG)m_CurStatus.m_rect.height };
					AdjustWindowRect(&wr, m_dwStyle, FALSE); // adjust the size

					SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_CurStatus.m_rect.x, m_CurStatus.m_rect.y,
						(wr.right - wr.left), (wr.bottom - wr.top), SWP_SHOWWINDOW);
				}
			}
		}

		if (m_Setting.m_iAphaTrans >= 0)
		{
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
			SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), m_Setting.m_iAphaTrans, LWA_ALPHA);
		}

		return true;
	}

	/*******************************************************************************
	*! @brief  : Hàm tạo window handle và thông số
	*! @param  : [In] strWndClassName : tên class register
	*! @return : bool 
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool CreateHandle(const wchar_t* strWndClassName)
	{
		// Create GDI+ startup incantation
		if (m_Setting.m_bGDIplus)
		{
			this->CreateGDIplus();
		}

		// Kích thức thực tế của vùng có thể vẽ
		RECT wr = { 0, 0, (LONG)m_CurStatus.m_rect.width, (LONG)m_CurStatus.m_rect.height };
		AdjustWindowRect(&wr, m_dwStyle, FALSE);// adjust the size

		m_hWnd = CreateWindowEx( m_dwStyleEx,                //
								 strWndClassName,            //
								 m_CurStatus.m_title.c_str(),//
								 m_dwStyle,                  //
								 m_CurStatus.m_rect.x,       //
								 m_CurStatus.m_rect.y,       // Postion 
								 wr.right - wr.left,         // Actual width size
								 wr.bottom - wr.top,         // Actual height size
								 m_hWndPar,                  //
								 NULL,                       //
								 NULL,                       //
								 NULL                        //
		);

		// Create window failed or associate failed
		if (!m_hWnd)
		{
			this->DeleteGDIplus();
			return false;
		}
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

		// Update size window after created
		RECT rect;
		if (GetClientRect(m_hWnd, &rect))
		{
			m_CurStatus.m_rect.width  = rect.right - rect.left;
			m_CurStatus.m_rect.height = rect.bottom - rect.top;
		}

		this->UpdateTitle();

		// Khởi tạo người dùng định nghĩa - hãy để nó cuối cùng
		this->OnCreated(this);

		return true;
	}

	/*******************************************************************************
	*! @brief  : Phá hủy window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void DestroyHandle()
	{
		// Xử lý hủy mặc định
		DeleteGDIplus();

	}

	/*******************************************************************************
	*! @brief  : Hàm hủy người dùng định nghĩa 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void DestroyCustom()
	{
		CHECK_RUN_FUNCTION(m_funOnDestroy, this);
	}

//==================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                     
//==================================================================================
protected:

	/*******************************************************************************
	*! @brief  : Khởi tạo ngữ cảnh OpenGL - sử dụng cho one hoặc nhiều thread
	*! @param  : void
	*! @return : bool - true :ok | false : failed
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	bool CreateOpenGLContext()
	{
		m_iDrawMode = m_Setting.m_iModeDraw;

		// Any given OpenGL rendering context can be active at only one thread at a time.
		// if I create opengl context in main thread then this thread not active
		if (m_iDrawMode == 1)
		{
			if (!m_drawthread.IsCreated())
			{
				// Context not created in main thread, so draw not main thread
				std::unique_lock< std::mutex> lock(m_renderinfo_mutex);
				m_drawthread.Create(&Window::CreateOpenGLContextThread, this);
				m_drawthread.Detach();

				// wait for create OpenGL ok
				while (!m_bOpenGLCreated)
				{
					m_sycn_renderinfo.wait(lock);
					break;
				}
			}
		}
		// Create OpenGL main thread
		else
		{
			m_bOpenGLCreated = false;

			bool bUseOpenGLEx = opengl_extension();

			if (InitContext(m_hWnd, bUseOpenGLEx) && CreateContext(m_Setting.m_iAntialiasing))
			{
				m_bOpenGLCreated = true;
				m_gpu_device_name = GetNameGPUDevice();
				this->ReloadTextRender();
			}
		}

		return m_bOpenGLCreated;
	}

	/*******************************************************************************
	*! @brief  : Khởi tạo OpenGL context trong một thread
	*! @param  : void
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void CreateOpenGLContextThread()
	{
		m_bOpenGLCreated = false;

		bool bUseOpenGLEx = opengl_extension();

		if (InitContext(m_hWnd, bUseOpenGLEx) && CreateContext(m_Setting.m_iAntialiasing))
		{
			m_bOpenGLCreated = true;
			m_gpu_device_name = GetNameGPUDevice();
			this->ReloadTextRender();
		}

		m_sycn_renderinfo.notify_all();

		// Draw not main thread
		while (!this->closed())
		{
			this->OnDraw();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}

//==================================================================================
//⮟⮟ Triển khai cập nhật trạng thái của window                                     
//==================================================================================
private:
	//==============================================================================
	// Cập nhật trạng thái thời gian mỗi khi một frame trôi qua                     
	//==============================================================================
	void UpdateInfo()
	{
		m_fpscounter.update();
	}

	//==============================================================================
	// Khởi tạo trạng thái và thông tin thuộc tính khi đã tạo windows xong          
	//==============================================================================
	void InitProperties()
	{
		m_fpscounter.start();
	}

	/*******************************************************************************
	*! @brief  : Cập nhật lại title window
	*! @param  : void
	*! @return : bool - true :ok | false : failed
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	std::wstring GetTitleInfo()
	{
		NULL_RETURN(m_hWnd, L"");

		wchar_t titlebuff[256];

		// m_gpu_deive_name updated in render thread
		swprintf_s(titlebuff, L"%s - %d x %d - %s", m_title.c_str(), m_CurStatus.m_rect.width,
			m_CurStatus.m_rect.height, m_gpu_device_name.c_str());

		return titlebuff;
	}

	//==============================================================================
	// Load và cập nhật lại text render trên window                                 
	//==============================================================================
	void ReloadTextRender()
	{
		UINT width = m_CurStatus.m_rect.width;
		UINT height = m_CurStatus.m_rect.height;

		m_text_render.Create(GetHwnd(), width, height);
		m_text_render.LoadFont(m_fontNameTextRender, m_fontSizeTextRender);
	}


//==================================================================================
//⮟⮟ Triển khai chức năng hỗ trợ cho window                                        
//==================================================================================
public:

	//==============================================================================
	// Hiển thị text ra window                                                      
	//==============================================================================
	void WriteText(const char* text, int x, int y, float r = 1.0f, float g = 1.0f,
		float b = 1.0f, float a = 1.0f)
	{
		m_text_render.Use();
		m_text_render.Write(x, y, text, r, g, b, a);
		m_text_render.DontUse();
	}

public:
	const Window& operator=(const Window& win) = delete;

	Window(	const wchar_t* title, const int& xpos, const int& ypos,
								  const int& width = 640, const int height = 480,
								  const WindowSetting* setting = NULL) 
		: WindowBase()
	{
		m_title						= title;
		m_CurStatus.m_title			= title;
		m_CurStatus.m_rect.x		= xpos;
		m_CurStatus.m_rect.y		= ypos;
		m_CurStatus.m_rect.width	= width;
		m_CurStatus.m_rect.height	= height;

		// openGL information
		this->m_bOpenGLCreated = false;

		// setup default text render
		this->m_fontNameTextRender = "Consolas";
		this->m_fontSizeTextRender = 16;

		this->m_bClosed = false;

		if (setting)
		{
			Setting(*setting);
		}
	}

	void Setting(const WindowSetting& setting)
	{
		this->m_Setting = setting;
	}

	void ExitFullScreen()
	{
		this->CreateStyleWindow();
		this->UpdateStyle();
	}

	void FullScreen()
	{
		this->CreateStyleWindow();
		this->UpdateStyle();
	}

	//======================================================================================
	// Lấy thời gian trôi qua từ frame trước sang frame hiện tại                        
	//======================================================================================
	double GetFrameTime()
	{
		return m_fpscounter.frametime();
	}

	//======================================================================================
	// Lấy FPS window                                                                   
	//======================================================================================
	unsigned int GetFPS()
	{
		return m_fpscounter.fps();
	}

//==================================================================================
//⮟⮟ Triển khai chức cập nhật thông tin trạng thái window                          
//==================================================================================
private:


private:
	void SetUpHint()
	{
		this->CreateStyleWindow();
	}

	//======================================================================================
	// Hàm hủy Window                                                                   
	//======================================================================================
	virtual void OnDestroy()
	{
		// Gọi hàm mở rộng trước
		if (m_funOnDestroy)
		{
			m_funOnDestroy(this);
		}

		// Xử lý hủy mặc định
		DeleteGDIplus();
		DeleteContext();
		DestroyControl();       //N.V.Thuong 22.04.2022
		DestroySubWindow();
	}

private:

public:

	// function draw opengl : makecontext + swapbuffer
	void draw()
	{
		if (m_Setting.m_iModeDraw == 1)
		{
			if (!m_drawthread.IsDetach())
			{
				m_drawthread.Detach();
			}
		}
		else
		{
			this->OnDraw();
		}
	}

	void process()
	{
		this->OnProcess(this);
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
		Send_Message(WM_CLOSE, NULL, NULL);
	}

	friend int     init_window();
	friend Window* create_window(const wchar_t* , int , int , int , int, const WindowSetting*);
	friend Window* create_window(Window*);
};


____END_NAMESPACE____

#endif // !XWINDOW_H
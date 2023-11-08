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

#include "wctrl.h"
#include "xcontrolbase.h"

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
	typedef std::vector<ControlBase*>	ControlList;
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
	virtual int AddControl(ControlBase* control)
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
////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xsubwindow.h
* @create   Nov 15, 2022
* @brief    Create and handle event sub window handle
* @note     For conditions of distribution and use, see copyright notice in readme.txt
***********************************************************************************/
#ifndef XSUBWINDOW_H
#define XSUBWINDOW_H

#include "xsystype.h"
#include "xwinbase.h"
#include "controls/xbutton.h"
#include <controls/xcombobox.h>

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: SubWindow
* Thông tin và ngữ cảnh của một handle
***********************************************************************************/
class Dllexport SubWindow : public WindowBase , public WindowEvent
{
protected:
	enum ControlID
	{
		BEGIN_CONTROL_ID = 4000,
		IDC_BTN_MINIMIZE = BEGIN_CONTROL_ID + 0x001,
		IDC_BTN_CLOSE    = BEGIN_CONTROL_ID + 0x002,
	};

protected:
	GDIplusCtrlRender	m_render;
	HDC					m_hdc;


	// Window control information
	int					m_height_title_bar;
	Gdiplus::Rect		m_rect_title;
	Gdiplus::Rect		m_rect_body;

	bool				m_move_window;
	POINT				m_last_cursor_pos;

	bool				m_minimize_window;

public:
	SubWindow() : WindowBase(), m_hdc(NULL),
		m_height_title_bar(0),
		m_minimize_window(false),
		m_move_window(false)
	{
		m_last_cursor_pos = { 0, 0 };
	}

	~SubWindow()
	{

	}

//==================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                     
//==================================================================================
protected:
	/*******************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void InitProperties()
	{
		m_move_window = false;
	}

public:
	/*******************************************************************************
	*! @brief  : Hàm khỏi tạo style window trước khi tạo
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool CreateStyleWindow()
	{
		m_dwStyleEx = 0;  // Window Extended Style
		m_dwStyle = WS_CHILDWINDOW | WS_VISIBLE;  // Windows Style

		if (m_bVisble == false)
		{
			// dwMyFlags ^= dwSomeFlag; remove flag 
			m_dwStyle &= ~WS_VISIBLE;
		}
		return true;
	}

	/*******************************************************************************
	*! @brief  : Hàm ảo tạo và hủy cửa sổ
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool HandleBeforeCreateWindow()
	{
		return true;
	}

	virtual bool HandleAfterCreateWindow()
	{
		return true;
	}

	/*******************************************************************************
	*! @brief  : Khởi tạo window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual bool CreateHandle(const wchar_t* strWndClassName)
	{
		// Create GDI+ startup incantation
		this->CreateGDIplus();

		// Kích thức thực tế của vùng có thể vẽ
		RECT wr = { 0, 0, (LONG)m_CurStatus.m_rect.width, (LONG)m_CurStatus.m_rect.height };
		AdjustWindowRect(&wr, m_dwStyle, FALSE);// adjust the size

		m_hWnd = CreateWindowEx( m_dwStyleEx,                //
								 strWndClassName,            //
								 m_CurStatus.m_title.c_str(),//
								 m_dwStyle,                  //
								 m_CurStatus.m_rect.x,       // Postion X
								 m_CurStatus.m_rect.y,       // Postion Y
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

		if (m_Setting.m_iAphaTrans > 0)
		{
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
			SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 60, LWA_ALPHA);
		}

		// Update size window after created
		RECT rect;
		if (GetClientRect(m_hWnd, &rect))
		{
			m_CurStatus.m_rect.width  = rect.right - rect.left;
			m_CurStatus.m_rect.height = rect.bottom - rect.top;
		}

		// Init Gdiplus render control
		m_hdc = GetDC(m_hWnd);
		m_render.Init(m_hdc, rect);
		m_render.LoadFont(L"Consolas");

		return true;
	}

	/*******************************************************************************
	*! @brief  : Phá hủy window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void DestroyHandle()
	{
		ReleaseDC(m_hWnd, m_hdc);

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
//⮟⮟ Xử lý message window                                                          
//==================================================================================
protected:

	/*******************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	int ProcessEventDefault(ControlBase* pControl)
	{
		NULL_RETURN(pControl, 0);

		auto uiID = pControl->GetID();

		switch (uiID)
		{
			case IDC_BTN_MINIMIZE:
				Send_Message(MINI_WIN, PROCESS_MSG, 0);
				break;
			case IDC_BTN_CLOSE:
				Send_Message(CLOSE_WIN, PROCESS_MSG, 0);
				break;
			default:
				return 0;
		}

		return 1;
	}

	/*******************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	int OnProcessMessage(UINT msgID, WPARAM wParam, LPARAM lParam)
	{
		switch (msgID)
		{
			case MOVE_WIN:
			{
				if (this->m_move_window == false)
					break;

				POINT cursor_pos = get_cursor_in_screen();
				POINT move = {cursor_pos.x - m_last_cursor_pos.x,
							  cursor_pos.y - m_last_cursor_pos.y};

				MovePosition(move.x, move.y);

				m_last_cursor_pos = cursor_pos;

				break;
			}

			case MINI_WIN:
			{
				m_minimize_window = !m_minimize_window;

				if (m_minimize_window)
				{
					PushWindowStatus(tagWndStatus::PushType::SIZE);
					SetSize(m_rect_title.Width + 4, m_rect_title.Height + 2);
				}
				else
				{
					if (!IsEmptyStackWindowStatus())
					{
						WindowStatus status = PopWindowStatus();
						SetSize(status.m_rect.width, status.m_rect.height);
					}
				}
				break;
			}

			case CLOSE_WIN:
			{
				this->Close();
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
	static LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		SubWindow* subwin = (SubWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		NULL_RETURN(subwin, DefWindowProc(hWnd, message, wParam, lParam));

		if (wParam == PROCESS_MSG)
		{
			return subwin->OnProcessMessage(message, wParam, lParam);
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
			subwin->Destroy();
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
			if (subwin->m_move_window)
			{
				subwin->m_move_window = false;
				InvalidateRect(subwin->GetHwnd(), NULL, FALSE);
			}
			
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
					subwin->m_last_cursor_pos = subwin->get_cursor_in_screen();
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
				subwin->Send_Message(MOVE_WIN, PROCESS_MSG, 0);
			}
			break;
		}
		case WM_SIZE: //Check if the window has been resized
		{
			subwin->m_CurStatus.m_rect.width  = LOWORD(lParam); // width
			subwin->m_CurStatus.m_rect.height = HIWORD(lParam); // height

			// cannot use opengl context in this tunnel
			subwin->OnResize(subwin);

			break;
		}
		case WM_SIZING:
		{
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
				auto pControl = (ControlBase*)(GetWindowLongPtr(hwndControl, GWLP_USERDATA));
				if (!subwin->ProcessEventDefault(pControl))
				{
					pControl->OnCommand(subwin, wID, wEvt);
				}
			}
			break;
		}
		case WM_DRAWITEM:
		{
			WORD wID = LOWORD(wParam); // item, control, or accelerator identifier
			LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
			auto pControl = (ControlBase*)(GetWindowLongPtr(pdis->hwndItem, GWLP_USERDATA));
			if (pControl) pControl->Draw(pdis);
			return TRUE;
		}
		case WM_MEASUREITEM:
		{
			LPMEASUREITEMSTRUCT DrawMenuSize = (LPMEASUREITEMSTRUCT)lParam;

			for (int i = 0; i < subwin->m_ControlList.size(); i++)
			{
				if (subwin->m_ControlList[i]->GetID() == DrawMenuSize->CtlID)
				{
					Combobox* pControl = (Combobox*)subwin->m_ControlList[i];
					return FALSE;
				}
			}
			

			break;
		}
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN:
		{
			return TRUE;
		}
		case WM_PAINT:
		{
			subwin->OnPaintDefault();
			subwin->OnPaint(subwin);
			break;
		}
		//case WM_ERASEBKGND:
		//{
		//	return TRUE;
		//}
		default:
		{
			break;
		}
		}
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

private:
	fox::Color4			m_background_color;
	fox::Color4			m_title_background_color;

//==================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                     
//==================================================================================
public:
	/*******************************************************************************
	*! @brief  : Tạo ra các control mặc định và thêm chúng vào
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void OnCreateControl()
	{
		m_background_color = { 24, 26, 27, 255.f };
		m_title_background_color = { 14, 14, 13, 255 };

		GDIPLUS_DRAW_INFO_PTR pRenderInfo = static_cast<GDIPLUS_DRAW_INFO_PTR>(m_render.RenderInfo());
		NULL_RETURN(pRenderInfo, );

		m_height_title_bar = 18;

		m_rect_title = { 1, 1, pRenderInfo->rect.Width -3, m_height_title_bar };

		// Create minimize control
		Button* btn_mini = new Button();
		btn_mini->SetID(IDC_BTN_MINIMIZE);
		btn_mini->SetLabel(L"▼");
		btn_mini->SetPosition(2, 2);
		btn_mini->SetBackgroundColor(m_title_background_color);
		btn_mini->SetSize(m_height_title_bar , m_height_title_bar -1);
		AddControl(btn_mini);

		// Create button control
		Button* btn_close = new Button();
		btn_close->SetID(IDC_BTN_CLOSE);
		btn_close->SetLabel(L"x");
		btn_close->SetBackgroundColor(m_title_background_color);
		btn_close->SetPosition(pRenderInfo->rect.Width - (m_height_title_bar) -2, 2);
		btn_close->SetSize(m_height_title_bar, m_height_title_bar -1);
		AddControl(btn_close);
	}

	void OnPaintDefault()
	{
		GDIPLUS_DRAW_INFO_PTR pRenderInfo = static_cast<GDIPLUS_DRAW_INFO_PTR>(m_render.RenderInfo());
		NULL_RETURN(pRenderInfo, );

		// 0 > Draw background color
		int iRadiusBorder = 0;
		// Fill erase background
		auto rect	 = pRenderInfo->rect;
		m_render.DrawRectangle(rect, NULL, m_background_color.wrefcol, iRadiusBorder);

		// Fill rectangle background;
		rect.Width	-= 1;
		rect.Height -= 1;
		Gdiplus::Pen pen_back(Gdiplus::Color(255, 255, 255), 1);
		m_render.DrawRectangle(rect, &pen_back, nullptr, iRadiusBorder);

		// 1 > Draw title bar
		Gdiplus::SolidBrush brush_title(m_title_background_color.wrefcol);
		m_render.DrawRectangle(m_rect_title, NULL, &brush_title, 0);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::SolidBrush text_normal_color(Gdiplus::Color(255, 255, 255));
		m_render.DrawTextRect(m_rect_title, m_CurStatus.m_title.c_str(), &text_normal_color, &format, Gdiplus::PointF(m_height_title_bar + 1, 0));

		m_render.Flush();
	}

	/*******************************************************************************
	*! @brief  : Khởi tạo màu cho button ảnh hưởng bởi subwwindow
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void SetColorControl(ControlBase* pControl)
	{
		ControlRectUI* pControlRectUI = dynamic_cast<ControlRectUI*>(pControl);

		if (pControlRectUI)
		{
			pControlRectUI->SetEraseBackgroundColor(m_background_color);
		}

		if (pControl->GetType() == ControlType::CHECKBOX)
		{
			pControlRectUI->SetBackgroundColor(m_background_color);
		}
	}

//==================================================================================
//⮟⮟ Triển hàm thao tác từ bên ngoài tác động vào Window class                     
//==================================================================================
public:
	virtual WindowType GetType() { return WindowType::SubWin; }

	/*******************************************************************************
	*! @brief  : Khởi tạo toàn bộ control đã được thêm
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void OnInitControl()
	{
		this->OnCreateControl();

		for (int i = 0; i < m_ControlList.size(); i++)
		{
			if (!m_ControlList[i]->IsCreated())
			{
				int iNextControlID = m_ControlList[i]->Create(m_hWnd, m_uiControlIDs);

				if (iNextControlID == m_uiControlIDs)
				{
					std::cerr << "[!] Init control failed !" << std::endl;
					continue;
				}
				
				SetColorControl(m_ControlList[i]);

				m_uiControlIDs = iNextControlID;
			}
		}
	
		return;
	}

	/*******************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int AddControl(ControlBase* pControl)
	{
		NULL_RETURN(pControl, 0);

		this->SetColorControl(pControl);

		return WindowBase::AddControl(pControl);
	}

	friend int init_window();
};

____END_NAMESPACE____

#endif // !XSUBWINDOW_H
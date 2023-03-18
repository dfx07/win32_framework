////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xbutton.h
* @create   Nov 15, 2022
* @brief    Create and handle event button
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XBUTTON_H
#define XBUTTON_H

#include "xcontrolbase.h"
#include "xeasing.h"

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Button control
* Button control for window
***********************************************************************************/
class Dllexport Button : public ControlBase, public ControlRectUI
{
	enum class BtnState
	{
		Click,
		Normal,
		Hover,
	};

	enum { IDC_EFFECT_X1	  = 12003 };
	enum { WIDTH_DEF		  = 80	  };
	enum { HEIGHT_DEF		  = 25	  };

	enum { TIME_UPDATE_EFFECT = 5	  };

private:
	bool				m_track_leave;
	bool				m_bUseEffect = false;

protected:
	std::wstring		m_sLabel;
	BtnState			m_eState;
	BtnState			m_eOldState;
	Gdiplus::Brush*		m_background_normal;
	Gdiplus::Brush*		m_backgroundclick;
	Gdiplus::Brush*		m_backgroundhover;

	Gdiplus::Bitmap*	m_image;

	EasingEngine		m_easing;

	GDIplusCtrlRender	m_render;


	void(*m_EventFun)(WindowBase* window, Button* btn) = NULL;
	void(*m_EventFunEnter)(WindowBase* window, Button* btn) = NULL;
	void(*m_EventFunLeave)(WindowBase* window, Button* btn) = NULL;

	static WNDPROC& getproc()
	{
		static WNDPROC prevWndProc;
		return prevWndProc;
	}

public:
	Button() : ControlBase(), m_eState(BtnState::Normal), m_sLabel(L""), m_image(NULL),
		m_track_leave(false),
		m_eOldState(BtnState::Normal),
		m_background_normal(NULL),
		m_backgroundclick(NULL),
		m_backgroundhover(NULL)
	{
		m_rect.x = 0;
		m_rect.y = 0;
		m_rect.width = WIDTH_DEF;
		m_rect.height = HEIGHT_DEF;

		m_property.background_color		  = std::move(Color4(59, 91, 179));
		m_property.background_hover_color = std::move(Color4(229, 241, 255));
		m_property.background_click_color = std::move(Color4(201, 224, 247));

		m_property.border_radius = 0;
		m_property.border_width  = 0;
		m_property.text_color = std::move(Color4(255, 255, 255));
		m_property.text_hover_color = std::move(Color4(0, 0, 0));
	}

	~Button()
	{
		delete m_background_normal;
		delete m_backgroundhover;
		delete m_backgroundclick;

		delete m_image;
	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::BUTTON); }
	void	SetLabel(std::wstring lab) { m_sLabel = lab; }

private:

	/*******************************************************************************
	*! @brief  : Init button control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int OnInitControl()
	{
		m_hWnd = (HWND)CreateWindow(L"BUTTON", m_sLabel.c_str(),		// Button text 
						WS_CHILD | WS_VISIBLE | BS_OWNERDRAW  /*| BS_NOTIFY*/,
						(int)m_rect.x,									// x position 
						(int)m_rect.y,									// y position 
						m_rect.width,									// Button width
						m_rect.height,									// Button height
						m_hWndPar,										// Parent window
						(HMENU)(UINT_PTR)m_ID,							// menu.
						(HINSTANCE)GetWindowLongPtr(m_hWndPar, GWLP_HINSTANCE),
						NULL);

		NULL_RETURN(m_hWnd, 0);

		getproc() = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&ButtonProcHandle);
		return 1;
	}

	static void TrackMouse(HWND hwnd)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE; //Type of events to track & trigger.
		tme.dwHoverTime = 1; //How long the mouse has to be in the window to trigger a hover event.
		tme.hwndTrack = hwnd;
		TrackMouseEvent(&tme);
	}

	/*******************************************************************************
	*! @brief  : Hàm xử lý sự kiện cho button window
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	static LRESULT CALLBACK ButtonProcHandle(HWND hwndBtn, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Button* btn = (Button*)::GetWindowLongPtr(hwndBtn, GWLP_USERDATA);
		NULL_RETURN(btn, 0);

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			if (btn->m_eState == BtnState::Click ||
				btn->m_eState == BtnState::Hover)
				break;

			btn->EndX1ThemeEffect();

			btn->SetState(BtnState::Hover);
			if (!btn->m_track_leave)
			{
				TrackMouse(hwndBtn);
				btn->m_track_leave = true;
			}

			InvalidateRect(hwndBtn, NULL, FALSE);
			break;
		}
		case WM_MOUSELEAVE:
		{
			btn->m_track_leave = false;
			btn->SetState(BtnState::Normal, true);
			btn->BeginX1ThemeEffect();
			InvalidateRect(hwndBtn, NULL, FALSE);
			break;
		}
		case WM_TIMER:
		{
			btn->OnTimer(wParam);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			btn->SetState(BtnState::Click);
			break;
		}
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		{
			btn->m_eState = btn->m_eOldState;
			break;
		}
		case WM_ERASEBKGND:
			return TRUE;
			break;
		case WM_CTLCOLORBTN:
		{
			break;
		}
		}
		return CallWindowProc(getproc(), hwndBtn, uMsg, wParam, lParam);
	}

private:
	void BeginX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return;

		::SetTimer(m_hWnd, IDC_EFFECT_X1, TIME_UPDATE_EFFECT, (TIMERPROC)NULL);

		m_easing.AddExec(EaseType::Expo, EaseMode::In, S2MS(1),m_property.background_hover_color.r, m_property.background_color.r);
		m_easing.AddExec(EaseType::Expo, EaseMode::In, S2MS(1),m_property.background_hover_color.g, m_property.background_color.g);
		m_easing.AddExec(EaseType::Expo, EaseMode::In, S2MS(1),m_property.background_hover_color.b, m_property.background_color.b);

		m_easing.Start();

		delete m_background_normal;
		m_background_normal = new Gdiplus::SolidBrush(m_property.background_hover_color.wrefcol);
	}

	bool UpdateX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return true;

		m_easing.Update(TIME_UPDATE_EFFECT);

		int r = static_cast<int>(m_easing.Value(0));
		int g = static_cast<int>(m_easing.Value(1));
		int b = static_cast<int>(m_easing.Value(2));

		delete m_background_normal;
		m_background_normal = new Gdiplus::SolidBrush(Gdiplus::Color(r, g, b));

		return m_easing.IsActive();
	}

	void EndX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return;

		KillTimer(m_hWnd, IDC_EFFECT_X1);

		delete m_background_normal;
		m_background_normal = new Gdiplus::SolidBrush(m_property.background_color.wrefcol);
	}

	virtual void OnTimer(DWORD wParam)
	{
		switch (wParam)
		{
			case IDC_EFFECT_X1:
			{
				if (this->UpdateX1ThemeEffect())
				{
					InvalidateRect(m_hWnd, NULL, FALSE);
					UpdateWindow(m_hWnd);
				}
				else
				{
					EndX1ThemeEffect();
					InvalidateRect(m_hWnd, NULL, FALSE);
				}
				break;
			}
		}
	}

protected:
	void CreateColorButton()
	{
		if (!m_background_normal)
		{
			m_background_normal = new Gdiplus::SolidBrush(Gdiplus::Color(m_property.background_color.wrefcol));
		}
		if (!m_backgroundclick)
		{
			m_backgroundclick = new Gdiplus::SolidBrush(Gdiplus::Color(m_property.background_click_color.wrefcol));
		}
		if (!m_backgroundhover)
		{
			m_backgroundhover = new Gdiplus::SolidBrush(Gdiplus::Color(m_property.background_hover_color.wrefcol));
		}
	}

	void Draw(LPDRAWITEMSTRUCT& pdis)
	{
		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
		m_render.Init(pdis->hDC, pdis->rcItem);
		m_render.LoadFont(L"Segoe UI");
		
		this->CreateColorButton();

		std::cout << "draw" << std::endl;

		// [2] Draw color button state
		const unsigned int iRadius = m_property.border_radius;
		const unsigned int iBorderWidth = m_property.border_width;
		if (m_eState == BtnState::Click)
		{
			Gdiplus::Pen pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
			m_render.DrawRectangleFull(iBorderWidth > 0 ? &pen : NULL, m_backgroundclick, iRadius);
		}
		else if (m_eState == BtnState::Hover)
		{
			Gdiplus::Pen pen(m_property.background_color.wrefcol, iBorderWidth);
			m_render.DrawRectangleFull(iBorderWidth > 0 ? &pen : NULL, m_backgroundhover, iRadius);
		}
		else
		{
			Gdiplus::Pen pen(Gdiplus::Color(255, 255, 255, 255), iBorderWidth);
			m_render.DrawRectangleFull(iBorderWidth > 0 ? &pen : NULL, m_background_normal, iRadius);
		}

		// [2] Draw image
		if (m_image)
		{
			GdiplusEx::ImageFormat imgformat;
			imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
			imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
			m_render.DrawImageFullRect(m_image, &imgformat);
		}

		// [3] Draw text for button
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		if (m_eState == BtnState::Hover)
		{
			Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(m_property.text_hover_color.wrefcol)); // color text normal
			m_render.DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &format);
		}
		else
		{
			Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(m_property.text_color.wrefcol)); // color text normal
			m_render.DrawTextFullRect(this->m_sLabel.c_str(), &normal_textcolor, &format);
		}

		m_render.Flush(true);
	}

public:
	virtual void Event(WindowBase* window, WORD _id, WORD _event)
	{
		NULL_RETURN(m_EventFun, );

		m_EventFun(window, this);
	}

	void SetEvent(void(*mn)(WindowBase*, Button*))
	{
		m_EventFun = mn;
	}

	void SetState(BtnState state, bool free_oldstate = false)
	{
		m_eOldState = (free_oldstate) ? BtnState::Normal : m_eState;

		if ( m_EventFunEnter && state == BtnState::Hover)
		{
			m_EventFunEnter(NULL, this);
		}
		else if (m_EventFunLeave && state == BtnState::Normal)
		{
			m_EventFunLeave(NULL, this);
		}
		m_eState = state;
	}

	void SetEventEnterCallBack(void(*fun)(WindowBase* window, Button* btn))
	{
		this->m_EventFunEnter = fun;
	}
	void SetEventLeaveCallBack(void(*fun)(WindowBase* window, Button* btn))
	{
		this->m_EventFunLeave = fun;
	}

	void UseEffect(bool bUseEffect)
	{
		m_bUseEffect = bUseEffect;
	}
};

____END_NAMESPACE____

#endif // !XBUTTON_H
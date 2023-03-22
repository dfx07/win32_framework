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

	static WNDPROC		sfunButtonWndProc;

protected:
	std::wstring		m_sLabel;
	BtnState			m_eState;
	BtnState			m_eOldState;

	Gdiplus::Bitmap*	m_image;

	EasingEngine		m_easing;
	Gdiplus::Color		m_cur_background_color;

	GDIplusCtrlRender	m_render;

	void(*m_EventFun)(WindowBase* window, Button* btn) = NULL;
	void(*m_EventFunEnter)(WindowBase* window, Button* btn) = NULL;
	void(*m_EventFunLeave)(WindowBase* window, Button* btn) = NULL;

public:
	Button() : ControlBase(), m_eState(BtnState::Normal), m_sLabel(L""), m_image(NULL),
		m_track_leave(false),
		m_eOldState(BtnState::Normal)
	{
		m_rect.x	  = 0;
		m_rect.y	  = 0;
		m_rect.width  = WIDTH_DEF;
		m_rect.height = HEIGHT_DEF;

		this->SetDefaultPropertyUI();
	}

	~Button()
	{
		delete m_image;
	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::BUTTON); }
	void SetLabel(std::wstring lab) { m_sLabel = lab; }

protected:

	virtual void SetDefaultPropertyUI()
	{
		m_property.m_hover_color	= std::move(Color4(255, 255, 255));
		m_property.m_click_color	= std::move(Color4(255, 255, 245));

		m_property.border_radius	= 0;
		m_property.border_width		= 0;
		m_property.text_color		= std::move(Color4(255, 255, 255));
		m_property.text_hover_color = std::move(Color4(0, 0, 0));
	}

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

		sfunButtonWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&ButtonProcHandle);

		m_cur_background_color = m_property.m_background_color.wrefcol;

		m_easing.AddExec(EaseType::Sine, EaseMode::Out, S2MS(0.5), m_property.m_hover_color.r, m_property.m_background_color.r);
		m_easing.AddExec(EaseType::Sine, EaseMode::Out, S2MS(0.5), m_property.m_hover_color.g, m_property.m_background_color.g);
		m_easing.AddExec(EaseType::Sine, EaseMode::Out, S2MS(0.5), m_property.m_hover_color.b, m_property.m_background_color.b);

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
		//case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		{
			btn->m_eState = btn->m_eOldState;
			break;
		}
		case WM_ERASEBKGND:
			//return TRUE;
			break;
		case WM_CTLCOLORBTN:
		{

			break;
		}
		}

		return CallWindowProc(sfunButtonWndProc, hwndBtn, uMsg, wParam, lParam);
	}

private:
	void BeginX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return;

		::SetTimer(m_hWnd, IDC_EFFECT_X1, TIME_UPDATE_EFFECT, (TIMERPROC)NULL);

		m_easing.Reset();
		m_easing.Start();

		m_cur_background_color = m_property.m_hover_color.wrefcol;
	}

	bool UpdateX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return true;

		m_easing.Update(TIME_UPDATE_EFFECT);

		int r = static_cast<int>(m_easing.Value(0));
		int g = static_cast<int>(m_easing.Value(1));
		int b = static_cast<int>(m_easing.Value(2));

		m_cur_background_color = m_cur_background_color.MakeARGB(255, r, g, b);

		return m_easing.IsActive();
	}

	void EndX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return;

		KillTimer(m_hWnd, IDC_EFFECT_X1);

		m_cur_background_color.SetValue(m_property.m_background_color.wrefcol);
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

	void Draw(LPDRAWITEMSTRUCT& pdis)
	{
		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
		m_render.Init(pdis->hDC, pdis->rcItem);
		m_render.LoadFont(L"Segoe UI");

		// [2] Draw color button state
		const unsigned int iRadius      = m_property.border_radius;
		const unsigned int iBorderWidth = m_property.border_width;

		Gdiplus::Brush* background_color = NULL;
		Gdiplus::Pen*	pen_color		 = NULL;

		if (m_eState == BtnState::Click)
		{
			background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
			pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
		}
		else if (m_eState == BtnState::Hover)
		{
			background_color = new Gdiplus::SolidBrush(m_property.m_hover_color.wrefcol);
			pen_color = new Gdiplus::Pen(m_property.m_background_color.wrefcol, iBorderWidth);
		}
		else
		{
			background_color = new Gdiplus::SolidBrush(m_cur_background_color);
			pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 253), iBorderWidth);
		}

		// Fill erase background
		this->DrawEraseBackground(&m_render);

		// Fill rectangle background;
		this->DrawFillBackground(&m_render, background_color);

		// Draw rectangle background;
		this->DrawBorderBackground(&m_render, pen_color);

		SAFE_DELETE(pen_color);
		SAFE_DELETE(background_color);

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
			Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(m_property.text_hover_color.wrefcol));
			m_render.DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &format);
		}
		else if (m_eState == BtnState::Click)
		{
			Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(255, 0, 0));
			m_render.DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &format);
		}
		else
		{
			Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(m_property.text_color.wrefcol));
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

WNDPROC Button::sfunButtonWndProc = NULL;

____END_NAMESPACE____

#endif // !XBUTTON_H
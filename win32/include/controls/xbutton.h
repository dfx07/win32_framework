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
#include "math/xeasing.h"

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Button control
* Button control for window
***********************************************************************************/
class Dllexport Button : public ControlBase, public RectUIControl
{
protected:
	typedef void(*typeFunButtonEvent)   (WindowBase* window, Button* btn);

protected:
	enum class BtnState
	{
		Normal,
		Hover,
		Click,
	};

	enum { IDC_EFFECT_X1	  = 12003 };
	enum { WIDTH_DEF		  = 80	  };
	enum { HEIGHT_DEF		  = 25	  };

	enum { TIME_UPDATE_EFFECT = USER_TIMER_MINIMUM  };

private:
	bool				m_track_leave;
	bool				m_bUseEffect = false;

	static WNDPROC		sfunButtonWndProc;

protected:
	std::wstring		m_sLabel;
	BtnState			m_eState;
	BtnState			m_eOldState;

	//////////////////////////////////////////////////////////////////
	// Effect (animation)
	easing::EasingEngine		m_easing;
	Gdiplus::Color				m_cur_background_color;
	Gdiplus::Color				m_cur_border_color;


	//////////////////////////////////////////////////////////////////
	// Event Function
	typeFunButtonEvent			m_EventFun		 = NULL;
	typeFunButtonEvent			m_EventFunEnter	 = NULL;
	typeFunButtonEvent			m_EventFunLeave	 = NULL;

protected:

	//////////////////////////////////////////////////////////////////
	// Property
	Gdiplus::StringFormat		m_StringFormat;
	GdiplusEx::ImageFormat		m_ImgFormat;
	Gdiplus::Bitmap*			m_image;

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
		UI_Background.bk_color				= std::move(Color(37, 37, 38));
		UI_Background.bk_hover_color		= std::move(Color(66, 66, 68));
		UI_Background.bk_click_color		= std::move(Color(53, 53, 54));

		UI_Background.border_color			= std::move(Color(77, 77, 80));
		UI_Background.border_hover_color	= std::move(Color(66, 166, 254));

		UI_Background.border_radius			= 0;
		UI_Background.border_width			= 0;

		UI_Text.text_color					= std::move(Color(255, 255, 255));
		UI_Text.text_hover_color			= std::move(Color(255, 255, 255));
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

		m_cur_background_color = UI_Background.bk_color.wrefcol;
		m_cur_border_color = UI_Background.border_color.wrefcol;

		easing::EaseType eType = easing::EaseType::Sine;
		easing::EaseMode eMode = easing::EaseMode::In;

		m_easing.AddExec(eType, eMode, S2MS(0.2), UI_Background.bk_color.r, UI_Background.bk_hover_color.r);
		m_easing.AddExec(eType, eMode, S2MS(0.2), UI_Background.bk_color.g, UI_Background.bk_hover_color.g);
		m_easing.AddExec(eType, eMode, S2MS(0.2), UI_Background.bk_color.b, UI_Background.bk_hover_color.b);

		m_easing.AddExec(eType, eMode, S2MS(0.2), UI_Background.border_color.r, UI_Background.border_hover_color.r);
		m_easing.AddExec(eType, eMode, S2MS(0.2), UI_Background.border_color.g, UI_Background.border_hover_color.g);
		m_easing.AddExec(eType, eMode, S2MS(0.2), UI_Background.border_color.b, UI_Background.border_hover_color.b);

		m_StringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		m_StringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		m_ImgFormat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
		m_ImgFormat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);

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

			if (!btn->m_track_leave)
			{
				TrackMouse(hwndBtn);
				btn->m_track_leave = true;
			}

			btn->Draw(TRUE);
			break;
		}
		case WM_MOUSELEAVE:
		{
			btn->m_track_leave = false;
			btn->UpdateState(BtnState::Normal);
			btn->BeginX1ThemeEffect();

			btn->Draw(TRUE);
			break;
		}
		case WM_MOUSEHOVER:
		{
			btn->UpdateState(BtnState::Hover);
			btn->BeginX1ThemeEffect();

			btn->Draw(TRUE);
			break;
		}
		case WM_TIMER:
		{
			btn->OnTimer(wParam);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			btn->UpdateState(BtnState::Click);
			break;
		}
		case WM_LBUTTONUP:
		{
			btn->m_eState = btn->m_eOldState;
			break;
		}
		case WM_ERASEBKGND:
		{
			return TRUE;
		}
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

		easing::EasingEngine* pEasing = &m_easing;

		if (m_eState == BtnState::Hover)
		{
			pEasing->SetReverse(false);
		}
		else if (m_eState == BtnState::Normal)
		{
			pEasing->SetReverse(true);
		}

		pEasing->ClearState();
		pEasing->Start();
	}

	bool UpdateX1ThemeEffect(bool bFirst =false)
	{
		if (m_bUseEffect == false)
			return false;

		easing::EasingEngine* pEasing = &m_easing;

		pEasing->Update(TIME_UPDATE_EFFECT);

		int r = static_cast<int>(pEasing->Value(0));
		int g = static_cast<int>(pEasing->Value(1));
		int b = static_cast<int>(pEasing->Value(2));

		m_cur_background_color = m_cur_background_color.MakeARGB(255, r, g, b);

		r = static_cast<int>(pEasing->Value(3));
		g = static_cast<int>(pEasing->Value(4));
		b = static_cast<int>(pEasing->Value(5));

		m_cur_border_color = m_cur_border_color.MakeARGB(255, r, g, b);

		return pEasing->IsActive();
	}

	void EndX1ThemeEffect()
	{
		if (m_bUseEffect == false)
			return;

		::KillTimer(m_hWnd, IDC_EFFECT_X1);
	}

	virtual void OnTimer(DWORD wParam)
	{
		static int a = 0;
		switch (wParam)
		{
			case IDC_EFFECT_X1:
			{
				if (UpdateX1ThemeEffect())
				{
					InvalidateRect(m_hWnd, NULL, FALSE);
				}
				else
				{
					EndX1ThemeEffect();
				}
				break;
			}
		}
	}

protected:

	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			Gdiplus::Rect rect = m_pRender->GetDrawRect();

			// [2] Draw color button state
			const unsigned int iBorderRadius= UI_Background.border_radius;
			const unsigned int iBorderWidth = UI_Background.border_width;

			Gdiplus::Brush* background_color = NULL;
			Gdiplus::Pen* pen_color = NULL;

			if (m_bUseEffect)
			{
				if (m_eState == BtnState::Click)
				{
					background_color = new Gdiplus::SolidBrush(UI_Background.bk_click_color.wrefcol);
					pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
				}
				else if (m_eState == BtnState::Hover)
				{
					background_color = new Gdiplus::SolidBrush(m_cur_background_color);
					pen_color = new Gdiplus::Pen(m_cur_border_color, iBorderWidth);
				}
				else
				{
					background_color = new Gdiplus::SolidBrush(m_cur_background_color);
					pen_color = new Gdiplus::Pen(m_cur_border_color, iBorderWidth);
				}
			}
			else
			{
				if (m_eState == BtnState::Click)
				{
					background_color = new Gdiplus::SolidBrush(UI_Background.bk_click_color.wrefcol);
					pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
				}
				else if (m_eState == BtnState::Hover)
				{
					background_color = new Gdiplus::SolidBrush(UI_Background.bk_hover_color.wrefcol);
					pen_color = new Gdiplus::Pen(UI_Background.border_hover_color.wrefcol, iBorderWidth);
				}
				else
				{
					background_color = new Gdiplus::SolidBrush(UI_Background.bk_color.wrefcol);
					pen_color = new Gdiplus::Pen(UI_Background.border_color.wrefcol, iBorderWidth);
				}
			}
			// Fill erase background
			this->EraseBackground(m_pRender);

			// Draw rectangle background
			this->DrawBackground(m_pRender, pen_color, background_color, iBorderRadius);

			SAFE_DELETE(pen_color);
			SAFE_DELETE(background_color);

			// [2] Draw image
			if (m_image)
			{
				m_pRender->DrawImageFullRect(m_image, &m_ImgFormat);
			}

			// [3] Draw text for button
			if (m_eState == BtnState::Hover)
			{
				Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(UI_Text.text_hover_color.wrefcol));
				m_pRender->DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &m_StringFormat);
			}
			else if (m_eState == BtnState::Click)
			{
				Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(235, 235, 235));
				m_pRender->DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &m_StringFormat);
			}
			else
			{
				Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(UI_Text.text_color.wrefcol));
				m_pRender->DrawTextFullRect(this->m_sLabel.c_str(), &normal_textcolor, &m_StringFormat);
			}
		}
		m_pRender->EndDrawRect(bDraw);
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

	void UpdateState(BtnState state, bool free_oldstate = false)
	{
		m_eOldState = (free_oldstate) ? BtnState::Normal : m_eState;

		if (state == BtnState::Hover)
		{
			CHECK_RUN_FUNCTION(m_EventFunEnter, NULL, this);
		}
		else if (state == BtnState::Normal)
		{
			CHECK_RUN_FUNCTION(m_EventFunLeave, NULL, this);
		}
		else if (state == BtnState::Click)
		{
			CHECK_RUN_FUNCTION(m_EventFun, NULL, this);
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

	friend class Combobox;
};

WNDPROC Button::sfunButtonWndProc = NULL;

____END_NAMESPACE____

#endif // !XBUTTON_H
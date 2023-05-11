////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xtrackbar.h
* @create   Nov 15, 2022
* @brief    Create and handle event trackbar
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XTRACKBAR_H
#define XTRACKBAR_H

#include "xcontrolbase.h"
#include "math/xeasing.h"

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Trackbar control
* Trackbar control for window
***********************************************************************************/
class Dllexport Trackbar : public ControlBase, public ControlRectUI
{
protected:
	typedef void(*typeFunButtonEvent)   (WindowBase* window, Trackbar* btn);

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

	static WNDPROC		sfunTrackbarWndProc;

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
	Trackbar() : ControlBase(), m_eState(BtnState::Normal), m_sLabel(L""), m_image(NULL),
		m_track_leave(false),
		m_eOldState(BtnState::Normal)
	{
		m_rect.x	  = 0;
		m_rect.y	  = 0;
		m_rect.width  = WIDTH_DEF;
		m_rect.height = HEIGHT_DEF;

		this->SetDefaultPropertyUI();
	}

	~Trackbar()
	{
		delete m_image;
	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::BUTTON); }
	void SetLabel(std::wstring lab) { m_sLabel = lab; }

protected:

	virtual void SetDefaultPropertyUI()
	{
		m_property.m_bk_color			= std::move(Color4(37, 37, 38));
		m_property.m_bk_hover_color		= std::move(Color4(66, 66, 68));
		m_property.m_click_color		= std::move(Color4(53, 53, 54));

		m_property.border_radius		= 0;
		m_property.border_width			= 0;
		m_property.text_color			= std::move(Color4(255, 255, 255));
		m_property.text_hover_color		= std::move(Color4(255, 255, 255));

		m_property.m_border_color		= std::move(Color4(77, 77, 80));
		m_property.m_border_hover_color	= std::move(Color4(66, 166, 254));
	}

private:

	/*******************************************************************************
	*! @brief  : Init button control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int OnInitControl()
	{
		DWORD style = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW /*| SS_OWNERDRAW*/ /*| SS_NOTIFY*/;

		m_hWnd = (HWND)CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control", style  /*| BS_NOTIFY*/,
						(int)m_rect.x,									// x position 
						(int)m_rect.y,									// y position 
						m_rect.width,									// Button width
						m_rect.height,									// Button height
						m_hWndPar,										// Parent window
						(HMENU)(UINT_PTR)m_ID,							// menu.
						(HINSTANCE)GetWindowLongPtr(m_hWndPar, GWLP_HINSTANCE),
						NULL);

		NULL_RETURN(m_hWnd, 0);

		sfunTrackbarWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&TrackbarProcHandle);

		m_cur_background_color = m_property.m_bk_color.wrefcol;
		m_cur_border_color = m_property.m_border_color.wrefcol;

		//easing::EaseType eType = easing::EaseType::Sine;
		//easing::EaseMode eMode = easing::EaseMode::In;

		//m_easing.AddExec(eType, eMode, S2MS(0.2), m_property.m_bk_color.r, m_property.m_bk_hover_color.r);
		//m_easing.AddExec(eType, eMode, S2MS(0.2), m_property.m_bk_color.g, m_property.m_bk_hover_color.g);
		//m_easing.AddExec(eType, eMode, S2MS(0.2), m_property.m_bk_color.b, m_property.m_bk_hover_color.b);

		//m_easing.AddExec(eType, eMode, S2MS(0.2), m_property.m_border_color.r, m_property.m_border_hover_color.r);
		//m_easing.AddExec(eType, eMode, S2MS(0.2), m_property.m_border_color.g, m_property.m_border_hover_color.g);
		//m_easing.AddExec(eType, eMode, S2MS(0.2), m_property.m_border_color.b, m_property.m_border_hover_color.b);

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
	static LRESULT CALLBACK TrackbarProcHandle(HWND hwndTrack, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Trackbar* trackbar = (Trackbar*)::GetWindowLongPtr(hwndTrack, GWLP_USERDATA);

		NULL_RETURN(trackbar, 0);

		std::cout << "1234" << std::endl;
		switch (uMsg)
		{

		case WM_MOUSEMOVE:
		{
			std::cout << "move" << std::endl;
			InvalidateRect(hwndTrack, NULL, FALSE);
			break;
		}
		case WM_PAINT:
		{
			trackbar->Draw(true);
			return FALSE;
		}
		//case WM_MOUSELEAVE:
		//{
		//	btn->m_track_leave = false;
		//	btn->UpdateState(BtnState::Normal);
		//	btn->BeginX1ThemeEffect();
		//	InvalidateRect(hwndBtn, NULL, FALSE);
		//	break;
		//}
		//case WM_MOUSEHOVER:
		//{
		//	btn->UpdateState(BtnState::Hover);
		//	btn->BeginX1ThemeEffect();
		//	InvalidateRect(hwndBtn, NULL, FALSE);
		//	break;
		//}
		//case WM_TIMER:
		//{
		//	btn->OnTimer(wParam);
		//	break;
		//}
		//case WM_LBUTTONDOWN:
		//{
		//	btn->UpdateState(BtnState::Click);
		//	break;
		//}
		//case WM_LBUTTONUP:
		//{
		//	btn->m_eState = btn->m_eOldState;
		//	break;
		//}
		case WM_ERASEBKGND:
		{
			return TRUE;
		}
		case WM_CTLCOLORBTN:
		{
			break;
		}
		}

		return CallWindowProc(sfunTrackbarWndProc, hwndTrack, uMsg, wParam, lParam);
	}

private:
	//void BeginX1ThemeEffect()
	//{
	//	if (m_bUseEffect == false)
	//		return;

	//	::SetTimer(m_hWnd, IDC_EFFECT_X1, TIME_UPDATE_EFFECT, (TIMERPROC)NULL);

	//	easing::EasingEngine* pEasing = &m_easing;

	//	if (m_eState == BtnState::Hover)
	//	{
	//		pEasing->SetReverse(false);
	//	}
	//	else if (m_eState == BtnState::Normal)
	//	{
	//		pEasing->SetReverse(true);
	//	}

	//	pEasing->ClearState();
	//	pEasing->Start();
	//}

	//bool UpdateX1ThemeEffect(bool bFirst =false)
	//{
	//	if (m_bUseEffect == false)
	//		return false;

	//	easing::EasingEngine* pEasing = &m_easing;

	//	pEasing->Update(TIME_UPDATE_EFFECT);

	//	int r = static_cast<int>(pEasing->Value(0));
	//	int g = static_cast<int>(pEasing->Value(1));
	//	int b = static_cast<int>(pEasing->Value(2));

	//	m_cur_background_color = m_cur_background_color.MakeARGB(255, r, g, b);

	//	r = static_cast<int>(pEasing->Value(3));
	//	g = static_cast<int>(pEasing->Value(4));
	//	b = static_cast<int>(pEasing->Value(5));

	//	m_cur_border_color = m_cur_border_color.MakeARGB(255, r, g, b);

	//	return pEasing->IsActive();
	//}

	//void EndX1ThemeEffect()
	//{
	//	if (m_bUseEffect == false)
	//		return;

	//	::KillTimer(m_hWnd, IDC_EFFECT_X1);
	//}

	//virtual void OnTimer(DWORD wParam)
	//{
	//	static int a = 0;
	//	switch (wParam)
	//	{
	//		case IDC_EFFECT_X1:
	//		{
	//			if (UpdateX1ThemeEffect())
	//			{
	//				InvalidateRect(m_hWnd, NULL, FALSE);
	//			}
	//			else
	//			{
	//				EndX1ThemeEffect();
	//			}
	//			break;
	//		}
	//	}
	//}

protected:

	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			Gdiplus::Rect rect = m_pRender->GetDrawRect();

			// [2] Draw color button state
			const unsigned int iRadius = m_property.border_radius;
			const unsigned int iBorderWidth = m_property.border_width;

			Gdiplus::Brush* background_color = NULL;
			Gdiplus::Pen* pen_color = NULL;

			if (m_bUseEffect)
			{
				if (m_eState == BtnState::Click)
				{
					background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
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
					background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
					pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
				}
				else if (m_eState == BtnState::Hover)
				{
					background_color = new Gdiplus::SolidBrush(m_property.m_bk_hover_color.wrefcol);
					pen_color = new Gdiplus::Pen(m_property.m_border_hover_color.wrefcol, iBorderWidth);
				}
				else
				{
					background_color = new Gdiplus::SolidBrush(m_property.m_bk_color.wrefcol);
					pen_color = new Gdiplus::Pen(m_property.m_border_color.wrefcol, iBorderWidth);
				}
			}

			// Fill erase background
			this->DrawEraseBackground(m_pRender);

			// Fill rectangle background;
			this->DrawFillBackground(m_pRender, background_color);

			// Draw rectangle background;
			this->DrawBorderBackground(m_pRender, pen_color);

			SAFE_DELETE(pen_color);
			SAFE_DELETE(background_color);

			Gdiplus::PointF ptStart = { (Gdiplus::REAL)rect.X , (Gdiplus::REAL)(rect.Y + rect.Height / 2) };
			Gdiplus::PointF ptEnd = { (Gdiplus::REAL)(rect.X + rect.Width) , (Gdiplus::REAL)(rect.Y + rect.Height / 2) };
			Gdiplus::Pen penLine(Gdiplus::Color(255, 100, 100), 4.f);
			m_pRender->DrawLineFull(ptStart, ptEnd, &penLine);
		}
		m_pRender->EndDrawRect(bDraw);
	}

public:
	virtual void Event(WindowBase* window, WORD _id, WORD _event)
	{
		NULL_RETURN(m_EventFun, );

		m_EventFun(window, this);
	}

	void SetEvent(void(*mn)(WindowBase*, Trackbar*))
	{
		m_EventFun = mn;
	}

	void UpdateState(BtnState state, bool free_oldstate = false)
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

	void SetEventEnterCallBack(void(*fun)(WindowBase* window, Trackbar* btn))
	{
		this->m_EventFunEnter = fun;
	}
	void SetEventLeaveCallBack(void(*fun)(WindowBase* window, Trackbar* btn))
	{
		this->m_EventFunLeave = fun;
	}

	void UseEffect(bool bUseEffect)
	{
		m_bUseEffect = bUseEffect;
	}

	friend class Combobox;
};

WNDPROC Trackbar::sfunTrackbarWndProc = NULL;

____END_NAMESPACE____

#endif // !XBUTTON_H
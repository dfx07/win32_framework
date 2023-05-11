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
#include "math/xgeometry.h"

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
	enum class TrackbarState
	{
		Normal     ,
		MoveTracker,
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
	TrackbarState		m_eState;
	TrackbarState		m_eOldState;

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


	typedef Gdiplus::RectF		foxRect;
	typedef Gdiplus::PointF		foxPoint;

	//////////////////////////////////////////////////////////////////
	// Struct information 
	foxRect						m_track_rect;

	Gdiplus::PointF				m_start_loc;
	Gdiplus::PointF				m_track_loc;
	Gdiplus::SizeF				m_track_size;
	Gdiplus::PointF				m_end_loc;

	float						m_fValue;
	float						m_fMinValue;
	float						m_fMaxValue;


	int							m_iModeGuide;

	HWND						m_hWndTT;
	std::wstring				m_strTT;

public:
	Trackbar() : ControlBase(), m_eState(TrackbarState::Normal), 
		m_sLabel(L""), m_image(NULL),
		m_track_leave(false),
		m_eOldState(TrackbarState::Normal)
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

	Gdiplus::PointF GetNormalVectorTracker()
	{
		Gdiplus::PointF v = m_end_loc - m_start_loc;
		float mag = std::sqrt(v.X * v.X + v.Y * v.Y);

		if (mag <= 0.001f)
		{
			return Gdiplus::PointF(0, 0);
		}
		return Gdiplus::PointF(v.X / mag , v.Y / mag);
	}

	float GetMagnitudeTracker()
	{
		Gdiplus::PointF v = m_end_loc - m_start_loc;
		float mag = std::sqrt(v.X * v.X + v.Y * v.Y);

		return mag;
	}

	void MoveTracker(float dis)
	{
		auto vec = GetNormalVectorTracker();

		m_track_loc.X = m_track_loc.X + vec.X * dis;
		m_track_loc.Y = m_track_loc.Y + vec.Y * dis;
	}

	virtual void UpdateTrackerSize()
	{
		m_track_rect.X = m_track_loc.X - m_track_size.Width  / 2;
		m_track_rect.Y = m_track_loc.Y - m_track_size.Height / 2;

		m_track_rect.Width  = m_track_size.Width;
		m_track_rect.Height = m_track_size.Height;
	}

	virtual void UpdateValue()
	{
		if (m_fValue <= m_fMinValue)
		{
			m_fValue = m_fMinValue;
			m_track_loc = m_start_loc;
		}
		else if (m_fValue >= m_fMaxValue)
		{
			m_fValue = m_fMaxValue;
			m_track_loc = m_end_loc;
		}
		else
		{
			m_track_loc = m_start_loc;

			float fScale = (m_fValue - m_fMinValue)/(m_fMaxValue - m_fMinValue);
			float fTrackLength = GetMagnitudeTracker();
			float value = fScale * fTrackLength;

			MoveTracker(value);
		}

		this->UpdateTrackerSize();
	}

	virtual void UpdateValue(long x, long y)
	{
		Point2D pt;
		geo::GetPerpPoint2Segment({ m_start_loc.X, m_start_loc.Y }, { m_end_loc.X, m_end_loc.Y }, { x , y }, &pt, true);
		float fValuePix  = geo::GetMagnitude(pt - Point2D{ m_start_loc.X, m_start_loc.Y });
		float fLengthPix = geo::GetMagnitude(Point2D{ m_end_loc.X, m_end_loc.Y } - Point2D{ m_start_loc.X, m_start_loc.Y });

		float fscale = fValuePix / fLengthPix;
		float value = m_fMinValue + fscale * (m_fMaxValue - m_fMinValue);

		m_fValue = value;
		std::cout << m_fValue << std::endl;

		this->UpdateValue();
	}

	virtual bool InsideTracker(long x, long y)
	{
		return m_track_rect.Contains(x, y);
	}

public:
	void SetMinValue(float value)
	{
		m_fMinValue = value;
	}

	void SetMaxValue(float value)
	{
		m_fMaxValue = value;
	}

	void SetValue(float value)
	{
		m_fValue = value;

		NULL_RETURN(m_hWnd, );

		this->UpdateValue();
	}

private:

	/*******************************************************************************
	*! @brief  : Init button control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int OnInitControl()
	{
		DWORD style = WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY;

		//m_hWnd = (HWND)CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control", style,
		//				(int)m_rect.x,									// x position 
		//				(int)m_rect.y,									// y position 
		//				m_rect.width,									// Button width
		//				m_rect.height,									// Button height
		//				m_hWndPar,										// Parent window
		//				(HMENU)(UINT_PTR)m_ID,							// menu.
		//				(HINSTANCE)GetWindowLongPtr(m_hWndPar, GWLP_HINSTANCE),
		//				NULL);

		m_hWnd = (HWND)(HWND)CreateWindow(L"STATIC", L"", style,
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

		CreateMyTooltip(m_hWnd);

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

		m_start_loc = Gdiplus::PointF((Gdiplus::REAL)m_rect.x, (Gdiplus::REAL)(m_rect.y + m_rect.height / 2));
		m_end_loc = Gdiplus::PointF((Gdiplus::REAL)(m_rect.x + m_rect.width), (Gdiplus::REAL)(m_rect.y + m_rect.height / 2));

		m_track_size.Width  = 10.f;
		m_track_size.Height = 20.f;

		this->SetMinValue(0.f);
		this->SetMaxValue(100.f);

		this->SetValue(25.f);

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

		long x, y;

		switch (uMsg)
		{
			case WM_MOUSEMOVE:
			{
				if (trackbar->m_eState == TrackbarState::MoveTracker)
				{
					if (trackbar->GetCursorPosInParent(x, y))
					{
						trackbar->UpdateValue(x, y);
						trackbar->Draw(true);
					}
				}
				break;
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
			case WM_NOTIFY :
			{
				NMHDR* pnm = reinterpret_cast<NMHDR*>(lParam);
				if (pnm->hwndFrom == trackbar->m_hWndTT)
				{
					std::cout << pnm->code << std::endl;
					switch (pnm->code)
					{
					case TTN_SHOW:
						return trackbar->OnToolTipShow(pnm);
					case TTN_POP:
					{
						std::cout << "hide" << std::endl;
						return TRUE;
					}
						
					//case NM_CUSTOMDRAW:
					//	return trackbar->OnToolTipCustomDraw((NMTTCUSTOMDRAW*)pnm);
					}
				}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				if (trackbar->GetCursorPosInParent(x, y))
				{
					if (trackbar->InsideTracker(x, y))
					{
						trackbar->m_eState = TrackbarState::MoveTracker;
						
					}
				}
				break;
			}
			case WM_LBUTTONUP:
			{
				trackbar->m_eState = TrackbarState::Normal;
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

		return CallWindowProc(sfunTrackbarWndProc, hwndTrack, uMsg, wParam, lParam);
	}

private:

	// Determine the required size of the client area of the tooltip
	BOOL GetToolTipContentSize(SIZE* psz)
	{
		BOOL ret = FALSE;

		HDC hdc = GetDC(m_hWndTT);
		if (hdc != NULL)
		{
			HFONT hfontTT = (HFONT)SendMessage(m_hWndTT, WM_GETFONT, 0, 0);
			HFONT hfontTTOld = (HFONT)SelectObject(hdc, hfontTT);
			if (hfontTTOld != NULL)
			{
				SIZE szText;
				if (GetTextExtentPoint32(hdc, m_strTT.c_str(), lstrlen(m_strTT.c_str()), &szText))
				{
					psz->cx = szText.cx;
					psz->cy = szText.cy;
					ret = TRUE;
				}

				SelectObject(hdc, hfontTTOld);
			}

			ReleaseDC(m_hWndTT, hdc);
		}

		return ret;
	}

	// Determine the required client rectangle of the tooltip to fit the
	// text
	BOOL GetToolTipContentRect(RECT* prc)
	{
		BOOL ret = FALSE;

		SIZE sz;
		if (GetToolTipContentSize(&sz))
		{
			if (GetWindowRect(m_hWndTT, prc))
			{
				prc->right = prc->left + sz.cx;
				prc->bottom = prc->top + sz.cy;
				ret = TRUE;
			}
		}

		return ret;
	}

	//https://www.stevenengelhardt.com/2007/08/29/custom-drawn-win32-tooltips/

	LRESULT OnToolTipShow(NMHDR* pnm)
	{
		std::cout << "show tool tip" << std::endl;
		LRESULT ret = 0;
		RECT rc;

		if (GetToolTipContentRect(&rc))
		{
			// Adjust the rectangle to be the proper size to contain the
			// content
			if (SendMessage(m_hWndTT, TTM_ADJUSTRECT, TRUE, (LPARAM)&rc))
			{
				


				if (SetWindowPos(m_hWndTT, NULL, m_track_loc.X, m_track_loc.Y, 100, 24,
					SWP_NOZORDER | SWP_NOACTIVATE))
				{
					ret = TRUE;
				}
			}
		}

		return ret;
	}

	void CreateMyTooltip(HWND hparent)
	{
		m_hWndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 0, 0, 0, 0, hparent, NULL, (HINSTANCE)GetWindowLongPtr(m_hWndPar, GWLP_HINSTANCE), NULL);

		TTTOOLINFO ti = { 0 };

		//ti.cbSize = sizeof(TTTOOLINFO);
		//*********************************************************
		// Specific settings for specific compiler options (Unicode/VC2013)
		//*********************************************************
		ti.cbSize = TTTOOLINFOW_V2_SIZE;

		ti.uFlags = TTF_SUBCLASS;
		ti.hwnd = hparent;
		wchar_t tooltip[30] = L"A main window";
		ti.lpszText = tooltip;
		GetClientRect(hparent, &ti.rect);

		if (!SendMessage(m_hWndTT, TTM_ADDTOOL, 0, (LPARAM)&ti))
			MessageBox(0, TEXT("Failed: TTM_ADDTOOL"), 0, 0);
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

	virtual void DrawTracker()
	{
		Gdiplus::SolidBrush brushTracker(Gdiplus::Color(255, 100, 100));
		Gdiplus::Pen penTracker(Gdiplus::Color(255, 100, 100), 1.f);
		m_pRender->DrawRectangle1(m_track_rect, &penTracker, &brushTracker);
	}

	virtual void DrawGuideLine()
	{

	}

	virtual void DrawTextInfo()
	{

	}

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

			//if (m_bUseEffect)
			//{
			//	if (m_eState == TrackbarState::Click)
			//	{
			//		background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
			//		pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
			//	}
			//	else if (m_eState == TrackbarState::Hover)
			//	{
			//		background_color = new Gdiplus::SolidBrush(m_cur_background_color);
			//		pen_color = new Gdiplus::Pen(m_cur_border_color, iBorderWidth);
			//	}
			//	else
			//	{
			//		background_color = new Gdiplus::SolidBrush(m_cur_background_color);
			//		pen_color = new Gdiplus::Pen(m_cur_border_color, iBorderWidth);
			//	}
			//}
			//else
			//{
			//	if (m_eState == TrackbarState::Click)
			//	{
			//		background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
			//		pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
			//	}
			//	else if (m_eState == TrackbarState::Hover)
			//	{
			//		background_color = new Gdiplus::SolidBrush(m_property.m_bk_hover_color.wrefcol);
			//		pen_color = new Gdiplus::Pen(m_property.m_border_hover_color.wrefcol, iBorderWidth);
			//	}
			//	else
			//	{
			//		background_color = new Gdiplus::SolidBrush(m_property.m_bk_color.wrefcol);
			//		pen_color = new Gdiplus::Pen(m_property.m_border_color.wrefcol, iBorderWidth);
			//	}
			//}

			// Fill erase background
			this->DrawEraseBackground(m_pRender);

			// Fill rectangle background;
			this->DrawFillBackground(m_pRender, background_color);

			// Draw rectangle background;
			this->DrawBorderBackground(m_pRender, pen_color);

			SAFE_DELETE(pen_color);
			SAFE_DELETE(background_color);

			Gdiplus::Pen penLine(Gdiplus::Color(255, 100, 100), 4.f);
			m_pRender->DrawLineFull(m_start_loc, m_end_loc, &penLine);

			// [1] Draw Text [Min Max]
			this->DrawTextInfo();

			// [2] Draw Guide line 
			this->DrawGuideLine();

			// [4] Draw Tracker
			this->DrawTracker();
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

	//void UpdateState(BtnState state, bool free_oldstate = false)
	//{
	//	m_eOldState = (free_oldstate) ? BtnState::Normal : m_eState;

	//	if ( m_EventFunEnter && state == BtnState::Hover)
	//	{
	//		m_EventFunEnter(NULL, this);
	//	}
	//	else if (m_EventFunLeave && state == BtnState::Normal)
	//	{
	//		m_EventFunLeave(NULL, this);
	//	}
	//	m_eState = state;
	//}

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
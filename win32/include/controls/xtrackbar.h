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
#include <vector>

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Trackbar control
* Trackbar control for window
***********************************************************************************/
class Dllexport Trackbar : public ControlBase, public RectUIControl
{
protected:
	typedef void(*typeFunTrackbarEvent)   (Trackbar* track);

protected:
	enum CTB_EVENT
	{
		CTB_EVT_TRACKER_CLICK	= 1002,
		CTB_EVT_UPDATE_TRACKER	= 1003,
	};

	enum class TrackbarState
	{
		Normal		,
		MoveTracker	,
		Click		,
	};

	enum { IDC_EFFECT_X1	  = 12003 };
	enum { WIDTH_DEF		  = 80	  };
	enum { HEIGHT_DEF		  = 25	  };

	enum { TIME_UPDATE_EFFECT = USER_TIMER_MINIMUM  };


	struct GuideLineData
	{
		float value;
		bool  bMajor;
	};

private:
	bool				m_track_leave;
	bool				m_bUseEffect = false;

	static WNDPROC		sfunTrackbarWndProc;

protected:
	TrackbarState				m_eState;

	////////////////////////////////////////////////////////////////////////////////
	// Effect (animation)
	easing::EasingEngine		m_easing;
	Gdiplus::Color				m_cur_background_color;
	Gdiplus::Color				m_cur_border_color;

	////////////////////////////////////////////////////////////////////////////////
	// Event Function
	typeFunTrackbarEvent		m_funcTrackbarChanged = {nullptr};

protected:

	////////////////////////////////////////////////////////////////////////////////
	// Property
	Gdiplus::StringFormat		m_StringFormat;

	////////////////////////////////////////////////////////////////////////////////
	// Struct information 

	Gdiplus::RectF				m_track_rect;
	std::vector<Gdiplus::PointF>m_triangle_track;

	Gdiplus::RectF				m_rect_value_show;
	Gdiplus::SizeF				m_rect_value_show_size;

	Gdiplus::PointF				m_start_loc;
	Gdiplus::PointF				m_track_loc;
	Gdiplus::SizeF				m_track_size;
	Gdiplus::PointF				m_end_loc;
	float						m_track_thickness;

	float						m_fValue;
	float						m_fMinValue;
	float						m_fMaxValue;


	int							m_iModeGuide;

	CRect						m_actual_rect;

	std::vector<GuideLineData> m_guideline_list;

public:
	Trackbar() : ControlBase(), m_eState(TrackbarState::Normal), m_track_leave(false)
	{
		m_rect.x	  = 0;
		m_rect.y	  = 0;
		m_rect.width  = WIDTH_DEF;
		m_rect.height = HEIGHT_DEF;

		m_track_thickness = 2.f;

		this->SetDefaultPropertyUI();
	}

	~Trackbar()
	{

	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::BUTTON); }

protected:

	virtual void SetDefaultPropertyUI()
	{
		UI_Background.bk_color			= std::move(Color(59, 91, 179));
		UI_Background.bk_hover_color	= std::move(Color(229, 241, 255));
		UI_Background.bk_click_color	= std::move(Color(201, 224, 247));
		UI_Background.border_radius		= 0;
		UI_Background.border_width		= 0;

		UI_Text.text_color				= std::move(Color(255, 255, 255));
		UI_Text.text_hover_color		= std::move(Color(0, 0, 0));
	}

private:
	/*******************************************************************************
	*! @brief  : Function support
	*! @return : {*_*}
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
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

	Gdiplus::PointF GetPositionFromValue(float value)
	{
		if (value <= m_fMinValue)
		{
			return m_start_loc;
		}

		if (value >= m_fMinValue)
		{
			return m_end_loc;
		}

		auto vec = GetNormalVectorTracker();

		float dis = value - m_fMinValue;

		Gdiplus::PointF loc;

		loc.X = m_track_loc.X + vec.X * dis;
		loc.Y = m_track_loc.Y + vec.Y * dis;

		return loc;
	}

	/*******************************************************************************
	*! @brief  : Update rect draw information
	*! @return : {*_*}
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
	virtual void UpdateRectTrackerInfo()
	{
		m_track_rect.X = m_track_loc.X - m_track_size.Width  / 2;
		m_track_rect.Y = m_track_loc.Y - m_track_size.Height / 2;

		m_track_rect.Width  = m_track_size.Width;
		m_track_rect.Height = m_track_size.Height;

		m_triangle_track.clear();

		float fWidthTrack   = 10.f;
		float fHeightTrack  = 10.f;

		m_triangle_track.emplace_back(Gdiplus::PointF{ m_track_loc.X, m_track_loc.Y });
		m_triangle_track.emplace_back(Gdiplus::PointF{ m_track_loc.X - fWidthTrack/2, m_track_loc.Y + fHeightTrack });
		m_triangle_track.emplace_back(Gdiplus::PointF{ m_track_loc.X + fWidthTrack/2, m_track_loc.Y + fHeightTrack });
	}

	virtual void UpdateRectValueShowInfo()
	{
		m_rect_value_show.X = m_track_loc.X  - m_rect_value_show_size.Width / 2;
		m_rect_value_show.Y = m_track_loc.Y - m_rect_value_show_size.Height;

		m_rect_value_show.Width  = m_rect_value_show_size.Width;
		m_rect_value_show.Height = m_rect_value_show_size.Height;
	}

	/*******************************************************************************
	*! @brief  : update value and recalc draw value when the value update
	*! @return : CRect
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
	virtual void UpdateValue(float value)
	{
		float fOldValue = m_fValue;

		m_fValue = value;

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

		this->UpdateRectTrackerInfo();
		this->UpdateRectValueShowInfo();

		if (std::fabs(fOldValue - m_fValue) <= 0.0001f)
		{
			CHECK_RUN_FUNCTION(m_funcTrackbarChanged, this);
		}
	}

	/*******************************************************************************
	*! @brief  : Check point inside track
	*! @return : CRect
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
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
		NULL_RETURN(m_hWnd, );

		this->UpdateValue(value);
	}

	void SetThickness(float value)
	{
		m_track_thickness = value;

		NULL_RETURN(m_hWnd, );

		this->Draw(true);
	}

private:

	/*******************************************************************************
	*! @brief  : Recalculater value size window handle when use margin
	*! @return : CRect
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
	CRect RecalcActualSizeControl(CRect rect)
	{
		CRect ret_rect;

		ret_rect.x		= rect.x	  +   m_margin.left;
		ret_rect.y		= rect.y	  +   m_margin.top;
		ret_rect.width	= rect.width  - 2*m_margin.right;
		ret_rect.height	= rect.height - 2*m_margin.bottom;

		return ret_rect;
	}

	/*******************************************************************************
	*! @brief  : Recalculater guidede data = value + draw
	*! @return : void
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
	void RecalcGuideLine()
	{
		m_guideline_list.clear();

		float fmin = m_fMinValue;
		float fmax = m_fMaxValue;

		m_guideline_list.emplace_back(GuideLineData{ m_fMinValue , true});
		m_guideline_list.emplace_back(GuideLineData{ m_fMaxValue , true});
	}

protected: // Support function
	Gdiplus::PointF GetProjectionPointToLsegment(const Gdiplus::PointF& ptLine1, const Gdiplus::PointF& ptLine2, const Gdiplus::PointF& pt)
	{

		auto funDot = [](const Gdiplus::PointF& v1, const Gdiplus::PointF& v2) ->float
		{
			return v1.X * v2.X + v1.Y * v2.Y;
		};

		Gdiplus::PointF ptPer;
		Gdiplus::PointF vp1p2 = ptLine2 - ptLine1;  // p1p2
		Gdiplus::PointF vp1p = pt - ptLine1;  // p1p
		Gdiplus::PointF vp2p = pt - ptLine2;  // p2p

		float fDis = vp1p2.X * vp1p2.X + vp1p2.Y * vp1p2.Y;

		if (std::abs(fDis) < MATH_EPSILON)
		{
			float fDet = vp1p.X * vp1p2.X + vp1p.Y * vp1p2.Y;

			float t = fDet / fDis;
			ptPer.X = ptLine1.X + t * (ptLine2.X - ptLine1.X);
			ptPer.Y = ptLine1.Y + t * (ptLine2.Y - ptLine1.Y);
		}
		else
		{
			ptPer = pt; // case 3 points coincide
		}

		int iRet = 0;

		// Point in side line Segment
		float fdot1 = funDot(ptLine2 - ptLine1, ptPer - ptLine1);
		float fdot2 = funDot(ptLine1 - ptLine2, ptPer - ptLine2);

		if (fdot1 >= MATH_EPSILON && fdot2 >= MATH_EPSILON)
		{
			iRet = 1;
		}
		else if (true)
		{
			float fDis1 = Magnitude(ptLine1 - ptPer);
			float fDis2 = Magnitude(ptLine2 - ptPer);
			iRet = (fDis1 <= fDis2) ? 2 : 3;
		}

		if (iRet == 2)
		{
			ptPer = ptLine1;
		}
		else if (iRet == 3)
		{
			ptPer = ptLine2;
		}

		return ptPer;
	}

	float Magnitude(const Gdiplus::PointF& v)
	{
		return std::sqrtf(v.X * v.X + v.Y * v.Y);
	}

protected:
	/*******************************************************************************
	*! @brief  : Process message event
	*! @return : void
	*! @author : thuong.nv          - [Date] : 18/03/2023
	*******************************************************************************/
	int OnProcessMessage(UINT msgID, WPARAM wParam, LPARAM lParam)
	{
		switch (msgID)
		{
			case CTB_EVT_TRACKER_CLICK:
			{
				m_eState = TrackbarState::MoveTracker;

				this->Draw(true);

				break;
			}
			case CTB_EVT_UPDATE_TRACKER:
			{
				long x = static_cast<long>(wParam);
				long y = static_cast<long>(lParam);

				Gdiplus::PointF pt = GetProjectionPointToLsegment({ m_start_loc.X, m_start_loc.Y }, { m_end_loc.X, m_end_loc.Y }, {Gdiplus::REAL(x) , Gdiplus::REAL(y) });

				float fValuePix  = Magnitude(pt - Gdiplus::PointF{ m_start_loc.X, m_start_loc.Y });
				float fLengthPix = Magnitude(Gdiplus::PointF{ m_end_loc.X, m_end_loc.Y } - Gdiplus::PointF{ m_start_loc.X, m_start_loc.Y });

				float value = m_fMinValue + (fValuePix / fLengthPix) * (m_fMaxValue - m_fMinValue);

				this->UpdateValue(value);

				this->Draw(true);

				break;
			}
			default:
			{
				break;
			}
		}

		return 0;
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

		m_margin = { -10.f, -11.f, -11.f , -10.f};

		// Actual size will be wider when setting
		m_actual_rect = this->RecalcActualSizeControl(m_rect);

		m_hWnd = (HWND)(HWND)CreateWindow(L"STATIC", L"", style,
						(int)m_actual_rect.x,							// x position 
						(int)m_actual_rect.y,							// y position 
						m_actual_rect.width,							// Button width
						m_actual_rect.height,							// Button height
						m_hWndPar,										// Parent window
						(HMENU)(UINT_PTR)m_ID,							// menu.
						(HINSTANCE)GetWindowLongPtr(m_hWndPar, GWLP_HINSTANCE),
						NULL);

		NULL_RETURN(m_hWnd, 0);

		sfunTrackbarWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&TrackbarProcHandle);

		m_cur_background_color = UI_Background.bk_color.wrefcol;
		m_cur_border_color     = UI_Background.border_color.wrefcol;

		m_StringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		m_StringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		m_start_loc = Gdiplus::PointF((Gdiplus::REAL)m_rect.x, (Gdiplus::REAL)(m_rect.y + m_rect.height / 2));
		m_end_loc = Gdiplus::PointF((Gdiplus::REAL)(m_rect.x + m_rect.width), (Gdiplus::REAL)(m_rect.y + m_rect.height / 2));

		m_track_size.Width  = 10.f;
		m_track_size.Height = 20.f;

		m_rect_value_show_size.Width  = 20.f;
		m_rect_value_show_size.Height = 10.f;

		this->SetMinValue(0.f);
		this->SetMaxValue(100.f);

		this->RecalcGuideLine();

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
						trackbar->OnProcessMessage(CTB_EVT_UPDATE_TRACKER, (WPARAM)x, (LPARAM)y);
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
						trackbar->OnProcessMessage(CTB_EVT_TRACKER_CLICK, 0L, 0L);
					}
				}
				break;
			}
			case WM_LBUTTONUP:
			{
				trackbar->m_eState = TrackbarState::Normal;
				trackbar->Draw(true);
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

	virtual void DrawLineValue()
	{
		// Draw line rangle
		Gdiplus::Pen penLine(Gdiplus::Color(255, 100, 100), m_track_thickness);
		m_pRender->DrawLineFull(m_start_loc, m_end_loc, &penLine);

		// Draw rangle line value
		Gdiplus::Pen penLineValue(Gdiplus::Color(0, 255, 0), m_track_thickness);
		m_pRender->DrawLineFull(m_start_loc, m_track_loc, &penLineValue);
	}

	virtual void DrawTracker()
	{
		Gdiplus::SolidBrush brushTracker(Gdiplus::Color(255, 100, 100));
		Gdiplus::Pen penTracker(Gdiplus::Color(255, 100, 100), 1.f);

		if (m_triangle_track.size() < 3)
			return;

		m_pRender->DrawTriangle(m_triangle_track[0], m_triangle_track[1], m_triangle_track[2], &penTracker, &brushTracker);
	}

	virtual void DrawGuideLine()
	{
		Gdiplus::Pen penLineValue(Gdiplus::Color(0, 255, 0), m_track_thickness);

		const int nMaxBuff = 10;
		wchar_t buff[nMaxBuff];
		Gdiplus::Rect rect_text_guideline;

		Gdiplus::SolidBrush guideline_text_color(UI_Text.text_color.wrefcol);

		for (int i = 0; i < m_guideline_list.size(); i++)
		{
			auto& guideline = m_guideline_list[i];

			float fLengthGuideLine = guideline.bMajor ? 4.f: 2.f;

			auto point_value = GetPositionFromValue(guideline.value);
			m_pRender->DrawLine({ point_value.X, point_value.Y + m_track_thickness/2}, { point_value.X, point_value.Y - fLengthGuideLine }, penLineValue);

			swprintf_s(buff, nMaxBuff, L"%0.f", guideline.value);

			auto rect_text = m_pRender->MeasureString(buff, static_cast<int>(wcslen(buff) +1), &m_StringFormat);

			rect_text_guideline.X		= point_value.X - rect_text.Width/2;
			rect_text_guideline.Y		= point_value.Y - fLengthGuideLine - rect_text.Height;
			rect_text_guideline.Width	= rect_text.Width;
			rect_text_guideline.Height	= rect_text.Height;

			m_pRender->DrawTextRect(rect_text_guideline, buff, &guideline_text_color, &m_StringFormat);
		}
	}

	virtual void DrawTextInfo()
	{
		if(m_eState == TrackbarState::MoveTracker)
		{
			// Draw text value current
			Gdiplus::SolidBrush textValueColor(Gdiplus::Color(255, 255, 255));
			const int nBuffSize = 20;
			wchar_t strBuff[nBuffSize];
			swprintf_s(strBuff, nBuffSize, L"%0.f", m_fValue);
			m_pRender->DrawTextRect(m_rect_value_show, strBuff, &textValueColor, &m_StringFormat);
		}
	}

	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			Gdiplus::Rect rect = m_pRender->GetDrawRect();

			// [2] Draw color button state
			auto iBorderRadius = UI_Background.border_radius;
			auto iBorderWidth  = UI_Background.border_width;

			Gdiplus::Brush* background_color = new Gdiplus::SolidBrush(UI_Background.bk_color.wrefcol);
			Gdiplus::Pen* pen_color = new Gdiplus::Pen(UI_Background.border_color.wrefcol, iBorderWidth);

			// Fill erase background
			this->EraseBackground(m_pRender);

			// Draw rectangle background
			this->DrawBackground(m_pRender, pen_color, nullptr, iBorderRadius);

			SAFE_DELETE(pen_color);
			SAFE_DELETE(background_color);

			this->DrawLineValue();

			// [1] Draw Guide line 
			this->DrawGuideLine();

			// [2] Draw Tracker
			this->DrawTracker();

			// [3] Draw Text [Min Max]
			this->DrawTextInfo();
		}
		m_pRender->EndDrawRect(bDraw);
	}

public:

	void SetEventChangeValue(typeFunTrackbarEvent fun)
	{
		m_funcTrackbarChanged = fun;
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
////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xcheckbox.h
* @create   Nov 15, 2022
* @brief    Create and handle event checkbox
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XCHECKBOX_H
#define XCHECKBOX_H

#include "xcontrolbase.h"

___BEGIN_NAMESPACE___

/**********************************************************************************
* ⮟⮟ Class name: Checkbox control
* Checkbox control for window
***********************************************************************************/
class Dllexport Checkbox : public ControlBase, public ControlRectUI
{
private:
	std::wstring		m_sLabel;
	bool				m_bChecked;

	GDIplusCtrlRender	m_render;

	Gdiplus::Bitmap*	m_image_check;
	Gdiplus::Bitmap*	m_image_uncheck;

protected:

	static WNDPROC& get_proc()
	{
		static WNDPROC prevWndProc;
		return prevWndProc;
	}

public:
	Checkbox(const wchar_t* text = L""): ControlBase()
	{
		m_sLabel = text;
		m_bChecked = false;

		this->SetDefaultPropertyUI();
	}

	~Checkbox()
	{
		delete m_image_check;
		delete m_image_uncheck;
	}

	virtual ControlType GetType() { return ControlType::CHECKBOX; }

public:

	/***************************************************************************
	*! @brief  : Set text lable
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetText(const wchar_t* text = L"")
	{
		m_sLabel = text;
	}

	/***************************************************************************
	*! @brief  : CalcTextSize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetCheck(bool bCheck)
	{
		m_bChecked = bCheck;
		NULL_RETURN(m_hWnd, );

		InvalidateRect(m_hWnd, NULL, FALSE);
	}

	/***************************************************************************
	*! @brief  : CalcTextSize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool Checked()
	{
		return m_bChecked;
	}

protected:

	/***************************************************************************
	*! @brief  : CalcTextSize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void CalcTextSize(int width, int height)
	{
		SIZE size;
		HDC hdc = GetDC(m_hWnd);
		GetTextExtentPoint32(hdc, m_sLabel.c_str(), (int)wcslen(m_sLabel.c_str()), &size);

		width  = size.cx;
		height = size.cy;

		ReleaseDC(m_hWnd, hdc);
	}

protected:

	virtual void SetDefaultPropertyUI()
	{
		m_property.m_background_color	= std::move(Color4(59, 91, 179));
		m_property.m_hover_color		= std::move(Color4(229, 241, 255));
		m_property.m_click_color		= std::move(Color4(201, 224, 247));

		m_property.border_radius = 0;
		m_property.border_width  = 0;
		m_property.text_color	 = std::move(Color4(255, 255, 255));
		m_property.text_hover_color = std::move(Color4(0, 0, 0));
	}

protected:

	/***************************************************************************
	*! @brief  : OnInitControl
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int OnInitControl()
	{
		DWORD style = WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_OWNERDRAW;

		CalcTextSize(m_rect.width, m_rect.height);

		m_hWnd = CreateWindow(L"Button", m_sLabel.c_str(), style,	// style + control name
								(int)m_rect.x, (int)m_rect.y,		// x, y
								m_rect.width, m_rect.height,		// width / height
								m_hWndPar,							// Handle parent
								(HMENU)(UINT_PTR)m_ID,						// ID
								NULL, NULL);

		NULL_RETURN(m_hWnd, 0);

		get_proc() = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&CheckBoxProcHandle);

		return 1;
	}

	static LRESULT CALLBACK CheckBoxProcHandle(HWND hwndChk, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Checkbox* chk = (Checkbox*)::GetWindowLongPtr(hwndChk, GWLP_USERDATA);
		NULL_RETURN(chk, 0);

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			InvalidateRect(hwndChk, NULL, FALSE);
			break;
		}
		case WM_MOUSELEAVE:
		{
			InvalidateRect(hwndChk, NULL, FALSE);
			break;
		}
		case WM_TIMER:
		{
			break;
		}
		case WM_LBUTTONDOWN:
		{
			chk->UpdateCheck();
		}
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		{
			break;
		}
		case WM_COMMAND:
		{
			break;
		}
		case WM_ERASEBKGND:
			return TRUE;
		case WM_CTLCOLORBTN:
		{
			break;
		}

		}
		return CallWindowProc(get_proc(), hwndChk, uMsg, wParam, lParam);
	}
protected:

	void UpdateCheck()
	{
		m_bChecked = !m_bChecked;
		InvalidateRect(m_hWnd, NULL, FALSE);
	}

public:
	void CreateColorButton()
	{
		if (!m_image_uncheck)
		{
			m_image_uncheck = Gdiplus::Bitmap::FromFile(L"resources/chk_uncheck.png", false);
			m_image_check = Gdiplus::Bitmap::FromFile(L"resources/chk_check.png", false);
		}
	}

	void Draw(LPDRAWITEMSTRUCT& pdis)
	{
		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
		m_render.Init(pdis->hDC, pdis->rcItem);
		m_render.LoadFont(L"Segoe UI");

		this->CreateColorButton();

		// [2] Draw color button state
		const unsigned int iRadius		= m_property.border_radius;
		const unsigned int iBorderWidth = m_property.border_width;

		// Fill erase background
		Gdiplus::Rect rect = ConvertToRect(pdis->rcItem);
		rect.X		-= 1;
		rect.Y		-= 1;
		rect.Width	+= 1;
		rect.Height += 1;
		m_render.DrawRectangle(rect, NULL, m_property.m_erase_color.wrefcol, 0);

		// Fill rectangle background;
		rect = ConvertToRect(pdis->rcItem);
		rect.X		+= 2;
		rect.Y		+= 2;
		rect.Width	-= iBorderWidth + 3;
		rect.Height -= iBorderWidth + 3;
		m_render.DrawRectangle(rect, NULL,m_property.m_background_color.wrefcol, iRadius);

		//Gdiplus::Pen pen(, iBorderWidth);
		//m_render.DrawRectangleFull(iBorderWidth > 0 ? &pen : NULL, m_background_normal, iRadius);

		// [2] Draw image check
		if (m_bChecked)
		{
			if (m_image_check)
			{
				GdiplusEx::ImageFormat imgformat;
				imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
				imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
				m_render.DrawImageFullRect(m_image_check, &imgformat, Gdiplus::PointF(2, 0));
			}
		}
		else
		{
			if (m_image_uncheck)
			{
				GdiplusEx::ImageFormat imgformat;
				imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
				imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
				m_render.DrawImageFullRect(m_image_uncheck, &imgformat, Gdiplus::PointF(2, 0));
			}
		}

		// [3] Draw text for button
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(m_property.text_color.wrefcol)); // color text normal
		m_render.DrawTextFullRect(m_sLabel.c_str(), &normal_textcolor, &format, Gdiplus::PointF(20, 0));

		m_render.Flush(true);
	}
};
____END_NAMESPACE____

#endif // !XCHECKBOX_H
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
class Dllexport Checkbox : public ControlBase, public RectUIControl
{
	enum CHK_EVENT
	{
		CBB_EVT_CHECK_CHANGED  = 1002,
	};

protected:
	typedef void(*typeFunCheckboxEvent) (Checkbox* btn);

private:
	std::wstring			m_sLabel;
	bool					m_bChecked;

	////////////////////////////////////////////////////////////////////////////////
	// Image property
	Gdiplus::Bitmap*		m_image_check;
	Gdiplus::Bitmap*		m_image_uncheck;

protected:
	static WNDPROC			sfunCheckboxWndProc;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Function property
	typeFunCheckboxEvent	m_funcCheckChanged = { nullptr };

public:
	Checkbox(const wchar_t* text = L"", bool bCheck = false): ControlBase(), 
		m_image_check(NULL), m_image_uncheck(NULL)
	{
		m_sLabel	= text;
		m_bChecked	= bCheck;

		this->SetDefaultPropertyUI();
	}

	~Checkbox()
	{
		SAFE_DELETE(m_image_check);
		SAFE_DELETE(m_image_uncheck);
	}

	virtual ControlType GetType() { return ControlType::CHECKBOX; }

public:
	/*******************************************************************************
	*! @brief  : set function event : checkbox changed
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void SetCheckEvent(typeFunCheckboxEvent fun)
	{
		m_funcCheckChanged = fun;
	}

	/*******************************************************************************
	*! @brief  : Set text lable
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void SetText(const wchar_t* text = L"")
	{
		m_sLabel = text;

		CHECK_RETURN(m_hWnd == NULL || !m_pRender, );

		::InvalidateRect(m_hWnd, NULL, FALSE);
	}

	/*******************************************************************************
	*! @brief  : CalcTextSize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void SetCheck(bool bCheck)
	{
		m_bChecked = bCheck;
		NULL_RETURN(m_hWnd, );

		::InvalidateRect(m_hWnd, NULL, FALSE);
	}

	/*******************************************************************************
	*! @brief  : CalcTextSize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	bool Checked()
	{
		return m_bChecked;
	}

protected:

	/*******************************************************************************
	*! @brief  : CalcTextSize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
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
		UI_Background.bk_color			= std::move(Color(59, 91, 179));
		UI_Background.bk_hover_color	= std::move(Color(229, 241, 255));
		UI_Background.bk_click_color	= std::move(Color(201, 224, 247));
		UI_Background.border_radius		= 0;
		UI_Background.border_width		= 0;

		UI_Text.text_color				= std::move(Color(255, 255, 255));
		UI_Text.text_hover_color		= std::move(Color(0, 0, 0));
	}

protected:

	/*******************************************************************************
	*! @brief  : OnInitControl
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int OnInitControl()
	{
		DWORD style = WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_OWNERDRAW;

		CalcTextSize(m_rect.width, m_rect.height);

		m_hWnd = CreateWindow(L"Button", m_sLabel.c_str(), style,	// style + control name
								(int)m_rect.x, (int)m_rect.y,		// x, y
								m_rect.width, m_rect.height,		// width / height
								m_hWndPar,							// Handle parent
								(HMENU)(UINT_PTR)m_ID,				// ID
								NULL, NULL);

		NULL_RETURN(m_hWnd, 0);

		this->LoadResource();

		sfunCheckboxWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&CheckBoxProcHandle);

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
				break;
			}
			case WM_MOUSELEAVE:
			{
				break;
			}
			case WM_TIMER:
			{
				break;
			}
			case WM_LBUTTONDOWN:
			{
				break;
			}
			case WM_RBUTTONUP:
			case WM_LBUTTONUP:
			{
				chk->OnProcessMessage(CBB_EVT_CHECK_CHANGED, 0L, 0L);
				break;
			}
			case WM_COMMAND:
			{
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
		return CallWindowProc(sfunCheckboxWndProc, hwndChk, uMsg, wParam, lParam);
	}

protected:
	/*******************************************************************************
	*! @brief  : Cập nhật thông tin stype của window 
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	int OnProcessMessage(UINT msgID, WPARAM wParam, LPARAM lParam)
	{
		switch (msgID)
		{
			case CBB_EVT_CHECK_CHANGED:
			{
				m_bChecked = !m_bChecked;
				this->Draw(true);
				CHECK_RUN_FUNCTION(m_funcCheckChanged, this);

				break;
			}
			default:
				break;
		}

		return 0;
	}
public:
	/*******************************************************************************
	*! @brief  : Load resource after after initialize
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void LoadResource()
	{
		SAFE_DELETE(m_image_uncheck);
		SAFE_DELETE(m_image_check);

		m_image_uncheck = Gdiplus::Bitmap::FromFile(L"resources/chk_uncheck.png", false);
		m_image_check   = Gdiplus::Bitmap::FromFile(L"resources/chk_check.png", false);
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

			Gdiplus::Pen* pPen = new Gdiplus::Pen(UI_Background.border_color.wrefcol, iBorderWidth);
			Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(UI_Background.bk_color.wrefcol);

			// Fill erase background
			this->EraseBackground(m_pRender);

			// Draw rectangle background
			this->DrawBackground(m_pRender, pPen, pBrush, iBorderRadius);

			// [2] Draw image check
			if (m_bChecked)
			{
				if (m_image_check)
				{
					GdiplusEx::ImageFormat imgformat;
					imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
					imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
					m_pRender->DrawImageFullRect(m_image_check, &imgformat, Gdiplus::PointF(2, 0));
				}
			}
			else
			{
				if (m_image_uncheck)
				{
					GdiplusEx::ImageFormat imgformat;
					imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
					imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
					m_pRender->DrawImageFullRect(m_image_uncheck, &imgformat, Gdiplus::PointF(2, 0));
				}
			}

			SAFE_DELETE(pPen);
			SAFE_DELETE(pBrush);

			// [3] Draw text for button
			Gdiplus::StringFormat format;
			format.SetAlignment(Gdiplus::StringAlignmentNear);
			format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

			Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(UI_Text.text_color.wrefcol)); // color text normal
			m_pRender->DrawTextFullRect(m_sLabel.c_str(), &normal_textcolor, &format, Gdiplus::PointF(20, 0));
		}
		m_pRender->EndDrawRect(bDraw);
	}
};

WNDPROC Checkbox::sfunCheckboxWndProc = NULL;

____END_NAMESPACE____

#endif // !XCHECKBOX_H
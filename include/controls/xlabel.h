﻿////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xlabel.h
* @create   Nov 15, 2022
* @brief    Create and handle event label
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XLABEL_H
#define XLABEL_H

#include "xcontrolbase.h"

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Label control
* Label control for window
***********************************************************************************/
class Dllexport Label : public ControlBase, public RectUIControl
{
private:
	std::wstring			m_text;

	Gdiplus::StringFormat	m_format;

private:
	/*******************************************************************************
	*! @brief  : Set text and update draw text
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void CalcTextSize(unsigned int& width, unsigned int& height)
	{
		NULL_RETURN(m_pRender, );

		Gdiplus::RectF bound = m_pRender->MeasureString(m_text.c_str(), (int)m_text.length()+1, &m_format);
		width  = bound.Width;
		height = bound.Height;

		if (bound.Width * bound.Height == 0)
		{
			std::cerr << "Update label size failed !" << std::endl;
		}
	}

	virtual ControlType GetType() { return ControlType::LABEL; }

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

public:
	Label(const wchar_t* text = L""): ControlBase()
	{
		m_text = text;

		m_format.SetAlignment(Gdiplus::StringAlignmentNear);
		m_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		this->SetDefaultPropertyUI();
	}

	/*******************************************************************************
	*! @brief  : Set text and update draw text
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void SetText(const wchar_t* text)
	{
		m_text = text;
		NULL_RETURN(m_pRender, );

		this->Draw(true);
	}

	std::wstring GetText()
	{
		return m_text;
	}

public:
	virtual int OnInitControl()
	{
		DWORD style = WS_VISIBLE | WS_CHILD | SS_OWNERDRAW;
		this->CalcTextSize(m_rect.width, m_rect.height);

		m_hWnd = CreateWindow(L"STATIC", m_text.c_str(), style, (int)m_rect.x, (int)m_rect.y, // x, y
					m_rect.width, m_rect.height,					// width / height
					m_hWndPar,										// Handle parent
					(HMENU)(UINT_PTR)m_ID,							// ID
					NULL, NULL);

		NULL_RETURN(m_hWnd, 0);

		return 1;
	}

	/*******************************************************************************
	*! @brief  : Draw function 
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			Gdiplus::Rect rect = m_pRender->GetDrawRect();

			// Fill erase background
			this->EraseBackground(m_pRender);

			Gdiplus::SolidBrush text_color(Gdiplus::Color(UI_Text.text_color.wrefcol));
			m_pRender->DrawTextFullRect(m_text.c_str(), &text_color, &m_format);
		}
		m_pRender->EndDrawRect(bDraw);
	}
};
____END_NAMESPACE____

#endif // !XLABEL_H

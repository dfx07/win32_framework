﻿////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xtextbox.h
* @create   Nov 15, 2022
* @brief    Create and handle event textbox
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XTEXTBOX_H
#define XTEXTBOX_H

#include "xcontrolbase.h"
#include "math/xeasing.h"
#include <list>

#pragma warning( disable : 26451)
#pragma warning( disable : 4267)

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Textbox control
* Textbox control for window
***********************************************************************************/
class Dllexport Textbox : public ControlBase, public RectUIControl
{
	enum { IDC_EFFECT_X1	  = 12003 };
	enum { WIDTH_DEF		  = 80	  };
	enum { HEIGHT_DEF		  = 25	  };

	enum { TIME_UPDATE_EFFECT = 5	  };

	enum {EVENT_TIMER_CURSOR =  50001};

	enum class TB_ACTION
	{
		MOVE_LEFT ,
		MOVE_RIGHT,

		DELETE_BACKSPACE,
		DELETE_BTN,
	};

	enum class TB_STATE
	{
		TB_HOVER,
		TB_INPUT,
		TB_NORMAL,
	};

private:
	bool					m_bUseEffect = false;

	Gdiplus::RectF			m_rect_text;
	Gdiplus::PointF			m_ptTextStart;
	int						m_icur;

	Gdiplus::PointF			m_ptCursor;
	bool					m_bStateCursor = false;
	bool					m_bActive = false;
	
	TB_STATE				m_eStateTB = TB_STATE::TB_NORMAL;
	bool					m_bTrackLeave = false;

	Gdiplus::StringFormat	m_text_format;

	static WNDPROC			sfunTexboxWndProc;

protected:
	std::wstring		m_stext;
	Gdiplus::Bitmap*	m_image;

public:
	Textbox() : ControlBase(), m_stext(L""), m_image(NULL)
	{
		m_rect.x	  = 0;
		m_rect.y	  = 0;
		m_rect.width  = WIDTH_DEF;
		m_rect.height = HEIGHT_DEF;

		m_padding = { 3, 2, 2, 2 };

		this->SetDefaultPropertyUI();
	}

	~Textbox()
	{
		delete m_image;
	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::TEXTBOX); }

	void SetText(const wchar_t* txt)
	{
		m_stext = txt;
	}

protected:

	virtual void SetDefaultPropertyUI()
	{
		m_property.m_bk_color			= std::move(Color4(252, 252, 252));
		m_property.m_bk_hover_color		= std::move(Color4(100, 110, 217));
		m_property.m_click_color		= std::move(Color4(255, 255, 245));

		m_property.border_radius		= 4;
		m_property.border_width			= 1;
		m_property.m_border_color		= std::move(Color4(255, 255, 255));
		m_property.text_color			= std::move(Color4(0, 0, 0));
		m_property.text_hover_color		= std::move(Color4(0, 0, 0));
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

		m_hWnd = (HWND)CreateWindow(L"STATIC", L"", style,
						(int)m_rect.x,									// x position 
						(int)m_rect.y,									// y position 
						m_rect.width,									// Button width
						m_rect.height,									// Button height
						m_hWndPar,										// Parent window
						(HMENU)(UINT_PTR)m_ID,							// menu.
						(HINSTANCE)GetWindowLongPtr(m_hWndPar, GWLP_HINSTANCE),
						NULL);

		NULL_RETURN(m_hWnd, 0);

		sfunTexboxWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&TextboxProcHandle);

		this->CalcRectText();

		m_ptTextStart = Gdiplus::PointF(m_rect_text.X, m_rect_text.Y + m_rect_text.Height/2);
		m_ptCursor = Gdiplus::PointF(m_rect_text.X, m_rect_text.Y);

		INT flags = 0x0;
		flags |= Gdiplus::StringFormatFlags::StringFormatFlagsNoWrap;
		flags |= Gdiplus::StringFormatFlags::StringFormatFlagsMeasureTrailingSpaces;

		m_text_format.SetFormatFlags(flags);
		m_text_format.SetAlignment(Gdiplus::StringAlignmentNear);
		m_text_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		m_icur = -1;

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
	static LRESULT CALLBACK TextboxProcHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Textbox* tb = (Textbox*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

		NULL_RETURN(tb, 0);

		switch (uMsg)
		{
			case WM_MOUSEMOVE:
			{
				if (tb->m_bTrackLeave == false)
				{
					auto hCursor = LoadCursor(NULL, IDC_IBEAM);
					SetCursor(hCursor);

					TrackMouse(hWnd);
					tb->m_bTrackLeave = true;
				}

				if (tb->m_eStateTB == TB_STATE::TB_NORMAL)
				{
					tb->m_eStateTB = TB_STATE::TB_HOVER;
					tb->Draw(true);
				}

				break;
			}
			case WM_SETCURSOR:
			{
				if (LOWORD(lParam) == HTCLIENT)
				{
					return TRUE;
				}
				break;
			}

			case WM_MOUSELEAVE:
			{
				tb->m_bTrackLeave = false;
				if (tb->m_eStateTB == TB_STATE::TB_HOVER)
				{
					auto hCursor = LoadCursor(NULL, IDC_IBEAM);
					SetCursor(hCursor);
					tb->m_eStateTB = TB_STATE::TB_NORMAL;
					tb->Draw(true);
				}
				break;
			}
			case WM_KEYDOWN:
			{
				tb->ProcessKeyControl((wchar_t)wParam);
				tb->Draw(true);
				break;
			}
			case WM_CHAR:
			{
				tb->m_bStateCursor = true;
				tb->ProcessKeyDown((wchar_t)wParam);
				tb->Draw(true);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				long x, y;
				if (tb->GetCursorPosInParent(x, y))
				{
					tb->BeginInput(x, y);
				}
				else
				{
					tb->BeginInput(-999, -999);
				}
				break;
			}

			case WM_KILLFOCUS:
			{
				tb->m_eStateTB = TB_STATE::TB_NORMAL;
				tb->EndInput();
				tb->Draw(true);
				break;
			}
			case WM_TIMER:
			{
				switch (wParam)
				{
					case EVENT_TIMER_CURSOR:
					{
						tb->UpdateInput();
						break;
					}
				}
				break;
			}
			case WM_LBUTTONUP:
			{
				break;
			}
		}
		return CallWindowProc(sfunTexboxWndProc, hWnd, uMsg, wParam, lParam);
	}

protected:

	void SetSelectCursor(int index)
	{
		if (index >= 0)
		{
			m_icur = index;
			m_ptCursor.X = m_ptTextStart.X + m_pRender->MeasureString(m_stext.c_str(), m_icur + 1, &m_text_format).Width - 2.f;
		}
		else
		{
			m_icur = -1;
			m_ptCursor.X = m_rect_text.X;
		}
	}

	/*******************************************************************************
	*! @brief  : Cập nhật trạng thái input textbox
	*! @return : | 0: thêm thành công và không full
	*!           | 1: thêm thành công và full
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void UpdateInput()
	{
		if (m_bActive)
		{
			m_bStateCursor = !m_bStateCursor;
			Draw(true);
		}
		else
		{
			this->EndInput();
			Draw(true);
		}
	}

	void BeginInput(long x, long y)
	{
		int iSelect = -1;

		if (m_stext.length() > 0)
		{
			iSelect = DetectIndexActive(x, y);
		}
		std::cout <<" >>> " << iSelect << std::endl;

		SetSelectCursor(iSelect);

		if (m_bActive == false)
		{
			m_bStateCursor = true;
			m_bActive = true;
			SetTimer(m_hWnd, EVENT_TIMER_CURSOR, 500, NULL); // 0.5 second
			SetFocus(m_hWnd);
			this->Draw(true);
		}
	}

	void EndInput()
	{
		m_bStateCursor = false;
		if (m_bActive == true)
		{
			KillTimer(m_hWnd, EVENT_TIMER_CURSOR);
		}
		m_bActive = false;
	}


	virtual void ProcessKeyControl(wchar_t ch)
	{
		int iAction = 0;
		if (ch == VK_BACK)     // press backspace
		{
			iAction = 2;
		}
		else if (ch == VK_LEFT) // move cursor left 
		{
			iAction = 3;
		}
		else if (ch == VK_RIGHT) // move cursor right
		{
			iAction = 4;
		}
		else if (ch == VK_TAB)  // lost forcus
		{
			iAction = 5;
		}
		else if (ch == VK_DELETE) // delete charactor
		{
			iAction = 6;
		}

		if (iAction == 2) //back
		{
			DeleteCharacter(TB_ACTION::DELETE_BACKSPACE);
		}
		else if (iAction == 3)
		{
			MoveCursor(TB_ACTION::MOVE_LEFT);
		}
		else if (iAction == 4)
		{
			MoveCursor(TB_ACTION::MOVE_RIGHT);
		}
		else if (iAction == 5)
		{
			Send_Message(WM_KILLFOCUS, 0, 0);
		}
		else if (iAction == 6)
		{
			DeleteCharacter(TB_ACTION::DELETE_BTN);
		}
	}

	virtual void ProcessKeyDown(wchar_t ch)
	{
		int iAction = 1;

		std::wstring clipboard = L"";

		if (ch == VK_BACK)  // press backspace
		{
			return;
		}

		else if (ch == 0x16) // patse value
		{
			// Try opening the clipboard
			if (::OpenClipboard(nullptr))
			{
				// Get handle of clipboard object for ANSI text
				HANDLE hData = GetClipboardData(CF_TEXT);
				if (hData != nullptr)
				{
					// Lock the handle to get the actual text pointer
					char* pzChar = static_cast<char*>(GlobalLock(hData));
					clipboard = ToWchar(pzChar, -1);
					// Release the lock
					GlobalUnlock(hData);
				}
				else
				{
					std::cerr << "hData clip board error !" << std::endl;
				}

				// Release the clipboard
				CloseClipboard();
			}
			else
			{
				std::cerr << "clip board empty !" << std::endl;
			}

			iAction = 5;
		}

		if (iAction == 1) // add + insert value
		{
			AddCharacter(ch);
		}
		else if (iAction == 5 && !clipboard.empty()) // paste value
		{
			PasteValue(clipboard);
		}
	}

	/*******************************************************************************
	*! @brief  : Patse giá trị vào vị trí con trỏ
	*! @return : | 0: thêm thành công và không full
	*!           | 1: thêm thành công và full
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int DetectIndexActive(long x, long y)
	{
		typedef std::pair<int, int> POS_DETECT;

		std::cout << x << ":" << y << std::endl;
		int iSelect = 0;

		float fWidthText = 0.f;

		int nTextLength = m_stext.length();
		int iStartIndex = 0;

		int iStart1, nLength1; float xStart1, xEnd1;
		int iStart2, nLength2; float xStart2, xEnd2;

		while (nTextLength > 1)
		{
			iStart1  = iStartIndex;
			nLength1 = nTextLength / 2;
			fWidthText = m_pRender->MeasureString(&m_stext[iStart1], nLength1, &m_text_format).Width;

			float offset = 0.f;
			if (iStart1 <= 0)
			{
				offset = 0.f;
			}
			else
			{
				offset = m_pRender->MeasureString(&m_stext[0], iStart1 - 1, &m_text_format).Width;
			}

			xStart1  = m_ptTextStart.X + offset;
			xEnd1 = xStart1 + fWidthText;

			iStart2  = iStart1 + nLength1;
			nLength2 = nTextLength - nLength1 + 1;
			fWidthText = m_pRender->MeasureString(&m_stext[iStart2], nLength2, &m_text_format).Width;

			xStart2 = xEnd1;
			xEnd2 = xStart2 + fWidthText;

			if(x >= xStart1 && x <= xEnd1)
			{
				iStartIndex = iStart1;
				nTextLength = nLength1;
				continue;
			}
			else if (x >= xStart2 && y <= xEnd2)
			{
				iStartIndex = iStart2;
				nTextLength = nLength2;
				continue;
			}

			break;
		}

		iSelect = iStartIndex - 1;

		if (iSelect < -1)
		{
			iSelect = -1;
		}

		return iSelect;
	}

	/*******************************************************************************
	*! @brief  : Patse giá trị vào vị trí con trỏ
	*! @return : | 0: thêm thành công và không full
	*!           | 1: thêm thành công và full
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void PasteValue(std::wstring& txt)
	{
		if (txt.empty()) return;

		int iIndexClipboard = 0;
		int nClipboardLength =  wcslen(txt.c_str());

		while ( iIndexClipboard < nClipboardLength &&
				AddCharacter(txt[iIndexClipboard]) == 0)
		{
			iIndexClipboard++;
		}

		if (iIndexClipboard < nClipboardLength)
		{
			float fWidthMove = 0.f;

			float fWidth_Cur = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format).Width;
			m_stext.insert(m_icur + 1, &txt[iIndexClipboard + 1]);
			float fWidth_Top = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format).Width;

			fWidthMove += std::fabs(fWidth_Top - fWidth_Cur);

			m_ptTextStart.X -= fWidthMove;

			m_icur -= iIndexClipboard + 1;
			m_icur += nClipboardLength;
		}
	}

	/*******************************************************************************
	*! @brief  : Thêm ký tự vào list danh sách
	*! @return : | 0: thêm thành công và không full
	*!           | 1: thêm thành công và full
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual int AddCharacter(wchar_t ch)
	{
		int iRet = 0;

		Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
		m_stext.insert(m_stext.begin() + (m_icur + 1), ch);
		Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
		float fWidthMove = std::fabs(rect_string_top.Width - rect_string_cur.Width);

		m_ptCursor.X += fWidthMove;

		if (m_icur < 0)
		{
			m_ptCursor.X -= 2.f;
		}

		if (m_ptCursor.X > m_rect_text.GetRight() &&
			std::fabs(m_ptCursor.X - m_rect_text.GetRight()) >= 0.001f)
		{
			m_ptCursor.X    -= fWidthMove;
			m_ptTextStart.X -= fWidthMove;

			iRet = 1; // full text;
		}

		m_icur++;

		return iRet; // ok add one;
	}

	/*******************************************************************************
	*! @brief  : Xóa ký tự tại vị trí con trỏ
	*! @return : | 0: thêm thành công và không full
	*!           | 1: thêm thành công và full
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void DeleteCharacter(TB_ACTION action)
	{
		if (action == TB_ACTION::DELETE_BACKSPACE)
		{
			int length = static_cast<int>(m_stext.length());

			if (m_icur >= 0 && m_icur < length)
			{
				Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
				m_stext.erase(m_stext.begin() + m_icur);
				Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);

				float fWidthMove = std::fabs(rect_string_top.Width - rect_string_cur.Width);

				// remove charactor and move cursor text
				if (std::fabs(m_ptTextStart.X - m_rect_text.X) > 0.001f)
				{
					m_ptTextStart.X += fWidthMove;
				}
				else
				{
					m_ptCursor.X -= fWidthMove;

					if (m_icur == 0)
					{
						m_ptTextStart.X = m_rect_text.X;
						m_ptCursor.X = m_rect_text.X;
					}
				}

				m_icur--;
			}
		}
		else if (action == TB_ACTION::DELETE_BTN)
		{
			int length = static_cast<int>(m_stext.length());

			int idel = m_icur + 1;

			if (idel >= 0 && idel < length)
			{
				Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
				m_stext.erase(m_stext.begin() + idel);
				Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);

				float fWidthMove = std::fabs(rect_string_top.Width - rect_string_cur.Width);

				if (m_ptTextStart.X + fWidthMove > m_rect_text.X)
				{
					m_ptTextStart.X = m_rect_text.X;
				}
				else if (m_ptTextStart.X + rect_string_top.Width < m_rect_text.GetRight())
				{
					m_ptTextStart.X += fWidthMove;
					m_ptCursor.X += fWidthMove;
				}
			}
		}
	}

	/*******************************************************************************
	*! @brief  : Di chuyển con trỏ từ vị trí hiện tại
	*! @return : | 0: thêm thành công và không full
	*!           | 1: thêm thành công và full
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	virtual void MoveCursor(TB_ACTION action)
	{
		if (action == TB_ACTION::MOVE_LEFT) // move left
		{
			int length = static_cast<int>(m_stext.length());

			if (m_icur >= 0)
			{
				float fWidthMove = 0.f;

				Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_icur +1, &m_text_format);
				Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_icur   , &m_text_format);

				fWidthMove += std::fabs(rect_string_top.Width - rect_string_cur.Width);

				m_ptCursor.X -= fWidthMove;

				if (m_ptCursor.X <= m_rect_text.X)
				{
					m_ptCursor.X    += fWidthMove;
					m_ptTextStart.X += fWidthMove;
				}

				if (m_icur == 0)
				{
					m_ptCursor.X	= m_rect_text.X;
					m_ptTextStart.X = m_rect_text.X;
				}
				m_icur--;
			}
		}
		else if (action == TB_ACTION::MOVE_RIGHT) // move right
		{
			int length = static_cast<int>(m_stext.length());

			if (m_icur < length - 1)
			{
				float fWidthMove = 0.f;

				Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_icur + 1, &m_text_format);
				Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_icur + 2, &m_text_format);

				fWidthMove = std::fabs(rect_string_top.Width - rect_string_cur.Width);

				m_ptCursor.X += fWidthMove;

				if (m_icur < 0)
				{
					m_ptCursor.X -= 2.f;
				}

				if (m_ptCursor.X >= m_rect_text.GetRight())
				{
					m_ptCursor.X    -= fWidthMove;
					m_ptTextStart.X -= fWidthMove;
				}
				m_icur++;
			}
		}
	}

	void CalcRectText()
	{
		m_rect_text.X		= m_rect.x		+ m_padding.left;
		m_rect_text.Y		= m_rect.y		+ m_padding.top;
		m_rect_text.Width	= m_rect.width	- m_padding.right  - 4 ;
		m_rect_text.Height	= m_rect.height - m_padding.bottom - 4 ;
	}

	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			Gdiplus::Rect rect = m_pRender->GetDrawRect();

			// [2] Draw color button state
			const unsigned int iRadius      = UI_Background.border_radius;
			const unsigned int iBorderWidth = UI_Background.border_width;

			Gdiplus::Brush* background_color = NULL;
			Gdiplus::Pen* pen_color = NULL;

			if (m_eStateTB == TB_STATE::TB_NORMAL)
			{
				background_color = new Gdiplus::SolidBrush(UI_Background.bk_color.wrefcol);
				pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 189, 196, 209), iBorderWidth);
			}
			else
			{
				background_color = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 253));
				pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 21, 120, 214), iBorderWidth);
			}

			//// Fill erase background
			//this->DrawEraseBackground(m_pRender);

			//// Fill rectangle background;
			//this->DrawFillBackground(m_pRender, background_color);

			//// Draw rectangle background;
			//this->DrawBorderBackground(m_pRender, pen_color);

			SAFE_DELETE(pen_color);
			SAFE_DELETE(background_color);

			Gdiplus::SolidBrush text_color(Gdiplus::Color(UI_Text.text_color.wrefcol));

			//Gdiplus::Pen pen_text_color(Gdiplus::Color(m_property.text_color.wrefcol));
			//m_pRender->DrawRectangle1(m_rect_text, &pen_text_color, nullptr);

			m_pRender->DrawTextInSideRect(m_rect_text, Gdiplus::PointF(m_ptTextStart.X, m_ptTextStart.Y), m_stext.c_str(), &text_color, &m_text_format);

			if (m_bActive == true && m_bStateCursor == true)
			{
				Gdiplus::Pen pen_cursor_color(Gdiplus::Color::Red);

				Gdiplus::PointF pCurS = m_ptCursor;
				Gdiplus::PointF pCurE = m_ptCursor;
				pCurE.Y = pCurE.Y + m_rect_text.Height;

				m_pRender->DrawLine(pCurS, pCurE, pen_cursor_color);
			}
		}
		m_pRender->EndDrawRect(bDraw);
	}
};

WNDPROC Textbox::sfunTexboxWndProc = NULL;

____END_NAMESPACE____

#endif // !XTEXTBOX_H
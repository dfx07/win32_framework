////////////////////////////////////////////////////////////////////////////////////
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
#include "xeasing.h"
#include <list>

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Textbox control
* Textbox control for window
***********************************************************************************/
class Dllexport Textbox : public ControlBase, public ControlRectUI, public RectUI
{
	enum { IDC_EFFECT_X1	  = 12003 };
	enum { WIDTH_DEF		  = 80	  };
	enum { HEIGHT_DEF		  = 25	  };

	enum { TIME_UPDATE_EFFECT = 5	  };

private:
	bool					m_bUseEffect = false;

	Gdiplus::RectF			m_rect_text;
	Gdiplus::PointF			m_ptTextStart;
	Gdiplus::PointF			m_ptCursor;
	UINT					m_last;

	std::list<float>		m_list_width_charac;
	int						m_icur;

	Gdiplus::StringFormat	m_text_format;

	static WNDPROC			sfunTexboxWndProc;

protected:
	std::wstring		m_stext;

	Gdiplus::Bitmap*	m_image;

	EasingEngine		m_easing;

public:
	Textbox() : ControlBase(), m_stext(L""), m_image(NULL)
	{
		m_rect.x	  = 0;
		m_rect.y	  = 0;
		m_rect.width  = WIDTH_DEF;
		m_rect.height = HEIGHT_DEF;

		m_padding = { 2, 2, 2, 2 };

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
		m_property.m_background_color	= std::move(Color4(255, 91, 179));
		m_property.m_hover_color		= std::move(Color4(100, 110, 217));
		m_property.m_click_color		= std::move(Color4(255, 255, 245));

		m_property.border_radius		= 4;
		m_property.border_width			= 1;
		m_property.m_border_color		= std::move(Color4(255, 255, 255));
		m_property.text_color			= std::move(Color4(255, 255, 255));
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

	/*******************************************************************************
	*! @brief  : Hàm xử lý sự kiện cho button window
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	static LRESULT CALLBACK TextboxProcHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Textbox* tb = (Textbox*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

		NULL_RETURN(tb, 0);


		//std::cout << "Move : " << uMsg << std::endl;
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
		//case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		//case WM_SYSCHAR:
		{
			//BYTE keyState[256];
			//GetKeyboardState(keyState);
			//keyState[VK_CONTROL] = keyState[VK_SHIFT] = keyState[VK_MENU] = 0;
			//keyState[VK_LCONTROL] = keyState[VK_LSHIFT] = keyState[VK_LMENU] = 0;
			//keyState[VK_RCONTROL] = keyState[VK_RSHIFT] = keyState[VK_RMENU] = 0;

			//UINT scanCode = (wParam >> 16) & 0xFF;

			//WCHAR temp[100];
			//int i = ToUnicode(wParam, scanCode, keyState, temp, 100, 0);

			std::cout << wParam << std::endl;

			tb->ProcessKeyDown((wchar_t)wParam);
			tb->Draw(true);
			break;
		}
		case WM_DRAWITEM:
		{
			break;
		}
		case WM_PAINT:
		{
			//tb->Draw(true);
			
			//return TRUE;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetFocus(tb->GetHwnd());

			break;
		}
		case WM_LBUTTONUP:
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

		//std::cout << uMsg << std::endl;

		return CallWindowProc(sfunTexboxWndProc, hWnd, uMsg, wParam, lParam);
	}

protected:

	virtual void ProcessKeyDown(wchar_t ch)
	{
		int iAction = 0; // 0 : no action | 1 : add | 2 : remove | 3: move left | 4: move right

		std::cout << ch << std::endl;
		iAction = 1;

		if (ch == 8)
		{
			if (m_icur >= 0)
			{
				m_last = m_stext[m_icur];
			}

			iAction = 2;
		}
		else if (ch == 0x25) // left
		{
			iAction = 3;
		}
		else if (ch == 0x27) // right
		{
			iAction = 4;
		}

		//if (ch == 231)
		//{
		//	BYTE keyState[256];
		//	GetKeyboardState(keyState);
		//	keyState[VK_CONTROL] = keyState[VK_SHIFT] = keyState[VK_MENU] = 0;
		//	keyState[VK_LCONTROL] = keyState[VK_LSHIFT] = keyState[VK_LMENU] = 0;
		//	keyState[VK_RCONTROL] = keyState[VK_RSHIFT] = keyState[VK_RMENU] = 0;

		//	UINT scanCode = (ch >> 16) & 0xFF;

		//	WCHAR temp[100];
		//	int i = ToUnicode(ch, scanCode, keyState, temp, 100, 0);

		//	WPARAM unicode1 = (ch >> 16) & 0xFF;
		//	WPARAM unicode2 = (m_last << 16) & 0xFF;
		//	WPARAM unicode = unicode1 | unicode2;
		//}

		if (iAction == 1)
		{
			float fWidthMove = 0.f;

			Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
			m_stext.insert(m_stext.begin() + (m_icur + 1), ch);
			Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
			fWidthMove += std::fabs(rect_string_top.Width - rect_string_cur.Width);

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
		else if (iAction == 2) //back
		{
			int length = static_cast<int>(m_stext.length());

			if (m_icur >= 0 && m_icur < length)
			{
				Gdiplus::RectF rect_string_cur = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);
				m_stext.erase(m_stext.begin() + m_icur);
				Gdiplus::RectF rect_string_top = m_pRender->MeasureString(m_stext.c_str(), m_stext.length(), &m_text_format);

				float fWidthMove = std::fabs(rect_string_top.Width - rect_string_cur.Width);

				// remove charactor and move cursor text
				if (m_ptTextStart.X < m_rect_text.X)
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
		else if (iAction == 3) // move left
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

			std::cout << m_icur << std::endl;
		}
		else if (iAction == 4) // move right
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
			const unsigned int iRadius = m_property.border_radius;
			const unsigned int iBorderWidth = m_property.border_width;

			Gdiplus::Brush* background_color = NULL;
			Gdiplus::Pen* pen_color = NULL;

			background_color = new Gdiplus::SolidBrush(m_property.m_erase_color.wrefcol);
			pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 253), iBorderWidth);

			// Fill erase background
			this->DrawEraseBackground(m_pRender);

			// Fill rectangle background;
			this->DrawFillBackground(m_pRender, background_color);

			// Draw rectangle background;
			this->DrawBorderBackground(m_pRender, pen_color);

			SAFE_DELETE(pen_color);
			SAFE_DELETE(background_color);

			Gdiplus::SolidBrush text_color(Gdiplus::Color(m_property.text_color.wrefcol));
			Gdiplus::Pen pen_text_color(Gdiplus::Color(m_property.text_color.wrefcol));
			Gdiplus::Pen pen_cursor_color(Gdiplus::Color::Red);


			m_pRender->DrawRectangle1(m_rect_text, &pen_text_color, nullptr);

			m_pRender->DrawTextInSideRect(m_rect_text, Gdiplus::PointF(m_ptTextStart.X, m_ptTextStart.Y), m_stext.c_str(), &text_color, &m_text_format);

			Gdiplus::PointF pCurS = m_ptCursor;
			Gdiplus::PointF pCurE = m_ptCursor;
			pCurE.Y = pCurE.Y + m_rect_text.Height;

			m_pRender->DrawLine(pCurS, pCurE, pen_cursor_color);
		}
		m_pRender->EndDrawRect(bDraw);
	}
};

WNDPROC Textbox::sfunTexboxWndProc = NULL;

____END_NAMESPACE____

#endif // !XTEXTBOX_H
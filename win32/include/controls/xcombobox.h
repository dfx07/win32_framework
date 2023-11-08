////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xcombobox.h
* @create   Nov 15, 2022
* @brief    Create and handle combobox control
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XCOMBOBOX_H
#define XCOMBOBOX_H

#include "xcontrolbase.h"
#include <Commctrl.h>
#include <iostream>

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Combobox control
* Combobox control for window
***********************************************************************************/
class Dllexport Combobox : public ControlBase, public RectUIControl
{
	enum CBB_EVENT
	{
		CBB_EVT_DROPDOWN		= 1002,
		CBB_EVT_HOVER_DROPDOWN	= 1003,
		CBB_EVT_CLICK_DROPDOWN	= 1004,
	};

	enum { WIDTH_DEF = 100 };
	enum { HEIGHT_DEF = 50 };

	enum class CBB_State
	{
		NORMAL  ,
		DROPDOWN,
	};

	struct CBB_ITEM
	{
		std::wstring	text = L"";  // dữ liệu text hiển thị trên cbb
		void*			data = NULL; // dữ liêu của item tự định nghĩa và kiểm soát

		Gdiplus::Rect	rect;
	};

	typedef std::vector<CBB_ITEM> ItemList;
private:
	bool				m_bEditText;

	int					m_iSelected;
	int					m_iItemHover;

	ItemList			m_items;

	CBB_State			m_eState;

	Gdiplus::Rect		m_rect_btn_dropdown;
	Gdiplus::Rect		m_rect_titlebar;

	HWND				m_hWndDropDown;

	bool				m_dropdown_leave;

	static WNDPROC		sfunComboboxWndProc;
	static WNDPROC		sfunDropDownProc;

	void(*m_EventSelectedChangedFun)(Combobox*) = NULL;

public:
	Combobox(	int _x = 0, int _y = 0,
				int _width	= WIDTH_DEF,
				int _height = HEIGHT_DEF) :ControlBase()
	{
		m_rect.x		= _x;
		m_rect.y		= _y;
		m_rect.width	= _width;
		m_rect.height	= _height;

		m_bEditText		= false;
		m_iSelected		= -1;

		m_eState		= CBB_State::NORMAL;
		m_iItemHover	= -1;
		m_dropdown_leave = false;

		this->SetDefaultPropertyUI();
	}

	~Combobox()
	{
		m_hWnd = 0;
		this->RemoveAllItem();
	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::COMBOBOX); }

private:
	/*******************************************************************************
	*! @brief  : update all item and recalc item rectangle
	*! @return : void
	*! @author : thuong.nv          - [Date] : 16/03/2023
	*******************************************************************************/
	void UpdateItems()
	{
		NULL_RETURN(m_hWnd, );

		this->RecalcRectItems();

		NULL_RETURN(m_pRender, );

		this->Draw(true);
	}

public:
	void SetEventSelectedChange(void(*fun)(Combobox*))
	{
		m_EventSelectedChangedFun = fun;
	}

	/*******************************************************************************
	*! @brief  : Set select item index
	*! @return : void
	*! @author : thuong.nv          - [Date] : 16/03/2023
	*******************************************************************************/
	void Select(int index)
	{
		if (index < 0 || index >= m_items.size())
		{
			assert(0);
			return;
		}

		m_iSelected = index;

		NULL_RETURN(m_hWnd, );

		this->Draw(true);
	}

	void TextEdit(bool bEn)
	{
		m_bEditText = bEn;
	}

	/*******************************************************************************
	*! @brief  : add item - note : clone and use new function when create data
	*! @return : void
	*! @author : thuong.nv          - [Date] : 16/03/2023
	*******************************************************************************/
	void AddItem(const wchar_t* text, void* data = NULL)
	{
		CBB_ITEM    item;
		item.text = text;
		item.data = data;

		m_items.push_back(item);
	}

	//===================================================================================
	// Xóa một item được chỉ định bằng text : tất cả các item có text sẽ bị xóa          
	//===================================================================================
	void RemoveItem(const wchar_t* text)
	{
		for (auto it = m_items.begin(); it != m_items.end(); /*it++*/)
		{
			if (wcscmp(text, it->text.c_str()) == 0)
			{
				SAFE_DELETE(it->data);
				it = m_items.erase(it);
			}
			else ++it;
		}

		UpdateItems();
	}

	//===================================================================================
	// Xóa một item được chỉ định bằng index : tất cả các item có text sẽ bị xóa         
	//===================================================================================
	void RemoveItem(int index)
	{
		if (index < 0 || index >= m_items.size())
		{
			return;
		}

		delete m_items[index].data;
		m_items.erase(m_items.begin() + index);

		UpdateItems();
	}

	//===================================================================================
	// Xóa toàn bộ item đang co trong combobxo                                           
	//===================================================================================
	void RemoveAllItem()
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			delete m_items[i].data;
		}
		m_items.clear();

		UpdateItems();
	}

	/*******************************************************************************
	*! @brief  : Get text value item 
	*! @return : std::wstring
	*! @author : thuong.nv          - [Date] : 16/03/2023
	*******************************************************************************/
	std::wstring GetItemText(int index)
	{
		if (index < 0 || index >= m_items.size())
		{
			return L"";
		}

		return m_items[index].text;
	}

	/*******************************************************************************
	*! @brief  : Get data value item - note : don't delete the data
	*! @return : void *
	*! @author : thuong.nv          - [Date] : 16/03/2023
	*******************************************************************************/
	void* GetItemData(int index)
	{
		if (index < 0 || index >= m_items.size())
		{
			return NULL;
		}

		return m_items[index].data;
	}

protected:
	/*******************************************************************************
	*! @brief  : Hàm xử lý sự kiện cho button window
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	static LRESULT CALLBACK ComboboxProcHandle(HWND hwndBtn, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Combobox* cbb = (Combobox*)::GetWindowLongPtr(hwndBtn, GWLP_USERDATA);
		NULL_RETURN(cbb, 0);

		if (wParam == PROCESS_MSG)
		{
			return cbb->OnProcessMessage(uMsg, wParam, lParam);
		}

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
			case WM_MEASUREITEM:
			{
				return TRUE;
			}
			case WM_PAINT:
			{
				cbb->Draw(true);
				break;
			}
			case CBN_DROPDOWN:
			{
				int d = 100;
				return TRUE;
			}
			case WM_LBUTTONDOWN:
			{
				long x, y;
				if (cbb->GetCursorPosInParent(x, y) &&
					cbb->m_rect_btn_dropdown.Contains(x , y))
				{
					cbb->Send_Message(CBB_EVT_DROPDOWN, PROCESS_MSG, 0);
				}
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

		return CallWindowProc(sfunComboboxWndProc, hwndBtn, uMsg, wParam, lParam);
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
	static LRESULT CALLBACK DropdownProcHandle(HWND hwndBtn, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Combobox* cbb = (Combobox*)::GetWindowLongPtr(hwndBtn, GWLP_USERDATA);
		NULL_RETURN(cbb, 0L);

		if (wParam == PROCESS_MSG)
		{
			return cbb->OnProcessMessage(uMsg, wParam, lParam);
		}

		long x, y;

		switch (uMsg)
		{
			case WM_SIZE:
			{
				return FALSE;
			}
			case WM_MOUSEMOVE:
			{
				if (!cbb->m_dropdown_leave)
				{
					TrackMouse(hwndBtn);
					cbb->m_dropdown_leave = true;
				}

				if (cbb->GetCursorPosInParent(x, y))
				{
					int iHover = cbb->GetItemHover(x, y);
					cbb->Send_Message(CBB_EVT_HOVER_DROPDOWN, PROCESS_MSG, iHover);
				}

				break;
			}
			case WM_MOUSELEAVE:
			{
				cbb->m_dropdown_leave = false;
				cbb->Send_Message(CBB_EVT_HOVER_DROPDOWN, PROCESS_MSG, -2);
				break;
			}
			case WM_MEASUREITEM:
			{
				return TRUE;
			}
			case WM_PAINT:
			{
				cbb->OnDrawDropDown(true);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				if (cbb->GetCursorPosInParent(x, y) == TRUE)
				{
					int iHover = cbb->GetItemHover(x, y);
					cbb->Send_Message(CBB_EVT_CLICK_DROPDOWN, PROCESS_MSG, iHover);
				}

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

		return CallWindowProc(sfunDropDownProc, hwndBtn, uMsg, wParam, lParam);
	}


protected:
	virtual void SetDefaultPropertyUI()
	{
		UI_Background.bk_color		= std::move(Color(42, 65, 128));
		UI_Background.bk_hover_color= std::move(Color(229, 241, 255));
		UI_Background.bk_click_color= std::move(Color(201, 224, 247));
		UI_Background.border_radius = 2;
		UI_Background.border_width  = 1;

		UI_Background.border_color	= std::move(Color(255, 255, 255));

		UI_Text.text_color			= std::move(Color(255, 255, 255));
		UI_Text.text_hover_color	= std::move(Color(0, 0, 0));
	}

	int GetItemHover(long x, long y)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].rect.Contains(x, y))
			{
				return i;
			}
		}

		return -1;
	}

	/*******************************************************************************
	*! @brief  : Recalculate title bar information
	*! @return : void
	*! @author : thuong.nv          - [Date] : 16/05/2023
	*******************************************************************************/
	void RecalcRectItems()
	{
		auto rect = this->GetRect(m_hWndDropDown, true);

		for (int i = 0; i < m_items.size(); i++)
		{
			Gdiplus::Rect rect_item({ rect.left, rect.top + (int)m_rect.height * i }, { (INT)m_rect.width, (INT)m_rect.height });
			m_items[i].rect = rect_item;
		}
	}

	/*******************************************************************************
	*! @brief  : Recalculate title bar information
	*! @return : void
	*! @author : thuong.nv          - [Date] : 16/05/2023
	*******************************************************************************/
	void RecalcTitleBarInfo()
	{
		auto rect = this->GetRect(true);

		// [1] Draw border title bar
		auto iBorderWidth  = UI_Background.border_width;
		auto iBorderRadius = UI_Background.border_radius;

		m_rect_titlebar.X		= rect.left + iBorderWidth/2;
		m_rect_titlebar.Y		= rect.top  + iBorderWidth/2;
		m_rect_titlebar.Width	= (rect.right  - rect.left);
		m_rect_titlebar.Height	= (rect.bottom - rect.top);
		m_rect_titlebar.Width	-=(int) iBorderWidth/2;
		m_rect_titlebar.Height	-=(int) iBorderWidth/2;

		m_rect_btn_dropdown.X		=(m_rect_titlebar.X + m_rect_titlebar.Width) - (m_rect_titlebar.Height);
		m_rect_btn_dropdown.Y		= m_rect_titlebar.Y;
		m_rect_btn_dropdown.Width	= m_rect_titlebar.Height - (int)UI_Background.border_width/2;
		m_rect_btn_dropdown.Height	= m_rect_titlebar.Height;
	}

protected:
	/*******************************************************************************
	*! @brief  : Oninit control function create window handle
	*! @return : int : number control created
	*! @author : thuong.nv          - [Date] : 16/05/2023
	*******************************************************************************/
	virtual int OnInitControl()
	{
		DWORD style = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW;

		//if (m_bEditText) style |= CBS_DROPDOWN;
		//else			 style |= CBS_DROPDOWNLIST;

		m_hWnd = CreateWindow(L"BUTTON", NULL, style, (int)m_rect.x, (int)m_rect.y,
								m_rect.width, m_rect.height, m_hWndPar,
								(HMENU)(UINT_PTR)m_ID,
								NULL, NULL);
		style = 0;
		style = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW;

		// try use static control but it alway draw paint
		m_hWndDropDown = CreateWindow(L"BUTTON", NULL, style,
										(int)m_rect.x, (int)m_rect.y + m_rect.height + UI_Background.border_width,
										m_rect.width, m_rect.height * static_cast<int>(m_items.size()),
										m_hWndPar, NULL, NULL, NULL);
		NULL_RETURN(m_hWnd, 0);

		sfunComboboxWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&ComboboxProcHandle);
		sfunDropDownProc = (WNDPROC)SetWindowLongPtr(m_hWndDropDown, GWLP_WNDPROC, (LONG_PTR)&DropdownProcHandle);

		SetWindowLongPtr(m_hWndDropDown, GWLP_USERDATA, (LONG_PTR)this);

		this->RecalcTitleBarInfo();

		UpdateItems();

		return 1;
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
			case CBB_EVT_DROPDOWN:
			{
				m_eState = (m_eState == CBB_State::NORMAL)? CBB_State::DROPDOWN : CBB_State::NORMAL;

				::ShowWindow(m_hWndDropDown, (m_eState == CBB_State::NORMAL) ? SW_HIDE : SW_SHOW);

				if (m_eState == CBB_State::DROPDOWN)
				{
					this->OnDrawDropDown(TRUE);
				}

				this->Draw(true);

				break;
			}
			case CBB_EVT_HOVER_DROPDOWN:
			{
				m_iItemHover = lParam;

				if (m_iItemHover >= 0 || m_iItemHover == -2)
				{
					this->OnDrawDropDown(TRUE);
				}

				break;
			}
			case CBB_EVT_CLICK_DROPDOWN:
			{
				if (lParam >= 0)
				{
					m_eState = CBB_State::NORMAL;

					::ShowWindow(m_hWndDropDown, SW_HIDE);

					this->Draw(true);

					if (m_iSelected != lParam) // selected changed
					{
						m_iSelected = lParam;
						CHECK_RUN_FUNCTION(m_EventSelectedChangedFun, this);
					}
				}

				break;
			}

			default:
				break;
		}

		return 0;
	}

public:

	/*******************************************************************************
	*! @brief  : Draw title bar and button dropdown
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void OnDrawTitleBar()
	{
		// [1] Draw border title bar
		auto iBorderWidth  = UI_Background.border_width;
		auto iBorderRadius = UI_Background.border_radius;

		Gdiplus::Rect rect = m_rect_titlebar;

		// [2] Draw drop down icon combobox
		Gdiplus::SolidBrush brush_dropdown({ 255, 123, 0 });
		Gdiplus::Pen pen_border_dropdown(UI_Background.border_color.wrefcol, iBorderWidth);
		m_pRender->DrawRectangle(m_rect_btn_dropdown, &pen_border_dropdown, &brush_dropdown, iBorderRadius);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		std::wstring text_dropdown = (m_eState == CBB_State::NORMAL)? L"▼": L"▲";
		Gdiplus::SolidBrush dropdown_color(Gdiplus::Color(UI_Text.text_color.wrefcol));
		m_pRender->DrawTextRect(m_rect_btn_dropdown, text_dropdown.c_str(), &dropdown_color, &format);

		CHECK_RETURN(m_iSelected < 0, );

		const wchar_t* strSelect = L"";
		strSelect = m_items[m_iSelected].text.c_str();

		// [3] Draw text for combobox
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::SolidBrush text_color(Gdiplus::Color(UI_Text.text_color.wrefcol));
		m_pRender->DrawTextRect(m_rect_titlebar, strSelect, &text_color, &format, {5 ,0});
	}

	void DrawItem(int index)
	{
		if (index >= m_items.size())
			return;

		Gdiplus::Rect& rect_item = m_items[index].rect;

		Gdiplus::PointF pStart = { (float)rect_item.X +1		 , (float)rect_item.GetBottom() };
		Gdiplus::PointF pEnd   = { (float)rect_item.GetRight() -1, (float)rect_item.GetBottom() };

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		if (index == m_iItemHover)
		{
			Gdiplus::SolidBrush text_color(Gdiplus::Color(255, 0, 0));
			m_pRender->DrawTextRect(rect_item, m_items[index].text.c_str(), &text_color, &format, { 5 ,0 });
		}
		else
		{
			Gdiplus::SolidBrush text_color(Gdiplus::Color(UI_Text.text_color.wrefcol));
			m_pRender->DrawTextRect(rect_item, m_items[index].text.c_str(), &text_color, &format, { 5 ,0 });
		}

		if (index < m_items.size() -1)
		{
			Gdiplus::Pen pen(UI_Background.border_color.wrefcol);
			m_pRender->DrawLineFull(pStart, pEnd, &pen);
		}
	}

	void OnDrawDropDown(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(m_hWndDropDown, true));
		{
			if (m_eState == CBB_State::DROPDOWN)
			{
				// [2] Draw color button state
				auto iBorderRadius = UI_Background.border_radius;
				auto iBorderWidth = UI_Background.border_width;

				Gdiplus::Pen* pPen = new Gdiplus::Pen(UI_Background.border_color.wrefcol, iBorderWidth);
				Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(UI_Background.bk_color.wrefcol);

				// [1] Erase background color
				this->EraseBackground(m_pRender);

				// [2] Draw background combobox
				this->DrawBackground(m_pRender, pPen, pBrush, iBorderRadius);

				for (int i = 0; i < m_items.size(); i++)
				{
					DrawItem(i);
				}

				SAFE_DELETE(pPen);
				SAFE_DELETE(pBrush);
			}
		}
		m_pRender->EndDrawRect(bDraw);
	}

	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			// [2] Draw color button state
			auto iBorderRadius = UI_Background.border_radius;
			auto iBorderWidth = UI_Background.border_width;

			Gdiplus::Pen* pPen = new Gdiplus::Pen(UI_Background.border_color.wrefcol, iBorderWidth);
			Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(UI_Background.bk_color.wrefcol);

			// [1] Erase background color
			this->EraseBackground(m_pRender);

			// [2] Draw background combobox
			this->DrawBackground(m_pRender, pPen, pBrush, iBorderRadius);

			this->OnDrawTitleBar();

			this->OnDrawDropDown(false);

			SAFE_DELETE(pPen);
			SAFE_DELETE(pBrush);
		}
		m_pRender->EndDrawRect(bDraw);
	}
};

WNDPROC Combobox::sfunComboboxWndProc = NULL;
WNDPROC Combobox::sfunDropDownProc = NULL;

____END_NAMESPACE____

#endif // !XCOMBOBOX_H

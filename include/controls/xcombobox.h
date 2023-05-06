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
class Dllexport Combobox : public ControlBase , public ControlRectUI
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

	struct BTN_DROPDOWN
	{
		Gdiplus::Rect	rect;
	};

	struct CBB_ITEM
	{
		std::wstring	text = L""; // dữ liệu text hiển thị trên cbb
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
	Combobox(int _x = 0, int _y = 0, int _width = WIDTH_DEF,
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
		for (int i = 0; i < m_items.size(); i++)
		{
			delete m_items[i].data;
		}
	}

	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::COMBOBOX); }

private:

	void UpdateItems()
	{
		NULL_RETURN(m_hWnd, );

		this->RecalRectItems();
	}

	void UpdateSelect(int iSelect)
	{
		NULL_RETURN(m_hWnd, );
		if (iSelect >= 0 && iSelect < m_items.size())
		{
			m_iSelected = iSelect;
		}
		else
		{
			assert(0);
		}
	}

	//===================================================================================
	// Lấy ra chỉ số được select  : nó sẽ được lưu trữ  vào biến seleted                 
	//===================================================================================
	int GetSelectIndexItem()
	{
		if (!m_hWnd)
		{
			m_iSelected = -1;
		}
		else
		{
			m_iSelected = (int)SendMessage(m_hWnd, CB_GETCURSEL, NULL, NULL);
		}
		return m_iSelected;
	}

public:
	void SetEventSelectedChange(void(*fun)(Combobox*))
	{
		m_EventSelectedChangedFun = fun;
	}

	void SetSelect(int index)
	{
		m_iSelected = index;
	}

	void TextEdit(bool bEn)
	{
		m_bEditText = bEn;
	}

	void SetMinItemVisiable(int iMinVisible)
	{
		if (!m_hWnd) return;
		SendMessage((m_hWnd), CB_SETMINVISIBLE, (WPARAM)iMinVisible, 0);
	}

	// Chú ý cần clone và tạo data bằng new 
	void AddItem(std::wstring text, void* data = NULL)
	{
		CBB_ITEM    item;
		item.text = text;
		item.data = data;

		m_items.push_back(item);
	}

	//===================================================================================
	// Xóa một item được chỉ định bằng text : tất cả các item có text sẽ bị xóa          
	//===================================================================================
	void RemoveItem(std::wstring text)
	{
		for (auto it = m_items.begin(); it != m_items.end(); /*it++*/)
		{
			if (it->text == text)
			{
				delete it->data;
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

	//===================================================================================
	// Xóa toàn bộ item đang co trong combobxo                                           
	//===================================================================================
	void SelectItem(int sel)
	{
		UpdateSelect(sel);
	}

	//===================================================================================
	// Lấy text của item dựa vào index                                                   
	//===================================================================================
	std::wstring GetItemText(int index)
	{
		if (index < 0 || index >= m_items.size())
		{
			return L"";
		}

		return m_items[index].text;
	}

	//===================================================================================
	// Lấy giá trị của item                                                              
	//===================================================================================
	void* GetItemData(int index)
	{
		if (index < 0 || index >= m_items.size())
		{
			return NULL;
		}

		return m_items[index].data;
	}

	//===================================================================================
	// Lấy giá trị text của item selected                                                
	//===================================================================================
	std::wstring GetSelectText()
	{
		GetSelectIndexItem();
		if (m_iSelected < 0 || m_iSelected >= m_items.size())
		{
			return L"";
		}

		return m_items[m_iSelected].text;
	}

	//===================================================================================
	// Lấy dữ liệu item đang select                                                      
	//===================================================================================
	void* GetSelectData()
	{
		GetSelectIndexItem();

		if (m_iSelected < 0 || m_iSelected >= m_items.size())
		{
			return NULL;
		}

		return m_items[m_iSelected].data;
	}

	//===================================================================================
	// Lấy chỉ số của item selected                                                      
	//===================================================================================
	int GetSelectIndex()
	{
		GetSelectIndexItem();
		return m_iSelected;
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
				return TRUE;
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
				//btn->m_eState = btn->m_eOldState;
				break;
			}
			case WM_ERASEBKGND:
				return TRUE;
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
		NULL_RETURN(cbb, 0);

		if (wParam == PROCESS_MSG)
		{
			return cbb->OnProcessMessage(uMsg, wParam, lParam);
		}

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

				long x, y;
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
				return TRUE;
			}
			case WM_LBUTTONDOWN:
			{
				POINT cursor_pos = get_cursor_in_screen();
				if (ScreenToClient(cbb->m_hWndDropDown, &cursor_pos) == TRUE)
				{
					int iHover = cbb->GetItemHover(cursor_pos.x, cursor_pos.y);
					cbb->Send_Message(CBB_EVT_CLICK_DROPDOWN, PROCESS_MSG, iHover);
				}

				break;
			}
			case WM_LBUTTONUP:
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

		return CallWindowProc(sfunComboboxWndProc, hwndBtn, uMsg, wParam, lParam);
	}


protected:
	virtual void SetDefaultPropertyUI()
	{
		m_property.m_bk_color		= std::move(Color4(42, 65, 128));
		m_property.m_bk_hover_color	= std::move(Color4(229, 241, 255));
		m_property.m_click_color			= std::move(Color4(201, 224, 247));
		m_property.m_border_color			= std::move(Color4(255, 255, 255));

		m_property.border_radius	= 2;
		m_property.border_width		= 1;
		m_property.text_color		= std::move(Color4(255, 255, 255));
		m_property.text_hover_color = std::move(Color4(0, 0, 0));
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

	void RecalRectItems()
	{
		auto rect = this->GetRect(m_hWndDropDown, true);

		for (int i = 0; i < m_items.size(); i++)
		{
			Gdiplus::Rect rect_item({ rect.left, rect.top + (int)m_rect.height * i }, { (INT)m_rect.width, (INT)m_rect.height });
			m_items[i].rect = rect_item;
		}
	}

	void RecalTitleBarInfo()
	{
		auto rect = this->GetRect(true);

		m_rect_titlebar.X		= rect.left;
		m_rect_titlebar.Y		= rect.top;
		m_rect_titlebar.Width	= rect.right - rect.left;
		m_rect_titlebar.Height	= m_rect.height;

		m_rect_btn_dropdown.X = m_rect_titlebar.X + m_rect_titlebar.Width - m_rect_titlebar.Height - m_property.border_width +1;
		m_rect_btn_dropdown.Y = m_rect_titlebar.Y + m_property.border_width;
		m_rect_btn_dropdown.Width  = m_rect_titlebar.Height -  m_property.border_width;
		m_rect_btn_dropdown.Height = m_rect_titlebar.Height -  (m_property.border_width +1);
	}

protected:
	virtual int OnInitControl()
	{
		DWORD style = WS_CHILD | WS_VISIBLE | CBS_OWNERDRAWVARIABLE;

		if (m_bEditText) style |= CBS_DROPDOWN;
		else			 style |= CBS_DROPDOWNLIST;

		m_hWnd = CreateWindow(L"Combobox", NULL, style,			//
								(int)m_rect.x, (int)m_rect.y,	// x, y
								m_rect.width, m_rect.height,	// chiều rộng / chiều cao
								m_hWndPar,						// handle parent
								(HMENU)(UINT_PTR)m_ID,
								NULL, NULL);

		style = WS_CHILD | WS_VISIBLE | SS_OWNERDRAW;

		m_hWndDropDown = CreateWindow(L"STATIC", NULL, style,
										(int)m_rect.x, (int)m_rect.y + m_rect.height +1,
										m_rect.width, m_rect.height * static_cast<int>(m_items.size()),
										m_hWndPar, NULL, NULL, NULL);
		NULL_RETURN(m_hWnd, 0);

		Send_Message(CB_SETITEMHEIGHT, -1, m_rect.height - 6);

		sfunComboboxWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&ComboboxProcHandle);
		sfunDropDownProc = (WNDPROC)SetWindowLongPtr(m_hWndDropDown, GWLP_WNDPROC, (LONG_PTR)&DropdownProcHandle);

		SetWindowLongPtr(m_hWndDropDown, GWLP_USERDATA, (LONG_PTR)this);

		this->RecalTitleBarInfo();

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
					InvalidateRect(m_hWndDropDown, NULL, FALSE);
				}
				

				this->Draw(true);

				break;
			}
			case CBB_EVT_HOVER_DROPDOWN:
			{
				m_iItemHover = lParam;

				if (m_iItemHover >= 0 || m_iItemHover == -2)
				{
					InvalidateRect(m_hWndDropDown, NULL, FALSE);
				}

				break;
			}
			case CBB_EVT_CLICK_DROPDOWN:
			{
				m_iSelected = lParam;

				if (m_iSelected >= 0)
				{
					m_eState = CBB_State::NORMAL;
					::ShowWindow(m_hWndDropDown, SW_HIDE);
					InvalidateRect(m_hWndPar, NULL, FALSE);

					CHECK_RUN_FUNCTION(m_EventSelectedChangedFun, this);
				}

				break;
			}

			default:
				break;
		}

		return 0;
	}

public:

	void OnDrawTitleBar()
	{
		// [1] Draw border title bar
		int iBorderWidth  = m_property.border_width;
		int iBorderRadius = m_property.border_radius;

		Gdiplus::Rect rect = m_rect_titlebar;

		rect.X		+= 1;
		rect.Y		+= 1;
		rect.Width	-= iBorderWidth + 1;
		rect.Height -= iBorderWidth + 1;

		if (iBorderWidth > 0)
		{
			m_pRender->DrawRectangle(rect, m_property.m_border_color.wrefcol, nullptr, iBorderWidth, iBorderRadius);
		}

		// [4] Draw drop down icon combobox
		Gdiplus::SolidBrush brush_dropdown({ 255, 123, 0 });
		m_pRender->DrawRectangle(m_rect_btn_dropdown, nullptr, &brush_dropdown, m_property.border_radius);
		m_pRender->DrawRectangle(m_rect_btn_dropdown, m_property.m_border_color.wrefcol, nullptr, m_property.border_width, m_property.border_radius);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		std::wstring text_dropdown = (m_eState == CBB_State::NORMAL)? L"▼": L"▲";
		Gdiplus::SolidBrush dropdown_color(Gdiplus::Color(m_property.text_color.wrefcol));
		m_pRender->DrawTextRect(m_rect_btn_dropdown, text_dropdown.c_str(), &dropdown_color, &format);

		CHECK_RETURN(m_iSelected < 0, );

		const wchar_t* strSelect = L"";
		strSelect = m_items[m_iSelected].text.c_str();

		// [5] Draw text for combobox
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::SolidBrush text_color(Gdiplus::Color(m_property.text_color.wrefcol));
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
			std::wstring temp = L"ᐅ " + m_items[index].text;
			Gdiplus::SolidBrush text_color(Gdiplus::Color(255, 0, 0));
			m_pRender->DrawTextRect(rect_item, temp.c_str(), &text_color, &format, { 5 ,0 });
		}
		else
		{
			Gdiplus::SolidBrush text_color(Gdiplus::Color(m_property.text_color.wrefcol));
			m_pRender->DrawTextRect(rect_item, m_items[index].text.c_str(), &text_color, &format, { 5 ,0 });
		}

		if (index < m_items.size() -1)
		{
			Gdiplus::Pen pen(m_property.m_border_color.wrefcol);
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
				// [1] Erase background color
				this->DrawEraseBackground(m_pRender);

				// [2] Draw background combobox
				this->DrawFillBackground(m_pRender);

				this->DrawBorderBackground(m_pRender);

				for (int i = 0; i < m_items.size(); i++)
				{
					DrawItem(i);
				}
			}
		}
		m_pRender->EndDrawRect(bDraw);
	}

	virtual void Draw(bool bDraw = false)
	{
		NULL_RETURN(m_pRender, );

		m_pRender->BeginDrawRect(this->GetRect(true));
		{
			// [1] Erase background color
			this->DrawEraseBackground(m_pRender);

			// [2] Draw background combobox
			this->DrawFillBackground(m_pRender);

			this->DrawBorderBackground(m_pRender);

			this->OnDrawTitleBar();

			this->OnDrawDropDown(false);
		}
		m_pRender->EndDrawRect(bDraw);
	}
};

WNDPROC Combobox::sfunComboboxWndProc = NULL;
WNDPROC Combobox::sfunDropDownProc = NULL;

____END_NAMESPACE____

#endif // !XCOMBOBOX_H

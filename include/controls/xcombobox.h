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

___BEGIN_NAMESPACE___

/***********************************************************************************
* ⮟⮟ Class name: Combobox control
* Combobox control for window
***********************************************************************************/
class Dllexport Combobox : public ControlBase , public ControlRectUI
{
	enum { WIDTH_DEF = 100 };
	enum { HEIGHT_DEF = 50 };

	struct CBB_ITEM
	{
		std::wstring  text = L""; // dữ liệu text hiển thị trên cbb
		void*         data = NULL; // dữ liêu của item tự định nghĩa và kiểm soát
	};

	typedef std::vector<CBB_ITEM> ItemList;
private:
	bool				m_bEditText;

	int					m_iSelected;
	ItemList			m_items;
	GDIplusCtrlRender	m_render;

	static WNDPROC		sfunComboboxWndProc;


	void(*m_EventSelectedChangedFun)(Window*, Combobox*) = NULL;

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
		if (!m_hWnd) return;

		SendMessage(m_hWnd, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		for (int i = 0; i < m_items.size(); i++)
		{
			SendMessage(m_hWnd, CB_ADDSTRING, i, (LPARAM)m_items[i].text.c_str());
		}

		//SetWindowPos(NULL, NULL, m_x, m_y, m_width, m_height,SWP_NOSIZE|SWP_NOZORDER);

		UpdateSelect();
	}

	void UpdateSelect()
	{
		if (!m_hWnd) return;
		SendMessage(m_hWnd, CB_SETCURSEL, (WPARAM)m_iSelected, (LPARAM)0);
		m_iSelected = GetSelectIndexItem();
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
	void SetEventSelectedChange(void(*fun)(Window*, Combobox*))
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
		if (sel < 0 || sel >= m_items.size())
		{
			sel = -1;
		}
		UpdateSelect();
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

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			break;
		}
		case WM_MOUSELEAVE:
		{
			InvalidateRect(hwndBtn, NULL, FALSE);
			break;
		}
		case WM_TIMER:
		{
			//btn->OnTimer(wParam);
			break;
		}
		case WM_DRAWITEM:
		{
			int a = 10;
			return TRUE;
		}
		case WM_MEASUREITEM:
		{
			return TRUE;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(cbb->m_hWnd, &ps);

			cbb->OnPaint(ps);

			return EndPaint(cbb->m_hWnd, &ps);
		}
		case CBN_DROPDOWN:
		{
			int d = 100;
			return TRUE;
		}
		case WM_LBUTTONDOWN:
		{
			//btn->SetState(BtnState::Click);
			break;
		}
		//case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		{
			//btn->m_eState = btn->m_eOldState;
			break;
		}
		case WM_ERASEBKGND:
			return TRUE;
			break;
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
		m_property.m_background_color	= std::move(Color4(42, 65, 128));
		m_property.m_hover_color		= std::move(Color4(229, 241, 255));
		m_property.m_click_color		= std::move(Color4(201, 224, 247));
		m_property.m_border_color		= std::move(Color4(255, 255, 255));

		m_property.border_radius	= 3;
		m_property.border_width		= 1;
		m_property.text_color		= std::move(Color4(255, 255, 255));
		m_property.text_hover_color = std::move(Color4(0, 0, 0));
	}

protected:
	virtual int OnInitControl()
	{
		DWORD style = WS_CHILD | WS_VISIBLE | CBS_OWNERDRAWFIXED;

		if (m_bEditText) style |= CBS_DROPDOWN;
		else			 style |= CBS_DROPDOWNLIST;

		m_hWnd = CreateWindow(L"Combobox", NULL, style,			//
								(int)m_rect.x, (int)m_rect.y,	// x, y
								m_rect.width, m_rect.height,	// chiều rộng / chiều cao
								m_hWndPar,						// handle parent
								(HMENU)(UINT_PTR)m_ID,
								NULL, NULL);
		NULL_RETURN(m_hWnd, 0);

		sfunComboboxWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)&ComboboxProcHandle);

		UpdateItems();

		return 1;
	}

	virtual void Event(Window* window, WORD _id, WORD _event)
	{
		if (_event == CBN_SELCHANGE)
		{
			GetSelectIndexItem();
			if (m_EventSelectedChangedFun)  m_EventSelectedChangedFun(window, this);
		}
	}

public:
	void OnPaint(PAINTSTRUCT& ps)
	{
		m_render.Init(ps.hdc, ps.rcPaint);
		m_render.LoadFont(L"Consolas");

		// [1] Erase background color
		this->DrawEraseBackground(&m_render);

		// [2] Draw background combobox
		this->DrawFillBackground(&m_render);

		// [3] Draw border combobox
		this->DrawBorderBackground(&m_render);

		const wchar_t* strSelect = L"";

		if (m_iSelected < 0)
		{
			return;
		}

		strSelect = m_items[m_iSelected].text.c_str();

		// [4] Draw text for combobox
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(m_property.text_color.wrefcol));
		m_render.DrawTextFullRect(strSelect, &hover_textcolor, &format, {5 ,0});


		// [5] Draw drop down icon combobox
		format.SetAlignment(Gdiplus::StringAlignmentFar);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		m_render.DrawTextFullRect(L"▼", &hover_textcolor, &format, { -2 ,0 });

		m_render.Flush();
	}

	void Draw(LPDRAWITEMSTRUCT& pdis)
	{
		return;

		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
		m_render.Init(pdis->hDC, pdis->rcItem);
		m_render.LoadFont(L"Segoe UI");


		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0));

		Gdiplus::Rect rect = ConvertToRect(pdis->rcItem);
		rect.X -= 1;
		rect.Y -= 1;
		rect.Width += 100;
		rect.Height += 100;
		m_render.DrawRectangle(rect, nullptr, &brush, 0);

		//// [2] Draw color button state
		//const unsigned int iRadius      = m_property.border_radius;
		//const unsigned int iBorderWidth = m_property.border_width;

		//Gdiplus::Brush* background_color = NULL;
		//Gdiplus::Pen*	pen_color		 = NULL;

		//if (m_eState == BtnState::Click)
		//{
		//	background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
		//	pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
		//}
		//else if (m_eState == BtnState::Hover)
		//{
		//	background_color = new Gdiplus::SolidBrush(m_property.m_hover_color.wrefcol);
		//	pen_color = new Gdiplus::Pen(m_property.m_background_color.wrefcol, iBorderWidth);
		//}
		//else
		//{
		//	background_color = new Gdiplus::SolidBrush(m_cur_background_color);
		//	pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 253), iBorderWidth);
		//}

		//// Fill erase background
		//Gdiplus::Rect rect = ConvertToRect(pdis->rcItem);
		//rect.X		-= 1;
		//rect.Y		-= 1;
		//rect.Width  += 1;
		//rect.Height += 1;
		//m_render.DrawRectangle(rect, NULL, m_property.m_erase_color.wrefcol, 0);

		//// Fill rectangle background;
		//rect = ConvertToRect(pdis->rcItem);
		//if (iBorderWidth <= 0)
		//{
		//	rect.X -= 1;
		//	rect.Y -= 1;

		//	rect.Width -= iBorderWidth - 1;
		//	rect.Height -= iBorderWidth - 1;
		//}
		//else
		//{
		//	rect.X += 1;
		//	rect.Y += 1;

		//	rect.Width	-= iBorderWidth +1;
		//	rect.Height -= iBorderWidth +1;
		//}


		//m_render.DrawRectangle(rect, NULL, background_color, iRadius);

		//// Draw rectangle background;
		//if (iBorderWidth > 0)
		//{
		//	rect = ConvertToRect(pdis->rcItem);
		//	rect.X		+= 1;
		//	rect.Y		+= 1;
		//	rect.Width	-= iBorderWidth+1;
		//	rect.Height -= iBorderWidth+1;

		//	m_render.DrawRectangle(rect, pen_color, nullptr, iRadius);
		//}
		//	

		//SAFE_DELETE(pen_color);
		//SAFE_DELETE(background_color);

		//// [2] Draw image
		//if (m_image)
		//{
		//	GdiplusEx::ImageFormat imgformat;
		//	imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
		//	imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
		//	m_render.DrawImageFullRect(m_image, &imgformat);
		//}

		//// [3] Draw text for button
		//Gdiplus::StringFormat format;
		//format.SetAlignment(Gdiplus::StringAlignmentCenter);
		//format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		//if (m_eState == BtnState::Hover)
		//{
		//	Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(m_property.text_hover_color.wrefcol)); // color text normal
		//	m_render.DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &format);
		//}
		//else
		//{
		//	Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(m_property.text_color.wrefcol)); // color text normal
		//	m_render.DrawTextFullRect(this->m_sLabel.c_str(), &normal_textcolor, &format);
		//}

		m_render.Flush(true);
	}

	
	void Draw2( RECT _rect)
	{
		HDC hdc = GetDC(m_hWnd);
		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
		m_render.Init(hdc, _rect);
		m_render.LoadFont(L"Segoe UI");


		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0));

		Gdiplus::Rect rect = ConvertToRect(_rect);
		rect.X -= 1;
		rect.Y -= 1;
		rect.Width += 1;
		rect.Height += 1;
		m_render.DrawRectangle(rect, nullptr, &brush, 0);

		

		//// [2] Draw color button state
		//const unsigned int iRadius      = m_property.border_radius;
		//const unsigned int iBorderWidth = m_property.border_width;

		//Gdiplus::Brush* background_color = NULL;
		//Gdiplus::Pen*	pen_color		 = NULL;

		//if (m_eState == BtnState::Click)
		//{
		//	background_color = new Gdiplus::SolidBrush(m_property.m_click_color.wrefcol);
		//	pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 98, 162, 228), iBorderWidth);
		//}
		//else if (m_eState == BtnState::Hover)
		//{
		//	background_color = new Gdiplus::SolidBrush(m_property.m_hover_color.wrefcol);
		//	pen_color = new Gdiplus::Pen(m_property.m_background_color.wrefcol, iBorderWidth);
		//}
		//else
		//{
		//	background_color = new Gdiplus::SolidBrush(m_cur_background_color);
		//	pen_color = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 253), iBorderWidth);
		//}

		//// Fill erase background
		//Gdiplus::Rect rect = ConvertToRect(pdis->rcItem);
		//rect.X		-= 1;
		//rect.Y		-= 1;
		//rect.Width  += 1;
		//rect.Height += 1;
		//m_render.DrawRectangle(rect, NULL, m_property.m_erase_color.wrefcol, 0);

		//// Fill rectangle background;
		//rect = ConvertToRect(pdis->rcItem);
		//if (iBorderWidth <= 0)
		//{
		//	rect.X -= 1;
		//	rect.Y -= 1;

		//	rect.Width -= iBorderWidth - 1;
		//	rect.Height -= iBorderWidth - 1;
		//}
		//else
		//{
		//	rect.X += 1;
		//	rect.Y += 1;

		//	rect.Width	-= iBorderWidth +1;
		//	rect.Height -= iBorderWidth +1;
		//}


		//m_render.DrawRectangle(rect, NULL, background_color, iRadius);

		//// Draw rectangle background;
		//if (iBorderWidth > 0)
		//{
		//	rect = ConvertToRect(pdis->rcItem);
		//	rect.X		+= 1;
		//	rect.Y		+= 1;
		//	rect.Width	-= iBorderWidth+1;
		//	rect.Height -= iBorderWidth+1;

		//	m_render.DrawRectangle(rect, pen_color, nullptr, iRadius);
		//}
		//	

		//SAFE_DELETE(pen_color);
		//SAFE_DELETE(background_color);

		//// [2] Draw image
		//if (m_image)
		//{
		//	GdiplusEx::ImageFormat imgformat;
		//	imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
		//	imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
		//	m_render.DrawImageFullRect(m_image, &imgformat);
		//}

		//// [3] Draw text for button
		//Gdiplus::StringFormat format;
		//format.SetAlignment(Gdiplus::StringAlignmentCenter);
		//format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		//if (m_eState == BtnState::Hover)
		//{
		//	Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(m_property.text_hover_color.wrefcol)); // color text normal
		//	m_render.DrawTextFullRect(this->m_sLabel.c_str(), &hover_textcolor, &format);
		//}
		//else
		//{
		//	Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(m_property.text_color.wrefcol)); // color text normal
		//	m_render.DrawTextFullRect(this->m_sLabel.c_str(), &normal_textcolor, &format);
		//}

		m_render.Flush(true);
		ReleaseDC(m_hWnd, hdc);
	}
};

WNDPROC Combobox::sfunComboboxWndProc = NULL;

____END_NAMESPACE____

#endif // !XCOMBOBOX_H

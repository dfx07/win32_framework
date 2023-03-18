////////////////////////////////////////////////////////////////////////////////////
///*!*********************************************************************************
//* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
//*            All rights reserved.
//************************************************************************************
//* @file     wcontrol.h
//* @create   Nov 15, 2022
//* @brief    Create and handle event control the window
//* @note     For conditions of distribution and use, see copyright notice in readme.txt
//************************************************************************************/
//#ifndef WCONTROL_H
//#define WCONTROL_H
//
//#include <gdiplus.h>
//#include <CommCtrl.h>
//
//#include "xeasing.h"
//#include "wbase.h"
//
// disable waring visual studio c++14
//#pragma warning( disable : 26495)
//#pragma warning( disable : 26812)
//
//
//___BEGIN_NAMESPACE___
//
///// ////////////////////////////////////////////////////////////////////////////////
///// Control class definition and declaration
///// ////////////////////////////////////////////////////////////////////////////////
///// 
//class MenuContext;
//class MenuBar;
//class Window;
//
//
///**********************************************************************************
//* ⮟⮟ Class name: MenuBarContext control
//* MenuBarContext control for window
//***********************************************************************************/
//class Dllexport MenuBarItem
//{
//protected:
//
//	HMENU						m_hMenu;
//	std::wstring				m_text;
//
//	std::vector<MenuItemBase>	m_items;
//
//public:
//	MenuBarItem(std::wstring lab = L"") : m_text(lab),
//		m_hMenu()
//	{
//
//	}
//
//private:
//
//	/***************************************************************************
//	*! @brief  : Init Menubar control
//	*! @return : number of menu created
//	*! @author : thuong.nv          - [Date] : 05/03/2023
//	***************************************************************************/
//	int CreateMenuBarItem(UINT IDS)
//	{
//		m_hMenu = CreateMenu();
//		int nItemCreated = 0;
//
//		for (int i = 0; i < m_items.size(); i++)
//		{
//			m_items[i].SetID(IDS++);
//
//			if (m_items[i].GetType() == MF_SEPARATOR)
//			{
//				nItemCreated += (AppendMenu(m_hMenu, MF_SEPARATOR, m_items[i].GetID(), NULL) == TRUE) ? 1 :0;
//			}
//			else
//			{
//				nItemCreated += (AppendMenu(m_hMenu, m_items[i].GetType(), m_items[i].GetID(),
//					m_items[i].GetLabel().c_str()) == TRUE) ? 1 : 0;
//			}
//		}
//
//		return nItemCreated;
//	}
//
//public:
//
//	void AddItem(MenuItemBase item)
//	{
//		m_items.push_back(item);
//	}
//
//	void SetText(std::wstring txt)
//	{
//		m_text = txt;
//	}
//
//	bool ContainID(INT ID)
//	{
//		for (int i = 0; i < m_items.size(); i++)
//		{
//			if (m_items[i].m_ID == ID) return true;
//		}
//		return false;
//	}
//
//	bool CallEvent(Window* win, Control* ctrl, INT ID)
//	{
//		for (int i = 0; i < m_items.size(); i++)
//		{
//			if (m_items[i].m_ID == ID)
//			{
//				m_items[i].m_EventFun(win, ctrl);
//				return true;
//			}
//		}
//		return false;
//	}
//
//	friend class MenuBar;
//};
//
//class Dllexport MenuBar : public Control
//{
//public:
//	HMENU					 m_hMenuBar;
//	std::vector<MenuBarItem> m_items;
//
//public:
//	MenuBar() : Control(), m_hMenuBar()
//	{
//
//	}
//
//public:
//	virtual ControlType GetType() { return ControlType::MENUBAR; };
//
//	/***************************************************************************
//	*! @brief  : Init Menubar control
//	*! @return : number of control created
//	*! @author : thuong.nv          - [Date] : 05/03/2023
//	***************************************************************************/
//	virtual int OnInitControl()
//	{
//		m_hMenuBar = ::CreateMenu();
//		UINT uiStartID = m_ID + 1;
//
//		for (int i = 0; i < m_items.size(); i++)
//		{
//			int nMenuCreated = m_items[i].CreateMenuBarItem(uiStartID);
//			if (nMenuCreated > 0)
//			{
//				uiStartID += nMenuCreated;
//				AppendMenu(m_hMenuBar, MF_POPUP, (UINT_PTR)m_items[i].m_hMenu, m_items[i].m_text.c_str());
//				SetMenu(m_hwndPar, m_hMenuBar);
//			}
//		}
//
//		return (uiStartID - m_ID);
//	}
//
//public:
//
//	void AddItem(MenuBarItem item)
//	{
//		m_items.push_back(item);
//	}
//
//	virtual void Event(Window* window, WORD _id, WORD _event)
//	{
//		for (int i = 0; i < m_items.size(); i++)
//		{
//			if (m_items[i].CallEvent(window, this, _id))
//				return;
//		}
//	}
//
//	bool ContainID(INT ID)
//	{
//		for (int i = 0; i < m_items.size(); i++)
//		{
//			if (m_items[i].ContainID(ID))
//				return true;
//		}
//		return false;
//	}
//
//	friend class Window;
//};
//
//
///**********************************************************************************
//* ⮟⮟ Class name: Combobox control
//* Combobox control for window
//***********************************************************************************/
//
//class Dllexport Combobox : public Control
//{
//	enum { WIDTH_DEF = 100 };
//	enum { HEIGHT_DEF = 50 };
//
//	struct CBB_ITEM
//	{
//		std::wstring  text = L""; // dữ liệu text hiển thị trên cbb
//		void*         data = NULL; // dữ liêu của item tự định nghĩa và kiểm soát
//	};
//
//private:
//	ControlRect		 m_rect;
//	bool             m_editText;
//
//	int              selected;
//	std::vector<CBB_ITEM> items;
//
//
//	void(*m_EventSelectedChangedFun)(Window*, Combobox*) = NULL;
//
//public:
//	Combobox(int _x = 0, int _y = 0, int _width = WIDTH_DEF,
//		int _height = HEIGHT_DEF) :Control()
//	{
//		m_rect.Set(_x, _y, _width, _height);
//		m_editText = false;
//		selected = -1;
//	}
//
//	~Combobox()
//	{
//		for (int i = 0; i < items.size(); i++)
//		{
//			delete items[i].data;
//		}
//	}
//
//	virtual ControlType GetType() { static_cast<ControlType>(ControlType::COMBOBOX); }
//
//private:
//
//	void UpdateItems()
//	{
//		if (!m_hwnd) return;
//
//		SendMessage(m_hwnd, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
//
//		for (int i = 0; i < items.size(); i++)
//		{
//			SendMessage(m_hwnd, CB_ADDSTRING, i, (LPARAM)items[i].text.c_str());
//		}
//
//		//SetWindowPos(NULL, NULL, m_x, m_y, m_width, m_height,SWP_NOSIZE|SWP_NOZORDER);
//
//		UpdateSelect();
//	}
//
//	void UpdateSelect()
//	{
//		if (!m_hwnd) return;
//		SendMessage(m_hwnd, CB_SETCURSEL, (WPARAM)selected, (LPARAM)0);
//		selected = GetSelectIndexItem();
//	}
//
//	//===================================================================================
//	// Lấy ra chỉ số được select  : nó sẽ được lưu trữ  vào biến seleted                 
//	//===================================================================================
//	int GetSelectIndexItem()
//	{
//		if (!m_hwnd)
//		{
//			selected = -1;
//		}
//		else
//		{
//			selected = (int)SendMessage(m_hwnd, CB_GETCURSEL, NULL, NULL);
//		}
//		return selected;
//	}
//
//public:
//	void SetEventSelectedChange(void(*fun)(Window*, Combobox*))
//	{
//		m_EventSelectedChangedFun = fun;
//	}
//
//	void SetSelect(int index)
//	{
//		selected = index;
//	}
//
//	void TextEdit(bool bEn)
//	{
//		m_editText = bEn;
//	}
//
//	void SetMinItemVisiable(int iMinVisible)
//	{
//		if (!m_hwnd) return;
//		SendMessage((m_hwnd), CB_SETMINVISIBLE, (WPARAM)iMinVisible, 0);
//	}
//
//	// Chú ý cần clone và tạo data bằng new 
//	void AddItem(std::wstring text, void* data = NULL)
//	{
//		CBB_ITEM    item;
//		item.text = text;
//		item.data = data;
//
//		items.push_back(item);
//	}
//
//	//===================================================================================
//	// Xóa một item được chỉ định bằng text : tất cả các item có text sẽ bị xóa          
//	//===================================================================================
//	void RemoveItem(std::wstring text)
//	{
//		for (auto it = items.begin(); it != items.end(); /*it++*/)
//		{
//			if (it->text == text)
//			{
//				delete it->data;
//				it = items.erase(it);
//			}
//			else ++it;
//		}
//
//		UpdateItems();
//	}
//
//	//===================================================================================
//	// Xóa một item được chỉ định bằng index : tất cả các item có text sẽ bị xóa         
//	//===================================================================================
//	void RemoveItem(int index)
//	{
//		if (index < 0 || index >= items.size())
//		{
//			return;
//		}
//
//		delete items[index].data;
//		items.erase(items.begin() + index);
//
//		UpdateItems();
//	}
//
//	//===================================================================================
//	// Xóa toàn bộ item đang co trong combobxo                                           
//	//===================================================================================
//	void RemoveAllItem()
//	{
//		for (int i = 0; i < items.size(); i++)
//		{
//			delete items[i].data;
//		}
//		items.clear();
//
//		UpdateItems();
//	}
//
//	//===================================================================================
//	// Xóa toàn bộ item đang co trong combobxo                                           
//	//===================================================================================
//	void SelectItem(int sel)
//	{
//		if (sel < 0 || sel >= items.size())
//		{
//			sel = -1;
//		}
//		UpdateSelect();
//	}
//
//	//===================================================================================
//	// Lấy text của item dựa vào index                                                   
//	//===================================================================================
//	std::wstring GetItemText(int index)
//	{
//		if (index < 0 || index >= items.size())
//		{
//			return L"";
//		}
//
//		return items[index].text;
//	}
//
//	//===================================================================================
//	// Lấy giá trị của item                                                              
//	//===================================================================================
//	void* GetItemData(int index)
//	{
//		if (index < 0 || index >= items.size())
//		{
//			return NULL;
//		}
//
//		return items[index].data;
//	}
//
//	//===================================================================================
//	// Lấy giá trị text của item selected                                                
//	//===================================================================================
//	std::wstring GetSelectText()
//	{
//		GetSelectIndexItem();
//		if (selected < 0 || selected >= items.size())
//		{
//			return L"";
//		}
//
//		return items[selected].text;
//	}
//
//	//===================================================================================
//	// Lấy dữ liệu item đang select                                                      
//	//===================================================================================
//	void* GetSelectData()
//	{
//		GetSelectIndexItem();
//
//		if (selected < 0 || selected >= items.size())
//		{
//			return NULL;
//		}
//
//		return items[selected].data;
//	}
//
//	//===================================================================================
//	// Lấy chỉ số của item selected                                                      
//	//===================================================================================
//	int GetSelectIndex()
//	{
//		GetSelectIndexItem();
//		return selected;
//	}
//
//public:
//	virtual int OnInitControl()
//	{
//		DWORD style = WS_CHILD | WS_VISIBLE;
//
//		if (m_editText) style |= CBS_DROPDOWN;
//		else            style |= CBS_DROPDOWNLIST;
//
//		m_hwnd = CreateWindow(L"Combobox", NULL, style,								//
//								(int)m_rect.m_x, (int)m_rect.m_y,					// x, y
//								m_rect.m_width, m_rect.m_height*(int)items.size(),	// chiều rộng / chiều cao
//								m_hwndPar,											// Handle cha
//								(HMENU)(UINT_PTR)m_ID,
//								NULL, NULL);
//
//		NULL_RETURN(m_hwnd, 0);
//
//		UpdateItems();
//		return 1;
//	}
//
//	virtual void Event(Window* window, WORD _id, WORD _event)
//	{
//		if (_event == CBN_SELCHANGE)
//		{
//			GetSelectIndexItem();
//			if (m_EventSelectedChangedFun)  m_EventSelectedChangedFun(window, this);
//		}
//	}
//};
//
///**********************************************************************************
//* ⮟⮟ Class name: Label control
//* Label control for window
//***********************************************************************************/
//class Dllexport Label : public Control
//{
//private:
//	std::wstring m_text;
//	Color3		 m_color;
//
//private:
//	void UpdateText()
//	{
//		if (!m_hwnd) return;
//		SetWindowText(m_hwnd, m_text.c_str());
//	}
//
//	void CalcTextSize(int width, int height)
//	{
//		SIZE size;
//		HDC hdc = GetDC(m_hwnd);
//		GetTextExtentPoint32(hdc, m_text.c_str(), (int)wcslen(m_text.c_str()), &size);
//		
//		width = size.cx;
//		height = size.cy;
//
//		ReleaseDC(m_hwnd, hdc);
//	}
//public:
//	void UpdateTextColor(HDC hdc)
//	{
//		if (!hdc) return;
//
//		SetTextColor(hdc, RGB(m_color.r, m_color.g, m_color.b));
//	}
//
//public:
//	Label(const wchar_t* text = L""): Control()
//	{
//		m_text = text;
//	}
//
//	virtual ControlType GetType() { return ControlType::LABEL; }
//
//public:
//	void SetText(const wchar_t* text)
//	{
//		m_text = text;
//		this->UpdateText();
//	}
//
//	void SetColor(float r, float g, float b)
//	{
//		m_color.r = r;
//		m_color.g = g;
//		m_color.b = b;
//
//		this->UpdateTextColor(NULL);
//	}
//
//public:
//	virtual int OnInitControl(UINT& IDS)
//	{
//		DWORD style = WS_VISIBLE | WS_CHILD;
//
//		m_hwnd = CreateWindow(L"STATIC", m_text.c_str(), style,		// style + control name
//					(int)m_rect.m_x, (int)m_rect.m_y,				// x, y
//					m_rect.m_width, m_rect.m_height,				// width / height
//					m_hwndPar,										// Handle parent
//					(HMENU)(UINT_PTR)m_ID,							// ID
//					NULL, NULL);
//
//		NULL_RETURN(m_hwnd, 0);
//
//		return 1;
//	}
//};
//
///**********************************************************************************
//* ⮟⮟ Class name: Checkbox control
//* Checkbox control for window
//***********************************************************************************/
//class Dllexport Checkbox : public Control, public RectPropertyUI
//{
//private:
//	std::wstring		m_sLabel;
//	Color3				m_color;
//	bool				m_bChecked;
//
//	GDIplusCtrlRender	m_render;
//
//	Gdiplus::Bitmap*	m_image_check;
//	Gdiplus::Bitmap*	m_image_uncheck;
//
//	Gdiplus::Brush*		m_background_normal;
//	Gdiplus::Brush*		m_backgroundclick;
//	Gdiplus::Brush*		m_backgroundhover;
//protected:
//
//	static WNDPROC& get_proc()
//	{
//		static WNDPROC prevWndProc;
//		return prevWndProc;
//	}
//
//public:
//	Checkbox(const wchar_t* text = L""): Control()
//	{
//		m_sLabel = text;
//		m_bChecked = false;
//
//		m_property.background_color		  = std::move(Color4(59, 91, 179));
//		m_property.background_hover_color = std::move(Color4(229, 241, 255));
//		m_property.background_click_color = std::move(Color4(201, 224, 247));
//
//		m_property.border_radius = 0;
//		m_property.border_width  = 0;
//		m_property.text_color = std::move(Color4(255, 255, 255));
//		m_property.text_hover_color = std::move(Color4(0, 0, 0));
//	}
//
//	~Checkbox()
//	{
//		delete m_background_normal;
//		delete m_backgroundhover;
//		delete m_backgroundclick;
//
//		delete m_image_check;
//		delete m_image_uncheck;
//	}
//
//	virtual ControlType GetType() { return ControlType::CHECKBOX; }
//
//public:
//
//	void SetText(const wchar_t* text = L"")
//	{
//		m_sLabel = text;
//	}
//
//	void SetCheck()
//	{
//
//	}
//
//	bool Checked()
//	{
//
//	}
//protected:
//	virtual void OnCreated()
//	{
//
//	}
//
//	void CalcTextSize(int width, int height)
//	{
//		SIZE size;
//		HDC hdc = GetDC(m_hwnd);
//		GetTextExtentPoint32(hdc, m_sLabel.c_str(), (int)wcslen(m_sLabel.c_str()), &size);
//
//		width  = size.cx;
//		height = size.cy;
//
//		ReleaseDC(m_hwnd, hdc);
//	}
//
//protected:
//	virtual int OnInitControl()
//	{
//		DWORD style = WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_OWNERDRAW;
//
//		CalcTextSize(m_rect.m_width, m_rect.m_height);
//
//		m_hwnd = CreateWindow(L"Button", m_sLabel.c_str(), style,	// style + control name
//					(int)m_rect.m_x, (int)m_rect.m_y,				// x, y
//					m_rect.m_width, m_rect.m_height,				// width / height
//					m_hwndPar,										// Handle parent
//					(HMENU)(UINT_PTR)m_ID,							// ID
//					NULL, NULL);
//
//		NULL_RETURN(m_hwnd, 0);
//
//		get_proc() = (WNDPROC)SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)&CheckBoxProcHandle);
//
//		return 1;
//	}
//
//	static LRESULT CALLBACK CheckBoxProcHandle(HWND hwndChk, UINT uMsg, WPARAM wParam, LPARAM lParam)
//	{
//		Checkbox* chk = (Checkbox*)::GetWindowLongPtr(hwndChk, GWLP_USERDATA);
//		if (!chk) return 0;
//
//		switch (uMsg)
//		{
//		case WM_MOUSEMOVE:
//		{
//			InvalidateRect(hwndChk, NULL, FALSE);
//			break;
//		}
//		case WM_MOUSELEAVE:
//		{
//			InvalidateRect(hwndChk, NULL, FALSE);
//			break;
//		}
//		case WM_TIMER:
//		{
//			break;
//		}
//		case WM_LBUTTONDOWN:
//		{
//			chk->UpdateCheck();
//		}
//		case NM_CLICK:
//			break;
//		case WM_RBUTTONUP:
//		case WM_LBUTTONUP:
//		{
//			break;
//		}
//		case WM_COMMAND:
//		{
//			break;
//		}
//		case WM_ERASEBKGND:
//			return TRUE;
//			break;
//		case WM_CTLCOLORBTN:
//		{
//			break;
//		}
//
//		}
//		return CallWindowProc(get_proc(), hwndChk, uMsg, wParam, lParam);
//	}
//protected:
//
//	void UpdateCheck()
//	{
//		m_bChecked = !m_bChecked;
//	}
//
//public:
//	void CreateColorButton()
//	{
//		if (!m_background_normal)
//		{
//			m_background_normal = new Gdiplus::SolidBrush(Gdiplus::Color(m_property.background_color.wrefcol));
//		}
//		if (!m_backgroundclick)
//		{
//			m_backgroundclick = new Gdiplus::SolidBrush(Gdiplus::Color(m_property.background_click_color.wrefcol));
//		}
//		if (!m_backgroundhover)
//		{
//			m_backgroundhover = new Gdiplus::SolidBrush(Gdiplus::Color(m_property.background_hover_color.wrefcol));
//		}
//		if (!m_image_uncheck)
//		{
//			m_image_uncheck = Gdiplus::Bitmap::FromFile(L"resources/chk_uncheck.png", false);
//			m_image_check = Gdiplus::Bitmap::FromFile(L"resources/chk_check.png", false);
//		}
//	}
//
//	void Draw(LPDRAWITEMSTRUCT& pdis)
//	{
//		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
//		m_render.Init(pdis->hDC, pdis->rcItem);
//		m_render.LoadFont(L"Segoe UI");
//
//		this->CreateColorButton();
//
//		// [2] Draw color button state
//		const unsigned int iRadius = m_property.border_radius;
//		const unsigned int iBorderWidth = m_property.border_width;
//
//		Gdiplus::Pen pen(m_property.background_color.wrefcol, iBorderWidth);
//		m_render.DrawRectangleFull(iBorderWidth > 0 ? &pen : NULL, m_background_normal, iRadius);
//
//		// [2] Draw image check
//		if (m_bChecked)
//		{
//			if (m_image_check)
//			{
//				GdiplusEx::ImageFormat imgformat;
//				imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
//				imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
//				m_render.DrawImageFullRect(m_image_check, &imgformat, Gdiplus::PointF(2, 0));
//			}
//		}
//		else
//		{
//			if (m_image_uncheck)
//			{
//				GdiplusEx::ImageFormat imgformat;
//				imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
//				imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
//				m_render.DrawImageFullRect(m_image_uncheck, &imgformat, Gdiplus::PointF(2, 0));
//			}
//		}
//
//		// [3] Draw text for button
//		Gdiplus::StringFormat format;
//		format.SetAlignment(Gdiplus::StringAlignmentNear);
//		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
//
//		Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(m_property.text_color.wrefcol)); // color text normal
//		m_render.DrawTextFullRect(m_sLabel.c_str(), &normal_textcolor, &format, Gdiplus::PointF(20, 0));
//
//		m_render.Flush(true);
//	}
//};
//
//____END_NAMESPACE____
//
//#endif // WCONTROL_H
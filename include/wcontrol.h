﻿////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     wcontrol.h
* @create   Nov 15, 2022
* @brief    Create and handle event control the window
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef WCONTROL_H
#define WCONTROL_H

#include <gdiplus.h>
#include <CommCtrl.h>

#include "xeasing.h"
#include "wdef.h"


___BEGIN_NAMESPACE___

/// ////////////////////////////////////////////////////////////////////////////////
/// Control class definition and declaration
/// ////////////////////////////////////////////////////////////////////////////////
/// 
class MenuContext;
class MenuBar;
class Window;

enum ControlType
{
	BUTTON,
	MENUCONTEXT,
	MENUBAR,
	COMBOBOX,
	LABEL,
};

struct ControlRect
{
	float     m_x;
	float     m_y;
	UINT      m_width;
	UINT      m_height;

public:
	void Set(float x, float y, UINT w, UINT h)
	{
		this->SetPos(x, y);
		this->SetSize(w, h);
	}

	void SetPos(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	void SetSize(UINT w, UINT h)
	{
		m_width = w;
		m_height = h;
	}
};

/**********************************************************************************
* ⮟⮟ Class name: Control base
* Base class for inherited window controls
***********************************************************************************/
class Dllexport Control
{
protected:
	UINT         m_ID;
	HWND         m_hwnd;
	HWND         m_hwndPar;

	bool         m_bEnable;
	bool         m_bVisble;

	ControlRect  m_rect;

public:
	Control(): m_hwnd(NULL), m_ID(0),
		m_hwndPar(NULL)
	{
		m_bEnable = true;
		m_bVisble = true;
	}

public:
	void SetPosition(int x, int y)
	{
		m_rect.SetPos(x, y);
	}

	void SetSize(int width, int height)
	{
		m_rect.SetSize(width, height);
	}

public:
	virtual int OnInitControl(UINT& IDS) // 0 false | 1 :true
	{
		if (!m_hwnd || m_ID ==0)
			return 0;
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		return 1;
	}

	virtual ControlType GetType() = 0;
	virtual int			IsCreated() { return m_hwnd ? TRUE : FALSE; }

	virtual void		OnDestroy() {};
	virtual void		Draw(LPDRAWITEMSTRUCT& ) {};
	virtual void		Event(Window* window, WORD _id, WORD _event) {};
	virtual bool		ContainID(INT ID) { return false; };

public:
	HWND GetHwnd()  { return m_hwnd; }
	INT  GetID()	{ return m_ID;	 }
	void SetParent(HWND hwnd) { m_hwndPar = hwnd; }
	void Enable(bool bEnable)
	{
		m_bEnable = bEnable;
		::EnableWindow(m_hwnd, m_bEnable ? TRUE : FALSE);
	}

	void Visible(bool bVisible)
	{
		m_bVisble = bVisible;
		::ShowWindow(m_hwnd, m_bVisble ? TRUE : FALSE);
	}
};


/**********************************************************************************
* ⮟⮟ Class name: MenuContext control
* MenuContext control for window
***********************************************************************************/
class Dllexport MenuItemBase
{
public:
	UINT			  m_ID;
	UINT			  m_type;
	std::wstring      m_label;

public:
	void(*m_EventFun)(Window* window, Control* ctrl) = NULL;

public:
	MenuItemBase()
	{
		m_ID = 0;
		m_label = L"";
	};

	void   SetEvent(void(*mn)(Window*, Control*)) { m_EventFun = mn; }
	void   SetLabel(std::wstring lab) { m_label = lab; }
	void   SetType(UINT type) { m_type = type; }
	void   SetID(UINT id) { m_ID = id; }

	std::wstring GetLabel() { return m_label; }
	UINT		 GetType()   { return m_type; }
	UINT		 GetID()     { return m_ID; }
};

class Dllexport MenuContext : public Control
{
protected:
	HMENU                      m_hMenu;
	std::vector<MenuItemBase>  m_items;

public:
	MenuContext() : Control(),
		m_hMenu()
	{

	}

private:
	bool CreateMenuContext(UINT& IDS)
	{
		m_hMenu = CreatePopupMenu();

		for (int i = 0; i < m_items.size(); i++)
		{
			m_items[i].SetID(IDS++);

			if (m_items[i].GetType() == MF_SEPARATOR)
			{
				AppendMenu(m_hMenu, MF_SEPARATOR, m_items[i].GetID(), NULL);
			}
			else
			{
				AppendMenu(m_hMenu, m_items[i].GetType(), m_items[i].GetID(), m_items[i].GetLabel().c_str());
			}
		}

		return true;
	}

public:
	virtual ControlType GetType() { return ControlType::MENUCONTEXT; };

	virtual int OnInitControl(UINT& IDS)
	{
		if (!CreateMenuContext(IDS))
		{
			return 0;
		}
		return Control::OnInitControl(IDS);
	}

	virtual void Event(Window* window, WORD _id, WORD _event)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].m_ID == _id && m_items[i].m_EventFun)
			{
				m_items[i].m_EventFun(window, this);
				break;
			}
		}
	}

	bool ContainID(INT ID)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].m_ID == ID) return true;
		}
		return false;
	}

public:
	void AddItem(const MenuItemBase& item)
	{
		m_items.push_back(item);
	}

	void Show(POINT point)
	{
		if (m_hwndPar)
		{
			ClientToScreen(m_hwndPar, &point);
			TrackPopupMenu(m_hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, m_hwndPar, NULL);
		}
	}

	friend class Window;
};

/**********************************************************************************
* ⮟⮟ Class name: MenuBarContext control
* MenuBarContext control for window
***********************************************************************************/
class Dllexport MenuBarItem
{
protected:

	HMENU						m_hMenu;
	std::string					m_text;

	std::vector<MenuItemBase>	m_items;

public:
	MenuBarItem(std::string lab = "") : m_text(lab),
		m_hMenu()
	{

	}

private:

	bool CreateMenuBarItem(UINT& IDS)
	{
		m_hMenu = CreateMenu();
		for (int i = 0; i < m_items.size(); i++)
		{
			m_items[i].SetID(IDS++);

			if (m_items[i].GetType() == MF_SEPARATOR)
			{
				AppendMenu(m_hMenu, MF_SEPARATOR, m_items[i].GetID(), NULL);
			}
			else
			{
				AppendMenu(m_hMenu, m_items[i].GetType(), m_items[i].GetID(), m_items[i].GetLabel().c_str());
			}
		}

		return true;
	}

public:

	void AddItem(MenuItemBase item)
	{
		m_items.push_back(item);
	}

	void SetText(std::string txt)
	{
		m_text = txt;
	}

	bool ContainID(INT ID)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].m_ID == ID) return true;
		}
		return false;
	}

	bool CallEvent(Window* win, Control* ctrl, INT ID)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].m_ID == ID)
			{
				m_items[i].m_EventFun(win, ctrl);
				return true;
			}
		}
		return false;
	}

	friend class MenuBar;
};

class Dllexport MenuBar : public Control
{
public:
	HMENU					 m_hMenuBar;
	std::vector<MenuBarItem> m_items;

public:
	MenuBar() : Control()
	{

	}

protected:

	bool CreateMenuBar(UINT& IDS)
	{
		m_hMenuBar = CreateMenu();

		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].CreateMenuBarItem(IDS))
			{
				// TODO : Created menubar ok
				AppendMenuA(m_hMenuBar, MF_POPUP, (UINT_PTR)m_items[i].m_hMenu, m_items[i].m_text.c_str());
				SetMenu(m_hwndPar, m_hMenuBar);
			}
			else
			{
				std::cout << "[X]  Create Menubar failed ! " << std::endl;
			}
		}
		return true;
	}

public:
	virtual ControlType GetType() { return ControlType::MENUBAR; };

	virtual int OnInitControl(UINT& IDS)
	{
		if (!CreateMenuBar(IDS))
		{
			return 0;
		}
		return Control::OnInitControl(IDS);
	}

public:

	void AddItem(MenuBarItem item)
	{
		m_items.push_back(item);
	}

	virtual void Event(Window* window, WORD _id, WORD _event)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].CallEvent(window, this, _id))
				return;
		}
	}

	bool ContainID(INT ID)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].ContainID(ID))
				return true;
		}
		return false;
	}
};

/**********************************************************************************
* ⮟⮟ Class name: Button control
* Button control for window
***********************************************************************************/
class Dllexport Button : public Control
{
	enum BtnState
	{
		Click,
		Normal,
		Hover,
	};

	enum { IDC_EFFECT_X1 = 12003 };
	enum { WIDTH_DEF	 = 80 };
	enum { HEIGHT_DEF	 = 25 };

private:
	bool		m_track_leave;

protected:
	ControlRect			m_rect;

	std::wstring		m_label;
	BtnState			m_eState;
	BtnState			m_eOldState;

	Gdiplus::Brush*		m_background_normal;
	Gdiplus::Brush*		m_backgroundclick;
	Gdiplus::Brush*		m_backgroundhover;

	Color4				m_normal_color;
	Color4				m_hover_color;
	Color4				m_hot_color;

	Gdiplus::Bitmap*	m_image;

	EasingEngine		m_easing;

	GDIplusCtrlRender	m_render;

	int					m_border_width;
	int					m_color_click;


	void(*m_EventFun)(Window* window, Button* btn) = NULL;
	void(*m_EventFunEnter)(Window* window, Button* btn) = NULL;
	void(*m_EventFunLeave)(Window* window, Button* btn) = NULL;


	static WNDPROC& getproc()
	{
		static WNDPROC prevWndProc;
		return prevWndProc;
	}

public:
	Button() : Control(),
		m_eState(BtnState::Normal)
	{
		m_rect.Set(0, 0, WIDTH_DEF, HEIGHT_DEF);
	}

	~Button()
	{
		delete m_background_normal;
		delete m_backgroundhover;
		delete m_backgroundclick;

		delete m_image;
	}

public:

	void SetLabel(std::wstring lab) { m_label = lab; }

public:
	int OnInitControl(UINT& IDS)
	{
		UINT BackupIDS = IDS;
		m_ID = IDS++;
		m_hwnd = (HWND)CreateWindow(L"BUTTON", m_label.c_str(),			// Button text 
						WS_CHILD | WS_VISIBLE | BS_OWNERDRAW  /*| BS_NOTIFY*/,
						m_rect.m_x,										// x position 
						m_rect.m_y,										// y position 
						m_rect.m_width,									// Button width
						m_rect.m_height,								// Button height
						m_hwndPar,										// Parent window
						(HMENU)(UINT_PTR)m_ID,							// menu.
						(HINSTANCE)GetWindowLongPtr(m_hwndPar, GWLP_HINSTANCE),
						NULL);
		if (!m_hwnd)  // Hoàn trả nếu nó khởi tạo không thành công
		{
			m_ID = 0;
			IDS = BackupIDS;
			return 0;
		}
		getproc() = (WNDPROC)SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)&ButtonProcHandle);

		return Control::OnInitControl(IDS);
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

	static LRESULT CALLBACK ButtonProcHandle(HWND hwndBtn, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Button* btn = (Button*)::GetWindowLongPtr(hwndBtn, GWLP_USERDATA);
		if (!btn) return 0;

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			if (btn->m_eState == BtnState::Click ||
				btn->m_eState == BtnState::Hover)
				break;

			btn->EndX1ThemeEffect();

			btn->SetState(BtnState::Hover);
			if (!btn->m_track_leave)
			{
				TrackMouse(hwndBtn);
				btn->m_track_leave = true;
			}

			InvalidateRect(hwndBtn, NULL, FALSE);
			break;
		}
		case WM_MOUSELEAVE:
		{
			btn->m_track_leave = false;
			btn->SetState(BtnState::Normal, true);
			btn->BeginX1ThemeEffect();
			InvalidateRect(hwndBtn, NULL, FALSE);
			break;
		}
		case WM_TIMER:
		{
			btn->OnTimer(wParam);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			btn->SetState(BtnState::Click);
			break;
		}
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		{
			btn->m_eState = btn->m_eOldState;
			break;
		}

		case WM_ERASEBKGND:
			return TRUE;
			break;
		case WM_CTLCOLORBTN: {
			//SetBkMode((HDC)wParam, TRANSPARENT);
			break;
		}
		}
		return CallWindowProc(getproc(), hwndBtn, uMsg, wParam, lParam);
	}

private:
	const float m_effect_time_update = 5;

	void BeginX1ThemeEffect()
	{
		::SetTimer(m_hwnd, IDC_EFFECT_X1, m_effect_time_update, (TIMERPROC)NULL);

		m_easing.AddExec(EaseType::Expo, EaseMode::In, 1, m_hover_color.r, m_normal_color.r);
		m_easing.AddExec(EaseType::Expo, EaseMode::In, 1, m_hover_color.g, m_normal_color.g);
		m_easing.AddExec(EaseType::Expo, EaseMode::In, 1, m_hover_color.b, m_normal_color.b);

		m_easing.Start();

		delete m_background_normal;
		m_background_normal = new Gdiplus::SolidBrush(m_hover_color.wrefcol);
	}

	bool UpdateX1ThemeEffect()
	{
		m_easing.Update(m_effect_time_update);

		auto r = m_easing.Value(0);
		auto g = m_easing.Value(1);
		auto b = m_easing.Value(2);

		delete m_background_normal;
		m_background_normal = new Gdiplus::SolidBrush(Gdiplus::Color(r, g, b));

		return m_easing.IsActive();
	}

	void EndX1ThemeEffect()
	{
		KillTimer(m_hwnd, IDC_EFFECT_X1);

		delete m_background_normal;
		m_background_normal = new Gdiplus::SolidBrush(m_normal_color.wrefcol);
	}

	virtual void OnTimer(DWORD wParam)
	{
		switch (wParam)
		{
			case IDC_EFFECT_X1:
			{
				if (this->UpdateX1ThemeEffect())
				{
					InvalidateRect(m_hwnd, NULL, FALSE);
					UpdateWindow(m_hwnd);
				}
				else
				{
					EndX1ThemeEffect();
					InvalidateRect(m_hwnd, NULL, FALSE);
				}

				break;
			}
		}
	}

public:
	virtual void Event(Window* window, WORD _id, WORD _event)
	{
		if (!m_EventFun) return;

		m_EventFun(window, this);
	}

	void SetEvent(void(*mn)(Window*, Button*))
	{
		m_EventFun = mn;
	}

	void SetState(BtnState state, bool free_oldstate = false)
	{
		m_eOldState = (free_oldstate) ? BtnState::Normal : m_eState;

		if ( m_EventFunEnter && state == BtnState::Hover)
		{
			m_EventFunEnter(NULL, this);
		}
		else if (m_EventFunLeave && state == BtnState::Normal)
		{
			m_EventFunLeave(NULL, this);
		}
		m_eState = state;
	}

	void SetEventEnterCallBack(void(*fun)(Window* window, Button* btn))
	{
		this->m_EventFunEnter = fun;
	}
	void SetEventLeaveCallBack(void(*fun)(Window* window, Button* btn))
	{
		this->m_EventFunLeave = fun;
	}

	void CreateColorButton()
	{
		if (!m_background_normal)
		{
			m_normal_color = std::move(Color4(59, 91, 179));
			m_background_normal = new Gdiplus::SolidBrush(Gdiplus::Color(m_normal_color.wrefcol));
		}
		if (!m_backgroundclick)
		{
			m_hot_color = std::move(Color4(201, 224, 247));
			m_backgroundclick = new Gdiplus::SolidBrush(Gdiplus::Color(m_hot_color.wrefcol));
		}
		if (!m_backgroundhover)
		{
			m_hover_color = std::move(Color4(229, 241, 255));
			m_backgroundhover = new Gdiplus::SolidBrush(Gdiplus::Color(m_hover_color.wrefcol));
		}
	}

	void Draw(LPDRAWITEMSTRUCT& pdis)
	{
		//TODO : draw use swap buffer image (hdc) -> not draw each element (OK)
		m_render.Init(pdis->hDC, pdis->rcItem);
		m_render.LoadFont(L"Segoe UI");
		
		this->CreateColorButton();

		// [2] Draw color button state
		const int radius = 3;
		if (m_eState == BtnState::Click)
		{
			Gdiplus::Pen pen(Gdiplus::Color(255, 98, 162, 228), 2);
			m_render.DrawRectangle(&pen, m_backgroundclick, radius);
		}
		else if (m_eState == BtnState::Hover)
		{
			Gdiplus::Pen pen(Gdiplus::Color(255, 255, 255, 255), 2);
			m_render.DrawRectangle(&pen, m_backgroundhover, radius);
		}
		else
		{
			Gdiplus::Pen pen(Gdiplus::Color(255, 255, 255, 255), 2);
			m_render.DrawRectangle(&pen, m_background_normal, radius);
		}

		// [2] Draw image
		if (m_image)
		{
			GdiplusEx::ImageFormat imgformat;
			imgformat.SetVerticalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentNear);
			imgformat.SetHorizontalAlignment(GdiplusEx::ImageAlignment::ImageAlignmentCenter);
			m_render.DrawImageFullRect(m_image, &imgformat);
		}

		// [3] Draw text for button
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		if (m_eState == BtnState::Hover)
		{
			Gdiplus::SolidBrush hover_textcolor(Gdiplus::Color(255, 255, 255, 255)); // color text normal
			m_render.DrawTextFullRect(this->m_label.c_str(), &hover_textcolor, &format);
		}
		else
		{
			Gdiplus::SolidBrush normal_textcolor(Gdiplus::Color(255, 255, 255, 255)); // color text normal
			m_render.DrawTextFullRect(this->m_label.c_str(), &normal_textcolor, &format);
		}

		m_render.Flush(true);
	}

	virtual ControlType GetType() { return ControlType::BUTTON; };
};

/**********************************************************************************
* ⮟⮟ Class name: Combobox control
* Combobox control for window
***********************************************************************************/


class Dllexport Combobox : public Control
{
	enum { WIDTH_DEF = 100 };
	enum { HEIGHT_DEF = 50 };

	struct CBB_ITEM
	{
		std::wstring     text; // dữ liệu text hiển thị trên cbb
		void*            data; // dữ liêu của item tự định nghĩa và kiểm soát
	};

private:
	ControlRect		 m_rect;
	bool             m_editText;

	int              selected;
	std::vector<CBB_ITEM> items;


	void(*m_EventSelectedChangedFun)(Window*, Combobox*) = NULL;

public:
	Combobox(int _x = 0, int _y = 0, int _width = WIDTH_DEF,
		int _height = HEIGHT_DEF) :Control()
	{
		m_rect.Set(_x, _y, _width, _height);
		m_editText = false;
		selected = -1;
	}

	~Combobox()
	{
		for (int i = 0; i < items.size(); i++)
		{
			delete items[i].data;
		}
	}
private:

	void UpdateItems()
	{
		if (!m_hwnd) return;

		SendMessage(m_hwnd, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		for (int i = 0; i < items.size(); i++)
		{
			SendMessage(m_hwnd, CB_ADDSTRING, i, (LPARAM)items[i].text.c_str());
		}

		//SetWindowPos(NULL, NULL, m_x, m_y, m_width, m_height,SWP_NOSIZE|SWP_NOZORDER);

		UpdateSelect();
	}

	void UpdateSelect()
	{
		if (!m_hwnd) return;
		SendMessage(m_hwnd, CB_SETCURSEL, (WPARAM)selected, (LPARAM)0);
		selected = GetSelectIndexItem();
	}

	//===================================================================================
	// Lấy ra chỉ số được select  : nó sẽ được lưu trữ  vào biến seleted                 
	//===================================================================================
	int GetSelectIndexItem()
	{
		if (!m_hwnd)
		{
			selected = -1;
		}
		else
		{
			selected = (int)SendMessage(m_hwnd, CB_GETCURSEL, NULL, NULL);
		}
		return selected;
	}

public:
	void SetEventSelectedChange(void(*fun)(Window*, Combobox*))
	{
		m_EventSelectedChangedFun = fun;
	}

	void SetSelect(int index)
	{
		selected = index;
	}

	void TextEdit(bool bEn)
	{
		m_editText = bEn;
	}

	void SetMinItemVisiable(int iMinVisible)
	{
		if (!m_hwnd) return;
		SendMessage((m_hwnd), CB_SETMINVISIBLE, (WPARAM)iMinVisible, 0);
	}

	// Chú ý cần clone và tạo data bằng new 
	void AddItem(std::wstring text, void* data = NULL)
	{
		CBB_ITEM    item;
		item.text = text;
		item.data = data;

		items.push_back(item);
	}

	//===================================================================================
	// Xóa một item được chỉ định bằng text : tất cả các item có text sẽ bị xóa          
	//===================================================================================
	void RemoveItem(std::wstring text)
	{
		for (auto it = items.begin(); it != items.end(); /*it++*/)
		{
			if (it->text == text)
			{
				delete it->data;
				it = items.erase(it);
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
		if (index < 0 || index >= items.size())
		{
			return;
		}

		delete items[index].data;
		items.erase(items.begin() + index);

		UpdateItems();
	}

	//===================================================================================
	// Xóa toàn bộ item đang co trong combobxo                                           
	//===================================================================================
	void RemoveAllItem()
	{
		for (int i = 0; i < items.size(); i++)
		{
			delete items[i].data;
		}
		items.clear();

		UpdateItems();
	}

	//===================================================================================
	// Xóa toàn bộ item đang co trong combobxo                                           
	//===================================================================================
	void SelectItem(int sel)
	{
		if (sel < 0 || sel >= items.size())
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
		if (index < 0 || index >= items.size())
		{
			return L"";
		}

		return items[index].text;
	}

	//===================================================================================
	// Lấy giá trị của item                                                              
	//===================================================================================
	void* GetItemData(int index)
	{
		if (index < 0 || index >= items.size())
		{
			return NULL;
		}

		return items[index].data;
	}

	//===================================================================================
	// Lấy giá trị text của item selected                                                
	//===================================================================================
	std::wstring GetSelectText()
	{
		GetSelectIndexItem();
		if (selected < 0 || selected >= items.size())
		{
			return L"";
		}

		return items[selected].text;
	}

	//===================================================================================
	// Lấy dữ liệu item đang select                                                      
	//===================================================================================
	void* GetSelectData()
	{
		GetSelectIndexItem();

		if (selected < 0 || selected >= items.size())
		{
			return NULL;
		}

		return items[selected].data;
	}

	//===================================================================================
	// Lấy chỉ số của item selected                                                      
	//===================================================================================
	int GetSelectIndex()
	{
		GetSelectIndexItem();
		return selected;
	}

	virtual ControlType GetType() { return ControlType::COMBOBOX; }

public:
	virtual int OnInitControl(UINT& IDS)
	{
		m_ID = IDS++;

		int style = WS_CHILD | WS_VISIBLE;

		if (m_editText) style |= CBS_DROPDOWN;
		else            style |= CBS_DROPDOWNLIST;

		m_hwnd = CreateWindow(L"Combobox", NULL, style,							//
							m_rect.m_x, m_rect.m_y,								// x, y
							m_rect.m_width, m_rect.m_height*(int)items.size(),	// chiều rộng / chiều cao
							m_hwndPar,											// Handle cha
							(HMENU)(UINT_PTR)m_ID, NULL, NULL);

		// Create combobox failed !
		if (!m_hwnd)
		{
			m_ID = 0;
			IDS--;

			return 0;
		}

		UpdateItems();
		return Control::OnInitControl(IDS);
	}

	virtual void Event(Window* window, WORD _id, WORD _event)
	{
		if (_event == CBN_SELCHANGE)
		{
			GetSelectIndexItem();
			if (m_EventSelectedChangedFun)  m_EventSelectedChangedFun(window, this);
		}
	}
};

/**********************************************************************************
* ⮟⮟ Class name: Label control
* Label control for window
***********************************************************************************/
class Dllexport Label : public Control
{
private:
	std::wstring m_text;
	Color3		 m_color;

private:
	void UpdateText()
	{
		if (!m_hwnd) return;
		SetWindowText(m_hwnd, m_text.c_str());
	}

	virtual ControlType GetType() { return ControlType::LABEL; }

	void CalcTextSize(int width, int height)
	{
		SIZE size;
		HDC hdc = GetDC(m_hwnd);
		GetTextExtentPoint32(hdc, m_text.c_str(), wcslen(m_text.c_str()), &size);
		
		width = size.cx;
		height = size.cy;

		ReleaseDC(m_hwnd, hdc);
	}
public:
	void UpdateTextColor(HDC hdc)
	{
		if (!hdc) return;

		SetTextColor(hdc, RGB(m_color.r, m_color.g, m_color.b));
	}

public:
	Label(const wchar_t* text = L"")
		: Control()
	{
		m_text = text;
	}

	void SetText(const wchar_t* text)
	{
		m_text = text;
		this->UpdateText();
	}

	void SetColor(float r, float g, float b)
	{
		m_color.r = r;
		m_color.g = g;
		m_color.b = b;

		this->UpdateTextColor(NULL);
	}

public:
	virtual int OnInitControl(UINT& IDS)
	{
		m_ID = IDS++;

		int style = WS_VISIBLE | WS_CHILD ;

		CalcTextSize(m_rect.m_width, m_rect.m_height);

		m_hwnd = CreateWindow(L"STATIC", m_text.c_str(),
					style,											// style + control name
					m_rect.m_x, m_rect.m_y,							// x, y
					m_rect.m_width, m_rect.m_height,				// width / height
					m_hwndPar,										// Handle parent
					(HMENU)(UINT_PTR)m_ID,							// ID
					NULL, NULL);

		// Create combobox failed !
		if (!m_hwnd)
		{
			m_ID = 0;
			IDS--;
			return 0;
		}

		return Control::OnInitControl(IDS);
	}
};

____END_NAMESPACE____

#endif // WCONTROL_H
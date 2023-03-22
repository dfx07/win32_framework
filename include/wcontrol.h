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

//
//____END_NAMESPACE____
//
//#endif // WCONTROL_H
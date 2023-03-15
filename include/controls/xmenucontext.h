////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xmenucontext.h
* @create   Nov 15, 2022
* @brief    Create and handle event menu context
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XMENU_CONTEXT_H
#define XMENU_CONTEXT_H

#include "wbase.h"
#include <map>
#include <memory>

___BEGIN_NAMESPACE___
class WindowBase;
class ControlBase;

/**********************************************************************************
* ⮟⮟ Class name: MenuContext control
* MenuContext control for window
***********************************************************************************/

class Dllexport MenuItemBase
{
protected:
	typedef void(*typeEventFun)(ControlBase* ctrl);

public:
	virtual int Type() = 0;
};

class MenuItem : public MenuItemBase
{
public:
	int Type() { return 1; }

public:
	UINT			m_type;
	std::wstring	m_label;

	typeEventFun	m_EventFun = NULL;
};

class Dllexport MenuContext : public ControlBase , public MenuItemBase
{
	enum { MAX_MENU_ITEM  = 10};
	typedef MenuItemBase*		ItemPtr;
	typedef std::map<UINT, MenuItemBase*>	MapMenuItemID;

public:
	int Type() { return 2; }

protected:
	HMENU			m_hMenu;
	MapMenuItemID	m_items;

public:
	MenuContext() : ControlBase(), m_hMenu(NULL)
	{

	}

private:
	bool CreateMenuItem(UINT ID, MenuItem& item)
	{
		if (item.m_type == MF_SEPARATOR)
		{
			AppendMenu(m_hMenu, MF_SEPARATOR, ID, NULL);
		}
		else
		{
			AppendMenu(m_hMenu, item.m_type, ID, item.m_label.c_str());
		}
	}

	bool DeleteMenuItem(UINT ID)
	{
		::DeleteMenu(m_hMenu, ID, MF_BYPOSITION);
	}

	void ClearItem()
	{
		for (auto it = m_items.begin(); it != m_items.end(); it++)
		{
			ItemPtr item = it->second;

			if (item->Type() == 1)
			{
				delete it->second;
			}
			else if (item->Type())
			{
				((MenuContext*)item)->ClearItem();
			}
		}
	}

public:
	virtual ControlType GetType() { return static_cast<ControlType>(ControlType::MENUCONTEXT); };

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int OnInitControl()
	{
		m_hMenu = ::CreatePopupMenu();
		
		UINT uiStartID = m_ID + 1;

		for (auto it = m_items.begin(); it != m_items.end(); it++)
		{
			if (it->first <= 0)
				continue;

			ItemPtr item = it->second;
			if (item->Type() == 1)
			{
				CreateMenuItem(uiStartID++, *(MenuItem*)item);
			}
			else if (item->Type())
			{
				((MenuContext*)item)->SetID(uiStartID);
				uiStartID += ((MenuContext*)item)->OnInitControl();
			}
		}

		return uiStartID + MAX_MENU_ITEM - m_ID;
	}

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void OnCommand(WORD _id, WORD _event)
	{
		auto itItem = m_items.find(_id);
		if (itItem != m_items.end() )
		{
			//itItem->second->m_EventFun(this);
		}
	}

public:

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void AddItem(const wchar_t* label, LONG type, typeEventFun fun_event)
	{
		int tempID = -m_items.size();

		MenuItem* item = new MenuItem();
		item->m_label = label;
		item->m_type = type;

		if (m_hMenu != NULL)
		{
			if (CreateMenuItem(m_ID + m_items.size(), *item));
			{
				tempID = m_ID + m_items.size();
			}
		}
		m_items.insert({ tempID, item });
	}

	/***************************************************************************
	*! @brief  : Remove item
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void RemoveItem(UINT id)
	{
		DeleteMenuItem(id);
	}

	/***************************************************************************
	*! @brief  : Remove item
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Insert(MenuContext* menu)
	{
		int tempID = -m_items.size();

		if (m_hMenu != NULL)
		{
			menu->SetID(m_ID + m_items.size());
			menu->OnInitControl();
			tempID = m_ID + m_items.size();
		}

		m_items.insert({ tempID, menu });
	}

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Show(POINT point)
	{
		NULL_CHECK_RETURN(m_hWndPar, );

		ClientToScreen(m_hWndPar, &point);
		TrackPopupMenu(m_hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, m_hWndPar, NULL);
	}
};

____END_NAMESPACE____

#endif // !XMENU_CONTEXT_H

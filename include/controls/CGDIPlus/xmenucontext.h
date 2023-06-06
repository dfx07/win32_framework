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

#include "xwcontrol.h"
#include <unordered_map>
#include <memory>

___BEGIN_NAMESPACE___

class WindowBase;
class Control;

/**********************************************************************************
* ⮟⮟ Class name: MenuItemBase
* MenuContext control for window
***********************************************************************************/
class Dllexport MenuItemBase
{
protected:
	typedef void(*typeEventFun)(Control* ctrl);

public:
	virtual int GetItemType() = 0;
};

/**********************************************************************************
* ⮟⮟ Class name: MenuItemBase
* MenuContext control for window
***********************************************************************************/
class Dllexport MenuItem : public MenuItemBase
{
public:
	int GetItemType() { return 1; }

public:
	UINT			m_iType	= 0;
	std::wstring	m_sLabel	= L"";
	typeEventFun	m_EventFun = NULL;
};

/**********************************************************************************
* ⮟⮟ Class name: MenuContext control
* MenuContext control for window
***********************************************************************************/
class Dllexport MenuContext : public Control, public MenuItemBase
{
	enum { MAX_MENU_ITEM  = 10};
	typedef	 MenuItemBase* ItemPtr;
	typedef std::unordered_map<INT, MenuItemBase*> MapMenuItemID;

public:
	int GetItemType() { return 2; }

protected:
	HMENU			m_hMenu;
	MapMenuItemID	m_Items;
	std::wstring	m_sLabel;

public:
	MenuContext(const wchar_t* label = L"") : Control(),
		m_hMenu(NULL)
	{
		m_sLabel = label;
	}

	~MenuContext()
	{
		this->ClearItem();
	}

private:

	/***************************************************************************
	*! @brief  : Win API add MenuItem
	*! @return : bool
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool WIN32_AppendMenuItem(UINT ID, MenuItem& item)
	{
		if (item.m_iType == MF_SEPARATOR)
		{
			AppendMenu(m_hMenu, MF_SEPARATOR, ID, NULL);
		}
		else
		{
			AppendMenu(m_hMenu, item.m_iType, ID, item.m_sLabel.c_str());
		}

		return true;
	}

	/***************************************************************************
	*! @brief  : Win API add MenuContext
	*! @return : bool
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool WIN32_AppendMenuContext(MenuContext* menu)
	{
		if (!menu || !menu->IsCreated())
			return false;

		AppendMenu(m_hMenu, MF_POPUP, (UINT_PTR)menu->m_hMenu, menu->m_sLabel.c_str());

		return true;
	}

	bool DeleteMenuItem(UINT ID)
	{
		::DeleteMenu(m_hMenu, ID, MF_BYPOSITION);
	}

	/***************************************************************************
	*! @brief  : Clear data menucontext
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void ClearItem()
	{
		for (auto it = m_Items.begin(); it != m_Items.end(); it++)
		{
			ItemPtr item = it->second;

			int iType = item->GetItemType();

			if (iType == 1) // menu item
			{
				delete it->second;
			}
			else if (iType == 2) // menu context
			{
				((MenuContext*)item)->ClearItem();
			}
		}
	}

protected:
	virtual bool IsCreated()
	{
		return m_hMenu ? true : false;
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

		for (auto it = m_Items.begin(); it != m_Items.end(); it++)
		{
			if (it->first > 0)
				continue;

			ItemPtr item = it->second;

			int iItem = item->GetItemType();

			if (iItem == 1)
			{
				WIN32_AppendMenuItem(uiStartID++, *(MenuItem*)item);
			}
			else if (iItem == 2)
			{
				MenuContext* menu_item = (MenuContext*)item;
				NULL_CONTINUE(menu_item);

				menu_item->SetID(uiStartID);
				uiStartID += menu_item->OnInitControl();
				WIN32_AppendMenuContext(menu_item);
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
		auto itItem = m_Items.find(_id);
		if (itItem != m_Items.end() )
		{
			if (itItem->second->GetItemType() == 1)
			{
				MenuItem* item =  (MenuItem*)itItem->second;
				item->m_EventFun(this);
			}
		}
	}

public:

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetLable(const wchar_t* label)
	{
		m_sLabel = label;
	}

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void AddItem(const wchar_t* label, LONG type, typeEventFun fun_event)
	{
		int tempID = int(-1* m_Items.size());

		MenuItem* item = new MenuItem();
		item->m_sLabel = label;
		item->m_iType = type;

		if (m_hMenu != NULL)
		{
			tempID = (int)(m_ID + m_Items.size());

			if (!WIN32_AppendMenuItem(tempID, *item))
			{
				std::cerr << "AddItem failed" << std::endl;
				return;
			}
		}
		m_Items.insert({ tempID, item });
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
		int tempID = (int)(-1* m_Items.size());

		if (m_hMenu != NULL)
		{
			tempID = (int)(m_ID + m_Items.size());

			menu->SetID(tempID);
			menu->OnInitControl();

			if (!WIN32_AppendMenuContext(menu))
			{
				std::cerr << "WIN32_AppendMenuContext  failed" << std::endl;
				return;
			}
		}

		m_Items.insert({ tempID, menu });
	}

	/***************************************************************************
	*! @brief  : Init Menubar control
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Show(POINT point)
	{
		NULL_RETURN(m_hWndPar, );

		ClientToScreen(m_hWndPar, &point);
		TrackPopupMenu(m_hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, m_hWndPar, NULL);
	}
};

____END_NAMESPACE____

#endif // !XMENU_CONTEXT_H

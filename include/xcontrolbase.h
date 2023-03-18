////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xcontrolbase.h
* @create   Nov 15, 2022
* @brief    Create and handle event base control
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XCONTROLBASE_H
#define XCONTROLBASE_H

#include "wctrl.h"

___BEGIN_NAMESPACE___

class WindowBase;

/**********************************************************************************
* ⮟⮟ Class name: ControlBase - Created: 15/03/2023
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport ControlBase : public XControl
{
public:
	ControlBase(): XControl()
	{

	}

protected:
	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual ControlType GetType() = 0;

	/***************************************************************************
	*! @brief  : Base function destroy window
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void OnDestroy(){};

	/***************************************************************************
	*! @brief  : Active when user use BS_OWNERDRAW control
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void Draw(LPDRAWITEMSTRUCT& ){};

	/***************************************************************************
	*! @brief  : Active when user use BS_OWNERDRAW control
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void OnCommand(WindowBase* window, WORD _id, WORD _event){};

	/***************************************************************************
	*! @brief  : Active when user use BS_OWNERDRAW control
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool ContainID(INT ID){ return false; };

public:
	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int OnInitControl() = 0;

	/***************************************************************************
	*! @brief  : function design pattern inti control
	*! @return : next id
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*! @note   : auto define id 
	***************************************************************************/
	int Create(HWND hwndParent, UINT uiID)
	{
		this->SetParent(hwndParent);

		// If user set id then not set auto id
		if (GetID() <= 0)
		{
			this->SetID(uiID);
		}

		int nControlCreated = OnInitControl();

		// Init control done -> Ok
		if (nControlCreated > 0 && IsCreated())
		{
			uiID += nControlCreated;
			SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
		}
		else
		{
			m_hWnd = NULL;
			this->SetParent(NULL);
			this->SetID(0);
		}

		return uiID;
	}

	friend class WindowBase;
	friend class Window;
	friend class SubWindow;
};


____END_NAMESPACE____

#endif // !XCONTROLBASE_H
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
	static Gdiplus::Rect ConvertToRect(RECT& _rect)
	{
		Gdiplus::Rect rect;

		rect.X = _rect.left;
		rect.Y = _rect.top;
		rect.Width = _rect.right - _rect.left;
		rect.Height = _rect.bottom - _rect.top;

		return rect;
	}

	static std::wstring ToWchar(const char* strTxt, const int iLength)
	{
		std::wstring utf16;

		int length = iLength;
		if (iLength < 0)
			length = static_cast<int>(strlen(strTxt));

		utf16.resize( static_cast<int64_t>(length) + 2, 0);

		int nWide = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, strTxt, iLength,
			(LPWSTR)utf16.c_str(), (int)utf16.size());

		return utf16;
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void SetRenderControl(GDIplusCtrlRender* pRender)
	{
		m_pRender = pRender;
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
	virtual void Draw(bool bDraw = false ){};

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

	/***************************************************************************
	*! @brief  : Get current cursor position [left - top]
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	static POINT get_cursor_in_screen()
	{
		POINT cursor_pos = {0, 0};
		if (::GetCursorPos(&cursor_pos) == TRUE)
		{
			return cursor_pos;
		}
		else
		{
			return { 0,0 };
		}
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [left - top]
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetCursorPos(long& xpos, long& ypos)
	{
		POINT cursor_pos = get_cursor_in_screen();
		if (ScreenToClient(m_hWnd, &cursor_pos) == TRUE)
		{
			xpos = cursor_pos.x;
			ypos = cursor_pos.y;
			return true;
		}
		else
		{
			xpos = 0;
			ypos = 0;
			return false;
		}
	}

	/***************************************************************************
	*! @brief  : Get current cursor position in parent window [left - top]
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	bool GetCursorPosInParent(long& xpos, long& ypos)
	{
		POINT cursor_pos = get_cursor_in_screen();
		if (ScreenToClient(m_hWndPar, &cursor_pos) == TRUE)
		{
			xpos = cursor_pos.x;
			ypos = cursor_pos.y;
			return true;
		}
		else
		{
			xpos = 0;
			ypos = 0;
			return false;
		}
	}

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

public:
	GDIplusCtrlRender* m_pRender = nullptr;  // render draw parent

	friend class WindowBase;
	friend class Window;
	friend class SubWindow;
};

____END_NAMESPACE____

#endif // !XCONTROLBASE_H
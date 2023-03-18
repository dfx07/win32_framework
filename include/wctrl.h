////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     wctrl.h
* @create   Nov 15, 2022
* @brief    Define the basic classes that the control needs to inherit
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef WCTRL_H
#define WCTRL_H

#include "wdef.h"

___BEGIN_NAMESPACE___

// control define type
enum class ControlType
{
	BUTTON		,
	MENUCONTEXT	,
	MENUBAR		,
	COMBOBOX	,
	LABEL		,
	CHECKBOX	,
};

// window define type
enum class WindowType
{
	NoDefine,
	MainWin ,
	SubWin  ,
};

/**********************************************************************************
* ⮟⮟ Class name: WindowBase
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport XControl
{
protected:
	typedef struct tagRectControl
	{
	public:
		int			 x;
		int			 y;
		unsigned int width;
		unsigned int height;

		tagRectControl() : x(0), y(0),
			width(0), height(0)
		{

		}

	public:
		const tagRectControl& operator=(const tagRectControl& sed)
		{
			x		=  sed.x;
			y		=  sed.y;
			width	=  sed.width;
			height	=  sed.height;

			return *this;
		}
	} CRect;

protected:
	UINT         m_ID;			 // ID control
	HWND         m_hWnd;		 // win handle control
	HWND         m_hWndPar;		 // parent control

	bool         m_bEnable;		 // enable control
	bool         m_bVisble;		 // is visible

	CRect        m_rect;		 // state information

public:
	XControl(): m_hWnd(NULL), m_ID(0),
		m_hWndPar(NULL)
	{
		m_bEnable = true;
		m_bVisble = true;
	}

protected:
	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetParent(HWND hwnd)
	{ 
		m_hWndPar = hwnd;
		NULL_RETURN(m_hWnd, );
		::SetParent(m_hWnd, m_hWndPar);
	}

	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetID(UINT uiID){ m_ID = uiID; }

	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual bool IsCreated() { return m_hWnd ? true : false; }

public:
	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	HWND  GetHwnd()  { return m_hWnd; }

	/***************************************************************************
	*! @brief  : Function design pattern init control
	*! @return : Number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	UINT  GetID()    { return m_ID;	  }

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Enable(bool bEnable)
	{
		m_bEnable = bEnable;

		NULL_RETURN(m_hWnd, );
		::EnableWindow(m_hWnd, m_bEnable ? TRUE : FALSE);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Visible(bool bVisible)
	{
		m_bVisble = bVisible;

		NULL_RETURN(m_hWnd, );
		::ShowWindow(m_hWnd, m_bVisble ? TRUE : FALSE);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void SetPosition(int x, int y)
	{
		m_rect.x = x;
		m_rect.y = y;
		NULL_RETURN(m_hWnd, );
		::SetWindowPos(m_hWnd, NULL, m_rect.x, m_rect.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	/***************************************************************************
	*! @brief  : move window position
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void MovePosition(const int x, const int y)
	{
		m_rect.x += x;
		m_rect.y += y;
		SetPosition(m_rect.x, m_rect.y);
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual void SetSize(int width, int height)
	{
		m_rect.width = width;
		m_rect.height = height;

		NULL_RETURN(m_hWnd, );
		::SetWindowPos(m_hWnd, NULL, m_rect.x, m_rect.y, m_rect.width, m_rect.height, SWP_NOMOVE | SWP_NOZORDER);
	}

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetWidth()  { return m_rect.width; }

	/***************************************************************************
	*! @brief  : Hide window -> update status
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual int GetHeight() { return m_rect.height; }

public:
	/***************************************************************************
	*! @brief  : send message window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Send_Message(UINT message, WPARAM wParam, LPARAM lParam)
	{
		NULL_RETURN(m_hWnd, );
		::SendMessage(m_hWnd, message, wParam, lParam);
	}

	/***************************************************************************
	*! @brief  : post message window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	void Post_Message(UINT message, WPARAM wParam, LPARAM lParam)
	{
		NULL_RETURN(m_hWnd, );
		::PostMessage(m_hWnd, message, wParam, lParam);
	}

	friend class ControlBase;
	friend class WindowBase;
};

/**********************************************************************************
* ⮟⮟ Class name: WindowContext
* Base class for window use draw gpu inheritance
***********************************************************************************/
interface Dllexport WindowContext
{
public:
	virtual void* Render() = 0;
	virtual bool  CreateContext(int iAntialiasing = 0) = 0;
	virtual void  DeleteContext() = 0;
	virtual bool  MakeContext() = 0;
	virtual void  SwapBuffer() = 0;
};

/**********************************************************************************
* ⮟⮟ Class name: Control base
* Base class for inherited window controls
***********************************************************************************/
class Dllexport ControlRectUI
{
protected:

	typedef struct tagPropertyUI
	{
		unsigned int border_radius   = 0;
		unsigned int border_width    = 0;
		Color4		 text_color      = {255, 255, 255, 255};
		Color4		 text_hover_color= {255, 255, 255, 255};
		Color4		 background_color= {255, 255, 255, 255};
		Color4		 background_hover_color = { 255, 0, 0, 255 };
		Color4		 background_click_color = { 0, 255, 0, 255 };

	} PropertyControlUI;

	PropertyControlUI	m_property;

public:
	void SetBorderWidth(unsigned int iWidth)
	{
		m_property.border_width = iWidth;
	}

	void SetBorderRadius(unsigned int iWidth)
	{
		m_property.border_radius = iWidth;
	}

	void SetBackgroundColor(const Color4 cl)
	{
		m_property.background_color = cl;
	}

	void SetBackgroundHoverColor(const Color4 cl)
	{
		m_property.background_hover_color = cl;
	}

	void SetBackgroundClickColor(const Color4 cl)
	{
		m_property.background_hover_color = cl;
	}

	void SetTextColor(const Color4 cl)
	{
		m_property.text_color = cl;
	}

	void SetTextHoverColor(const Color4 cl)
	{
		m_property.text_hover_color = cl;
	}

	friend class WindowBase;
};

____END_NAMESPACE____

#endif // !WCTRL_H


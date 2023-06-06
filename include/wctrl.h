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
	TEXTBOX		,
};

// window define type
enum class WindowType
{
	NoDefine,
	MainWin ,
	SubWin  ,
};

enum class Visible
{
	SHOW	 ,
	HIDE	 ,
	COLLAPSE ,
};

/**********************************************************************************
* ⮟⮟ Class name: Location, Size
* Base class for window handle inheritance
***********************************************************************************/
struct Location
{
	float		m_x;
	float		m_y;
};

struct Size
{
	float		m_width;
	float		m_height;
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

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual RECT GetRect(bool bWithParent = true)
	{
		RECT rect = { 0 ,0 ,0 ,0 };
		NULL_RETURN(m_hWnd, rect);
		::GetWindowRect(m_hWnd, &rect);

		if (bWithParent)
		{
			MapWindowPoints(HWND_DESKTOP, m_hWndPar, (LPPOINT)&rect, 2);
		}

		return rect;
	}

	/***************************************************************************
	*! @brief  : Get current cursor position [Center]
	*! @return : true : ok | false : not ok
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
	virtual RECT GetRect(HWND hwnd, bool bWithParent = true)
	{
		RECT rect = { 0 ,0 ,0 ,0 };
		NULL_RETURN(hwnd, rect);
		::GetWindowRect(hwnd, &rect);

		if (bWithParent)
		{
			MapWindowPoints(HWND_DESKTOP, m_hWndPar, (LPPOINT)&rect, 2);
		}

		return rect;
	}

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
* ⮟⮟ Class name: Control property - Created: 13/05/2023
* Base class for window handle inheritance
***********************************************************************************/
template<typename T>
struct Control4Value
{
	using _type = T;

public:
	union { _type top   ;  };
	union { _type left  ;  };
	union { _type right ;  };
	union { _type bottom;  };

public:
	Control4Value() : left(static_cast<T>(0)), top(static_cast<T>(0)),
		right(static_cast<T>(0)), bottom(static_cast<T>(0))
	{

	}

	template< typename U>
	Control4Value(const U& value) :
		left(static_cast<T>(value)), top(static_cast<T>(value)),
		right(static_cast<T>(value)), bottom(static_cast<T>(value))
	{

	}

	template< typename A, typename B, typename C, typename D>
	Control4Value(const A& _left,
		const B& _top = static_cast<B>(0),
		const C& _right = static_cast<C>(0),
		const D& _bottom = static_cast<D>(0)):
		left(static_cast<T>(_left)), top(static_cast<T>(_top)),
		right(static_cast<T>(_right)), bottom(static_cast<T>(_bottom))
	{

	}

	template< typename U>
	const Control4Value& operator=(const U& v)
	{
		this->top = left = right = bottom =  static_cast<T>(v);
		return *this;
	}
};

typedef Control4Value<float> CMargin;
typedef Control4Value<float> CPadding;

/**********************************************************************************
* ⮟⮟ Class name: Control base
* Base class for inherited window controls
***********************************************************************************/
class Dllexport RectUIControl
{
public:
	using Color = GdiplusEx::Color4;

protected:
	typedef struct tagBackgroundUI
	{
		UINT		 border_radius      = 0;
		UINT		 border_width       = 0;

		Color		 bk_erase_color     = { 255, 255, 255, 255 };
		Color		 bk_color           = { 255, 255, 255, 255 };
		Color		 bk_hover_color     = { 255,   0,   0, 255 };
		Color		 bk_click_color     = { 0  , 255,   0, 255 };

		Color		 border_color       = { 255, 255, 255, 255 };
		Color		 border_hover_color = { 255, 255, 255, 255 };

	} BackgroundProperty;

	typedef struct tagTextUI
	{
		Color		 text_color         = { 255, 255, 255, 255 };
		Color		 text_hover_color   = { 255, 255, 255, 255 };

	} TextProperty;

protected:
	virtual void SetDefaultPropertyUI() = 0;

public:
	////////////////////////////////////////////////////////////////////////////////
	// Background Property setter

	void SetBorderWidth(unsigned int iWidth)
	{
		UI_Background.border_width = iWidth;
	}

	void SetBorderRadius(unsigned int iWidth)
	{
		UI_Background.border_radius = iWidth;
	}

	void SetBackgroundColor(const Color cl)
	{
		UI_Background.bk_color = cl;
	}

	void SetEraseBackgroundColor(const Color& cl)
	{
		UI_Background.bk_erase_color = cl;
	}

	void SetBackgroundHoverColor(const Color& cl)
	{
		UI_Background.bk_hover_color = cl;
	}

	void SetBackgroundClickColor(const Color& cl)
	{
		UI_Background.bk_click_color = cl;
	}

	void SetBorderColor(const Color& col)
	{
		UI_Background.border_color = col;
	}

	void SetBorderHoverColor(const Color& col)
	{
		UI_Background.border_hover_color = col;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Text Property setter

	void SetTextColor(const Color& cl)
	{
		UI_Text.text_color = cl;
	}

	void SetTextHoverColor(const Color& cl)
	{
		UI_Text.text_hover_color = cl;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Margin, padding Property setter

	void SetPadding(int left, int top, int right, int bottom)
	{
		m_padding = CPadding(left , top , right, bottom);
	}

	void SetMargin(int left, int top, int right, int bottom)
	{
		m_margin = CMargin( left , top , right, bottom );
	}

	/*******************************************************************************
	*! @brief  : send message window
	*! @return : void
	*! @author : thuong.nv          - [Date] : 05/03/2023
	*******************************************************************************/
	void EraseBackground(GDIplusCtrlRender*	pRender, Gdiplus::Brush* brush = NULL)
	{
		NULL_RETURN(pRender, );

		Gdiplus::Rect rect = pRender->GetDrawRect();

		if (brush)
		{
			pRender->DrawRectangle(rect, NULL, brush , 0);
		}
		else
		{
			pRender->DrawRectangle(rect, NULL, UI_Background.bk_erase_color.wrefcol, 0);
		}
	}

	void DrawBackground(GDIplusCtrlRender*	  pRender,
						const Gdiplus::Pen*   pen    = NULL,
						const Gdiplus::Brush* brush  = NULL,
						unsigned int		  border_radius = 0)
	{
		NULL_RETURN(pRender, );

		Gdiplus::Rect rect = pRender->GetDrawRect();

		float border_width = pen ? pen->GetWidth() : 0.f;

		rect.X		+= border_width/2;
		rect.Y		+= border_width/2;
		rect.Width  -= (int) border_width/2;
		rect.Height -= (int) border_width/2;

		if (border_width <= 0)
		{
			pRender->DrawRectangle(rect, nullptr, brush, border_radius);
		}
		else
		{
			pRender->DrawRectangle(rect, pen, brush, border_radius);
		}
	}

protected:
	friend class WindowBase;
	friend class SubWindow;
	friend class ControlBase;

protected:
	BackgroundProperty		UI_Background;
	TextProperty			UI_Text;

	CMargin					m_margin;
	CPadding				m_padding;
};
____END_NAMESPACE____

#endif // !WCTRL_H


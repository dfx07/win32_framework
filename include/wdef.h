////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     wdef.h
* @create   Nov 15, 2022
* @brief    Create and handle event control the window
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef WDEF_H
#define WDEF_H

#include "xsysdef.h"

#include <Windows.h>
#include <gdiplus.h>
#include <wingdi.h>
#include <winuser.h>
#include <assert.h>
#include <stack>
#include <iostream>


// disable waring visual studio c++14
#pragma warning( disable : 4244)

#define GL_WIN_CLASS		L"FOX_WINHANDLE_CLASS"
#define GL_SUBWIN_CLASS		L"FOX_SUBWINHANDLE_CLASS"

#define GL_PRESSED			1
#define GL_RELEASE			0

#define ADD_ATRIBUTE(attribs, name, value)\
{\
    assert((size_t) attribCount < attribsize);\
    attribs[attribCount++] = name;  \
    attribs[attribCount++] = value; \
}

#define END_ATRIBUTE(attribs)\
{\
    assert((size_t) attribCount < attribsize); \
    attribs[attribCount++] = 0;  \
}

#define ON_FUNCTION_WINDOW(bFunCheck, ...)\
{\
    if(bFunCheck) \
    {\
        return bFunCheck(__VA_ARGS__);\
    }\
}

#define CHECK_RUN_FUNCTION(bFunCheck, ...)\
{\
    if(bFunCheck) \
    {\
        bFunCheck(__VA_ARGS__);\
    }\
}

___BEGIN_NAMESPACE___

/**********************************************************************************
* Class name  : Gdi render information
* Brief : Provides type and define common function time
***********************************************************************************/
typedef struct GDI_DRAW_INFO
{
	HDC  hDC  = NULL;
	RECT rect = RECT();

} *GDI_DRAW_INFO_PTR;

typedef struct GDIPLUS_DRAW_INFO
{
	Gdiplus::Graphics*  render = NULL;
	Gdiplus::Rect		rect   = {0,0,0,0};

} *GDIPLUS_DRAW_INFO_PTR;

typedef struct GDIPLUS_TOKEN
{
	bool		m_iGdipluStatus;
	ULONG_PTR	m_pGdiplusToken;

}  *GDIPLUS_TOKEN_PTR;

typedef struct WindowRender
{
	HWND	m_hWnd		= NULL;
	HDC		m_hDc		= NULL;
	HGLRC	m_hGLRC		= NULL;

	bool	m_bCreated  = false;	// create ok

} *WindowRenderPtr;


typedef enum tagGLKeyboard
{
    AltLeft         = 0x12,      // (18)
    AltRight        = 0x12,      // (18)
    CapsLock        = 0x14,      // (20)
    ControlLeft     = 0x11,      // (17)
    ControlRight    = 0x11,      // (17)
    OSLeft          = 0x5B,      // (91)
    OSRight         = 0x5C,      // (92)
    ShiftLeft       = 0x10,      // (16)
    ShiftRight      = 0x10,      // (16)
    Delete          = 0x2E,      // (46)
    Escapex         = 0x1B,      // (27)

    Digit1          = 0x31,      // (49)
    Digit2          = 0x32,      // (50)
    Digit3          = 0x33,      // (51)
    Digit4          = 0x34,      // (52)
    Digit5          = 0x35,      // (53)
    Digit6          = 0x36,      // (54)
    Digit7          = 0x37,      // (55)
    Digit8          = 0x38,      // (56)
    Digit9          = 0x39,      // (57)
    Digit0          = 0x30,      // (48)
    KeyA            = 0x41,      // (65)
    KeyB            = 0x42,      // (66)
    KeyC            = 0x43,      // (67)
    KeyD            = 0x44,      // (68)
    KeyE            = 0x45,      // (69)
    KeyF            = 0x46,      // (70)
    KeyG            = 0x47,      // (71)
    KeyH            = 0x48,      // (72)
    KeyI            = 0x49,      // (73)
    KeyJ            = 0x4A,      // (74)
    KeyK            = 0x4B,      // (75)
    KeyL            = 0x4C,      // (76)
    KeyM            = 0x4D,      // (77)
    KeyN            = 0x4E,      // (78)
    KeyO            = 0x4F,      // (79)
    KeyP            = 0x50,      // (80)
    KeyQ            = 0x51,      // (81)
    KeyR            = 0x52,      // (82)
    KeyS            = 0x53,      // (83)
    KeyT            = 0x54,      // (84)
    KeyU            = 0x55,      // (85)
    KeyV            = 0x56,      // (86)
    KeyW            = 0x57,      // (87)
    KeyX            = 0x58,      // (88)
    KeyY            = 0x59,      // (89)
    KeyZ            = 0x5A,      // (90)


    Left            = 0x25,      // (37)   Left arrow.
    Up              = 0x26,      // (38)   Up arrow.
    Right           = 0x27,      // (39)   Right arrow.
    Down            = 0x28,      // (40)   Down arrow.


    F1              = 0x70,      // (112)
    F2              = 0x71,      // (113)
    F3              = 0x72,      // (114)
    F4              = 0x73,      // (115)
    F5              = 0x74,      // (116)
    F6              = 0x75,      // (117)
    F7              = 0x76,      // (118)
    F8              = 0x77,      // (119)
    F9              = 0x78,      // (120)
    F10             = 0x79,      // (121)
    F11             = 0x7A,      // (122)
    F12             = 0x7B,      // (123)
    F13             = 0x7C,      // (124)
    F14             = 0x7D,      // (125)
    F15             = 0x7E,      // (126)
    F16             = 0x7F,      // (127)
    F17             = 0x80,      // (128)
    F18             = 0x81,      // (129)
    F19             = 0x82,      // (130)
    F20             = 0x83,      // (131)
    F21             = 0x84,      // (132)
    F22             = 0x85,      // (133)
    F23             = 0x86,      // (134)
    F24             = 0x87,      // (135)

} GLKeyboard;

typedef enum tagGLMouse
{
    LeftButton      = 0x01,      // (1)
    RightButton     = 0x02,      // (2)
    MidButton       = 0x04,      // (4)

} GLMouse;

typedef enum tagMESSAGE_WIN
{
	PROCESS_MSG		=  100000,
	MESSAGE_START	=  100002,
	MOVE_WIN		=  MESSAGE_START + 0x0001,
	MINI_WIN		=  MESSAGE_START + 0x0002,
	CLOSE_WIN		=  MESSAGE_START + 0x0003,

} MESSAGE_WIN;

class Dllexport GdiplusToken
{
private:
	bool Create()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		m_token.m_iGdipluStatus = Gdiplus::GdiplusStartup(&m_token.m_pGdiplusToken, &gdiplusStartupInput, NULL)
			== Gdiplus::Status::Ok ? true : false;

		return m_token.m_iGdipluStatus;
	}

public:
	void Shutdown()
	{
		Gdiplus::GdiplusShutdown(m_token.m_pGdiplusToken);
	}

public:
	GdiplusToken()  { Create();   }
	GdiplusToken(ULONG_PTR token, bool status)
	{ 
		m_token.m_pGdiplusToken = token;
		m_token.m_iGdipluStatus = status;
	}

	~GdiplusToken() { Shutdown(); }
	bool IsOk()     { return m_token.m_iGdipluStatus; }
	void ReCreate() { this->Shutdown(); this->Create();}

private:
	GDIPLUS_TOKEN	m_token;
};

/**********************************************************************************
* Class name  : Color define
* Brief : Provides type and define common function time
***********************************************************************************/
struct Color4
{
public:
	void set(float r = 255.f,
			 float g = 255.f,
			 float b = 255.f,
			 float a = 255.f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		wrefcol = Gdiplus::Color::MakeARGB(a, r, g, b);
	}

	Color4(	float r =  255.f,
			float g =  255.f,
			float b =  255.f,
			float a =  255.f)
	{
		this->set(r, g, b, a);
	}

	~Color4()
	{

	}

	const Color4& operator=(const Color4& color)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = color.a;
		this->wrefcol = color.wrefcol;

		return *this;
	}

public:
	float			r;
	float			g;
	float			b;
	float			a;

	Gdiplus::ARGB   wrefcol;
};

struct Color3
{
public:
	void set(float r = 255.f,
			 float g = 255.f,
			 float b = 255.f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		wrefcol = Gdiplus::Color::MakeARGB(255, r, g, b);
	}

	Color3( float r = 255.f,
			float g = 255.f,
			float b = 255.f)
	{
		this->set(r, g, b);
	}

public:
	float	 r;
	float	 g;
	float	 b;

	Gdiplus::ARGB wrefcol;
};

/**********************************************************************************
* Class name  : GdiplusEx define
* Brief : Provides gdiplus define extension
***********************************************************************************/
namespace GdiplusEx
{
	enum class ImageAlignment
	{
		ImageAlignmentNear		= 0, // Left edge for left-to-right text,
		ImageAlignmentCenter	= 1, // right for right-to-left text,
		ImageAlignmentFar		= 2  // and top for vertical
	};

	class ImageFormat
	{
	public:
		GdiplusEx::ImageAlignment	m_veralign = ImageAlignment::ImageAlignmentNear;
		GdiplusEx::ImageAlignment	m_horalign = ImageAlignment::ImageAlignmentNear;

		Gdiplus::Size				m_offset;

	public:
		void SetVerticalAlignment(GdiplusEx::ImageAlignment align)
		{
			m_veralign = align;
		}

		void SetHorizontalAlignment(GdiplusEx::ImageAlignment align)
		{
			m_horalign = align;
		}

		void SetOffset(int x, int y)
		{
			m_offset.Width = x;
			m_offset.Height = y;
		}
	};
};

class IControlRender
{
	virtual void  CreateRender(const HDC& _hdc, const RECT& _rect) = 0;
	virtual void  DestroyRender() = 0;
	virtual void* Render() = 0;						// Information function
	virtual void  Flush(bool bDestroy = false) = 0; // Render function
};

/**********************************************************************************
* Class name: GDIRender information
* Provides type and define common function time
***********************************************************************************/
class Dllexport GDICtrlRender : public IControlRender
{
private:
	void CreateRender(const HDC& hdc, const RECT& rect)
	{
		m_origDrawInfo.hDC = hdc;
		m_origDrawInfo.rect = rect;

		m_pRender = new GDI_DRAW_INFO();

		// create new memory draw information
		m_pRender->hDC = CreateCompatibleDC(hdc);
		m_pRender->rect = rect;

		newBmp = CreateCompatibleBitmap(m_origDrawInfo.hDC, m_origDrawInfo.rect.right - m_origDrawInfo.rect.left,
										m_origDrawInfo.rect.bottom - m_origDrawInfo.rect.top);

		// we need to save original bitmap, and select it back when we are done, in order to avoid GDI leaks!
		oldBmp = (HBITMAP)SelectObject(m_pRender->hDC, newBmp);
	}

	void DestroyRender()
	{
		SelectObject(m_pRender->hDC, oldBmp); // select back original bitmap
		DeleteObject(newBmp);				  // delete bitmap since it is no longer required
		DeleteDC(m_pRender->hDC);			  // delete memory DC since it is no longer required

		SAFE_DELETE(m_pRender);
	}

public:
	void Init(const HDC& hdc, const RECT& rect)
	{
		CreateRender(hdc, rect);
	}

public:
	GDICtrlRender() : oldBmp(NULL), newBmp(NULL),
		m_pRender(NULL)
	{
	}

	GDICtrlRender(HDC& _hdc, RECT& _rect)
	{
		Init(_hdc, _rect);
	}

	~GDICtrlRender()
	{
		this->Flush(true);
	}

	void* Render()
	{
		if (m_pRender->hDC != NULL)
		{
			return &m_pRender;
		}
		return NULL;
	}

	void Flush(bool bDestroy = false)
	{
		BitBlt(m_origDrawInfo.hDC, 0, 0, m_pRender->rect.right - m_pRender->rect.left,
			m_pRender->rect.bottom - m_pRender->rect.top, m_pRender->hDC, 0, 0, SRCCOPY);

		// all done, now we need to cleanup
		if (bDestroy)
		{
			this->DestroyRender();
		}
	}

public:
	void DrawRoundRect(const RECT& rect, const HPEN pen, const HBRUSH brush)
	{
		HGDIOBJ old_pen = NULL;
		HGDIOBJ old_brush = NULL;

		if (pen)
			old_pen = SelectObject(m_pRender->hDC, pen);

		old_brush = SelectObject(m_pRender->hDC, brush);

		RoundRect(m_pRender->hDC, rect.left, rect.top, rect.right, rect.bottom, 0, 0);

		if (pen)
			SelectObject(m_pRender->hDC, old_pen);

		SelectObject(m_pRender->hDC, old_brush);
	}

private:
	HBITMAP				oldBmp;
	HBITMAP				newBmp;

	GDI_DRAW_INFO		m_origDrawInfo;

public:
	GDI_DRAW_INFO_PTR	m_pRender;
};

/**********************************************************************************
* Class name: GDIplusCtrlRender information
* Provides type and define common function time
***********************************************************************************/
class Dllexport GDIplusCtrlRender: public IControlRender
{
private:
	void CreateRender(const HDC& hdc, const RECT& rect)
	{
		m_origDrawInfo.hDC  = hdc;
		m_origDrawInfo.rect = rect;

		m_DrawInfo.hDC  = CreateCompatibleDC(hdc);
		m_DrawInfo.rect = rect;

		newBmp = CreateCompatibleBitmap(m_origDrawInfo.hDC, 
										m_origDrawInfo.rect.right  - m_origDrawInfo.rect.left,
										m_origDrawInfo.rect.bottom - m_origDrawInfo.rect.top);

		oldBmp = (HBITMAP)SelectObject(m_DrawInfo.hDC, newBmp);

		// gdiplus render main
		m_pRender = new GDIPLUS_DRAW_INFO();

		m_pRender->render = Gdiplus::Graphics::FromHDC(m_DrawInfo.hDC);
		m_pRender->render->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		m_pRender->rect = ConvertToGdiplusRect(rect, 0, 0);
	}

	void DestroyRender()
	{
		SAFE_DELETE(m_pRender->render);
		SAFE_DELETE(m_font_render);
		SAFE_DELETE(m_pRender);

		SelectObject(m_DrawInfo.hDC, oldBmp);	// select back original bitmap
		DeleteObject(newBmp);					// delete bitmap since it is no longer required
		DeleteDC(m_DrawInfo.hDC);				// delete memory DC since it is no longer required
	}

public:
	GDIplusCtrlRender() : newBmp(NULL), oldBmp(NULL)
		, m_font_render(NULL)
		, m_pRender(NULL)
		, m_smoothMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias)
	{
	}

	GDIplusCtrlRender(const HDC& hdc, const RECT& rect):
		  newBmp(NULL), oldBmp(NULL)
		, m_font_render(NULL)
		, m_pRender(NULL)
	{
		this->Init(hdc, rect);
	}

	~GDIplusCtrlRender()
	{
		this->Flush(true);
	}

	void* Render()
	{
		if (m_pRender->render != NULL)
		{
			return m_pRender;
		}
		return NULL;
	}

	HDC GetHDC()
	{
		return m_DrawInfo.hDC;
	}

	void Init(const HDC& hdc, const RECT& rect)
	{
		this->CreateRender(hdc, rect);
	}

	Gdiplus::Rect GetDrawRect()
	{
		return m_pRender->rect;
	}

	void BeginDrawRect(const RECT& rect)
	{
		m_stackRect.push(m_pRender->rect);
		m_pRender->rect = ConvertToGdiplusRect(rect);
	}

	void EndDrawRect(bool bDraw = false)
	{
		// restore draw view old -> check set draw rect
		if (!m_stackRect.empty())
		{
			m_pRender->rect = m_stackRect.top();
			m_stackRect.pop();
		}

		// handle instant drawing immediately.
		if (bDraw)
		{
			Flush();
		}
	}

public:
	void LoadFont(const wchar_t* family= L"Arial")
	{
		Gdiplus::FontFamily fontFamily(family);

		m_font_render = new Gdiplus::Font(&fontFamily, 9, Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
	}

	void Flush(bool bDestroy = false)
	{
		// TODO: http://www.winprog.org/tutorial/transparency.html
		BitBlt(m_origDrawInfo.hDC, 0, 0, m_origDrawInfo.rect.right - m_origDrawInfo.rect.left,
			m_origDrawInfo.rect.bottom - m_origDrawInfo.rect.top, m_DrawInfo.hDC, 0, 0, SRCCOPY);

		// all done, now we need to cleanup
		if (bDestroy)
		{
			this->DestroyRender();
		}
	}

	void AttachSmoothingMode(Gdiplus::SmoothingMode mode)
	{
		NULL_RETURN(m_pRender->render,);

		m_stackSmoothMode.push(m_smoothMode);
		m_smoothMode = mode;

		m_pRender->render->SetSmoothingMode(m_smoothMode);
	}

	void DetachSmoothingMode()
	{
		NULL_RETURN(m_pRender->render, );

		if (!m_stackSmoothMode.empty())
		{
			m_smoothMode = m_stackSmoothMode.top();
			m_stackSmoothMode.pop();
		}
		else
		{
			m_smoothMode = Gdiplus::SmoothingMode::SmoothingModeDefault;
		}
		m_pRender->render->SetSmoothingMode(m_smoothMode);
	}

// Function support draw gdiplus
private:
	static Gdiplus::Rect ConvertToGdiplusRect(const RECT& rect, const int offset_x = 0, const int offset_y = 0)
	{
		return Gdiplus::Rect(rect.left, rect.top, (rect.right - rect.left) + offset_x, (rect.bottom - rect.top )+ offset_y);
	}

	static Gdiplus::RectF Rect2RectF(const Gdiplus::Rect* rect)
	{
		return Gdiplus::RectF(rect->X, rect->Y, rect->Width, rect->Height);
	}

	static void funcDrawRoundRectangle(Gdiplus::Graphics* g, const Gdiplus::Pen* p, const Gdiplus::Rect& rect, const int radius)
	{
		Gdiplus::GraphicsPath path;

		path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
		path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
		path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
		path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
		path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
		path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
		path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
		path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
		path.CloseFigure();

		g->DrawPath(p, &path);
	}

	void funcFillRoundRectangle(Gdiplus::Graphics* g, const Gdiplus::Brush* brush, const Gdiplus::Rect& rect,const int radius)
	{
		Gdiplus::GraphicsPath path;

		path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
		path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
		path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
		path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
		path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
		path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
		path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
		path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
		path.CloseFigure();

		g->FillPath(brush, &path);
	}

	void funDrawImage(Gdiplus::Graphics* g, Gdiplus::Image* img, const GdiplusEx::ImageFormat* imageformat, const Gdiplus::PointF& offset= Gdiplus::PointF(0,0))
	{
		using namespace Gdiplus;

		Gdiplus::RectF rectbound = Rect2RectF(&m_pRender->rect);
		Gdiplus::RectF rect = rectbound;

		unsigned int image_width = img->GetWidth();
		unsigned int image_height = img->GetHeight();

		if (imageformat)
		{
			// alignment x
			if (imageformat->m_veralign == GdiplusEx::ImageAlignment::ImageAlignmentCenter)
			{
				rect.X = rectbound.X + offset.X + (rectbound.Width - image_width) / 2;
			}
			else if (imageformat->m_veralign == GdiplusEx::ImageAlignment::ImageAlignmentFar)
			{
				rect.X = rectbound.X + offset.X + rectbound.Width - image_width;
			}
			else
			{
				rect.X = rectbound.X + offset.X;
			}

			// Y alignment
			if (imageformat->m_horalign == GdiplusEx::ImageAlignment::ImageAlignmentCenter)
			{
				rect.Y = rectbound.Y + offset.Y + (rectbound.Height - image_height) / 2;
			}
			else if (imageformat->m_horalign == GdiplusEx::ImageAlignment::ImageAlignmentFar)
			{
				rect.Y = rectbound.Y + offset.Y + rectbound.Height - image_height;
			}
			else
			{
				rect.Y = rectbound.Y + offset.Y;
			}

			int rect_width  = image_width < rectbound.Width ? image_width : rectbound.Width;
			int rect_height = image_height < rectbound.Height ? image_height : rectbound.Height;

			rect.Width  = rect_width;
			rect.Height = rect_height;

			rect.X += imageformat->m_offset.Width;
			rect.Y += imageformat->m_offset.Height;
		}

		g->DrawImage(img, rect);
	}

	void funDrawText(Gdiplus::Graphics* g, 
					const Gdiplus::RectF* rect,
					const wchar_t* text,
					const Gdiplus::Font* font,
					const Gdiplus::Brush* brush,
					const Gdiplus::StringFormat* stringFormat)
	{
		g->DrawString(text, -1, font, *rect, stringFormat, brush);
	}


	void funDrawLine(Gdiplus::Graphics* g, Gdiplus::PointF* pStart, Gdiplus::PointF* pEnd, const Gdiplus::Pen* pen = NULL)
	{
		g->DrawLine(pen, *pStart, *pEnd);
	}

// Draw function
public:
	Gdiplus::RectF GetRectWchar(const wchar_t ch, const Gdiplus::RectF& rectLayout, Gdiplus::StringFormat* stringFormat)
	{
		Gdiplus::RectF rect = { 0, 0 ,0 ,0 };
		NULL_RETURN(m_pRender->render, rect);

		Gdiplus::CharacterRange charRanges(0, 1);
		stringFormat->SetMeasurableCharacterRanges(1, &charRanges);

		int count = stringFormat->GetMeasurableCharacterRangeCount();
		Gdiplus::Region pCharRangeRegions;

		if (m_pRender->render->MeasureCharacterRanges(&ch, 1, m_font_render, rectLayout, stringFormat, 1, &pCharRangeRegions)
			== Gdiplus::Ok)
		{
			pCharRangeRegions.GetBounds (&rect, m_pRender->render);

			Gdiplus::SolidBrush redBrush(Gdiplus::Color(100, 255, 0, 0));
			m_pRender->render->FillRegion(&redBrush, &pCharRangeRegions);
		}
		return rect;
	}

	Gdiplus::RectF MeasureString(const wchar_t* str, const int length, const Gdiplus::StringFormat* stringFormat = NULL)
	{
		Gdiplus::RectF rect = { 0, 0 ,0 ,0 };

		NULL_RETURN(m_pRender->render, rect);

		Gdiplus::PointF oriPoint{0,0};

		if (Gdiplus::Status::Ok != m_pRender->render->MeasureString(str, length, m_font_render, oriPoint, stringFormat, &rect))
		{
			std::cerr << " MeasureString failed !" << std::endl;
		}

		return rect;
	}

	void DrawTriangle(const Gdiplus::PointF& p1, const Gdiplus::PointF& p2, const Gdiplus::PointF& p3,
					  const Gdiplus::Pen* pen, const Gdiplus::Brush* brush)
	{
		NULL_RETURN(m_pRender->render, );

		Gdiplus::PointF points[] = { p1, p2, p3 };

		if (pen)
			m_pRender->render->DrawPolygon(pen, points, 3);
		if (brush)
			m_pRender->render->FillPolygon(brush, points, 3);
	}

	void DrawRectangle1(const Gdiplus::RectF& rect, const Gdiplus::Pen* pen, const Gdiplus::Brush* brush)
	{
		NULL_RETURN(m_pRender->render, );

		//Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(brush);
		if(pen)
			m_pRender->render->DrawRectangle(pen, rect);
		if(brush)
			m_pRender->render->FillRectangle(brush, rect);
		//delete pBrush;
	}

	void DrawRectangle(const Gdiplus::Rect& rect, const Gdiplus::Pen* pen, const Gdiplus::ARGB& brush, int radius)
	{
		NULL_RETURN(m_pRender->render, );

		Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(brush);

		DrawRectangle(rect, pen, pBrush, radius);

		delete pBrush;
	}

	void DrawRectangle(const Gdiplus::Rect& rect, const Gdiplus::ARGB& pen, const Gdiplus::Brush* brush,int iborderwidth, int radius)
	{
		NULL_RETURN(m_pRender->render, );

		Gdiplus::Pen* pPen = new Gdiplus::Pen(pen, iborderwidth);

		DrawRectangle(rect, pPen, brush, radius);

		delete pPen;
	}

	void DrawRectangle(const Gdiplus::Rect& rect, const Gdiplus::Pen* pen, const Gdiplus::Brush* brush, int radius)
	{
		NULL_RETURN(m_pRender->render, );

		if (brush)
			funcFillRoundRectangle(m_pRender->render, brush, rect, radius);
		if (pen && pen->GetWidth() > 0)
			funcDrawRoundRectangle(m_pRender->render, pen, rect, radius);
	}

	void DrawRectangleFull(const Gdiplus::Pen* pen, const Gdiplus::Brush* brush, int radius)
	{
		NULL_RETURN(m_pRender->render, );

		if(brush)
			funcFillRoundRectangle(m_pRender->render, brush, m_pRender->rect, radius);
		if(pen && pen->GetWidth() > 0)
			funcDrawRoundRectangle(m_pRender->render, pen, m_pRender->rect, radius);
	}

	void DrawTextRect(	const Gdiplus::RectF&		 rect,
						const wchar_t*				 text,
						const Gdiplus::Brush*		 brush,
						const Gdiplus::StringFormat* stringFormat = NULL,
						const Gdiplus::PointF&		 offset = Gdiplus::PointF(0, 0))
	{
		if (!m_pRender->render || !brush || !m_font_render) return;
		Gdiplus::RectF rectf = rect;
		rectf.X += offset.X;
		rectf.Y += offset.Y;

		funDrawText(m_pRender->render, &rectf, text, m_font_render, brush, stringFormat);
	}

	void DrawLine(const Gdiplus::PointF& pStart, const Gdiplus::PointF& pEnd,
				  const Gdiplus::Pen&	pen)
	{
		NULL_RETURN(m_pRender->render, );

		m_pRender->render->DrawLine(&pen, pStart, pEnd);
	}

	void DrawTextInSideRect(const Gdiplus::RectF&	rect,
							const Gdiplus::PointF&	ptStartText,
							const wchar_t*			text,
							const Gdiplus::Brush*	brush,
							const Gdiplus::StringFormat* stringFormat = NULL)
	{
		Gdiplus::GraphicsPath path;
		path.AddRectangle(rect);

		Gdiplus::Region region(&path);
		m_pRender->render->SetClip(rect);
		{
			m_pRender->render->DrawString(text, -1, m_font_render, ptStartText, stringFormat, brush);
		}
		m_pRender->render->ResetClip();
	}

	void DrawTextRect(	const Gdiplus::Rect&		 rect,
						const wchar_t*				 text,
						const Gdiplus::Brush*		 brush,
						const Gdiplus::StringFormat* stringFormat = NULL,
						const Gdiplus::PointF&		 offset = Gdiplus::PointF(0, 0))
	{
		if (!m_pRender->render || !brush || !m_font_render) return;

		Gdiplus::RectF rectf = Rect2RectF(&rect);
		rectf.X += offset.X;
		rectf.Y += offset.Y;

		funDrawText(m_pRender->render, &rectf, text, m_font_render, brush, stringFormat);
	}

	void DrawTextFullRect(const wchar_t* text, const Gdiplus::Brush* brush, const Gdiplus::StringFormat* stringFormat = NULL, const Gdiplus::PointF& offset = Gdiplus::PointF(0, 0))
	{
		if (!m_pRender->render || !brush || !m_font_render) return;

		Gdiplus::RectF rectf = Rect2RectF(&m_pRender->rect);
		rectf.X += offset.X;
		rectf.Y += offset.Y;

		funDrawText(m_pRender->render, &rectf, text, m_font_render, brush, stringFormat);
	}

	void DrawImageFullRect(Gdiplus::Image* img, const GdiplusEx::ImageFormat* imageFormat = NULL, const Gdiplus::PointF& offset = Gdiplus::PointF(0, 0))
	{
		NULL_RETURN(m_pRender, );

		funDrawImage(m_pRender->render, img, imageFormat, offset);
	}

	void DrawLineFull(Gdiplus::PointF& ptStart , Gdiplus::PointF& ptEnd, const Gdiplus::Pen* pen= NULL)
	{
		NULL_RETURN(m_pRender, );
		NULL_RETURN(pen, );

		funDrawLine(m_pRender->render, &ptStart, &ptEnd, pen);
	}

private:
	HBITMAP					oldBmp;
	HBITMAP					newBmp;
	GdiplusToken			m_gdiplus;

private:
	GDI_DRAW_INFO			m_origDrawInfo;
	GDI_DRAW_INFO			m_DrawInfo;

private:
	GDIPLUS_DRAW_INFO_PTR		m_pRender;
	Gdiplus::Font*				m_font_render;
	std::stack<Gdiplus::Rect>	m_stackRect;


	std::stack<Gdiplus::SmoothingMode>	m_stackSmoothMode;
	Gdiplus::SmoothingMode				m_smoothMode;
};

/**********************************************************************************
* GetMonitorInfoEx fucntion
* Provides type and define common function time
***********************************************************************************/
static xMonitorInfo GetMonitorInfoEx()
{
	xMonitorInfo infor;
	DEVMODE devmode;

	devmode.dmSize = sizeof(DEVMODE);
	BOOL bResult = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

	if (bResult)
	{
		infor.WIDTH		= devmode.dmPelsWidth;
		infor.HEIGHT	= devmode.dmPelsHeight;
		infor.DISFREQ	= devmode.dmDisplayFrequency;
		infor.VERSION	= devmode.dmDriverVersion;
		memcpy_s(infor.NAME, MAX_DEVICENAME, devmode.dmDeviceName, MAX_DEVICENAME);
	}

	return infor;
}
____END_NAMESPACE____

#endif // WCONTROL_H

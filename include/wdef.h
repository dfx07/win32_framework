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

#include <Windows.h>
#include <gdiplus.h>

#include "xsysdef.h"


// disable waring visual studio c++14
#pragma warning( disable : 4244)

#define GL_WIN_CLASS	 L"FOX_WINHANDLE_CLASS"
#define GL_SUBWIN_CLASS  L"FOX_SUBWINHANDLE_CLASS"

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
	HWND	m_hWnd ;
	HDC		m_hDc  ;
	HGLRC	m_hGLRC;

	bool	m_bCreated;	// create ok

} *WindowRenderPtr;


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

public:
	float	 r;
	float	 g;
	float	 b;
	float	 a;

	Gdiplus::ARGB wrefcol;
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
		m_pRender->rect = ConvertToGdiplusRect(rect, -2, -2);
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
			return &m_pRender;
		}
		return NULL;
	}

	void Init(const HDC& hdc, const RECT& rect)
	{
		this->CreateRender(hdc, rect);
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

// Function support draw gdiplus
private:
	static Gdiplus::Rect ConvertToGdiplusRect(const RECT& rect, const int offset_x = 0, const int offset_y = 0)
	{
		return Gdiplus::Rect(rect.left, rect.top, rect.right + offset_x, rect.bottom + offset_y);
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

// Draw function
public:
	void DrawRectangle(const Gdiplus::Pen* pen, const Gdiplus::Brush* brush, int radius)
	{
		if (!m_pRender->render) return;

		if(brush)
			funcFillRoundRectangle(m_pRender->render, brush, m_pRender->rect, radius);
		if(pen)
			funcDrawRoundRectangle(m_pRender->render, pen, m_pRender->rect, radius);
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
		if (!m_pRender) return;

		funDrawImage(m_pRender->render, img, imageFormat, offset);
	}

private:
	HBITMAP					oldBmp;
	HBITMAP					newBmp;
	GdiplusToken			m_gdiplus;

public:
	GDI_DRAW_INFO			m_origDrawInfo;
	GDI_DRAW_INFO			m_DrawInfo;

public:
	GDIPLUS_DRAW_INFO_PTR	m_pRender;
	Gdiplus::Font*			m_font_render;
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

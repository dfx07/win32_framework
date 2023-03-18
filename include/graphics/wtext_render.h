////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     wtext_render.h
* @create   Nov 15, 2022
* @brief    Create and handle opengl render
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XLABEL_H
#define XLABEL_H

#include "xsysdef.h"

#include <Windows.h>
#include <iostream>
#include <unordered_map>

#include "GL/glew.h"

___BEGIN_NAMESPACE___

/* =================================================================================
Display 2D text use system font . not support zoom
Feature:
+ Support bitmap vietnamese
+ Support special characters
Bitmap fonts offer a simple way to display 2D text on the screen.
Information about the characters in a bitmap font is stored as bitmap images.

[*] Advantage to bitmap fonts is that they provide a high performance method for render
image text to the screen
[*] If you not use Unicode , please reduce RANG_BASE_LIST = asscii
====================================================================================*/
#pragma comment (lib,"Glu32.lib")

#ifdef X_UNICODE
#define RANGE_DATA 9000
#else
#define RANGE_DATA 128
#endif


/**********************************************************************************
* ⮟⮟ Class name: MenuContext control
* MenuContext control for window
***********************************************************************************/
class GLWinFontRender
{
	enum { RANG_BASE_LIST = RANGE_DATA };
	typedef std::unordered_map<short, unsigned int> MapCharacter;

public:
	enum class FontType
	{
		Normal	,
		Bold	,
		Thin	,
	};

private:
	// Render information
	GLuint			m_textbase;
	GLuint			m_renlist;

	HWND			m_hwnd;
	HDC				m_hdc;
	HFONT			m_hfont;

	// View information
	int				m_width;
	int				m_height;

	bool			m_bInitOK;

	MapCharacter	m_character_map_list;

public:
	//==================================================================================
	// Get text bound size (width , height)                                             
	//==================================================================================
	void GetSizeText(const char* text, int& width, int& height)
	{
		SelectObject(m_hdc, m_hfont);
		SIZE _size;
		BOOL rel = GetTextExtentPointA(m_hdc, text, (int)strlen(text), &_size);

		if (rel) // it OK
		{
			width  = (int)_size.cx;
			height = (int)_size.cy;
		}
		else
		{
			width = height = 0;
		}
	}

	void GetSizeText(const wchar_t* text, int& width, int& height)
	{
		SelectObject(m_hdc, m_hfont);
		SIZE _size;
		BOOL rel = GetTextExtentPointW(m_hdc, text, (int)wcslen(text), &_size);

		if (rel) // it OK
		{
			width = (int)_size.cx;
			height = (int)_size.cy;
		}
		else
		{
			width = height = 0;
		}
	}

private:
	//==================================================================================
	// Load font system data                                                            
	//==================================================================================
	bool CreateBitmapFont(const char* fontname, unsigned int fontsize, DWORD fonttype)
	{
		if (m_hdc == NULL) return false;

		if (m_hfont) DeleteObject(m_hfont); // delete old font

		// Create list all charactor vietnamese
		m_textbase = glGenLists(RANG_BASE_LIST);

		if (m_textbase == 0) return false;

		m_hfont = CreateFontA(fontsize, 0, 0, 0, fonttype, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
				FF_DONTCARE | DEFAULT_PITCH, fontname);

		// Verify font creation
		if (!m_hfont)
			return false;

		// Select a device context for the font
		SelectObject(m_hdc, m_hfont);

		if (!wglUseFontBitmapsW(m_hdc, 32, RANG_BASE_LIST, m_textbase))
		{
			DeleteObject(m_hfont);
			m_hfont = NULL;
		}

		return m_hfont ? true : false;
	}

	//==================================================================================
	// Load character by character -> store to map character data                       
	// Defect    : Slow program                                                         
	//==================================================================================
	unsigned int GetWchar(const wchar_t& ch)
	{
		if (m_character_map_list.find(ch) != m_character_map_list.end())
		{
			return m_character_map_list[ch];
		}

		unsigned int id_list_char = glGenLists(1);
		SelectObject(m_hdc, m_hfont);

		BOOL rel = wglUseFontBitmaps(m_hdc, (DWORD)ch, 1, id_list_char);
		rel = (!rel) ? wglUseFontBitmapsW(m_hdc, (DWORD)ch, 1, id_list_char) : rel;

		if (!rel)
		{
			glDeleteLists(id_list_char, 1);
			return 0;
		}

		// Add list id to map character
		m_character_map_list[ch] = id_list_char;

		return id_list_char;
	}

public:
	GLWinFontRender() : m_hfont(NULL), m_bInitOK(false),
		m_hwnd(NULL)
	{
		m_textbase	= 0;
		m_hdc		= NULL;
		m_width		= 0;
		m_height	= 0;
		m_renlist	= 0;
	}

	~GLWinFontRender()
	{
		DeleteObject(m_hfont);

		glDeleteLists(m_textbase, RANG_BASE_LIST);
		glDeleteLists(m_renlist, 1);

		for (auto& it : m_character_map_list)
		{
			glDeleteLists(it.second, 1);
		}

		ReleaseDC(m_hwnd, m_hdc);
	}

	void Create(HWND hwnd, int width, int height)
	{
		m_hwnd		= hwnd;
		m_hdc		= GetDC(hwnd);
		m_width		= width;
		m_height	= height;
	}

	//==================================================================================
	// Update screen view information                                                   
	//==================================================================================
	void UpdateView(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

	//==================================================================================
	// Create font                                                                      
	//==================================================================================
	bool LoadFont(const char* fontname, unsigned int fontsize, FontType fonttype = FontType::Normal)
	{
		// Create font system
		DWORD sfonttype = FW_REGULAR;
		switch (fonttype)
		{
		case FontType::Bold:
			sfonttype = FW_BOLD;
			break;
		case FontType::Thin:
			sfonttype = FW_THIN;
			break;
		default:
			break;
		}

		m_bInitOK = CreateBitmapFont(fontname, fontsize, sfonttype);

		if (!m_bInitOK)
		{
			std::cerr << "Create bitmap font failed" << std::endl;
		}

		return m_bInitOK;
	}

	//==================================================================================
	// Use before render text to device context                                         
	//==================================================================================
	void Use()
	{
		if (!m_bInitOK) return;

		// Only initialized once
		if (!m_renlist)
		{
			m_renlist = glGenLists(1);

			glNewList(m_renlist, GL_COMPILE);
			{
				glListBase(m_textbase - 32);

				// Push information matrix
				glPushAttrib(GL_LIST_BIT);

				// Load model view matrix
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();

				// Load projection matrix + can use glm;
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glLoadIdentity();

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_DEPTH_TEST);
			}
			glEndList();
		}

		glCallList(m_renlist);
		gluOrtho2D(0.0, m_width, m_height, 0.0);
	}

	//==================================================================================
	// Use after render text to device context                                          
	//==================================================================================
	void DontUse()
	{
		if (!m_bInitOK) return;

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// Restore matrices
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glListBase(0);
		// Pop information matrix
		glPopAttrib();
	}

	//==================================================================================
	// Render text Assci to device context                                              
	//==================================================================================
	void Write(int x, int y, const char* text, float r = 1.0f, float g = 1.0f, // Text out  [input]
		float b = 1.0f, float a = 1.0f)
	{
		if (!m_bInitOK || m_textbase == 0 || !text)
			return;

		glColor4f(r, g, b, a);
		glRasterPos2i(x, y);
		glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);
	}

	//==================================================================================
	// Render text Unicode to device context - Each element                             
	// [*] it slooww -> Use for special characters                                      
	//==================================================================================
	void WriteEach(int x, int y, const wchar_t* text, float r = 1.0f, float g = 1.0f, // Text out  [input]
		float b = 1.0f, float a = 1.0f)
	{
		if (!m_bInitOK || m_textbase == 0 || !text)
			return;

		glColor4f(r, g, b, a);
		glRasterPos2i(x, y);

		GLint id = 0;
		for (int i = 0; i < wcslen(text); i++)
		{
			if (short(text[i]) > RANG_BASE_LIST)
			{
				id = GetWchar(text[i]);
			}
			else
			{
				// 31 because 32 list not init + 1 list complie
				id = text[i] - 31;
			}

			if (id <= 0) continue;
			glCallList((GLuint)id);
		}
	}

	//==================================================================================
	// Render text Unicode to device context  - All                                     
	// In case if you can't display the UNICODE character please Use: WriteEach         
	//==================================================================================
	void Write(int x, int y, const wchar_t* text, float r = 1.0f, float g = 1.0f, // Text out  [input]
		float b = 1.0f, float a = 1.0f)
	{
		if (!m_bInitOK || m_textbase == 0 || !text)
			return;

		glColor4f(r, g, b, a);
		glRasterPos2i(x, y);
		glCallLists((int)wcslen(text), GL_UNSIGNED_SHORT, text);
	}
};

____END_NAMESPACE____

#endif // !XLABEL_H
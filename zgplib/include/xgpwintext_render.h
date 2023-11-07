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
#ifndef XGPWINTEXT_RENDER_H
#define XGPWINTEXT_RENDER_H

#include "xsysdef.h"

#include <Windows.h>
#include <iostream>
#include <unordered_map>

#include "GL/glew.h"

____BEGIN_SECTION____(gp)

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
class Dllexport GLWinFontRender
{
	enum { RANG_BASE_LIST = RANGE_DATA };
	typedef std::unordered_map<short, unsigned int> MapCharacter;

public:
	enum class FontType
	{
		Normal,
		Bold,
		Thin,
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
			width = (int)_size.cx;
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
	bool CreateBitmapFont(const char* fontname, unsigned int fontsize, DWORD fonttype);

	//==================================================================================
	// Load character by character -> store to map character data                       
	// Defect    : Slow program                                                         
	//==================================================================================
	unsigned int GetWchar(const wchar_t& ch);

public:
	GLWinFontRender() : m_hfont(NULL), m_bInitOK(false),
		m_hwnd(NULL)
	{
		m_textbase = 0;
		m_hdc = NULL;
		m_width = 0;
		m_height = 0;
		m_renlist = 0;
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
		m_hwnd = hwnd;
		m_hdc = GetDC(hwnd);
		m_width = width;
		m_height = height;
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
	bool LoadFont(const char* fontname, unsigned int fontsize, FontType fonttype = FontType::Normal);

	//==================================================================================
	// Use before render text to device context                                         
	//==================================================================================
	void Use()
	{

	}

	//==================================================================================
	// Use after render text to device context                                          
	//==================================================================================
	void DontUse();

	//==================================================================================
	// Render text Assci to device context                                              
	//==================================================================================
	void Write(int x, int y, const char* text,  float r = 1.0f, float g = 1.0f, // Text out  [input]
												float b = 1.0f, float a = 1.0f);

	//==================================================================================
	// Render text Unicode to device context - Each element                             
	// [*] it slooww -> Use for special characters                                      
	//==================================================================================
	void WriteEach(int x, int y, const wchar_t* text, float r = 1.0f, float g = 1.0f, // Text out  [input]
													  float b = 1.0f, float a = 1.0f);

	//==================================================================================
	// Render text Unicode to device context  - All                                     
	// In case if you can't display the UNICODE character please Use: WriteEach         
	//==================================================================================
	void Write(int x, int y, const wchar_t* text, float r = 1.0f, float g = 1.0f, // Text out  [input]
												  float b = 1.0f, float a = 1.0f);
};

_____END_SECTION_____

#endif // !XGPWINTEXT_RENDER_H
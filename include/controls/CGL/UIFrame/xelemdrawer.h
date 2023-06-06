////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xUIElement.h
* @create   june 15, 2023
* @brief    Create property control
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XUIELEMENT_H
#define XUIELEMENT_H

#include "xsysdef.h"
#include "graphics/gp_shader.h"
#include <vector>

___BEGIN_NAMESPACE___

____BEGIN_SECTION____(gui)

typedef struct tagContentColor
{
	Color4 bordercolor;
	Color4 bkcolor;
} CONTENT_COLOR;

typedef struct tagRectangle
{
	float  width;
	float  height;
	float  border;
	float  radius;
	float  x;
	float  y;
	float  z;
} RECT_DRAW_DATA;

typedef std::vector<std::mar tagRectangle> CacheRectangle;

class XDataDrawer
{
public:
	CacheRectangle		m_rectangle_cache;

public:
	XDataDrawer()
	{
		m_rectangle_cache.reserve(100);
	}

public:
	int AddRectangle(RECT_DRAW_DATA& rect, CONTENT_COLOR& color)
	{

	}

	bool UpdateRectangleData(int index, RECT_DRAW_DATA* rect = NULL, CONTENT_COLOR* color = NULL)
	{

	}
};

class XGLDrawer : protected XDataDrawer
{
protected:
	Shader				m_rectangle_shader;
	Shader				m_line_shader;

protected:
	virtual void Init(const wchar_t* pathVertex, const wchar_t* pathFragment = NULL, const wchar_t* pathGeometry = NULL) = 0;

	virtual void Create() { };

	virtual void DrawRectangle() {};

	virtual void DrawLine() {};

	virtual void DrawCicle() {};

	virtual void UpdateTime() {};
};


class VukaDrawer : protected XDataDrawer
{

};

_____END_SECTION_____

____END_NAMESPACE____

#endif // !XUIELEMENT_H

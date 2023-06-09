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
#include "graphics/gp_shader_type.h"
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
	SD_Point3D		_pt;
	SD_Size2D		_size;
	SD_Border2D		_border;
	SD_Color4		_bgColor;
	SD_Color4		_bdColor;

} RECT_DRAW_DATA;

typedef struct tagCircle
{
	SD_Point3D		_pt;
	SD_Size2D		_size;
	SD_Border2D		_border;
	SD_Color4		_bgColor;
	SD_Color4		_bdColor;

} CRICLE_DRAW_DATA;



class SUpdateListManage
{

	typedef std::vector<RECT_DRAW_DATA> CacheData;

	CacheData m_data;

public:

	void Clear()
	{
		m_data.clear();
	}

	void Add(const RECT_DRAW_DATA& data)
	{

	}
}


class XShapeData
{
	virtual void Clear() = 0;
};

class CRectangleSDData : public XShapeData
{
protected:
	typedef std::vector<RECT_DRAW_DATA> CacheRectangle;

protected:
	CacheRectangle		m_rect;

public:
	void Add()
	{

	}
};

class XShapeDrawer
{
protected:
	virtual void Init(const wchar_t* pathVertex, const wchar_t* pathFragment = NULL, const wchar_t* pathGeometry = NULL) = 0;
	virtual void Create() = 0;
};

class CRectangleDrawer : public XShapeDrawer
{
	virtual void Init(const wchar_t* pathVertex, const wchar_t* pathFragment = NULL, const wchar_t* pathGeometry = NULL)
	{

	}
	virtual void Create()
	{

	}
};


_____END_SECTION_____

____END_NAMESPACE____

#endif // !XUIELEMENT_H

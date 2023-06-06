////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xguictrlbase.h
* @create   june 15, 2023
* @brief    Create property control
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGUICTRLBASE_H
#define XGUICTRLBASE_H

#include "xsysdef.h"

___BEGIN_NAMESPACE___

____BEGIN_SECTION____(gui)

enum class Visible
{
	SHOW,
	HIDE
};

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

template< typename T>
struct ContentCtrl
{
	T left;
	T top;
	T right;
	T bottom;

public:
	ContentCtrl() : left(static_cast<T>(0)), top(static_cast<T>(0)),
		right(static_cast<T>(0)), bottom(static_cast<T>(0))
	{

	}

	template< typename U>
	ContentCtrl(const U& value) :
		left(static_cast<T>(value)), top(static_cast<T>(value)),
		right(static_cast<T>(value)), bottom(static_cast<T>(value))
	{

	}

	template< typename A, typename B, typename C, typename D>
	ContentCtrl(const A& _left,
				const B& _top    = static_cast<B>(0),
				const C& _right  = static_cast<C>(0),
				const D& _bottom = static_cast<D>(0)):
		left(static_cast<T>(_left)), top(static_cast<T>(_top)),
		right(static_cast<T>(_right)), bottom(static_cast<T>(_bottom))
	{

	}
};

typedef ContentCtrl<float> Padding;
typedef ContentCtrl<float> Margin;

class XControlGLBase
{

};

class XControlBase
{
protected:
	virtual void UpdateVisible()  {};
	virtual void UpdateEnable()   {};
	virtual void UpdatePosition() {};
	virtual void UpdateSize()     {};

public:
	void SetVisible(Visible eVisible)
	{
		m_visible = eVisible;
		this->UpdateVisible();
	}

	void SetVisible(bool bEnable)
	{
		m_enable = bEnable;
		this->UpdateEnable();
	}

protected:
	Location	m_pos;
	Size		m_size;

	Visible		m_visible; // 0 : Hidden | 1 : Show
	bool		m_enable;  // false : Disable| true : Enable
};


class XContentControlBase : public XControlBase
{
public:
	void    SetPadding(Padding padding)	{ m_padding = padding;}
	void    SetMargin(Margin margin)	{ m_margin = margin;  }
	Padding GetPadding()				{ return m_padding;   }
	Margin  GetMargin()					{ return m_margin;    }

protected:
	Padding		m_padding;
	Margin		m_margin;
};


_____END_SECTION_____

____END_NAMESPACE____

#endif // !XGUICTRLBASE_H

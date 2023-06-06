////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     XControlBase.h
* @create   june 15, 2023
* @brief    Create property control
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XCONTROLBASE_H
#define XCONTROLBASE_H

#include "wctrl.h"

___BEGIN_NAMESPACE___

____BEGIN_SECTION____(gui)

class XControlBase
{
protected:
	virtual void UpdateVisible() {};
	virtual void UpdateEnable() {};
	virtual void UpdatePosition() {};
	virtual void UpdateSize() {};

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
	void     SetPadding(CPadding padding)	{ m_padding = padding;	}
	void     SetMargin(CMargin margin)		{ m_margin = margin;	}
	CPadding GetPadding()					{ return m_padding;		}
	CMargin  GetMargin()					{ return m_margin;		}

protected:
	CPadding	m_padding;
	CMargin		m_margin;
};


_____END_SECTION_____

____END_NAMESPACE____

#endif // !XCONTROLBASE_H
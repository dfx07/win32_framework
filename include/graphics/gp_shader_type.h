////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     gp_shader_type.h
* @create   June 08, 2023
* @brief    Define shader data type
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef GP_SHADER_TYPE_H
#define GP_SHADER_TYPE_H

#include "xsysdef.h"
#include <vector>

___BEGIN_NAMESPACE___

typedef struct tagSD_Point3D
{
	float x; float y; float z;

} SD_Point3D;

typedef struct tagSD_Size2D
{
	float width; float height;

}SD_Size2D;

typedef struct tagSD_Border2D
{
	float width;
	float radius;

}SD_Border2D;

typedef struct tagSD_Color4
{
	float r;
	float g;
	float b;
	float a;

}SD_Color4;



class SUpdateListManage
{

	typedef std::vector<tagSD_Color4> CacheData;

	CacheData m_data;

public:

	void Clear()
	{
		m_data.clear();
	}

	void Add(const tagSD_Color4& data)
	{

	}
}

____END_NAMESPACE____

#endif // !GP_SHADER_TYPE_H
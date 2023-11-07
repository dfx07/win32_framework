////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     gp_geometry_render.h
* @create   April 15, 2023
* @brief    Use OpenGL draw basic shape
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGPGEORENDER_H
#define XGPGEORENDER_H

#include "xgeotype.h"
#include <vector>

____BEGIN_SECTION____(gp)

/**********************************************************************************
* ⮟⮟ Class name: GeometryRender
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport GeometryRender
{
	template<typename T>
	using CacheData = std::vector<T>;

	template<typename T>
	using BufferData = std::vector<T>;

	template<typename T>
	using PointList = std::vector<T>;

	typedef PointList<Point2D> PointList2D;
	typedef PointList<Point3D> PointList3D;

protected:

	typedef struct tagPointData
	{
		Point3D	m_pt;
		Vec3D	m_color;
		float	m_fSize;

	} PointData;

	typedef struct tagLineData
	{
		Point3D	 m_pt1;
		Point3D	 m_pt2;
		Color3	 m_color1;
		Color3	 m_color2;

		float	 m_width;
	} LineData;

	typedef struct tagTrigData
	{
		Point3D	 m_pt1;
		Point3D	 m_pt2;
		Point3D	 m_pt3;
		Color3	 m_color1;
		Color3	 m_color2;
		Color3	 m_color3;

	} TrigData;

	typedef struct tagQuaData
	{
		Point3D	 m_pt1;
		Point3D	 m_pt2;
		Point3D	 m_pt3;
		Color3	 m_color1;
		Color3	 m_color2;
		Color3	 m_color3;

	} QuaData;

	typedef struct tagPolyData
	{
		PointList3D		m_ptList;
		Color3			m_color;
	} PolyData;

public:
	GeometryRender();

	void AddLine(VERTEX_3D vtxStart, VERTEX_3D vtxEnd);
	void AddPoint(VERTEX_3D& vt, float _size);
	void AddTriangle(VERTEX_3D& vt1, VERTEX_3D& vt2, VERTEX_3D& vt3);
	void AddRectangle(VERTEX_3D vt1, VERTEX_3D vt2, VERTEX_3D vt3, VERTEX_3D vt4);
	void AddRectangle2D(float x, float y, float z, float width, float height, Color3 color = { 1.f ,1.f, 1.f });

public:
	void UpdateRenderData();

public:
	void DrawLine();
	void DrawRectangle();

public:
	void Draw();

public:
	void Clear();

public:
	// Cache data
	CacheData<tagLineData>	m_vecLineData;
	CacheData<tagTrigData>	m_vecTrigData;
	CacheData<tagPointData>	m_vecPointData;
	CacheData<PolyData>		m_vecPolyData;
	CacheData<PolyData>		m_vecRectData;

	// Render data
	BufferData<float>		m_LineDataRender;
	BufferData<float>		m_TrigDataRender;
	BufferData<float>		m_PointDataRender;
	BufferData<float>		m_RectDataRender;
};

_____END_SECTION_____

#endif // !XGPGEORENDER_H


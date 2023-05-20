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
#ifndef GP_GEOMETRY_RENDER_H
#define GP_GEOMETRY_RENDER_H

#include "math/xgeotype.h"
#include <GL/glew.h>


___BEGIN_NAMESPACE___

/**********************************************************************************
* ⮟⮟ Class name: GeometryRender
* Base class for window handle inheritance
***********************************************************************************/
class GeometryRender
{
	template<typename T>
	using CacheData   = std::vector<T>;

	template<typename T>
	using BufferData  = std::vector<T>;

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
	GeometryRender()
	{

	}

	virtual void AddLine(VERTEX_3D vtxStart, VERTEX_3D vtxEnd)
	{
		m_vecLineData.push_back({ vtxStart.pt, vtxEnd.pt, vtxStart.color, vtxEnd.color, 1.f });
	}

	virtual void AddPoint(VERTEX_3D& vt, float _size )
	{
		m_vecPointData.push_back({ vt.pt, vt.color, _size });
	}

	virtual void AddTriangle(VERTEX_3D& vt1, VERTEX_3D& vt2, VERTEX_3D& vt3)
	{
		m_vecTrigData.push_back({ vt1.pt, vt2.pt, vt3.pt, vt1.color, vt2.color, vt3.color });
	}

	virtual void AddRectangle(VERTEX_3D vt1, VERTEX_3D vt2, VERTEX_3D vt3, VERTEX_3D vt4)
	{
		m_vecRectData.push_back({{ vt1.pt, vt2.pt, vt3.pt, vt4.pt }, vt1.color});
	}

	virtual void AddRectangle2D(float x, float y, float z, float width, float height, Color3 color = {1.f ,1.f, 1.f})
	{
		Vec3D pt1 = { x			, y			, z};
		Vec3D pt2 = { x + width , y			, z};
		Vec3D pt3 = { x + width , y + height, z};
		Vec3D pt4 = { x			, y + height, z};

		m_vecRectData.push_back({ {pt1, pt2, pt3, pt4 }, color });
	}

public:
	void UpdateRenderData()
	{
		// Update Line Buffer 
		for (int i = 0; i < m_vecLineData.size(); i++)
		{
			LineData& line = m_vecLineData[i];

			m_LineDataRender.push_back(line.m_pt1.x);
			m_LineDataRender.push_back(line.m_pt1.y);
			m_LineDataRender.push_back(line.m_pt1.z);

			m_LineDataRender.push_back(line.m_color1.r);
			m_LineDataRender.push_back(line.m_color1.g);
			m_LineDataRender.push_back(line.m_color1.b);

			m_LineDataRender.push_back(line.m_pt2.x);
			m_LineDataRender.push_back(line.m_pt2.y);
			m_LineDataRender.push_back(line.m_pt2.z);

			m_LineDataRender.push_back(line.m_color2.r);
			m_LineDataRender.push_back(line.m_color2.g);
			m_LineDataRender.push_back(line.m_color2.b);
		}

		// Update Quads Buffer 
		for (int i = 0; i < m_vecRectData.size(); i++)
		{
			PolyData& poly = m_vecRectData[i];

			m_RectDataRender.push_back(poly.m_ptList[0].x);
			m_RectDataRender.push_back(poly.m_ptList[0].y);
			m_RectDataRender.push_back(poly.m_ptList[0].z);

			m_RectDataRender.push_back(poly.m_color.r);
			m_RectDataRender.push_back(poly.m_color.g);
			m_RectDataRender.push_back(poly.m_color.b);

			m_RectDataRender.push_back(poly.m_ptList[1].x);
			m_RectDataRender.push_back(poly.m_ptList[1].y);
			m_RectDataRender.push_back(poly.m_ptList[1].z);

			m_RectDataRender.push_back(poly.m_color.r);
			m_RectDataRender.push_back(poly.m_color.g);
			m_RectDataRender.push_back(poly.m_color.b);

			m_RectDataRender.push_back(poly.m_ptList[2].x);
			m_RectDataRender.push_back(poly.m_ptList[2].y);
			m_RectDataRender.push_back(poly.m_ptList[2].z);

			m_RectDataRender.push_back(poly.m_color.r);
			m_RectDataRender.push_back(poly.m_color.g);
			m_RectDataRender.push_back(poly.m_color.b);

			m_RectDataRender.push_back(poly.m_ptList[3].x);
			m_RectDataRender.push_back(poly.m_ptList[3].y);
			m_RectDataRender.push_back(poly.m_ptList[3].z);

			m_RectDataRender.push_back(poly.m_color.r);
			m_RectDataRender.push_back(poly.m_color.g);
			m_RectDataRender.push_back(poly.m_color.b);
		}
	}


public:
	void DrawLine()
	{
		if (m_LineDataRender.size() <= 0)
			return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &m_LineDataRender[0]);
		glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat) , &m_LineDataRender[3]);

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_LineDataRender.size() / 6));

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	void DrawRectangle()
	{
		if (m_RectDataRender.size() <= 0)
			return;

		glPolygonMode(GL_BACK, GL_LINE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &m_RectDataRender[0]);
		glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &m_RectDataRender[3]);

		glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(m_RectDataRender.size() / 6));

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glPolygonMode(GL_BACK, GL_FILL);
	}

public:

	void Draw()
	{
		this->DrawLine();

		this->DrawRectangle();
	}

public:

	void Clear()
	{
		m_vecLineData.clear();
		m_vecTrigData.clear();
		m_vecPointData.clear();
	}

public:
	// Cache data
	CacheData<tagLineData>	m_vecLineData ;
	CacheData<tagTrigData>	m_vecTrigData ;
	CacheData<tagPointData>	m_vecPointData;
	CacheData<PolyData>		m_vecPolyData;
	CacheData<PolyData>		m_vecRectData;

	// Render data
	BufferData<FLOAT>		m_LineDataRender;
	BufferData<FLOAT>		m_TrigDataRender;
	BufferData<FLOAT>		m_PointDataRender;
	BufferData<FLOAT>		m_RectDataRender;
};

____END_NAMESPACE____

#endif // !GP_GEOMETRY_RENDER_H


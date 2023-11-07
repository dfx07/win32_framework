#include "xgpgeo_render.h"
#include <GL/glew.h>

____BEGIN_SECTION____(gp)

GeometryRender::GeometryRender()
{

}

void GeometryRender::AddLine(VERTEX_3D vtxStart, VERTEX_3D vtxEnd)
{
	m_vecLineData.push_back({ vtxStart.pt, vtxEnd.pt, vtxStart.col, vtxEnd.col, 1.f });
}

void GeometryRender::AddPoint(VERTEX_3D& vt, float _size)
{
	m_vecPointData.push_back({ vt.pt, vt.col, _size });
}

void GeometryRender::AddTriangle(VERTEX_3D& vt1, VERTEX_3D& vt2, VERTEX_3D& vt3)
{
	m_vecTrigData.push_back({ vt1.pt, vt2.pt, vt3.pt, vt1.col, vt2.col, vt3.col });
}

void GeometryRender::AddRectangle(VERTEX_3D vt1, VERTEX_3D vt2, VERTEX_3D vt3, VERTEX_3D vt4)
{
	m_vecRectData.push_back({ { vt1.pt, vt2.pt, vt3.pt, vt4.pt }, vt1.col });
}

void GeometryRender::AddRectangle2D(float x, float y, float z, float width, float height, Color3 color)
{
	Vec3D pt1 = { x			, y			, z };
	Vec3D pt2 = { x + width , y			, z };
	Vec3D pt3 = { x + width , y + height, z };
	Vec3D pt4 = { x			, y + height, z };

	m_vecRectData.push_back({ {pt1, pt2, pt3, pt4 }, color });
}

void GeometryRender::UpdateRenderData()
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

void GeometryRender::DrawLine()
{
	if (m_LineDataRender.size() <= 0)
		return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &m_LineDataRender[0]);
	glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &m_LineDataRender[3]);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_LineDataRender.size() / 6));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void GeometryRender::DrawRectangle()
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

void GeometryRender::Draw()
{
	this->DrawLine();
	this->DrawRectangle();
}

void GeometryRender::Clear()
{
	m_vecLineData.clear();
	m_vecTrigData.clear();
	m_vecPointData.clear();
}

_____END_SECTION_____
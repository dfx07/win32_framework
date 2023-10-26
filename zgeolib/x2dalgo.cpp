#include "x2dalgo.h"
#include "x2drel.h"
#include "x2dpoly.h"
#include "x2dint.h"

#include <map>
#include <set>

#pragma warning(disable : 26812)

namespace geo {	namespace v2 {

API_EXPR VecPoint2D convex_hull_giftwap(const VecPoint2D& vecPoints)
{
	if (vecPoints.size() < 3)
	{
		_ASSERT(0);
		return vecPoints;
	}

	int nPointCnt = static_cast<int>(vecPoints.size());

	// Find the leftmost point
	int nIdxLeftMost = 0;
	for (int i = 1; i < nPointCnt; i++)
	{
		if (vecPoints[i].x < vecPoints[nIdxLeftMost].x)
			nIdxLeftMost = i;
	}

	VecPoint2D vecHull; vecHull.reserve(nPointCnt);
	bool* arrMarkCheck = new bool[nPointCnt]{ false };

	int p = nIdxLeftMost, q;
	do
	{
		// Add current point to result
		vecHull.push_back(vecPoints[p]);
		arrMarkCheck[p] = true;

		// Search for a point 'q' such that orientation(p, q,
		// x) is counterclockwise for all points 'x'. The idea
		// is to keep track of last visited most counterclock-
		// wise point in q. If any point 'i' is more counterclock-
		// wise than q, then update q.
		q = (p + 1) % nPointCnt;
		for (int i = 0; i < nPointCnt; i++)
		{
			// If i is more counterclockwise than current q, then
			// update q

			if (get_orientation_point_vector(vecPoints[i], vecPoints[p], vecPoints[q]) == EnumOrien::RIGHT)
				q = i;
		}

		// Now q is the most counterclockwise with respect to p
		// Set p as q for next iteration, so that q is added to
		// result 'hull'
		p = q;

		// Close or over
		if (arrMarkCheck[p] == true || vecHull.size() > nPointCnt)
		{
			break;
		}

	} while (p != nIdxLeftMost);  // While we don't come to first point

	delete[] arrMarkCheck;

	return vecHull;
}

/***********************************************************************************
*! @brief  : Clipping Polygons use Sutherland–Hodgman algorithm [rect]
*! @param  : [in]  poly     : polygon
*! @param  : [in]  clipRect : clip (rectangle)
*! @return : VecPoint2D clip polygon
*! @author : thuong.nv         - [Date] : 08/07/2023
*! @note   : Polygon input is counterclockwise (CCW).
*! @refer  : https://en.wikipedia.org/wiki/Sutherland–Hodgman_algorithm
***********************************************************************************/
API_EXPR VecPoint2D clip_polygon_hodgman(const VecPoint2D& poly, const VecPoint2D& clipConvexPoly)
{
	if (is_convex_polygon(clipConvexPoly) == GFalse)
		return poly;

	// Allway right (ccw)
	if (clipConvexPoly.size() < 3 || poly.size() < 3)
	{
		_ASSERT(0);
		return VecPoint2D();
	}

	VecPoint2D vecOutPoints = poly;

	// If polygon is not counterclockwise will recalculate
	if (is_ccw(vecOutPoints) == GFalse) reverse_polygon(vecOutPoints);

	int nClipCnt = static_cast<int>(clipConvexPoly.size());
	int nVertexCnt = 0;

	Point2D ptInter, ptSEdge, ptEEdge;

	for (int nEdge = 0; nEdge < nClipCnt; nEdge++)
	{
		ptSEdge = clipConvexPoly[nEdge];
		ptEEdge = clipConvexPoly[(nEdge + 1) % nClipCnt];

		VecPoint2D vecInputPoints = std::move(vecOutPoints);
		nVertexCnt = static_cast<int>(vecInputPoints.size());

		vecOutPoints.clear();

		for (int nVertex = 0; nVertex < nVertexCnt; nVertex++)
		{
			Point2D ptCur = vecInputPoints[nVertex];
			Point2D ptNext = vecInputPoints[(nVertex + 1) % nVertexCnt];

			EnumOrien eRetOriCur  = get_orientation_point_vector(ptCur, ptSEdge, ptEEdge);
			EnumOrien eRetOriNext = get_orientation_point_vector(ptNext, ptSEdge, ptEEdge);

			if (eRetOriCur == EnumOrien::LEFT) // left
			{
				vecOutPoints.push_back(ptCur);

				if (eRetOriNext == EnumOrien::COLLINEAR ||
					eRetOriNext == EnumOrien::RIGHT)
				{
					if (intersect_2line(ptCur, ptNext, ptSEdge, ptEEdge, &ptInter))
					{
						vecOutPoints.push_back(ptInter);
					}
				}
			}
			else if (eRetOriNext == EnumOrien::LEFT)
			{
				if (intersect_2line(ptCur, ptNext, ptSEdge, ptEEdge, &ptInter))
				{
					vecOutPoints.push_back(ptInter);
				}
			}
		}
	}

	return vecOutPoints;
}

/***********************************************************************************
*! @brief  : Clipping Polygons use Weiler Atherton  algorithm
*! @param  : [in]  _poly     : polygon
*! @param  : [in]  _clipPoly : clip (polygon)
*! @return : VecPolyList list poly clip
*! @author : thuong.nv   - [Date] : 08/07/2023
*! @note   : Polygon input is counterclockwise (CCW).
*! @refer  : https://www.geeksforgeeks.org/weiler-atherton-polygon-clipping-algorithm/
***********************************************************************************/
VecPolyList clip_polygon_weiler(const VecPoint2D& _poly, const VecPoint2D& _clipPoly)
{
	typedef struct stMarkPoint
	{
		int		nIndex = -1;
		bool	bEnter = false;  //  true : enter | 1: leaving
		Point2D	pt;

	} MarkPoint;

	typedef std::vector<MarkPoint> VecMarkPoint;

	if (_poly.size() < 3 || _clipPoly.size() < 3)
	{
		_ASSERT(0);
		return VecPolyList();
	}

	VecPoint2D poly		= _poly;
	VecPoint2D clipPoly = _clipPoly;

	// If polygon is not counterclockwise will recalculate
	if (is_ccw(poly) == GFalse)  reverse_polygon(poly);
	if (is_ccw(clipPoly) == GFalse) reverse_polygon(clipPoly);

	int rel = rel_2polygon(poly, clipPoly);

	// Polygon inside clip polygon
	if (rel == 2)
	{
		return VecPolyList({ poly });
	}
	else if (rel == 3)
	{
		return VecPolyList({ clipPoly });
	}

	VecMarkPoint vecMarkPoly, vecMarkClipPoly;
	std::map<int, VecMarkPoint> mapInterPoly, mapInterClipPoly;
	std::set<int> setUsedIndex;

	int nPolyCnt     = static_cast<int>(poly.size());
	int nClipPolyCnt = static_cast<int>(clipPoly.size());

	auto funPushInterectionMarKPoint = [&](VecMarkPoint& vecPush, const Point2D& ptInter, const Point2D& ptOri, int nIndex) -> void
	{
		float fDisInter = mag(ptInter - ptOri);

		bool bPushBack = true;

		for (int nInter = 0; nInter < vecPush.size(); nInter++)
		{
			float fDisCur = mag(vecPush[nInter].pt - ptOri);

			if (fDisInter < fDisCur)
			{
				vecPush.insert(vecPush.begin() + nInter, MarkPoint{ nIndex, false, ptInter });

				bPushBack = false;
				break;
			}
		}

		if (bPushBack)
		{
			vecPush.push_back(MarkPoint{ nIndex, false, ptInter });
		}

		return;
	};

	Point2D ptInter;
	int nIndexInter = 1; int nNextIdx, nClipNextIdx;

	// 1. Found intersection point, and push map inter
	for (int nIdx = 0; nIdx < nPolyCnt; nIdx++)
	{
		nNextIdx = (nIdx + 1) % nPolyCnt;

		for (int nClipIdx = 0; nClipIdx < nClipPolyCnt; nClipIdx++)
		{
			nClipNextIdx = (nClipIdx + 1) % nClipPolyCnt; // next point index

			if (intersect_2lsegment(poly[nIdx], poly[nNextIdx], clipPoly[nClipIdx], clipPoly[nClipNextIdx], &ptInter) == GTrue)
			{
				// Push to map poly intersection
				auto itClip = mapInterPoly.insert(std::make_pair(nIdx, VecMarkPoint{ { nIndexInter, false, ptInter} }));

				if (itClip.second == false)
				{
					VecMarkPoint& vecInter = itClip.first->second;
					funPushInterectionMarKPoint(vecInter, ptInter, poly[nIdx], nIndexInter);
				}

				// Push map clip poly intersection
				auto it = mapInterClipPoly.insert(std::make_pair(nClipIdx, VecMarkPoint{ { nIndexInter, false, ptInter } }));

				if (it.second == false)
				{
					VecMarkPoint& vecInter = it.first->second;
					funPushInterectionMarKPoint(vecInter, ptInter, clipPoly[nClipIdx], nIndexInter);
				}

				nIndexInter++;
			}
		}
	}

	vecMarkPoly.reserve(poly.size() + static_cast<size_t>(nIndexInter) * 2);
	vecMarkClipPoly.reserve(clipPoly.size() + static_cast<size_t>(nIndexInter) * 2);

	// 2. Create two mark poly and clip poly data set enter and leaving flag

	auto funCreateMarkListPoly = [](VecMarkPoint& outvecMarkPoint,
									const VecPoint2D& _poly1,
									const VecPoint2D& _poly2,
									const std::map<int, VecMarkPoint>& mapMarkInter) -> void
	{
		MarkPoint markPoint;
		Point2D ptMid, ptTemp;

		int npoly_cnt = static_cast<int>(_poly1.size());

		for (int i = 0; i < npoly_cnt; i++)
		{
			markPoint.pt = _poly1[i];
			markPoint.nIndex = -1;
			markPoint.bEnter = false;

			outvecMarkPoint.push_back(markPoint);

			auto itFound = mapMarkInter.find(i);

			if (itFound != mapMarkInter.end())
			{
				int nInterCnt = static_cast<int>(itFound->second.size());
				const VecMarkPoint& vecInters = itFound->second;

				for (int ii = 0; ii < nInterCnt; ii++)
				{
					markPoint.pt = vecInters[ii].pt;
					markPoint.nIndex = vecInters[ii].nIndex;
					markPoint.bEnter = false;

					int nNextIdx = ii + 1;

					ptTemp = (nNextIdx < nInterCnt) ? vecInters[nNextIdx].pt : _poly1[(i + 1) % npoly_cnt];

					ptMid = mid_point(vecInters[ii].pt, ptTemp);

					if (is_point_in_polygon(ptMid, _poly2) == GTrue)
					{
						markPoint.bEnter = true;
					}

					outvecMarkPoint.push_back(markPoint);
				}
			}
		}
	};

	funCreateMarkListPoly(vecMarkPoly, poly, clipPoly, mapInterPoly);
	funCreateMarkListPoly(vecMarkClipPoly, clipPoly, poly, mapInterClipPoly);

	auto funIsMarkPointEnter = [&](const MarkPoint& mark_point) -> bool
	{
		return (mark_point.nIndex >= 0 && mark_point.bEnter);
	};

	auto funIsMarkPointLeave = [&](const MarkPoint& mark_point) -> bool
	{
		return (mark_point.nIndex >= 0 && !mark_point.bEnter);
	};

	auto funFindVertexEnd = [&](VecPoint2D& _vecResultClip, int _nSIndex, int _nEIndex) -> bool
	{
		bool bRet = false, bContinue = false;
		int nStartIdx = _nSIndex, nEndIdx = _nEIndex;
		int nCurIdx = -1;

		bool bUseMarkList = false; // true (mark) | false (mark clip)

		VecMarkPoint* pVecMarkFind;

		while (true)
		{
			bContinue = false;

			nCurIdx = -1;

			pVecMarkFind = (bUseMarkList) ? &vecMarkPoly : &vecMarkClipPoly;
			bUseMarkList = !bUseMarkList;

			int nMarkPoly = static_cast<int>(pVecMarkFind->size());

			for (int i = 0; i < nMarkPoly; i++)
			{
				if ((*pVecMarkFind)[i].nIndex == nStartIdx &&
					(*pVecMarkFind)[i].bEnter == true) //enter
				{

					nCurIdx = i;
					break;
				}
			}

			if (nCurIdx == -1)
				return false;

			setUsedIndex.insert(nStartIdx);

			for (int i = 1; i < nMarkPoly; i++)
			{
				MarkPoint& markPointClip = (*pVecMarkFind)[(nCurIdx + i) % nMarkPoly];

				// found end point -> end
				if (markPointClip.nIndex == nEndIdx)
				{
					bRet = true;
					break;
				}

				_vecResultClip.push_back(markPointClip.pt);

				// found leaving point
				if (funIsMarkPointLeave(markPointClip))
				{
					nStartIdx = markPointClip.nIndex;
					bContinue = true;
					break;
				}
			}

			if (bContinue) continue;

			return bRet;
		}

		return false;
	};

	// 3. emplement Weiler Atherton algorithm 
	VecPolyList vecClips; VecPoint2D vecClip;

	int nMarkPolyCnt = static_cast<int>(vecMarkPoly.size());

	for (int i = 0; i < nMarkPolyCnt; i++)
	{
		MarkPoint& markpt = vecMarkPoly[i];

		if (funIsMarkPointEnter(markpt) && setUsedIndex.find(markpt.nIndex) == setUsedIndex.end())
		{
			bool bClose = false;

			vecClip.push_back(markpt.pt);

			for (int nNext = 1; nNext < nMarkPolyCnt; nNext++)
			{
				MarkPoint& markNextpt = vecMarkPoly[(i + nNext) % nMarkPolyCnt];

				vecClip.push_back(markNextpt.pt);

				if (funIsMarkPointLeave(markNextpt))
				{
					bClose = funFindVertexEnd(vecClip, markNextpt.nIndex, markpt.nIndex);
					break;
				}
			}

			if (bClose)
			{
				vecClips.push_back(vecClip);
			}

			vecClip.clear();
		}
	}

	return vecClips;
}

}}
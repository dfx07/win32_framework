#include "x2dalgo.h"
#include "x2drel.h"
#include "x2dpoly.h"
#include "x2dint.h"

#include <map>
#include <set>
#include "xgeosp.h"

#pragma warning(disable : 26812)

namespace geo {	namespace v2 {

/***********************************************************************************
*! @brief  : shape in which all of its sides are pointing or protruding outwards
*! @param  : [in] vecPoints : point list
*! @author : thuong.nv			- [Date] : 25/10/2023
***********************************************************************************/
Dllexport VecPoint2D convex_hull_giftwap(const VecPoint2D& vecPoints)
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
Dllexport VecPoint2D clip_polygon_hodgman(const VecPoint2D& poly, const VecPoint2D& clipConvexPoly)
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

/***********************************************************************************
*! @brief  : Union two convex polygon (without hole)
*! @param  : [in]  poly1 : convex polygon
*! @param  : [in]  poly2 : convex polygon
*! @return : VecPolyList list poly clip
*! @author : thuong.nv   - [Date] : 13/11/2023
*! @note   : Polygon input is counterclockwise (CCW). without holes
*! @refer  : https://stackoverflow.com/questions/6844462/polygon-union-without-holes
************************************************************************************/
Dllexport VecPoint2D union_polygon(const VecPoint2D& poly1, const VecPoint2D& poly2)
{
	return VecPoint2D();
}

/***********************************************************************************
*! @brief  : Cut line and polygon
*! @param  : [in]  line : Line
*! @param  : [in]  poly : polygon
*! @return : VecPolyList list poly clip
*! @author : thuong.nv   - [Date] : 29/10/2023
*! @note   : Polygon input is counterclockwise (CCW).
***********************************************************************************/
Dllexport VecPolyList cut_line2poly(const Point2D& pt1, const Point2D& pt2, const VecPoint2D& _poly)
{
	VecPolyList vec_poly_split;
	VecPoint2D poly = _poly; // copy data

	if (_poly.size() < 3)
	{
		vec_poly_split.push_back(_poly);
		_ASSERT(0);
		return vec_poly_split;
	}

	struct MarkPointInterPoly
	{
		Point2D		pt;
		int			idx_start;
		int			idx_end;
	};

	typedef std::vector<MarkPointInterPoly> VecMarkPointInterPoly;

	GBool bExist; Point2D ptInter;
	VecMarkPointInterPoly vMarkInters;
	GFloat fDis = 0.f;

	// Polygon input is counterclockwise
	if (is_ccw(poly) == GFalse) reverse_polygon(poly);

	// seek all intersection point and index polygon between line and polygon
	int nPolyCnt = static_cast<int>(poly.size());

	for (int j = 0, i = nPolyCnt - 1; j < nPolyCnt; i = j++)
	{
		bExist = GFalse;
		if (intersect_2lsegment(poly[i], poly[j], pt1, pt2, &ptInter) == GTrue)
		{
			for (int ii = 0; ii < vMarkInters.size(); ii++)
			{
				fDis = mag(ptInter - vMarkInters[ii].pt);

				if (fDis < MATH_EPSILON)
				{
					bExist = GTrue;
					break;
				}
			}

			if (bExist == GFalse)
			{
				vMarkInters.push_back({ ptInter, i,j });
			}
		}
	}

	bool* arMark = new bool[nPolyCnt];
	std::memset(arMark, 0, sizeof(bool)*nPolyCnt);

	// check existed intersection point in line segment of polygon
	auto funExistIntersect = [&](int& idx, int& idx_next)
	{
		for (int in = 0; in < vMarkInters.size(); in++)
		{
			if (idx == vMarkInters[in].idx_start && idx_next == vMarkInters[in].idx_end)
			{
				return in;
			}
		}

		return -1;
	};

	// seek intersection point and near intersection point index
	// located on the left side of intersection point mark
	auto funGetIndexIntersectNearest = [&](int idxInter)
	{
		GFloat fDisMin = -1.f, fDis = 0.f;
		int idxInterMin = -1;
		EnumOrien ori_min;

		if (idxInter < 0 || idxInter >= vMarkInters.size())
			return -1;

		auto& markInter = vMarkInters[idxInter];

		for (int in = 0; in < vMarkInters.size(); in++)
		{
			fDis = mag(vMarkInters[in].pt - vMarkInters[idxInter].pt);
			ori_min = get_orientation_point_vector(poly[markInter.idx_start], poly[markInter.idx_end], vMarkInters[in].pt);

			if (in != idxInter && (ori_min == EnumOrien::LEFT && (fDisMin < 0 || fDis <= fDisMin)))
			{
				idxInterMin = in;
				fDisMin = fDis;
			}
		}

		return idxInterMin;
	};

	EnumOrien or_pfirst = EnumOrien::LEFT;
	int nIdx, nIdxNext, i, x, y;

	// loop to seek all polygon after cutting
	for (i = 0; i < nPolyCnt; i++)
	{
		if (arMark[i] == true) continue;

		VecPoint2D poly_split;
		int idx = i, count = 0;

		while (count < nPolyCnt) //PS: Prevents infinite looping
		{
			x = idx; y = (idx + 1) % nPolyCnt;

			poly_split.push_back(poly[x]);
			arMark[x] = true;
			idx = y;

			if ((nIdx = funExistIntersect(x, y)) >= 0)
			{
				poly_split.push_back(vMarkInters[nIdx].pt);

				if ((nIdxNext = funGetIndexIntersectNearest(nIdx)) >= 0)
				{
					poly_split.push_back(vMarkInters[nIdxNext].pt);

					int st = vMarkInters[nIdxNext].idx_start;
					int ed = vMarkInters[nIdxNext].idx_end;

					if (get_orientation_point_vector(vMarkInters[nIdx].pt, vMarkInters[nIdxNext].pt, poly[ed]) == or_pfirst)
					{
						idx = ed;
					}
					else if (get_orientation_point_vector(vMarkInters[nIdx].pt, vMarkInters[nIdxNext].pt, poly[st]) == or_pfirst)
					{
						idx = st;
					}
				}
			}

			count++;

			if (i == idx) break;
		}
		vec_poly_split.push_back(poly_split);
	}

	delete[] arMark;

	//for (INT i = 0; i < vec_poly_split.size(); i++)
	//{
	//	V2remove_double_point(vec_poly_split[i]);
	//}

	return vec_poly_split;
}

/***********************************************************************************
*! @brief  : is the partition of a polygonal area (simple polygon) P into a set of triangles
*! @param  : [in]  poly : polygon
*! @return : VecPolyList (Every 3 consecutive points will be a triangle)
*! @author : thuong.nv   - [Date] : 11/07/2023
*! @note   : Polygon input is counterclockwise (CCW winding).
*!           For convex or concave polygon
************************************************************************************/
Dllexport VecPoint2D poly2trig_ear_clipping(const VecPoint2D& poly)
{
	VecPoint2D list_trig; VecPoint2D vecPolyTemp;

	int nPolyCnt = static_cast<int>(poly.size());
	if (nPolyCnt < 3)
	{
		//_ASSERT(0);

		return poly;
	}

	list_trig.reserve(nPolyCnt * 2);

	vecPolyTemp = poly;

	EnumOrien orien, orp12, orp23, orp31;
	auto funCheckPointInTrigIdx = [&](VecPoint2D& _poly, int idx1, int idx2, int idx3)
	{
		int _nPolyCnt = static_cast<int>(_poly.size());

		for (int i = 0; i < _nPolyCnt; i++)
		{
			// don't check index input
			if (i == idx1 || i == idx2 || i == idx3)
				continue;

			orp12 = get_orientation_point_vector(_poly[idx1], _poly[idx2], _poly[i]);
			orp23 = get_orientation_point_vector(_poly[idx2], _poly[idx3], _poly[i]);
			orp31 = get_orientation_point_vector(_poly[idx3], _poly[idx1], _poly[i]);

			if (orp12 == orp23 && orp23 == orp31)
				return true;
		}

		return false;
	};

	int i, nOldSize, nCur, nNex, nPre;

	while (vecPolyTemp.size() > 0)
	{
		//PS: prevent in case of infinite loops 
		nPolyCnt = nOldSize = static_cast<int>(vecPolyTemp.size());

		for (i = 0; i < nPolyCnt; i++)
		{
			nCur = i; nPre = ((i - 1) < 0) ? (nPolyCnt - 1) : (i - 1);
			nNex = ((i + 1) >= nPolyCnt) ? 0 : (i + 1);

			orien = get_orientation_point_vector(vecPolyTemp[nPre], vecPolyTemp[nCur], vecPolyTemp[nNex]);

			if ((orien == EnumOrien::LEFT || orien == EnumOrien::COLLINEAR)
				&& funCheckPointInTrigIdx(vecPolyTemp, nPre, nCur, nNex) == false)
			{
				list_trig.push_back(vecPolyTemp[nPre]);
				list_trig.push_back(vecPolyTemp[nCur]);
				list_trig.push_back(vecPolyTemp[nNex]);
				vecPolyTemp.erase(vecPolyTemp.begin() + nCur);
			}

			nPolyCnt = static_cast<int>(vecPolyTemp.size());
		}

		if (vecPolyTemp.size() == nOldSize)
			break;
	}
	return list_trig;
}

}}
#include "Stdafx.h"
#include "BorderDrag.h"

#define IS_LKEYDOWN ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0)  //检测鼠标按下 弹起状态

CBorderdrag::CBorderdrag()
{
	m_bDargNormal = FALSE;
	m_bClick = FALSE;
}
CBorderdrag::~CBorderdrag()
{

}
LRESULT CBorderdrag::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&bHandled)
{
	switch (wParam)
	{
	case DORDERDRAGTIMERID:
		OnTimer();
		break;
	default:
		break;
	}
	return S_OK;
}
void CBorderdrag::OnTimer()
{
	if (!m_bDargNormal)
	{
		OnBorderMouseMoveNormal();
	}
	else
	{
		OnBorderMouseClick();
	}
	if (m_bDargNormal && m_bClick)
	{
		OnBorderMouseMoveClick();
		OnBorderMouseDragOver();
	}
}
	//鼠标松开状态在边框时移动
void CBorderdrag::OnBorderMouseMoveNormal()
{
	//如果是按压状态，直接返回
	if (IS_LKEYDOWN
		|| ::IsZoomed(m_hwnd)
		|| !::IsWindowVisible(m_hwnd))
	{
		return;
	}
	GetWndVisibleRect(m_mapST2Point);
	//获取窗口大小
	RECT wRect;
	::GetWindowRect(m_hwnd, &wRect);

	BOOL bLeft = FALSE, bTop = FALSE, bRight = FALSE, bBottom = FALSE;
	
	CheckCursorPos(m_mapST2Point, bLeft, bRight, bTop, bBottom);
	CURSORINFO pci = {0};
	pci.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&pci);
	UINT nHitTest = pci.flags;
	nHitTest = bLeft ? HTLEFT : nHitTest;
	nHitTest = bRight ? HTRIGHT : nHitTest;
	nHitTest = bTop ? HTTOP : nHitTest;
	nHitTest = bBottom ? HTBOTTOM : nHitTest;
	nHitTest = bLeft && bTop ? HTTOPLEFT : nHitTest;
	nHitTest = bLeft && bBottom ? HTBOTTOMLEFT : nHitTest;
	nHitTest = bRight && bTop ? HTTOPRIGHT : nHitTest;
	nHitTest = bRight && bBottom ? HTBOTTOMRIGHT : nHitTest;
	m_bIsBorder = FALSE;
	m_Cursor = HTNOWHERE;
	switch (nHitTest)
	{
	case HTLEFT:          SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTRIGHT:		  SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTTOP:           SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTBOTTOM:        SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTTOPLEFT:       SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTTOPRIGHT:      SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTBOTTOMLEFT:    SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	case HTBOTTOMRIGHT:   SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); m_bIsBorder = TRUE; m_Cursor = nHitTest; break;
	default:break;
	}
	m_bDargNormal = (bBottom || bRight || bTop || bLeft) ? TRUE : FALSE;
}
	//鼠标在边框中按压状态
void CBorderdrag::OnBorderMouseClick()
{
	if (m_bClick)
	{
		return;
	}
	RECT wRect;
	::GetWindowRect(m_hwnd, &wRect);
	POINT pt;
	::GetCursorPos(&pt);
	BOOL bLeft = FALSE, bTop = FALSE, bRight = FALSE, bBottom = FALSE;

	CheckCursorPos(m_mapST2Point, bLeft, bRight, bTop, bBottom);

	//是否是在边框区域上
	BOOL bInBorder = (bBottom || bRight || bTop || bLeft) ? TRUE : FALSE;
	if (!bInBorder)
	{
		m_bDrag = FALSE;
		m_bDargNormal = FALSE;
		return;
	}
	if (IS_LKEYDOWN)
	{
		m_bClick = TRUE;
	}

	switch (m_Cursor)
	{
	case HTLEFT:          SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));break;
	case HTRIGHT:		  SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); break;
	case HTTOP:           SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); break;
	case HTBOTTOM:        SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); break;
	case HTTOPLEFT:       SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); break;
	case HTTOPRIGHT:      SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); break;
	case HTBOTTOMLEFT:    SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); break;
	case HTBOTTOMRIGHT:   SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); break;
	default:break;
	}
}
	//鼠标在拖动边框
void CBorderdrag::OnBorderMouseMoveClick()
{
	if (!IS_LKEYDOWN)
	{
		m_bClick = FALSE;
		m_bDrag = FALSE;
		return;
	}
// 	RECT wRect;
// 	::GetWindowRect(m_hwnd, &wRect);
// 	POINT pt;
// 	::GetCursorPos(&pt);
// 	BOOL bLeft = FALSE, bTop = FALSE, bRight = FALSE, bBottom = FALSE;
// 	RECT rtLeft, rtTop, rtRight, rtBottom;
// 	rtLeft = rtTop = rtRight = rtBottom = wRect;
// 
// 	rtLeft.right = wRect.left + m_BorderSize;
// 	rtTop.bottom = wRect.top + m_BorderSize;
// 	rtRight.left = wRect.right - m_BorderSize;
// 	rtBottom.top = wRect.bottom - m_BorderSize;
// 	if (PtInRect(&rtLeft, pt))bLeft = TRUE;
// 	if (PtInRect(&rtTop, pt))bTop = TRUE;
// 	if (PtInRect(&rtRight, pt))bRight = TRUE;
// 	if (PtInRect(&rtBottom, pt))bBottom = TRUE;
// 
// 	CURSORINFO pci;
// 	GetCursorInfo(&pci);
// 	UINT nHitTest = pci.flags;
// 	nHitTest = bLeft ? HTLEFT : nHitTest;
// 	nHitTest = bLeft && bTop ? HTTOPLEFT : nHitTest;
// 	nHitTest = bLeft && bBottom ? HTBOTTOMLEFT : nHitTest;
// 	nHitTest = bRight ? HTRIGHT : nHitTest;
// 	nHitTest = bRight && bTop ? HTTOPRIGHT : nHitTest;
// 	nHitTest = bRight && bBottom ? HTBOTTOMRIGHT : nHitTest;
// 	nHitTest = bTop ? HTTOP : nHitTest;
// 	nHitTest = bBottom ? HTBOTTOM : nHitTest;
	m_bDrag = FALSE;
	POINT point;
	::GetCursorPos(&point);
	switch (m_Cursor)
	{
	case HTLEFT:		  ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y)); m_bDrag = TRUE; break;
	case HTRIGHT:         ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	case HTTOP:           ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	case HTBOTTOM:        ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	case HTTOPLEFT:       ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	case HTTOPRIGHT:      ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	case HTBOTTOMLEFT:    ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	case HTBOTTOMRIGHT:   ::PostMessage(m_hwnd, /*WM_NCHITTEST*/WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));  m_bDrag = TRUE; break;
	default: break;
	}

}
	//鼠标在边框中按压拖动结束
void CBorderdrag::OnBorderMouseDragOver()
{
	//获取窗口大小
// 	RECT wRect;
// 	::GetWindowRect(m_hwnd, &wRect);
// 	POINT pt;
// 	::GetCursorPos(&pt);
// 	BOOL bLeft = FALSE, bTop = FALSE, bRight = FALSE, bBottom = FALSE;
// 	RECT rtLeft, rtTop, rtRight, rtBottom;
// 	rtLeft = rtTop = rtRight = rtBottom = wRect;
// 
// 	rtLeft.right = wRect.left + m_BorderSize;
// 	rtTop.bottom = wRect.top + m_BorderSize;
// 	rtRight.left = wRect.right - m_BorderSize;
// 	rtBottom.top = wRect.bottom - m_BorderSize;
// 	if (PtInRect(&rtLeft, pt))bLeft = TRUE;
// 	if (PtInRect(&rtTop, pt))bTop = TRUE;
// 	if (PtInRect(&rtRight, pt))bRight = TRUE;
// 	if (PtInRect(&rtBottom, pt))bBottom = TRUE;
// 
// 	//是否是在边框区域上
// 	BOOL bInBorder = (bBottom || bRight || bTop || bLeft) ? TRUE : FALSE;

	if (!IS_LKEYDOWN /*&& !bInBorder*/)
	{
		m_bClick = FALSE;
		m_bDargNormal = FALSE;
		m_bDrag = FALSE;
	}
	return;
}

void CBorderdrag::GetWndVisibleRect(MAPTYPELINE &mapST2Point)
{
	m_mapST2Point.clear();
	vector<st_2Point> vecLeft, vecRight, vecTop, vecBottom;//用来存储其他窗口与m_hwnd窗口的相交的点
	st_2Point leftPt;
	RECT wRect, tmpRect;
	::GetWindowRect(m_hwnd, &wRect);
	tmpRect = wRect;
	tmpRect.left += m_BorderSize;
	tmpRect.right -= m_BorderSize;
	tmpRect.top += m_BorderSize;
	tmpRect.bottom -= m_BorderSize;
	HWND hWnd = m_hwnd;
// 	if (::GetDesktopWindow() == hWnd)
// 		hWnd = ::GetWindow(::GetTopWindow(hWnd), GW_HWNDLAST);
	HWND hCurWnd = hWnd;
	while (NULL != (hWnd = ::GetNextWindow(hWnd, GW_HWNDPREV)))
	{
		TCHAR chName[128] = { 0 };
		::GetClassName(hWnd, chName, 127);
		if (hWnd == m_hwnd
			|| 0 == CString(chName).Compare(_T("PerryShadowWnd")))//注意：ec主窗口的阴影窗口要过滤掉
			continue;
		if (::IsWindowVisible(hWnd))
		{
			RECT rcWnd;
			::GetWindowRect(hWnd, &rcWnd);

			if (!((rcWnd.right < wRect.left) || (rcWnd.left > wRect.right) ||
				(rcWnd.bottom < wRect.top) || (rcWnd.top > wRect.bottom)))
			{
				//说明是有覆盖的
				//1.目标窗口是否被其他窗口完全覆盖，直接返回
				if ((wRect.left >= rcWnd.left)
					&& (wRect.right <= rcWnd.right)
					&& (wRect.top >= rcWnd.top)
					&& (wRect.bottom <= rcWnd.bottom))
				{
					return;
				}
				//2.其他窗口是否被包含是包含关系
				else if ((rcWnd.left >= tmpRect.left)
					&& (rcWnd.right <= tmpRect.right)
					&& (rcWnd.top <= tmpRect.top)
					&& (rcWnd.bottom >= tmpRect.bottom))
				{
					continue;
				}
				//3.有相交的情况
				else
				{	//1.其他窗口的Left要小于m_hWnd的Left，这个时候m_hWnd的窗口Left有分割点
					if (rcWnd.left <= wRect.left)
					{
						st_2Point leftPt;
						leftPt.SetPoint(wRect.left, rcWnd.top, wRect.left, rcWnd.bottom);
						vecLeft.push_back(leftPt);
					}
					//2.其他窗口的Right要大于m_hWnd的Right，这个时候m_hWnd的窗口Right有分割点
					if (rcWnd.right >= wRect.right)
					{
						st_2Point rightPt;
						rightPt.SetPoint(wRect.right, rcWnd.top, wRect.right, rcWnd.bottom);
						vecRight.push_back(rightPt);
					}
					//3.其他窗口的Top要小于m_hWnd的Top，这个时候m_hWnd的窗口Top有分割点
					if (rcWnd.top <= wRect.top)
					{
						st_2Point topPt;
						topPt.SetPoint(rcWnd.left, wRect.top, rcWnd.right, wRect.top);
						vecTop.push_back(topPt);
					}
					//4.其他窗口的Bottom要大于m_hWnd的Bottom，这个时候m_hWnd的窗口Bottom有分割点
					if (rcWnd.bottom >= wRect.bottom)
					{
						st_2Point bottomPt;
						bottomPt.SetPoint(rcWnd.left, wRect.bottom, rcWnd.right, wRect.bottom);
						vecBottom.push_back(bottomPt);
					}
				}
			}
		}
	}

	st_2Point ltPt, rtPt, tpPt, bmPt;
	ltPt.SetPoint(wRect.left, wRect.top, wRect.left, wRect.bottom);
	rtPt.SetPoint(wRect.right, wRect.top, wRect.right, wRect.bottom);
	tpPt.SetPoint(wRect.left, wRect.top, wRect.right, wRect.top);
	bmPt.SetPoint(wRect.left, wRect.bottom, wRect.right, wRect.bottom);

	mapST2Point[ST2POINT_LEFT].push_back(ltPt);
	mapST2Point[ST2POINT_RIGHT].push_back(rtPt);
	mapST2Point[ST2POINT_TOP].push_back(tpPt);
	mapST2Point[ST2POINT_BOTTOM].push_back(bmPt);
	GetCutLine(mapST2Point[ST2POINT_LEFT], vecLeft);
	GetCutLine(mapST2Point[ST2POINT_RIGHT], vecRight);
	GetCutLine(mapST2Point[ST2POINT_TOP], vecTop);
	GetCutLine(mapST2Point[ST2POINT_BOTTOM], vecBottom);

}

void CBorderdrag::GetCutLine(list<st_2Point> &listLine, vector<st_2Point> &vecLine)
{
	INT32 nSize = vecLine.size();
	for (INT32 nIndex = 0; nIndex < nSize; nIndex++)
	{
		list<st_2Point>::iterator itr = listLine.begin();
		while (itr != listLine.end())//在比较分割点的时候，x轴上st_2Point点的方向是从左向右；y轴上st_2Point点的方向是从上向下；
		{
			st_2Point tp_2Pt1, tp_2Pt2;
			if ((vecLine[nIndex].pt1.x > itr->pt1.x)
				&& (vecLine[nIndex].pt1.x < itr->pt2.x))//x轴pt1点在内部；
			{
				tp_2Pt1.pt1 = itr->pt1;
				tp_2Pt1.pt2 = vecLine[nIndex].pt1;
				listLine.push_back(tp_2Pt1);
				if (vecLine[nIndex].pt2.x < itr->pt2.x)//x轴Pt2在内部
				{
					tp_2Pt2.pt1 = vecLine[nIndex].pt2;
					tp_2Pt2.pt2 = itr->pt2;
					listLine.push_back(tp_2Pt2);
				}
				listLine.erase(itr);
				break;
			}
			else if ((vecLine[nIndex].pt1.x < itr->pt1.x)//x轴Pt1在线外
				&& vecLine[nIndex].pt2.x < itr->pt2.x)
			{
				tp_2Pt1.pt1 = vecLine[nIndex].pt2;
				tp_2Pt1.pt2 = itr->pt2;
				listLine.push_back(tp_2Pt1);
				listLine.erase(itr);
				break;
			}
			else if (vecLine[nIndex].pt1.y > itr->pt1.y
				&& vecLine[nIndex].pt1.y < itr->pt2.y)//y轴:Pt1在线内
			{
				tp_2Pt1.pt1 = itr->pt1;
				tp_2Pt1.pt2 = vecLine[nIndex].pt1;
				listLine.push_back(tp_2Pt1);
				if (vecLine[nIndex].pt2.y < itr->pt2.y)//y轴：Pt2在线内
				{
					tp_2Pt2.pt1 = vecLine[nIndex].pt2;
					tp_2Pt2.pt2 = itr->pt2;
					listLine.push_back(tp_2Pt2);
				}
				listLine.erase(itr);
				break;
			}
			else if (vecLine[nIndex].pt1.y < itr->pt1.y//y轴：Pt1在线外
				&& vecLine[nIndex].pt2.y < itr->pt2.y)
			{
				tp_2Pt1.pt1 = vecLine[nIndex].pt2;
				tp_2Pt1.pt2 = itr->pt2;
				listLine.push_back(tp_2Pt1);
				listLine.erase(itr);
				break;
			}
			itr++;
		}
	}
}

void CBorderdrag::CheckCursorPos(MAPTYPELINE &mapST2Point, BOOL &bLeft, BOOL &bRight, BOOL &bTop, BOOL &bBottom)
{
	POINT pt;
	::GetCursorPos(&pt);
	MAPTYPELINE::iterator itor = mapST2Point.begin();
	while (itor != mapST2Point.end())
	{
		if (itor->first == ST2POINT_LEFT)
		{
			list<st_2Point>::iterator list_itr = itor->second.begin();
			while (list_itr != itor->second.end())
			{
				RECT rtLeft;
				rtLeft.left = list_itr->pt1.x;
				rtLeft.right = list_itr->pt1.x + m_BorderSize;
				rtLeft.top = list_itr->pt1.y;
				rtLeft.bottom = list_itr->pt2.y;
				if (PtInRect(&rtLeft, pt))
				{
					bLeft = TRUE;
					break;
				}
				list_itr++;
			}
		}
		else if (itor->first == ST2POINT_RIGHT)
		{
			list<st_2Point>::iterator list_itr = itor->second.begin();
			while (list_itr != itor->second.end())
			{
				RECT rtRight;
				rtRight.left = list_itr->pt1.x - m_BorderSize;
				rtRight.right = list_itr->pt1.x;
				rtRight.top = list_itr->pt1.y;
				rtRight.bottom = list_itr->pt2.y;
				if (PtInRect(&rtRight, pt))
				{
					bRight = TRUE;
					break;
				}
				list_itr++;
			}
		}
		else if (itor->first == ST2POINT_TOP)
		{
			list<st_2Point>::iterator list_itr = itor->second.begin();
			while (list_itr != itor->second.end())
			{
				RECT rtTop;
				rtTop.left = list_itr->pt1.x;
				rtTop.right = list_itr->pt2.x;
				rtTop.top = list_itr->pt1.y;
				rtTop.bottom = list_itr->pt1.y + m_BorderSize;
				if (PtInRect(&rtTop, pt))
				{
					bTop = TRUE;
					break;
				}
				list_itr++;
			}
		}
		else if (itor->first == ST2POINT_BOTTOM)
		{
			list<st_2Point>::iterator list_itr = itor->second.begin();
			while (list_itr != itor->second.end())
			{
				RECT rtBottom;
				rtBottom.left = list_itr->pt1.x;
				rtBottom.right = list_itr->pt2.x;
				rtBottom.top = list_itr->pt1.y - m_BorderSize;
				rtBottom.bottom = list_itr->pt1.y;
				if (PtInRect(&rtBottom, pt))
				{
					bBottom = TRUE;
					break;
				}
				list_itr++;
			}
		}
		itor++;
	}
}
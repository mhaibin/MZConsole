/*
说明：处理鼠标处于边框位置的时候，拖动窗口；
作者：manhaibin
时间：2017-10-9
*/
#ifndef __IBORDERDRAG_H__
#define __IBORDERDRAG_H__
#include <atlstr.h>
#include <vector>
#include <list>
#include <map>

#define BORDERSIZE 4
#define	DORDERDRAGTIMERID				(1000)

using namespace std;

enum enumST2PointType
{
	ST2POINT_NONE = 0,
	ST2POINT_LEFT = 1,
	ST2POINT_RIGHT = 2,
	ST2POINT_TOP = 3,
	ST2POINT_BOTTOM = 4
};
struct st_2Point
{
	st_2Point()
	{
		pt1.x = pt1.y = pt2.x = pt2.y = 0;
	}
	st_2Point(LONG pt1_x, LONG pt1_y, LONG pt2_x, LONG pt2_y)
	{
		pt1.x = pt1_x; pt1.y = pt1_y; pt2.x = pt2_x; pt2.y = pt2_y;
	}
	inline void SetPoint(LONG pt1_x, LONG pt1_y, LONG pt2_x, LONG pt2_y)
	{
		pt1.x = pt1_x; pt1.y = pt1_y;pt2.x = pt2_x; pt2.y = pt2_y;
	}
	POINT pt1;
	POINT pt2;
};

typedef map<enumST2PointType, list<st_2Point>> MAPTYPELINE;

class BorderDragBase : public CWindowImpl <BorderDragBase, CWindow, CWinTraits<WS_OVERLAPPEDWINDOW, 0> >
{
	BEGIN_MSG_MAP(BorderDragBase)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()
public:

	BorderDragBase(){
		m_hWnd = Create(NULL);
		ATLASSERT(m_hWnd);

		SetTimer(DORDERDRAGTIMERID, 100, NULL);

		m_BorderSize = BORDERSIZE;
		m_bIsBorder = FALSE;
		m_bDrag = FALSE;
		m_Cursor = HTNOWHERE;
	}
	virtual ~BorderDragBase(){
		KillTimer(DORDERDRAGTIMERID);
		if (NULL != m_hWnd)
		{
			DestroyWindow();
			m_hWnd = NULL;
		}
	}
	DECLARE_WND_CLASS(_T("BorderDragBase"))
public:
	void SetBorderSize(UINT32 uSize){ m_BorderSize = uSize; }
	void SetHWnd(HWND hwnd){ m_hwnd = hwnd; }
	BOOL IsBorderRect(){return m_bIsBorder; }
	BOOL IsDrag(){return m_bDrag;}
	UINT GetCursor(){ return m_Cursor; }
protected:
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&bHandled){return S_OK;}

	//定时器
	virtual void OnTimer(){};
	//鼠标松开状态在边框时移动
	virtual void OnBorderMouseMoveNormal() = 0;
	//鼠标在边框中按压状态
	virtual void OnBorderMouseClick() = 0;
	//鼠标在拖动边框
	virtual void OnBorderMouseMoveClick() = 0;
	//鼠标在边框中按压拖动结束
	virtual void OnBorderMouseDragOver() = 0;

protected:
	HWND m_hwnd;
	UINT32 m_BorderSize;
	BOOL m_bIsBorder;
	UINT m_Cursor;
	BOOL m_bDrag;
private:

};

class CBorderdrag : public BorderDragBase
{
public:
	CBorderdrag();
	~CBorderdrag();
protected:

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&bHandled);

	void OnTimer();
	//鼠标松开状态在边框时移动
	void OnBorderMouseMoveNormal();
	//鼠标在边框中按压状态
	void OnBorderMouseClick();
	//鼠标在拖动边框
	void OnBorderMouseMoveClick();
	//鼠标在边框中按压拖动结束
	void OnBorderMouseDragOver();

private:
	//获取窗口可见范围
	void GetWndVisibleRect(MAPTYPELINE &mapST2Point);
	//对线进行整理
	void GetCutLine(list<st_2Point> &listLine, vector<st_2Point> &vecLine);
	//判断光标是否在边框上
	void CheckCursorPos(MAPTYPELINE &mapST2Point, BOOL &bLeft, BOOL &bRight, BOOL &bTop, BOOL &bBottom);
private:
	map<enumST2PointType, list<st_2Point>> m_mapST2Point;
	BOOL m_bDargNormal;
	BOOL m_bClick;
};

#endif
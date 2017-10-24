#include "stdafx.h"
//#include "..\Main\MZIniLib.h"
#include <atlwin.h>
#include "..\Main\resource.h"
#include "MainFrame.h"
#include "MZConsoleDefine.h"
#include <SharedUI/Frame/TopMenuUI.h>
#include <SharedUI/Frame/CenterViewUI.h>
#include <SkinInner/SkinAPI.h>
#include "..\SharedUI\Frame\CDuiMenuISO.h"
#include "Singleton.h"
#include "AuxMainUI.h"
#include "..\Server\mzdio\MzdIOMgr.h"
#include "UtilPath.h"
#include "UtilString.h"
#include <time.h>

CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
	Singleton<CBorderdrag>::UnInstance();
}

CDuiString CMainFrame::GetSkinFolder()
{
	return _T("Resource");
}
CDuiString CMainFrame::GetSkinFile()
{
	return _T("xml\\MainFrame.xml");
}

LPCTSTR CMainFrame::GetWindowClassName(void) const
{
	return _T("MainFrameClass");
}
UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("TopMenu")) == 0)
	{
		return new CTopMenuUI();
	}
	else if (_tcscmp(pstrClass, _T("CenterView")) == 0)
	{
		return new CCenterViewUI();
	}
	return NULL;
}
void CMainFrame::InternationalString(LPCTSTR pstrCtlName, CDuiString &strText, CControlUI *pControl)
{
//	MZDUI_GET_GLOBAL((char *)NULL, "SuperWksNum");
}
void CMainFrame::Notify(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == sCtrlName.Compare(_T("closebtn")))
		{
			ShowWindow(false);
		}
		else if(0 == sCtrlName.Compare(_T("minbtn")))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if(0 == sCtrlName.Compare(_T("restorebtn")))
		{
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else if(0 == sCtrlName.Compare(_T("maxbtn")))
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
}

void CMainFrame::InitWindow()
{
	HICON hIcon = LoadIcon(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDI_ICON_MZ));
	SetIcon(hIcon, FALSE);

	InitWndRect();

	CString strTray = _T("名智客户端");
	map<EC_TRAY_ICON, HICON>  mapIcon;
	mapIcon[UNKNOWN_ICON] = hIcon;
	mapIcon[DEFAULT_NORMAL_ICON] = hIcon;
	Singleton<CTrayMsgboxFrame>::Instance().Init(strTray, this, mapIcon);

	//注册信息
	if(TRUE == Singleton<CMzdIOMgr>::Instance().IsRegster())
	{
		RegsterInfo info;
		Singleton<CMzdIOMgr>::Instance().GetRegsterInfo(info);
		CLabelUI *pLabNetBarName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_netbarname")));
		CLabelUI *pLabDiskName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_regdisknum")));
		CLabelUI *pLabRegSize = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_regsize")));
		CLabelUI *pLabValidity = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_regvalidity")));
		CLabelUI *pLabPass = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_regpass")));
		CString strText;
		strText.Format(_T("网吧名称: {c #A2B5CD}%s{/c}"), info.strNetBar);
		pLabNetBarName->SetText(strText);

		strText.Format(_T("硬盘序列号: {c #A2B5CD}%s{/c}"), info.strDiskNum);
		pLabDiskName->SetText(strText);

		strText.Format(_T("注册台数: {c #A2B5CD}%u{/c}"), info.u32Size);
		pLabRegSize->SetText(strText);

		strText.Format(_T("到期时间: {c #A2B5CD}%s{/c}"), info.strValidity);
		pLabValidity->SetText(strText);

		strText.Format(_T("通行证: {c #A2B5CD}%s{/c}"), info.strPass);
		pLabPass->SetText(strText);
	}
	else
	{
		//post一个消息，弹出注册窗口
		::PostMessage(m_hWnd, WM_REGISTERWND_MSG, NULL, NULL);
	}
	Singleton<CBorderdrag>::Instance().SetHWnd(m_hWnd);
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT hr = 0;
	switch(uMsg)
	{
	case UW_SHOWWINDOW_MSG:
		ShowWindow((TRUE == wParam) ? true : false);
		break;
	case WM_TIMER:
		OnTimer(uMsg, wParam, lParam, bHandled);
		break;
	case WM_REGISTERWND_MSG:
		Popup::OpenRegisterWnd();
		break;
	case UM_ISINBORDER_MSG:
		hr = (LRESULT)Singleton<CBorderdrag>::Instance().IsBorderRect();
		break;
	case WM_DISPLAYCHANGE:
		InitWndRect();
		break;
	default:
		break;
	}
	if (bHandled) return hr;
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((wParam & SC_SIZE) == SC_SIZE)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		UpdateWindowSizeByBorderDrag((UINT32)(wParam ^ SC_SIZE), pt);
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hTrayWin = Singleton<CTrayMsgboxFrame>::Instance().GetHWND();
	if (IsWindow(hTrayWin))
	{
		::DestroyWindow(hTrayWin);
	}
	Singleton<CTrayMsgboxFrame>::UnInstance();
	event::ExitProcessEvn().run();
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}
LRESULT CMainFrame::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CMainFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (Singleton<CBorderdrag>::Instance().IsBorderRect())
	{
		bHandled = FALSE;
		return S_OK;
	}
	return S_OK;
}
LRESULT CMainFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	if (Singleton<CBorderdrag>::Instance().IsDrag())
	if (Singleton<CBorderdrag>::Instance().IsBorderRect()
		|| Singleton<CBorderdrag>::Instance().IsDrag())
	{
		return Singleton<CBorderdrag>::Instance().GetCursor();
	}

	return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}
LRESULT CMainFrame::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (Singleton<CBorderdrag>::Instance().IsBorderRect())
	{
		bHandled = TRUE;
	}
	return S_OK;
}
//设置窗口主窗口
void CMainFrame::ShowMain()
{
	if (::IsIconic(m_hWnd)){
		::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		return;
	}
	if (!::IsWindowVisible(m_hWnd)){
		::ShowWindow(m_hWnd, SW_SHOW);
	}
	::SetForegroundWindow(m_hWnd);
}
//托盘左键单击事件
void  CMainFrame::LButtonDownEvent()
{
	ShowMain();
}

//托盘右键单击事件
void  CMainFrame::RButtonDownEvent()
{
	POINT point;
	CDuiMenu menu;
	::GetCursorPos(&point);
	menu.LoadFromXml(_T("xml//TrayRCMenu.xml"));
	CDuiString strCmd = menu.TrackPopupMenu(m_hWnd, point.x, point.y);	
	if (strCmd == _T("traymenu_displaymainwnd")) //显示主窗口
	{
		ShowMain();
	}
	else if (strCmd == _T("traymenu_reg")) //名智注册验证
	{

	}
	else if (strCmd == _T("traymenu_opendir")) //打开所有目录
	{
		OpenCurDir();
	}
	else if (strCmd == _T("traymenu_exit")) //退出
	{
		Close(IDCANCEL);
		PostMessage(WM_QUIT, 0, 0);
	}
}
void CMainFrame::UpdateWindowSizeByBorderDrag(UINT32 uflag, POINT &pt)
{
	SIZE minInfo = m_PaintManager.GetMinInfo();
	RECT rt, curRt;
	GetWindowRect(m_hWnd, &rt);
	curRt = rt;
	switch (uflag)
	{
	case WMSZ_LEFT:
		rt.left = (rt.right - pt.x) < minInfo.cx ? (rt.right - minInfo.cx) : pt.x;
		break;
	case WMSZ_RIGHT:
		rt.right = (pt.x - rt.left) < minInfo.cx ? (rt.left + minInfo.cx) : pt.x;
		break;
	case WMSZ_TOP:
		rt.top = (rt.bottom - pt.y) < minInfo.cy ? (rt.bottom - minInfo.cy) : pt.y;
		break;
	case WMSZ_BOTTOM:
		rt.bottom = (pt.y - rt.top) < minInfo.cy ? (rt.top + minInfo.cy) : pt.y;
		break;
	case WMSZ_TOPLEFT:
		rt.top = (rt.bottom - pt.y) < minInfo.cy ? (rt.bottom - minInfo.cy) : pt.y;
		rt.left = (rt.right - pt.x) < minInfo.cx ? (rt.right - minInfo.cx) : pt.x;
		break;
	case WMSZ_TOPRIGHT:
		rt.top = (rt.bottom - pt.y) < minInfo.cy ? (rt.bottom - minInfo.cy) : pt.y;
		rt.right = (pt.x - rt.left) < minInfo.cx ? (rt.left + minInfo.cx) : pt.x;
		break;
	case WMSZ_BOTTOMLEFT:
		rt.left = (rt.right - pt.x) < minInfo.cx ? (rt.right - minInfo.cx) : pt.x;
		rt.bottom = (pt.y - rt.top) < minInfo.cy ? (rt.top + minInfo.cy) : pt.y;
		break;
	case WMSZ_BOTTOMRIGHT:
		rt.bottom = (pt.y - rt.top) < minInfo.cy ? (rt.top + minInfo.cy) : pt.y;
		rt.right = (pt.x - rt.left) < minInfo.cx ? (rt.left + minInfo.cx) : pt.x;
		break;
	default:
		return;
	}
	if (curRt.left == rt.left 
		&& curRt.right == rt.right
		&& curRt.top == rt.top
		&& curRt.bottom == rt.bottom)
	{
		return;
	}
	INT32 nWidth = rt.right - rt.left;
	INT32 nHeight = rt.bottom - rt.top;
	// 	if (nWidth < minInfo.cx || nHeight < minInfo.cy)
	// 	{
	// 		return;
	// 	}
	MoveWindow(m_hWnd, rt.left, rt.top, nWidth, nHeight, FALSE);
}
LRESULT CMainFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return S_OK;
}
void CMainFrame::OpenCurDir()
{
	CString strDir = Util::Path::GetExeFolder();
	CStringA strADir;
	Util::String::W_2_Utf8(strDir, strADir);
	strADir = "Explorer.exe " + strADir;
	WinExec(strADir, SW_SHOW);
}
void CMainFrame::InitWndRect()
{
	RECT rt;
	GetWindowRect(m_hWnd, &rt);
	LONG lImageWidth = rt.right - rt.left;
	LONG lImageHeight = rt.bottom - rt.top;
	if (lImageWidth == 0 || lImageHeight == 0)
		return;

	//获取屏幕像素	
	HDC hDC = ::GetDC(NULL); // 获得屏幕设备描述表句柄
	LONG lScreentWidth = GetDeviceCaps(hDC, HORZRES); // 获取屏幕水平分辨率
	LONG lScreentHeight = GetDeviceCaps(hDC, VERTRES); // 获取屏幕垂直分辨率
	::ReleaseDC(NULL, hDC);

	//计算窗口大小
	LONG lRealWidth = (lImageWidth > lScreentWidth - 0/*IMAGE_SHOW_SPACE_W*/) ? (lScreentWidth - 0/*IMAGE_SHOW_SPACE_W*/) : lImageWidth;
	LONG lRealHeight = (lImageHeight > lScreentHeight - 0/*IMAGE_SHOW_SPACE_H*/) ? (lScreentHeight - 0/*IMAGE_SHOW_SPACE_H*/) : lImageHeight;
	lRealWidth = (lRealWidth > 800/*WINDOW_MIN_WIDTH*/) ? lRealWidth : 800/*WINDOW_MIN_WIDTH*/;
	lRealHeight = (lRealHeight > 600/*WINDOW_MIN_HEIGHT*/) ? lRealHeight : 600/*WINDOW_MIN_HEIGHT*/;
	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, lRealWidth, lRealHeight, SWP_NOMOVE);
	MoveWindow(m_hWnd, 0, 0, lRealWidth, lRealHeight, FALSE);
	CenterWindow();
}
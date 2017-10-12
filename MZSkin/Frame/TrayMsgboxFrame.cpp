#include "stdafx.h"
#include "TrayMsgboxFrame.h"
#include "UtilPath.h"
#include "UtilLZ.h"
#include <SkinInner//SkinAPI.h>

namespace DuiLib{

#define TRAYID							3233	//托盘图标ID
#define MOUSECHECK_TIMER_ID				123		//定时器ID	
#define TRAYFLASH_TIME_ID				23421	//托盘闪烁定时器ID
#define TRAYFLASH_TIME					400		//托盘闪烁间隔
#define TRAY_MSGBOX_CHECKMOUSE_TIME		500		//消息盒子窗口鼠标检测时间间隔
#define MSGBOX_TREEITEM_MAXCOUNT		5
#define MSGBOX_TREEUI_WIDTH				260
#define MSGBOX_TREEUI_HEIGHT			71
#define MSGBOX_TREEITEM_HEIGHT			50
#define UM_TRAYMSGNOTIFY				(WM_USER + 3001)	//托盘通知消息
#define TIMER_TRAY_REG                  9000    //托盘注册失败，延时注册

CTrayMsgboxFrame::CTrayMsgboxFrame() :
	m_pTrayEvent(NULL),
	m_bIsHoverMsgBox(FALSE),
	m_bIsHoverTray(FALSE),
	m_pTrayPos(NULL),
	m_bTrayFlashFlag(FALSE)
{
}

CTrayMsgboxFrame::~CTrayMsgboxFrame()
{

}

////////////////////////////////dui窗口相关//////////////////////////////////////////
CDuiString CTrayMsgboxFrame::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CTrayMsgboxFrame::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CTrayMsgboxFrame::GetSkinFile()
{
	return _T("xml\\TrayMsgboxFrame.xml");;
}

LPCTSTR CTrayMsgboxFrame::GetWindowClassName(void) const
{
	return _T("TrayMsgboxFrameClass");
}

void CTrayMsgboxFrame::InitWindow()
{
	::SetTimer(m_hWnd, MOUSECHECK_TIMER_ID, TRAY_MSGBOX_CHECKMOUSE_TIME, NULL); //启动鼠标检测定时器
	::SetTimer(m_hWnd, TRAYFLASH_TIME_ID, TRAYFLASH_TIME, NULL);

}

LRESULT CTrayMsgboxFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT hr = S_FALSE;
	bHandled = FALSE;
	
	switch(uMsg)
	{
	case WM_TIMER:		
		hr = OnTimer(uMsg, wParam, lParam, bHandled);
		break;
	case UM_TRAYMSGNOTIFY:
		hr = OnTrayMsg(uMsg, wParam, lParam, bHandled);
		break;
	}
	if (::RegisterWindowMessage(_T("TaskbarCreated")) == uMsg)
	{
		OnTaskBarCreated(wParam, lParam);
	}
	return hr;
}

void CTrayMsgboxFrame::Notify(TNotifyUI& msg)
{
	CDuiString sMsgType = msg.sType;
	if (sMsgType == DUI_MSGTYPE_CLICK){
		OnClick(msg);
	}
}

LRESULT CTrayMsgboxFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return FALSE;
	}

	return FALSE;
}

LRESULT CTrayMsgboxFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bIsHoverMsgBox){
		m_bIsHoverMsgBox = TRUE;
	}
	return __super::OnMouseMove(uMsg, wParam, lParam, bHandled);
}

LRESULT CTrayMsgboxFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::KillTimer(m_hWnd, MOUSECHECK_TIMER_ID);
	::KillTimer(m_hWnd, TRAYFLASH_TIME_ID);

	if (m_pTrayPos)
	{
		m_pTrayPos->DeleteIcon();
		delete m_pTrayPos;
		m_pTrayPos = NULL;
	}
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

//定时器
LRESULT CTrayMsgboxFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (TIMER_TRAY_REG == wParam)
    {
        static UINT nCnt = 0;
        nCnt++;
        if (nCnt > 5)
        {
            ::KillTimer(m_hWnd, TIMER_TRAY_REG);
        }
        if (m_pTrayPos)
        {
            BOOL bRet = m_pTrayPos->AddIcon();
            if (bRet)
            {
                ::KillTimer(m_hWnd, TIMER_TRAY_REG);
            }
        }
        
    }
	if (MOUSECHECK_TIMER_ID == wParam)
	{
		RECT rct;
		POINT Cursorpt = { 0, 0 };

		if (!m_bIsHoverMsgBox) return S_OK;

		::GetCursorPos(&Cursorpt);	
		::GetWindowRect(m_hWnd, &rct);
		if (Cursorpt.x < rct.left || Cursorpt.x > rct.right || Cursorpt.y < rct.top || Cursorpt.y > rct.bottom)
		{
			if (m_bIsHoverTray == FALSE)
			{
				HideTrayMsgboxWnd();
				m_bIsHoverMsgBox = FALSE;
			}
		}
	}
	else if (TRAYFLASH_TIME_ID == wParam)
	{
		EC_TRAY_ICON eCurrentIcon = DEFAULT_NORMAL_ICON;


		HICON hIcon = NULL;
		std::map<EC_TRAY_ICON, HICON>::iterator it = m_mapIcon.find(eCurrentIcon);
		if (it != m_mapIcon.end()) hIcon = it->second;
		if (m_pTrayPos) m_pTrayPos->SetIcon(hIcon);
	}
	return S_OK;
}

//托盘消息
LRESULT CTrayMsgboxFrame::OnTrayMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	switch (lParam)
	{
	case WM_MOUSELEAVE:
		OnMouseLeaveT(uMsg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMoveT(uMsg, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDownT(uMsg, wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDownT(uMsg, wParam, lParam);
		break;
	}
	return S_OK;
}

//托盘焦点进入
LRESULT CTrayMsgboxFrame::OnMouseMoveT(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nCount = 0;
	m_pTrayPos->OnMouseMove();

	POINT pt = { 0 };
	RECT rc = GetTrayWndRect();
	::GetCursorPos(&pt);
	if (PtInRect(&rc, pt))
	{

	}
	m_bIsHoverTray = TRUE;
	return S_OK;
}

//托盘焦点离开
LRESULT CTrayMsgboxFrame::OnMouseLeaveT(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bIsHoverMsgBox)
	{
		HideTrayMsgboxWnd();
	}
	m_bIsHoverTray = FALSE;
	return S_OK;
}

//鼠标点击托盘
LRESULT CTrayMsgboxFrame::OnLButtonDownT(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pTrayEvent == NULL)
		return E_FAIL;
	m_pTrayEvent->LButtonDownEvent();
	return S_OK;
}

//鼠标右键托盘
LRESULT CTrayMsgboxFrame::OnRButtonDownT(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pTrayEvent) m_pTrayEvent->RButtonDownEvent();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//获取当前托盘显示图标
EC_TRAY_ICON CTrayMsgboxFrame::GetCurrentIcon()
{
	EC_TRAY_ICON eCursorIcon = DEFAULT_NORMAL_ICON;
	return eCursorIcon;
}

//显示窗口
void CTrayMsgboxFrame::ShowTrayMsgboxWnd()
{
	//鼠标进入托盘	
	RECT  rcTray = { 0 };
	POINT ptCursor = { 0 };
	POINT ptMsgBox = { 0 };
	::GetCursorPos(&ptCursor);
	rcTray = GetTrayWndRect();

	//以下计算只考虑系统工具栏置于桌面底端的情况
	ptMsgBox.x = ptCursor.x - (MSGBOX_TREEUI_WIDTH / 2);
	ptMsgBox.y = rcTray.top;

	if (::IsWindow(m_hWnd))
	{

		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
		SIZE size = GetChangeWndSize();
		if (oMonitor.rcWork.right < ptMsgBox.x + size.cx){
			ptMsgBox.x = oMonitor.rcWork.right - size.cx;
		}
		if (ptMsgBox.x < oMonitor.rcWork.left){
			ptMsgBox.x = oMonitor.rcWork.left;
		}
		if (ptMsgBox.y - size.cy - 3 < oMonitor.rcWork.top){
			ptMsgBox.y = oMonitor.rcWork.top + size.cy + 3;
		}
		
		if (!::IsWindowVisible(m_hWnd))
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST, ptMsgBox.x, ptMsgBox.y - size.cy - 3, size.cx, size.cy, (::IsWindowVisible(m_hWnd) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW) | SWP_NOACTIVATE);
			::ShowWindow(m_hWnd, SW_SHOWNA);
		}
	}
}
//隐藏窗口
void CTrayMsgboxFrame::HideTrayMsgboxWnd()
{
	if (::IsWindowVisible(m_hWnd))
	{
		::ShowWindow(m_hWnd, SW_HIDE);
	}
}

//获取需要改变的窗口大小
SIZE CTrayMsgboxFrame::GetChangeWndSize()
{
	SIZE size = { MSGBOX_TREEUI_WIDTH, MSGBOX_TREEUI_HEIGHT };
	return size;
}

//获取需要改变的窗口位置（没有减窗口高度的位置）
POINT CTrayMsgboxFrame::GetChangeWndPos()
{
	int iWndY = MSGBOX_TREEUI_HEIGHT;
	POINT pt = { 0, 0 };
	if (0)
	{
		RECT rct;
		::GetWindowRect(m_hWnd, &rct);
		pt.x = rct.left;
		pt.y = GetTrayWndRect().top - 3;//没有减窗口高度的位置
	}
	return pt;
}

//获取托盘区窗口的位置信息
RECT CTrayMsgboxFrame::GetTrayWndRect()
{
	RECT rect = { 0 };

	BOOL bIsExistNotifyIconGetRect = FALSE;
	typedef HRESULT(WINAPI *PFNShell_NotifyIconGetRect)(const NOTIFYICONIDENTIFIER* identifier, RECT* iconLocation);
	HMODULE hSysModule = ::LoadLibrary(_T("Shell32.dll"));
	if (hSysModule)
	{
		PFNShell_NotifyIconGetRect fnNotifyIconGetRect = (PFNShell_NotifyIconGetRect)::GetProcAddress(hSysModule, "Shell_NotifyIconGetRect");
		if (fnNotifyIconGetRect)
		{
			NOTIFYICONIDENTIFIER nii;
			memset(&nii, 0, sizeof(nii));
			nii.cbSize = sizeof(nii);
			nii.hWnd = m_hWnd;
			nii.uID = TRAYID;
			fnNotifyIconGetRect(&nii, &rect);
			bIsExistNotifyIconGetRect = TRUE;
		}
	}
	if (!bIsExistNotifyIconGetRect)
	{
		HWND TrayHwnd = NULL;
		TrayHwnd = FindTrayWnd();

		if (TrayHwnd != NULL)
		{
			GetWindowRect(TrayHwnd, &rect);
		}
	}
	return rect;
}

//获取正常托盘区窗口句柄
HWND CTrayMsgboxFrame::FindTrayWnd()
{
	HWND hWnd = NULL;
	HWND hWndPaper = NULL;

	if ((hWnd = FindWindow(TEXT("Shell_TrayWnd"), NULL)) != NULL)
	{
		if ((hWnd = FindWindowEx(hWnd, 0, TEXT("TrayNotifyWnd"), NULL)) != NULL)
		{
			hWndPaper = FindWindowEx(hWnd, 0, TEXT("SysPager"), NULL);
			if (!hWndPaper)
				hWnd = FindWindowEx(hWnd, 0, TEXT("ToolbarWindow32"), NULL);
			else
				hWnd = FindWindowEx(hWndPaper, 0, TEXT("ToolbarWindow32"), NULL);
		}
	}
	return hWnd;
}

//按下控件
void CTrayMsgboxFrame::OnClick(TNotifyUI& msg)
{
	CDuiString strCtrlName = msg.pSender->GetName();
	if (strCtrlName == _T("btn_cancel")){
		HideTrayMsgboxWnd();
	}
	else if (strCtrlName == _T("btn_hide")){
		HideTrayMsgboxWnd();
	}
	else if (strCtrlName == _T("btn_close")){
		HideTrayMsgboxWnd();
	}
}

LRESULT CTrayMsgboxFrame::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)
{
	m_pTrayPos->DeleteIcon();
	m_pTrayPos->ShowIcon();
	return 0;
}
/////////////////////////////////////外部接口/////////////////////////////////////
//类初始化
void CTrayMsgboxFrame::Init(LPCTSTR szToolTip, ITrayEvent * pTrayEvent, std::map<EC_TRAY_ICON, HICON> mapIcon)
{
	m_pTrayEvent = pTrayEvent;
	m_mapIcon = mapIcon;
	HWND hWnd = Create(GetDesktopWindow(), _T("TrayMsgboxFrame"), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
	if (::IsWindow(hWnd))
	{
		HideTrayMsgboxWnd();
		std::map<EC_TRAY_ICON, HICON>::iterator it = m_mapIcon.find(DEFAULT_NORMAL_ICON);
		if (it != m_mapIcon.end())
		{
			HICON hIcon = it->second;
            m_pTrayPos = new CMsgTrayPos(hWnd, TRAYID, UM_TRAYMSGNOTIFY, hIcon, szToolTip);
            BOOL bRet = m_pTrayPos->AddIcon();
            if (!bRet)
            {
                ::SetTimer(hWnd, TIMER_TRAY_REG, 10 * 1000, NULL);
            }
		}
	}
}

//修改托盘图标文本
void CTrayMsgboxFrame::SetTooltipText(LPCTSTR szToolTip)
{
    ASSERT(szToolTip != NULL);
    if (m_pTrayPos)
    {
        m_pTrayPos->SetTooltipText(szToolTip);
    }
}


void CTrayMsgboxFrame::CloseTrayMsgboxWnd()
{
	HideTrayMsgboxWnd();
}


////////////////////////////////////////////////////////////////////

}



#include "stdafx.h"
#include "AuxMainUI.h"
#include "Singleton.h"
#include "UtilLog.h"
#include "..\Frame\MainFrame.h"

void Aux::MainUI::ShowMain()
{
	CMainFrame* pMainFrm = &Singleton<CMainFrame>::Instance();
	HWND hWnd = pMainFrm->Create(NULL, _T("名智管理系统"), WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0);

	RECT rc;
	if (!::IsRectEmpty(&rc))
	{
		RECT rcArea = { };
		MONITORINFO oMonitor = { };
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
		rcArea = oMonitor.rcWork;
		if (rcArea.left == rc.left && rc.right == rcArea.right && rc.bottom == rcArea.bottom && rc.top == rcArea.top)
			::PostMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		else
			::MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}
	else
	{
		pMainFrm->CenterWindow();
	}
	pMainFrm->ShowWindow(true, true);
	Util::Log::Info(_T("MZSKin"), _T("[info]Show Main Window\r\n"));
}

void Aux::MainUI::Exit()
{
	Singleton<CMainFrame>::UnInstance();
}
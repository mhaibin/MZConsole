#pragma once

#include <event.h>
#include <UIlib.h>

using namespace DuiLib;

namespace event{
	//推出进程的通知
	common_signal0 &ExitProcessEvn();
	//打开工作站列表页面
	common_signal0 &OpenWorkstationPage();
	//打开服务器列表页面
	common_signal0 &OpenServerPage();
	//打开磁盘管理页面
	common_signal0 &OpenDiskMgrPage();
	//打开资源更新页面
	common_signal0 &OpenResourceUpdatePage();
	//打开系统控制页面
	common_signal0 &OpenMonitorPage();
	//打开参数设置页面
	common_signal0 &OpenSetPage();
	//工作站状态更新
	common_signal0 &UpdateStatusBar();
}

namespace Popup
{
	//打开添加工作站窗口
	void OpenAddWorkstationWnd(HWND hWnd, UINT32 u32Flag);
	//打开修改工作站窗口
	void OpenModifyWorkstationWnd();
	//打开新增服务器窗口
	void OpenAddServerWnd(HWND hWnd, CString strSvrName);
	//打开磁盘维护设置窗口
	void OpenDiskInstructionWnd(UINT8 type);
	//打开注册验证窗口
	void OpenRegisterWnd();
}
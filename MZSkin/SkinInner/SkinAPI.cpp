#include "stdafx.h"
#include "SkinAPI.h"

namespace event{
common_signal0 &ExitProcessEvn()
{
	static common_signal0 __ExitProcess;
	return __ExitProcess;
}
common_signal0 &OpenWorkstationPage()
{
	static common_signal0 __OpenWorkstationPage;
	return __OpenWorkstationPage;
}
common_signal0 &OpenServerPage()
{
	static common_signal0 __OpenServerPage;
	return __OpenServerPage;
}
common_signal0 &OpenDiskMgrPage()
{
	static common_signal0 __OpenDiskMgrPage;
	return __OpenDiskMgrPage;
}
common_signal0 &OpenResourceUpdatePage()
{
	static common_signal0 __OpenResourceUpdatePage;
	return __OpenResourceUpdatePage;
}
common_signal0 &OpenMonitorPage()
{
	static common_signal0 __OpenMonitorPage;
	return __OpenMonitorPage;
}
common_signal0 &OpenSetPage()
{
	static common_signal0 __OpenSetPage;
	return __OpenSetPage;
}
common_signal0 &UpdateStatusBar()
{
	static common_signal0 __UpdateStatusBar;
	return __UpdateStatusBar;
}
}
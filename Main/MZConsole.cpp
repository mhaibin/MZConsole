// MZConsole.cpp : WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "MZConsole_i.h"
#include "UIlib.h"
#include "dlldatax.h"
#include "Utilkobj.h"
#include "UtilPath.h"
#include "MZConsoleDefine.h"
#include "UtilLog.h"
#include "UtilTools.h"
#include "AuxMainUI.h"
#include "AuxServerAPI.h"

using namespace DuiLib;

static CString Host_GetExeFolder()
{
	TCHAR szDir[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szDir, sizeof(szDir));

	CString	filepath = szDir;
	int ix = filepath.ReverseFind(_T('\\'));
	if (ix != -1)
	{
		return filepath.Mid(0, ix);
	}

	ATLASSERT(FALSE);
	return _T("");
}

class CMZConsoleModule : public CAtlExeModuleT< CMZConsoleModule >
{
public :
	DECLARE_LIBID(LIBID_MZConsoleLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MZCONSOLE, "{02AA8136-A4CC-4BFC-A48E-3D97193CD34F}")
};

CMZConsoleModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR lpCmdLine, int nShowCmd)
{
	//唯一实例
	HANDLE hMutex = Util::kObj::CreateMutexFormat(_T("Proc_MZConsole_IPC_Unique"));
	if (!hMutex)
	{
		ATLASSERT(FALSE);
		return -2;
	}
	Util::kObj::CMutexLifeWrapper objMutex(hMutex);
	HRESULT hr = GetLastError();
	if (ERROR_ALREADY_EXISTS == hr)
	{
		//如果存在就显示窗口
		DWORD nTid = 0;
#ifdef _DEBUG
		nTid = Util::Process::IsProcessExist(_T("MZConsoleD.exe"));
#else
		nTid = Util::Process::IsProcessExist(_T("MZConsole.exe"));
#endif // _DEBUG
		LRESULT lResult = 0;
		if (0 != nTid)
		{
			DWORD dResult = 0;
			HWND hWnd = Util::Process::GetWindowsHandleByPid(nTid, _T("名智管理系统"));//注意：这里要跟的窗口名一致；
			lResult = ::SendMessage(hWnd, UW_SHOWWINDOW_MSG, TRUE, NULL);
		}
		return -3;
	}
	Util::Path::SetExeFolder(Host_GetExeFolder());

	//界面初始化工作
	INITCOMMONCONTROLSEX ex;
	ex.dwSize = sizeof(ex);
	ex.dwICC = ICC_WIN95_CLASSES;
	::InitCommonControlsEx(&ex);

	DuiLib::CPaintManagerUI::SetInstance(hInstance);
	CString s = Util::Path::GetResFolder();
	DuiLib::CPaintManagerUI::SetResourcePath(s);

	Util::Log::SetLevel(Util::Log::E_RLL_NONE);
	Util::Log::Change(NULL, 0);
	Aux::MZDIO::InitMzdIOData();
	Aux::MainUI::ShowMain();
	
	CPaintManagerUI::MessageLoop();

	Aux::MainUI::Exit();
	Aux::Server::Exit();
	Util::Log::Flush();
    return 0;//_AtlModule.WinMain(nShowCmd);
}


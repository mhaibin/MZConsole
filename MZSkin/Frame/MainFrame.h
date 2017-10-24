#pragma once

#include <UIlib.h>
#include <vector>
#include <atlstr.h>
#include <map>
#include "sigslot.h"
#include "TrayMsgboxFrame.h"
#include <Border/BorderDrag.h>

using namespace DuiLib;
using namespace std;
using namespace ATL;

class CMainFrame : public WindowImplBase,
	public sigslot::has_slots<>,
	public ITrayEvent
{
public:
	CMainFrame();
	~CMainFrame();

public:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void InternationalString(LPCTSTR pstrCtlName, CDuiString &strText, CControlUI *pControl);
	virtual void Notify(TNotifyUI& msg);
	virtual void InitWindow();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void ShowMain();
	virtual void LButtonDownEvent();
	virtual void RButtonDownEvent();

	//拖动边框更新窗口大小
	void UpdateWindowSizeByBorderDrag(UINT32 uflag, POINT &pt);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	//打开程序所在的目录
	void OpenCurDir();

	void InitWndRect();

private:

};
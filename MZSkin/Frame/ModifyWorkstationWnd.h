/*
 * 文件名称：ModifyWorkstationWnd.h
 * 描    述：修改工作站窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.9.1
 */

#ifndef __MODIFYWORKSTATION_WND_H__
#define __MODIFYWORKSTATION_WND_H__

#include "UIlib.h"

using namespace std;
namespace DuiLib{

class CModifyWorkstationWnd : public WindowImplBase
{
public:
	CModifyWorkstationWnd();
	~CModifyWorkstationWnd(void);
protected:
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);

protected:
	void ExitProcessEvn();
};

}

#endif

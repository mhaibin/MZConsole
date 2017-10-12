/*
 * 文件名称：DiskMtFrame.h
 * 描    述：磁盘维护表窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.8.24
 */

#ifndef __DISKMT_FRAME_H__
#define __DISKMT_FRAME_H__

#include "UIlib.h"
#include <sigslot.h>

using namespace std;
namespace DuiLib{

class CDiskMtFrame : public WindowImplBase,
	public sigslot::has_slots<>
{
public:
	CDiskMtFrame();
	~CDiskMtFrame(void);
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
	virtual void OnItemSelect(TNotifyUI& msg);
	virtual void OnToolBarClick(TNotifyUI& msg);

protected:
	void ExitProcessEvn();

private:
	//删除指定磁盘
	void OnDel();
};

}

#endif

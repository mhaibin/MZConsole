/*
 * 文件名称：ServerFrame.h
 * 描    述：服务器列表窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.8.24
 */

#ifndef __SERVER_FRAME_H__
#define __SERVER_FRAME_H__

#include "UIlib.h"
#include <sigslot.h>
#include <ContactList\CompContactList.h>

using namespace std;
namespace DuiLib{

class CServerFrame : public WindowImplBase,
	public sigslot::has_slots<>
{
public:
	CServerFrame();
	~CServerFrame(void);
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

private:
	//删除服务器
	void OnDel();

	//更新工作站状态
	void OnUpdateStatus();

private:
	CListContainerElementUI			*m_pElement;
	CServerContactList				*m_pList;
};

}

#endif

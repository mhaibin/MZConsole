/*
 * 文件名称：AddServerWnd.h
 * 描    述：添加服务器窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.9.1
 */

#ifndef __ADDSERVER_WND_H__
#define __ADDSERVER_WND_H__

#include "UIlib.h"
#include "SharedUI/Frame/ResolutionWHListFrame.h"

using namespace std;
namespace DuiLib{

class CAddServerWnd : public WindowImplBase
{
public:
	CAddServerWnd();
	~CAddServerWnd(void);
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
	virtual void OnItemClick(TNotifyUI& msg);

public:
	void SetServerName(CString strName){m_strServerName = strName;}
	void SetWnd(HWND hWnd){m_hParentWnd = hWnd;}
protected:
	void ExitProcessEvn();

private:
	//初始化备用服务器列表
	void InitSpareServerlist();

	void OnDropDown(TNotifyUI& msg);
	//从EditComb列表中删除一项
	void OnDeleteResolutionWHItem(TNotifyUI& msg);
	//从EditComb列表中选择一项
	void OnSeleteResolutionWHItem(TNotifyUI& msg);

private:
	//查找指定IP服务器
	void OnCheckFindServer();
	//点击应用按钮
	void OnClickApply();
private:
	CResolutionWHListFrame	*m_pSpareSvr;
	CResolutionWHListFrame	*m_pRWriteDisk;

private:
	CString					m_strServerName;				//服务器名称
	HWND					m_hParentWnd;					//父窗口句柄;
};

}

#endif

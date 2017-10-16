/*
 * 文件名称：WorkstationFrame.h
 * 描    述：工作站列表窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.8.24
 */

#ifndef __WORKSTATION_FRAME_H__
#define __WORKSTATION_FRAME_H__

#include "UIlib.h"
#include <sigslot.h>
#include "SkinFieldDef.h"
#include <ContactList\CompContactList.h>
#include <SharedUI/Frame/ServerTreeUI.h>

using namespace std;
namespace DuiLib{

class CWorkstationFrame : public WindowImplBase,
	public sigslot::has_slots<>
{
public:
	CWorkstationFrame();
	~CWorkstationFrame(void);
protected:
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
//	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDBClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);

protected:
	void ExitProcessEvn();
	
	void UpdateStatusBar();
	//工作站有更新
	void OnUpdateWorkstation();
	//MZDIO服务器运行通知
	void OnMZDIORun(BOOL bRun);
private:
	//删除工作站
	void OnDel();
	//禁用工作站
	void OnDisable();
	//启用工作站
	void OnEnable();
	//重启工作站
	void OnReboot();
	//关机
	void OnPowerOff();
	//开超管
	void OnSuperMgr();

	//添加分组
	void AddGroup();
	//删除分组
	void DelGroup(CControlUI *pNode);
	//修改分组
	void ModifyGroup(CControlUI *pNode);

	//清除MAC
	void OnClearMac();
	//唤醒
	void OnWakeUp();

	//更新加载工作站
	void OnLoadWorkstation();

private:
	BOOL OnRButtonDownWorkstationGroupList(POINT &pt);
	BOOL OnRButtonDownWorkstationList(POINT &pt);

	//更新工作站状态
	void OnUpdateWorkstationStatus();
private:
	CWorkstationContactList *m_pList;
	CListContainerElementUI *m_pElement;
	CServerTreeUI			*m_GroupList;
	CTreeNodeUI				*m_pNode;
	CLabelUI				*m_pLabCurModifyGroup;	//当前修改的分组
	CLabelUI				*m_pLabStatus;

private:
	UINT32					m_u32CurWksCount;		//点击添加或修改时的工作站的个数

	BOOL					m_isMZDIORun;
};

}

#endif

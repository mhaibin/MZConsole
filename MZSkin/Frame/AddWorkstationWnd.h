/*
 * 文件名称：AddWorkstationWnd.h
 * 描    述：添加工作站窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.9.1
 */

#ifndef __ADDWORKSTATION_WND_H__
#define __ADDWORKSTATION_WND_H__

#include "UIlib.h"
#include "SharedUI/Frame/ResolutionWHListFrame.h"
#include <ContactList/ConfigContactList.h>

using namespace std;
namespace DuiLib{

class CAddWorkstationWnd : public WindowImplBase
{
public:
	CAddWorkstationWnd();
	~CAddWorkstationWnd(void);
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
	
	void OnCheck(TNotifyUI& msg);

public:
	void SetFlag(UINT32 u32Flag){m_u32Flag = u32Flag;}
	void SetWnd(HWND hWnd){m_hParentWnd = hWnd;}
private:
	//初始化分辨率大小列表
	void InitPDIlist();
	//初始化服务器列表
	void InitServerList();
	//初始化磁盘路径列表
	void InitDiskPathList(LPCTSTR pstrServerName);
	void OnDropDown(TNotifyUI& msg);
	//从EditComb列表中删除一项
	void OnDeleteResolutionWHItem(TNotifyUI& msg);
	//从EditComb列表中选择一项
	void OnSeleteResolutionWHItem(TNotifyUI& msg);

	void AddNum(CControlUI *pControl, INT32 nMaxVal = 999999);
	void SubNum(CControlUI *pControl, INT32 nMinVal = -99999);

	//点击了应用按钮
	void OnClickApply();
	//添加新工作站
	INT32 AddNewWorkstation();
	//修改工作站
	void ModifyWorkstation();

	void InitMenuDiskInfo();

protected:
	void ExitProcessEvn();

private:
	UINT32			m_u32MinIndex;	//最小编号
	UINT32			m_u32Flag;		//窗口类型,0:表示添加,>0:表示修改编号
	HWND			m_hParentWnd;	//父窗口句柄;
	CString			m_strError;

private:
	CResolutionWHListFrame *m_pPDI;
	CResolutionWHListFrame *m_pServerNameList;
	CResolutionWHListFrame *m_pDiskPathList;
	CListContainerElementUI *m_pCurMDSelElement;	//当前菜单列表选择的节点

	CEditUI					*m_EditNum;				//编号
	CEditUI					*m_EditSize;			//数量
	CEditUI					*m_EditNumLen;			//长度
	CEditUI					*m_EditFName;			//名称前缀
	CEditUI					*m_EditBName;			//名称后缀
	CEditUI					*m_EditDelay;			//菜单延时
	CEditUI					*m_EditName;			//工作站名称
	CEditUI					*m_EditMac;				//工作站MAC
	CCheckBoxUI				*m_ChkBoxRestore;		//自动还原
	CIPAddressExUI			*m_ipStartIP;			//起始IP
	CIPAddressExUI			*m_ipDNSI1;				//DNSI1
	CIPAddressExUI			*m_ipGetway;			//网关
	CIPAddressExUI			*m_ipMask;				//掩码
	CIPAddressExUI			*m_ipDNSI2;				//DNSI2

	CDiskInfoContactList	*m_pMenuList1;			//菜单1列表
	CDiskInfoContactList	*m_pMenuList2;			//菜单2列表
	CDiskInfoContactList	*m_pMenuList3;			//菜单3列表
	CDiskInfoContactList	*m_pMenuList4;			//菜单4列表
};

}

#endif

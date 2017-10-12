/*
 * 文件名称：RegisterWnd.h
 * 描    述：名智注册验证
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.9.25
 */

#ifndef __REGISTER_WND_H__
#define __REGISTER_WND_H__

#include "UIlib.h"
#include "SharedUI\Frame\ResolutionWHListFrame.h"
#include "..\Global\Control\EditCombUI.h"

using namespace std;
namespace DuiLib{

class CRegisterWnd : public WindowImplBase
{
public:
	CRegisterWnd();
	~CRegisterWnd(void);
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
	//初始化序列号列表
	void InitDiskSeriallist();
	void OnDropDown(TNotifyUI& msg);
	//从EditComb列表中删除一项
	void OnDeleteResolutionWHItem(TNotifyUI& msg);
	//从EditComb列表中选择一项
	void OnSeleteResolutionWHItem(TNotifyUI& msg);

	//点击确定按钮
	void OnClickBtnOK();
	//点击获取按钮
	void OnClickBtnGet();
	//用做IO服务器
	void OnClickBtnUserIO();

private:
	CResolutionWHListFrame			*m_pDiskSerial;		//磁盘序列号
	CEditCombUI						*m_pecbDiskSerial;	//磁盘序列号
	CEditUI							*m_EdtPass;
	CEditUI							*m_EdtPasWord;
	CEditUI							*m_EdtSecurity;
	CCheckBoxUI						*m_chbUndisplay;
	CLabelUI						*m_LabTips;
	CLabelUI						*m_LabSerialTips;
	CLabelUI						*m_LabPassTips;
	CLabelUI						*m_LabPasswordTips;
	CLabelUI						*m_LabSecurityTips;
};

}

#endif

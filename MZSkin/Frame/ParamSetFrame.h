/*
 * 文件名称：ParamSetFrame.h
 * 描    述：参数设置窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.9.28
 */

#ifndef __PARAMSET_FRAME_H__
#define __PARAMSET_FRAME_H__

#include "UIlib.h"
#include <sigslot.h>
#include "BtserverAPI.h"
#include <SharedUI/Frame/DownLoadSetLayoutUI.h>

using namespace std;
namespace DuiLib{

class CParamSetFrame : public WindowImplBase,
	public sigslot::has_slots<>
{
public:
	CParamSetFrame();
	~CParamSetFrame(void);
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
    LRESULT OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);

protected:
	void ExitProcessEvn();

private:
	CTabLayoutUI		*m_pTabParamSet;
	CDownLoadSetLayoutUI *m_pDownLoadSetUI;
	HINSTANCE			m_hLibFlag;
	_btuiCreateChild	m_BTCreateChild;

	tagMZDLABEL			m_BTSetLabel;
};

}

#endif

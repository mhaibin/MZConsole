#ifndef __KK_TRAY_MSGBOX_FRAME_HH__
#define __KK_TRAY_MSGBOX_FRAME_HH__

#include "UIlib.h"
#include <Tray/traypos.h>
#include <map>
#include <sigslot.h>


namespace DuiLib{

typedef enum MyEnum
{
	UNKNOWN_ICON,			//未知图标
	DEFAULT_NORMAL_ICON,	//默认正常图标
}EC_TRAY_ICON;

class ITrayEvent
{
public:
	~ITrayEvent() {}
public:
	virtual void LButtonDownEvent() = 0;
	virtual void RButtonDownEvent() = 0;
};

class CTrayMsgboxFrame : public WindowImplBase
	, public sigslot::has_slots<>
{
public:
	~CTrayMsgboxFrame();
	CTrayMsgboxFrame();
protected:
	////////////////////////////////dui窗口相关//////////////////////////////////////////
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	//////////////////////////////////事件////////////////////////////////////////
	//定时器
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//托盘消息
	LRESULT OnTrayMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//托盘焦点进入
	LRESULT OnMouseMoveT(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//托盘焦点离开
	LRESULT OnMouseLeaveT(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//鼠标点击托盘
	LRESULT OnLButtonDownT(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//鼠标右键托盘
	LRESULT OnRButtonDownT(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	//获取当前托盘显示图标
	EC_TRAY_ICON GetCurrentIcon();
	//显示窗口
	void ShowTrayMsgboxWnd();
	//获取正常托盘区窗口句柄
	HWND FindTrayWnd(); 
	//隐藏窗口
	void HideTrayMsgboxWnd();
	//获取需要改变的窗口大小
	SIZE GetChangeWndSize();
	//获取需要改变的窗口位置（没有减窗口高度的位置）
	POINT GetChangeWndPos();
	//获取托盘区窗口的位置信息
	RECT GetTrayWndRect(); 
	//按下控件
	void OnClick(TNotifyUI& msg);
	//托盘重建
	LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
public:
	/////////////////////////////////外部接口/////////////////////////////////////////
	//类初始化
	void Init(LPCTSTR szToolTip, ITrayEvent * pTrayEvent, std::map<EC_TRAY_ICON, HICON> mapIcon);
    //修改托盘图标文本
    void SetTooltipText(LPCTSTR szToolTip);

	//关闭
	void CloseTrayMsgboxWnd();

protected:
	BOOL m_bIsHoverMsgBox; //鼠标是否进入消息盒子
	BOOL m_bIsHoverTray; //检测鼠标是否进入托盘
	BOOL m_bTrayFlashFlag;//是否切换图片图标
	CMsgTrayPos * m_pTrayPos;//托盘对象
	ITrayEvent * m_pTrayEvent;//托盘事件通知
	std::map<EC_TRAY_ICON, HICON> m_mapIcon;

};

}

#endif
/*
 * 文件名称：AddServerWnd.h
 * 描    述：磁盘管理窗口
 * 当前版本：1.0
 * 作    者：mhaibin
 * 创建日期：2017.9.4
 */

#ifndef __DISKINSTRUCTION_WND_H__
#define __DISKINSTRUCTION_WND_H__

#include "UIlib.h"

using namespace std;
namespace DuiLib{

class CDiskInstructionWnd : public WindowImplBase
{
public:
	CDiskInstructionWnd();
	~CDiskInstructionWnd(void);
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

public:
	void SetType(UINT8 type){m_u8Type = type;}

private:
	void OnClickCreateBtn();

private:
	UINT8			m_u8Type;			//窗口类型
	CString			m_strImgPath;		//镜像文件
	CString			m_strSrcPath;		//目标文件
	CString			m_strServerName;	//服务器名称
	DWORD			m_dwServerIP;		//服务器IP
	UINT32			m_u32Size;			//扩容大小;
};

}

#endif

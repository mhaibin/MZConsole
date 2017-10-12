/*
 * �ļ����ƣ�ServerFrame.h
 * ��    �����������б���
 * ��ǰ�汾��1.0
 * ��    �ߣ�mhaibin
 * �������ڣ�2017.8.24
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
	//ɾ��������
	void OnDel();

	//���¹���վ״̬
	void OnUpdateStatus();

private:
	CListContainerElementUI			*m_pElement;
	CServerContactList				*m_pList;
};

}

#endif

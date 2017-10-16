/*
 * �ļ����ƣ�WorkstationFrame.h
 * ��    ��������վ�б���
 * ��ǰ�汾��1.0
 * ��    �ߣ�mhaibin
 * �������ڣ�2017.8.24
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
	//����վ�и���
	void OnUpdateWorkstation();
	//MZDIO����������֪ͨ
	void OnMZDIORun(BOOL bRun);
private:
	//ɾ������վ
	void OnDel();
	//���ù���վ
	void OnDisable();
	//���ù���վ
	void OnEnable();
	//��������վ
	void OnReboot();
	//�ػ�
	void OnPowerOff();
	//������
	void OnSuperMgr();

	//��ӷ���
	void AddGroup();
	//ɾ������
	void DelGroup(CControlUI *pNode);
	//�޸ķ���
	void ModifyGroup(CControlUI *pNode);

	//���MAC
	void OnClearMac();
	//����
	void OnWakeUp();

	//���¼��ع���վ
	void OnLoadWorkstation();

private:
	BOOL OnRButtonDownWorkstationGroupList(POINT &pt);
	BOOL OnRButtonDownWorkstationList(POINT &pt);

	//���¹���վ״̬
	void OnUpdateWorkstationStatus();
private:
	CWorkstationContactList *m_pList;
	CListContainerElementUI *m_pElement;
	CServerTreeUI			*m_GroupList;
	CTreeNodeUI				*m_pNode;
	CLabelUI				*m_pLabCurModifyGroup;	//��ǰ�޸ĵķ���
	CLabelUI				*m_pLabStatus;

private:
	UINT32					m_u32CurWksCount;		//�����ӻ��޸�ʱ�Ĺ���վ�ĸ���

	BOOL					m_isMZDIORun;
};

}

#endif

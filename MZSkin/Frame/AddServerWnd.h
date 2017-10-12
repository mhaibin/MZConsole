/*
 * �ļ����ƣ�AddServerWnd.h
 * ��    ������ӷ���������
 * ��ǰ�汾��1.0
 * ��    �ߣ�mhaibin
 * �������ڣ�2017.9.1
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
	//��ʼ�����÷������б�
	void InitSpareServerlist();

	void OnDropDown(TNotifyUI& msg);
	//��EditComb�б���ɾ��һ��
	void OnDeleteResolutionWHItem(TNotifyUI& msg);
	//��EditComb�б���ѡ��һ��
	void OnSeleteResolutionWHItem(TNotifyUI& msg);

private:
	//����ָ��IP������
	void OnCheckFindServer();
	//���Ӧ�ð�ť
	void OnClickApply();
private:
	CResolutionWHListFrame	*m_pSpareSvr;
	CResolutionWHListFrame	*m_pRWriteDisk;

private:
	CString					m_strServerName;				//����������
	HWND					m_hParentWnd;					//�����ھ��;
};

}

#endif

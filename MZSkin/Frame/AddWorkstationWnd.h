/*
 * �ļ����ƣ�AddWorkstationWnd.h
 * ��    ������ӹ���վ����
 * ��ǰ�汾��1.0
 * ��    �ߣ�mhaibin
 * �������ڣ�2017.9.1
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
	//��ʼ���ֱ��ʴ�С�б�
	void InitPDIlist();
	//��ʼ���������б�
	void InitServerList();
	//��ʼ������·���б�
	void InitDiskPathList(LPCTSTR pstrServerName);
	void OnDropDown(TNotifyUI& msg);
	//��EditComb�б���ɾ��һ��
	void OnDeleteResolutionWHItem(TNotifyUI& msg);
	//��EditComb�б���ѡ��һ��
	void OnSeleteResolutionWHItem(TNotifyUI& msg);

	void AddNum(CControlUI *pControl, INT32 nMaxVal = 999999);
	void SubNum(CControlUI *pControl, INT32 nMinVal = -99999);

	//�����Ӧ�ð�ť
	void OnClickApply();
	//����¹���վ
	INT32 AddNewWorkstation();
	//�޸Ĺ���վ
	void ModifyWorkstation();

	void InitMenuDiskInfo();

protected:
	void ExitProcessEvn();

private:
	UINT32			m_u32MinIndex;	//��С���
	UINT32			m_u32Flag;		//��������,0:��ʾ���,>0:��ʾ�޸ı��
	HWND			m_hParentWnd;	//�����ھ��;
	CString			m_strError;

private:
	CResolutionWHListFrame *m_pPDI;
	CResolutionWHListFrame *m_pServerNameList;
	CResolutionWHListFrame *m_pDiskPathList;
	CListContainerElementUI *m_pCurMDSelElement;	//��ǰ�˵��б�ѡ��Ľڵ�

	CEditUI					*m_EditNum;				//���
	CEditUI					*m_EditSize;			//����
	CEditUI					*m_EditNumLen;			//����
	CEditUI					*m_EditFName;			//����ǰ׺
	CEditUI					*m_EditBName;			//���ƺ�׺
	CEditUI					*m_EditDelay;			//�˵���ʱ
	CEditUI					*m_EditName;			//����վ����
	CEditUI					*m_EditMac;				//����վMAC
	CCheckBoxUI				*m_ChkBoxRestore;		//�Զ���ԭ
	CIPAddressExUI			*m_ipStartIP;			//��ʼIP
	CIPAddressExUI			*m_ipDNSI1;				//DNSI1
	CIPAddressExUI			*m_ipGetway;			//����
	CIPAddressExUI			*m_ipMask;				//����
	CIPAddressExUI			*m_ipDNSI2;				//DNSI2

	CDiskInfoContactList	*m_pMenuList1;			//�˵�1�б�
	CDiskInfoContactList	*m_pMenuList2;			//�˵�2�б�
	CDiskInfoContactList	*m_pMenuList3;			//�˵�3�б�
	CDiskInfoContactList	*m_pMenuList4;			//�˵�4�б�
};

}

#endif

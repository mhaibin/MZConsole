/*
 * �ļ����ƣ�RegisterWnd.h
 * ��    ��������ע����֤
 * ��ǰ�汾��1.0
 * ��    �ߣ�mhaibin
 * �������ڣ�2017.9.25
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
	//��ʼ�����к��б�
	void InitDiskSeriallist();
	void OnDropDown(TNotifyUI& msg);
	//��EditComb�б���ɾ��һ��
	void OnDeleteResolutionWHItem(TNotifyUI& msg);
	//��EditComb�б���ѡ��һ��
	void OnSeleteResolutionWHItem(TNotifyUI& msg);

	//���ȷ����ť
	void OnClickBtnOK();
	//�����ȡ��ť
	void OnClickBtnGet();
	//����IO������
	void OnClickBtnUserIO();

private:
	CResolutionWHListFrame			*m_pDiskSerial;		//�������к�
	CEditCombUI						*m_pecbDiskSerial;	//�������к�
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

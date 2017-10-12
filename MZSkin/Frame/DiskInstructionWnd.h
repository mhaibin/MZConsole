/*
 * �ļ����ƣ�AddServerWnd.h
 * ��    �������̹�����
 * ��ǰ�汾��1.0
 * ��    �ߣ�mhaibin
 * �������ڣ�2017.9.4
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
	UINT8			m_u8Type;			//��������
	CString			m_strImgPath;		//�����ļ�
	CString			m_strSrcPath;		//Ŀ���ļ�
	CString			m_strServerName;	//����������
	DWORD			m_dwServerIP;		//������IP
	UINT32			m_u32Size;			//���ݴ�С;
};

}

#endif

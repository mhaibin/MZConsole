#ifndef __KK_TRAY_MSGBOX_FRAME_HH__
#define __KK_TRAY_MSGBOX_FRAME_HH__

#include "UIlib.h"
#include <Tray/traypos.h>
#include <map>
#include <sigslot.h>


namespace DuiLib{

typedef enum MyEnum
{
	UNKNOWN_ICON,			//δ֪ͼ��
	DEFAULT_NORMAL_ICON,	//Ĭ������ͼ��
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
	////////////////////////////////dui�������//////////////////////////////////////////
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
	//////////////////////////////////�¼�////////////////////////////////////////
	//��ʱ��
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//������Ϣ
	LRESULT OnTrayMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//���̽������
	LRESULT OnMouseMoveT(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//���̽����뿪
	LRESULT OnMouseLeaveT(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//���������
	LRESULT OnLButtonDownT(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//����Ҽ�����
	LRESULT OnRButtonDownT(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	//��ȡ��ǰ������ʾͼ��
	EC_TRAY_ICON GetCurrentIcon();
	//��ʾ����
	void ShowTrayMsgboxWnd();
	//��ȡ�������������ھ��
	HWND FindTrayWnd(); 
	//���ش���
	void HideTrayMsgboxWnd();
	//��ȡ��Ҫ�ı�Ĵ��ڴ�С
	SIZE GetChangeWndSize();
	//��ȡ��Ҫ�ı�Ĵ���λ�ã�û�м����ڸ߶ȵ�λ�ã�
	POINT GetChangeWndPos();
	//��ȡ���������ڵ�λ����Ϣ
	RECT GetTrayWndRect(); 
	//���¿ؼ�
	void OnClick(TNotifyUI& msg);
	//�����ؽ�
	LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
public:
	/////////////////////////////////�ⲿ�ӿ�/////////////////////////////////////////
	//���ʼ��
	void Init(LPCTSTR szToolTip, ITrayEvent * pTrayEvent, std::map<EC_TRAY_ICON, HICON> mapIcon);
    //�޸�����ͼ���ı�
    void SetTooltipText(LPCTSTR szToolTip);

	//�ر�
	void CloseTrayMsgboxWnd();

protected:
	BOOL m_bIsHoverMsgBox; //����Ƿ������Ϣ����
	BOOL m_bIsHoverTray; //�������Ƿ��������
	BOOL m_bTrayFlashFlag;//�Ƿ��л�ͼƬͼ��
	CMsgTrayPos * m_pTrayPos;//���̶���
	ITrayEvent * m_pTrayEvent;//�����¼�֪ͨ
	std::map<EC_TRAY_ICON, HICON> m_mapIcon;

};

}

#endif
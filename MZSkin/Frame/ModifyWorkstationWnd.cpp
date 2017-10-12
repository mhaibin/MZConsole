#include "StdAfx.h"
#include "ModifyWorkstationWnd.h"
#include <ContactList\ConfigContactList.h>
#include "..\Global\Control\EditCombUI.h"
#include <SkinInner/SkinAPI.h>
#include "../Global/Common/Global.h"
namespace DuiLib{
	

CModifyWorkstationWnd::CModifyWorkstationWnd()
{

}

CModifyWorkstationWnd::~CModifyWorkstationWnd(void)
{

}

CDuiString CModifyWorkstationWnd::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CModifyWorkstationWnd::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CModifyWorkstationWnd::GetSkinFile()
{
	return _T("xml\\ModifyWorkstationWnd.xml");
}

LPCTSTR CModifyWorkstationWnd::GetWindowClassName(void) const
{
	return _T("ModifyWorkstationWnd");
}

void CModifyWorkstationWnd::InitWindow()
{
	event::ExitProcessEvn().connect(this, &CModifyWorkstationWnd::ExitProcessEvn);
}

CControlUI* CModifyWorkstationWnd::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass, _T("DiskInfoContactList")) == 0)
	{
		return new CDiskInfoContactList();
	}
	else if (_tcscmp(pstrClass, _T("EditComb")) == 0)
	{
		return new CEditCombUI();
	}
	return NULL;
}

LRESULT CModifyWorkstationWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CModifyWorkstationWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CModifyWorkstationWnd::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}
void CModifyWorkstationWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString	strName = msg.pSender->GetName();
		if(0 == strName.Compare(_T("closebtn")))
		{
			Close(IDCANCEL);
		}
		else if(0 == strName.Compare(_T("minbtn")))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
	}
}
void CModifyWorkstationWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CModifyWorkstationWnd::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
}
namespace Popup
{
	void OpenModifyWorkstationWnd()
	{
		static HWND hModifyWorkstationWnd = NULL;
		if (!::IsWindow(hModifyWorkstationWnd))
		{
			CModifyWorkstationWnd * pModifyWorkstationWnd = new CModifyWorkstationWnd();
			pModifyWorkstationWnd->Create(::GetDesktopWindow(), _T("ÐÞ¸Ä¹¤×÷Õ¾"), WS_POPUPWINDOW, 0, 0, 0, 0, 0);
			pModifyWorkstationWnd->CenterWindow();
			pModifyWorkstationWnd->ShowWindow(true, false);
			hModifyWorkstationWnd = pModifyWorkstationWnd->GetHWND();
		}
		global::FrontWindow(hModifyWorkstationWnd);
	}
}
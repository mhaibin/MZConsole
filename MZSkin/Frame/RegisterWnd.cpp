#include "StdAfx.h"
#include "RegisterWnd.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <SkinInner/SkinAPI.h>
#include "..\Global\Control\EditCombUI.h"
#include "..\Global\Common\Global.h"
#include "..\Server\mzdio\MzdIOMgr.h"

namespace DuiLib{
	

CRegisterWnd::CRegisterWnd()
{
	m_pDiskSerial = NULL;
	m_pecbDiskSerial = NULL;
	m_EdtPasWord = NULL;
	m_EdtSecurity = NULL;
	m_LabTips = NULL;
	m_LabSerialTips = NULL;
	m_LabPassTips = NULL;
	m_LabPasswordTips = NULL;
	m_LabSecurityTips = NULL;
}

CRegisterWnd::~CRegisterWnd(void)
{

}

CDuiString CRegisterWnd::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CRegisterWnd::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CRegisterWnd::GetSkinFile()
{
	return _T("xml\\RegisterWnd.xml");
}

LPCTSTR CRegisterWnd::GetWindowClassName(void) const
{
	return _T("RegisterWnd");
}

void CRegisterWnd::InitWindow()
{
	InitDiskSeriallist();

	m_pecbDiskSerial = static_cast<CEditCombUI*>(m_PaintManager.FindControl(_T("ecd_diskSerial")));
	m_EdtPass = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_pass")));
	m_EdtPasWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_password")));
	m_EdtSecurity = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_security")));
	m_chbUndisplay = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_undisplay")));
	m_LabTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_tips")));

	m_LabSerialTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_SerialTips")));
	m_LabPassTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_PassTips")));
	m_LabPasswordTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_PasswordTips")));
	m_LabSecurityTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_SecurityTips")));

	event::ExitProcessEvn().connect(this, &CRegisterWnd::ExitProcessEvn);
}

CControlUI* CRegisterWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("EditComb")) == 0)
	{
		return new CEditCombUI();
	}
	return NULL;
}

LRESULT CRegisterWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CRegisterWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CRegisterWnd::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}
void CRegisterWnd::Notify(TNotifyUI& msg)
{
	CDuiString	strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == strName.Compare(_T("closebtn")))
		{
			Close(IDCANCEL);
		}
		else if(0 == strName.Compare(_T("minbtn")))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if(0 == strName.Compare(_T("btn_ok")))
		{
			OnClickBtnOK();
		}
		else if(0 == strName.Compare(_T("btn_security")))
		{
			OnClickBtnGet();
		}
		else if(0 == strName.Compare(_T("btn_userio")))
		{
			OnClickBtnUserIO();
		}
		else if(0 == strName.Compare(_T("btn_reg")))
		{
			ShellExecute(NULL, _T("open"), _T("http://mzd.mz99.com/Account/Register.aspx"), NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else if (msg.sType == _T("deleteCombItem")){
		OnDeleteResolutionWHItem(msg);
	}
	else if (msg.sType == _T("selectCombItem")){
		OnSeleteResolutionWHItem(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_DROPDOWN){
		OnDropDown(msg);
	}
	else if(msg.sType == DUI_MSGTYPE_SETFOCUS){
		if(0 == strName.Compare(_T("ecd_diskSerial")))
		{
			m_LabSerialTips->SetVisible(false);
		}
		else if(0 == strName.Compare(_T("edt_pass")))
		{
			m_LabPassTips->SetVisible(false);
		}
		else if(0 == strName.Compare(_T("edt_password")))
		{
			m_LabPasswordTips->SetVisible(false);
		}
		else if(0 == strName.Compare(_T("edt_security")))
		{
			m_LabSecurityTips->SetVisible(false);
		}
	}
}
void CRegisterWnd::OnDropDown(TNotifyUI& msg)
{
	if(0 == msg.pSender->GetName().Compare(_T("ecd_diskSerial")))
		m_pDiskSerial->Init(msg.pSender, CDuiString(_T("ecd_diskSerial")));
}

void CRegisterWnd::OnDeleteResolutionWHItem(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if(0 == msg.pSender->GetParent()->GetText().Compare(_T("ecd_diskSerial")))
	{	
		m_pDiskSerial->RemoveItem(strName);
	}
}

void CRegisterWnd::OnSeleteResolutionWHItem(TNotifyUI& msg)
{
	CDuiString szText = msg.pSender->GetText();
	if(0 == msg.pSender->GetParent()->GetText().Compare(_T("ecd_diskSerial")))
	{
		CEditCombUI *pEditCombDiskSerial = static_cast<CEditCombUI*>(m_PaintManager.FindControl(_T("ecd_diskSerial")));
		pEditCombDiskSerial->SetText(szText);
		m_LabSerialTips->SetVisible(false);
	}
}
void CRegisterWnd::OnClickBtnOK()
{
	CString strDiskSerial, strPass, strPassword, strSecurity;
	strDiskSerial = m_pecbDiskSerial->GetText();
	strPass = m_EdtPass->GetText();
	strPassword = m_EdtPasWord->GetText();
	strSecurity = m_EdtSecurity->GetText();

	if(strDiskSerial.IsEmpty())
	{
		//提示请输入磁盘序列号
		m_LabSerialTips->SetVisible(true);
		return;
	}
	if(strPass.IsEmpty())
	{
		//提示请输入通行证
		m_LabPassTips->SetVisible(true);
		return;
	}
	if(strPassword.IsEmpty())
	{
		//提示请输入通行证密码
		m_LabPasswordTips->SetVisible(true);
		return;
	}
	if(strSecurity.IsEmpty())
	{
		//提示验证码为空
		m_LabSecurityTips->SetVisible(true);
		return;
	}
	CString strTips;
	INT32 nRel = Singleton<CMzdIOMgr>::Instance().Register(strDiskSerial, strPass, strPassword, strSecurity, strTips);
	m_LabTips->SetText(strTips);

}
void CRegisterWnd::OnClickBtnGet()
{
	CString strDiskSerial, strPass, strPassword, strSecurity;
	strDiskSerial = m_pecbDiskSerial->GetText();
	strPass = m_EdtPass->GetText();
	strPassword = m_EdtPasWord->GetText();
	strSecurity = m_EdtSecurity->GetText();

	if(strDiskSerial.IsEmpty())
	{
		//提示请输入磁盘序列号
		m_LabSerialTips->SetVisible(true);
		return;
	}
	if(strPass.IsEmpty())
	{
		//提示请输入通行证
		m_LabPassTips->SetVisible(true);
		return;
	}
	if(strPassword.IsEmpty())
	{
		//提示请输入通行证密码
		m_LabPasswordTips->SetVisible(true);
		return;
	}
	CString strTips;
	INT32 nRel = Singleton<CMzdIOMgr>::Instance().Register(strDiskSerial, strPass, strPassword, strSecurity, strTips);
	m_LabTips->SetText(strTips);
}
void CRegisterWnd::OnClickBtnUserIO()
{
	INT32 nRel = Singleton<CMzdIOMgr>::Instance().UserIOServer();
	if(0 > nRel)
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]设置用做IO服务器操作失败<Rel=%d>\r\n"), nRel);
	}
}
void CRegisterWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CRegisterWnd::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
void CRegisterWnd::InitDiskSeriallist()
{
	m_pDiskSerial = new CResolutionWHListFrame();
	m_pDiskSerial->Create(m_hWnd, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	m_pDiskSerial->ShowWindow(false);

	vector<CString> vecDiskSerial;
	Singleton<CMzdIOMgr>::Instance().GetDiskSerial(vecDiskSerial);

	INT32 nSize = vecDiskSerial.size();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		m_pDiskSerial->AddItem(vecDiskSerial[nIndex]);
	}
}
}
namespace Popup
{
	void OpenRegisterWnd()
	{
		static HWND hRegisterWnd = NULL;
		if (!::IsWindow(hRegisterWnd))
		{
			CRegisterWnd * pRegisterWnd = new CRegisterWnd();
			pRegisterWnd->Create(::GetDesktopWindow(), _T("registerwnd"), WS_POPUPWINDOW, 0, 0, 0, 0, 0);		
			pRegisterWnd->CenterWindow();
			pRegisterWnd->ShowWindow(true, false);
			hRegisterWnd = pRegisterWnd->GetHWND();
		}
		global::FrontWindow(hRegisterWnd);
	}
}
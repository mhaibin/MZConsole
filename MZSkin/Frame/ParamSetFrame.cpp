#include "StdAfx.h"
#include "ParamSetFrame.h"
#include "UtilPath.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <SkinInner/SkinAPI.h>

namespace DuiLib{
	

CParamSetFrame::CParamSetFrame()
{
	m_pTabParamSet = NULL;
	m_pDownLoadSetUI = NULL;
}

CParamSetFrame::~CParamSetFrame(void)
{

}

CDuiString CParamSetFrame::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CParamSetFrame::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CParamSetFrame::GetSkinFile()
{
	return _T("xml\\ParamSetFrame.xml");
}

LPCTSTR CParamSetFrame::GetWindowClassName(void) const
{
	return _T("ParamSetClassFrame");
}

void CParamSetFrame::InitWindow()
{
	m_pTabParamSet = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("tab_paramset")));
	m_pDownLoadSetUI = static_cast<CDownLoadSetLayoutUI *>(m_PaintManager.FindControl(_T("downloadset")));
	//加载bt设置界面
	CString strDllPath;
	CString strExePath = Util::Path::GetExeFolder();
	strExePath = Util::Path::GetParentDirPath(strExePath);
	strDllPath = strExePath + _T("\\Module\\btsvc_UI.dll");
	m_hLibFlag = LoadLibrary(strDllPath);
	if(NULL == m_hLibFlag)
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]加载btsvc_UI.dll库失败\r\n"));
	}
	else
	{
		m_BTCreateChild = (_btuiCreateChild)GetProcAddress(m_hLibFlag, "CreateChild");	
		if(m_BTCreateChild)
		{
			m_BTCreateChild(m_hWnd, 0, 0, 0, 0, &m_BTSetLabel, 0x804);
		}
		else
		{
			Util::Log::Error(_T("MZSkin"), _T("[error]获取btsvc_UI.dll库的CreateChild函数指针出错\r\n"));
		}

		if(0 < m_BTSetLabel.dwCount)
		{
			m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[0], TRUE);
		}
	}

	event::ExitProcessEvn().connect(this, &CParamSetFrame::ExitProcessEvn);
}

CControlUI* CParamSetFrame::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass, _T("DownLoadSetLayout")) == 0)
	{
		return new CDownLoadSetLayoutUI();
	}
	return NULL;
}
LRESULT CParamSetFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_TIMER == uMsg)
	{
		OnTimer(wParam, lParam, bHandled);
	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CParamSetFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CParamSetFrame::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}
void CParamSetFrame::Notify(TNotifyUI& msg)
{
	CDuiString	strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{

	}
	else if(msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if(0 == strName.Compare(_T("Option01")))
		{
			if(0 < m_BTSetLabel.dwCount)
			{
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[1], FALSE);
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[2], FALSE);
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[0], TRUE);
			}
		}
		else if(0 == strName.Compare(_T("Option02")))
		{
			if(0 < m_BTSetLabel.dwCount)
			{
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[0], FALSE);
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[2], FALSE);
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[1], TRUE);
			}
		}
		else if(0 == strName.Compare(_T("Option03")))
		{
			if(0 < m_BTSetLabel.dwCount)
			{
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[0], FALSE);
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[1], FALSE);
				m_pDownLoadSetUI->SetShowInfo(m_BTSetLabel.Child[2], TRUE);
			}
		}
	}
}
void CParamSetFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CParamSetFrame::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
}
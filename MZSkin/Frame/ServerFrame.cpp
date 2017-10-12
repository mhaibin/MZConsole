#include "StdAfx.h"
#include "ServerFrame.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <Global\Control\FrontButtonUI.h>
#include <SkinInner/SkinAPI.h>
#include <SharedUI/Interface/SharedBase.h>
#include "..\Server\mzdio\MzdIOMgr.h"
#include <SharedUI/Frame/ServerTreeUI.h>

#define STATUSTIMER 1000
#define UPDATESTATUSLEN 3 * 60 * 1000

namespace DuiLib{
	

CServerFrame::CServerFrame()
{
	m_pList = NULL;
	m_pElement = NULL;
}

CServerFrame::~CServerFrame(void)
{
	::KillTimer(m_hWnd, STATUSTIMER);
}

CDuiString CServerFrame::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CServerFrame::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CServerFrame::GetSkinFile()
{
	return _T("xml\\ServerFrame.xml");
}

LPCTSTR CServerFrame::GetWindowClassName(void) const
{
	return _T("ServerClassFrame");
}

void CServerFrame::InitWindow()
{
	//获取服务器的状态
	map<CString, UINT32> mapStatus;
	Singleton<CMzdIOMgr>::Instance().GetServerStatus(mapStatus);
	map<CString, UINT32>::iterator itEnd = mapStatus.end();
	

	//将服务器添加到列表中
	map<CString, ServerItem> mapServer;
	m_pList = static_cast<CServerContactList*>(m_PaintManager.FindControl(_T("lt_server")));
	CServerTreeUI *pSvrList = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_serverlist")));
	CTreeNodeUI *pNode = pSvrList->AddGroupNodeLab(NULL, _T("所有服务器"), FALSE);
	Singleton<CMzdIOMgr>::Instance().GetAllServer(mapServer);
	map<CString, ServerItem>::iterator itor = mapServer.begin();
	while(itor != mapServer.end())
	{
		map<CString, UINT32>::iterator itFind = mapStatus.find(itor->first);
		if(itFind != itEnd)
		{
			itor->second.u8Status = (UINT8)(itFind->second);
		}

		m_pList->InsertItem(itor->first, itor->second);
		pSvrList->AddChildNodeLab(pNode, itor->first, itor->first, FALSE);
		itor++;
	}
	m_pElement = (CListContainerElementUI *)(m_pList->GetItemAt(0));

	::SetTimer(m_hWnd, STATUSTIMER, UPDATESTATUSLEN, NULL);
	event::ExitProcessEvn().connect(this, &CServerFrame::ExitProcessEvn);
}

CControlUI* CServerFrame::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass, _T("FrontButton")) == 0)
	{
		return new CFrontButtonUI();
	}
	else if(_tcscmp(pstrClass, _T("ServerContactList")) == 0)
	{
		return new CServerContactList();
	}
	else if(_tcscmp(pstrClass, _T("PropertyContactList")) == 0)
	{
		return new CPropertyContactList();
	}
	else if(_tcscmp(pstrClass, _T("StorageUseContactList")) == 0)
	{
		return new CStorageUseContactList();
	}
	else if(_tcscmp(pstrClass, _T("ServerTree")) == 0)
	{
		return new CServerTreeUI();
	}
	return NULL;
}

LRESULT CServerFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(UW_LOADSERVER_MSG == uMsg)
	{
		BOOL bOk = Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
		if(FALSE == bOk)
		{
			Util::Log::Error(_T("MZSkin"), _T("[error]更新服务器信息数据时，发送同步服务器数据命令出错\r\n"));
			return FALSE;
		}
		Singleton<CMzdIOMgr>::Instance().ServerLoadData();

		map<CString, UINT32> mapStatus;
		Singleton<CMzdIOMgr>::Instance().GetServerStatus(mapStatus);
		map<CString, UINT32>::iterator itEnd = mapStatus.end();

		map<CString, ServerItem> mapServer;
		m_pList = static_cast<CServerContactList*>(m_PaintManager.FindControl(_T("lt_server")));
		m_pList->RemoveAll();
		CServerTreeUI *pSvrList = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_serverlist")));
		pSvrList->RemoveAll();
		CTreeNodeUI *pNode = pSvrList->AddGroupNodeLab(NULL, _T("所有服务器"), FALSE);
		Singleton<CMzdIOMgr>::Instance().GetAllServer(mapServer);
		map<CString, ServerItem>::iterator itor = mapServer.begin();
		while(itor != mapServer.end())
		{
			map<CString, UINT32>::iterator itFind = mapStatus.find(itor->first);
			if(itFind != itEnd)
			{
				itor->second.u8Status = (UINT8)(itFind->second);
			}
			m_pList->InsertItem(itor->first, itor->second);
			pSvrList->AddChildNodeLab(pNode, itor->first, itor->first, FALSE);
			itor++;
		}
		m_pElement = (CListContainerElementUI *)(m_pList->GetItemAt(0));

	}
	else if(WM_TIMER == uMsg)
	{
		OnTimer(wParam, lParam, bHandled);
	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CServerFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CServerFrame::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if(STATUSTIMER == wParam)
	{
		OnUpdateStatus();
	}
    return 0;
}
void CServerFrame::Notify(TNotifyUI& msg)
{
	CDuiString	strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == strName.Compare(_T("bt_add")))
		{
			Popup::OpenAddServerWnd(m_hWnd, CString(_T("")));
		}
		else if(0 == strName.Compare(_T("bt_mdi")))
		{
			if(m_pElement)
			{
				CLabelUI *pLabName = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_name")));
				CDuiString strName = pLabName->GetText();
				Popup::OpenAddServerWnd(m_hWnd, strName.GetData());
			}
		}
		else if(0 == strName.Compare(_T("bt_del")))
		{
			OnDel();//post一个自定义消息
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if(0 == strName.Compare(_T("lt_server")))
		{
			m_pElement = (CListContainerElementUI *)(m_pList->GetItemAt(m_pList->GetCurSel()));
		}
	}
}
void CServerFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CServerFrame::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
void CServerFrame::OnDel()
{
	//提示确定要删除服务器吗？
	SIZE size;
	size.cx = 50;
	size.cy = 40;
	if(m_pElement)
	{
		if (IDOK != box::ShowModal(m_PaintManager.GetPaintWindow(), _T("提示"), _T("确定要删除选中的服务器吗？"), _T("确定"), _T(""), _T(""), _T("material//general//warning.png"), size))
		{
			return;
		}
		CLabelUI *pLabName = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_name")));
		CDuiString strName = pLabName->GetText();
		INT32 nVel = Singleton<CMzdIOMgr>::Instance().DelServer(strName);
		if(-1 == nVel)//该服务器存在相关的磁盘设置！请先删除磁盘设置！
		{
			box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("该服务器存在相关的磁盘设置！请先删除磁盘设置！"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
		}
		else
		{
			if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //同步INI
			{
				Util::Log::Error(_T("MZSkin"), _T("[error]删除服务器的时发送同步命令出错\r\n"));
				return;
			}
			::PostMessage(m_hWnd, UW_LOADSERVER_MSG, NULL, NULL);
		}
	}
	else
	{
		//提示没有选中要删除的服务器
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("选择要删除的服务器"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
	}
}
void CServerFrame::OnUpdateStatus()
{
	BOOL bOk = Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
	if(FALSE == bOk)
		return;

	map<CString, UINT32> mapStatus;
	Singleton<CMzdIOMgr>::Instance().GetServerStatus(mapStatus);
	map<CString, UINT32>::iterator itor = mapStatus.begin();

	while(itor != mapStatus.end())
	{
		ServerItem item;
		m_pList->GetItem(itor->first, item);
		item.u8Status = (UINT8)(itor->second);
		m_pList->SetItem(itor->first, item);
		itor++;
	}
}
}
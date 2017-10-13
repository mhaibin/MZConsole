#include "StdAfx.h"
#include "DiskMtFrame.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <Global\Control\FrontButtonUI.h>
#include <ContactList\CompContactList.h>
#include <SkinInner/SkinAPI.h>
#include "..\Server\mzdio\MzdIOMgr.h"
#include <SharedUI/Frame/ServerTreeUI.h>
#include "..\Server\DiskInfoMgr.h"
#include "..\SharedUI\Interface\SharedBase.h"

namespace DuiLib{
	

CDiskMtFrame::CDiskMtFrame()
{

}

CDiskMtFrame::~CDiskMtFrame(void)
{

}

CDuiString CDiskMtFrame::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CDiskMtFrame::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CDiskMtFrame::GetSkinFile()
{
	return _T("xml\\DiskMtFrame.xml");
}

LPCTSTR CDiskMtFrame::GetWindowClassName(void) const
{
	return _T("DiskMtClassFrame");
}

void CDiskMtFrame::InitWindow()
{
	//��ʼ��������б�
	map<CString, vector<DiskInfoItem>> mapDisk;
	CDiskInfoUpContactList *pList = static_cast<CDiskInfoUpContactList*>(m_PaintManager.FindControl(_T("lt_diskinfoup")));
	Singleton<CMzdIOMgr>::Instance().GetAllDisk(mapDisk);
	map<CString, vector<DiskInfoItem>>::iterator itor = mapDisk.begin();
	while(itor != mapDisk.end())
	{
		INT32 nSize = itor->second.size();
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			pList->InsertItem(itor->second[nIndex].strDiskPath, itor->second[nIndex]);
		}
		itor++;
	}
	map<CString, ServerItem> mapServer;
	CServerTreeUI *pSvrList = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_serverlist")));
	CTreeNodeUI *pNode = pSvrList->AddGroupNodeLab(NULL, _T("���з�����"), FALSE);
	Singleton<CMzdIOMgr>::Instance().GetAllServer(mapServer);
	map<CString, ServerItem>::iterator itor1 = mapServer.begin();
	while(itor1 != mapServer.end())
	{
		pSvrList->AddChildNodeLab(pNode, itor1->first, itor1->first, FALSE);
		itor1++;
	}
	
	//��ʼ��������б�
	CDiskInfoDownContactList *pListDown = static_cast<CDiskInfoDownContactList*>(m_PaintManager.FindControl(_T("lt_diskinfodown")));
	pNode = pNode->GetChildNode(0);
	if(pNode)
	{
		INT32 nCount = pNode->GetCount();
		CString strDefServer;
		if(nCount > 0)
		{
			for (INT32 nIndex=0; nIndex<nCount; nIndex++)
			{
				CControlUI *pControl = pNode->GetItemAt(nIndex);
				if(0 <= pControl->GetName().Find(_T("labc_")))
				{
					strDefServer.Format(_T("%s"), pControl->GetText());
				}
			}
		}
		if(!strDefServer.IsEmpty())
		{
			vector<DiskInfoItem> vecItem;
			Singleton<CMzdIOMgr>::Instance().GetIMGDiskInfo(strDefServer, vecItem);
			INT32 nSize = vecItem.size();
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				pListDown->InsertItem(vecItem[nIndex].strDiskPath, vecItem[nIndex]);
			}
		}
	}
	event::ExitProcessEvn().connect(this, &CDiskMtFrame::ExitProcessEvn);
}

CControlUI* CDiskMtFrame::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass, _T("FrontButton")) == 0)
	{
		return new CFrontButtonUI();
	}
	else if(_tcscmp(pstrClass, _T("DiskInfoUpContactList")) == 0)
	{
		return new CDiskInfoUpContactList();
	}
	else if(_tcscmp(pstrClass, _T("DiskInfoDownContactList")) == 0)
	{
		return new CDiskInfoDownContactList();
	}
	else if(_tcscmp(pstrClass, _T("ServerTree")) == 0)
	{
		return new CServerTreeUI();
	}
	return NULL;
}

LRESULT CDiskMtFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_LOADDISK_MSG == uMsg)
	{
		BOOL bOk = Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
		if(FALSE == bOk)
		{
			Util::Log::Error(_T("MZSkin"), _T("[error]ͬ����������������ش���(WM_LOADDISK_MSG)\r\n"));
			return FALSE;
		}
		Singleton<CMzdIOMgr>::Instance().DiskLoadData();//ע��ͬ���˷����������ݣ����øú���ʹ�������ݸ���ͬ������������ص�����һ����
		map<CString, vector<DiskInfoItem>> mapDisk;
		CDiskInfoUpContactList *pList = static_cast<CDiskInfoUpContactList*>(m_PaintManager.FindControl(_T("lt_diskinfoup")));
		pList->RemoveAll();
		Singleton<CMzdIOMgr>::Instance().GetAllDisk(mapDisk);
		map<CString, vector<DiskInfoItem>>::iterator itor = mapDisk.begin();
		while(itor != mapDisk.end())
		{
			INT32 nSize = itor->second.size();
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				pList->InsertItem(itor->second[nIndex].strDiskPath, itor->second[nIndex]);
			}
			itor++;
		}

	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CDiskMtFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CDiskMtFrame::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}
void CDiskMtFrame::Notify(TNotifyUI& msg)
{
	CDuiString	strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == strName.Compare(_T("bt_update"))
			|| 0 == strName.Compare(_T("bt_produce"))
			|| 0 == strName.Compare(_T("bt_exp"))
			|| 0 == strName.Compare(_T("bt_create")))
			OnToolBarClick(msg);
		else if(0 == strName.Compare(_T("bt_del")))
		{
			OnDel();//Postһ���Զ�����Ϣ
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		OnItemSelect(msg);
	}
}
void CDiskMtFrame::OnItemSelect(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if(0 == strName.Compare(_T("lt_diskinfodown")))
		return;
	if(0 <= strName.Find(_T("ndc_")))
	{
		CTreeNodeUI *pNode = (CTreeNodeUI *)msg.pSender;
		INT32 nSize = pNode->GetCount();
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			CControlUI *pControl = pNode->GetItemAt(nIndex);
			CDuiString strControlName = pControl->GetName();
			if(0 <= strControlName.Find(_T("labc_")))
			{
				CString strSvr = pControl->GetText();
				CDiskInfoDownContactList *pListDown = static_cast<CDiskInfoDownContactList*>(m_PaintManager.FindControl(_T("lt_diskinfodown")));
				pListDown->RemoveAll();
				vector<DiskInfoItem> vecItem;
				Singleton<CMzdIOMgr>::Instance().GetIMGDiskInfo(strSvr, vecItem);
				INT32 nSize = vecItem.size();
				for (INT32 nIndex=0; nIndex<nSize; nIndex++)
				{
					pListDown->InsertItem(vecItem[nIndex].strDiskPath, vecItem[nIndex]);
				}
			}
		}
	}
	else if(0 <= strName.Compare(_T("diskinfoupitem")))
	{
		CDiskInfoUpContactList *pList = static_cast<CDiskInfoUpContactList*>(m_PaintManager.FindControl(_T("lt_diskinfoup")));
		CListContainerElementUI *pElement = (CListContainerElementUI *)(pList->GetItemAt(pList->GetCurSel()));
		if(pElement)
		{
			CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
			CString strName = pName->GetText();
			CDiskInfoDownContactList *pListDown = static_cast<CDiskInfoDownContactList*>(m_PaintManager.FindControl(_T("lt_diskinfodown")));
			pListDown->RemoveAll();
			vector<DiskInfoItem> vecItem;
			Singleton<CMzdIOMgr>::Instance().GetIMGDiskInfo(strName, vecItem);
			INT32 nSize = vecItem.size();
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				pListDown->InsertItem(vecItem[nIndex].strDiskPath, vecItem[nIndex]);
			}
		}
	}
}
void CDiskMtFrame::OnToolBarClick(TNotifyUI& msg)
{
	SIZE size;
	size.cx = 50;
	size.cy = 40;
	CDuiString	strName = msg.pSender->GetName();
	CDiskInfoDownContactList *pListDown = static_cast<CDiskInfoDownContactList*>(m_PaintManager.FindControl(_T("lt_diskinfodown")));
	CListContainerElementUI *pElement = (CListContainerElementUI *)(pListDown->GetItemAt(pListDown->GetCurSel()));
	if(pElement)
	{
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pDSPath = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dspath")));
		CLabelUI *pDSFormat = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dsformat")));
		CLabelUI *pDSSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dssize")));
		CLabelUI *pMDTime = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mdtime")));
		CLabelUI *pRemark = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_remark")));
		vector<DiskInfoItem> vecItem;
		DiskInfoItem item;
		item.strName = pName->GetText();
		item.strDiskPath = pDSPath->GetText();
		item.strDiskType = pDSFormat->GetText();
		item.strSize = pDSSize->GetText();
		item.strModifyTime = pMDTime->GetText();
		item.strRemark = pRemark->GetText();
		vecItem.push_back(item);
		Singleton<CDiskInfoMgr>::Instance().SetCurSelDiskInfo(vecItem);
		if(0 == strName.Compare(_T("bt_update")))
		{
			if(0 < item.strDiskPath.Find(_T(".Upd")))
				Popup::OpenDiskInstructionWnd(0);
			else
			{
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("��ѡ��Upd�ļ����͸��´���"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
			}
		}
		if(0 == strName.Compare(_T("bt_produce")))
		{
			if(0 < item.strDiskPath.Find(_T(".Upd")))
				Popup::OpenDiskInstructionWnd(1);
			else
			{
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("��ѡ��Upd�ļ��������ɴ���"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
			}
		}
		if(0 == strName.Compare(_T("bt_exp")))
		{
			if(0 < item.strDiskPath.Find(_T(".img")))
				Popup::OpenDiskInstructionWnd(3);
			else
			{
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("��ѡ��img�ļ��������ݴ���"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
			}
		}
	}
	if(0 == strName.Compare(_T("bt_create")))
	{
		Popup::OpenDiskInstructionWnd(2);
	}
	else if(NULL == pElement)
	{
		CString strTips;
		if(0 == strName.Compare(_T("bt_update")))
		{
			strTips = _T("��ѡ������Ҫ���´��̵���Ŀ");
		}
		else if(0 == strName.Compare(_T("bt_produce")))
		{
			strTips = _T("��ѡ������Ҫ���ɴ��̵���Ŀ");
		}
		else if(0 == strName.Compare(_T("bt_exp")))
		{
			strTips = _T("��ѡ����Ҫ���ݴ��̵���Ŀ");
		}
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), strTips, NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);

	}
}
void CDiskMtFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CDiskMtFrame::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
void CDiskMtFrame::OnDel()
{
	//��ʾ�Ƿ�Ҫɾ������
	SIZE size;
	size.cx = 50;
	size.cy = 40;
	CDiskInfoDownContactList *pListDown = static_cast<CDiskInfoDownContactList*>(m_PaintManager.FindControl(_T("lt_diskinfodown")));
	CListContainerElementUI *pElement = (CListContainerElementUI *)(pListDown->GetItemAt(pListDown->GetCurSel()));
	if(pElement)
	{
		if (IDOK != box::ShowModal(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("ȷ��ɾ��ѡ�еĴ�����"), _T("ȷ��"), _T(""), _T(""), _T("material//general//warning.png"), size))
		{
			return;
		}
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pDSPath = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dspath")));
		CString strName, strPath;
		strName = pName->GetText();
		strPath = pDSPath->GetText();
		INT32 nVel = Singleton<CMzdIOMgr>::Instance().DelDisk(strName, strPath);
		if(-1 == nVel)
		{
			//��ʾ ѡ���Ĵ����ڹ���վ�˵�����ʹ�� ,����ɾ����
			box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("ѡ���Ĵ����ڹ���վ�˵���ʹ�ã����ܱ�ɾ��"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		}
		else
		{
			if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
			{
//				Util::Log::Error(_T("MZSkin"), _T("[error]ɾ������ʱ�����͸��·����������������(\r\n"));
//				return;
			}
			::PostMessage(m_hWnd, WM_LOADDISK_MSG, NULL, NULL);
		}
	}
	else
	{
		//��ʾ û��ѡ��Ҫɾ���Ĵ���
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("��ѡ��Ҫɾ���Ĵ���"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
	}
}
}
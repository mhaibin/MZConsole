#include "StdAfx.h"
#include "AddServerWnd.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <ContactList\ConfigContactList.h>
#include "..\Global\Control\EditCombUI.h"
#include <SharedUI/Interface/SharedBase.h>
#include <SkinInner/SkinAPI.h>
#include "../Global/Common/Global.h"
#include <SharedUI/Frame/ServerTreeUI.h>
#include "..\Server\mzdio\MzdIOMgr.h"
namespace DuiLib{
	

CAddServerWnd::CAddServerWnd()
{
	m_pSpareSvr = NULL;
}

CAddServerWnd::~CAddServerWnd(void)
{
	if(m_pSpareSvr)
	{
		delete m_pSpareSvr;
		m_pSpareSvr = NULL;
	}
}

CDuiString CAddServerWnd::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CAddServerWnd::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CAddServerWnd::GetSkinFile()
{
	return _T("xml\\AddServerWnd.xml");
}

LPCTSTR CAddServerWnd::GetWindowClassName(void) const
{
	return _T("AddServerWnd");
}

void CAddServerWnd::InitWindow()
{
	CLabelUI *pLabSvrName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("edt_servername")));
	CComboUI *pCombSpareServer = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comb_spareserver")));
	CEditUI *pEditSysSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_syssize")));
	CEditUI *pEditDataSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_datasize")));
	CEditUI *pEditSSDSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_ssdsize")));

	//如果是修改服务器，则要获取服务器的信息
	ServerItem item;
	if(!m_strServerName.IsEmpty())
	{
		Singleton<CMzdIOMgr>::Instance().GetSimpleServer(m_strServerName, item);
	}
	

	InitSpareServerlist();
	//获取服务器信息，并添加到界面列表中；
	map<CString, vector<s_ServerIPInfo>> mapIOServer;
	Singleton<CMzdIOMgr>::Instance().GetIOServer(mapIOServer, CString(_T("")));
	CServerTreeUI *pTree = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_serverip")));
	map<CString, vector<s_ServerIPInfo>>::iterator itor = mapIOServer.begin();
	while(itor != mapIOServer.end())
	{
		INT32 nSize = itor->second.size();
		CTreeNodeUI *pNode = pTree->AddGroupNode(NULL, itor->first, FALSE);
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			if(0 <= item.strIP.Find(itor->second[nIndex].strIP))
			{
				itor->second[nIndex].u8Sel = 1;
			}
			pTree->AddChildNode(pNode, itor->second[nIndex].strIP, itor->second[nIndex].strIP, itor->second[nIndex].u8Sel==1?TRUE:FALSE);
		}
		itor++;
	}
	CLabelUI *pLabTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title_name")));
	if(m_strServerName.IsEmpty())
	{
		pLabTitle->SetText(_T("新增服务器"));
	}
	else
	{
		pLabTitle->SetText(_T("修改服务器"));
		CCheckBoxUI *arrayChbDisk[4];
		arrayChbDisk[0] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk1")));
		arrayChbDisk[1] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk2")));
		arrayChbDisk[2] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk3")));
		arrayChbDisk[3] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk4")));

		//获取指定服务器的信息
		ServerInfo info;
		Singleton<CMzdIOMgr>::Instance().GetServerInfo(m_strServerName, info);

		//获取回写磁盘信息，并显示在界面上
		vector<CString> vecRWriteDisk;
		Singleton<CMzdIOMgr>::Instance().GetRWriteDisk(item.strIP, vecRWriteDisk);
		INT32 nSize = vecRWriteDisk.size();
		nSize = nSize>4 ? 4 : nSize;
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			arrayChbDisk[nIndex]->SetText(vecRWriteDisk[nIndex]);
			arrayChbDisk[nIndex]->SetVisible(true);
			INT32 nVecSize = info.vecRWrite.size();
			for (INT32 nIndex2=0; nIndex2<nVecSize; nIndex2++)
			{
				if(0 <= vecRWriteDisk[nIndex].Find(info.vecRWrite[nIndex2]))
				{
					arrayChbDisk[nIndex]->SetCheck(true);
					break;
				}
			}
		}
		//将服务器信息显示在界面上
		pLabSvrName->SetText(info.strName);
		pCombSpareServer->SetText(info.strSqareServer);
		CString strSize;
		strSize.Format(_T("%u"), info.u32SysDiskSize);
		pEditSysSize->SetText(strSize);
		strSize.Format(_T("%u"), info.u32DataDiskSize);
		pEditDataSize->SetText(strSize);
		strSize.Format(_T("%u"), info.u32SSDDiskSize);
		pEditSSDSize->SetText(strSize);

	}

	event::ExitProcessEvn().connect(this, &CAddServerWnd::ExitProcessEvn);
}

CControlUI* CAddServerWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("EditComb")) == 0)
	{
		return new CEditCombUI();
	}
	else if(_tcscmp(pstrClass, _T("ServerTree")) == 0)
	{
		return new CServerTreeUI();
	}
	return NULL;
}

LRESULT CAddServerWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CAddServerWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CAddServerWnd::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}
void CAddServerWnd::Notify(TNotifyUI& msg)
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
		else if(0 == strName.Compare(_T("btn_ipfind")))
		{
			OnCheckFindServer();
		}
		else if(0 == strName.Compare(_T("btn_apply")))
			OnClickApply();
		else if(0 <= strName.Find(_T("chbNod_")))
		{
			//当现在服务器IP时显示回写路径复选框
			CCheckBoxUI *arrayChbDisk[4];
			arrayChbDisk[0] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk1")));
			arrayChbDisk[1] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk2")));
			arrayChbDisk[2] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk3")));
			arrayChbDisk[3] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk4")));
			vector<CString> vecRWriteDisk;
			Singleton<CMzdIOMgr>::Instance().GetRWriteDisk(msg.pSender->GetText(), vecRWriteDisk);
			INT32 nSize = vecRWriteDisk.size();
			nSize = nSize>4 ? 4 : nSize;
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				vecRWriteDisk[nIndex].Replace(_T("\\\\.\\"), _T(""));
				arrayChbDisk[nIndex]->SetText(vecRWriteDisk[nIndex]);
				arrayChbDisk[nIndex]->SetVisible(true);
			}

			//判断父节点是否要取消勾选

		}
		else if(0 <= strName.Find(_T("chb_")))
		{
			CTreeNodeUI *pNode = (CTreeNodeUI *)(msg.pSender->GetParent());
			CCheckBoxUI *pChbGroup = (CCheckBoxUI *)msg.pSender;
			bool bSel = pChbGroup->GetCheck();
			INT32 nSize = pNode->GetCountChild();
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				CTreeNodeUI *pChildNode = pNode->GetChildNode(nIndex);
				INT32 nChildSize = pChildNode->GetCount();
				CCheckBoxUI *pChbUI = (CCheckBoxUI *)(pChildNode->GetItemAt(1));
				pChbUI->SetCheck(!bSel);
			}
			CCheckBoxUI *arrayChbDisk[4];
			arrayChbDisk[0] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk1")));
			arrayChbDisk[1] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk2")));
			arrayChbDisk[2] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk3")));
			arrayChbDisk[3] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk4")));
			vector<CString> vecRWriteDisk;
			Singleton<CMzdIOMgr>::Instance().GetRWriteDisk(msg.pSender->GetText(), vecRWriteDisk);
			nSize = vecRWriteDisk.size();
			nSize = nSize>4 ? 4 : nSize;
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				vecRWriteDisk[nIndex].Replace(_T("\\\\.\\"), _T(""));
				arrayChbDisk[nIndex]->SetText(vecRWriteDisk[nIndex]);
				arrayChbDisk[nIndex]->SetVisible(!bSel);
			}
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
	else if(msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		OnItemClick(msg);
	}
}
void CAddServerWnd::OnItemClick(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if(0 <= strName.Find(_T("ndg_")))
	{
		CTreeNodeUI *pNode = (CTreeNodeUI *)msg.pSender;
		INT32 nSize = pNode->GetCount();
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			CControlUI *pControl = pNode->GetItemAt(nIndex);
			CDuiString strControlName = pControl->GetName();
			if(0 <= strControlName.Find(_T("chbNod_")))
			{
				CLabelUI *pLabSvrName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("edt_servername")));
				CComboUI *pCombSpareServer = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comb_spareserver")));
				CEditUI *pEditSysSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_syssize")));
				CEditUI *pEditDataSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_datasize")));
				CEditUI *pEditSSDSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_ssdsize")));

				CCheckBoxUI *arrayChbDisk[4];
				arrayChbDisk[0] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk1")));
				arrayChbDisk[1] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk2")));
				arrayChbDisk[2] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk3")));
				arrayChbDisk[3] = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk4")));

				CDuiString strText = pControl->GetText();

				ServerItem item;
				Singleton<CMzdIOMgr>::Instance().GetSimpleServer(CString(strText.GetData()), item);

				ServerInfo info;
				Singleton<CMzdIOMgr>::Instance().GetServerInfo(CString(strText.GetData()), info);

				vector<CString> vecRWriteDisk;
				Singleton<CMzdIOMgr>::Instance().GetRWriteDisk(item.strIP, vecRWriteDisk);
				INT32 nSize = vecRWriteDisk.size();
				nSize = nSize>4 ? 4 : nSize;
				for (INT32 nIndex=0; nIndex<nSize; nIndex++)
				{
					vecRWriteDisk[nIndex].Replace(_T("\\\\.\\"), _T(""));
					arrayChbDisk[nIndex]->SetText(vecRWriteDisk[nIndex]);
					arrayChbDisk[nIndex]->SetVisible(true);
					INT32 nVecSize = info.vecRWrite.size();
					for (INT32 nIndex2=0; nIndex2<nVecSize; nIndex2++)
					{
						if(0 <= vecRWriteDisk[nIndex].Find(info.vecRWrite[nIndex2]))
						{
							arrayChbDisk[nIndex]->SetCheck(true);
							break;
						}
					}
				}

				pLabSvrName->SetText(info.strName);
				pCombSpareServer->SetText(info.strSqareServer);
				CString strSize;
				strSize.Format(_T("%u"), info.u32SysDiskSize);
				pEditSysSize->SetText(strSize);
				strSize.Format(_T("%u"), info.u32DataDiskSize);
				pEditDataSize->SetText(strSize);
				strSize.Format(_T("%u"), info.u32SSDDiskSize);
				pEditSSDSize->SetText(strSize);
			}
		}
	}
}
void CAddServerWnd::OnDropDown(TNotifyUI& msg)
{
	if(0 == msg.pSender->GetName().Compare(_T("comb_spareserver")))
		m_pSpareSvr->Init(msg.pSender, CDuiString(_T("comb_spareserver")));
}

//从分辨率大小列表中删除一项
void CAddServerWnd::OnDeleteResolutionWHItem(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if(0 == msg.pSender->GetParent()->GetText().Compare(_T("comb_spareserver")))
	{	
		m_pSpareSvr->RemoveItem(strName);
	}
}
//从分辨率大小列表中选择一项
void CAddServerWnd::OnSeleteResolutionWHItem(TNotifyUI& msg)
{
	CDuiString szText = msg.pSender->GetText();
	if(0 == msg.pSender->GetParent()->GetText().Compare(_T("comb_spareserver")))
	{
		CEditCombUI *pEditCombPDI = static_cast<CEditCombUI*>(m_PaintManager.FindControl(_T("comb_spareserver")));
		pEditCombPDI->SetText(szText);
	}
}
void CAddServerWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CAddServerWnd::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
void CAddServerWnd::InitSpareServerlist()
{
	m_pSpareSvr = new CResolutionWHListFrame();
	m_pSpareSvr->Create(m_hWnd, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	m_pSpareSvr->ShowWindow(false);

	m_pSpareSvr->AddItem(_T("未使用"));

	map<CString, ServerItem> mapServer;
	Singleton<CMzdIOMgr>::Instance().GetAllServer(mapServer);
	map<CString, ServerItem>::iterator itor = mapServer.begin();
	while(itor != mapServer.end())
	{
		m_pSpareSvr->AddItem(itor->second.strName);
		itor++;
	}
	m_pSpareSvr->AddItem(_T("备服自动负载"));
	m_pSpareSvr->AddItem(_T("手动选择服务器"));
}
void CAddServerWnd::OnCheckFindServer()
{
	CIPAddressExUI *pIPFind = static_cast<CIPAddressExUI*>(m_PaintManager.FindControl(_T("edt_ipfind")));
	CDuiString strFindIP = pIPFind->GetIP();
	if(0 == strFindIP.Compare(_T("0,0,0,0")))
	{
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("查收的IP地址输入不正确!"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
		return;
	}
	map<CString, vector<s_ServerIPInfo>> mapIOServer;
	Singleton<CMzdIOMgr>::Instance().GetIOServer(mapIOServer, CString(strFindIP.GetData()));
	CServerTreeUI *pTree = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_serverip")));
	map<CString, vector<s_ServerIPInfo>>::iterator itor = mapIOServer.begin();
	while(itor != mapIOServer.end())
	{
		INT32 nSize = itor->second.size();
		CTreeNodeUI *pNode = pTree->AddGroupNode(NULL, itor->first, FALSE);
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			pTree->AddChildNode(pNode, itor->second[nIndex].strIP, itor->second[nIndex].strIP, itor->second[nIndex].u8Sel==1?TRUE:FALSE);
		}
		itor++;
	}
}
void CAddServerWnd::OnClickApply()
{
	CString strServerName, strSysSize, strDataSize, strSSDSize, strSpareServer;
	Singleton<CMzdIOMgr>::Instance().CheckUnuserServer();

	CEditUI *pEditSvrName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_servername")));
	CEditUI *pEditSysSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_syssize")));
	CEditUI *pEditDataSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_datasize")));
	CEditUI *pEditSSDSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_ssdsize")));
	CComboUI *pCombSpareServer = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comb_spareserver")));
	strServerName = pEditSvrName->GetText();
	strSysSize = pEditSysSize->GetText();
	strDataSize = pEditDataSize->GetText();
	strSSDSize = pEditSSDSize->GetText();
	strSpareServer = pCombSpareServer->GetTextEx();

	SIZE size;
	size.cx = 50;
	size.cy = 40;
	if(strServerName.IsEmpty())
	{
		//提示没有指定服务器名称
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("请输入服务器名称"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
		return;
	}
	//回写路径
	CCheckBoxUI *pChbDisk1 = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk1")));
	CCheckBoxUI *pChbDisk2 = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk2")));
	CCheckBoxUI *pChbDisk3 = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk3")));
	CCheckBoxUI *pChbDisk4 = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_disk4")));
	CDuiString strDir1, strDir2, strDir3, strDir4;
	if(pChbDisk1->GetCheck()) strDir1 = pChbDisk1->GetText();
	if(pChbDisk2->GetCheck()) strDir2 = pChbDisk2->GetText();
	if(pChbDisk3->GetCheck()) strDir3 = pChbDisk3->GetText();
	if(pChbDisk4->GetCheck()) strDir4 = pChbDisk4->GetText();
	if(strDir1.IsEmpty() && strDir2.IsEmpty() && strDir3.IsEmpty() && strDir4.IsEmpty())
	{
		//提示没有指定回写路径
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("请指定回写路径"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
		return;
	}

	//获取选择IP
	CString strSelIP;
	//目前支持两层节点，如果程序逻辑改为多层，需要使用递归处理，或者在CServerTreeUI中添加表结构
	CServerTreeUI *pTree = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_serverip")));
	INT32 nSize1 = pTree->GetCount();
	for (INT32 nIndex1=0; nIndex1<nSize1; nIndex1++)
	{
		CTreeNodeUI *pNode1 = (CTreeNodeUI *)(pTree->GetItemAt(nIndex1));
		if(0 <= pNode1->GetName().Find(_T("ndc_")))//组节点
		{
			INT32 nSize2 = pNode1->GetCount();
			for (INT32 nIndex2=0; nIndex2<nSize2; nIndex2++)
			{
				CControlUI *pControl = pNode1->GetItemAt(nIndex2);
				CDuiString strControlName = pControl->GetName();
				if(0 <= strControlName.Find(_T("chbNod_")))
				{
					CCheckBoxUI *pChkBox = (CCheckBoxUI *)pControl;
					if(pChkBox->GetCheck())
					{
						CString strIP = pControl->GetText();
						strSelIP.AppendFormat(_T("%s, "), strIP);
					}
				}
			}
		}
	}
	strSelIP = strSelIP.Left(strSelIP.ReverseFind(','));
	if(strSelIP.IsEmpty())
	{
		//提示没有指定服务器IP
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("请指定服务器IP"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
		return;
	}

	map<CString, ServerItem> mapServer;
	Singleton<CMzdIOMgr>::Instance().GetAllServer(mapServer);
	map<CString, ServerItem>::iterator itor = mapServer.begin();
	while(itor != mapServer.end())
	{
		if(0 == itor->second.strIP.Compare(strSelIP) && m_strServerName != itor->first)
		{
			//提示IP有冲突
			box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("已经存在相同IP设定的服务器，请重新选择服务器IP"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
			return;
		}
		itor++;
	}
	ServerInfo info;
	info.strName = strServerName;
	info.strSqareServer = strSpareServer;
	info.u32DataDiskSize = _ttoi(strDataSize);
	info.u32SSDDiskSize = _ttoi(strSSDSize);
	info.u32SysDiskSize = _ttoi(strSysSize);
	if(!strDir1.IsEmpty())
		info.vecRWrite.push_back(CString(strDir1.GetData()));
	if(!strDir2.IsEmpty())
		info.vecRWrite.push_back(CString(strDir2.GetData()));
	if(!strDir3.IsEmpty())
		info.vecRWrite.push_back(CString(strDir3.GetData()));
	if(!strDir4.IsEmpty())
		info.vecRWrite.push_back(CString(strDir4.GetData()));
	if(m_strServerName.IsEmpty())//表示添加
	{
		Singleton<CMzdIOMgr>::Instance().SetServerInfoToMZD(strSelIP, strServerName, strServerName, info, 0);
	}
	else
	{
		Singleton<CMzdIOMgr>::Instance().SetServerInfoToMZD(strSelIP, m_strServerName, strServerName, info, 1);
	}
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())   //同步INI
	{
		Util::Log::Error(_T("MZSKin"), _T("[error]服务器添加或修改同步数据出错<serverName=%s>\r\n"), m_strServerName);
	}
	::PostMessage(m_hParentWnd, UW_LOADSERVER_MSG, NULL, NULL);
	Close(IDCANCEL);
}
}
namespace Popup
{
	void OpenAddServerWnd(HWND hWnd, CString strSvrName)
	{
		CString strWndName = _T("新增服务器");
		if(!strSvrName.IsEmpty())
			strWndName = _T("修改服务器");

		static HWND hAddServerWnd = NULL;
		if (!::IsWindow(hAddServerWnd))
		{
			CAddServerWnd * pAddServerWnd = new CAddServerWnd();
			pAddServerWnd->SetServerName(strSvrName);
			pAddServerWnd->SetWnd(hWnd);
			pAddServerWnd->Create(hWnd, strWndName, WS_POPUPWINDOW, 0, 0, 0, 0, 0);
			pAddServerWnd->CenterWindow();
			pAddServerWnd->ShowWindow(true, false);
			hAddServerWnd = pAddServerWnd->GetHWND();
		}
		global::FrontWindow(hAddServerWnd);
	}
}
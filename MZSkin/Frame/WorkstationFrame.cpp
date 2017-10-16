#include "StdAfx.h"
#include "WorkstationFrame.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <Global\Control\FrontButtonUI.h>
#include <SkinInner/SkinAPI.h>
#include "..\Server\mzdio\MzdIOMgr.h"
#include <SharedUI\Frame\CDuiMenuISO.h>
#include "..\Server\WorkstationInfoMgr.h"
#include "..\SharedUI\Interface\SharedBase.h"

#define STATUSTIMER	1000
#define WORKSTATIONINFOTIMER 1001
#define UPDATESTATUSLEN	3 * 60 * 1000

namespace DuiLib{
	

CWorkstationFrame::CWorkstationFrame()
{
	m_isMZDIORun  = FALSE;
	m_u32CurWksCount = 0;
	m_pList = NULL;
	m_pElement = NULL;
	m_GroupList = NULL;
	m_pNode = NULL;
	m_pLabCurModifyGroup = NULL;
	::KillTimer(m_hWnd, STATUSTIMER);
	event::UpdateStatusBar().disconnect(this);
}

CWorkstationFrame::~CWorkstationFrame(void)
{
	::KillTimer(m_hWnd, WORKSTATIONINFOTIMER);
}

CDuiString CWorkstationFrame::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CWorkstationFrame::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CWorkstationFrame::GetSkinFile()
{
	return _T("xml\\WorkstationFrame.xml");
}

LPCTSTR CWorkstationFrame::GetWindowClassName(void) const
{
	return _T("WorkstationClassFrame");
}

void CWorkstationFrame::InitWindow()
{
	m_pLabStatus = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_statusicon")));
	CLabelUI *pLabStart = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_start")));
	CFrontButtonUI *pSupBtn = static_cast<CFrontButtonUI *>(m_PaintManager.FindControl(_T("bt_sup")));
	CCheckBoxUI *pchkAutoAdd = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_autoadd")));
	BOOL bAutoAdd = Singleton<CMzdIOMgr>::Instance().isAutoAdd();
	pchkAutoAdd->SetCheck(bAutoAdd?true:false);
	//�жϳ���
	CString strSuperNum = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();
	UINT32 u32SuperNum = _ttoi(strSuperNum);
	if(0 < u32SuperNum)
	{
		pSupBtn->SetText(_T("ȡ������"));
		pSupBtn->SetEnabled(false);
	}
	else
	{
		pSupBtn->SetText(_T("������"));
		pSupBtn->SetEnabled(true);
	}
	//��ȡ���й���վ
	map<UINT32, WorkstationInfo> mapWorkstation;
	m_pList = static_cast<CWorkstationContactList*>(m_PaintManager.FindControl(_T("lt_workstation")));
	Singleton<CMzdIOMgr>::Instance().GetAllWorkstation(mapWorkstation);

	//��ȡ���й���վ��״̬
	map<CString, UINT32> mapStatus;
	Singleton<CMzdIOMgr>::Instance().GetWorkstationStatus(mapStatus);
	map<CString, UINT32>::iterator itEnd = mapStatus.end();

	map<UINT32, WorkstationInfo>::iterator itor = mapWorkstation.begin();
	while(itor != mapWorkstation.end())
	{
		WorkstationItem item;
		CString strNum;
		strNum.Format(_T("%u"), itor->first);
		map<CString, UINT32>::iterator itFind = mapStatus.find(strNum);
		if(u32SuperNum == itor->second.u32Num && itEnd != itFind && 1 == itFind->second)
			item.strIcon = _T("material/listbtn/super02.png");
		else if(u32SuperNum == itor->second.u32Num && itEnd != itFind && 0 == itFind->second)
			item.strIcon = _T("material/listbtn/super01.png");
		else if(itEnd != itFind && 1 == itFind->second)
			item.strIcon = _T("material/listbtn/computer02.png");
		else if(itEnd != itFind && 0 == itFind->second)
			item.strIcon = _T("material/listbtn/computer01.png");
		item.strIP = itor->second.strIP;
		item.strMac = itor->second.strMac;
		item.strMirrorFile = itor->second.strMirrorFile;
		item.strMirrorIP = itor->second.strMirrorIP;
		item.strName = itor->second.strName;
		item.strReturnWDir = itor->second.strReturnWDir;
		item.u32Num = itor->second.u32Num;
		item.u32Size = _ttoi(itor->second.strSize);
		item.u8Status = itor->second.u8Status;
		m_pList->InsertItem(itor->first, item);
		itor++;
	}
	m_pList->SelectItem(0);
	//��ʼ������
	map<UINT32, CString> mapGroup;
	Singleton<CMzdIOMgr>::Instance().GetAllGroup(mapGroup);
	m_GroupList = static_cast<CServerTreeUI*>(m_PaintManager.FindControl(_T("tree_WSLeft")));
	m_pNode = m_GroupList->AddGroupNodeLab(NULL, _T("���й���վ"), FALSE);
	m_GroupList->AddChildNodeLab(m_pNode, _T("Ĭ�Ϸ���"), _T("Ĭ�Ϸ���"), FALSE, 0);
	map<UINT32, CString>::iterator itor2 = mapGroup.begin();
	while(itor2 != mapGroup.end())
	{
		m_GroupList->AddChildNodeLab(m_pNode, itor2->second, itor2->second, FALSE, itor2->first);
		itor2++;
	}


	::SetTimer(m_hWnd, WORKSTATIONINFOTIMER, UPDATESTATUSLEN, NULL);
	event::ExitProcessEvn().connect(this, &CWorkstationFrame::ExitProcessEvn);
	if(TRUE == Singleton<CMzdIOMgr>::Instance().IsConnect())
	{
		m_isMZDIORun = TRUE;
		::SetTimer(m_hWnd, STATUSTIMER, 500, NULL);
	}
	else
	{
		pLabStart->SetText(_T("MZD������δ����"));
		m_pLabStatus->SetBkImage(_T("material/general/status01.png"));
	}
	event::UpdateStatusBar().connect(this, &CWorkstationFrame::UpdateStatusBar);
	UpdateStatusBar();
	
	Singleton<CMzdIOMgr>::Instance().SetWrokstationWnd(m_hWnd);
}

CControlUI* CWorkstationFrame::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass, _T("FrontButton")) == 0)
	{
		return new CFrontButtonUI();
	}
	else if(_tcscmp(pstrClass, _T("WorkstationContactList")) == 0)
	{
		return new CWorkstationContactList();
	}
	else if(_tcscmp(pstrClass, _T("ServerTree")) == 0)
	{
		return new CServerTreeUI();
	}
	return NULL;
}
//LRESULT CWorkstationFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
// 	LRESULT lRes = 0;
// 	BOOL bHandled = TRUE;
// 	switch (uMsg)
// 	{
// 	case WM_RBUTTONDOWN:
// 		OnRButtonDown(uMsg, wParam, lParam, bHandled);
// 	default:
// 		bHandled = FALSE;
// 		break;
// 	}
// 	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
// 	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//}
LRESULT CWorkstationFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(UW_LOADWORKSTATION_MSG == uMsg)
	{
		m_pList = static_cast<CWorkstationContactList*>(m_PaintManager.FindControl(_T("lt_workstation")));
		vector<WorkstationItem> vecItem;
		Singleton<CWorkstationInfoMgr>::Instance().GetNewWorkstation(vecItem);
		INT32 nSize = vecItem.size();
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			WorkstationItem item;
			if(TRUE == m_pList->GetItem(vecItem[nIndex].u32Num, item))
			{
				vecItem[nIndex].strIcon = item.strIcon;
				m_pList->SetItem(vecItem[nIndex].u32Num, vecItem[nIndex]);
			}
			else
			{
				m_u32CurWksCount++;
				m_pList->InsertItem(vecItem[nIndex].u32Num, vecItem[nIndex]);
			}
		}
		
		CLabelUI *pLabTatol = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_AllSize")));
		CString strText;
		strText.Format(_T("����վ̨����{c #5b75fb}%u{/c}"), m_u32CurWksCount);
		pLabTatol->SetText(strText);
		if(TRUE != Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd())
		{
			Util::Log::Error(_T("MZSkin"), _T("[error]���¹���վ������ͬ�������������\r\n"));
		}
//		OnLoadWorkstation();
	}
	else if(UM_UPDATEWROKSTATION_MSG == uMsg)
	{
		OnUpdateWorkstation();
	}
	else if(UM_ISMZDIOSERVICERUN  == uMsg)
	{
		OnMZDIORun((BOOL)lParam);
	}
	else if(WM_RBUTTONUP == uMsg)
	{
		OnRButtonDown(uMsg, wParam, lParam, bHandled);
	}
	else if(WM_LBUTTONDBLCLK == uMsg)
	{
		OnLButtonDBClk(uMsg, wParam, lParam, bHandled);
	}
	else if(UW_WORKSTATIONGROUPLIST_MENU == uMsg)
	{
		POINT pt = { 0 };
		pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		if(OnRButtonDownWorkstationGroupList(pt))
		{

		}
		else if(OnRButtonDownWorkstationList(pt))
		{

		}
	}
	else if(WM_TIMER == uMsg)
	{
		OnTimer(wParam, lParam, bHandled);
	}
	else if(UM_OPEN_SINGLEEDIT_WND == uMsg)
	{
		tagSingleEditBoxInfo stInfo;
		if(1 == (INT32)lParam)//��ӷ���
		{
			stInfo.strCaption = _T("��ӷ���");
			stInfo.strKey = _T("AddGroup");
		}
		else if(2 == (INT32)lParam)//��ӷ���
		{
			stInfo.strCaption = _T("�޸ķ���");
			stInfo.strKey = _T("ModifyGroup");
		}
		SingleEditBox::ShowModel(m_hWnd, stInfo);
	}
	else if(UM_SINGLEEDIT_MSG == uMsg)
	{
		tagSinleEditMsg *msg = NULL;
		msg = (tagSinleEditMsg *)lParam;
		if(0 == msg->strKey.Compare(_T("AddGroup")))
		{
			if(FALSE == Singleton<CMzdIOMgr>::Instance().IsExistGroup(msg->strContent))
			{
				INT32 nID = Singleton<CMzdIOMgr>::Instance().AddWorkstationGroup(msg->strContent);
				m_GroupList->AddChildNodeLab(m_pNode, msg->strContent, msg->strContent, FALSE, nID);
				if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())
				{
//					Util::Log::Error(_T("MZSkin"), _T("[error]��ӷ���ʱ������ͬ���������\r\n"));
				}
			}
			else
			{
				SIZE size;
				size.cx = 50;
				size.cy = 40;
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("�����Ѿ�����"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
			}
		}
		if(0 == msg->strKey.Compare(_T("ModifyGroup")))
		{
			if(FALSE == Singleton<CMzdIOMgr>::Instance().IsExistGroup(msg->strContent))
			{
				Singleton<CMzdIOMgr>::Instance().ModifyWorkstationGroup((UINT32)(m_pLabCurModifyGroup->GetTag()), msg->strContent);
				m_pLabCurModifyGroup->SetText(msg->strContent);
				if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())
				{
//					Util::Log::Error(_T("MZSkin"), _T("[error]�޸ķ���ʱ������ͬ���������\r\n"));
				}
			}
			else
			{
				SIZE size;
				size.cx = 50;
				size.cy = 40;
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("�����Ѿ�����"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
			}
		}
	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CWorkstationFrame::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	PostMessage(UW_WORKSTATIONGROUPLIST_MENU, wParam, lParam);
	return S_OK;
}
LRESULT CWorkstationFrame::OnLButtonDBClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { 0 };
	pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
	if(pControl && pControl->GetParent() && pControl->GetParent()->GetParent()
		&& (0 == pControl->GetParent()->GetParent()->GetName().Compare(_T("workstationitem"))))
	{
		m_u32CurWksCount = Singleton<CMzdIOMgr>::Instance().GetWokstationCount();
		Singleton<CWorkstationInfoMgr>::Instance().DeleteAllSelWorkstation();
		INT32 nSize = m_pList->GetCount();
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
			if(pElement->IsSelected())
			{
				CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
				CDuiString strNum = pLabNum->GetText();
				Singleton<CWorkstationInfoMgr>::Instance().AddWorkstation(strNum.GetData());
			}
		}
		UINT32 u32Num = 1;
		INT32 nSelSize = Singleton<CWorkstationInfoMgr>::Instance().GetSelCount();
		if(m_pElement && 1 == nSelSize)
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			u32Num = _ttoi(strNum);
		}
		if(0 < nSelSize)
		{
			Popup::OpenAddWorkstationWnd(m_hWnd, u32Num);
		}
		else
		{
			SIZE size;
			size.cx = 50;
			size.cy = 40;
			box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("��ѡ��Ҫ�޸ĵĹ���վ"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		}
	}



	return S_OK;
}
BOOL CWorkstationFrame::OnRButtonDownWorkstationGroupList(POINT &pt)
{
	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
	if(pControl && (0 == pControl->GetParent()->GetName().Compare(_T("ndg_���й���վ"))
		|| 0 <= pControl->GetParent()->GetName().Compare(_T("ndc_"))))//�ʼ������й���վ�ڵ�
	{
		::ClientToScreen(m_hWnd, &pt);
		CDuiMenuISO menu;
		menu.LoadFromXml(_T("xml\\WorkstationGroupMenu.xml"));
		CDuiMenuUI *pMenuUI = menu.FindMenuByName(_T("workstationGroup_menu"));
		CDuiMenuElementUI* pMenuElementAdd = static_cast<CDuiMenuElementUI*>(pMenuUI->FindSubControl(_T("add_group")));
		CDuiMenuElementUI* pMenuElementModify = static_cast<CDuiMenuElementUI*>(pMenuUI->FindSubControl(_T("modify_group")));
		CDuiMenuElementUI* pMenuElementDel = static_cast<CDuiMenuElementUI*>(pMenuUI->FindSubControl(_T("del_group")));
		if(0 <= pControl->GetParent()->GetName().Find(_T("ndg_")))
		{
			pMenuElementAdd->SetEnabled(true);
			pMenuElementModify->SetEnabled(false);
			pMenuElementDel->SetEnabled(false);
		}
		else if(0 <= pControl->GetParent()->GetName().Find(_T("ndc_")))
		{
			pMenuElementAdd->SetEnabled(false);
			pMenuElementModify->SetEnabled(true);
			pMenuElementDel->SetEnabled(true);
		}
		CDuiString strCmd = menu.TrackPopupMenu(m_PaintManager.GetPaintWindow(), pt.x+20, pt.y);
		if(strCmd == _T("addgroup"))
		{
			AddGroup();
		}
		else if(strCmd == _T("modifygroup"))
		{
			ModifyGroup(pControl);
		}
		else if(strCmd == _T("delgroup"))
		{
			DelGroup(pControl);
		}
		return TRUE;
	}
	return FALSE;
}
BOOL CWorkstationFrame::OnRButtonDownWorkstationList(POINT &pt)
{
	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
	CListContainerElementUI * pElement = (CListContainerElementUI *)(pControl->GetParent()->GetParent());
	if(0 == pElement->GetName().Compare(_T("workstationitem")))
	{
		CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
		CString strName = pLabNum->GetText();
		WorkstationInfo info;
		Singleton<CMzdIOMgr>::Instance().GetSimpleWorStation(_ttoi(strName), info);
		CDuiMenuISO menu;
		menu.LoadFromXml(_T("xml\\WorkstationListMenu.xml"));//
		UINT32 uID = 0;
		CDuiMenuUI *pExtendMenuUI = menu.FindMenuByName(_T("extend_groupset_menu"));
		map<UINT32, CString> mapGroup;
		Singleton<CMzdIOMgr>::Instance().GetAllGroup(mapGroup);
		if(pExtendMenuUI)
		{
			CDuiMenuElementUI *pMenuElementUI = new CDuiMenuElementUI;
			
			UINT32 nID = 0;
			pMenuElementUI->SetTag((UINT_PTR)(nID));
			CString strText;
			if(info.u32GroupID == 0)
			{
				strText.Format(_T("{x 3}{i material/general/menu_tick_normal.png}{x 3}Ĭ�Ϸ���"));
			}
			else
			{
				strText.Format(_T("{x 21}Ĭ�Ϸ���"));
			}
			pMenuElementUI->SetText(strText);
			pMenuElementUI->SetAttribute(_T("command"), _T("0"));
			pMenuElementUI->SetFixedHeight(28);
			pExtendMenuUI->SetFixedHeight(pExtendMenuUI->GetFixedHeight() + pMenuElementUI->GetFixedHeight());
			pExtendMenuUI->Add(pMenuElementUI);
			map<UINT32, CString>::iterator it = mapGroup.begin();
			while(it != mapGroup.end())
			{
				CDuiMenuElementUI *pMenuElementUI = new CDuiMenuElementUI;
				if(info.u32GroupID == it->first)
				{
					strText.Format(_T("{x 3}{i material/general/menu_tick_normal.png}{x 3}%s"), it->second);
				}
				else
				{
					strText.Format(_T("{x 21}%s"), it->second);
				}
				pMenuElementUI->SetText(strText);
				pMenuElementUI->SetTag((UINT_PTR)(it->first));
				CString strCommand;
				strCommand.Format(_T("%u"), it->first);
				pMenuElementUI->SetAttribute(_T("command"), strCommand);
				pMenuElementUI->SetFixedHeight(28);
				pExtendMenuUI->SetFixedHeight(pExtendMenuUI->GetFixedHeight() + pMenuElementUI->GetFixedHeight());
				pExtendMenuUI->Add(pMenuElementUI);
				it++;
			}
		}
		::ClientToScreen(m_hWnd, &pt);
		CDuiString strCmd = menu.TrackPopupMenu(m_PaintManager.GetPaintWindow(), pt.x+20, pt.y);
		if(strCmd.IsEmpty())
			return TRUE;
		if(strCmd == _T("wakeup"))
		{
			OnWakeUp();
		}
		else if(strCmd == _T("clearmac"))
		{
			OnClearMac();
		}
		else if(strCmd == _T("0"))
		{
			uID = _ttoi(strCmd);
			Singleton<CMzdIOMgr>::Instance().SetWorkStationGroupID(uID, strName);
			Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd();
		}
		else if(mapGroup.find(_ttoi(strCmd.GetData())) != mapGroup.end())
		{
			uID = _ttoi(strCmd);
			Singleton<CMzdIOMgr>::Instance().SetWorkStationGroupID(uID, strName);
			Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd();
		}
		return TRUE;
	}
	return FALSE;
}
void CWorkstationFrame::OnUpdateWorkstationStatus()
{
	BOOL bOk = Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
	if(FALSE == bOk)
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]���¹���վ״̬ʱ������ͬ�������������\r\n"));
		return;
	}
	CString strSuperNum = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();
	UINT32 u32SuperNum = _ttoi(strSuperNum);

	map<CString, UINT32> mapStatus;
	Singleton<CMzdIOMgr>::Instance().GetWorkstationStatus(mapStatus);
	map<CString, UINT32>::iterator itor = mapStatus.begin();
	while(itor != mapStatus.end())
	{
		UINT32 u32Num = _ttoi(itor->first);
		WorkstationItem item;
		m_pList->GetItem(u32Num, item);

		if(u32SuperNum == u32Num && 1 == itor->second)
			item.strIcon = _T("material/listbtn/super02.png");
		else if(u32SuperNum == u32Num && 0 == itor->second)
			item.strIcon = _T("material/listbtn/super01.png");
		else if(1 == itor->second)
			item.strIcon = _T("material/listbtn/computer02.png");
		else if(0 == itor->second)
			item.strIcon = _T("material/listbtn/computer01.png");

		m_pList->SetItem(u32Num, item);
		itor++;
	}
	//֪ͨ�����ڸ���״̬����Ϣ
	event::UpdateStatusBar().run();
}
LRESULT CWorkstationFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CWorkstationFrame::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if(WORKSTATIONINFOTIMER == wParam)
	{
		OnUpdateWorkstationStatus();
	}
	else if(STATUSTIMER == wParam)
	{
		CString strBkImage = m_pLabStatus->GetBkImage();
		if(m_isMZDIORun)
		{
			if(strBkImage.IsEmpty())
			{
				m_pLabStatus->SetBkImage(_T("material/general/status02.png"));
			}
			else
			{
				m_pLabStatus->SetBkImage(_T(""));
			}
		}
		else
		{
			m_pLabStatus->SetBkImage(_T("material/general/status01.png"));
		}
	}
    return 0;
}
void CWorkstationFrame::Notify(TNotifyUI& msg)
{
	CDuiString	strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == strName.Compare(_T("bt_add")))//���
		{
			m_u32CurWksCount = Singleton<CMzdIOMgr>::Instance().GetWokstationCount();
			Popup::OpenAddWorkstationWnd(m_hWnd, 0);
		}
		else if(0 == strName.Compare(_T("bt_mdi")))//�޸�
		{
			//ȷ���޸�ѡ��Ĺ���վ
			m_u32CurWksCount = Singleton<CMzdIOMgr>::Instance().GetWokstationCount();
			Singleton<CWorkstationInfoMgr>::Instance().DeleteAllSelWorkstation();
			INT32 nSize = m_pList->GetCount();
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
				if(pElement->IsSelected())
				{
					CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
					CDuiString strNum = pLabNum->GetText();
					Singleton<CWorkstationInfoMgr>::Instance().AddWorkstation(strNum.GetData());
				}
			}
			UINT32 u32Num = 1;
			INT32 nSelSize = Singleton<CWorkstationInfoMgr>::Instance().GetSelCount();
			if(m_pElement && 1 == nSelSize)
			{
				CLabelUI *pLabNum = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_num")));
				CDuiString strNum = pLabNum->GetText();
				u32Num = _ttoi(strNum);
			}

			if(0 < nSelSize)
			{
				Popup::OpenAddWorkstationWnd(m_hWnd, u32Num);
			}
			else
			{
				SIZE size;
				size.cx = 50;
				size.cy = 40;
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("��ѡ��Ҫ�޸ĵĹ���վ"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
			}
		}
		else if(0 == strName.Compare(_T("bt_del")))//ɾ��
		{
			OnDel();//postһ���Զ�����Ϣ

		}
		else if(0 == strName.Compare(_T("bt_dis")))//����
		{
			OnDisable();//postһ���Զ�����Ϣ
		}
		else if(0 == strName.Compare(_T("bt_str")))//����
		{
			OnEnable();//Postһ���Զ�����Ϣ
		}
		else if(0 == strName.Compare(_T("bt_reb")))//����
		{
			OnReboot();//Postһ���Զ�����Ϣ
		}
		else if(0 == strName.Compare(_T("bt_shd")))//�ػ�
		{
			OnPowerOff();//Postһ���Զ�����Ϣ
		}
		else if(0 == strName.Compare(_T("bt_sup")))//������
		{
			OnSuperMgr();
		}
		else if(0 == strName.Compare(_T("chb_autoadd")))//�Զ���ӹ���վ
		{
			CCheckBoxUI *pChkAutoAdd = (CCheckBoxUI *)msg.pSender;
			bool bCheck = pChkAutoAdd->GetCheck();
			Singleton<CMzdIOMgr>::Instance().SetAutoAdd(bCheck?FALSE:TRUE);
			Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd();
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if(0 == strName.Compare(_T("lt_workstation")))
		{
			m_pElement = (CListContainerElementUI *)(m_pList->GetItemAt(m_pList->GetCurSel()));
			if(m_pElement)
			{
				CString strSuperNum = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();
				CLabelUI *pLabNum = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_num")));
				CString strNum = pLabNum->GetText();
				CFrontButtonUI *pSupBtn = static_cast<CFrontButtonUI *>(m_PaintManager.FindControl(_T("bt_sup")));
				if(_ttoi(strSuperNum) == _ttoi(strNum))
				{
					pSupBtn->SetEnabled(true);
					pSupBtn->SerFrontimage(_T("material/listbtn/super02.png"));
				}
				else if(0 < _ttoi(strSuperNum))
				{
					pSupBtn->SetEnabled(false);
					pSupBtn->SerFrontimage(_T("material/listbtn/super01.png"));
				}
				else
				{
					pSupBtn->SetEnabled(true);
					pSupBtn->SerFrontimage(_T("material/listbtn/super02.png"));
				}
				pSupBtn->Invalidate();
			}
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMRCLICK)
	{
		if(0 == strName.Compare(_T("tree_WSLeft")))
		{

		}
	}

}
void CWorkstationFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CWorkstationFrame::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
void CWorkstationFrame::UpdateStatusBar()
{
	UINT32 u32Tatol = Singleton<CMzdIOMgr>::Instance().GetWokstationCount();
	UINT32 u32Online = Singleton<CMzdIOMgr>::Instance().GetWokstationOnlineCount();
	CString strSuperNum = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();

	CLabelUI *pLabTatol = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_AllSize")));
	CLabelUI *pLabOnline = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_onlineSize")));
	CLabelUI *pLabSuperNum = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_superMarNum")));
	CString strText;
	strText.Format(_T("����վ̨����{c #5b75fb}%u{/c}"), u32Tatol);
	pLabTatol->SetText(strText);

	strText.Format(_T("����̨����{c #5b75fb}%u{/c}"), u32Online);
	pLabOnline->SetText(strText);

	strText.Format(_T("���ܱ�ţ�{c #5b75fb}%s{/c}"), strSuperNum);
	pLabSuperNum->SetText(strText);
}
void CWorkstationFrame::OnUpdateWorkstation()
{
	CString strSuperNum = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();
	UINT32 u32SuperNum = _ttoi(strSuperNum);
	vector<WorkstationInfo> vecUpdateWorkstation;
	Singleton<CMzdIOMgr>::Instance().GetUpdateWorkstationInfo(vecUpdateWorkstation);
	INT32 nSize = vecUpdateWorkstation.size();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		WorkstationItem item;
		BOOL bIsExistItem = m_pList->GetItem(vecUpdateWorkstation[nIndex].u32Num, item);
		item.strIP = vecUpdateWorkstation[nIndex].strIP;
		item.strMac = vecUpdateWorkstation[nIndex].strMac;
		item.u32Num = vecUpdateWorkstation[nIndex].u32Num;
		item.u32Size = _ttoi(vecUpdateWorkstation[nIndex].strSize);
		item.strName = vecUpdateWorkstation[nIndex].strName;
		item.strMirrorIP = vecUpdateWorkstation[nIndex].strMirrorIP;
		item.strMirrorFile = vecUpdateWorkstation[nIndex].strMirrorFile;
		item.strReturnWDir = vecUpdateWorkstation[nIndex].strReturnWDir;
		if(TRUE == bIsExistItem)
		{
			m_pList->SetItem(vecUpdateWorkstation[nIndex].u32Num, item);
		}
		else
		{
			if(u32SuperNum == vecUpdateWorkstation[nIndex].u32Num && 1 == vecUpdateWorkstation[nIndex].u8Status)
				item.strIcon = _T("material/listbtn/super02.png");
			else if(u32SuperNum == vecUpdateWorkstation[nIndex].u32Num && 0 == vecUpdateWorkstation[nIndex].u8Status)
				item.strIcon = _T("material/listbtn/super01.png");
			else if(1 == vecUpdateWorkstation[nIndex].u8Status)
				item.strIcon = _T("material/listbtn/computer02.png");
			else if(0 == vecUpdateWorkstation[nIndex].u8Status)
				item.strIcon = _T("material/listbtn/computer01.png");
			item.u8Status = vecUpdateWorkstation[nIndex].u8Status;
			m_pList->InsertItem(vecUpdateWorkstation[nIndex].u32Num, item);
		}
	}
}
void CWorkstationFrame::OnMZDIORun(BOOL bRun)
{
	m_isMZDIORun = bRun;
	CLabelUI *pLabStart = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_start")));
	if(bRun)
	{
		pLabStart->SetText(_T("MZD����������"));
	}
	else
	{
		pLabStart->SetText(_T("MZD����δ����"));
	}
}
void CWorkstationFrame::OnDel()
{
	//��ȡ���ܱ�ţ������ǲ���ɾ����
	CString strSuperMgr = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();

	//��ʾ��ȷ��ɾ��ѡ�еĹ���վ��

	SIZE size;
	size.cx = 50;
	size.cy = 40;
	if (IDOK != box::ShowModal(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("ȷ��ɾ��ѡ�еĹ���վ��"), _T("ȷ��"), _T(""), _T(""), _T("material//general//warning.png"), size))
	{
		return;
	}
	vector<UINT32> vecDel;
	BOOL bIsHaveSuperMgr = FALSE;
	INT32 nSize = m_pList->GetCount();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement && pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			if(0 == strSuperMgr.Compare(strNum))
			{
				bIsHaveSuperMgr = TRUE;
				continue;
			}
			Singleton<CMzdIOMgr>::Instance().DelWorkstation(strNum.GetData());
			vecDel.push_back((UINT32)_ttoi(strNum.GetData()));
		}
	}
	//���б���ɾ��
	nSize = vecDel.size();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		m_pList->DeleteItem(vecDel[nIndex]);
		Singleton<CMzdIOMgr>::Instance().DelWorkStation(vecDel[nIndex]);
	}
	
	if(bIsHaveSuperMgr)
	{
		//��ʾ���ܲ������Ƴ�;
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("����ɾ�����ܹ���վ"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		return;
	}
	Singleton<CMzdIOMgr>::Instance().CheckUnuserMenu();   //δʹ�ò˵����
	Singleton<CMzdIOMgr>::Instance().CheckUnuserDisk();   //δʹ�ô��̼��
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
	{
// 		Util::Log::Error(_T("MZSkin"), _T("[error]ɾ������վ��ʱ�������ϴ������������\r\n"));
// 		return;
	}
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd())
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]ɾ������վ��ʱ������ͬ�������������\r\n"));
	}
//	::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
}
void CWorkstationFrame::OnDisable()
{
	//��ȡ���ܱ�ţ������ǲ���ɾ����
	CString strSuperMgr = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();

	BOOL bIsHaveSuperMgr = FALSE;
	INT32 nSize = m_pList->GetCount();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			if(0 == strSuperMgr.Compare(strNum))
			{
				bIsHaveSuperMgr = TRUE;
				continue;
			}
			Singleton<CMzdIOMgr>::Instance().DisableWorkstation(strNum.GetData());
		}
	}
	if(bIsHaveSuperMgr)
	{
		//��ʾ���ܲ����Խ���;
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("���ܽ��ó��ܹ���վ"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		return;
	}
	/*
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]���ù���վ��ʱ�������ϴ������������\r\n"));
	}
	*/
//	::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
}
void CWorkstationFrame::OnEnable()
{
	INT32 nSize = m_pList->GetCount();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			Singleton<CMzdIOMgr>::Instance().EnableWorkstation(strNum.GetData());
		}
	}
	/*
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]���ù���վ��ʱ�������ϴ������������\r\n"));
	}
	*/
//	::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
}
void CWorkstationFrame::OnReboot()
{
	INT32 nSize = m_pList->GetCount();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(m_pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			Singleton<CMzdIOMgr>::Instance().RebootWorkstation(strNum.GetData());
		}
	}
/*
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]��������վ��ʱ�������ϴ������������\r\n"));
	}
*/

//	::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
}
void CWorkstationFrame::OnPowerOff()
{
	INT32 nSize = m_pList->GetCount();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			Singleton<CMzdIOMgr>::Instance().CloseWorkstation(strNum.GetData());
		}
	}
	/*
	Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd();     //ͬ��INI
	*/
//	::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
}
void CWorkstationFrame::OnSuperMgr()
{
	CString strSuperMgr = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();
	CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(m_pList->GetCurSel()));
	if(pElement)
	{
		CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
		CLabelUI *pIcon = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_icon")));
		CDuiString strNum = pLabNum->GetText();
		CString strError;
		if(0 == strSuperMgr.Compare(strNum))
		{
			int iRet = Singleton<CMzdIOMgr>::Instance().EndSuperMgr(strNum);
			switch(iRet)
			{
			case -1:strError = _T("MZDH_IO ͨѶʧ��"); break;
			case -2:strError = _T("�����û�ʹ����,���ȹػ�"); break;
			case -3:strError = _T("ѡ�еĲ��ǵ�ǰ�����û�"); break;
			case -10:strError = _T("MZDH_IO �ڴ治��"); break;
			case -20:strError = _T("����ά�������У���ȴ�"); break;
			case -30:strError = _T("ȡ������ʧ��"); break;
			case -31:strError = _T("���������߳�ʧ��"); break;
			}
			if(0 <= iRet)
			{
				CFrontButtonUI *pSupBtn = static_cast<CFrontButtonUI *>(m_PaintManager.FindControl(_T("bt_sup")));
				UINT32 u32Status = Singleton<CMzdIOMgr>::Instance().GetWorkStationStatus(strNum);
				pSupBtn->SetText(_T("������"));
				pSupBtn->SetEnabled(true);
				pSupBtn->SerFrontimage(_T("material/listbtn/super02.png"));
				if(0 == u32Status)
				{
					pIcon->SetBkImage(_T("material/listbtn/computer01.png"));
				}
				else
				{
					pIcon->SetBkImage(_T("material/listbtn/computer02.png"));
				}
				
			}
			else
			{
				SIZE size;
				size.cx = 50;
				size.cy = 40;
				box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), strError, NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
				return;
			}
		}
		else
		{
			Singleton<CMzdIOMgr>::Instance().SetSuperMgr(strNum);
			Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd();
			CFrontButtonUI *pSupBtn = static_cast<CFrontButtonUI *>(m_PaintManager.FindControl(_T("bt_sup")));
			pSupBtn->SetText(_T("ȡ������"));
			pSupBtn->SetEnabled(true);
			UINT32 u32Status = Singleton<CMzdIOMgr>::Instance().GetWorkStationStatus(strNum);
			if(0 == u32Status)
			{
				pIcon->SetBkImage(_T("material/listbtn/super01.png"));
			}
			else
			{
				pIcon->SetBkImage(_T("material/listbtn/super02.png"));
			}
			pSupBtn->SerFrontimage(_T("material/listbtn/super02.png"));
			
		}
//		::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
		if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
		{
//			Util::Log::Error(_T("MZSkin"), _T("[error]���ù���վ����ʱ�������ϴ������������\r\n"));
		}
		event::UpdateStatusBar().run();
	}
}
//��ӷ���
void CWorkstationFrame::AddGroup()
{
	//������ӷ��鴰��
	::PostMessage(m_hWnd, UM_OPEN_SINGLEEDIT_WND, NULL, 1);
}
//ɾ������
void CWorkstationFrame::DelGroup(CControlUI *pNode)
{
	CString strGroupName = pNode->GetText();
	INT32 nGroupID = pNode->GetTag();
	if(0 < nGroupID)
	{
		Singleton<CMzdIOMgr>::Instance().DelWorkstationGroup((UINT32)nGroupID);
		m_GroupList->Remove((CTreeNodeUI *)(pNode->GetParent()));
		if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
		{
//			Util::Log::Error(_T("MZSkin"), _T("[error]��ӷ���ʱ�������ϴ������������\r\n"));
		}
	}
	else
	{
		//��ʾ��Ĭ�Ϸ��鲻��ɾ��
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("Ĭ�Ϸ��鲻�ܱ�ɾ��"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		return;
	}
}
//�޸ķ���
void CWorkstationFrame::ModifyGroup(CControlUI *pNode)
{
	CString strGroupName = pNode->GetText();
	INT32 nGroupID = pNode->GetTag();
	if(0 < nGroupID)
	{
		//�����޸���ʾ����
		m_pLabCurModifyGroup = (CLabelUI *)pNode;
		::PostMessage(m_hWnd, UM_OPEN_SINGLEEDIT_WND, NULL, 2);
	}
	else
	{
		//��ʾ��Ĭ�Ϸ��鲻���޸�
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("Ĭ�Ϸ������Ʋ����޸ĸ�"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		return;
	}
}
//���MAC
void CWorkstationFrame::OnClearMac()
{
	//��ȡ���ܱ�ţ������ǲ���ɾ����
	CString strSuperMgr = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();

	//��ʾ��ȷ��ɾ��ѡ�еĹ���վ��

	SIZE size;
	size.cx = 50;
	size.cy = 40;
	if (IDOK != box::ShowModal(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("ȷ�����ѡ�еĹ���վ��MAC��"), _T("ȷ��"), _T(""), _T(""), _T("material//general//warning.png"), size))
	{
		return;
	}

	BOOL bIsHaveSuperMgr = FALSE;
	INT32 nSize = m_pList->GetCount();
	WorkstationItem item;
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
			CLabelUI *pLabMac = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mac")));
			CDuiString strNum = pLabNum->GetText();
			if(0 == strSuperMgr.Compare(strNum))
			{
				bIsHaveSuperMgr = TRUE;
				continue;
			}
			Singleton<CMzdIOMgr>::Instance().ClearMac(strNum.GetData());
			WorkstationInfo itemData;
			Singleton<CMzdIOMgr>::Instance().GetSimpleWorStation(_ttoi(strNum), itemData);
			itemData.strMac = _T("");
			Singleton<CMzdIOMgr>::Instance().SetWorkstationSimple(_ttoi(strNum), itemData);
			
			m_pList->GetItem(_ttoi(strNum), item);
			item.strMac = _T("");
			m_pList->SetItem(_ttoi(strNum), item);
		}
	}
	if(bIsHaveSuperMgr)
	{
		//��ʾ���ܲ������Ƴ�;
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("��ʾ"), _T("����������ܹ���վMAC"), NULL, _T("ȷ��"), _T(""), _T("material//general//warning.png"), size);
		return;
	}

	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //ͬ��INI
	{
// 		Util::Log::Error(_T("MZSkin"), _T("[error]�������վMACʱ�������ϴ������������\r\n"));
// 		return;
	}
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd())
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]�������վMACʱ������ͬ�������������\r\n"));
	}
//	::PostMessage(m_hWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
}
void CWorkstationFrame::OnWakeUp()
{
	INT32 nSize = m_pList->GetCount();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)(m_pList->GetItemAt(nIndex));
		if(pElement->IsSelected())
		{
			CLabelUI *pLabNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
			CDuiString strNum = pLabNum->GetText();
			Singleton<CMzdIOMgr>::Instance().WakeUpWorkStation(strNum.GetData());
		}
	}
}
void CWorkstationFrame::OnLoadWorkstation()
{
	BOOL bOk = Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
	if(FALSE == bOk)
	{
		Util::Log::Error(_T("MZSkin"), _T("[error]���¼��ع���վʱ������ͬ�������������\r\n"));
		return ;
	}
	Singleton<CMzdIOMgr>::Instance().WorkstationLoadData();
	CFrontButtonUI *pSupBtn = static_cast<CFrontButtonUI *>(m_PaintManager.FindControl(_T("bt_sup")));
	CString strSuperNum = Singleton<CMzdIOMgr>::Instance().GetSuperMgr();

	map<CString, UINT32> mapStatus;
	Singleton<CMzdIOMgr>::Instance().GetWorkstationStatus(mapStatus);
	map<CString, UINT32>::iterator itEnd = mapStatus.end();

	UINT32 u32SuperNum = _ttoi(strSuperNum);
	if(0 < u32SuperNum)
	{
		pSupBtn->SetText(_T("ȡ������"));
		pSupBtn->SetEnabled(false);
	}
	else
	{
		pSupBtn->SetText(_T("������"));
		pSupBtn->SetEnabled(true);
	}

	map<UINT32, WorkstationInfo> mapWorkstation;
	m_pList = static_cast<CWorkstationContactList*>(m_PaintManager.FindControl(_T("lt_workstation")));
	m_pList->RemoveAll();
	Singleton<CMzdIOMgr>::Instance().GetAllWorkstation(mapWorkstation);
	map<UINT32, WorkstationInfo>::iterator itor = mapWorkstation.begin();
	while(itor != mapWorkstation.end())
	{
		WorkstationItem item;
		CString strNum;
		strNum.Format(_T("%u"), itor->first);
		map<CString, UINT32>::iterator itFind = mapStatus.find(strNum);
		if(u32SuperNum == itor->second.u32Num && itEnd != itFind && 1 == itFind->second)
			item.strIcon = _T("material/listbtn/super02.png");
		else if(u32SuperNum == itor->second.u32Num && itEnd != itFind && 0 == itFind->second)
			item.strIcon = _T("material/listbtn/super01.png");
		else if(itEnd != itFind && 1 == itFind->second)
			item.strIcon = _T("material/listbtn/computer02.png");
		else if(itEnd != itFind && 0 == itFind->second)
			item.strIcon = _T("material/listbtn/computer01.png");
		item.strIP = itor->second.strIP;
		item.strMac = itor->second.strMac;
		item.strMirrorFile = itor->second.strMirrorFile;
		item.strMirrorIP = itor->second.strMirrorIP;
		item.strName = itor->second.strName;
		item.strReturnWDir = itor->second.strReturnWDir;
		item.u32Num = itor->second.u32Num;
		item.u32Size = _ttoi(itor->second.strSize);
		item.u8Status = itor->second.u8Status;
		m_pList->InsertItem(itor->first, item);
		itor++;
	}
	event::UpdateStatusBar().run();
}
}
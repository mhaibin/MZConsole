#include "StdAfx.h"
#include "AddWorkstationWnd.h"
#include "Singleton.h"
#include "UtilLog.h"
#include <ContactList\ConfigContactList.h>
#include "..\Global\Control\EditCombUI.h"
#include <SkinInner/SkinAPI.h>
#include <SharedUI/Interface/SharedBase.h>
#include "../Global/Common/Global.h"
#include "..\Server\mzdio\MzdIOMgr.h"
#include "..\Server\WorkstationInfoMgr.h"
namespace DuiLib{
	

CAddWorkstationWnd::CAddWorkstationWnd()
{
	m_pPDI = NULL;
	m_EditNum = NULL;
	m_EditSize = NULL;
	m_EditNumLen = NULL;
	m_EditFName = NULL;
	m_EditBName = NULL;
	m_EditDelay = NULL;
	m_EditName = NULL;
	m_EditMac = NULL;
	m_ChkBoxRestore = NULL;
	m_ipStartIP = NULL;
	m_ipDNSI1 = NULL;
	m_ipGetway = NULL;
	m_ipMask = NULL;
	m_ipDNSI2 = NULL;
	m_pMenuList1 = NULL;
	m_pMenuList2 = NULL;
	m_pMenuList3 = NULL;
	m_pMenuList4 = NULL;
	m_u32MinIndex = 0;
	m_pServerNameList = NULL;
	m_pDiskPathList = NULL;
	m_u32Flag = 0;
	m_pCurMDSelElement = NULL;
	m_hParentWnd = NULL;

	m_WksSize[CString(_T("-1"))] = 0;
	m_WksSize[CString(_T("128"))] = 1;
	m_WksSize[CString(_T("256"))] = 2;
	m_WksSize[CString(_T("512"))] = 3;
	m_WksSize[CString(_T("1024"))] = 4;
	m_WksSize[CString(_T("2048"))] = 5;
	m_WksSize[CString(_T("4096"))] = 6;
	m_WksSize[CString(_T("0"))] = 7;
}

CAddWorkstationWnd::~CAddWorkstationWnd(void)
{
	if(m_pPDI)
	{
		delete m_pPDI;
		m_pPDI = NULL;
	}
	if(m_pServerNameList)
	{
		delete m_pServerNameList;
		m_pServerNameList = NULL;
	}
	if(m_pDiskPathList)
	{
		delete m_pDiskPathList;
		m_pDiskPathList = NULL;
	}
}

CDuiString CAddWorkstationWnd::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CAddWorkstationWnd::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CAddWorkstationWnd::GetSkinFile()
{
	return _T("xml\\AddWorkstationWnd.xml");
}

LPCTSTR CAddWorkstationWnd::GetWindowClassName(void) const
{
	return _T("AddWorkstationWnd");
}

void CAddWorkstationWnd::InitWindow()
{
	Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
	InitPDIlist();
	InitServerList();
	m_u32MinIndex = Singleton<CMzdIOMgr>::Instance().GetWorkstationMaxNum() + 1;
	CString strStartIP, strGetway, strDNSI1, strDNSI2, strMask;
	WorkstationInfo sInfo;
	if(0 == m_u32Flag)
	{
		strStartIP = Singleton<CMzdIOMgr>::Instance().GetSevDefIP();
		Singleton<CMzdIOMgr>::Instance().GetNetIP(strGetway, strDNSI1, strDNSI2, strMask);
	}
	else
	{
		vector<CString> vecSelWorkstation;
		Singleton<CWorkstationInfoMgr>::Instance().GetSelWorkstation(vecSelWorkstation);
		if(0 < vecSelWorkstation.size())
			Singleton<CMzdIOMgr>::Instance().GetSimpleWorStation(_ttoi(vecSelWorkstation[0]), sInfo);//修改多个工作站的时候，界面的初始数据显示第一个条件到容器中的数据信息
		else
			Singleton<CMzdIOMgr>::Instance().GetSimpleWorStation(m_u32Flag, sInfo);
		m_u32MinIndex = sInfo.u32Num;
		strStartIP = sInfo.strIP;
		strGetway = sInfo.strGetway;
		strDNSI1 = sInfo.strDNSI1;
		strDNSI2 = sInfo.strDNSI2;
		strMask = sInfo.strMask;
	}
	CLabelUI *pLabTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title_name")));
	m_EditNum = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_num")));
	m_EditSize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_size")));
	m_EditNumLen = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_numlen")));
	m_EditFName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_fname")));
	m_EditBName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_bname")));
	m_EditDelay = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_delay")));
	m_EditName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_name")));
	m_EditMac = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_mac")));
	CButtonUI* pBtn_numadd = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_numAdd")));
	CButtonUI* pBtn_numsub = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_numSub")));
	CButtonUI* pBtn_sizeadd = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_sizeAdd")));
	CButtonUI* pBtn_sizesub = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_sizeSub")));
	CLabelUI* pLab_IP = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_startip")));
	CEditCombUI *pEditCombPDI = static_cast<CEditCombUI*>(m_PaintManager.FindControl(_T("EdtComb_pdi")));
	CComboUI *pCombBuffer = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comb_buffer")));
	m_ChkBoxRestore = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chb_autorestore")));
	m_ipStartIP = static_cast<CIPAddressExUI*>(m_PaintManager.FindControl(_T("edt_startip")));
	m_ipDNSI1 = static_cast<CIPAddressExUI*>(m_PaintManager.FindControl(_T("edt_dnsi1")));
	m_ipGetway = static_cast<CIPAddressExUI*>(m_PaintManager.FindControl(_T("edt_getway")));
	m_ipMask = static_cast<CIPAddressExUI*>(m_PaintManager.FindControl(_T("edt_mask")));
	m_ipDNSI2 = static_cast<CIPAddressExUI*>(m_PaintManager.FindControl(_T("edt_dnsi2")));
	CHorizontalLayoutUI *pHorLayoutNumlen = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("Hor_size_numlen")));
	CHorizontalLayoutUI *pHorLayoutFBName = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hor_fbname")));
	CVerticalLayoutUI *pVerLayoutModify = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("ver_modify")));

	//判断是添加还是修改
	if(0 == m_u32Flag)
	{
		pHorLayoutNumlen->SetVisible(true);
		pHorLayoutFBName->SetVisible(true);
		pVerLayoutModify->SetVisible(false);
		pLabTitle->SetText(_T("批量添加工作站"));
	}
	else
	{
		pHorLayoutNumlen->SetVisible(false);
		pHorLayoutFBName->SetVisible(false);
		pVerLayoutModify->SetVisible(true);
		pBtn_numadd->SetVisible(false);
		pBtn_numsub->SetVisible(false);
		m_EditNum->SetEnabled(false);
		pLab_IP->SetText(_T("工作站IP"));
		pEditCombPDI->SetText(sInfo.strDPI);
		CString str;
		pCombBuffer->SelectItem(m_WksSize[sInfo.strSize]);
		str.Format(_T("%u"), sInfo.u32Delay);
		m_EditDelay->SetText(str);
		m_ChkBoxRestore->SetCheck((1 == sInfo.u8Status)?true:false);
		pLabTitle->SetText(_T("修改工作站"));
	}


	CString strNum;
	strNum.Format(_T("%u"), m_u32MinIndex);
	m_EditNum->SetText(strNum);
	m_ipStartIP->SetIP(strStartIP);
	m_ipGetway->SetIP(strGetway);
	m_ipDNSI1->SetIP(strDNSI1);
	m_ipDNSI2->SetIP(strDNSI2);
	m_ipMask->SetIP(strMask);
	m_EditMac->SetText(sInfo.strMac);
	m_EditName->SetText(sInfo.strName);

	//多选的情况
	if(1 < Singleton<CWorkstationInfoMgr>::Instance().GetSelCount())
	{
		pHorLayoutNumlen->SetVisible(true);
		pHorLayoutFBName->SetVisible(true);
		pVerLayoutModify->SetVisible(false);
		m_EditNum->SetText(_T(""));
		m_EditSize->SetText(_T(""));
		m_EditNum->SetEnabled(false);
		m_EditSize->SetEnabled(false);
		pBtn_numadd->SetEnabled(false);
		pBtn_numsub->SetEnabled(false);
		pBtn_sizeadd->SetEnabled(false);
		pBtn_sizesub->SetEnabled(false);
		m_ipStartIP->SetEnabled(false);
	}

	//列表控件初始化
	m_pMenuList1 = static_cast<CDiskInfoContactList*>(m_PaintManager.FindControl(_T("lt_diskporperty1")));
	m_pMenuList2 = static_cast<CDiskInfoContactList*>(m_PaintManager.FindControl(_T("lt_diskporperty2")));
	m_pMenuList3 = static_cast<CDiskInfoContactList*>(m_PaintManager.FindControl(_T("lt_diskporperty3")));
	m_pMenuList4 = static_cast<CDiskInfoContactList*>(m_PaintManager.FindControl(_T("lt_diskporperty4")));
	DiskPorpertyItem item1, item2, item3,item4;
	item1.strDiskName = _T("系统盘");
	item2.strDiskName = _T("数据盘1");
	item3.strDiskName = _T("数据盘2");
	item4.strDiskName = _T("数据盘3");
	m_pMenuList1->InsertItem(item1.strDiskName, item1);
	m_pMenuList1->InsertItem(item2.strDiskName, item2);
	m_pMenuList1->InsertItem(item3.strDiskName, item3);
	m_pMenuList1->InsertItem(item4.strDiskName, item4);

	m_pMenuList2->InsertItem(item1.strDiskName, item1);
	m_pMenuList2->InsertItem(item2.strDiskName, item2);
	m_pMenuList2->InsertItem(item3.strDiskName, item3);
	m_pMenuList2->InsertItem(item4.strDiskName, item4);

	m_pMenuList3->InsertItem(item1.strDiskName, item1);
	m_pMenuList3->InsertItem(item2.strDiskName, item2);
	m_pMenuList3->InsertItem(item3.strDiskName, item3);
	m_pMenuList3->InsertItem(item4.strDiskName, item4);

	m_pMenuList4->InsertItem(item1.strDiskName, item1);
	m_pMenuList4->InsertItem(item2.strDiskName, item2);
	m_pMenuList4->InsertItem(item3.strDiskName, item3);
	m_pMenuList4->InsertItem(item4.strDiskName, item4);

	if(0 < m_u32Flag)
	{
		InitMenuDiskInfo();
	}

	event::ExitProcessEvn().connect(this, &CAddWorkstationWnd::ExitProcessEvn);
}

CControlUI* CAddWorkstationWnd::CreateControl(LPCTSTR pstrClass)
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

LRESULT CAddWorkstationWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_ADDWORKSTATIONERROR_MSG == uMsg)
	{
		SIZE size;
		size.cx = 50;
		size.cy = 40;
		box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), m_strError, NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CAddWorkstationWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CAddWorkstationWnd::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}
void CAddWorkstationWnd::Notify(TNotifyUI& msg)
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
		else if(0 == strName.Compare(_T("opt_folder")))
		{
			COptionUI* pFolder = (COptionUI*)msg.pSender;
			CTreeViewUI *pTreeDisk = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree_disk")));
			CTreeNodeUI* pItem = (CTreeNodeUI*)pFolder->GetParent()->GetParent();
			pTreeDisk->SetItemExpand(!(pFolder->IsSelected()), pItem);
		}
		else
		{
			OnCheck(msg);
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
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if(0 == msg.pSender->GetName().Compare(_T("comb_diskmodel")))
		{
			CListContainerElementUI *pElementUI = (CListContainerElementUI *)(msg.pSender->GetParent()->GetParent());
			CComboUI *pCombModel = (CComboUI *)(msg.pSender);
			INT32 nModel = pCombModel->GetCurSel();
			CDiskInfoContactList	*pList = (CDiskInfoContactList *)(pElementUI->GetOwner());
			CLabelUI *pLabDiskName = static_cast<CLabelUI *>(pElementUI->FindSubControl(_T("leb_diskname")));
			CDuiString strDiskName = pLabDiskName->GetText();
			DiskPorpertyItem item;
			pList->GetItem(CString(strDiskName.GetData()), item);
			item.nModel = nModel;
			pList->SetItem(CString(strDiskName.GetData()), item);
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMDBCLICK)
	{
		if(0 <= msg.pSender->GetName().Find(_T("nd_menu")))
		{
			CTreeNodeUI *pNode = (CTreeNodeUI *)(msg.pSender);
			COptionUI *pOptFolder = static_cast<COptionUI *>(pNode->FindSubControl(_T("opt_folder")));
			pOptFolder->Selected(!pOptFolder->IsSelected());
			CTreeViewUI *pTreeDisk = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree_disk")));
			pTreeDisk->SetItemExpand(pOptFolder->IsSelected(), pNode);
		}
	}
}
void CAddWorkstationWnd::OnCheck(TNotifyUI& msg)
{
	CDuiString	strName = msg.pSender->GetName();
	if(0 == strName.Compare(_T("btn_numAdd")))
		AddNum(m_EditNum);
	else if(0 == strName.Compare(_T("btn_numSub")))
		SubNum(m_EditNum, m_u32MinIndex);
	if(0 == strName.Compare(_T("btn_sizeAdd")))
		AddNum(m_EditSize);
	else if(0 == strName.Compare(_T("btn_sizeSub")))
		SubNum(m_EditSize, 0);
	if(0 == strName.Compare(_T("btn_numlenAdd")))
		AddNum(m_EditNumLen);
	else if(0 == strName.Compare(_T("btn_numlenSub")))
		SubNum(m_EditNumLen, 0);
	if(0 == strName.Compare(_T("btn_delayAdd")))
		AddNum(m_EditDelay);
	else if(0 == strName.Compare(_T("btn_delaySub")))
		SubNum(m_EditDelay, 0);
	else if(0 == strName.Compare(_T("btn_apply")))
		OnClickApply();
	else if(0 == msg.pSender->GetParent()->GetName().Compare(_T("comb_diskmodel")))
	{

	}

}
void CAddWorkstationWnd::OnDropDown(TNotifyUI& msg)
{
	if(0 == msg.pSender->GetName().Compare(_T("EdtComb_pdi")))
		m_pPDI->Init(msg.pSender, CDuiString(_T("ECElement_Pdi")));
	else if(0 == msg.pSender->GetName().Compare(_T("comb_servername")))
		m_pServerNameList->Init(msg.pSender, CDuiString(_T("CElement_ServerList")));
	else if(0 == msg.pSender->GetName().Compare(_T("comb_diskpath")))
	{
		m_pCurMDSelElement = (CListContainerElementUI *)(msg.pSender->GetParent()->GetParent());
		CComboUI *pCombName = static_cast<CComboUI *>(m_pCurMDSelElement->FindSubControl(_T("comb_servername")));
		InitDiskPathList(pCombName->GetTextEx());
		if(m_pDiskPathList)
			m_pDiskPathList->Init(msg.pSender, CDuiString(_T("CElement_DiskPathList")));
	}
}

//从分辨率大小列表中删除一项
void CAddWorkstationWnd::OnDeleteResolutionWHItem(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();
	if(0 == msg.pSender->GetParent()->GetText().Compare(_T("ECElement_Pdi")))
	{	
		m_pPDI->RemoveItem(strName);
	}
	else if(0 == msg.pSender->GetParent()->GetText().Compare(_T("CElement_ServerList")))
	{
		m_pServerNameList->RemoveItem(strName);
	}
	else if(0 == msg.pSender->GetParent()->GetText().Compare(_T("CElement_DiskPathList")))
	{
		m_pDiskPathList->RemoveItem(strName);
	}
}
//从分辨率大小列表中选择一项
void CAddWorkstationWnd::OnSeleteResolutionWHItem(TNotifyUI& msg)
{
	CDuiString szText = msg.pSender->GetText();
	if(0 == msg.pSender->GetParent()->GetText().Compare(_T("ECElement_Pdi")))
	{
		CEditCombUI *pEditCombPDI = static_cast<CEditCombUI*>(m_PaintManager.FindControl(_T("EdtComb_pdi")));
		pEditCombPDI->SetText(szText);
	}
	else if(0 == msg.pSender->GetParent()->GetText().Compare(_T("CElement_ServerList")))
	{
		CComboUI * pControlUI = (CComboUI *)(m_pServerNameList->GetOwner());
		pControlUI->SetText(szText);
		InitDiskPathList(szText.GetData());
		//设置当前选择的Item信息
		CListContainerElementUI *pElementUI = (CListContainerElementUI *)(pControlUI->GetParent()->GetParent());
		CDiskInfoContactList	*pList = (CDiskInfoContactList *)(pElementUI->GetOwner());
		CLabelUI *pLabDiskName = static_cast<CLabelUI *>(pElementUI->FindSubControl(_T("leb_diskname")));
		CDuiString strDiskName = pLabDiskName->GetText();
		DiskPorpertyItem item;
		pList->GetItem(CString(strDiskName.GetData()), item);
		item.strServerName = szText;
		pList->SetItem(CString(strDiskName.GetData()), item);
	}
	else if(0 == msg.pSender->GetParent()->GetText().Compare(_T("CElement_DiskPathList")))
	{
		CControlUI * pControlUI = m_pDiskPathList->GetOwner();
		pControlUI->SetText(szText);
		//设置当前选择的Item信息
		CListContainerElementUI *pElementUI = (CListContainerElementUI *)(pControlUI->GetParent()->GetParent());
		CDiskInfoContactList	*pList = (CDiskInfoContactList *)(pElementUI->GetOwner());
		CLabelUI *pLabDiskName = static_cast<CLabelUI *>(pElementUI->FindSubControl(_T("leb_diskname")));
		CDuiString strDiskName = pLabDiskName->GetText();
		DiskPorpertyItem item;
		pList->GetItem(CString(strDiskName.GetData()), item);
		item.strDiskPath = szText;
		pList->SetItem(CString(strDiskName.GetData()), item);
	}
}
void CAddWorkstationWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CAddWorkstationWnd::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
void CAddWorkstationWnd::InitPDIlist()
{
	m_pPDI = new CResolutionWHListFrame();
	m_pPDI->Create(m_hWnd, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	m_pPDI->ShowWindow(false);

	m_pPDI->AddItem(_T("-1(自动)"));
	m_pPDI->AddItem(_T("1024*768*75Hz"));
	m_pPDI->AddItem(_T("1280*800*75Hz"));
	m_pPDI->AddItem(_T("1440*900*75Hz"));
	m_pPDI->AddItem(_T("1920*1080*75Hz"));
	m_pPDI->AddItem(_T("未启用"));
}
void CAddWorkstationWnd::InitServerList()
{
	m_pServerNameList = new CResolutionWHListFrame();
	m_pServerNameList->Create(m_hWnd, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	m_pServerNameList->ShowWindow(false);

	map<CString, ServerItem> mapServer;
	Singleton<CMzdIOMgr>::Instance().GetAllServer(mapServer);
	map<CString, ServerItem>::iterator itor = mapServer.begin();
	while(itor != mapServer.end())
	{
		m_pServerNameList->AddItem(itor->first);
		itor++;
	}
}
void CAddWorkstationWnd::InitDiskPathList(LPCTSTR pstrServerName)
{
	if(m_pDiskPathList)
	{
		m_pDiskPathList->RemoveAllItem();
	}
	if(CString(pstrServerName).IsEmpty())
		return;

	if(NULL == m_pDiskPathList)
	{
		m_pDiskPathList = new CResolutionWHListFrame();
		m_pDiskPathList->Create(m_hWnd, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	}
	m_pDiskPathList->ShowWindow(false);
	
	vector<CString> vecDiskPath;
	Singleton<CMzdIOMgr>::Instance().GetServerDiskData(pstrServerName, vecDiskPath);
	INT32 nSize = vecDiskPath.size();

	BOOL bFilter = FALSE;
	if(m_pCurMDSelElement)
	{
		CLabelUI *pLabDiskName = static_cast<CLabelUI *>(m_pCurMDSelElement->FindSubControl(_T("leb_diskname")));
		CString strDiskName = pLabDiskName->GetText();
		if(0 == strDiskName.Compare(_T("系统盘")))
			bFilter = TRUE;
	}
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		if(0 > vecDiskPath[nIndex].Find(_T(".img")) && bFilter)
			continue;
		m_pDiskPathList->AddItem(vecDiskPath[nIndex]);
	}
}

void CAddWorkstationWnd::AddNum(CControlUI *pControl, INT32 nMaxVal)
{
	CDuiString strNum = pControl->GetText();
	INT32 nNum = _ttoi(strNum.GetData());
	nNum++;
	if(nNum <= nMaxVal)
	{
		CString strAdd;
		strAdd.Format(_T("%d"), nNum);
		pControl->SetText(strAdd);
	}
}
void CAddWorkstationWnd::SubNum(CControlUI *pControl, INT32 nMinVal)
{
	CDuiString strNum = pControl->GetText();
	INT32 nNum = _ttoi(strNum.GetData());
	nNum--;
	if(nNum >= nMinVal)
	{
		CString strAdd;
		strAdd.Format(_T("%d"), nNum);
		pControl->SetText(strAdd);
	}
}
void CAddWorkstationWnd::InitMenuDiskInfo()
{
	map<CString, vector<MenuDiskInfo>> mapMenuDiskInfo;
	Singleton<CMzdIOMgr>::Instance().GetWorkstationMenuDiskInfo(m_u32Flag, mapMenuDiskInfo);
	map<CString, vector<MenuDiskInfo>>::iterator itor = mapMenuDiskInfo.begin();
	while(itor != mapMenuDiskInfo.end())
	{
		CString strControlName;
		strControlName.Format(_T("lt_diskporperty%s"), itor->first);
		CDiskInfoContactList *pDiskList = static_cast<CDiskInfoContactList*>(m_PaintManager.FindControl(strControlName));
		INT32 nSize = itor->second.size();
		for(INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			CString strDiskName;
			DiskPorpertyItem item;
			strDiskName = itor->second[nIndex].strDiskName;
			INT32 nPos = strDiskName.Find(_T("DskSet"));
			CString strI = strDiskName.Right(strDiskName.GetLength() - nPos - 6);
			UINT32 u32Num = _ttoi(strI.GetString())-1;
			if(0 == u32Num)
			{
				strDiskName = _T("系统盘");
			}
			else
			{
				strDiskName.Format(_T("数据盘%d"), u32Num);
			}
			pDiskList->GetItem(strDiskName, item);
			item.strDiskPath = itor->second[nIndex].strPath;
			item.nModel = _ttoi(itor->second[nIndex].strModel);
			item.strServerName = itor->second[nIndex].strServer;
			pDiskList->SetItem(strDiskName, item);
		}
		itor++;
	}
}

void CAddWorkstationWnd::OnClickApply()
{
	if(0 == AddNewWorkstation())
	{
		::PostMessage(m_hParentWnd, UW_LOADWORKSTATION_MSG, NULL, NULL);
		Close(IDCANCEL);
	}
}
INT32 CAddWorkstationWnd::AddNewWorkstation()
{
	CEditCombUI *pEditCombPDI = static_cast<CEditCombUI*>(m_PaintManager.FindControl(_T("EdtComb_pdi")));
	CComboUI *pCombBuffer = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comb_buffer")));
	CDuiString strNum = m_EditNum->GetText();
	CDuiString strFName = m_EditFName->GetText();
	CDuiString strBName = m_EditBName->GetText();
	CDuiString strStartIP = m_ipStartIP->GetIP();
	INT32 n32Num = _ttoi(strNum);
	INT32 n32Size = _ttoi(m_EditSize->GetText());
	INT32 n32Len = _ttoi(m_EditNumLen->GetText());

	//获取窗口界面数据；
	WorkstationInfo itemData;
	itemData.strName = m_EditName->GetText();
	itemData.strBName = strBName;
	itemData.strDNSI1 = m_ipDNSI1->GetText();
	if(0 == itemData.strDNSI1.Compare(_T("0.0.0.0")))
		itemData.strDNSI1 = _T("");
	itemData.strDNSI2 = m_ipDNSI2->GetText();
	if(0 == itemData.strDNSI2.Compare(_T("0.0.0.0")))
		itemData.strDNSI2 = _T("");
	itemData.strDPI = pEditCombPDI->GetText();
	itemData.strFName = m_EditFName->GetText();
	itemData.strGetway = m_ipGetway->GetText();
	if(0 == itemData.strGetway.Compare(_T("0.0.0.0")))
		itemData.strGetway = _T("");
	itemData.strIP = m_ipStartIP->GetText();
	if(0 == itemData.strIP.Compare(_T("0.0.0.0")))
		itemData.strIP = _T("");
	itemData.strMac = m_EditMac->GetText();
	itemData.strMask = m_ipMask->GetText();
	if(0 == itemData.strMask.Compare(_T("0.0.0.0")))
		itemData.strMask = _T("");
	itemData.strNumLen = m_EditNumLen->GetText();
	itemData.strSize = pCombBuffer->GetText();
	UINT32 nSel = pCombBuffer->GetCurSel();
	itemData.u32Delay = _ttoi(m_EditDelay->GetText());
	itemData.u8Status = m_ChkBoxRestore->GetCheck()==true?1:0;

	//判断用户是否没有输入信息

	if(0 >= n32Num)
	{
		m_strError = _T("请输入正确的工作站编号");
		::PostMessage(m_hWnd, WM_ADDWORKSTATIONERROR_MSG, NULL, NULL);
		return -1;
	}
	if(0 >= n32Size)
	{
		m_strError = _T("请输入需要添加工作站的数量");
		::PostMessage(m_hWnd, WM_ADDWORKSTATIONERROR_MSG, NULL, NULL);
		return -1;
	}
	if(itemData.strIP.IsEmpty())
	{
		m_strError = _T("请输入工作站的起始IP");
		::PostMessage(m_hWnd, WM_ADDWORKSTATIONERROR_MSG, NULL, NULL);
		return -1;
	}
	//获取窗口菜单数据；
	CDiskInfoContactList *arrayList[4]={m_pMenuList1, m_pMenuList2, m_pMenuList3, m_pMenuList4};
	map<CString, vector<MenuDiskInfo>> mapMenuDiskInfo;
	for (INT32 nIndex=0; nIndex<4; nIndex++)
	{
		DiskPorpertyItem item1, item2, item3, item4;
		item1.strDiskName = _T("系统盘");
		item2.strDiskName = _T("数据盘1");
		item3.strDiskName = _T("数据盘2");
		item4.strDiskName = _T("数据盘3");
		arrayList[nIndex]->GetItem(item1.strDiskName, item1);
		arrayList[nIndex]->GetItem(item2.strDiskName, item2);
		arrayList[nIndex]->GetItem(item3.strDiskName, item3);
		arrayList[nIndex]->GetItem(item4.strDiskName, item4);
		CString strMenu;
		strMenu.Format(_T("%d"), nIndex+1);
		MenuDiskInfo info;
		if(!item1.strServerName.IsEmpty())
		{
			info.strDiskName = item1.strDiskName;
			info.strModel.Format(_T("%d"), item1.nModel);
			info.strPath = item1.strDiskPath;
			INT32 nPos = info.strPath.Find(_T("<"));
			info.strPath = info.strPath.Mid(0, nPos-1);
			info.strServer = item1.strServerName;
			mapMenuDiskInfo[strMenu].push_back(info);
		}
		if(!item2.strServerName.IsEmpty())
		{
			info.strDiskName = item2.strDiskName;
			info.strModel.Format(_T("%d"), item2.nModel);
			info.strPath = item2.strDiskPath;
			INT32 nPos = info.strPath.Find(_T(" <"));
			info.strPath = info.strPath.Mid(0, nPos-1);
			info.strServer = item2.strServerName;
			mapMenuDiskInfo[strMenu].push_back(info);
		}
		if(!item3.strServerName.IsEmpty())
		{
			info.strDiskName = item3.strDiskName;
			info.strModel.Format(_T("%d"), item3.nModel);
			info.strPath = item3.strDiskPath;
			INT32 nPos = info.strPath.Find(_T("<"));
			info.strPath = info.strPath.Mid(0, nPos-1);
			info.strServer = item3.strServerName;
			mapMenuDiskInfo[strMenu].push_back(info);
		}
		if(!item4.strServerName.IsEmpty())
		{
			info.strDiskName = item4.strDiskName;
			info.strModel.Format(_T("%d"), item4.nModel);
			info.strPath = item4.strDiskPath;
			INT32 nPos = info.strPath.Find(_T("<"));
			info.strPath = info.strPath.Mid(0, nPos-1);
			info.strServer = item4.strServerName;
			mapMenuDiskInfo[strMenu].push_back(info);
		}
	}

	//设置菜单信息,这里itemData数据，不会用工作站名称编号信息，所以直接用第一个添加的工作站信息不会影响数据保存
	vector<CString> vecNewMenu;
	Singleton<CMzdIOMgr>::Instance().SetWorkStationMenuInfo(itemData, mapMenuDiskInfo, vecNewMenu);

	//设置磁盘路径
	Singleton<CMzdIOMgr>::Instance().SetWorkstationDiskInfo(mapMenuDiskInfo);
	
	if(0 == m_u32Flag)
	{
		Singleton<CWorkstationInfoMgr>::Instance().ClearAddWorkstation();
		for (INT32 nIndex=0; nIndex<n32Size; nIndex++)
		{
			WorkstationItem item;
			CString strWks;
			strWks.Format(_T("%d"), n32Num+nIndex);
			CString strWksNum, strWksIP;
			INT32 nCheckRel = Singleton<CMzdIOMgr>::Instance().CheckWorkstationNum(strWks, n32Len, strFName, strBName, strStartIP, 0, strWksNum, strWksIP);
			if (nCheckRel == -1) 
			{
				Util::Log::Error(_T("MZSKin"), _T("[error]添加工作站时，检测工作站编号出错<Rel=%d>\r\n"), nCheckRel);
				break;
			}
			if (nCheckRel == 0)
			{
				Util::Log::Warn(_T("MZSKin"), _T("[warnning]添加工作站时，检测出工作站编号用相同工作站<Rel=%d>\r\n"), nCheckRel);
				continue;
			}
			itemData.strName = strWksNum;
			itemData.u32Num = _ttoi(strWks);
			itemData.strIP = strWksIP;
			Singleton<CMzdIOMgr>::Instance().SetWorkStationToMZD(strWks, strWksNum, strWksIP, itemData, mapMenuDiskInfo,vecNewMenu, 0);
			//添加或修改工作站信息；
			item.strIcon = _T("material/listbtn/computer01.png");
			item.strIP = strStartIP;
			item.strMac = itemData.strMac;
			if(0 < mapMenuDiskInfo[_T("1")].size())
				item.strMirrorFile = mapMenuDiskInfo[_T("1")][0].strServer + _T(" | ") + mapMenuDiskInfo[_T("1")][0].strPath;
			item.strName = strWksNum;
			item.u32Num = _ttoi(strWks);
			if(itemData.strSize == _T("自动"))
				item.u32Size = -1;
			else
				item.u32Size = _ttoi(itemData.strSize);
			item.u8Status = itemData.u8Status;
			Singleton<CWorkstationInfoMgr>::Instance().AddNewWorkstation(item);
			Singleton<CMzdIOMgr>::Instance().AddWorkStation(itemData.u32Num, itemData);
		}
	}
	else if(0 < m_u32Flag)
	{
		Singleton<CWorkstationInfoMgr>::Instance().ClearAddWorkstation();
		vector<CString> vecSelWorkstation;
		Singleton<CWorkstationInfoMgr>::Instance().GetSelWorkstation(vecSelWorkstation);
		INT32 nSize = vecSelWorkstation.size();
		if(1 < nSize)
		{
			for (INT32 nIndex=0; nIndex<nSize; nIndex++)
			{
				WorkstationItem item;
				WorkstationInfo Data;
				Singleton<CMzdIOMgr>::Instance().GetSimpleWorStation(_ttoi(vecSelWorkstation[nIndex]), Data);
				CString strWksNum, strWksIP;
				strWksIP = Data.strIP;
				itemData.strName = vecSelWorkstation[nIndex];
				INT32 nCheckRel = Singleton<CMzdIOMgr>::Instance().CheckWorkstationNum(vecSelWorkstation[nIndex], 0, _T(""), _T(""), _T(""), m_u32Flag, strWksNum, strWksIP);
				if (nCheckRel == -1) 
				{
					Util::Log::Error(_T("MZSKin"), _T("[error]修改工作站时，检测工作站编号出错<Rel=%d>\r\n"), nCheckRel);
					continue;
				}
				if (nCheckRel == 0)
				{
					Util::Log::Warn(_T("MZSKin"), _T("[warnning]修改工作站时，检测出工作站编号用相同工作站<Rel=%d>\r\n"), nCheckRel);
					continue;
				}
				Singleton<CMzdIOMgr>::Instance().SetWorkStationToMZD(vecSelWorkstation[nIndex], strWksNum, strWksIP, itemData, mapMenuDiskInfo, vecNewMenu, _ttoi(vecSelWorkstation[0]));
				//添加或修改工作站信息；
				item.strIP = strStartIP;
				item.strMac = itemData.strMac;
				if(0 < mapMenuDiskInfo[_T("1")].size())
					item.strMirrorFile = mapMenuDiskInfo[_T("1")][0].strServer + _T(" |") + mapMenuDiskInfo[_T("1")][0].strPath;
				item.strName = itemData.strName;
				item.u32Num = _ttoi(vecSelWorkstation[nIndex]);
				if(itemData.strSize == _T("自动"))
					item.u32Size = -1;
				else
					item.u32Size = _ttoi(itemData.strSize);
				item.u8Status = itemData.u8Status;
				Singleton<CWorkstationInfoMgr>::Instance().AddNewWorkstation(item);
				//同步修改本地工作站的信息
				Data.strBName = itemData.strBName;
				Data.strDNSI1 = itemData.strDNSI1;
				Data.strDNSI2 = itemData.strDNSI2;
				Data.strDPI = itemData.strDPI;
				Data.strFName = itemData.strFName;
				Data.strNumLen = itemData.strNumLen;
				Data.strSize = itemData.strSize;
				Data.u32Delay = itemData.u32Delay;
				Data.u8Status = itemData.u8Status;
				Data.strIP = itemData.strIP;
				Data.strMask = itemData.strMask;
				Data.strGetway = itemData.strGetway;
				if(0 < mapMenuDiskInfo[_T("1")].size())
					Data.strMirrorFile = mapMenuDiskInfo[_T("1")][0].strServer + _T(" |") + mapMenuDiskInfo[_T("1")][0].strPath;
				Singleton<CMzdIOMgr>::Instance().SetWorkstationSimple(Data.u32Num, Data);
			}
		}
		else
		{
			WorkstationItem item;
			WorkstationInfo Data;
			Singleton<CMzdIOMgr>::Instance().GetSimpleWorStation(_ttoi(strNum), Data);
			CString strWksNum, strWksIP;
			strWksIP = itemData.strIP;
			INT32 nCheckRel = Singleton<CMzdIOMgr>::Instance().CheckWorkstationNum(strNum, _ttoi(Data.strNumLen), Data.strFName, Data.strBName, Data.strIP, m_u32Flag, strWksNum, strWksIP);
			if (nCheckRel == -1) 
			{
				Util::Log::Error(_T("MZSKin"), _T("[error]修改单个工作站时，检测工作站编号出错<Rel=%d>\r\n"), nCheckRel);
				return 0;
			}
			if (nCheckRel == 0)
			{
				Util::Log::Warn(_T("MZSKin"), _T("[warnning]修改单个工作站时，检测出工作站编号用相同工作站<Rel=%d>\r\n"), nCheckRel);
				return 0;
			}
			Singleton<CMzdIOMgr>::Instance().SetWorkStationToMZD(strNum, strWksNum, strWksIP, itemData, mapMenuDiskInfo, vecNewMenu, m_u32Flag);
			//添加或修改工作站信息；
			item.strIP = strStartIP;
			item.strMac = itemData.strMac;
			if(0 < mapMenuDiskInfo[_T("1")].size())
				item.strMirrorFile = mapMenuDiskInfo[_T("1")][0].strServer + _T(" |") + mapMenuDiskInfo[_T("1")][0].strPath;
			item.strName = itemData.strName;
			item.u32Num = _ttoi(strNum);
			if(itemData.strSize == _T("自动"))
				item.u32Size = -1;
			else
				item.u32Size = _ttoi(itemData.strSize);
			item.u8Status = itemData.u8Status;
			Singleton<CWorkstationInfoMgr>::Instance().AddNewWorkstation(item);
			//同步修改本地工作站的信息
			Data.strBName = itemData.strBName;
			Data.strDNSI1 = itemData.strDNSI1;
			Data.strDNSI2 = itemData.strDNSI2;
			Data.strDPI = itemData.strDPI;
			Data.strFName = itemData.strFName;
			Data.strNumLen = itemData.strNumLen;
			Data.strSize = itemData.strSize;
			Data.u32Delay = itemData.u32Delay;
			Data.u8Status = itemData.u8Status;
			Data.strIP = itemData.strIP;
			Data.strMask = itemData.strMask;
			Data.strGetway = itemData.strGetway;
			if(0 < mapMenuDiskInfo[_T("1")].size())
				Data.strMirrorFile = mapMenuDiskInfo[_T("1")][0].strServer + _T(" |") + mapMenuDiskInfo[_T("1")][0].strPath;
			Singleton<CMzdIOMgr>::Instance().SetWorkstationSimple(Data.u32Num, Data);
		}
	}

	Singleton<CMzdIOMgr>::Instance().CheckUnuserMenu();   //未使用菜单检测
	Singleton<CMzdIOMgr>::Instance().CheckUnuserDisk();   //未使用磁盘检测
	if(TRUE != Singleton<CMzdIOMgr>::Instance().SendUpLoadCmd())     //同步INI
	{
		//Util::Log::Error(_T("MZSKin"), _T("[error]添加或修改单个工作站信息同步信息出错<flag=%u>\r\n"), m_u32Flag);
	}
	return 0;
}
void CAddWorkstationWnd::ModifyWorkstation()
{

}
}
namespace Popup
{
	void OpenAddWorkstationWnd(HWND hWnd, UINT32 u32Flag)
	{
		CString strWndName;
		if(0 == u32Flag)
			strWndName = _T("批量添加工作站");
		else
			strWndName = _T("修改工作站");
		static HWND hAddWorkstationWnd = NULL;
		if (!::IsWindow(hAddWorkstationWnd))
		{
			CAddWorkstationWnd * pAddWorkstationWnd = new CAddWorkstationWnd();
			pAddWorkstationWnd->SetFlag(u32Flag);
			pAddWorkstationWnd->SetWnd(hWnd);
			pAddWorkstationWnd->Create(hWnd, strWndName, WS_POPUPWINDOW, 0, 0, 0, 0, 0);		
			pAddWorkstationWnd->CenterWindow();
			pAddWorkstationWnd->ShowWindow(true, false);
			hAddWorkstationWnd = pAddWorkstationWnd->GetHWND();
		}
		global::FrontWindow(hAddWorkstationWnd);
	}
}
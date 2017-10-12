#include "stdafx.h"
#include "Singleton.h"
#include "ConfigContactList.h"
#include "UtilLZ.h"
#include "..\Server\mzdio\MzdIOMgr.h"

CDiskInfoContactList::CDiskInfoContactList()
:CContactListBase(_T("xml\\DiskPorpertyItem.xml"))
{

}
CDiskInfoContactList::~CDiskInfoContactList()
{

}
LPVOID CDiskInfoContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("DiskInfoContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CDiskInfoContactList::GetClass() const
{
	return __super::GetClass();;
}
void CDiskInfoContactList::OnInsertItem(CControlUI* pItem, DiskPorpertyItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInfoContactList::OnSetItem(CControlUI *pItem, DiskPorpertyItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInfoContactList::ItemChange(CControlUI* pItem, DiskPorpertyItem &item)
{
	if(pItem)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *plabDiskName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("leb_diskname")));
		CComboUI *pcmbServername = static_cast<CComboUI*>(pElement->FindSubControl(_T("comb_servername")));
		CComboUI *pcmbDiskPath = static_cast<CComboUI*>(pElement->FindSubControl(_T("comb_diskpath")));
		CComboUI *pcmbDiskmodel = static_cast<CComboUI*>(pElement->FindSubControl(_T("comb_diskmodel")));
		if(plabDiskName)
		{
			plabDiskName->SetText(item.strDiskName);
		}
		if(pcmbServername)
		{
			pcmbServername->SetText(item.strServerName);
		}
		if(pcmbDiskPath)
		{
			pcmbDiskPath->SetText(item.strDiskPath);
		}
		if(0 == item.strDiskName.Compare(_T("ÏµÍ³ÅÌ")))
		{
			if(pcmbDiskmodel)
			{
				pcmbDiskmodel->SelectItem(item.nModel);
			}
			pcmbDiskmodel->SetEnabled(true);
		}
		else
		{
			pcmbDiskmodel->SetEnabled(false);
		}

	}
}


CDiskInstructionContactList::CDiskInstructionContactList()
:CContactListBase(_T("xml\\DiskInstructionItem.xml"))
{

}
CDiskInstructionContactList::~CDiskInstructionContactList()
{

}
LPVOID CDiskInstructionContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("DiskInstructionContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CDiskInstructionContactList::GetClass() const
{
	return __super::GetClass();;
}
void CDiskInstructionContactList::OnInsertItem(CControlUI* pItem, DiskInstructionItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInstructionContactList::OnSetItem(CControlUI *pItem, DiskInstructionItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInstructionContactList::ItemChange(CControlUI* pItem, DiskInstructionItem &item)
{
	if(pItem)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *plabFileDir = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_filedir")));
		CLabelUI *plabSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_size")));
		CLabelUI *plabModifyTime = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_modifytime")));
		CLabelUI *plabRemark = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_remark")));
		if(plabFileDir)
		{
			plabFileDir->SetText(item.strFileDir);
		}
		if(plabSize)
		{
			plabSize->SetText(item.strSize);
		}
		if(plabModifyTime)
		{
			plabModifyTime->SetText(item.strModifyTime);
		}
		if(plabRemark)
		{
			plabRemark->SetText(item.strRemark);
		}
	}
}
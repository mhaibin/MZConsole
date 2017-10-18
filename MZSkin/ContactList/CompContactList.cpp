#include "stdafx.h"
#include "CompContactList.h"
#include "UtilLZ.h"

CWorkstationContactList::CWorkstationContactList()
:CContactListBase(_T("xml\\WorkstationItem.xml"))
{
}
CWorkstationContactList::~CWorkstationContactList()
{

}
LPVOID CWorkstationContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("WorkstationContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}


LPCTSTR CWorkstationContactList::GetClass() const
{
	return __super::GetClass();
}
void CWorkstationContactList::OnInsertItem(CControlUI* pItem, WorkstationItem &item)
{
	ItemChange(pItem, item);
}
void CWorkstationContactList::OnSetItem(CControlUI *pItem, WorkstationItem &item)
{
	ItemChange(pItem, item);
}
void CWorkstationContactList::ItemChange(CControlUI* pItem, WorkstationItem &item)
{
	if(pItem)
	{
		CString str;
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *pIcon = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_icon")));
		CLabelUI *pNum = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_num")));
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pMac = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mac")));
		CLabelUI *pIP = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_ip")));
		CLabelUI *pStatus = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_status")));
		CLabelUI *pSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_size")));
		CLabelUI *pMIP = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mip")));
		CLabelUI *pMFile = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mfile")));
		CLabelUI *pRWDir = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_rwdir")));
		if(pIcon)
		{
			pIcon->SetBkImage(item.strIcon);
		}
		if(pNum)
		{
			str.Format(_T("%u"), item.u32Num);
			pNum->SetText(str);
		}
		if(pName)
		{
			pName->SetText(item.strName);
		}
		if(pMac)
		{
			pMac->SetText(item.strMac);
		}
		if(pIP)
		{
			pIP->SetText(item.strIP);
		}
		if(pStatus)
		{
			str.Format(_T("%u"), item.u8Status);
			pStatus->SetText(str);
		}
		if(pSize)
		{
			str.Format(_T("%d"), item.u32Size);
			pSize->SetText(str);
		}
		if(pMIP)
		{
			pMIP->SetText(item.strMirrorIP);
		}
		if(pMFile)
		{
			pMFile->SetText(item.strMirrorFile);
		}
		if(pRWDir)
		{
			pRWDir->SetText(item.strReturnWDir);
		}
	}
}

CServerContactList::CServerContactList()
:CContactListBase(_T("xml\\ServerItem.xml"))
{

}
CServerContactList::~CServerContactList()
{

}
LPVOID CServerContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("ServerContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CServerContactList::GetClass() const
{
	return __super::GetClass();;
}
void CServerContactList::OnInsertItem(CControlUI* pItem, ServerItem &item)
{
	ItemChange(pItem, item);
}
void CServerContactList::OnSetItem(CControlUI *pItem, ServerItem &item)
{
	ItemChange(pItem, item);
}
void CServerContactList::ItemChange(CControlUI* pItem, ServerItem &item)
{
	if(pItem)
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *pIcon = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_icon")));
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pIP = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_ip")));
		CLabelUI *pWtPath = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_wtpath")));
		CLabelUI *pBuffParam = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_buffparam")));
		CLabelUI *pSpName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_spname")));
		if(pIcon)
		{
			if(0 == item.u8Status)
			{
				pIcon->SetBkImage(_T("material/listbtn/server01.png"));
			}
			else
			{
				pIcon->SetBkImage(_T("material/listbtn/server02.png"));
			}
		}
		if(pName)
		{
			pName->SetText(item.strName);
		}
		if(pIP)
		{
			pIP->SetText(item.strIP);
		}
		if(pWtPath)
		{
			pWtPath->SetText(item.strReturnWDir);
		}
		if(pBuffParam)
		{
			pBuffParam->SetText(item.strBuffPrama);
		}
		if(pSpName)
		{
			pSpName->SetText(item.strSpareName);
		}
	}
}

CPropertyContactList::CPropertyContactList()
:CContactListBase(_T("xml\\PropertyItem.xml"))
{

}
CPropertyContactList::~CPropertyContactList()
{

}
LPVOID CPropertyContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("PropertyContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CPropertyContactList::GetClass() const
{
	return __super::GetClass();;
}
void CPropertyContactList::OnInsertItem(CControlUI* pItem, PropertyItem &item)
{
	ItemChange(pItem, item);
	pItem->SetVisible(false);
}
void CPropertyContactList::OnSetItem(CControlUI *pItem, PropertyItem &item)
{
	ItemChange(pItem, item);
}
void CPropertyContactList::ItemChange(CControlUI* pItem, PropertyItem &item)
{
	if(pItem)
	{
		CString str;
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *pType = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_type")));
		CLabelUI *pUse = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_use")));
		CLabelUI *pRead = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_read")));
		CLabelUI *pWrite = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_write")));
		CLabelUI *pPFLine = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_pfline")));
		CLabelUI *pAllRead = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_allread")));
		CLabelUI *pAllWrite = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_allwrite")));
		CLabelUI *pTotal = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_tatol")));
		if(pType)
		{
			str.Format(_T("%u"), item.u8Type);
			pType->SetText(str);
		}
		if(pUse)
		{
			str.Format(_T("%u"), item.u32Use);
			pUse->SetText(str);
		}
		if(pRead)
		{
			str.Format(_T("%u"), item.u32ReadSpeed);
			pRead->SetText(str);
		}
		if(pWrite)
		{
			str.Format(_T("%u"), item.u32WriteSpeed);
			pWrite->SetText(str);
		}
		if(pPFLine)
		{
			str.Format(_T("%u"), item.u32PF);
			pPFLine->SetText(str);
		}
		if(pAllRead)
		{
			str.Format(_T("%u"), item.u32AllRead);
			pAllRead->SetText(str);
		}
		if(pAllWrite)
		{
			str.Format(_T("%u"), item.u32AllWrite);
			pAllWrite->SetText(str);
		}
		if(pTotal)
		{
			str.Format(_T("%u"), item.u32Surplus);
			pTotal->SetText(str);
		}
	}
}

CStorageUseContactList::CStorageUseContactList()
:CContactListBase(_T("xml\\StorageUseItem.xml"))
{

}
CStorageUseContactList::~CStorageUseContactList()
{

}
LPVOID CStorageUseContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("StorageUseContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CStorageUseContactList::GetClass() const
{
	return __super::GetClass();;
}
void CStorageUseContactList::OnInsertItem(CControlUI* pItem, StorageUseItem &item)
{
	ItemChange(pItem, item);
}
void CStorageUseContactList::OnSetItem(CControlUI *pItem, StorageUseItem &item)
{
	ItemChange(pItem, item);
}
void CStorageUseContactList::ItemChange(CControlUI* pItem, StorageUseItem &item)
{
	if(pItem)
	{
		CString str;
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_size")));
		CLabelUI *pBuffType = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_bufftype")));
		CLabelUI *pBuffUser = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_buffuser")));
		CLabelUI *pUseSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_usesize")));
		CLabelUI *pAllRead = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_allread")));
		CLabelUI *pReadHit = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_readhit")));
		CLabelUI *pAllWrite = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_allwrite")));
		CLabelUI *pWriteHit = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_writehit")));
		if(pName)
		{
			pName->SetText(item.strName);
		}
		if(pSize)
		{
			str.Format(_T("%u"), item.u32Size);
			pSize->SetText(str);
		}
		if(pBuffType)
		{
			str.Format(_T("%u"), item.u8Type);
			pBuffType->SetText(str);
		}
		if(pBuffUser)
		{
			pBuffUser->SetText(item.strUseName);
		}
		if(pUseSize)
		{
			str.Format(_T("%u"), item.u32UseSize);
			pUseSize->SetText(str);
		}
		if(pAllRead)
		{
			str.Format(_T("%u"), item.u32AllRead);
			pAllRead->SetText(str);
		}
		if(pReadHit)
		{
			str.Format(_T("%u"), item.u32ReadPos);
			pReadHit->SetText(str);
		}
		if(pAllWrite)
		{
			str.Format(_T("%u"), item.u32AllWrite);
			pAllWrite->SetText(str);
		}
		if(pWriteHit)
		{
			str.Format(_T("%u"), item.u32WritePos);
			pWriteHit->SetText(str);
		}
	}
}

CDiskInfoUpContactList::CDiskInfoUpContactList()
:CContactListBase(_T("xml\\DiskInfoUpItem.xml"))
{

}
CDiskInfoUpContactList::~CDiskInfoUpContactList()
{

}
LPVOID CDiskInfoUpContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("DiskInfoUpContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CDiskInfoUpContactList::GetClass() const
{
	return __super::GetClass();;
}
void CDiskInfoUpContactList::OnInsertItem(CControlUI* pItem, DiskInfoItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInfoUpContactList::OnSetItem(CControlUI *pItem, DiskInfoItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInfoUpContactList::ItemChange(CControlUI* pItem, DiskInfoItem &item)
{
	if(pItem)
	{
		CString str;
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pDSPath = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dspath")));
		CLabelUI *pDSFormat = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dsformat")));
		CLabelUI *pDSSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dssize")));
		CLabelUI *pMDTime = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mdtime")));
		CLabelUI *pRemark = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_remark")));
		if(pName)
		{
			pName->SetText(item.strName);
		}
		if(pDSPath)
		{
			pDSPath->SetText(item.strDiskPath);
		}
		if(pDSFormat)
		{
			pDSFormat->SetText(item.strDiskType);
		}
		if(pDSSize)
		{
			pDSSize->SetText(item.strSize);
		}
		if(pMDTime)
		{
			pMDTime->SetText(item.strModifyTime);
		}
		if(pRemark)
		{
			pRemark->SetText(item.strRemark);
		}
	}
}

CDiskInfoDownContactList::CDiskInfoDownContactList()
:CContactListBase(_T("xml\\DiskInfoDownItem.xml"))
{

}
CDiskInfoDownContactList::~CDiskInfoDownContactList()
{

}
LPVOID CDiskInfoDownContactList::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("DiskInfoDownContactList")) == 0) return this;
	return __super::GetInterface(pstrName);
}

LPCTSTR CDiskInfoDownContactList::GetClass() const
{
	return __super::GetClass();;
}
void CDiskInfoDownContactList::OnInsertItem(CControlUI* pItem, DiskInfoItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInfoDownContactList::OnSetItem(CControlUI *pItem, DiskInfoItem &item)
{
	ItemChange(pItem, item);
}
void CDiskInfoDownContactList::ItemChange(CControlUI* pItem, DiskInfoItem &item)
{
	if(pItem)
	{
		CString str;
		CListContainerElementUI *pElement = (CListContainerElementUI *)pItem;
		CLabelUI *pName = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_name")));
		CLabelUI *pDSPath = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dspath")));
		CLabelUI *pDSFormat = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dsformat")));
		CLabelUI *pDSSize = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_dssize")));
		CLabelUI *pMDTime = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_mdtime")));
		CLabelUI *pRemark = static_cast<CLabelUI*>(pElement->FindSubControl(_T("lab_remark")));
		if(pName)
		{
			pName->SetText(item.strName);
		}
		if(pDSPath)
		{
			pDSPath->SetText(item.strDiskPath);
		}
		if(pDSFormat)
		{
			pDSFormat->SetText(item.strDiskType);
		}
		if(pDSSize)
		{
			pDSSize->SetText(item.strSize);
		}
		if(pMDTime)
		{
			pMDTime->SetText(item.strModifyTime);
		}
		if(pRemark)
		{
			pRemark->SetText(item.strRemark);
		}
	}
}
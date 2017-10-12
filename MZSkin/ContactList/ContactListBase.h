#pragma once

#include <UIlib.h>
#include <map>

using namespace DuiLib;
using namespace std;


template<typename t_key, typename t_item>
class CContactListBase : public CListUI
{
public:
	CContactListBase(LPCTSTR strXml){
		m_strItemXml.Format(_T("%s"), strXml);
	}
	virtual ~CContactListBase(){
		m_mapItem.clear();
	}

	struct stElement
	{
		stElement()
		{
			pElement = NULL;
		}
		t_item	item;
		CListContainerElementUI *pElement;
	};

	virtual LPCTSTR GetClass() const{
		return __super::GetClass();
	}
	virtual LPVOID GetInterface(LPCTSTR pstrName){
		if (_tcscmp(pstrName, _T("ContactListBase")) == 0) return this;
		return __super::GetInterface(pstrName);
	}
public:
	void InsertItem(t_key key, t_item &item){
		CDialogBuilder builder;
		stElement element;
		CListContainerElementUI *pElement = CreateItem(item, builder);
		if(pElement)
		{
			pElement->SetOwner(this);
			element.pElement = pElement;
			element.item = item;
			m_mapItem[key] = element;
//			OnInsertItem(pElement, item);
			__super::Add(pElement);
		}
	}
	void DeleteItem(t_key key){
		map<t_key, stElement>::iterator itor = m_mapItem.find(key);
		if(itor != m_mapItem.end())
		{
			__super::Remove(itor->second.pElement);
			m_mapItem.erase(itor);
			
		}
	}
	void DeleteItem(CControlUI *pItem){
		map<t_key, stElement>::iterator itor = m_mapItem.begin();
		while(itor != m_mapItem.end())
		{
			if(itor->second.pElement == pItem)
			{
				break;
			}
			itor++;
		}
		if(itor != m_mapItem.end())
		{
			DeleteItem(itor->first);
		}
	}
	void DeleteItemAll(){
		map<t_key, stElement>::iterator itor = m_mapItem.begin();
		while(itor != m_mapItem.end())
		{
			__super::Remove(itor->second.pElement);
			itor++;
		}
		m_mapItem.clear();
	}
	void SetItem(t_key key, t_item &item){
		map<t_key, stElement>::iterator itor = m_mapItem.find(key);
		if(itor != m_mapItem.end())
		{
			OnSetItem(itor->second.pElement, item);
			itor->second.item = item;
		}
	}
	BOOL GetItem(t_key key, t_item &item){
		map<t_key, stElement>::iterator itor = m_mapItem.find(key);
		if(itor != m_mapItem.end())
		{
			item = itor->second.item;
			return TRUE;
		}
		return FALSE;
	}

protected:
	virtual void OnInsertItem(CControlUI* pItem, t_item &item) = 0;
	virtual void OnSetItem(CControlUI *pItem, t_item &item) = 0;

private:
	CListContainerElementUI *CreateItem(t_item &item, CDialogBuilder &builder){
		
		CListContainerElementUI* pItem = NULL;
		if(m_strItemXml.IsEmpty())
		{
			return pItem;
		}
		if(!builder.GetMarkup()->IsValid())
		{
			pItem = static_cast<CListContainerElementUI*>(builder.Create(m_strItemXml.GetString(), (UINT)0, NULL, GetManager()));
		}
		else
		{
			pItem = static_cast<CListContainerElementUI*>(builder.Create((UINT)0, GetManager()));
		}
		OnInsertItem(pItem, item);
		return pItem;
	}
private:
	map<t_key, stElement>		m_mapItem;
	CString						m_strItemXml;
};
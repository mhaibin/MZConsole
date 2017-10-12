#pragma once

#include "ContactListBase.h"
#include "SkinFieldDef.h"

//����վ�����б�
class CDiskInfoContactList : public CContactListBase<CString, DiskPorpertyItem>
{
public:
	CDiskInfoContactList();
	~CDiskInfoContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, DiskPorpertyItem &item);
	void OnSetItem(CControlUI *pItem, DiskPorpertyItem &item);
private:
	void ItemChange(CControlUI* pItem, DiskPorpertyItem &item);
};

//����վ�����б�
class CDiskInstructionContactList : public CContactListBase<CString, DiskInstructionItem>
{
public:
	CDiskInstructionContactList();
	~CDiskInstructionContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, DiskInstructionItem &item);
	void OnSetItem(CControlUI *pItem, DiskInstructionItem &item);
private:
	void ItemChange(CControlUI* pItem, DiskInstructionItem &item);
};

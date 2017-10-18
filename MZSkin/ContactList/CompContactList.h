#pragma once

#include "ContactListBase.h"
#include "SkinFieldDef.h"

//����վ�����б�
class CWorkstationContactList : public CContactListBase<UINT32, WorkstationItem>
{
public:
	CWorkstationContactList();
	~CWorkstationContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);

protected:
	void OnInsertItem(CControlUI* pItem, WorkstationItem &item);
	void OnSetItem(CControlUI *pItem, WorkstationItem &item);
private:
	void ItemChange(CControlUI* pItem, WorkstationItem &item);
};

//�����������б�
class CServerContactList : public CContactListBase<CString, ServerItem>
{
public:
	CServerContactList();
	~CServerContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, ServerItem &item);
	void OnSetItem(CControlUI *pItem, ServerItem &item);
private:
	void ItemChange(CControlUI* pItem, ServerItem &item);
};

//�����������б�
class CPropertyContactList : public CContactListBase<UINT32, PropertyItem>
{
public:
	CPropertyContactList();
	~CPropertyContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, PropertyItem &item);
	void OnSetItem(CControlUI *pItem, PropertyItem &item);
private:
	void ItemChange(CControlUI* pItem, PropertyItem &item);
};

//�������洢ʹ������б�
class CStorageUseContactList : public CContactListBase<CString, StorageUseItem>
{
public:
	CStorageUseContactList();
	~CStorageUseContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, StorageUseItem &item);
	void OnSetItem(CControlUI *pItem, StorageUseItem &item);
private:
	void ItemChange(CControlUI* pItem, StorageUseItem &item);
};

//����ʹ����Ϣ(����)�б�
class CDiskInfoUpContactList : public CContactListBase<CString, DiskInfoItem>
{
public:
	CDiskInfoUpContactList();
	~CDiskInfoUpContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, DiskInfoItem &item);
	void OnSetItem(CControlUI *pItem, DiskInfoItem &item);
private:
	void ItemChange(CControlUI* pItem, DiskInfoItem &item);
};

//����ʹ����Ϣ(����)�б�
class CDiskInfoDownContactList : public CContactListBase<CString, DiskInfoItem>
{
public:
	CDiskInfoDownContactList();
	~CDiskInfoDownContactList();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
protected:
	void OnInsertItem(CControlUI* pItem, DiskInfoItem &item);
	void OnSetItem(CControlUI *pItem, DiskInfoItem &item);
private:
	void ItemChange(CControlUI* pItem, DiskInfoItem &item);
};
#pragma once

#include "ContactListBase.h"
#include "SkinFieldDef.h"

//工作站管理列表
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

//服务器管理列表
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

//服务器性能列表
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

//服务器存储使用情况列表
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

//磁盘使用信息(上面)列表
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

//磁盘使用信息(下面)列表
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
#pragma once
/**************************************************
*文件名称：WorkstationInfoMgr.h
*创建  人：mhaibin
*创建时间：2017.9.20
*描    述：工作站信息数据交互处理
***************************************************/
#include <vector>
#include "SkinFieldDef.h"

using namespace std;

class CWorkstationInfoMgr
{
public:
	CWorkstationInfoMgr();
	~CWorkstationInfoMgr();

public:
	void AddWorkstation(LPCTSTR pStrNum);
	void DeleteAllSelWorkstation();
	void GetSelWorkstation(vector<CString> &vecSelNum);
	UINT32 GetSelCount();

	//工作站添加，修改；
	void ClearAddWorkstation();
	void AddNewWorkstation(WorkstationItem &item);
	void GetNewWorkstation(vector<WorkstationItem> &vecItem);
private:
	vector<CString>				m_vecSelNum;			//选择的工作站

	vector<WorkstationItem>		m_vecAddWorkstation;	//添加的工作站
};
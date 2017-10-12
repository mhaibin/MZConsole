#include "stdafx.h"
#include "WorkstationInfoMgr.h"

CWorkstationInfoMgr::CWorkstationInfoMgr()
{

}
CWorkstationInfoMgr::~CWorkstationInfoMgr()
{

}
void CWorkstationInfoMgr::AddWorkstation(LPCTSTR pStrNum)
{
	CString strNum(pStrNum);
	m_vecSelNum.push_back(strNum);
}
void CWorkstationInfoMgr::DeleteAllSelWorkstation()
{
	m_vecSelNum.clear();
}
void CWorkstationInfoMgr::GetSelWorkstation(vector<CString> &vecSelNum)
{
	vecSelNum = m_vecSelNum;
}
UINT32 CWorkstationInfoMgr::GetSelCount()
{
	return m_vecSelNum.size();
}
void CWorkstationInfoMgr::AddNewWorkstation(WorkstationItem &item)
{
	m_vecAddWorkstation.push_back(item);
}
void CWorkstationInfoMgr::ClearAddWorkstation()
{
	m_vecAddWorkstation.clear();
}
void CWorkstationInfoMgr::GetNewWorkstation(vector<WorkstationItem> &vecItem)
{
	vecItem = m_vecAddWorkstation;
}
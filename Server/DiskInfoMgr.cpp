#include "stdafx.h"
#include "DiskInfoMgr.h"

CDiskInfoMgr::CDiskInfoMgr()
{

}
CDiskInfoMgr::~CDiskInfoMgr()
{

}
void CDiskInfoMgr::SetCurSelDiskInfo(vector<DiskInfoItem> &vecInfo)
{
	m_vecCurSelInfo.clear();
	m_vecCurSelInfo = vecInfo;
}
void CDiskInfoMgr::GetCurSelDiskInfo(vector<DiskInfoItem> &vecInfo)
{
	vecInfo = m_vecCurSelInfo;
}
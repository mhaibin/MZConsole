#pragma once
/**************************************************
*文件名称：DiskInfoMgr.h
*创建  人：mhaibin
*创建时间：2017.9.14
*描    述：磁盘信息数据交互缓存管理
***************************************************/
#include <vector>
#include "SkinFieldDef.h"

using namespace std;

class CDiskInfoMgr
{
public:
	CDiskInfoMgr();
	~CDiskInfoMgr();

public:
	void SetCurSelDiskInfo(vector<DiskInfoItem> &vecInfo);
	void GetCurSelDiskInfo(vector<DiskInfoItem> &vecInfo);

private:
	vector<DiskInfoItem>		m_vecCurSelInfo;		//当前磁盘维护界面选择的磁盘信息
};
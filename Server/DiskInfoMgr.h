#pragma once
/**************************************************
*�ļ����ƣ�DiskInfoMgr.h
*����  �ˣ�mhaibin
*����ʱ�䣺2017.9.14
*��    ����������Ϣ���ݽ����������
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
	vector<DiskInfoItem>		m_vecCurSelInfo;		//��ǰ����ά������ѡ��Ĵ�����Ϣ
};
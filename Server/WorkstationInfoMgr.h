#pragma once
/**************************************************
*�ļ����ƣ�WorkstationInfoMgr.h
*����  �ˣ�mhaibin
*����ʱ�䣺2017.9.20
*��    ��������վ��Ϣ���ݽ�������
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

	//����վ��ӣ��޸ģ�
	void ClearAddWorkstation();
	void AddNewWorkstation(WorkstationItem &item);
	void GetNewWorkstation(vector<WorkstationItem> &vecItem);
private:
	vector<CString>				m_vecSelNum;			//ѡ��Ĺ���վ

	vector<WorkstationItem>		m_vecAddWorkstation;	//��ӵĹ���վ
};
#pragma once
/***********************************************************
*�ļ���:MzdIOMgr.h
*������:Mhaibin
*����  :2017.9.5
*����  :��MZD����վ���ݣ����������ݣ��������ݽ��л�ȡ�޸ĵ���ز�����
*ȫ��Ψһ����
************************************************************/
#include "MZConsoleDefine.h"
#include "SkinFieldDef.h"
#include <map>
#include <vector>

using namespace std;

struct s_ServerIPInfo
{
	s_ServerIPInfo()
	{
		u8Sel = 8;
	}
	CString strIP;
	UINT8 u8Sel;
};

class CMzdIOMgr
{
public:
	CMzdIOMgr();
	~CMzdIOMgr();

public:
	//��ʼ��
	void Init();
	//�Ƿ�����
	BOOL IsConnect();
	//�ӷ�������������
	BOOL SendDownLoadCmd();
	//�ϴ��޸ĵ�����
	BOOL SendUpLoadCmd();
	//����վ���ر�������
	void WorkstationLoadData();
	//���������ر�������
	void ServerLoadData();
	//���̼��ر�������
	void DiskLoadData();
	//���ط���������·��
	void DiskPathLoadData();
	//���ع���վ������Ϣ
	void WorkstationGroupLoadData();
	//��ȡ����ע����Ϣ
	void GetRegsterInfo(RegsterInfo &info);
	//�Ƿ�ע��
	BOOL IsRegster();

public:
	//��ȡ��ʼIP
	CString GetSevDefIP();
	//�ַ���IPת����IP
	DWORD IPStrToInt(LPCTSTR pStrIP);
	//����IPת�ַ���IP
	CString IPIntToStr(DWORD dwIP);
	//ͨ��Upd��ȡimg�ļ�·��
	CString GetImgPath(LPCTSTR pStrUpd);
	//��ȡMZD buffer
	UINT8 *GetMzdCmdBuff();
	//��ȡ����ֵ
	INT32 GetProgressPos(LPCTSTR pServerName);
	//��������
	INT32 SendUICmdSR(UINT32 u32Cmd, UINT8 *pBuff, UINT32 u32Size, DWORD dwIP, UINT32 u32Flag);
	//��ȡ����IP��strGetway:����;strDNSI1:DNSI1;strDNSI2:DNSI2;strMask:���룻
	void GetNetIP(CString &strGetway, CString &strDNSI1, CString &strDNSI2, CString &strMask);
	//��ȡ����վ�����
	UINT32 GetWorkstationMaxNum();
	//ͨ��������IP��ȡ��д�̷�
	void GetRWriteDisk(LPCTSTR pStrIP, vector<CString> &vecDisk);
	//��ȡ������IO������
	void GetIOServer(map<CString, vector<s_ServerIPInfo>> &mapIOServer, CString &strFindIP);
	//��⹤��վ���
	INT32 CheckWorkstationNum(LPCTSTR pStrNum, INT32 nNumLen, LPCTSTR strComF, LPCTSTR strComB, LPCTSTR pStartIP, UINT32 u32Flag, CString &strWksNum, CString &strWksIP);
	//���δʹ�õĲ˵�
	void CheckUnuserMenu();
	//���δʹ�õĴ���
	void CheckUnuserDisk();
	//���δʹ�õķ�����
	void CheckUnuserServer();
	//��ȡ���ܱ��
	CString GetSuperMgr();
	//���ó���
	void SetSuperMgr(LPCTSTR strNum);
	//��������
	INT32 EndSuperMgr(LPCTSTR pStrNum);
	//ɾ��ָ����ŵĹ���վ
	void DelWorkstation(LPCTSTR pStrNum);
	//����ָ����ŵĹ���վ
	void DisableWorkstation(LPCTSTR pStrNum);
	//����ָ����ŵĹ���վ
	void EnableWorkstation(LPCTSTR pStrNum);
	//����ָ����ŵĹ���վ
	void RebootWorkstation(LPCTSTR pStrNum);
	//�ر�ָ����ŵĹ���վ
	void CloseWorkstation(LPCTSTR pStrNum);
	//ɾ��ָ��������
	INT32 DelServer(LPCTSTR pStrName);
	//ɾ��ָ������
	INT32 DelDisk(LPCTSTR pStrName, LPCTSTR pStrPath);
	//��ô������к�
	void GetDiskSerial(vector<CString> &vecDiskSerial);
	//����ע��
	INT32 Register(LPCTSTR pStrSerial, LPCTSTR pStrPass, LPCTSTR pStrPassword, LPCTSTR pStrSecurity, CString &strTips);
	//����IO������
	INT32 UserIOServer();
	//���MAC
	void ClearMac(LPCTSTR pStrNum);
	//���ѹ���վ
	void WakeUpWorkStation(LPCTSTR pStrNum);
public:
	//��ȡ��������ش������ݣ�
	void GetServerDiskData(LPCTSTR strName, vector<CString> &vecDiskPath);
	//��ȡ����վ�˵�������Ϣ
	void GetWorkstationMenuDiskInfo(UINT32 u32Num, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo);
	//���ù���վ��Ϣ��ͬ����������
	void SetWorkStationToMZD(LPCTSTR strNum, LPCTSTR strWksNum, LPCTSTR strWksIP, WorkstationInfo &itemData, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo, UINT32 u32Flag);
	//���÷�������Ϣ��ͬ����������
	void SetServerInfoToMZD(LPCTSTR pstrIP, LPCTSTR pStrOldName, LPCTSTR pStrNewName, ServerInfo &Info, UINT32 u32Flag);
	//��ȡ����վ��״̬
	void GetWorkstationStatus(map<CString, UINT32> &mapStatus);
	UINT32 GetWorkStationStatus(LPCTSTR pStrNum);
	UINT32 GetWokstationOnlineCount();
	UINT32 GetWokstationCount();
	//��ȡ��������״̬
	void GetServerStatus(map<CString, UINT32> &mapStatus);
	UINT32 GetServerCount();
	UINT32 GetServerOnlineCount();
public:

	/**********************����վ���ݲ���**************************/
	void GetAllWorkstation(map<UINT32, WorkstationInfo> &mapWorkstation);
	void SetWorkStationBatch(map<UINT32, WorkstationInfo> &mapWorkstation);
	void GetSimpleWorStation(UINT32 uID, WorkstationInfo &itemData);
	void SetWorkstationSimple(UINT32 uID, WorkstationInfo &itemData);
	void SetWorkStationGroupID(UINT32 uID, LPCTSTR pStrName);
	void AddWorkStation(UINT32 uID, WorkstationInfo &itemData);
	void DelWorkStation(UINT32 uID);
	/**************************************************************/

	/**********************���������ݲ���**************************/
	void GetAllServer(map<CString, ServerItem> &mapServer);
	void SetServerBatch(map<CString, ServerItem> &mapServer);
	void GetSimpleServer(CString strName, ServerItem &itemData);
	void SetServerSimple(CString strName, ServerItem &itemData);
	void GetServerInfo(CString strName, ServerInfo &info);
	/**************************************************************/

	/**********************�������ݲ���**************************/
	void GetAllDisk(map<CString, vector<DiskInfoItem>> &mapDisk);
	void GetIMGDiskInfo(CString strName, vector<DiskInfoItem> &vecItem);
	void SetDiskBatch(map<CString, vector<DiskInfoItem>> &mapDisk);
	void GetSimpleDisk(CString strName, vector<DiskInfoItem> &itemData);
	void SetDiskSimple(CString strName, vector<DiskInfoItem> &itemData);
	/**************************************************************/

	/*********************����վ������Ϣ����********************/
	//��ȡ����
	void GetAllGroup(map<UINT32, CString> &mapGroup);
	//��ӷ���
	INT32 AddWorkstationGroup(LPCTSTR pStrGroupName);
	//�޸Ĺ���վ����
	void ModifyWorkstationGroup(UINT32 uGroupID, LPCTSTR pStrGroupName);
	//ɾ������վ����
	void DelWorkstationGroup(UINT32 uGroupID);
	//�жϷ��������Ѿ�����
	BOOL IsExistGroup(LPCTSTR pStrGroup);
	/**********************************************************/

private:
	//����վ��ų�ͻ���
	INT32 WorkstationNumClashCheck(INT32 Type, char *strChk_KN, CString strChk_KV, CString strWksNum, CString strErrType, CString strErrMsg);
	//��������վ���
	CString CreateWorkstationNum(LPCTSTR pStrNum, INT32 nNumLen, LPCTSTR strComF, LPCTSTR strComB, LPCTSTR strComStart);
	//��������վIP
	CString CreateWorkstationIP(LPCTSTR pStrNum, LPCTSTR StrWksStartIp);
	//�����˵�����
	CString CreateMenuName();
private:
	BOOL			m_bConnect;				//��MZD�����������Ƿ�����
	UINT32			m_u32OnlineCount;		//���ߵĹ���վ����
	UINT32			m_u32OnlineCountSvr;	//���ߵķ���������

	map<UINT32, WorkstationInfo>			m_mapWorkstation;		//����վ������Ϣ
	map<CString, ServerItem>				m_mapServer;			//������������Ϣ
	map<CString, vector<DiskInfoItem>>		m_mapDisk;				//����������Ϣ
	map<CString, vector<CString>>			m_mapDiskInfo;			//��������Ӧ�Ĵ���·��
	map<CString, UINT32>					m_mapWorkstationStatus;	//����վ��״̬��Ϣ
	map<CString, UINT32>					m_mapServerStatus;		//��������״̬��Ϣ
	map<UINT32,	CString>					m_mapWorkstationGroup;	//����վ������Ϣ
};
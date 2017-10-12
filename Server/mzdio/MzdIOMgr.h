#pragma once
/***********************************************************
*文件名:MzdIOMgr.h
*创建人:Mhaibin
*日期  :2017.9.5
*描述  :对MZD工作站数据，服务器数据，磁盘数据进行获取修改等相关操作；
*全局唯一对象
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
	//初始化
	void Init();
	//是否连接
	BOOL IsConnect();
	//从服务器下载数据
	BOOL SendDownLoadCmd();
	//上传修改的数据
	BOOL SendUpLoadCmd();
	//工作站加载本地数据
	void WorkstationLoadData();
	//服务器加载本地数据
	void ServerLoadData();
	//磁盘加载本地数据
	void DiskLoadData();
	//加载服务器磁盘路径
	void DiskPathLoadData();
	//加载工作站分组信息
	void WorkstationGroupLoadData();
	//获取服务注册信息
	void GetRegsterInfo(RegsterInfo &info);
	//是否注册
	BOOL IsRegster();

public:
	//获取起始IP
	CString GetSevDefIP();
	//字符串IP转整型IP
	DWORD IPStrToInt(LPCTSTR pStrIP);
	//整型IP转字符串IP
	CString IPIntToStr(DWORD dwIP);
	//通过Upd获取img文件路径
	CString GetImgPath(LPCTSTR pStrUpd);
	//获取MZD buffer
	UINT8 *GetMzdCmdBuff();
	//获取进度值
	INT32 GetProgressPos(LPCTSTR pServerName);
	//发送命令
	INT32 SendUICmdSR(UINT32 u32Cmd, UINT8 *pBuff, UINT32 u32Size, DWORD dwIP, UINT32 u32Flag);
	//获取网络IP，strGetway:网关;strDNSI1:DNSI1;strDNSI2:DNSI2;strMask:掩码；
	void GetNetIP(CString &strGetway, CString &strDNSI1, CString &strDNSI2, CString &strMask);
	//获取工作站最大编号
	UINT32 GetWorkstationMaxNum();
	//通过服务器IP获取回写盘符
	void GetRWriteDisk(LPCTSTR pStrIP, vector<CString> &vecDisk);
	//获取内网的IO服务器
	void GetIOServer(map<CString, vector<s_ServerIPInfo>> &mapIOServer, CString &strFindIP);
	//检测工作站编号
	INT32 CheckWorkstationNum(LPCTSTR pStrNum, INT32 nNumLen, LPCTSTR strComF, LPCTSTR strComB, LPCTSTR pStartIP, UINT32 u32Flag, CString &strWksNum, CString &strWksIP);
	//检测未使用的菜单
	void CheckUnuserMenu();
	//检测未使用的磁盘
	void CheckUnuserDisk();
	//检测未使用的服务器
	void CheckUnuserServer();
	//获取超管编号
	CString GetSuperMgr();
	//设置超管
	void SetSuperMgr(LPCTSTR strNum);
	//结束超管
	INT32 EndSuperMgr(LPCTSTR pStrNum);
	//删掉指定编号的工作站
	void DelWorkstation(LPCTSTR pStrNum);
	//禁用指定编号的工作站
	void DisableWorkstation(LPCTSTR pStrNum);
	//启用指定编号的工作站
	void EnableWorkstation(LPCTSTR pStrNum);
	//重启指定编号的工作站
	void RebootWorkstation(LPCTSTR pStrNum);
	//关闭指定编号的工作站
	void CloseWorkstation(LPCTSTR pStrNum);
	//删除指定服务器
	INT32 DelServer(LPCTSTR pStrName);
	//删除指定磁盘
	INT32 DelDisk(LPCTSTR pStrName, LPCTSTR pStrPath);
	//获得磁盘序列号
	void GetDiskSerial(vector<CString> &vecDiskSerial);
	//进行注册
	INT32 Register(LPCTSTR pStrSerial, LPCTSTR pStrPass, LPCTSTR pStrPassword, LPCTSTR pStrSecurity, CString &strTips);
	//用作IO服务器
	INT32 UserIOServer();
	//清除MAC
	void ClearMac(LPCTSTR pStrNum);
	//唤醒工作站
	void WakeUpWorkStation(LPCTSTR pStrNum);
public:
	//获取服务器相关磁盘数据；
	void GetServerDiskData(LPCTSTR strName, vector<CString> &vecDiskPath);
	//获取工作站菜单磁盘信息
	void GetWorkstationMenuDiskInfo(UINT32 u32Num, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo);
	//设置工作站信息并同步到服务器
	void SetWorkStationToMZD(LPCTSTR strNum, LPCTSTR strWksNum, LPCTSTR strWksIP, WorkstationInfo &itemData, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo, UINT32 u32Flag);
	//设置服务器信息并同步到服务器
	void SetServerInfoToMZD(LPCTSTR pstrIP, LPCTSTR pStrOldName, LPCTSTR pStrNewName, ServerInfo &Info, UINT32 u32Flag);
	//获取工作站的状态
	void GetWorkstationStatus(map<CString, UINT32> &mapStatus);
	UINT32 GetWorkStationStatus(LPCTSTR pStrNum);
	UINT32 GetWokstationOnlineCount();
	UINT32 GetWokstationCount();
	//获取服务器的状态
	void GetServerStatus(map<CString, UINT32> &mapStatus);
	UINT32 GetServerCount();
	UINT32 GetServerOnlineCount();
public:

	/**********************工作站数据操作**************************/
	void GetAllWorkstation(map<UINT32, WorkstationInfo> &mapWorkstation);
	void SetWorkStationBatch(map<UINT32, WorkstationInfo> &mapWorkstation);
	void GetSimpleWorStation(UINT32 uID, WorkstationInfo &itemData);
	void SetWorkstationSimple(UINT32 uID, WorkstationInfo &itemData);
	void SetWorkStationGroupID(UINT32 uID, LPCTSTR pStrName);
	void AddWorkStation(UINT32 uID, WorkstationInfo &itemData);
	void DelWorkStation(UINT32 uID);
	/**************************************************************/

	/**********************服务器数据操作**************************/
	void GetAllServer(map<CString, ServerItem> &mapServer);
	void SetServerBatch(map<CString, ServerItem> &mapServer);
	void GetSimpleServer(CString strName, ServerItem &itemData);
	void SetServerSimple(CString strName, ServerItem &itemData);
	void GetServerInfo(CString strName, ServerInfo &info);
	/**************************************************************/

	/**********************磁盘数据操作**************************/
	void GetAllDisk(map<CString, vector<DiskInfoItem>> &mapDisk);
	void GetIMGDiskInfo(CString strName, vector<DiskInfoItem> &vecItem);
	void SetDiskBatch(map<CString, vector<DiskInfoItem>> &mapDisk);
	void GetSimpleDisk(CString strName, vector<DiskInfoItem> &itemData);
	void SetDiskSimple(CString strName, vector<DiskInfoItem> &itemData);
	/**************************************************************/

	/*********************工作站分组信息操作********************/
	//获取分组
	void GetAllGroup(map<UINT32, CString> &mapGroup);
	//添加分组
	INT32 AddWorkstationGroup(LPCTSTR pStrGroupName);
	//修改工作站分组
	void ModifyWorkstationGroup(UINT32 uGroupID, LPCTSTR pStrGroupName);
	//删除工作站分组
	void DelWorkstationGroup(UINT32 uGroupID);
	//判断分组名称已经存在
	BOOL IsExistGroup(LPCTSTR pStrGroup);
	/**********************************************************/

private:
	//工作站编号冲突检测
	INT32 WorkstationNumClashCheck(INT32 Type, char *strChk_KN, CString strChk_KV, CString strWksNum, CString strErrType, CString strErrMsg);
	//创建工作站编号
	CString CreateWorkstationNum(LPCTSTR pStrNum, INT32 nNumLen, LPCTSTR strComF, LPCTSTR strComB, LPCTSTR strComStart);
	//创建工作站IP
	CString CreateWorkstationIP(LPCTSTR pStrNum, LPCTSTR StrWksStartIp);
	//创建菜单名称
	CString CreateMenuName();
private:
	BOOL			m_bConnect;				//与MZD服务器连接是否正常
	UINT32			m_u32OnlineCount;		//在线的工作站个数
	UINT32			m_u32OnlineCountSvr;	//在线的服务器个数

	map<UINT32, WorkstationInfo>			m_mapWorkstation;		//工作站数据信息
	map<CString, ServerItem>				m_mapServer;			//服务器数据信息
	map<CString, vector<DiskInfoItem>>		m_mapDisk;				//磁盘数据信息
	map<CString, vector<CString>>			m_mapDiskInfo;			//服务器对应的磁盘路径
	map<CString, UINT32>					m_mapWorkstationStatus;	//工作站的状态信息
	map<CString, UINT32>					m_mapServerStatus;		//服务器的状态信息
	map<UINT32,	CString>					m_mapWorkstationGroup;	//工作站分组信息
};
#include "stdafx.h"
#include "..\Main\MZIniLib.h"
#include "MzdIOMgr.h"
#include "UtilString.h"
#include "UtilTools.h"
#include "UtilLog.h"
#include "MZConsoleDefine.h"

#define	UPDATEDATATIMER			1000
#define CLICKIOSERVERRUN		1001			//MZDIO服务是否运行

CMzdIOMgr::CMzdIOMgr()
{
	m_hWnd = Create(NULL);
	ATLASSERT(m_hWnd);

	SetTimer(UPDATEDATATIMER, 3 * 1000, NULL);

	SetTimer(CLICKIOSERVERRUN, 3 * 1000, NULL);

	m_bConnect = FALSE;
	m_u32OnlineCount = 0;
	m_u32OnlineCountSvr = 0;

	m_hWorkstationWnd = NULL;
	m_hServerWnd = NULL;
//	Init();
}
CMzdIOMgr::~CMzdIOMgr()
{
	KillTimer(UPDATEDATATIMER);
	KillTimer(CLICKIOSERVERRUN);
	if (NULL != m_hWnd)
	{
		DestroyWindow();
		m_hWnd = NULL;
	}
	WSACleanup();
}

//初始化
void CMzdIOMgr::Init()
{
	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err = 0;  
	wVersionRequested = MAKEWORD(2, 2);  
	err = WSAStartup(wVersionRequested, &wsaData);
	if(0 != err)
	{
		m_bConnect = FALSE;
	}
	m_bConnect = TRUE;
	if (0 > UI_Cmd_SR(eMZDH_CMD_CHECK_REG,0,0))
	{
		m_bConnect = FALSE;
	}
}
//是否连接
BOOL CMzdIOMgr::IsConnect()
{
	return m_bConnect;
}
//从服务器下载数据
BOOL CMzdIOMgr::SendDownLoadCmd()
{
	return (0 == UI_Load_Ini())? TRUE : FALSE;
}
//上传修改的数据
BOOL CMzdIOMgr::SendUpLoadCmd()
{
	return (0 == UI_Updata_Ini())? TRUE : FALSE;
}
//工作站加载本地数据
void CMzdIOMgr::WorkstationLoadData()
{
	m_mapWorkstation.clear();
	CString strWks, strSize, strStatus, strMenu;
	for( int i = 0; i<MZD_MAX_WKS_NUMS; i++ )
	{
		if (( strWks = MZDUI_GET_WKS(i, "WksNumber")) == _T("")) 
			break;
		if ( strWks == _T("0") ) 
			continue; // 0: Wks Is DHCP 
		WorkstationInfo data;
		strMenu = MZDUI_GET_WKS(strWks, "MenuSet1");
		UINT32 uNumber = (UINT32)_ttoi(strWks.GetString());
		data.strIP = MZDUI_GET_WKS(strWks, "WksIP");
		data.strMac = MZDUI_GET_WKS(strWks, "MAC");
		data.strMirrorFile = MZDUI_GET_MENU( MZDUI_GET_WKS(strWks, "MenuSet1"), "DskSet1" );
		data.strMirrorIP = MZDUI_GET_WKS(strWks, "Disk_IP1");
		data.strName = MZDUI_GET_WKS(strWks, "ComName");
		data.strReturnWDir = MZDUI_GET_WKS(strWks, "DiskWorkDir1");
		data.u32Num = uNumber;
		data.strSize = MZDUI_GET_WKS(strWks, "WksMemory");
		strStatus = MZDUI_GET_WKS(strWks, "AutoClear");
		data.u32GroupID = _ttoi(MZDUI_GET_WKS(strWks, "Group_ID"));
		data.u8Status = _ttoi(strStatus.GetString());
		data.strDNSI1 = MZDUI_GET_MENU(strMenu, "Dns1");
		data.strDNSI2 = MZDUI_GET_MENU(strMenu, "Dns2");
		data.strNumLen = MZDUI_GET_MENU(strMenu, "NumberLength");
		data.strFName = MZDUI_GET_MENU(strMenu, "ComPre");
		data.strBName = MZDUI_GET_MENU(strMenu, "ComPos");
		data.strDPI = MZDUI_GET_MENU(strMenu, "Scr_Res");
		data.strGetway = MZDUI_GET_MENU(strMenu, "Gateway");
		data.strMask = MZDUI_GET_MENU(strMenu, "wksSubMask");
		data.u32Delay = _ttoi(MZDUI_GET_MENU(strMenu, "Wait_Times"));
		
		m_mapWorkstation[uNumber] = data;
	}
}
//服务器加载本地数据
void CMzdIOMgr::ServerLoadData()
{
	m_mapServer.clear();
	CString strSev, strSev2, strBuffParam;
	INT32 nNum = 0;
	while(true)
	{
		strSev = MZDUI_GET_SERVER(nNum, "ServerName");
		if(strSev.IsEmpty())
			break;
		ServerItem data;

		data.strName = strSev;

		strBuffParam.Format(_T("系统盘:%s,数据盘:%s,写缓存:%s"), MZDUI_GET_SERVER(nNum, "SystemCacheSize"), MZDUI_GET_SERVER(nNum, "DataCacheSize"), MZDUI_GET_SERVER(nNum, "WriteCacheSize"));
		data.strBuffPrama = strBuffParam;

		strSev = MZDUI_GET_SERVER(nNum, "AdaptIp1");
		strSev2 = MZDUI_GET_SERVER(nNum, "AdaptIp2");
		if ( strSev2!=L"" && strSev2!=L"0.0.0.0" )
			strSev += L", " + strSev2;

		strSev2 = MZDUI_GET_SERVER(nNum, "AdaptIp3");
		if ( strSev2!=L"" && strSev2!=L"0.0.0.0" )
			strSev += L", " + strSev2;

		strSev2 = MZDUI_GET_SERVER(nNum, "AdaptIp4");
		if ( strSev2!=L"" && strSev2!=L"0.0.0.0" )
			strSev += L", " + strSev2;

		data.strIP = strSev;

		strSev = MZDUI_GET_SERVER(nNum, "workDir1");
		strSev2 = MZDUI_GET_SERVER(nNum, "workDir2");
		if ( strSev2!=L"" ) strSev += L", " + strSev2;
		strSev2 = MZDUI_GET_SERVER(nNum, "workDir3");
		if ( strSev2!=L"" ) strSev += L", " + strSev2;
		strSev2 = MZDUI_GET_SERVER(nNum, "workDir4");
		if ( strSev2!=L"" ) strSev += L", " + strSev2;

		data.strReturnWDir = strSev;
		data.strSpareName = MZDUI_GET_SERVER(nNum, "BackupServer");
		data.u8Status = 0;

		m_mapServer[data.strName] = data;
		nNum++;
	}
}
//磁盘加载本地数据
void CMzdIOMgr::DiskLoadData()
{
	m_mapDisk.clear();
	CString strDsk;
	INT32 nIndex = 0;
	while(true)
	{
		strDsk = MZDUI_GET_DISK(nIndex, "DiskName");
		if(strDsk.IsEmpty())
			break;
		INT32 nPos = strDsk.Find(_T("|"));
		CString strServerName = strDsk.Mid(0, nPos-1);
		CString strDiskPath = strDsk.Mid(nPos+2, strDsk.GetLength());
		DWORD SrvIP = UI_Get_Server_IP(strDiskPath);

		if (Get_Image_Info(MZDUI_GET_DISK(nIndex, "DiskDir"), SrvIP) < 0) 
		{
			nIndex++;
			continue;
		}
		DiskInfoItem data;
		data.strName = strServerName;
		data.strDiskPath = MZDUI_GET_DISK(nIndex, "DiskDir");
 		data.strDiskType = Get_Image_Type(MZDUI_GET_IMAGE(0, "Type"));
 		data.strModifyTime = MZDUI_GET_IMAGE(0, "CurrentModifyTime");
		data.strRemark = MZDUI_GET_IMAGE(0, "Note");
		data.strSize = Get_Image_Size(MZDUI_GET_IMAGE(0, "SizeMB"));
		m_mapDisk[strServerName].push_back(data);
		nIndex++;
	}
}
void CMzdIOMgr::DiskPathLoadData()
{
	map<CString, ServerItem>::iterator itor = m_mapServer.begin();
	while(itor != m_mapServer.end())
	{
		DWORD SrvIP = UI_Get_Server_IP((CStringW)(itor->first));
		vector<CString> vecDiskPath;
		CString strDebar = L"";
		CString strTmp;
		CStringA strAServerName;
		Util::String::W_2_Utf8(itor->first, strAServerName);
		strTmp = MZDUI_GET_SERVER((char *)strAServerName.GetString(), "workDir1");
		if ( strTmp != L"" ){  strDebar += strTmp[0]; strDebar += strTmp[1]; }

		strTmp = MZDUI_GET_SERVER((char *)strAServerName.GetString(), "workDir2");
		if ( strTmp != L"" ){  strDebar += strTmp[0]; strDebar += strTmp[1]; }

		strTmp = MZDUI_GET_SERVER((char *)strAServerName.GetString(), "workDir3");
		if ( strTmp != L"" ){  strDebar += strTmp[0]; strDebar += strTmp[1]; }

		strTmp = MZDUI_GET_SERVER((char *)strAServerName.GetString(), "workDir4");
		if ( strTmp != L"" ){  strDebar += strTmp[0]; strDebar += strTmp[1]; }

		if ( UI_Cmd_SR(eMZDH_CMD_IMAGE_Lists, strDebar, SrvIP)<0 ) return;
		INT32 nIndex=0;
		while (true)
		{
			if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Ret")==L"" ) break; // INI End
			if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Ret")==L"-1" ) break; // Img Error
			CString strTmp = MZDUI_GET_IMAGE_LISTS(nIndex, "Path_Size");
			vecDiskPath.push_back(strTmp);
			nIndex++;
		}
		m_mapDiskInfo[itor->first] = vecDiskPath;
		itor++;
	}
}
void CMzdIOMgr::WorkstationGroupLoadData()
{
	for (INT32 nIndex = 0; ; nIndex++)
	{  
		CString strGrpName = MZDUI_GET_GROUP(nIndex, "GroupName");
		if(strGrpName.IsEmpty()) 
			break;
		UINT32 u32GroupID = _ttoi(MZDUI_GET_GROUP(nIndex, "GroupID"));
		m_mapWorkstationGroup[u32GroupID] = strGrpName;
	}
}
//获取服务注册信息
void CMzdIOMgr::GetRegsterInfo(RegsterInfo &info)
{
	info.strNetBar = MZDUI_GET_REG(0, "agentName");
	info.strDiskNum = MZDUI_GET_REG(0, "hdid");
	CString strSqare = MZDUI_GET_REG(0, "spareHdid");
	if(strSqare.GetLength() != 0)
	{
		info.strDiskNum = info.strDiskNum + L" | ";
		info.strDiskNum = info.strDiskNum + strSqare;
	}
	info.u32Size = _ttoi(MZDUI_GET_REG(0, "nums"));
	info.strValidity = MZDUI_GET_REG(0, "date");
	info.strPass = MZDUI_GET_REG(0, "wbName");
}
//是否注册
BOOL CMzdIOMgr::IsRegster()
{
	if (UI_Cmd_SR(eMZDH_CMD_CHECK_REG,0,0)<0)
	{
		return FALSE;
	}
	CString strNum = MZDUI_GET_REG(0, "nums");
	if(0 >= _ttoi(strNum))
		return FALSE;
	return TRUE;
}
BOOL CMzdIOMgr::IsServiceRun(LPCTSTR pStrServerName)
{
	SC_HANDLE hScm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

	SC_HANDLE hService = NULL;
	SERVICE_STATUS stStatus;

	hService = OpenService(hScm, pStrServerName, SERVICE_ALL_ACCESS);
	if (hService == NULL)
	{
		TCHAR wszMessageBuffer[128] = { 0 };
		CloseServiceHandle( hScm );
	}

	memset(&stStatus, 0, sizeof(stStatus));
	QueryServiceStatus(hService, &stStatus);
	if (stStatus.dwCurrentState == SERVICE_RUNNING)
	{
		return TRUE ;
	}

	return FALSE ;
}
void CMzdIOMgr::GetAllWorkstation(map<UINT32, WorkstationInfo> &mapWorkstation)
{
	mapWorkstation = m_mapWorkstation;
}
void CMzdIOMgr::SetWorkStationBatch(map<UINT32, WorkstationInfo> &mapWorkstation)
{
	map<UINT32, WorkstationInfo>::iterator itor = mapWorkstation.begin();
	while(itor != mapWorkstation.end())
	{
		m_mapWorkstation[itor->first] = itor->second;
		itor++;
	}
}
void CMzdIOMgr::GetSimpleWorStation(UINT32 uID, WorkstationInfo &itemData)
{
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.find(uID);
	if(itor != m_mapWorkstation.end())
	{
		itemData = itor->second;
	}
}
void CMzdIOMgr::SetWorkstationSimple(UINT32 uID, WorkstationInfo &itemData)
{
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.find(uID);
	if(itor != m_mapWorkstation.end())
	{
		itor->second = itemData;
	}
}
void CMzdIOMgr::SetWorkStationGroupID(UINT32 uID, LPCTSTR pStrName)
{
	CString strID;
	strID.Format(_T("%u"), uID);
	MZDUI_SET_WKS(CString(pStrName), "Group_ID", strID);
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.find(_ttoi(pStrName));
	if(itor != m_mapWorkstation.end())
	{
		itor->second.u32GroupID = uID;
	}
}
void CMzdIOMgr::AddWorkStation(UINT32 uID, WorkstationInfo &itemData)
{
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.find(uID);
	if(itor == m_mapWorkstation.end())
	{
		m_mapWorkstation[uID] = itemData;
	}
}
void CMzdIOMgr::DelWorkStation(UINT32 uID)
{
	CString strWks;
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.find(uID);
	if(itor != m_mapWorkstation.end())
	{
		strWks.Format(_T("%u"), itor->second.u32Num);
		m_mapWorkstation.erase(itor);
	}
	//状态列表也要删掉
	map<CString, UINT32>::iterator itor2 = m_mapWorkstationStatus.find(strWks);
	if(itor2 != m_mapWorkstationStatus.end())
	{
		m_mapWorkstationStatus.erase(itor2);
	}
}
void CMzdIOMgr::GetAllServer(map<CString, ServerItem> &mapServer)
{
	mapServer = m_mapServer;
}
void CMzdIOMgr::SetServerBatch(map<CString, ServerItem> &mapServer)
{
	map<CString, ServerItem>::iterator itor = mapServer.begin();
	while(itor != mapServer.end())
	{
		m_mapServer[itor->first] = itor->second;
		itor++;
	}
}
void CMzdIOMgr::GetSimpleServer(CString strName, ServerItem &itemData)
{
	map<CString, ServerItem>::iterator itor = m_mapServer.find(strName);
	if(itor != m_mapServer.end())
	{
		itemData = itor->second;
	}
}
void CMzdIOMgr::SetServerSimple(CString strName, ServerItem &itemData)
{
	map<CString, ServerItem>::iterator itor = m_mapServer.find(strName);
	if(itor != m_mapServer.end())
	{
		itor->second = itemData;
	}
}
void CMzdIOMgr::GetServerInfo(CString strName, ServerInfo &info)
{
	info.strName = strName;
	info.strSqareServer = MZDUI_GET_SERVER(strName, "BackupServer");
	info.u32SysDiskSize = _ttoi(MZDUI_GET_SERVER(strName, "SystemCacheSize"));
	info.u32DataDiskSize = _ttoi(MZDUI_GET_SERVER(strName, "DataCacheSize"));
	info.u32SSDDiskSize = _ttoi(MZDUI_GET_SERVER(strName, "SSDCacheSize"));
	for( int i = 0; i < 8; i++ )
	{ 
		CStringA strAWSet;
		strAWSet.Format("workDir%d", i+1);		
		CString strDisk = MZDUI_GET_SERVER(strName, (char *)strAWSet.GetString());
		strDisk = strDisk.Mid(0,2);
		if(strDisk.IsEmpty())
			continue;
		info.vecRWrite.push_back(strDisk);
	}
}
void CMzdIOMgr::GetAllDisk(map<CString, vector<DiskInfoItem>> &mapDisk)
{
	mapDisk = m_mapDisk;
}
void CMzdIOMgr::GetIMGDiskInfo(CString strName, vector<DiskInfoItem> &vecItem)
{
	DWORD SrvIP = UI_Get_Server_IP(strName);
	if(UI_Cmd_SR( eMZDH_CMD_IMAGE_Lists, _T(""), SrvIP) < 0) 
	{
		Util::Log::Error(_T("Server"), _T("[error]发送获取磁盘命令出错\r\n"));
		return;
	}
	for(INT32 nIndex = 0; ; nIndex++)
	{
		if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Ret")==L"" ) break; // INI End
		if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Ret")==L"-1" ) break; // Img Error

		if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Type")==L"1" ) continue; // Phy
		if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Type")==L"2" ) continue; // Phy

		DiskInfoItem item;
		item.strName = strName;
		item.strDiskPath = MZDUI_GET_IMAGE_LISTS(nIndex, "Path");  
		item.strDiskType = Get_Image_Type(MZDUI_GET_IMAGE_LISTS(nIndex, "Type"));
		item.strSize = Get_Image_Size(MZDUI_GET_IMAGE_LISTS(nIndex, "SizeMB")); 
		item.strModifyTime = MZDUI_GET_IMAGE_LISTS(nIndex, "CurrentModifyTime");
		item.strRemark = MZDUI_GET_IMAGE_LISTS(nIndex, "Note");
		vecItem.push_back(item);
	}
}
void CMzdIOMgr::SetDiskBatch(map<CString, vector<DiskInfoItem>> &mapDisk)
{
	map<CString, vector<DiskInfoItem>>::iterator itor = mapDisk.begin();
	while(itor != mapDisk.end())
	{
		m_mapDisk[itor->first] = itor->second;
		itor++;
	}
}
void CMzdIOMgr::GetSimpleDisk(CString strName, vector<DiskInfoItem> &itemData)
{
	map<CString, vector<DiskInfoItem>>::iterator itor = m_mapDisk.find(strName);
	if(itor != m_mapDisk.end())
	{
		itemData = itor->second;
	}
}
void CMzdIOMgr::SetDiskSimple(CString strName, vector<DiskInfoItem> &itemData)
{
	map<CString, vector<DiskInfoItem>>::iterator itor = m_mapDisk.find(strName);
	if(itor != m_mapDisk.end())
	{
		itor->second = itemData;
	}
}

//获取起始IP
CString CMzdIOMgr::GetSevDefIP()
{
	CString strIP;
	CString strStartIP = GetSrvDefaultIP();
	char sSIP[128] = {0}; 
	size_t nSize = 0;
	wcstombs_s(&nSize, sSIP, 128, strStartIP.GetString(), sizeof(sSIP));
	DWORD dwIP = inet_addr(sSIP) & 0xFFFFFF;

	sockaddr_in sMSrvIP = { 0 };
	sMSrvIP.sin_addr.S_un.S_addr = dwIP + 0x01000000;
//	if ( m_iFlag == 3 ) sMSrvIP.sin_addr.S_un.S_addr = dwIP + 0x02000000;

	strIP = inet_ntoa(sMSrvIP.sin_addr);
	return strIP;
}
DWORD CMzdIOMgr::IPStrToInt(LPCTSTR pStrIP)
{
	return UI_Get_Server_IP(CString(pStrIP));
}
CString IPIntToStr(DWORD dwIP)
{
	return IP_To_Str(dwIP);
}
CString CMzdIOMgr::GetImgPath(LPCTSTR pStrUpd)
{
	CString strImgPath;
	if(MZDUI_GET_IMAGE_LISTS(CString(pStrUpd), "CreateType") == L"1")
	{
		strImgPath = MZDUI_GET_IMAGE_LISTS(CString(pStrUpd), "Own_Up");
	}
	return strImgPath;
}
UINT8 *CMzdIOMgr::GetMzdCmdBuff()
{
	return (UINT8 *)UI_Get_Cmd_Buf();
}
INT32 CMzdIOMgr::GetProgressPos(LPCTSTR pServerName)
{
	CString strSvrName(pServerName);
	DWORD SrvIP = UI_Get_Server_IP(strSvrName);

	MZDH_IMG_MERGE Img_Merge = { 0 };

	int iMgrRet = UI_Cmd_SR(eMZDH_CMD_IMAGE_Merge, (char *)&Img_Merge, sizeof(BYTE), SrvIP);
	if ( iMgrRet<0 ) 
	{
		Util::Log::Error(_T("Server"), _T("[error]发送获取进度值命令出错<ServerName=%s>\r\n"), pServerName);
		return -1; // Cmd_SR Error
	}

	PMZDH_IMG_MERGE pImg_Merge = &Img_Merge;

	iMgrRet = *(PDWORD)(&pImg_Merge->UdoNote); // Ack Is Merge_Rate
	if ( iMgrRet<0 ) 
	{
		Util::Log::Error(_T("Server"), _T("[error]任务进度出错\r\n"));
		return -2; // 任务错误
	}
	if ( iMgrRet ==0 )
	{
		Util::Log::Error(_T("Server"), _T("[error]没有任务进度\r\n"));
		return 0;  // 没有任务
	}
	if ( iMgrRet>=100) 
	{
		Util::Log::Info(_T("Server"), _T("[info]任务进度完成\r\n"));
		return 100;  //完成
	}
	return iMgrRet;
}
INT32 CMzdIOMgr::SendUICmdSR(UINT32 u32Cmd, UINT8 *pBuff, UINT32 u32Size, DWORD dwIP, UINT32 u32Flag)
{
	INT32 nRel = 0;
	if(NULL == pBuff)
		return nRel;
	if(0 == u32Flag)//表示缓冲区
	{
		nRel = UI_Cmd_SR(u32Cmd, (char *)pBuff, u32Size, dwIP);
	}
	else if(1 == u32Flag)//MZDH_IMG_MERGE
	{
		MzdImgMerge *pMerge = (MzdImgMerge *)pBuff;
		MZDH_IMG_MERGE Img_Merge = { 0 };
		CStringA strATmp;
		Util::String::W_2_Utf8(pMerge->strDst, strATmp);
//		strcpy(Img_Merge.Dst, strATmp.GetString());
		strcpy_s(Img_Merge.Dst, MAX_PATH, strATmp.GetString());
		Util::String::W_2_Utf8(pMerge->strSrc, strATmp);
//		strcpy(Img_Merge.Src, strATmp.GetString());
		strcpy_s(Img_Merge.Src, MAX_PATH, strATmp.GetString());
		Util::String::W_2_Utf8(pMerge->strNote, strATmp);
//		strcpy(Img_Merge.UdoNote, strATmp.GetString());
		strcpy_s(Img_Merge.UdoNote, MZDH_NOTE_SIZE, strATmp.GetString());
		Img_Merge.UdoNums = pMerge->n32Type;
		nRel = UI_Cmd_SR(u32Cmd, (char *)&Img_Merge, sizeof(MZDH_IMG_MERGE), dwIP);
	}
	return nRel;
}
void CMzdIOMgr::GetNetIP(CString &strGetway, CString &strDNSI1, CString &strDNSI2, CString &strMask)
{
	CString strWks, strMenu;
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.begin();
	if(itor != m_mapWorkstation.end())
	{
		strWks.Format(_T("%u"), itor->first);
		strMenu = MZDUI_GET_WKS(strWks, "MenuSet1");
		strGetway = MZDUI_GET_MENU(strMenu, "Gateway");
		strDNSI1 = MZDUI_GET_MENU(strMenu, "Dns1");
		strDNSI2 = MZDUI_GET_MENU(strMenu, "Dns2");
		strMask = MZDUI_GET_MENU(strMenu, "wksSubMask");
	}
}
UINT32 CMzdIOMgr::GetWorkstationMaxNum()
{
	map<UINT32, WorkstationInfo>::reverse_iterator reitor = m_mapWorkstation.rbegin();
	if(reitor != m_mapWorkstation.rend())
		return reitor->first;
	return 0;
}
void  CMzdIOMgr::GetServerDiskData(LPCTSTR strName, vector<CString> &vecDiskPath)
{
	map<CString, vector<CString>>::iterator itor = m_mapDiskInfo.find(strName);
	if(itor != m_mapDiskInfo.end())
	{
		vecDiskPath = itor->second;
	}
}
void CMzdIOMgr::GetWorkstationMenuDiskInfo(UINT32 u32Num, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo)
{
	//注：DiskSet1固定是系统盘
	CString strWksNum, strMenu, strRet, strRetB;
	strWksNum.Format(_T("%u"), u32Num);
	for ( int i = 0; i < 4; i++ )
	{
		MenuDiskInfo info;
		CStringA strMSet; 
		strMSet.Format("MenuSet%d", i+1); // !!
		CString	strMenuName;
		strMenuName.Format(_T("%d"), i+1);
		strMenu = MZDUI_GET_WKS(strWksNum, strMSet.GetBuffer());
		if(strMenu.GetLength() != 0)
		{
			strRet = MZDUI_GET_MENU(strMenu, "displayName");
			if(strRet.GetLength() == 0)
			{
				strRet.Format( _T("菜单%d"), i+1 );
			}
			info.strModel = MZDUI_GET_MENU(strMenu, "BootOrd");
			
			vector<MenuDiskInfo> vecDiskInfo;
			CStringA strASet;
			for( int iCyc = 1; iCyc <= 4; iCyc++ )
			{ 
				strASet.Format("DskSet%d", iCyc);				
				CString strDiskName;
				strDiskName.Format(_T("DskSet%d"), iCyc);
				info.strDiskName = strDiskName;
				strRet = MZDUI_GET_MENU(strMenu, (char *)strASet.GetString());
				if(strRet.GetLength() != 0)  			
				{   
					INT32 nPos = strRet.Find(_T("|"));
					CString strSrvName = strRet.Left(nPos-1);
					CString strDiskDir = strRet.Right(strRet.GetLength() - nPos - 2);
					int iRet = Get_Image_Info(strDiskDir, UI_Get_Server_IP( strSrvName ));

					strDiskDir = strDiskDir + L" " + Get_Image_Size_new(MZDUI_GET_IMAGE(0, "SizeMB") );
					info.strServer = strSrvName;
					info.strPath = strDiskDir;
					vecDiskInfo.push_back(info);
				}
			}//四个磁盘
			mapMenuDiskInfo[strMenuName] = vecDiskInfo;
		}
	}//四个菜单
}
void CMzdIOMgr::SetWorkStationToMZD(LPCTSTR strNum, LPCTSTR strWksNum, LPCTSTR strWksIP, WorkstationInfo &itemData, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo, vector<CString> &vecNewMenu, UINT32 u32Flag)
{
	//u8Flag:0表示添加
	CString str;
	if(0 == u32Flag)
	{
		MZDUI_SET_WKS(CString(strNum), "WksNumber", CString(strNum));
		MZDUI_SET_WKS(CString(strNum), "AutoClear", (itemData.u8Status==1)?CString(_T("1")):CString(_T("0")));
		if (itemData.strSize == L"自动") str = L"-1";
		else str = itemData.strSize;
		MZDUI_SET_WKS(CString(strNum), "WksMemory", str);
		MZDUI_SET_WKS(CString(strNum), "ComName", CString(strWksNum));
		MZDUI_SET_WKS(CString(strNum), "WksIP", CString(strWksIP));
		if ( itemData.strDPI == L"-1(自动)" ) str = L"-1";
		else str = itemData.strDPI;
		MZDUI_SET_WKS(CString(strNum), "Scr_Res", str);
	}
	else if(0 < u32Flag)
	{
		CString strOldNum;
		strOldNum.Format(_T("%u"), u32Flag);
		MZDUI_SET_WKS(strOldNum, "WksNumber", CString(strNum));
		if (itemData.strSize == L"自动") str = L"-1";
		else str = itemData.strSize;
		MZDUI_SET_WKS(CString(strNum), "WksMemory", str);
		MZDUI_SET_WKS(CString(strNum), "ComName", itemData.strName);
		MZDUI_SET_WKS(CString(strNum), "WksIP", itemData.strIP);
		if ( itemData.strDPI == L"-1(自动)" ) str = L"-1";
		else str = itemData.strDPI;
		MZDUI_SET_WKS(CString(strNum), "Scr_Res", str);
		WorkstationInfo info;
		GetSimpleWorStation(u32Flag, info);
		if(0 != info.strMac.Compare(itemData.strMac))
			MZDUI_SET_WKS(CString(strNum), "isMacChanged", CString(_T("1"))); 
		MZDUI_SET_WKS(CString(strNum), "MAC", itemData.strMac);

		MZDUI_SET_WKS(CString(strNum), "MenuSet1", (CStringW)L"" );
		MZDUI_SET_WKS(CString(strNum), "MenuSet2", (CStringW)L"" );
		MZDUI_SET_WKS(CString(strNum), "MenuSet3", (CStringW)L"" );
		MZDUI_SET_WKS(CString(strNum), "MenuSet4", (CStringW)L"" );
	}
	int Wks_Menum_Cfgs = 1;
	INT32 nVecSize = vecNewMenu.size();
	map<CString, vector<MenuDiskInfo>>::iterator itor = mapMenuDiskInfo.begin();
	while(itor != mapMenuDiskInfo.end())
	{
		CStringA MenuSet; 
		MenuSet.Format("MenuSet%d", Wks_Menum_Cfgs);
		CString strMenu;
		if(Wks_Menum_Cfgs>nVecSize)
			strMenu = _T("");
		else
			strMenu = vecNewMenu[(Wks_Menum_Cfgs-1)];
		MZDUI_SET_WKS(CString(strNum), MenuSet.GetBuffer(), strMenu);
		Wks_Menum_Cfgs++;
		itor++;
	}

}
void CMzdIOMgr::SetWorkStationMenuInfo(WorkstationInfo &itemData, map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo, vector<CString> &vecNewMenu)
{
	int Wks_Menum_Cfgs = 1;
	map<CString, vector<MenuDiskInfo>>::iterator itor = mapMenuDiskInfo.begin();
	while(itor != mapMenuDiskInfo.end())
	{
		CString strMeunName, strBootType, strDiskPath1, strDiskPath2, strDiskPath3, strDiskPath4;
		INT32 nVecSize = itor->second.size();
		for (INT32 nIndex=0; nIndex<nVecSize; nIndex++)
		{
			if(strBootType.IsEmpty())//只有系统盘才有启动模式
				strBootType = itor->second[0].strModel;
			if(0 == nIndex && !itor->second[0].strServer.IsEmpty() && !itor->second[0].strPath.IsEmpty())
				strDiskPath1 = itor->second[0].strServer + _T(" | ") + itor->second[0].strPath;
			if(1 == nIndex && !itor->second[1].strServer.IsEmpty() && !itor->second[1].strPath.IsEmpty())
				strDiskPath2 = itor->second[0].strServer + _T(" | ") + itor->second[1].strPath;
			if(2 == nIndex && !itor->second[2].strServer.IsEmpty() && !itor->second[2].strPath.IsEmpty())
				strDiskPath3 = itor->second[0].strServer + _T(" | ") + itor->second[2].strPath;
			if(3 == nIndex && !itor->second[3].strServer.IsEmpty() && !itor->second[3].strPath.IsEmpty())
				strDiskPath4 = itor->second[0].strServer + _T(" | ") + itor->second[3].strPath;
		}
		strDiskPath1.Replace(_T("  "), _T(" "));
		strDiskPath2.Replace(_T("  "), _T(" "));
		strDiskPath3.Replace(_T("  "), _T(" "));
		strDiskPath4.Replace(_T("  "), _T(" "));
		CString strMenu; // 在已有菜单里找相同的

		for ( int MenuID = 0; ; MenuID++ )
		{ 
			strMenu = MZDUI_GET_MENU(MenuID, "MenuName");
			if ( *strMenu==0 ) break;

			BOOL isExistMenu = FALSE;
			do{
				if ( MZDUI_GET_MENU(MenuID, "displayName") != strMeunName ) break;
				if (0 != MZDUI_GET_MENU(MenuID, "DskSet1").Compare(strDiskPath1)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "DskSet2").Compare(strDiskPath2)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "DskSet3").Compare(strDiskPath3)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "DskSet4").Compare(strDiskPath4)) break;

				if (0 != MZDUI_GET_MENU(MenuID, "BootOrd").Compare(strBootType)) break;

				if (0 != MZDUI_GET_MENU(MenuID, "NumberLength").Compare(itemData.strNumLen)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "ComPre").Compare(itemData.strFName)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "ComPos").Compare(itemData.strBName)) break;
				//					if ( MZDUI_GET_MENU(MenuID, "StartNumber") != StartNumber ) break;
				if (0 != MZDUI_GET_MENU(MenuID, "wksStartIp").Compare(itemData.strIP)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "wksSubMask").Compare(itemData.strMask)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "Gateway").Compare(itemData.strGetway)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "Dns1").Compare(itemData.strDNSI1)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "Dns2").Compare(itemData.strDNSI2)) break;
				CString strDelay;
				strDelay.Format(_T("%u"), itemData.u32Delay);
				if (0 != MZDUI_GET_MENU(MenuID, "Wait_Times").Compare(strDelay)) break;
				if (0 != MZDUI_GET_MENU(MenuID, "Scr_Res").Compare(itemData.strDPI)) break;

				isExistMenu = TRUE;	
			}while(FALSE);

			if ( isExistMenu==TRUE ) break;
		}

		if ( *strMenu==0 ) // 生成新菜单
		{
			strMenu = CreateMenuName();
			MZDUI_SET_MENU(strMenu, "MenuName", strMenu);
			MZDUI_SET_MENU(strMenu, "displayName", strMeunName);

			MZDUI_SET_MENU(strMenu, "DskSet1", strDiskPath1);
			MZDUI_SET_MENU(strMenu, "DskSet2", strDiskPath2);
			MZDUI_SET_MENU(strMenu, "DskSet3", strDiskPath3);
			MZDUI_SET_MENU(strMenu, "DskSet4", strDiskPath4);
			MZDUI_SET_MENU(strMenu, "BootOrd", strBootType);

			MZDUI_SET_MENU(strMenu, "NumberLength", itemData.strNumLen);
			MZDUI_SET_MENU(strMenu, "ComPre", itemData.strFName);
			MZDUI_SET_MENU(strMenu, "ComPos", itemData.strBName);
			MZDUI_SET_MENU(strMenu, "StartNumber", CString(_T("0")));
			MZDUI_SET_MENU(strMenu, "wksStartIp", itemData.strIP);
			MZDUI_SET_MENU(strMenu, "wksSubMask", itemData.strMask);
			MZDUI_SET_MENU(strMenu, "Gateway", itemData.strGetway);
			MZDUI_SET_MENU(strMenu, "Dns1", itemData.strDNSI1);
			MZDUI_SET_MENU(strMenu, "Dns2", itemData.strDNSI2);
			CString strDelay;
			strDelay.Format(_T("%u"), itemData.u32Delay);
			MZDUI_SET_MENU(strMenu, "Wait_Times", strDelay);
			MZDUI_SET_MENU(strMenu, "Scr_Res", itemData.strDPI);
		}
		vecNewMenu.push_back(strMenu);
		itor++;
	}
}
void CMzdIOMgr::SetWorkstationDiskInfo(map<CString, vector<MenuDiskInfo>> &mapMenuDiskInfo)
{
	map<CString, vector<MenuDiskInfo>>::iterator itor = mapMenuDiskInfo.begin();
	while(itor != mapMenuDiskInfo.end())
	{
		INT32 nSize = itor->second.size();
		for(INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			CString strDiskPath = itor->second[nIndex].strPath;
			if(strDiskPath.IsEmpty())
			{
				continue;
			}
			CString strDiskOwn;
			INT32 iRet = strDiskPath.ReverseFind(L'.');
			if(iRet > 0)
			{
				CString strSuffix = strDiskPath.Right(strDiskPath.GetLength() - iRet);
				if(strSuffix.CompareNoCase(L".Upd") == 0)    //需要改进
				{
					DWORD SrvIP = UI_Get_Server_IP(itor->second[nIndex].strServer);

					if ( UI_Cmd_SR(eMZDH_CMD_IMAGE_Own, strDiskPath, SrvIP)>=0 )
					{
						strDiskOwn = MZDUI_GET_IMAGE(0, "Path");
					}
				}
			}
			CString strPath = itor->second[nIndex].strServer + _T(" | ") + strDiskPath;

			MZDUI_SET_DISK(strPath, "DiskDir", strDiskPath);
			MZDUI_SET_DISK(strPath, "DiskName", strPath);  
			MZDUI_SET_DISK(strPath, "DiskOwn", strDiskOwn);
		}
		itor++;
	}
}
void CMzdIOMgr::SetServerInfoToMZD(LPCTSTR pstrIP, LPCTSTR pStrOldName, LPCTSTR pStrNewName, ServerInfo &Info, UINT32 u32Flag)
{
	CString strServerName;
	if(0 == u32Flag)
	{
		MZDUI_SET_SERVER(Info.strName, "ServerName", Info.strName);
		strServerName = Info.strName;
	}
	else 
	{
		strServerName = pStrOldName;
	}
	CString str;
	str.Format(_T("%u"), Info.u32SysDiskSize);
	MZDUI_SET_SERVER(strServerName, "SystemCacheSize", str);
	str.Format(_T("%u"), Info.u32DataDiskSize);
	MZDUI_SET_SERVER(strServerName, "DataCacheSize", str);
	str.Format(_T("%u"), Info.u32SSDDiskSize);
	MZDUI_SET_SERVER(strServerName, "SSDCacheSize", str);

	MZDUI_SET_SERVER(strServerName, "BackupServer", Info.strSqareServer);


	INT32 nSize = Info.vecRWrite.size();
	for (INT32 nIndex=0; nIndex<nSize; nIndex++)
	{
		char NodeName[20];
//		sprintf(NodeName, "workDir%d", nIndex + 1);
		sprintf_s(NodeName, 20, "workDir%d", nIndex + 1);
		Info.vecRWrite[nIndex].Replace(_T("\\\\.\\"), _T(""));
		MZDUI_SET_SERVER(strServerName, NodeName, Info.vecRWrite[nIndex] + L"\\Wks");
	}
	for (INT32 nIndex2=nSize ; nIndex2<8; nIndex2++ )
	{
		char NodeName[20];
//		sprintf(NodeName, "workDir%d", nIndex2 + 1 );
		sprintf_s(NodeName, 20, "workDir%d", nIndex2 + 1);
		MZDUI_SET_SERVER(strServerName, NodeName, (CStringW)L"");
	}
	vector<CString> vecSqlit;
	Util::String::StringSplit(pstrIP, _T(", "), vecSqlit);
	nSize = vecSqlit.size();
	for (INT32 nIndex3=0; nIndex3<4; nIndex3++)
	{
		char NodeName[20];
//		sprintf(NodeName, "AdaptIp%d", nIndex3+1);
		sprintf_s(NodeName, 20, "AdaptIp%d", nIndex3+1);
		CString strIP;
		if(nIndex3 < nSize)
		{
			strIP = vecSqlit[nIndex3];
		}
		MZDUI_SET_SERVER(strServerName, NodeName, strIP);
	}
	
	if(0 != u32Flag)
	{
		MZDUI_SET_SERVER(CString(pStrOldName), "ServerName", CString(pStrNewName));  //最后用新值覆盖原有值
	}
	for( int i = 0; MZDUI_GET_DISK(i, "DiskName")!=L""; i++ )
	{  
		CString strTmp = MZDUI_GET_DISK(i, "DiskName");
		vector<CString> vecSq;
		Util::String::StringSplit(strTmp, _T(" | "), vecSq);
		CString strOldName(pStrOldName);
		if(strOldName.CompareNoCase(vecSq[0]) == 0 ) 
		{
			CString strDTmp = CString(pStrNewName) + L" | " + vecSq[1];
			MZDUI_SET_DISK(strTmp, "DiskName", strDTmp);

			for( int i = 0; MZDUI_GET_MENU(i, "MenuName")!=L""; i++ )
			{  
				if( MZDUI_GET_MENU(i, "DskSet1") == strTmp )  MZDUI_SET_MENU(MZDUI_GET_MENU(i, "MenuName"), "DskSet1", strDTmp);
				if( MZDUI_GET_MENU(i, "DskSet2") == strTmp )  MZDUI_SET_MENU(MZDUI_GET_MENU(i, "MenuName"), "DskSet2", strDTmp);
				if( MZDUI_GET_MENU(i, "DskSet3") == strTmp )  MZDUI_SET_MENU(MZDUI_GET_MENU(i, "MenuName"), "DskSet3", strDTmp);
				if( MZDUI_GET_MENU(i, "DskSet4") == strTmp )  MZDUI_SET_MENU(MZDUI_GET_MENU(i, "MenuName"), "DskSet4", strDTmp);
			}
		}
	}
}
void CMzdIOMgr::GetWorkstationStatus(map<CString, UINT32> &mapStatus)
{
	CString strWks, strOnline;
	m_u32OnlineCount = 0;
	m_mapWorkstationStatus.clear();
	for( int i = 0; i<MZD_MAX_WKS_NUMS; i++ )
	{
		if (( strWks = MZDUI_GET_WKS(i, "WksNumber")) == _T("")) 
			break;
		if ( strWks == _T("0") ) 
			continue; // 0: Wks Is DHCP 
		strOnline = MZDUI_GET_WKS(strWks, "OnLine_Flag");
		if(strOnline == _T("1"))
			m_u32OnlineCount++;
		m_mapWorkstationStatus[strWks] = _ttoi(strOnline);
	}
	mapStatus = m_mapWorkstationStatus;
}
UINT32 CMzdIOMgr::GetWorkStationStatus(LPCTSTR pStrNum)
{
	map<CString, UINT32>::iterator itor = m_mapWorkstationStatus.find(CString(pStrNum));
	if(itor != m_mapWorkstationStatus.end())
	{
		return itor->second;
	}
	return 0;
}
UINT32 CMzdIOMgr::GetWokstationOnlineCount()
{
	return m_u32OnlineCount;
}
UINT32 CMzdIOMgr::GetWokstationCount()
{
	return m_mapWorkstationStatus.size();
}
void CMzdIOMgr::GetServerStatus(map<CString, UINT32> &mapStatus)
{
	UI_Cmd_SR(eMZDH_CMD_FIND_IO_SRV, 0, 0);

	if(UI_Cmd_SR(eMZDH_CMD_GET_IO_SRV, 0, 0) < 0) 
	{
		return ; 
	}
	CString strName;
	INT32 nNum = 0;
	while(true)
	{
		strName = MZDUI_GET_SERVER(nNum, "ServerName");
		if(strName.IsEmpty())
			break;

		CString strIP = MZDUI_GET_SERVER(nNum, "AdaptIp1");
		if (strIP==L"") 
		{
			m_mapServerStatus[strName] = 0;
			nNum++;
			continue;
		}
		if (strIP==L"127.0.0.1") 
			m_mapServerStatus[strName] = 1;
		else 
		{
			for(INT32 nIndex = 0; ; nIndex++)
			{
				CString strIO =  MZDUI_GET_IO_SRV(nIndex, "IO_SRV_IP00");
				if (strIO == L"") 
					break;

				if (strIO == strIP) 
				{ 
					strIP = L""; 
					break; 
				}

				CString strTmp = MZDUI_GET_IO_SRV(strIO, "IO_SRV_IP01");
				if (strTmp == strIP) 
				{
					strIP = L""; 
					break; 
				}

				strTmp = MZDUI_GET_IO_SRV(strIO, "IO_SRV_IP02");
				if (strTmp == strIP) 
				{ 
					strIP = L""; 
					break;
				}

				strTmp = MZDUI_GET_IO_SRV(strIO, "IO_SRV_IP03");
				if (strTmp == strIP)
				{ 
					strIP = L"";
					break;
				}
			}
			(strIP == L"") ? (m_mapServerStatus[strName] = 1) : (m_mapServerStatus[strName] = 0); 	
		}
		nNum++;
	}
	mapStatus = m_mapServerStatus;
}
UINT32 CMzdIOMgr::GetServerCount()
{
	return m_mapServerStatus.size();
}
UINT32 CMzdIOMgr::GetServerOnlineCount()
{
	return m_u32OnlineCountSvr;
}
void CMzdIOMgr::GetRWriteDisk(LPCTSTR pStrIP, vector<CString> &vecDisk)
{
	DWORD dwIP = Str_To_IP(CString(pStrIP));
	CString strDiskDir; 
	if (UI_Cmd_SR(eMZDH_CMD_IMAGE_Lists, strDiskDir, dwIP)>=0)
	{
		INT32 nIndex=0;
		while(true)
		{
			if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Ret")==L"" ) break; // INI End
			if ( MZDUI_GET_IMAGE_LISTS(nIndex, "Ret")==L"-1" ) break; // Img Error

			if( MZDUI_GET_IMAGE_LISTS(nIndex, "Type") != L"2" ) 
			{
				nIndex++;
				continue;
			}
			strDiskDir = MZDUI_GET_IMAGE_LISTS(nIndex, "Path");

			vecDisk.push_back(strDiskDir);
			nIndex++;
		}
	}
}
void CMzdIOMgr::GetIOServer(map<CString, vector<s_ServerIPInfo>> &mapIOServer, CString &strFindIP)
{
	DWORD dwFindIP = 0;
	if(!strFindIP.IsEmpty())
		dwFindIP = Str_To_IP(strFindIP);
	UI_Cmd_SR(eMZDH_CMD_FIND_IO_SRV,dwFindIP, 0, 0); 
	if(UI_Cmd_SR(eMZDH_CMD_GET_IO_SRV, 0, 0)< 0) 
	{
		Util::Log::Error(_T("Server"), _T("[error]发送获取IO服务器命令出错\r\n"));
		return; 
	}

	INT32 nIndex = 0;
	while(true)
	{
		vector<s_ServerIPInfo> vecIP;
		CString strIP = MZDUI_GET_IO_SRV(nIndex, "IO_SRV_IP00");
		nIndex++;
		if (strIP==L"") 
			break;		
		CString strName =  MZDUI_GET_IO_SRV(strIP, "IO_SRV_Name");

		CString strSrvIP1 = MZDUI_GET_SERVER(strName, "AdaptIp1");
		CString strSrvIP2 = MZDUI_GET_SERVER(strName, "AdaptIp2");
		CString strSrvIP3 = MZDUI_GET_SERVER(strName, "AdaptIp3");
		CString strSrvIP4 = MZDUI_GET_SERVER(strName, "AdaptIp4");
		if(!strIP.IsEmpty())
		{
			s_ServerIPInfo of;
			of.strIP = strIP;
			if(strIP == strSrvIP1 || strIP == strSrvIP2 || strIP == strSrvIP3 || strIP == strSrvIP4)
				of.u8Sel = 1;
			vecIP.push_back(of);
		}

		CString strTmp = MZDUI_GET_IO_SRV(strIP, "IO_SRV_IP01");
		if(!strTmp.IsEmpty())
		{
			s_ServerIPInfo of;
			of.strIP = strTmp;
			if(strTmp == strSrvIP1 || strTmp == strSrvIP2 || strTmp == strSrvIP3 || strTmp == strSrvIP4)
				of.u8Sel = 1;
			vecIP.push_back(of);
		}
		strTmp = MZDUI_GET_IO_SRV(strIP, "IO_SRV_IP02");
		if(!strTmp.IsEmpty())
		{
			s_ServerIPInfo of;
			of.strIP = strTmp;
			if(strTmp == strSrvIP1 || strTmp == strSrvIP2 || strTmp == strSrvIP3 || strTmp == strSrvIP4)
				of.u8Sel = 1;
			vecIP.push_back(of);
		}
		strTmp = MZDUI_GET_IO_SRV(strIP, "IO_SRV_IP03");
		if(!strTmp.IsEmpty())
		{
			s_ServerIPInfo of;
			of.strIP = strTmp;
			if(strTmp == strSrvIP1 || strTmp == strSrvIP2 || strTmp == strSrvIP3 || strTmp == strSrvIP4)
				of.u8Sel = 1;
			vecIP.push_back(of);
		}
		mapIOServer[strName] = vecIP;
	}
}
INT32 CMzdIOMgr::CheckWorkstationNum(LPCTSTR pStrNum, INT32 nNumLen, LPCTSTR strComF, LPCTSTR strComB, LPCTSTR pStartIP, UINT32 u32Flag, CString &strWksNum, CString &strWksIP)
{
	//u8Flag:0表示添加工作站
	CString strNum(pStrNum);
	CString strWksName;
	INT32 nRel = 0;
	if(0 == u32Flag)//添加工作站
	{
		nRel = WorkstationNumClashCheck(0, "WksNumber", strNum, strNum, _T("与工作站"), _T("编号冲突"));
		if(nRel!=2)
			return nRel;
	}
	if(0 == u32Flag)
	{
		CString NumberLength, ComPre, ComPos, StartNumber, wksStartIp, wksSubMask, Gateway;

		strWksName = CreateWorkstationNum(pStrNum, nNumLen, strComF, strComB, _T("0"));
		strWksNum = strWksName;
		INT32 nRel = WorkstationNumClashCheck(0, "WksName", strWksName, strNum, _T("与工作站"), _T("计算机名冲突"));
		if (nRel != 2) 
			return nRel;

		strWksIP = CreateWorkstationIP(strNum, pStartIP);
		nRel = WorkstationNumClashCheck(2, "WksIP", strWksIP, pStrNum,  _T("IP错误"), _T(""));
		if (nRel != 2) 
			return nRel;
	}
	nRel = WorkstationNumClashCheck(0, "WksIP", strWksIP, pStrNum, _T("与工作站"), _T("IP冲突"));
	if (nRel != 2) 
		return nRel;

	nRel = WorkstationNumClashCheck(1, "AdaptIp1", strWksIP, pStrNum, _T("与服务器设置"), _T("的网卡IP冲突"));
	if (nRel != 2) 
		return nRel;
	nRel = WorkstationNumClashCheck(1, "AdaptIp2", strWksIP, pStrNum, _T("与服务器设置"), _T("的网卡IP冲突"));
	if (nRel != 2) 
		return nRel;
	nRel = WorkstationNumClashCheck(1, "AdaptIp3", strWksIP, pStrNum, _T("与服务器设置"), _T("的网卡IP冲突"));
	if (nRel != 2) 
		return nRel;
	nRel = WorkstationNumClashCheck(1, "AdaptIp4", strWksIP, pStrNum, _T("与服务器设置"), _T("的网卡IP冲突"));
	if (nRel != 2)
		return nRel;

	return (nRel==2) ? 1 : nRel;
}
void CMzdIOMgr::CheckUnuserMenu()
{
	for ( int i = 0; ; i++ )
	{  
		CStringW strMenu = MZDUI_GET_MENU(i, "MenuName");
		if  ( *strMenu==0 ) break;

		for ( int j = 0; MZDUI_GET_WKS(j, "WksNumber")!=L""; j++ )
		{
			if ( MZDUI_GET_WKS(j, "MenuSet1")==strMenu ) { strMenu = L""; break; }
			if ( MZDUI_GET_WKS(j, "MenuSet2")==strMenu ) { strMenu = L""; break; }
			if ( MZDUI_GET_WKS(j, "MenuSet3")==strMenu ) { strMenu = L""; break; }
			if ( MZDUI_GET_WKS(j, "MenuSet4")==strMenu ) { strMenu = L""; break; }
		}
		if ( *strMenu!=0 ) 
		{
			MZDUI_DEL_MENU(strMenu); 
			i--; 
		}
	}
}
void CMzdIOMgr::CheckUnuserDisk()
{
	for ( int iCnt = 0; ; iCnt++ )
	{  
		CString strDName = MZDUI_GET_DISK(iCnt, "DiskName");
		if ( *strDName==0 ) break;

		for ( int jCnt = 0; MZDUI_GET_MENU(jCnt, "MenuName")!=L""; jCnt++ )
		{  
			if( ( MZDUI_GET_MENU(jCnt, "DskSet1") == strDName ) || 
				( MZDUI_GET_MENU(jCnt, "DskSet2") == strDName ) ||
				( MZDUI_GET_MENU(jCnt, "DskSet3") == strDName ) || 
				( MZDUI_GET_MENU(jCnt, "DskSet4") == strDName )) 
			{
				strDName = L""; break;

			}
		}

		if ( *strDName!=0 )
		{ 
			MZDUI_DEL_DISK(strDName); 
			iCnt--; 
		}
	}
}
void CMzdIOMgr::CheckUnuserServer()
{
	for( int iCnt = 0; ; iCnt++ )
	{  
		CStringW strSrvName = MZDUI_GET_SERVER(iCnt, "ServerName");
		if ( *strSrvName == 0 ) break;

		CStringW strDir =  MZDUI_GET_SERVER(strSrvName, "workDir1");
		if ( strDir==L"" ) 
		{ 
			MZDUI_DEL_SERVER(strSrvName);
			iCnt--;
		}
	}
}
CString CMzdIOMgr::GetSuperMgr()
{
	return MZDUI_GET_GLOBAL(0, "SuperWksNum");
}
INT32 CMzdIOMgr::EndSuperMgr(LPCTSTR pStrNum)
{
	return UI_End_Super_Wks(_ttoi(pStrNum));
}
void CMzdIOMgr::SetSuperMgr(LPCTSTR strNum)
{
	MZDUI_SET_GLOBAL("SuperWksNum", CString(strNum));
}
void CMzdIOMgr::DelWorkstation(LPCTSTR pStrNum)
{
	MZDUI_DEL_WKS(CString(pStrNum));
}
void CMzdIOMgr::DisableWorkstation(LPCTSTR pStrNum)
{
	UI_Cmd_SR(eMZDH_CMD_WKS_DISABLE, _ttoi(pStrNum), 0, 0);
}
void CMzdIOMgr::EnableWorkstation(LPCTSTR pStrNum)
{
	UI_Cmd_SR(eMZDH_CMD_WKS_ENABLE, _ttoi(pStrNum), 0, 0);
}
void CMzdIOMgr::RebootWorkstation(LPCTSTR pStrNum)
{
	UI_Cmd_SR(eMZDH_CMD_WKS_REBOOT, _ttoi(pStrNum), 0, 0);
}
void CMzdIOMgr::CloseWorkstation(LPCTSTR pStrNum)
{
	UI_Cmd_SR(eMZDH_CMD_WKS_POWEROFF, _ttoi(pStrNum), 0, 0);
}
INT32 CMzdIOMgr::DelServer(LPCTSTR pStrName)
{
	CString strDiskName;
	for(INT32 nIndex = 0; ; nIndex++ )
	{   
		strDiskName = MZDUI_GET_DISK(nIndex, "DiskName");
		if (strDiskName==L"") 
			break;
		vector<CString> vecSqlite;
		Util::String::StringSplit(strDiskName, _T(" | "), vecSqlite);

	if(0 > vecSqlite[0].Find(pStrName)) 
		continue;
	return -1;
	}
	
	MZDUI_DEL_SERVER(CString(pStrName));
	return 0;
}
INT32 CMzdIOMgr::DelDisk(LPCTSTR pStrName, LPCTSTR pStrPath)
{
	CString CheckRet = MZUI_INI_CheckString("DiskCfg", "DiskName", (CString)L"", "DiskName", CString(pStrName) + L" | " + CString(pStrPath));
	if ( CheckRet != L"" )
	{
		return - 1;//选定的磁盘在工作站菜单中已使用 ,不能删除！
	}

	DWORD SrvIP = UI_Get_Server_IP(CString(pStrName));
	PCHAR pBuff = UI_Get_Cmd_Buf(); 
	CStringA strAPath;
	Util::String::W_2_Utf8(pStrPath, strAPath);
	strcpy(pBuff, strAPath.GetString());
	UI_Cmd_SR(eMZDH_CMD_IMAGE_Del, pBuff, MAX_PATH, SrvIP);
	return 0;
}
void CMzdIOMgr::GetDiskSerial(vector<CString> &vecDiskSerial)
{
	UI_Cmd_SR(eMZDH_CMD_GET_HDID,0,0);
	CString strHdid;
	INT32 nIndex = 0;
	do 
	{
		strHdid = MZDUI_GET_HDID(nIndex++, "HDID");
		if(!strHdid.IsEmpty())
		{
			vecDiskSerial.push_back(strHdid);
		}
	} while (!strHdid.IsEmpty());
}
INT32 CMzdIOMgr::Register(LPCTSTR pStrSerial, LPCTSTR pStrPass, LPCTSTR pStrPassword, LPCTSTR pStrSecurity, CString &strTips)
{
	INT32 iRet = UI_Get_Reg(CString(pStrSerial), CString(pStrSecurity), CString(pStrPass), CString(pStrPassword));
	CString RegMsg = L"MZDH_IO服务错误";

	do{
		if ( iRet<0 ) 
		{ 
			RegMsg = L"连接MZD服务 错误"; 
			break; 
		}
		RegMsg = MZDUI_GET_REG(0, "RegRet");
		if ( RegMsg==L"" ) 
		{ 
			RegMsg = L"MZDH_IO服务错误"; 
			break; 
		}

		iRet =  _wtoi(RegMsg);

		if ( iRet >= 0 ) 
		{ 
			UI_Cmd_SR(eMZDH_CMD_CHECK_REG,0,0);
			RegMsg = L"名智系统注册成功！"; 
			break; 
		}

		switch (iRet)
		{
		case -4: RegMsg = L"连接注册服务器超时"; break;
		case -7: RegMsg = L"注册服务器忙"; break;
		case -8: RegMsg = L"注册服务器返回数据错误"; break;
		case -20: RegMsg = MZDUI_GET_REG(0, "RegMsg"); break;
		case -30: RegMsg = L"无法保存注册信息"; break; 
		default: break;
		}

	}while(FALSE);

	if( RegMsg == L"名智系统注册成功！" )
	{
		CString strTmp = _T("0");
		MZDUI_SET_GLOBAL("isHideRegFMFlag", strTmp);
	}
	if(0 > iRet)
	{
		Util::Log::Error(_T("Server"), _T("[error]注册失败:%s\r\n"), RegMsg);
	}
	return iRet;
}
INT32 CMzdIOMgr::UserIOServer()
{
	INT32 nRel = UI_Cmd_SR(eMZDH_CMD_CLEAR_REG, 0, 0);
	if(nRel < 0) 
		return nRel;

	CString strTmp = _T("1");
	MZDUI_SET_GLOBAL("isHideRegFMFlag", strTmp);
	return nRel;
}
//清楚mac
void CMzdIOMgr::ClearMac(LPCTSTR pStrNum)
{
	CString strNum(pStrNum);
	MZDUI_SET_WKS(strNum, "WksNumber", strNum);
	MZDUI_SET_WKS(strNum, "isMacChanged", CString(_T("1"))); 
	MZDUI_SET_WKS(strNum, "MAC", CString(_T("")));
}
//唤醒工作站
void CMzdIOMgr::WakeUpWorkStation(LPCTSTR pStrNum)
{
	UI_Cmd_SR(eMZDH_CMD_WKS_WAKEUP, _ttoi(pStrNum), 0, 0);
}
//是否自动添加工作站
BOOL CMzdIOMgr::isAutoAdd()
{
	return (0 == MZDUI_GET_GLOBAL(0, "AutoAdd").Compare(_T("1")))? TRUE : FALSE;
}
//设置自动添加工作站
void CMzdIOMgr::SetAutoAdd(BOOL bAdd)
{
	MZDUI_SET_GLOBAL("AutoAdd", bAdd? CString(_T("1")) : CString(_T("0")));
}
//获取分组
void CMzdIOMgr::GetAllGroup(map<UINT32, CString> &mapGroup)
{
	mapGroup = m_mapWorkstationGroup;
}
//添加分组
INT32 CMzdIOMgr::AddWorkstationGroup(LPCTSTR pStrGroupName)
{
	MZDUI_SET_GROUP(CString(pStrGroupName), "GroupName", CString(pStrGroupName));
	UINT32 nMaxID = 0;
	if(0 < m_mapWorkstationGroup.size())
	{
		map<UINT32,	CString>::reverse_iterator itor = m_mapWorkstationGroup.rbegin();
		nMaxID = itor->first;
	}
	nMaxID += 1;
	CString strID;
	strID.Format(_T("%u"), nMaxID);
	MZDUI_SET_GROUP(CString(pStrGroupName), "GroupID", strID);
	strID = MZDUI_GET_GROUP(CString(pStrGroupName), "GroupID");
	UINT32 uID = (UINT32)_ttoi(strID);
	m_mapWorkstationGroup[uID] = CString(pStrGroupName);
	return (INT32)uID;
}
//修改工作站分组
void CMzdIOMgr::ModifyWorkstationGroup(UINT32 uGroupID, LPCTSTR pStrGroupName)
{
	map<UINT32,	CString>::iterator itor = m_mapWorkstationGroup.find(uGroupID);
	if(itor != m_mapWorkstationGroup.end())
	{
		MZDUI_SET_GROUP(itor->second, "GroupName", CString(pStrGroupName));
		itor->second = CString(pStrGroupName);
	}
	
}
//删除工作站分组
void CMzdIOMgr::DelWorkstationGroup(UINT32 uGroupID)
{
	map<UINT32,	CString>::iterator itor = m_mapWorkstationGroup.find(uGroupID);
	if(itor != m_mapWorkstationGroup.end())
	{
		MZDUI_DEL_GROUP(itor->second);
		m_mapWorkstationGroup.erase(itor);
	}

	//同时更新工作站分组ID
	CString strWks;
	map<UINT32, WorkstationInfo>::iterator itor2 = m_mapWorkstation.begin();
	while(itor2 != m_mapWorkstation.end())
	{
		if(itor2->second.u32GroupID == uGroupID)
		{
			strWks.Format(_T("%u"), itor2->first);
			MZDUI_SET_WKS(strWks, "Group_ID", CString(_T("0")));
			itor2->second.u32GroupID = 0;
		}
		itor2++;
	}
}
BOOL CMzdIOMgr::IsExistGroup(LPCTSTR pStrGroup)
{
	map<UINT32,	CString>::iterator itor = m_mapWorkstationGroup.begin();
	if(itor != m_mapWorkstationGroup.end())
	{
		if(0 == itor->second.Compare(pStrGroup))
		{
			return TRUE;
		}
		itor++;
	}
	return FALSE;
}
void CMzdIOMgr::GetUpdateWorkstationInfo(vector<WorkstationInfo> &vecUpdateWorkstation)
{
	vecUpdateWorkstation = m_vecUpdateWorkstation;
	m_vecUpdateWorkstation.clear();
}
void CMzdIOMgr::GetUpdateServerInfo(vector<ServerItem> &vecUpdateServerInfo)
{
	vecUpdateServerInfo = m_vecUpdateServer;
	m_vecUpdateServer.clear();
}
INT32 CMzdIOMgr::WorkstationNumClashCheck(INT32 Type, char *strChk_KN, CString strChk_KV, CString strWksNum, CString strErrType, CString strErrMsg)
{
//	int nCount = 0;

	if (Type==0) 
	{
		CString strCheck = MZDUI_CHK_WKS(strWksNum, strChk_KN, strChk_KV);
		if(strCheck.IsEmpty())
			return 2;
	}
	else if ( Type==1 ) 
	{
		BOOL isExist = FALSE;
		for(INT32 nIndex = 0; MZDUI_GET_SERVER(nIndex, "ServerName") != L""; nIndex++)
		{ 
			if( (MZDUI_GET_SERVER(nIndex, strChk_KN) == strChk_KV)  &&  (strChk_KV != L"") )
			{
				isExist = TRUE;
				break;
			}
		}
		if ( isExist==FALSE ) 
			return 2; // 没有冲突
	}
	else if ( Type==2 ) // IP错误
	{
		if ( strChk_KV != L"" )
			return 2;
	}

// 	CString  strMsg = _T("\r\n\r\n错误: 工作站 ") + strWksNum + L" ";
// 	strMsg +=  strErrType + L" " + m_strWksChkRet + L" " + strErrMsg; //  "编号", IP MAC 网关 服务器IP
// 	strMsg += _T("\r\n");
// 	nCount++ ;
// 
// 	strMsg += _T("\r\n继续: 忽略错误，强制覆盖 | 跳过: 不执行操作\r\n");
// 	nCount++ ;

// 	CMErrorMsgDlg MEMDlg_W;
// 	MEMDlg_W.SetCaption(L"工作站_异常状态");
// 	MEMDlg_W.Init( strMsg, nCount );
// 	switch(MEMDlg_W.DoModal())
// 	{
// 	case IDOK:
// 		iWksCheckState = WKS_IGNORE_ONE_ERROR;
// 		return 1;
// 	case IDRETRY:
// 		iWksCheckState = WKS_IGNORE_ALL_ERROR;
// 		return 1;
// 	case IDIGNORE : 
// 		iWksCheckState = WKS_JUMP_ALL_ERROR;
// 		return 0;
// 	case IDABORT:
// 		iWksCheckState = WKS_JUMP_ONE_ERROR;
// 		return 0;
// 	}

	return -1; // 取消
}
CString CMzdIOMgr::CreateWorkstationNum(LPCTSTR pStrNum, INT32 nNumLen, LPCTSTR strComF, LPCTSTR strComB, LPCTSTR strComStart)
{
	CString strWks;
	CString LenFmt;
	LenFmt.Format(L"%d", nNumLen); 
	LenFmt = L"%s%0" + LenFmt + L"d%s";

	INT32 n32Num = _ttoi(pStrNum) + _ttoi(strComStart);
	if (n32Num<0) 
		return strWks;

	strWks.Format(LenFmt, strComF, n32Num, strComB);
	return strWks;
}
CString CMzdIOMgr::CreateWorkstationIP(LPCTSTR pStrNum, LPCTSTR StrWksStartIp)
{
	INT32 n32Num = _ttoi(pStrNum);
	DWORD iStartIP = Str_To_IP(StrWksStartIp);

	n32Num = Util::Tools::CalcIp(n32Num, iStartIP, 0);
	if (n32Num==0) 
		return L"";

	return IP_To_Str(n32Num);
}
CString CMzdIOMgr::CreateMenuName()
{
	CString newName;
	for ( int MenuID = 0; MenuID<9999; MenuID++)
	{
		newName.Format(L"MENU%04d", MenuID);
		for ( int i = 0; MZDUI_GET_MENU(i, "MenuName")!=L""; i++ )
		{  
			if ( MZDUI_GET_MENU(i, "MenuName")==newName ) 
			{	 newName = L""; break; }
		}
		if ( newName!=L"" ) return newName;
	}
	return L"";
}

LRESULT CMzdIOMgr::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&bHandled)
{
	switch (wParam)
	{
	case UPDATEDATATIMER:
		OnTimerUpdateData();
		break;
	case CLICKIOSERVERRUN:
		if(!IsServiceRun(_T("MZDH_IO")))
		{
			::PostMessage(m_hWorkstationWnd, UM_ISMZDIOSERVICERUN, NULL, 0);
		}
		else
		{
			::PostMessage(m_hWorkstationWnd, UM_ISMZDIOSERVICERUN, NULL, 1);
		}
		break;
	default:
		break;
	}
	return S_OK;
}

void CMzdIOMgr::OnTimerUpdateData()
{
	BOOL bOk = SendDownLoadCmd();
	if(FALSE == bOk)
	{
		Util::Log::Error(_T("Server"), _T("[error]定时同步服务器数据，发送同步数据命令失败\r\n"));
		return ;
	}
	//1.现将本地工作站的数据临时存储一份；
	map<UINT32, WorkstationInfo> mapTempWorkstationInfo;
	mapTempWorkstationInfo = m_mapWorkstation;
	WorkstationLoadData();
	//2.判断那些数据已经更新
	map<UINT32, WorkstationInfo>::iterator itor = m_mapWorkstation.begin();
	while(itor != m_mapWorkstation.end())
	{
		map<UINT32, WorkstationInfo>::iterator itFind = mapTempWorkstationInfo.find(itor->first);
		if(itFind == mapTempWorkstationInfo.end())
		{
			m_vecUpdateWorkstation.push_back(itor->second);
			itor++;
			continue;
		}
		else
		{
			if(!(itor->second == itFind->second))
			{
				m_vecUpdateWorkstation.push_back(itor->second);
				itor++;
				continue;
			}
		}
		itor++;
	}
	if(0 < m_vecUpdateWorkstation.size())
	{
		//发送消息给工作站窗口，进行更新
		::PostMessage(m_hWorkstationWnd, UM_UPDATEWROKSTATION_MSG, 0, 0);
	}

	//1.将本地服务器的数据临时存储一份
	map<CString, ServerItem> mapTempServerInfo;
	mapTempServerInfo = m_mapServer;
	ServerLoadData();
	//2.判断更新的服务器信息
	map<CString, ServerItem>::iterator itor2 = m_mapServer.begin();
	while(itor2 != m_mapServer.end())
	{
		map<CString, ServerItem>::iterator itFind2 = mapTempServerInfo.find(itor2->first);
		if(itFind2 == mapTempServerInfo.end())
		{
			m_vecUpdateServer.push_back(itor2->second);
			itor2++;
			continue;
		}
		else
		{
			if(!(itor2->second == itFind2->second))
			{
				m_vecUpdateServer.push_back(itor2->second);
				itor2++;
				continue;
			}
		}
		itor2++;
	}
	if(0 < m_vecUpdateServer.size())
	{
		//发送消息给服务器窗口，进行更新
		::PostMessage(m_hServerWnd, UM_UPDATESERVER_MSG, 0, 0);
	}

}
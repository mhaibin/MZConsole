
//#define MZDH_UI
//#include "..\\..\\..\\mzdh_io\\mzdh_io.cpp"

#include "stdafx.h"
#include <TlHelp32.h>
#include "MZIniLib.h"
#ifdef DEBUG_MODE // XREF: MountMzdImg
	#undef DEBUG_MODE
#endif

#undef  MZDH_AS_H
#define MZDH_UI
#define MZDH_IO_IS_DIRE
#ifdef MZDH_IO_IS_DIRE
	#include "..\\..\\mzdh_io\\mzdh_io\\mzdh_io.cpp"
#else
	#include "..\\..\\mzdh_io\\mzdh_io.cpp"
#endif

////查找UDP文件对应的IMG信息
//int Get_ImageOwner_Info( CStringW &strDiskDir, DWORD dwIP)
//{
//	PCHAR pBuff = UI_Get_Cmd_Buf();
//	strcpy( pBuff, CStrW2CStrA(strDiskDir).GetBuffer() );
//
//	if ( UI_Cmd_SR(eMZDH_CMD_IMAGE_Own, pBuff, MAX_PATH, dwIP) < 0 ) return -1; // Comm Error
//
//	CStringW str =  MZDUI_GET_IMAGE(0, "Ret");
//
//	if ( str == L"0" ) return -2; // File Error Or Not Find Need
//	if ( str == L"-1" ) return -3; // Find End Error
//
//	return ( str == L"1") ? 0 : 1; // 0: File, 1: Dire
//}


//清除分组信息
void MZD_Del_GroupInfo()
{
	for( int i = 0; ; i++ )
	{  
		CStringW strGrpName = MZDUI_GET_GROUP(i, "GroupName");
		if( *strGrpName == 0 ) break;

		if( strGrpName != L"" ) 
		{
			MZDUI_DEL_GROUP(strGrpName); 
			i--; 
		}
	}
}

CStringW GetSrvDefaultIP()
{
	for ( int iNID = 0; iNID<MZDH_MAX_ADAPT_NUMS; iNID++ )
	{ 
		PMZDH_ADAPT_INFO pAdapt_Info = &MZDH_ADAPT_CLASS::s_Create().Adapt_Info[iNID];
		if ( pAdapt_Info->IpAddress==0 ) break; // End

		CStringW strSrv;
		char sSIP[128] = {0};   

		for( int i = 0; (strSrv = MZDUI_GET_SERVER(i, "ServerName")) != _T(""); i++ )
		{
			//服务器网卡IP
			strSrv = MZDUI_GET_SERVER(i, "AdaptIp1");
			if(strSrv == L"127.0.0.1") break;
			wcstombs(sSIP, strSrv.GetBuffer(0), sizeof(sSIP));
			if(inet_addr(sSIP) == pAdapt_Info->IpAddress) break;

			strSrv = MZDUI_GET_SERVER(i, "AdaptIp2");
			wcstombs(sSIP, strSrv.GetBuffer(0), sizeof(sSIP));
			if(inet_addr(sSIP) == pAdapt_Info->IpAddress) break;

			strSrv = MZDUI_GET_SERVER(i, "AdaptIp3");
			wcstombs(sSIP, strSrv.GetBuffer(0), sizeof(sSIP));
			if(inet_addr(sSIP) == pAdapt_Info->IpAddress) break;

			strSrv = MZDUI_GET_SERVER(i, "AdaptIp4");
			wcstombs(sSIP, strSrv.GetBuffer(0), sizeof(sSIP));
			if(inet_addr(sSIP) == pAdapt_Info->IpAddress) break;
			strSrv = L"";
		}

		if(strSrv != L"") return (CStringW)INET_NTOA(pAdapt_Info->IpAddress);
	}

	DWORD dwIPD = MZDH_DEFAULT_IP;
	return (CStringW)INET_NTOA(dwIPD);
}


//获取指定进程的ID
DWORD ProcessIDGetted(TCHAR* ProcessName)
{
	DWORD ProcessID = 0x00;

	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		CString strError;
		strError.Format( _T("创建进程快照句柄失败 code = %d"), GetLastError() );
		//WRITE_LOG( LOG_LEVEL_WARN, strError );
		return 0;
	}

	PROCESSENTRY32W pe32;
	ZeroMemory(&pe32, sizeof(PROCESSENTRY32W));
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	BOOL bProcess = Process32FirstW( hProcessSnap, &pe32 );
	while(bProcess)
	{
		//if( _tcscmp(ProcessName, pe32.szExeFile) == 0 )   //stricmp
		if( memcmp(pe32.szExeFile, ProcessName, lstrlen(ProcessName)) == 0 )
		{
			ProcessID = pe32.th32ProcessID;
			break;
		}
		bProcess = Process32NextW( hProcessSnap, &pe32 );
	}

	CloseHandle(hProcessSnap);    

	return ProcessID;
}

CStringW WCHAR2CStrW(wchar_t *wzString)
{
	CStringW strReturn;
	for(DWORD i=0; i < wcslen(wzString); i++)  
		strReturn.AppendChar( wzString[i] );  
	return strReturn;
}

CStringA CStrW2CStrA(const CStringW &cstrSrcW)
{
	return (CStringA)cstrSrcW;
}

DWORD Str_To_IP(const CStringW& strIP)
{ 
	return inet_addr( CStrW2CStrA(strIP) );
}

CStringW IP_To_Str(DWORD IP)
{
	CStringA Ipstr = inet_ntoa( *(in_addr *)&IP );
	return (CStringW)Ipstr;
}

int Handle_Srv_CacheInfo(DWORD dwWksCmd, int iEnumID, DWORD dwSrvIP)
{
	return UI_Cmd_SR(dwWksCmd, iEnumID, 0, dwSrvIP); 
} 

int Get_Wks_WorkedInfo(DWORD dwWksCmd, CStringW &strWksID, int strDiskID)
{
	return UI_Cmd_SR(dwWksCmd, _ttoi(strWksID.GetBuffer()), strDiskID, 0) ; 
}

//int Get_Wks_WorkedInfo(CStringW &strWksID, int strDiskID, DWORD dwIP)
//{
//	DWORD* pBuff = (DWORD*)UI_Get_Cmd_Buf();
//	pBuff[0] = _ttoi(strWksID.GetBuffer());
//	pBuff[1] = strDiskID;
//
//	if ( UI_Cmd_SR(eMZDH_CMD_WKS_INFO, 3*sizeof(DWORD), dwIP) < 0 ) return -1; // Comm Error
//	return 1;
//}

CStringW MZUI_Make_WksName(CStringW& Nums, CStringW& NumberLength, CStringW& ComPre, CStringW& ComPos, CStringW& ComStart)
{
	CStringW LenFmt; LenFmt.Format(L"%d", _ttol(NumberLength)); 
	LenFmt = L"%s%0" + LenFmt + L"d%s";

	int i = _ttol(Nums) + _ttol(ComStart);
	if ( i<0 ) return L"";

	CStringW WksName; WksName.Format(LenFmt, ComPre, i, ComPos);
	return WksName;
}

CStringW MZUI_Make_WksIp(CStringW Nums, CStringW& wksStartIp)
{
	DWORD iNums = _ttol(Nums);
	DWORD iStartIP = Str_To_IP(wksStartIp);

	iNums = _MZDH_IP_MAC::Calc_WksIp(iNums, iStartIP, 0);
	if ( iNums==0 ) return L"";

	return IP_To_Str(iNums);
}

// 删除未使用的菜单
void MZD_Check_Menu() 
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

//删除未使用磁盘
void MZD_Check_Disk() 	
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

//删除未使用服务器
//#define     MZ_Count_WPath   8  //每个服务器支持的回写路径个数
void MZD_Check_Server() 	
{
	for( int iCnt = 0; ; iCnt++ )
	{  
		CStringW strSrvName = MZDUI_GET_SERVER(iCnt, "ServerName");
		if ( *strSrvName == 0 ) break;

		CStringW strDir =  MZDUI_GET_SERVER(strSrvName, "workDir1");
		if ( strDir==L"" ) 
		{ 
			//DBGPRINT("\n" __FUNCTION__ " Delete %ws", strSrvName.GetBuffer());
			MZDUI_DEL_SERVER(strSrvName);
			iCnt--;
		}
	}
}


int Get_Image_Info( CStringW &strDiskDir, DWORD dwIP)
{
	PCHAR pBuff = UI_Get_Cmd_Buf();
	strcpy( pBuff, CStrW2CStrA(strDiskDir).GetBuffer() );
    
	*(int *)(&pBuff[MAX_PATH]) = 0;

//MZDUI_DEL_IMAGE("Ret");    //待完善 
	if ( UI_Cmd_SR(eMZDH_CMD_IMAGE_Info, pBuff, MAX_PATH + sizeof(DWORD), dwIP) < 0 ) return -1; // Comm Error

	CStringW str =  MZDUI_GET_IMAGE(0, "Ret");

	if ( str == L"0" ) return -2; // File Error Or Not Find Need
	if ( str == L"-1" ) return -3; // Find End Error

	return ( str == L"1") ? 0 : 1; // 0: File, 1: Dire
}

CStringW Get_Image_Size(CStringW &strSize)
{
	CStringW str = strSize;

	DWORD sizeMB =  _ttoi(str);
	if ( sizeMB<1024 ) str += L"<MB>";
	else str.Format(_T("%d<GB>"), sizeMB / 1024);

	return str;
}

CStringW Get_Image_Size_new(CStringW &strSize)
{
	CStringW str = strSize;

	DWORD sizeMB = _ttoi(str);
	if ( sizeMB<1024 ) 
	{
		str = L"<" + str + L"MB>";
	}	
	else
	{
		str.Format( _T("<%dGB>"), sizeMB/1024 );
	}

	return str;
}


CStringW Get_Image_Type(CStringW &strType)
{
	CStringW str;
	switch( _ttoi(strType) )
	{
		case MZD_VDD_PHY: str = L"物理盘"; break;
		case MZD_VDD_PARTI: str = L"逻辑盘"; break;
		case MZD_VDD_RAW: str = L"镜像包"; break;
		case MZD_VDD_SPARSE: str = L"稀疏包"; break;
		case MZD_VDD_NORMAL: str = L"普用包"; break;
		case MZD_VDD_IMG:  str = L"MZD专用"; break;
		default: str = L"未知";
	}
	return str;
}

//CStringW Get_Image_Times(char *TimeType)
//{
//	CStringW strTime = MZDUI_GET_IMAGE(0, TimeType);
//	if ( strTime==L"" ) return L"";
//
//	FILETIME ft; 
//  SYSTEMTIME st;
//	swscanf(strTime.GetBuffer(), L"%I64d", &ft);
//	FileTimeToSystemTime(&ft, &st);
//
//	strTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
//	return strTime;
//}


//获取指定镜像的操作进度
int Progress_Rate(CStringW &strSrvName, CStringW &strInfo)
{
	//CStringW strImg, strDiff;
	DWORD SrvIP = UI_Get_Server_IP( strSrvName );

	MZDH_IMG_MERGE Img_Merge = { 0 };
	strInfo = L"";

	//case eMZDH_CMD_IMAGE_Merge: // char* pDst, char* pSrc, char *pOrgImg
	int iMgrRet = UI_Cmd_SR(eMZDH_CMD_IMAGE_Merge, (char *)&Img_Merge, sizeof(BYTE), SrvIP);
	if ( iMgrRet<0 ) return -1; // Cmd_SR Error

	PMZDH_IMG_MERGE pImg_Merge = &Img_Merge;

	iMgrRet = *(PDWORD)(&pImg_Merge->UdoNote); // Ack Is Merge_Rate
	if ( iMgrRet<0 ) return -2; // 任务错误
	if ( iMgrRet ==0 )return 0;  // 没有任务
	if ( iMgrRet>=100) return 100;  //完成

	CStringA strTmp;

	strTmp = pImg_Merge->Src;
	if ( pImg_Merge->UdoNums>0 ) //备份更新
	{
	  strTmp += " -> ";
  	  strTmp += pImg_Merge->Dst;
	}
	else
	{
  	  if ( strTmp!="" ) strTmp += " + ";
	  strTmp += pImg_Merge->UdoDire_Img; // OrgImg
	  strTmp += " -> ";
	  strTmp += pImg_Merge->Dst;

	}

	strInfo = strTmp;
	strTmp.Format(" %d%%", iMgrRet);
	strInfo += strTmp;
	return iMgrRet;
}

//--------------------------------------------------------------------------------------------------
//接口函数
//--------------------------------------------------------------------------------------------------
char* MZUI_INI_GetString(char* SectName, char* keyName, int SectNums, int No_Cost)
{
	return MZDUI_INI_FUNC.GetString(SectName, keyName, SectNums, NULL);
}
char* MZUI_INI_GetString(char* SectName, char* keyName, char* keyValue, char *NodeName)
{
	return MZDUI_INI_FUNC.GetString(SectName, keyName, keyValue, NodeName);
}

CStringW MZUI_INI_GetString(char* SectName, char *keyName, int SectNums, char *NodeName)
{
	return (CStringW)MZDUI_INI_FUNC.GetString(SectName, keyName, SectNums, NodeName);
}

CStringW MZUI_INI_GetString(char* SectName, char* keyName, CStringW& wkeyValue, char *NodeName)
{
	CStringA strTmp = CStrW2CStrA(wkeyValue);
	return (CStringW)MZDUI_INI_FUNC.GetString(SectName, keyName, strTmp.GetBuffer(), NodeName);
}

int MZUI_INI_WriteString(char* SectName, char* keyName, char* keyValue, char *NodeName,  char* NodeVal)
{
	return MZDUI_INI_FUNC.WriteString( SectName, keyName, keyValue, NodeName, NodeVal );
}

int MZUI_INI_WriteString(char* SectName, char* keyName, CStringW& wkeyValue, char *NodeName, CStringW& wNodeVal)
{
	CStringA keyValue = CStrW2CStrA(wkeyValue);
	CStringA NodeVal = CStrW2CStrA(wNodeVal);
	return MZDUI_INI_FUNC.WriteString( SectName, keyName, keyValue.GetBuffer(), NodeName, NodeVal.GetBuffer() );

	//return MZDUI_INI_FUNC.WriteString( SectName, keyName, wkeyValue, NodeName, wNodeVal );
}

void MZUI_INI_Del_Sect_Key(char* SectName, char* keyName, char *keyValue)
{
	MZDUI_INI_FUNC.Del_Sect_Key( SectName, keyName, keyValue);
}
void MZUI_INI_Del_Sect_Key(char* SectName, char* keyName, CStringW& wkeyValue) 
{
	CStringA keyValue = CStrW2CStrA(wkeyValue);
	MZDUI_INI_FUNC.Del_Sect_Key( SectName, keyName, keyValue.GetBuffer());
}

void MZUI_INI_Mov_Sect_Key(char* SectName, char* keyName, CStringW& wkeyValue) //
{
	CStringA keyValue = CStrW2CStrA(wkeyValue);
	MZDUI_INI_FUNC.Mov_Sect_Key( SectName, keyName, keyValue.GetBuffer());
}

char* MZUI_INI_CheckString(char* SectName, char* keyName, char* keyValueSkip, char *NodeName, char* NodeVal)
{
	return MZDUI_INI_FUNC.CheckString(SectName, keyName, keyValueSkip, NodeName, NodeVal);
}

CStringW MZUI_INI_CheckString(char* SectName, char* keyName, CStringW& keyValueSkip, char *NodeName, CStringW& NodeVal)
{
	CStringA skeyValueSkip = CStrW2CStrA(keyValueSkip);
	CStringA sNodeVal = CStrW2CStrA(NodeVal);
	return (CStringW)MZDUI_INI_FUNC.CheckString(SectName, keyName, skeyValueSkip.GetBuffer(), NodeName, sNodeVal.GetBuffer());
}

char*    MZUI_INI_Check_Sect_Key(char* SectName, char* keyName, char* keyValue)
{
	return MZDUI_INI_FUNC.Check_Sect_Key(SectName, keyName, keyValue);
}

CStringW MZUI_INI_Check_Sect_Key(char* SectName, char* keyName, CStringW& wkeyValue)
{
	CStringA keyValue = CStrW2CStrA(wkeyValue);
	return(CStringW)MZDUI_INI_FUNC.Check_Sect_Key(SectName, keyName, keyValue.GetBuffer());
}

//--------------------------------------------------------------------------------------------------
int UI_Load_Ini() // Set: UI_Cmd_SR(eMZDH_CMD_SET_INI, 
{
	return MZDH_CMD_UI::s_Create().Load_Ini();
}

int UI_Updata_Ini() // pGlobalCfg->wksDefPassword Need CryptString(xx, MZD_ENCRYPT, xx)
{
	return MZDH_CMD_UI::s_Create().Updata_Ini();
}

void UI_Start_3801(int Timer)
{
	MZDH_CMD_UI::s_Create().Start_3801(Timer);
}

void UI_Pause_3801()
{
	MZDH_CMD_UI::s_Create().Pause_3801(); 
}

char* UI_Get_Cmd_Buf()
{
	return (char *)MZDH_CMD_UI::s_Create().UI_Get_Buff();
}

void UI_Free_Cmd_Buf()
{
	MZDH_CMD_UI::s_Create().UI_Free_Buff();
}

int UI_Cmd_SR(DWORD CmdID, char *pBuff, int iLen, DWORD dwIP) // eMZDH_CMD_GET_HDID, eMZDH_CMD_CHECK_REG, eMZDH_CMD_GET_IO_SRV
{
	return MZDH_CMD_UI::s_Create().UI_Cmd_SR(CmdID, pBuff, iLen, dwIP);
}

int UI_Cmd_SR(DWORD CmdID, CStringW wStr, DWORD dwIP) // eMZDH_CMD_GET_HDID, eMZDH_CMD_CHECK_REG, eMZDH_CMD_GET_IO_SRV
{ 
	if ( wStr==L"" )
	{ return MZDH_CMD_UI::s_Create().UI_Cmd_SR(CmdID, NULL, 0, dwIP); }

	PCHAR pBuff = UI_Get_Cmd_Buf();
	CStringA cStr = (CStringA)wStr;

	int iLen = cStr.GetLength() + 1; // Inculde End Zero
	memcpy(pBuff, cStr.GetBuffer(), iLen);
	return MZDH_CMD_UI::s_Create().UI_Cmd_SR(CmdID, pBuff, iLen, dwIP);
}

int UI_Cmd_SR(DWORD Cmd_ID, DWORD Idx, DWORD ID, DWORD Ip) 
{
	return MZDH_CMD_UI::s_Create().UI_Cmd_SR(Cmd_ID, Idx, ID, Ip);
}

//--------------------------------------------------------------------------------------------------

int UI_Get_Reg(CStringW& whdid, CStringW& wsmsCode, CStringW& wmzdUser, CStringW& wmzdPass)
// int UI_Get_Reg(WCHAR* whdid, WCHAR* wagentNumber, WCHAR* wpassword, WCHAR* wsmsCode, WCHAR* wmzdUser, WCHAR* wmzdPass)
{
	//return MZDH_CMD_UI::s_Create()->Get_Reg( whdid,  wagentNumber,  wpassword,  wsmsCode,  wmzdUser,  wmzdPass );
	return MZDH_CMD_UI::s_Create().Get_Reg( whdid.GetBuffer(), L"", L"", wsmsCode.GetBuffer(), wmzdUser.GetBuffer(), wmzdPass.GetBuffer() );
}

DWORD UI_Get_Server_IP(CStringW &strSrv)
{
	for ( int i = 0; ; i++ )
	{
		CStringW sName = MZDUI_GET_SERVER(i, "ServerName");
		if ( sName==L"" ) break;
		if ( sName!=strSrv ) continue;
		return Str_To_IP( MZDUI_GET_SERVER(i, "AdaptIp1") );
	}
	return 0;
}
//
//int UI_Cmd_Wks_Close(int Wks_ID, DWORD Ip)
//{
//	return MZDH_CMD_UI::s_Create().Cmd_Wks_Close(Wks_ID, Ip);
//}
//
//int UI_Set_Super_Wks(int Wks_ID, DWORD Ip)
//{
//	return MZDH_CMD_UI::s_Create().Set_Super_Wks(Wks_ID, Ip);
//}
//
int UI_End_Super_Wks(int Wks_ID, DWORD Ip)
{
	return MZDH_CMD_UI::s_Create().End_Super_Wks(Wks_ID);
}

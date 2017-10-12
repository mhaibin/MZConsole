//Ini接口定义类

//#include "stdafx.h"
//#include <tchar.h>

#include <WINSOCK2.H>
#include <windows.h>
#include <WinIoCtl.h>
#include <WinSvc.h>
#include <assert.h>

#include <tchar.h>
#include <atlstr.h>  // Ctring

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef _DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif
//#define DEBUG_MODE 0
#define MZDH_APP_NAME _T("MZDH_UI")

// ====================================
#define MZDH_AS_H

// #define MZDH_IO_IS_DIRE
// 
// #ifdef MZDH_IO_IS_DIRE
// 	#include "..\\..\\mzdh_io\\mzdh_io\\mzdh_io.cpp"
// #else
// 	#include "..\\..\\mzdh_io\\mzdh_io.cpp"
// #endif
#include "..\\..\\mzdh_io\\mzdh_io\\MZDH_IO.h"

// ====================================
#undef MZDUI_GET_INI
#undef MZDUI_SET_INI
#undef MZDUI_CHK_INI
#undef MZDUI_DEL_INI
#undef MZDUI_MOV_INI
#undef MZDUI_CMP_INI

#define MZDUI_GET_INI  MZUI_INI_GetString
#define MZDUI_SET_INI  MZUI_INI_WriteString
#define MZDUI_CHK_INI  MZUI_INI_CheckString
#define MZDUI_DEL_INI  MZUI_INI_Del_Sect_Key
#define MZDUI_MOV_INI  MZUI_INI_Mov_Sect_Key
#define MZDUI_CMP_INI  MZUI_INI_Check_Sect_Key

//--------------------------------------------------------------------------------------------------
//int Get_ImageOwner_Info( CStringW &strDiskDir, DWORD dwIP);

CStringW GetSrvDefaultIP();

void MZD_Del_GroupInfo(); //清除分组信息

DWORD ProcessIDGetted(TCHAR* ProcessName);

CStringW WCHAR2CStrW(wchar_t *wzString);
CStringA CStrW2CStrA(const CStringW &cstrSrcW);

DWORD Str_To_IP(const CStringW& strIP);
CStringW IP_To_Str(DWORD IP);

CStringW MZUI_Make_WksName(CStringW& Nums, CStringW& NumberLength, CStringW& ComPre, CStringW& ComPos, CStringW& ComStart);
CStringW MZUI_Make_WksIp(CStringW Nums, CStringW& wksStartIp);

void MZD_Check_Menu(); // 删除未使用的菜单
void MZD_Check_Disk(); //删除未使用磁盘
void MZD_Check_Server(); //删除未使用服务器

//服务器缓存信息处理：获取、清除
int Handle_Srv_CacheInfo(DWORD dwWksCmd, int iEnumID, DWORD dwSrvIP);   

//获取工作站信息
int Get_Wks_WorkedInfo(DWORD dwWksCmd, CStringW &strWksID, int strDiskID); 

////获取工作站主镜像文件所在的服务器IP、流量
// int Get_Wks_WorkedInfo(CStringW &strWksID, int strDiskID, DWORD dwIP);  

int Get_Image_Info(CStringW &strDiskDir, DWORD dwIP);

CStringW Get_Image_Size_new(CStringW &strSize);
CStringW Get_Image_Size(CStringW &strSize);
CStringW Get_Image_Type(CStringW &strType);
//CStringW Get_Image_Times(char *TimeType);

//CStringW PropertyTimeChecked(CStringW &strSrvName, CStringW &strDestFile);
int Progress_Rate(CStringW &strSrvName, CStringW &strInfo);

//--------------------------------------------------------------------------------------------------
char* MZUI_INI_GetString(char* SectName, char* keyName, int SectNums, int No_Cost);
char* MZUI_INI_GetString(char* SectName, char* keyName, char* keyValue, char *NodeName);

CStringW MZUI_INI_GetString(char* SectName, char *keyName, int SectNums, char *NodeName);
CStringW MZUI_INI_GetString(char* SectName, char* keyName, CStringW& wkeyValue, char *NodeName);

int MZUI_INI_WriteString(char* SectName, char* keyName, char* keyValue, char *NodeName,char* NodeVal); 
int MZUI_INI_WriteString(char* SectName, char* keyName, CStringW& wkeyValue, char *NodeName, CStringW& wNodeVal); 

void MZUI_INI_Del_Sect_Key(char* SectName, char* keyName, CStringW& wkeyValue);
void MZUI_INI_Del_Sect_Key(char* SectName, char* keyName, char *keyValue);

void MZUI_INI_Mov_Sect_Key(char* SectName, char* keyName, CStringW& wkeyValue);

char*    MZUI_INI_CheckString(char* SectName, char* keyName, char* keyValueSkip, char *NodeName, char* NodeVal);
CStringW MZUI_INI_CheckString(char* SectName, char* keyName, CStringW& keyValueSkip, char *NodeName, CStringW& NodeVal);

char*    MZUI_INI_Check_Sect_Key(char* SectName, char* keyName, char* keyValue);
CStringW MZUI_INI_Check_Sect_Key(char* SectName, char* keyName, CStringW& keyValue);
//--------------------------------------------------------------------------------------------------
int UI_Load_Ini();   // Set: UI_Cmd_SR(eMZDH_CMD_SET_INI, 
int UI_Updata_Ini(); // pGlobalCfg->wksDefPassword Need CryptString(xx, MZD_ENCRYPT, xx)

void UI_Start_3801(int Timer);
void UI_Pause_3801();

int UI_Cmd_SR(DWORD CmdID, char *pBuff, int iLen, DWORD dwIP);
int UI_Cmd_SR(DWORD CmdID, CStringW cStr, DWORD dwIP);
int UI_Cmd_SR(DWORD Cmd_ID, DWORD Idx, DWORD ID, DWORD Ip);

//--------------------------------------------------------------------------------------------------
char* UI_Get_Cmd_Buf();
void UI_Free_Cmd_Buf();

//int UI_Get_Reg(WCHAR* whdid, WCHAR* wagentNumber, WCHAR* wpassword, WCHAR* wsmsCode, WCHAR* wmzdUser, WCHAR* wmzdPass);
int UI_Get_Reg(CStringW& whdid, CStringW& wsmsCode, CStringW& wmzdUser, CStringW& wmzdPass);

DWORD UI_Get_Server_IP(CStringW &strSrv);

//int UI_Cmd_Wks_Close(int Wks_ID, DWORD Ip);
//
//int UI_Set_Super_Wks(int Wks_ID, DWORD Ip = 0);
//
int UI_End_Super_Wks(int Wks_ID, DWORD Ip = 0);
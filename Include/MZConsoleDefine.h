#pragma once
#include <atlstr.h>
#include <vector>

using namespace std;
using namespace ATL;

#define	UM_OPEN_SINGLEEDIT_WND				WM_USER+1000	//打开单个编辑框窗口	
#define UM_SINGLEEDIT_MSG					WM_USER+1001	//单个编辑框窗口发送的消息；
#define UM_WORKSTATIONLIST_MENU				WM_USER+1002	//右键工作站列表菜单
#define UM_SWITCHCONFIGTYPE_MSG				WM_USER+1003	//配置窗口配置类型列表切换消息；
#define UW_SHOWWINDOW_MSG					WM_USER+1004	//显示主窗口消息
#define UW_SHOWPREVIEW_WND					WM_USER+1005	//显示预览窗口
#define UW_SHOWDRIVEPREVIEW_WND				WM_USER+1006	//显示驱动预览窗口
#define UW_LOADWORKSTATION_MSG				WM_USER+1007	//加载工作站列表
#define UW_WORKSTATIONGROUPLIST_MENU		WM_USER+1008	//右键工作站分组列表
#define UW_LOADSERVER_MSG					WM_USER+1009	//加载服务器列表
#define WM_LOADDISK_MSG						WM_USER+1010	//加载磁盘控件
#define WM_REGISTERWND_MSG					WM_USER+1011	//弹出注册窗口信息
#define WM_ADDWORKSTATIONERROR_MSG			WM_USER+1012	//添加工作站的时候，错误提示窗口消息
#define UM_ISINBORDER_MSG					WM_USER+1013	//获取主窗口是否是在边框消息；
#define UM_UPDATEWROKSTATION_MSG			WM_USER+1014	//本地工作站数据信息更新，需要刷新工作站列表
#define UM_UPDATESERVER_MSG					WM_USER+1015	//本地服务器数据信息更新，需要刷新服务器列表
#define UM_ISMZDIOSERVICERUN				WM_USER+1016	//MZD服务是否运行


typedef struct _tagWorkstationInfo
{
	_tagWorkstationInfo()
	{
		u8Status = 0;
		u32Num = 0;
		u32Delay = 0;
		u32GroupID = 0;
	}

	BOOL operator==(const _tagWorkstationInfo &other)
	{
		BOOL bRel = TRUE;
		if(u32Num != other.u32Num
			|| u32Delay != other.u32Delay
			|| u32GroupID != other.u32GroupID
			|| strFName != other.strFName
			|| strBName != other.strBName
			|| strNumLen != other.strNumLen
			|| strSize != other.strSize
			|| strDPI != other.strDPI
			|| strName != other.strName
			|| strMac != other.strMac
			|| strMask != other.strMask
			|| strGetway != other.strGetway
			|| strDNSI1 != other.strDNSI1
			|| strDNSI2 != other.strDNSI2
			|| strIP != other.strIP
			|| strMirrorIP != other.strMirrorIP
			|| strMirrorFile != other.strMirrorFile
			|| strReturnWDir != other.strReturnWDir)
		{
			bRel = FALSE;
		}
		return bRel;
	}
	UINT8 u8Status;			//保护状态
	UINT32 u32Num;			//编号
	UINT32 u32Delay;		//菜单延时
	UINT32 u32GroupID;		//分组ID
	CString	strFName;		//前缀
	CString strBName;		//后缀
	CString	strNumLen;		//编号长度
	CString strSize;			//缓存大小
	CString	strDPI;			//分辨率
	CString strName;		//计算机名
	CString strMac;			//Mac地址
	CString	strMask;		//掩码
	CString strGetway;		//网关
	CString	strDNSI1;		//DNSI1
	CString	strDNSI2;		//DNSI2
	CString	strIP;			//工作站IP
	CString strMirrorIP;	//镜像IP
	CString strMirrorFile;	//镜像文件
	CString strReturnWDir;	//回写目录
}WorkstationInfo;

typedef struct _tagServerInfo
{
	_tagServerInfo()
	{
		u32SysDiskSize = 0;
		u32SSDDiskSize = 0;
		u32DataDiskSize = 0;
	}
	UINT32 u32SysDiskSize;
	UINT32 u32DataDiskSize;
	UINT32 u32SSDDiskSize;
	CString strName;
	CString strSqareServer;
	vector<CString> vecRWrite;
}ServerInfo;

typedef struct _tagMenuDiskInfo
{
	CString strDiskName;
	CString strServer;
	CString strPath;
	CString strModel;
}MenuDiskInfo;

typedef struct _tagMzdImgMerge
{
	_tagMzdImgMerge()
	{
		n32Type = 0;
	}
	INT32 n32Type;
	CString strDst;
	CString	strSrc;
	CString	strNote;
}MzdImgMerge;

typedef struct _tagRegsterInfo 
{
	_tagRegsterInfo()
	{
		u32Size = 0;
	}
	UINT32 u32Size;		//注册台数
	CString	strNetBar;	//网吧名称
	CString strDiskNum;	//硬盘序号
	CString strValidity;//有效期
	CString strPass;	//通行证
}RegsterInfo;
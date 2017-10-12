#pragma once
#include <atlstr.h>

using namespace ATL;

//工作站管理列表
typedef struct _tagWorkstationItem
{
	_tagWorkstationItem()
	{
		u8Status = 0;
		u32Num = 0;
		u32Size = 0;
	}
	UINT8 u8Status;			//保护状态
	UINT32 u32Num;			//编号
	INT32 u32Size;			//缓存大小
	CString strName;		//计算机名
	CString strMac;			//Mac地址
	CString	strIP;			//工作站IP
	CString strMirrorIP;	//镜像IP
	CString strMirrorFile;	//镜像文件
	CString strReturnWDir;	//回写目录
	CString strIcon;		//图标
}WorkstationItem;

//服务器管理列表
typedef struct _tagServerItem
{
	_tagServerItem()
	{
		u8Status = 0;
	}
	UINT8	u8Status;		//服务器状态
	CString strName;		//服务器名称
	CString strIP;			//服务器IP
	CString strReturnWDir;	//回写目录
	CString	strBuffPrama;	//缓存参数
	CString strSpareName;	//备用服务器名称
}ServerItem;

//性能类型
typedef struct _tagPropertyItem
{
	_tagPropertyItem()
	{
		u8Type = 0;
		u32Use = 0;
		u32ReadSpeed = 0;
		u32WriteSpeed = 0;
		u32PF = 0;
		u32AllRead = 0;
		u32AllWrite = 0;
		u32Surplus = 0;
	}
	UINT8 u8Type;		//性能类型
	UINT32 u32Use;		//利用率
	UINT32 u32ReadSpeed;//读速率
	UINT32 u32WriteSpeed;//写速率
	UINT32 u32PF;		//PF/队列深度
	UINT32 u32AllRead;	//总读
	UINT32 u32AllWrite;	//总写
	UINT32 u32Surplus;	//剩余
}PropertyItem;

//缓存使用情况
typedef struct _tagStorageUseItem
{
	_tagStorageUseItem()
	{
		u8Type = 0;
		u32Size = 0;
		u32UseSize = 0;
		u32AllRead = 0;
		u32ReadPos = 0;
		u32AllWrite = 0;
		u32WritePos = 0;
	}
	UINT8 u8Type;			//缓存类型
	UINT32 u32Size;			//大小
	UINT32 u32UseSize;		//已经使用
	UINT32 u32AllRead;		//总读
	UINT32 u32ReadPos;		//读命中
	UINT32 u32AllWrite;		//总写
	UINT32 u32WritePos;		//写命中
	CString strName;		//服务器名称
	CString strUseName;		//使用者

}StorageUseItem;

//磁盘信息列表
typedef struct _tagDiskInfoItem
{
	CString strSize;				//磁盘大小
	CString strName;			//服务器名称
	CString	strDiskPath;		//磁盘路径
	CString strDiskType;		//磁盘格式
	CString strModifyTime;		//修改时间
	CString strRemark;			//备注
}DiskInfoItem;

//磁盘属性列表
typedef struct _tagDiskPorpertyItem
{
	_tagDiskPorpertyItem()
	{
		nModel = 0;
	}
	CString strDiskName;
	CString	strServerName;
	CString strDiskPath;
	INT32	nModel;
}DiskPorpertyItem;

//磁盘说明列表
typedef struct _tagDiskInstructionItem
{
	CString	strSize;		//磁盘大小
	CString	strModifyTime;	//修改时间
	CString	strFileDir;
	CString	strRemark;		//备注
}DiskInstructionItem;
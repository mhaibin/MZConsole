#pragma once
#include <atlstr.h>
#include <vector>

using namespace std;
using namespace ATL;

#define	UM_OPEN_SINGLEEDIT_WND				WM_USER+1000	//�򿪵����༭�򴰿�	
#define UM_SINGLEEDIT_MSG					WM_USER+1001	//�����༭�򴰿ڷ��͵���Ϣ��
#define UM_WORKSTATIONLIST_MENU				WM_USER+1002	//�Ҽ�����վ�б�˵�
#define UM_SWITCHCONFIGTYPE_MSG				WM_USER+1003	//���ô������������б��л���Ϣ��
#define UW_SHOWWINDOW_MSG					WM_USER+1004	//��ʾ��������Ϣ
#define UW_SHOWPREVIEW_WND					WM_USER+1005	//��ʾԤ������
#define UW_SHOWDRIVEPREVIEW_WND				WM_USER+1006	//��ʾ����Ԥ������
#define UW_LOADWORKSTATION_MSG				WM_USER+1007	//���ع���վ�б�
#define UW_WORKSTATIONGROUPLIST_MENU		WM_USER+1008	//�Ҽ�����վ�����б�
#define UW_LOADSERVER_MSG					WM_USER+1009	//���ط������б�
#define WM_LOADDISK_MSG						WM_USER+1010	//���ش��̿ؼ�
#define WM_REGISTERWND_MSG					WM_USER+1011	//����ע�ᴰ����Ϣ
#define WM_ADDWORKSTATIONERROR_MSG			WM_USER+1012	//��ӹ���վ��ʱ�򣬴�����ʾ������Ϣ
#define UM_ISINBORDER_MSG					WM_USER+1013	//��ȡ�������Ƿ����ڱ߿���Ϣ��
#define UM_UPDATEWROKSTATION_MSG			WM_USER+1014	//���ع���վ������Ϣ���£���Ҫˢ�¹���վ�б�
#define UM_UPDATESERVER_MSG					WM_USER+1015	//���ط�����������Ϣ���£���Ҫˢ�·������б�
#define UM_ISMZDIOSERVICERUN				WM_USER+1016	//MZD�����Ƿ�����


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
	UINT8 u8Status;			//����״̬
	UINT32 u32Num;			//���
	UINT32 u32Delay;		//�˵���ʱ
	UINT32 u32GroupID;		//����ID
	CString	strFName;		//ǰ׺
	CString strBName;		//��׺
	CString	strNumLen;		//��ų���
	CString strSize;			//�����С
	CString	strDPI;			//�ֱ���
	CString strName;		//�������
	CString strMac;			//Mac��ַ
	CString	strMask;		//����
	CString strGetway;		//����
	CString	strDNSI1;		//DNSI1
	CString	strDNSI2;		//DNSI2
	CString	strIP;			//����վIP
	CString strMirrorIP;	//����IP
	CString strMirrorFile;	//�����ļ�
	CString strReturnWDir;	//��дĿ¼
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
	UINT32 u32Size;		//ע��̨��
	CString	strNetBar;	//��������
	CString strDiskNum;	//Ӳ�����
	CString strValidity;//��Ч��
	CString strPass;	//ͨ��֤
}RegsterInfo;
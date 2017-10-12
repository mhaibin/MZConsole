#pragma once
#include <atlstr.h>

using namespace ATL;

//����վ�����б�
typedef struct _tagWorkstationItem
{
	_tagWorkstationItem()
	{
		u8Status = 0;
		u32Num = 0;
		u32Size = 0;
	}
	UINT8 u8Status;			//����״̬
	UINT32 u32Num;			//���
	INT32 u32Size;			//�����С
	CString strName;		//�������
	CString strMac;			//Mac��ַ
	CString	strIP;			//����վIP
	CString strMirrorIP;	//����IP
	CString strMirrorFile;	//�����ļ�
	CString strReturnWDir;	//��дĿ¼
	CString strIcon;		//ͼ��
}WorkstationItem;

//�����������б�
typedef struct _tagServerItem
{
	_tagServerItem()
	{
		u8Status = 0;
	}
	UINT8	u8Status;		//������״̬
	CString strName;		//����������
	CString strIP;			//������IP
	CString strReturnWDir;	//��дĿ¼
	CString	strBuffPrama;	//�������
	CString strSpareName;	//���÷���������
}ServerItem;

//��������
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
	UINT8 u8Type;		//��������
	UINT32 u32Use;		//������
	UINT32 u32ReadSpeed;//������
	UINT32 u32WriteSpeed;//д����
	UINT32 u32PF;		//PF/�������
	UINT32 u32AllRead;	//�ܶ�
	UINT32 u32AllWrite;	//��д
	UINT32 u32Surplus;	//ʣ��
}PropertyItem;

//����ʹ�����
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
	UINT8 u8Type;			//��������
	UINT32 u32Size;			//��С
	UINT32 u32UseSize;		//�Ѿ�ʹ��
	UINT32 u32AllRead;		//�ܶ�
	UINT32 u32ReadPos;		//������
	UINT32 u32AllWrite;		//��д
	UINT32 u32WritePos;		//д����
	CString strName;		//����������
	CString strUseName;		//ʹ����

}StorageUseItem;

//������Ϣ�б�
typedef struct _tagDiskInfoItem
{
	CString strSize;				//���̴�С
	CString strName;			//����������
	CString	strDiskPath;		//����·��
	CString strDiskType;		//���̸�ʽ
	CString strModifyTime;		//�޸�ʱ��
	CString strRemark;			//��ע
}DiskInfoItem;

//���������б�
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

//����˵���б�
typedef struct _tagDiskInstructionItem
{
	CString	strSize;		//���̴�С
	CString	strModifyTime;	//�޸�ʱ��
	CString	strFileDir;
	CString	strRemark;		//��ע
}DiskInstructionItem;
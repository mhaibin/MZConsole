#pragma once
#include <atlstr.h>
#include <vector>
using namespace std;
using namespace ATL;

struct tagSingleEditBoxInfo
{
	tagSingleEditBoxInfo()
	{
		bSelText = FALSE;
		u32Len = 0;
	}
	BOOL	bSelText;		//�Ƿ�Ҫȫѡ�༭���ı�
	UINT32	u32Len;			//�ַ�������
	CString	strKey;			//��ʾ���ڵ�Key��
	CString strCaption;		//�����ı�
	CString strLableText;	//�༭����������
	CString strInputText;	//�༭���ʼ����
};

struct tagSinleEditMsg
{
	CString	strKey;			//��ʾ���ڵ�Key��
	CString strContent;		//�༭������ݣ�
};
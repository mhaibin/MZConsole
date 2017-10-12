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
	BOOL	bSelText;		//是否要全选编辑框文本
	UINT32	u32Len;			//字符个数；
	CString	strKey;			//提示窗口的Key；
	CString strCaption;		//标题文本
	CString strLableText;	//编辑框内容名称
	CString strInputText;	//编辑框初始内容
};

struct tagSinleEditMsg
{
	CString	strKey;			//提示窗口的Key；
	CString strContent;		//编辑框的类容；
};
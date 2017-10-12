#pragma once

typedef struct CHILDLABEL 
{ 
	CHILDLABEL()
	{
		hIcon = NULL;
		hWnd = NULL;
		dwCmd = 0;
		ZeroMemory(wsKey, 20 * sizeof(WCHAR));
		ZeroMemory(wsLabel, 20 * sizeof(WCHAR));
		ZeroMemory(wsUnused, 40 * sizeof(WCHAR));
	}
	CHILDLABEL(const CHILDLABEL &other)
	{
		hIcon = other.hIcon;
		hWnd = other.hWnd;
		dwCmd = other.dwCmd;
		_tcscpy(wsKey, other.wsKey);
		_tcscpy(wsLabel, other.wsLabel);
		_tcscpy(wsUnused, other.wsUnused);
	}
	CHILDLABEL &operator=(const CHILDLABEL &other)
	{
		if(&other != this)
		{
			hIcon = other.hIcon;
			hWnd = other.hWnd;
			dwCmd = other.dwCmd;
			_tcscpy(wsKey, other.wsKey);
			_tcscpy(wsLabel, other.wsLabel);
			_tcscpy(wsUnused, other.wsUnused);
		}
		return *this;
	}
	HICON hIcon; // 窗口对应的图标 
	HWND hWnd;   // 窗口句柄 
	DWORD dwCmd;  // 命令ID 
	WCHAR wsKey[20]; // 窗口要创建到哪个KEY下 
	WCHAR wsLabel[20]; // 标签文本 
	WCHAR wsUnused[40]; // 未用 
}tagCHILDLABEL; 

typedef struct MZDLABEL
{
	MZDLABEL()
	{
		dwCount=10;
	}
	DWORD dwCount;
	CHILDLABEL Child[10];
}tagMZDLABEL;

typedef void*	(_stdcall* _btuiInitialize)();
typedef void*	(_stdcall* _btuiSetParam)(DWORD,WCHAR*,BOOL);
typedef void*	(_stdcall* _btuiNotify)(UINT,WPARAM,LPARAM);
typedef void*	(_stdcall* _btuiCreate)(HWND,int,int,int,int,MZDLABEL*,int);
typedef void*	(_stdcall* _btuiCreateChild)(HWND,int,int,int,int,MZDLABEL*,int);
typedef void*	(_stdcall* _btuiSize)(CHILDLABEL*,int,int,int,int);
typedef void*	(_stdcall* _btuiShow)(CHILDLABEL*,int);
typedef void*	(_stdcall* _btuiClose)(CHILDLABEL*);
typedef void*	(_stdcall* _btuiCloseMain)(CHILDLABEL*);
typedef void*	(_stdcall* _btuiUninitialize)();
typedef void*	(_stdcall* _btuiCloseChild)(CHILDLABEL*);

typedef BOOL	(_stdcall* _btcInitConnect)(wchar_t* pwszIP, wchar_t* pwszPassword);	//初始化设置连接IP和密码
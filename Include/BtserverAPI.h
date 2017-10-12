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
	HICON hIcon; // ���ڶ�Ӧ��ͼ�� 
	HWND hWnd;   // ���ھ�� 
	DWORD dwCmd;  // ����ID 
	WCHAR wsKey[20]; // ����Ҫ�������ĸ�KEY�� 
	WCHAR wsLabel[20]; // ��ǩ�ı� 
	WCHAR wsUnused[40]; // δ�� 
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

typedef BOOL	(_stdcall* _btcInitConnect)(wchar_t* pwszIP, wchar_t* pwszPassword);	//��ʼ����������IP������
#pragma once


namespace Aux
{
	namespace MZDIO
	{
		//����mzd����
		BOOL ActiveMzdServer();
		//��ʼ��mzd����
		BOOL InitMzdIOData();
// 		//��ȡ��ʼIP
// 		CString GetSevDefIP();
// 		//��ȡ����
// 		CString GetGetway();
// 		//��ȡDNSI1
// 		CString GetDNSI1();
// 		//��ȡDNSI2
// 		CString GetDNSI2();
// 		//��ȡ����
// 		CString GetMask();
	}
	namespace Server
	{
		void Exit();
	}
}
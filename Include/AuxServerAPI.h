#pragma once


namespace Aux
{
	namespace MZDIO
	{
		//启动mzd服务
		BOOL ActiveMzdServer();
		//初始化mzd数据
		BOOL InitMzdIOData();
// 		//获取起始IP
// 		CString GetSevDefIP();
// 		//获取网关
// 		CString GetGetway();
// 		//获取DNSI1
// 		CString GetDNSI1();
// 		//获取DNSI2
// 		CString GetDNSI2();
// 		//获取掩码
// 		CString GetMask();
	}
	namespace Server
	{
		void Exit();
	}
}
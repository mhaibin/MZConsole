#include "stdafx.h"
#include "AuxServerAPI.h"
#include "Singleton.h"
#include "UtilLog.h"
#include "mzdio/MzdIOMgr.h"
#include "DiskInfoMgr.h"
#include "WorkstationInfoMgr.h"

BOOL Aux::MZDIO::ActiveMzdServer()
{
	return TRUE;
}
BOOL Aux::MZDIO::InitMzdIOData()
{
	Singleton<CMzdIOMgr>::Instance().Init();
	BOOL bOk = Singleton<CMzdIOMgr>::Instance().IsConnect();
	if(FALSE == bOk)
	{
		Util::Log::Warn(_T("Server"), _T("[warnning]MZD������û������\r\n"));
		return FALSE;
	}
	bOk = Singleton<CMzdIOMgr>::Instance().SendDownLoadCmd();
	if(FALSE == bOk)
	{
		Util::Log::Error(_T("Server"), _T("[error]��ʼ��ʱ������ͬ����������ʧ��\r\n"));
		return FALSE;
	}
	Singleton<CMzdIOMgr>::Instance().WorkstationLoadData();
	Singleton<CMzdIOMgr>::Instance().ServerLoadData();
	Singleton<CMzdIOMgr>::Instance().DiskLoadData();
	Singleton<CMzdIOMgr>::Instance().DiskPathLoadData();
	Singleton<CMzdIOMgr>::Instance().WorkstationGroupLoadData();
	return TRUE;
}
void Aux::Server::Exit()
{
	Singleton<CMzdIOMgr>::UnInstance();
	Singleton<CDiskInfoMgr>::UnInstance();
	Singleton<CWorkstationInfoMgr>::UnInstance();
}
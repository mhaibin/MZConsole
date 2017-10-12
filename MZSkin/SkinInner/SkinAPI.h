#pragma once

#include <event.h>
#include <UIlib.h>

using namespace DuiLib;

namespace event{
	//�Ƴ����̵�֪ͨ
	common_signal0 &ExitProcessEvn();
	//�򿪹���վ�б�ҳ��
	common_signal0 &OpenWorkstationPage();
	//�򿪷������б�ҳ��
	common_signal0 &OpenServerPage();
	//�򿪴��̹���ҳ��
	common_signal0 &OpenDiskMgrPage();
	//����Դ����ҳ��
	common_signal0 &OpenResourceUpdatePage();
	//��ϵͳ����ҳ��
	common_signal0 &OpenMonitorPage();
	//�򿪲�������ҳ��
	common_signal0 &OpenSetPage();
	//����վ״̬����
	common_signal0 &UpdateStatusBar();
}

namespace Popup
{
	//����ӹ���վ����
	void OpenAddWorkstationWnd(HWND hWnd, UINT32 u32Flag);
	//���޸Ĺ���վ����
	void OpenModifyWorkstationWnd();
	//����������������
	void OpenAddServerWnd(HWND hWnd, CString strSvrName);
	//�򿪴���ά�����ô���
	void OpenDiskInstructionWnd(UINT8 type);
	//��ע����֤����
	void OpenRegisterWnd();
}
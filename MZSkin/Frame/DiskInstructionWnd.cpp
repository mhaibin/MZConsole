#include "StdAfx.h"
#include "Singleton.h"
#include "UtilLog.h"
#include "DiskInstructionWnd.h"
#include <ContactList\ConfigContactList.h>
#include "..\Global\Control\EditCombUI.h"
#include <SkinInner/SkinAPI.h>
#include "../Global/Common/Global.h"
#include "..\Server\DiskInfoMgr.h"
#include "..\Server\mzdio\MzdIOMgr.h"
#include "UtilString.h"
#include "MZConsoleDefine.h"
#include "..\SharedUI\Interface\SharedBase.h"

#define PROGRESSTIMTER 1000

namespace DuiLib{
	

CDiskInstructionWnd::CDiskInstructionWnd()
{
	m_dwServerIP = 0;
	m_u32Size = 0;
}

CDiskInstructionWnd::~CDiskInstructionWnd(void)
{

}

CDuiString CDiskInstructionWnd::GetSkinFolder()
{
	return _T("Resource");
}

UILIB_RESOURCETYPE CDiskInstructionWnd::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString CDiskInstructionWnd::GetSkinFile()
{
	return _T("xml\\DiskInstructionWnd.xml");
}

LPCTSTR CDiskInstructionWnd::GetWindowClassName(void) const
{
	return _T("DiskInstructionWnd");
}

void CDiskInstructionWnd::InitWindow()
{
	CLabelUI *pLabTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title_name")));
	CLabelUI *pLabMPath = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_mpath")));
	CEditUI *pEdtMPath = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_mpath")));
	CLabelUI *pLabText = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_text")));
	CButtonUI *pBtnCreate = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_create")));
	CDiskInstructionContactList *pList= static_cast<CDiskInstructionContactList*>(m_PaintManager.FindControl(_T("lt_diskinstrution")));

	CString strTitle;
	if(0 == m_u8Type)//磁盘更新
	{
		strTitle = _T("磁盘更新");
		pLabMPath->SetText(_T("镜像路径"));
		pLabText->SetText(_T("UPD备注"));
		pBtnCreate->SetText(_T("磁盘更新"));
		vector<DiskInfoItem> vecItem;
		Singleton<CDiskInfoMgr>::Instance().GetCurSelDiskInfo(vecItem);
		m_strSrcPath = vecItem[0].strDiskPath;
		m_strServerName = vecItem[0].strName;
		m_dwServerIP = Singleton<CMzdIOMgr>::Instance().IPStrToInt(m_strServerName);
		m_strImgPath = Singleton<CMzdIOMgr>::Instance().GetImgPath(vecItem[0].strDiskPath);
		pEdtMPath->SetText(m_strImgPath);
	}
	else if(1 == m_u8Type)//生成新磁盘
	{
		strTitle = _T("生成新磁盘");
		pLabMPath->SetText(_T("目标路径"));
		pLabText->SetText(_T("UPD备注"));
		pBtnCreate->SetText(_T("复制磁盘"));
		vector<DiskInfoItem> vecItem;
		Singleton<CDiskInfoMgr>::Instance().GetCurSelDiskInfo(vecItem);
		m_strServerName = vecItem[0].strName;
		m_dwServerIP = Singleton<CMzdIOMgr>::Instance().IPStrToInt(m_strServerName);
		m_strSrcPath = vecItem[0].strDiskPath;
		m_strImgPath = Singleton<CMzdIOMgr>::Instance().GetImgPath(vecItem[0].strDiskPath);
		pEdtMPath->SetText(m_strImgPath);
	}
	else if(2 == m_u8Type)//创建新磁盘
	{
		strTitle = _T("创建新磁盘");
		pLabMPath->SetText(_T("镜像路径"));
		pLabText->SetText(_T("大小<GB>"));
		pBtnCreate->SetText(_T("新增"));
		vector<DiskInfoItem> vecItem;
		Singleton<CDiskInfoMgr>::Instance().GetCurSelDiskInfo(vecItem);
		if(0 < vecItem.size())
		{
			m_strServerName = vecItem[0].strName;
			m_dwServerIP = Singleton<CMzdIOMgr>::Instance().IPStrToInt(m_strServerName);
			m_strSrcPath = vecItem[0].strDiskPath;
			m_strSrcPath = m_strSrcPath.Left(3);
			pEdtMPath->SetText(m_strSrcPath);
		}
	}
	else if(3 == m_u8Type)//磁盘扩容
	{
		strTitle = _T("磁盘扩容");
		pLabMPath->SetText(_T("镜像路径"));
		pLabText->SetText(_T("大小<GB>"));
		pBtnCreate->SetText(_T("扩容"));
		vector<DiskInfoItem> vecItem;
		Singleton<CDiskInfoMgr>::Instance().GetCurSelDiskInfo(vecItem);
		m_u32Size = _ttoi(vecItem[0].strSize);
		m_strServerName = vecItem[0].strName;
		m_dwServerIP = Singleton<CMzdIOMgr>::Instance().IPStrToInt(m_strServerName);
		m_strSrcPath = vecItem[0].strDiskPath;
		INT32 nSize = vecItem.size();
		for (INT32 nIndex=0; nIndex<nSize; nIndex++)
		{
			DiskInstructionItem item;
			item.strFileDir = vecItem[nIndex].strDiskPath;
			item.strRemark = vecItem[nIndex].strRemark;
			item.strSize = vecItem[nIndex].strSize;
			item.strModifyTime = vecItem[nIndex].strModifyTime;
			pList->InsertItem(item.strFileDir, item);
		}
		pEdtMPath->SetText(vecItem[0].strDiskPath);
	}
	pLabTitle->SetText(strTitle);

	event::ExitProcessEvn().connect(this, &CDiskInstructionWnd::ExitProcessEvn);
}

CControlUI* CDiskInstructionWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("DiskInstructionContactList")) == 0)
	{
		return new CDiskInstructionContactList();
	}
	return NULL;
}

LRESULT CDiskInstructionWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}
LRESULT CDiskInstructionWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	event::ExitProcessEvn().disconnect(this);
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CDiskInstructionWnd::OnTimer(WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if(PROGRESSTIMTER == wParam)
	{
		INT32 nRel = Singleton<CMzdIOMgr>::Instance().GetProgressPos(m_strServerName);
		CString strProgressText;
		if(0 < nRel && 100 > nRel)
		{
			CProgressUI *pProgress= static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("pro_pos")));
			strProgressText.Format(_T("%d%"), nRel);
			pProgress->SetText(strProgressText);
			pProgress->SetValue(nRel);
		}
		if(nRel < 0)
		{
			::KillTimer(m_hWnd, PROGRESSTIMTER);
			Util::Log::Error(_T("MZSKin"), _T("[error]磁盘操作获取进度出错<error=%d, Type=%u>\r\n"),nRel, m_u8Type);
		}
		if(nRel >= 100)
		{
			CProgressUI *pProgress= static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("pro_pos")));
			strProgressText.Format(_T("%d%"), nRel);
			pProgress->SetText(strProgressText);
			pProgress->SetValue(100);
			::KillTimer(m_hWnd, PROGRESSTIMTER);
			Util::Log::Info(_T("MZSKin"), _T("[info]磁盘操作进度完成<Type=%u>\r\n"),m_u8Type);
		}
	}
    return 0;
}
void CDiskInstructionWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString	strName = msg.pSender->GetName();
		if(0 == strName.Compare(_T("closebtn")))
		{
			Close(IDCANCEL);
		}
		else if(0 == strName.Compare(_T("minbtn")))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		if(0 == strName.Compare(_T("btn_create")))
		{
			OnClickCreateBtn();
		}
	}
}
void CDiskInstructionWnd::OnClickCreateBtn()
{
	MzdImgMerge Img_Merge;
	INT32 nRel = 0;
	if(0 == m_u8Type)//磁盘更新
	{
		Img_Merge.strDst = m_strImgPath;
		Img_Merge.strSrc = m_strSrcPath;
		Img_Merge.n32Type = 0;    //磁盘更新 undoNums = 0
		CEditUI *pEdtContent = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_content")));
		CString strContent = pEdtContent->GetText();
		Img_Merge.strNote = strContent;
		::SetTimer(m_hWnd, PROGRESSTIMTER, 1000, 0);
	}
	else if(1 == m_u8Type)
	{
		Img_Merge.strDst = m_strImgPath;
		Img_Merge.strSrc = m_strSrcPath;
		Img_Merge.n32Type = -2;   //生成新磁盘 undoNums = -2
		CEditUI *pEdtContent = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_content")));
		CString strContent = pEdtContent->GetText();
		Img_Merge.strNote = strContent;
		::SetTimer(m_hWnd, PROGRESSTIMTER, 1000, 0);
	}
	else if(2 == m_u8Type
		|| 3 == m_u8Type)
	{
		PCHAR pBuff = (PCHAR)Singleton<CMzdIOMgr>::Instance().GetMzdCmdBuff();
		CString strPath = m_strSrcPath + _T(".img");
		CStringA strAPath;
		Util::String::W_2_Utf8(strPath.GetString(), strAPath);
		strcpy(pBuff, strAPath.GetString());
		CEditUI *pEdtContent = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_content")));
		CString strContent = pEdtContent->GetText();
		DWORD dwSizeTmp = _ttol(strContent);
		if(3 == m_u8Type && dwSizeTmp <= m_u32Size)
		{
			SIZE size;
			size.cx = 50;
			size.cy = 40;
			box::ShowWindow(m_PaintManager.GetPaintWindow(), _T("提示"), _T("请输入大于当前磁盘容量的数值"), NULL, _T("确定"), _T(""), _T("material//general//warning.png"), size);
			return;
		}
		if( dwSizeTmp > 2000 ) dwSizeTmp = 2000;
		*(PDWORD)&pBuff[MAX_PATH] = dwSizeTmp * 1024; 
		nRel = Singleton<CMzdIOMgr>::Instance().SendUICmdSR(17666, (UINT8 *)pBuff, MAX_PATH + sizeof(DWORD), m_dwServerIP, 0);
		::SetTimer(m_hWnd, PROGRESSTIMTER, 1000, 0);
		Util::Log::Info(_T("MZSKin"), _T("[info]磁盘操作执行结果1<Type=%u, Rel=%d>\r\n"),m_u8Type, nRel);
		return;
	}
	nRel = Singleton<CMzdIOMgr>::Instance().SendUICmdSR(17666, (UINT8 *)&Img_Merge, sizeof(MzdImgMerge), m_dwServerIP, 1);
	Util::Log::Info(_T("MZSKin"), _T("[info]磁盘操作执行结果2<Type=%u, Rel=%d>\r\n"),m_u8Type, nRel);
}
void CDiskInstructionWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
void CDiskInstructionWnd::ExitProcessEvn()
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
}
}
namespace Popup
{
	void OpenDiskInstructionWnd(UINT8 type)
	{
		CString strWndName;
		if(0 == type)//磁盘更新
		{
			strWndName = _T("磁盘更新");
		}
		else if(1 == type)//生成新磁盘
		{
			strWndName = _T("生成新磁盘");
		}
		else if(2 == type)//创建新磁盘
		{
			strWndName = _T("创建新磁盘");
		}
		else if(3 == type)//磁盘扩容
		{
			strWndName = _T("磁盘扩容");
		}
		static HWND hDiskInstructionWnd = NULL;	
		if (!::IsWindow(hDiskInstructionWnd))
		{
			CDiskInstructionWnd * pWnd = new CDiskInstructionWnd();
			pWnd->SetType(type);
			pWnd->Create(::GetDesktopWindow(), strWndName, WS_POPUPWINDOW, 0, 0, 0, 0, 0);
			pWnd->CenterWindow();
			pWnd->ShowWindow(true, false);
			hDiskInstructionWnd = pWnd->GetHWND();
		}

		global::FrontWindow(hDiskInstructionWnd);
	}
}
// PrintMessageManager.cpp

#include "stdafx.h"
#include "PrintMessageManager.h"

CListCtrl * PrintMessageManager::m_listMessages = NULL;

PrintMessageManager::PrintMessageManager()
{

}

void PrintMessageManager::AddMessage(CString &info, CString &message)
{
	if (m_listMessages != NULL)
	{
		int nItem = m_listMessages->GetItemCount();
		m_listMessages->InsertItem(nItem, info);
		m_listMessages->SetItemText(nItem, 1, message);
		CString timeStamp;
		CTime mytime = CTime::GetCurrentTime();
		timeStamp.Format(_T("%d %d %d H %d %d %d"), mytime.GetYear(), mytime.GetMonth(), mytime.GetDay(), mytime.GetHour(), mytime.GetMinute(), mytime.GetSecond());
		m_listMessages->SetItemText(nItem, 2, timeStamp);
	}
}
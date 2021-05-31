// PrintMessageManager.h

#ifndef __PrintMessageManager_h__
#define __PrintMessageManager_h__

class PrintMessageManager
{
public:
	PrintMessageManager();

	static CListCtrl *m_listMessages;

	static void AddMessage(CString &info, CString &message);
};

#endif
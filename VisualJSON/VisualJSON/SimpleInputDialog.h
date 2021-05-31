// SimpleInputDialog.h

#ifndef __SimpleInputDialog_h__
#define __SimpleInputDialog_h__

#include "resource.h"

class SimpleInputDialog : public CDialogEx
{
	// Construction
public:
	SimpleInputDialog(const CString &keyStr , const CString &valStr , CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_DIALOG_SIMPLEINPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
public:
	CString m_keyStr;
	CString m_valStr;
};

#endif
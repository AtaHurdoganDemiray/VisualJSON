// SimpleInputDialog.cpp

#include "stdafx.h"
#include "SimpleInputDialog.h"

SimpleInputDialog::SimpleInputDialog(const CString &keyStr, const CString &valStr, CWnd* pParent)	// standard constructor
	:CDialogEx(SimpleInputDialog::IDD, pParent), m_keyStr(keyStr), m_valStr(valStr)
{

}

void SimpleInputDialog::DoDataExchange(CDataExchange* pDX)	// DDX/DDV support
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTVALUE, m_valStr);
}

BEGIN_MESSAGE_MAP(SimpleInputDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, OnBnClickedOK)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL SimpleInputDialog::OnInitDialog()
{
	try
	{
		BOOL ck = CDialogEx::OnInitDialog();
		CWnd *wndptr = GetDlgItem(IDC_STATIC_INPUTKEY);
		if (wndptr != NULL)
		{
			wndptr->SetWindowText(m_keyStr);
		}
		return ck;
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("SimpleInputDialog::OnInitDialog exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
		return FALSE;
	}
}

void SimpleInputDialog::OnBnClickedOK()
{
	try
	{
		CDialogEx::OnOK();

	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("SimpleInputDialog::OnBnClickedOK exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

void SimpleInputDialog::OnBnClickedCancel()
{
	try
	{
		CDialogEx::OnCancel();

	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("SimpleInputDialog::OnBnClickedCancel exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}



